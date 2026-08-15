

UInt32 cmd_GetSUPF4SEVersion(StaticFunctionTag* base)
{
	return iVersion;
}


float cmd_GetSystemTrait(StaticFunctionTag* base, UInt32 iTrait)
{

	_DMESSAGE("GetSystemTrait starts, request is %f", iTrait);

	if (iTrait == 0 || iTrait == 1 || iTrait == 6)
	{
		NTSTATUS(WINAPI * RtlGetVersion)(LPOSVERSIONINFOEXW);
		OSVERSIONINFOEXW osInfo;

		auto module_Handle = GetModuleHandleA("ntdll");

		if (!module_Handle)
		{
			_DMESSAGE("GetSystemTrait::no ntdll");
			return -1;
		}

		*reinterpret_cast<FARPROC*>(&RtlGetVersion) = GetProcAddress(module_Handle, "RtlGetVersion");

		if (nullptr != RtlGetVersion)
		{
			osInfo.dwOSVersionInfoSize = sizeof osInfo;
			RtlGetVersion(&osInfo);
			if (iTrait == 0)
			{
				return osInfo.dwMajorVersion;
			}
			else if (iTrait == 1)
			{
				return osInfo.dwMinorVersion;
			}
			else
			{
				return osInfo.dwBuildNumber;
			}
		}

	}
	else if (iTrait == 2 || iTrait == 3 || iTrait == 4)
	{
		MEMORYSTATUSEX statex;
		statex.dwLength = sizeof(statex);
		GlobalMemoryStatusEx(&statex);
		if (iTrait == 2)
		{
			return statex.ullTotalPhys / 1024;
		}
		else if (iTrait == 3)
		{
			return statex.ullAvailPhys / 1024;
		}
		else if (iTrait == 4)
		{
			return statex.dwMemoryLoad;
		}
	}
	else if (iTrait == 5)
	{
		return GetTickCount();
	}
	else if (iTrait == 7) // width
	{
		return GetDesktopResolution(0);
	}
	else if (iTrait == 8) // height
	{
		return GetDesktopResolution(1);
	}

	return -1;
}




bool cmd_IsProcessRunning(StaticFunctionTag* base, BSFixedString name)
{
	return IsProcessRunning(f_Convert_String_to_StringW(name.c_str()).c_str());
}


UInt32 cmd_GetMousePosition(StaticFunctionTag* base, UInt32 iRequest) // 0 for width, 1 for height
{
	POINT p;
	if (GetCursorPos(&p))
	{
		if (iRequest)
		{
			return p.y;

		}
		else {

			return p.x;
		}
	}
	else {
		return -1;
	}

}


BSFixedString cmd_ClipboardToString(StaticFunctionTag* base)
{
	_DMESSAGE("ClipboardToString started");

	OpenClipboard(0);
	string sResult{};


	char* pResult = (char*)GetClipboardData(CF_TEXT);

	if (pResult != NULL) {

		sResult = pResult;
		GlobalFree(pResult);
	}
	CloseClipboard();

	_DMESSAGE("ClipboardToString result is %s", sResult.c_str());
	return sResult.c_str();
}


bool cmd_StringToClipboard(StaticFunctionTag* base, BSFixedString sIN)
{
	_DMESSAGE("StringToClipboard started");

	string s = sIN.c_str();

	OpenClipboard(0);

	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
	if (!hg) {
		CloseClipboard();
		_DMESSAGE("StringToClipboard Failed");
		return false;
	}
	memcpy(GlobalLock(hg), s.c_str(), s.size() + 1);
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
	_DMESSAGE("StringToClipboard Succeeded");
	return true;
}


BSFixedString cmd_GetFalloutRootFolder(StaticFunctionTag* base)
{
	return g_FalloutFolderPath.c_str();
}



bool cmd_IsDLLLoaded(StaticFunctionTag* base, BSFixedString name)
{
	HMODULE DLL_Module = GetModuleHandleA(name.c_str());


	if (DLL_Module == NULL)
	{
		return false;
	}
	else {
		return true;
	}
}


BSFixedString cmd_GetSHA1FromFile(StaticFunctionTag* base, BSFixedString sFilePath)
{
	if (!f_Does_FileExist(sFilePath.c_str()))
	{
		return "";
	}
	return (SHA1::from_file(sFilePath.c_str())).c_str();
}

BSFixedString cmd_GetSHA1FromString(StaticFunctionTag* base, BSFixedString sString)
{
	SHA1 s_SHA{};
	s_SHA.update(sString.c_str());
	return (s_SHA.final().c_str());
}

BSFixedString cmd_GetUserTimeStamp(StaticFunctionTag* base)
{
	const auto now = std::chrono::system_clock::now();
	const auto in_time_t = std::chrono::system_clock::to_time_t(now);


	std::stringstream output_stream;
	struct tm time_info;
	const auto errno_value = localtime_s(&time_info, &in_time_t);
	if (errno_value != 0)
	{
		return "";
	}


	output_stream << std::put_time(&time_info, "%Y%m%d%H%M%S");
	return output_stream.str().c_str();
}


VMArray<UInt32> cmd_GetUserTime(StaticFunctionTag* base)
{
	VMArray<UInt32> result;
	const auto now = std::chrono::system_clock::now();
	const auto in_time_t = std::chrono::system_clock::to_time_t(now);
	struct tm time_info;
	const auto errno_value = localtime_s(&time_info, &in_time_t);
	if (errno_value != 0)
	{
		return result;
	}

	UInt32 iTempValue = time_info.tm_year + 1900;
	result.Push(&iTempValue);
	iTempValue = time_info.tm_mon + 1;
	result.Push(&iTempValue);
	iTempValue = time_info.tm_mday;
	result.Push(&iTempValue);
	iTempValue = time_info.tm_hour;
	result.Push(&iTempValue);
	iTempValue = time_info.tm_min;
	result.Push(&iTempValue);
	iTempValue = time_info.tm_sec;
	result.Push(&iTempValue);
	return result;
}





using SUPUserTimeStruct = RE::BSScript::structure_wrapper<"SUP_F4SE", "UserTimeStruct">;

SUPUserTimeStruct cmd_GetUserTimeStruct(StaticFunctionTag* base)
{
	SUPUserTimeStruct result;
	const auto now = std::chrono::system_clock::now();
	const auto in_time_t = std::chrono::system_clock::to_time_t(now);
	struct tm time_info;
	const auto errno_value = localtime_s(&time_info, &in_time_t);
	if (errno_value != 0)
	{
		return result;
	}

	UInt32 iTempValue = time_info.tm_year + 1900;
	result.insert("Year", iTempValue);
	iTempValue = time_info.tm_mon + 1;
	result.insert("Month", iTempValue);
	iTempValue = time_info.tm_mday;
	result.insert("Day", iTempValue);
	iTempValue = time_info.tm_hour;
	result.insert("Hour", iTempValue);
	iTempValue = time_info.tm_min;
	result.insert("Minute", iTempValue);
	iTempValue = time_info.tm_sec;
	result.insert("Second", iTempValue);

	return result;
}





BSFixedString cmd_GetUserWindowsLanguage(StaticFunctionTag* base)
{
	WCHAR localeName[LOCALE_NAME_MAX_LENGTH] = { 0 };
	int iresult = GetUserDefaultLocaleName(localeName, (sizeof(localeName) / sizeof(*(localeName))));
	if (iresult == 0)
	{
		_DMESSAGE("Cant extract locale");
		return "";
	}
	else
	{
		string sRes = f_Convert_WString_to_string(localeName);
		return sRes.c_str();
	}
}




FileVersion cmd_GetFileVersion(StaticFunctionTag* base, BSFixedString FilePath)
{
	_DMESSAGE("GetEXEVersion started");
	FileVersion result;

	string s_FilePath = FilePath.c_str();

	if (s_FilePath.length() == 0)
	{
		s_FilePath = "Fallout4.exe";
	}


	if (!IsFilePathValid(s_FilePath))
	{
		return result;
	}


	if (!f_Does_FileExist(s_FilePath))
	{
		_DMESSAGE("file doesn't exist");
		return result;
	}

	DWORD               dwSize = 0;
	VS_FIXEDFILEINFO* pFileInfo = NULL;
	UINT                pLenFileInfo = 0;

	dwSize = GetFileVersionInfoSizeA(s_FilePath.c_str(), NULL);

	if (dwSize == 0)
	{
		_DMESSAGE("No Version Information!");
		return result;
	}
	else {
		_DMESSAGE("Version Information present");
	}

	char* buffer = new char[dwSize];
	if (!GetFileVersionInfoA(s_FilePath.c_str(), 0, dwSize, (void*)buffer))
	{
		return result;
	}


	unsigned int len;
	VS_FIXEDFILEINFO* data = NULL;
	bool bResult = VerQueryValueA(buffer, "\\", (void**)&data, &len);

	if (data)
	{

		result.Set<UInt32>("Success", 1);
		result.Set<UInt32>("VerMajor", (data->dwFileVersionMS >> 16) & 0xffff);
		result.Set<UInt32>("VerMinor", (data->dwFileVersionMS) & 0xffff);
		result.Set<UInt32>("VerHotfix", (data->dwFileVersionLS >> 16) & 0xffff);
		result.Set<UInt32>("VerOther", (data->dwFileVersionLS) & 0xffff);
		return result;
	}
	else {
		return result;
	}
}


