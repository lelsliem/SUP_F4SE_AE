// SUP F4SE — in-process minidump capture (see include/sup/util/CrashDump.h).

#include "sup/util/CrashDump.h"

#include <windows.h>
#include <dbghelp.h>

#include <cstdio>
#include <cwchar>

namespace
{
	// Resolved once at install time — LoadLibrary/GetProcAddress inside a dying
	// process is not safe, so the handler itself does no loader work.
	using MiniDumpWriteDumpFn = BOOL(WINAPI*)(HANDLE, DWORD, HANDLE, MINIDUMP_TYPE,
		PMINIDUMP_EXCEPTION_INFORMATION, PMINIDUMP_USER_STREAM_INFORMATION,
		PMINIDUMP_CALLBACK_INFORMATION);

	MiniDumpWriteDumpFn g_MiniDumpWriteDump = nullptr;

	bool IsFatal(const unsigned long a_code) noexcept
	{
		switch (a_code) {
		case EXCEPTION_ACCESS_VIOLATION:      // 0xC0000005
		case EXCEPTION_IN_PAGE_ERROR:         // 0xC0000006
		case EXCEPTION_ILLEGAL_INSTRUCTION:   // 0xC000001D
		case EXCEPTION_INT_DIVIDE_BY_ZERO:    // 0xC0000094
		case EXCEPTION_INT_OVERFLOW:          // 0xC0000095
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: // 0xC000008C
		case EXCEPTION_STACK_OVERFLOW:        // 0xC00000FD
		case 0xC0000409:                      // STATUS_STACK_BUFFER_OVERRUN (fast fail)
			return true;
		default:
			return false;
		}
	}

	// Fallout 4 1.11.221 has a known vanilla null-deref: at Fallout4.exe+0x4CAF53
	// the code does `mov rdi,[rax+0x50]`, and when rax is null (which happens on
	// shutdown and when another window grabs focus during launch) it faults
	// reading address 0x50. It reproduces with zero mods and is unrelated to SUP,
	// so we skip the dump instead of littering SUP_Crash_*.dmp on clean sessions.
	// Matched by module + RVA + "read from 0x50" so it is robust to ASLR (the
	// faulting module base differs from run to run; the RVA does not).
	bool IsBenignVanillaCrash(const EXCEPTION_RECORD* a_rec, const void* a_addr) noexcept
	{
		if (a_rec->ExceptionCode != EXCEPTION_ACCESS_VIOLATION ||
			a_rec->NumberParameters < 2 ||
			a_rec->ExceptionInformation[0] != 0 ||      // read, not write
			a_rec->ExceptionInformation[1] != 0x50) {  // null + 0x50
			return false;
		}

		HMODULE mod = nullptr;
		if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
				GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
				static_cast<LPCWSTR>(a_addr), &mod) || !mod) {
			return false;
		}

		wchar_t full[MAX_PATH] = {};
		if (!GetModuleFileNameW(mod, full, MAX_PATH))
			return false;
		const wchar_t* name = wcsrchr(full, L'\\');
		name = name ? name + 1 : full;
		if (_wcsicmp(name, L"Fallout4.exe") != 0)
			return false;

		const auto rva = reinterpret_cast<std::uintptr_t>(a_addr) -
			reinterpret_cast<std::uintptr_t>(mod);
		return rva == 0x4CAF53;
	}

	// ENBHelperF4's exported getters are SEH-guarded on the write side: ENB's
	// d3d11 proxy calls them with a mismatched prototype and passes garbage output
	// pointers (observed value: -1), and each guarded write swallows the fault.
	// Those guards live in another module, so sup::crash::g_SehGuardDepth (a
	// per-module thread-local) can't see them, and the VEH would dump a false
	// alarm on every ENB call. Skip dumps when the fault is an access violation
	// inside ENBHelperF4.dll whose target is not a plausible user pointer — that
	// is the handled-fault signature, not a crash.
	bool IsEnbHelperHandledFault(const EXCEPTION_RECORD* a_rec, const void* a_addr) noexcept
	{
		if (a_rec->ExceptionCode != EXCEPTION_ACCESS_VIOLATION || a_rec->NumberParameters < 2) {
			return false;
		}

		const std::uintptr_t target = a_rec->ExceptionInformation[1];
		const bool implausible = target < 0x10000 || target >= 0x0000800000000000ull;
		if (!implausible) {
			return false;
		}

		HMODULE mod = nullptr;
		if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
				GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
				static_cast<LPCWSTR>(a_addr), &mod) ||
			!mod) {
			return false;
		}

		wchar_t full[MAX_PATH] = {};
		if (!GetModuleFileNameW(mod, full, MAX_PATH))
			return false;
		const wchar_t* name = wcsrchr(full, L'\\');
		name = name ? name + 1 : full;
		return _wcsicmp(name, L"ENBHelperF4.dll") == 0;
	}

	// Name of the module that owns a faulting address, or L"?" if it can't be
	// resolved. Best-effort — never fatal if the loader is already damaged.
	void FaultingModuleName(const void* a_addr, wchar_t (&a_out)[MAX_PATH]) noexcept
	{
		HMODULE mod = nullptr;
		if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
				GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
			static_cast<LPCWSTR>(a_addr), &mod) ||
			!mod) {
			wcscpy_s(a_out, L"?");
			return;
		}

		wchar_t full[MAX_PATH] = {};
		if (!GetModuleFileNameW(mod, full, MAX_PATH)) {
			wcscpy_s(a_out, L"?");
			return;
		}

		if (const wchar_t* p = wcsrchr(full, L'\\')) {
			wcscpy_s(a_out, p + 1);
		} else {
			wcscpy_s(a_out, full);
		}
	}

	// Raw fallback trace: the first few stack qwords. The real dumps we've seen
	// can carry a corrupted exception context (RSP=0, garbage registers — an
	// exception-during-dispatch double fault), which makes a normal unwind
	// impossible. A flat sample of stack memory at the exception still shows
	// return addresses we can map to modules afterwards. Guarded so a bad RSP
	// can't recurse into the handler.
	void AppendRawStack(HANDLE a_info, const void* a_sp) noexcept
	{
		__try {
			if (!a_sp)
				return;

			char buf[512] = {};
			int n = _snprintf_s(buf, _TRUNCATE, "Raw stack:\n");
			if (n > 0) {
				DWORD written = 0;
				WriteFile(a_info, buf, static_cast<DWORD>(n), &written, nullptr);
			}

			const auto* sp = static_cast<const std::uintptr_t*>(a_sp);
			for (int i = 0; i < 24; i++) {
				n = _snprintf_s(buf, _TRUNCATE, "  [%02d] %p\n", i,
					reinterpret_cast<const void*>(sp[i]));
				if (n <= 0)
					break;
				DWORD written = 0;
				WriteFile(a_info, buf, static_cast<DWORD>(n), &written, nullptr);
			}
		} __except (EXCEPTION_EXECUTE_HANDLER) {
			// unreachable in practice (WriteFile doesn't fault) — swallow anything.
		}
	}

	LONG WINAPI Handler(EXCEPTION_POINTERS* a_exc) noexcept
	{
		// Never recurse: one dump per process, and only for fatal codes. Handled
		// exceptions (breakpoints, invalid handles, ...) pass straight through.
		static volatile LONG s_dumped = 0;

		if (!a_exc || !a_exc->ExceptionRecord || !IsFatal(a_exc->ExceptionRecord->ExceptionCode))
			return EXCEPTION_CONTINUE_SEARCH;

		if (IsBenignVanillaCrash(a_exc->ExceptionRecord, a_exc->ExceptionRecord->ExceptionAddress))
			return EXCEPTION_CONTINUE_SEARCH;

		// Handled faults inside another plugin's SEH guards (ENBHelperF4): our
		// thread-local can't see cross-module guards, so match the signature.
		if (IsEnbHelperHandledFault(a_exc->ExceptionRecord, a_exc->ExceptionRecord->ExceptionAddress))
			return EXCEPTION_CONTINUE_SEARCH;

		// The fault is inside an SEH-guarded region (fn_IsSnappedConnectionLoop,
		// GetHavokWorld, SnapQueryGuarded). Our __except will swallow it, so a dump
		// here is a false alarm — the VEH runs first-chance, before __except does.
		if (sup::crash::g_SehGuardDepth > 0)
			return EXCEPTION_CONTINUE_SEARCH;

		if (InterlockedExchange(&s_dumped, 1) != 0)
			return EXCEPTION_CONTINUE_SEARCH;

		if (!g_MiniDumpWriteDump)
			return EXCEPTION_CONTINUE_SEARCH;

		const auto* rec = a_exc->ExceptionRecord;
		const unsigned long code = rec->ExceptionCode;
		const void* const addr = rec->ExceptionAddress;

		// Documents\\My Games\\Fallout4\\F4SE\\SUP_Crash_<pid>.{txt,dmp} — the same
		// folder CommonLibF4 writes SUP_F4SE.log to. The .txt companion preserves
		// the ORIGINAL crash info before the dump write: MiniDumpWriteDump reads the
		// whole process and can itself fault on a corrupted one, and the second
		// exception would otherwise mask the real cause (which is exactly what the
		// 2026-08-14 20:05 dump did — its stack was the dump writer, not the crash).
		wchar_t profile[MAX_PATH] = {};
		if (!GetEnvironmentVariableW(L"USERPROFILE", profile, MAX_PATH))
			return EXCEPTION_CONTINUE_SEARCH;

		wchar_t base[MAX_PATH] = {};
		swprintf_s(base, L"%s\\Documents\\My Games\\Fallout4\\F4SE\\SUP_Crash_%lu",
			profile, GetCurrentProcessId());

		wchar_t modName[MAX_PATH] = {};
		FaultingModuleName(addr, modName);

		wchar_t infoPath[MAX_PATH] = {};
		swprintf_s(infoPath, L"%s.txt", base);
		const HANDLE info = CreateFileW(infoPath, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, nullptr);
		const bool haveInfo = info != INVALID_HANDLE_VALUE;

		if (haveInfo) {
			char buf[512] = {};
			int n = 0;
			if (rec->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && rec->NumberParameters >= 2) {
				n = _snprintf_s(buf, _TRUNCATE,
					"SUP F4SE crash @ 0x%p (%ls)\n"
					"Exception code 0x%08lX (%s, address 0x%p)\n",
					addr, modName, code,
					rec->ExceptionInformation[0] ? "write" : "read",
					reinterpret_cast<const void*>(rec->ExceptionInformation[1]));
			} else {
				n = _snprintf_s(buf, _TRUNCATE,
					"SUP F4SE crash @ 0x%p (%ls)\n"
					"Exception code 0x%08lX\n",
					addr, modName, code);
			}
			if (n > 0) {
				DWORD written = 0;
				WriteFile(info, buf, static_cast<DWORD>(n), &written, nullptr);
			}

			// Capture a raw stack sample even if the minidump write fails or the
			// exception context is corrupted (the 20:05 / 21:23 dumps had RSP=0).
			if (a_exc->ContextRecord)
				AppendRawStack(info, reinterpret_cast<const void*>(a_exc->ContextRecord->Rsp));

			static const char pending[] = "Writing minidump...\n";
			DWORD written = 0;
			WriteFile(info, pending, sizeof(pending) - 1, &written, nullptr);
		}

		wchar_t path[MAX_PATH] = {};
		swprintf_s(path, L"%s.dmp", base);
		const HANDLE file = CreateFileW(path, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, nullptr);
		if (file == INVALID_HANDLE_VALUE) {
			if (haveInfo) {
				static const char failed[] = "Failed to open dump file\n";
				DWORD written = 0;
				WriteFile(info, failed, sizeof(failed) - 1, &written, nullptr);
				CloseHandle(info);
			}
			return EXCEPTION_CONTINUE_SEARCH;
		}

		MINIDUMP_EXCEPTION_INFORMATION mei{};
		mei.ThreadId = GetCurrentThreadId();
		mei.ExceptionPointers = a_exc;
		mei.ClientPointers = FALSE;

		// MiniDumpNormal: exception record, faulting thread stack, module list —
		// exactly what we need to name the faulting module and unwind the call.
		const BOOL wrote = g_MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
			file, static_cast<MINIDUMP_TYPE>(MiniDumpNormal), &mei, nullptr, nullptr);

		CloseHandle(file);

		if (haveInfo) {
			char done[128] = {};
			const int n = _snprintf_s(done, _TRUNCATE,
				wrote ? "Minidump written OK\n" : "Minidump write FAILED (error %lu)\n",
				GetLastError());
			if (n > 0) {
				DWORD written = 0;
				WriteFile(info, done, static_cast<DWORD>(n), &written, nullptr);
			}
			CloseHandle(info);
		}

		return EXCEPTION_CONTINUE_SEARCH;
	}
}

namespace sup::crash
{
	void Install() noexcept
	{
		if (const HMODULE dbghelp = LoadLibraryW(L"dbghelp.dll")) {
			g_MiniDumpWriteDump = reinterpret_cast<MiniDumpWriteDumpFn>(
				reinterpret_cast<void*>(GetProcAddress(dbghelp, "MiniDumpWriteDump")));
		}

		if (g_MiniDumpWriteDump)
			AddVectoredExceptionHandler(1, Handler);
	}
}
