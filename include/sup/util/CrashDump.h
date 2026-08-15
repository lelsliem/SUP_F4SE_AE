#pragma once

// SUP F4SE — in-process minidump capture.
//
// Windows WER produced no crash artifacts for the NISTRON placement CTDs, so we
// capture our own: a vectored exception handler writes a MiniDumpNormal dump next
// to SUP_F4SE.log on any fatal exception (access violation, stack overflow, ...).
// The game still crashes as normal afterwards — we just leave evidence behind.

#include <cstdint>

namespace sup::crash
{
	// Depth of SEH-guarded regions currently executing on this thread. The
	// vectored handler skips dumping while > 0: the fault is inside a region whose
	// __try/__except will swallow it, so a dump there would be a false alarm (the
	// handler fires first-chance, before our __except runs). Only regions that
	// are guaranteed to handle *any* exception raise this.
	inline thread_local std::uint32_t g_SehGuardDepth = 0;

	// Installs the handler. Safe to call once at plugin load; the handler never
	// touches the loader or heap when the process is already dying.
	void Install() noexcept;
}
