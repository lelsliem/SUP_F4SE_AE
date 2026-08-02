



float cmd_ReadINIFloatFromFile(StaticFunctionTag* base, BSFixedString s_FilePath, BSFixedString s_IniSection, BSFixedString s_IniValue)
{

	CSimpleIniA ini;
	ini.SetUnicode();


	string s_FilePathINI = s_FilePath;

	if (!IsFilePathValid(s_FilePathINI))
	{
		return -1;
	}

	auto errVal = ini.LoadFile(s_FilePathINI.c_str());


	if (errVal != SI_OK) {
		return -1;
	};
	float fValue = ini.GetDoubleValue(s_IniSection.c_str(), s_IniValue.c_str());
	return fValue;
}


BSFixedString cmd_ReadINIStringFromFile(StaticFunctionTag* base, BSFixedString s_FilePath, BSFixedString s_IniSection, BSFixedString s_IniValue)
{
	CSimpleIniA ini;
	ini.SetUnicode();
	string s_FilePathINI = s_FilePath;

	if (!IsFilePathValid(s_FilePathINI))
	{
		return "";
	}

	auto errVal = ini.LoadFile(s_FilePathINI.c_str());
	if (errVal != SI_OK) { return ""; };
	const char* sResult = ini.GetValue(s_IniSection.c_str(), s_IniValue.c_str());
	return sResult;
}



VMArray<BSFixedString> cmd_ReadINISectionsFromFile(StaticFunctionTag* base, BSFixedString s_FilePath)
{
	VMArray<BSFixedString> result;
	CSimpleIniA ini;
	ini.SetUnicode();
	string s_FilePathINI = s_FilePath;

	if (!IsFilePathValid(s_FilePathINI))
	{
		return result;
	}

	auto errVal = ini.LoadFile(s_FilePathINI.c_str());
	if (errVal != SI_OK) { return result; };

	CSimpleIniA::TNamesDepend sections;
	ini.GetAllSections(sections);

	CSimpleIniA::TNamesDepend::const_iterator it;
	int i;
	for (i = 0, it = sections.begin(); it != sections.end(); ++i, ++it) {
		result.Push(&BSFixedString(it->pItem));
	}

	return result;
}


VMArray<BSFixedString> cmd_ReadINISectionKeysFromFile(StaticFunctionTag* base, BSFixedString s_FilePath, BSFixedString s_Section)
{

	VMArray<BSFixedString> result;

	CSimpleIniA ini;
	ini.SetUnicode();
	string s_FilePathINI = s_FilePath;

	if (!IsFilePathValid(s_FilePathINI))
	{
		return result;
	}

	auto errVal = ini.LoadFile(s_FilePathINI.c_str());
	if (errVal != SI_OK) { return result; };

	CSimpleIniA::TNamesDepend keys;
	ini.GetAllKeys(s_Section, keys);

	CSimpleIniA::TNamesDepend::const_iterator it;
	int i;
	for (i = 0, it = keys.begin(); it != keys.end(); ++i, ++it) {
		result.Push(&BSFixedString(it->pItem));
	}

	return result;
}



UInt32 cmd_SetINIFloat(StaticFunctionTag* base, BSFixedString s_FilePath, BSFixedString s_IniSection, BSFixedString s_IniValue, float Value)
{
	_DMESSAGE("SetINIFloat for file %s", s_FilePath.c_str());
	std::string s_TempPath = s_ConfigPath + s_FilePath.c_str();

	CSimpleIniA ini;
	ini.SetUnicode(1);


	if (!IsFilePathValid(s_TempPath))
	{
		return -4;
	}

	if (f_Does_FileExist(s_TempPath.c_str()))
	{
		auto errVal = ini.LoadFile(s_TempPath.c_str());
		if (errVal != SI_OK) { return errVal; };
	}
	else {
		if (!f_Does_FolderExist("Data/Config"))
		{
			std::filesystem::create_directory("Data/Config");
			_DMESSAGE("Creating Data//Config folder");
		}
	}

	

	ini.SetDoubleValue(s_IniSection.c_str(), s_IniValue.c_str(), Value);
	return ini.SaveFile(s_TempPath.c_str());
}

UInt32 cmd_SetINILong(StaticFunctionTag* base, BSFixedString s_FilePath, BSFixedString s_IniSection, BSFixedString s_IniValue, UInt32 Value)
{
	_DMESSAGE("SetINILong for file %s", s_FilePath.c_str());
	std::string s_TempPath = s_ConfigPath + s_FilePath.c_str();

	CSimpleIniA ini;
	ini.SetUnicode(1);

	if (!IsFilePathValid(s_TempPath))
	{
		return -4;
	}

	if (f_Does_FileExist(s_TempPath.c_str()))
	{
		auto errVal = ini.LoadFile(s_TempPath.c_str());
		if (errVal != SI_OK) { return errVal; };
	}else {
		if (!f_Does_FolderExist("Data/Config"))
		{
			std::filesystem::create_directory("Data/Config");
			_DMESSAGE("Creating Data//Config folder");
		}
	}


	ini.SetLongValue(s_IniSection.c_str(), s_IniValue.c_str(), Value);
	return ini.SaveFile(s_TempPath.c_str());
}

UInt32 cmd_SetINIString(StaticFunctionTag* base, BSFixedString s_FilePath, BSFixedString s_IniSection, BSFixedString s_IniValue, BSFixedString Value)
{
	_DMESSAGE("SetINIString for file %s", s_FilePath.c_str());
	std::string s_TempPath = s_ConfigPath + s_FilePath.c_str();

	CSimpleIniA ini;
	ini.SetUnicode(1);

	if (!IsFilePathValid(s_TempPath))
	{
		return -4;
	}

	if (f_Does_FileExist(s_TempPath.c_str()))
	{
		auto errVal = ini.LoadFile(s_TempPath.c_str());
		if (errVal != SI_OK) { return errVal; };
	}else {
		if (!f_Does_FolderExist("Data/Config"))
		{
			std::filesystem::create_directory("Data/Config");
			_DMESSAGE("Creating Data//Config folder");
		}
	}

	

	ini.SetValue(s_IniSection.c_str(), s_IniValue.c_str(), Value);
	return ini.SaveFile(s_TempPath.c_str());
}




float cmd_GetINIFloat(StaticFunctionTag* base, BSFixedString s_FilePath, BSFixedString s_IniSection, BSFixedString s_IniValue)
{
	_DMESSAGE("GetINIFloat for file %s", s_FilePath.c_str());
	std::string s_TempPath = s_ConfigPath + s_FilePath.c_str();

	if (!IsFilePathValid(s_TempPath))
	{
		return -4;
	}



	CSimpleIniA ini;
	ini.SetUnicode(1);
	auto errVal = ini.LoadFile(s_TempPath.c_str());
	if (errVal != SI_OK) { return errVal; };
	return ini.GetDoubleValue(s_IniSection.c_str(), s_IniValue.c_str());

}


UInt32 cmd_GetINILong(StaticFunctionTag* base, BSFixedString s_FilePath, BSFixedString s_IniSection, BSFixedString s_IniValue)
{
	_DMESSAGE("GetINIFloat for file %s", s_FilePath.c_str());
	std::string s_TempPath = s_ConfigPath + s_FilePath.c_str();


	if (!IsFilePathValid(s_TempPath))
	{
		return -4;
	}


	CSimpleIniA ini;
	ini.SetUnicode(1);
	auto errVal = ini.LoadFile(s_TempPath.c_str());
	if (errVal != SI_OK) { return errVal; };
	return ini.GetLongValue(s_IniSection.c_str(), s_IniValue.c_str());

}

BSFixedString cmd_GetINIString(StaticFunctionTag* base, BSFixedString s_FilePath, BSFixedString s_IniSection, BSFixedString s_IniValue)
{
	_DMESSAGE("GetINIFloat for file %s", s_FilePath.c_str());
	std::string s_TempPath = s_ConfigPath + s_FilePath.c_str();

	if (!IsFilePathValid(s_TempPath))
	{
		return "";
	}


	CSimpleIniA ini;
	ini.SetUnicode(1);
	auto errVal = ini.LoadFile(s_TempPath.c_str());
	if (errVal != SI_OK) { return ""; };
	return ini.GetValue(s_IniSection.c_str(), s_IniValue.c_str());

}




string sDocPath{};



string f_GetMyDocumentsPath(UInt32 iRequest)
{
	_DMESSAGE("f_GetMyDocumentsPath starts, request is>>%d", iRequest);
	if (!sDocPath.length())
	{
		wchar_t wpath[MAX_PATH];
		char path[MAX_PATH];
		HRESULT err = SHGetFolderPathW(NULL, CSIDL_MYDOCUMENTS | CSIDL_FLAG_CREATE, NULL, SHGFP_TYPE_CURRENT, wpath);

		if (!SUCCEEDED(err))
		{
			_DMESSAGE("Cannot get doc path folder");
			return "";
		}
		WideCharToMultiByte(CP_UTF8, 0, wpath, -1, path, MAX_PATH, NULL, NULL);
		sDocPath = path;
	}

	string sFinalPath = sDocPath;

	if (iRequest == 0) {

		sFinalPath = sFinalPath + "\\My Games\\Fallout4\\Fallout4.ini";
	}
	else if (iRequest == 1)
	{
		sFinalPath = sFinalPath + "\\My Games\\Fallout4\\Fallout4Custom.ini";
	}
	else if (iRequest == 2)
	{
		sFinalPath = sFinalPath + "\\My Games\\Fallout4\\Fallout4Prefs.ini";
	}
	else {
		sFinalPath = "";
	}

	_DMESSAGE("result>>>%s", sFinalPath.c_str());
	return sFinalPath;

}







float cmd_ReadINIFloatFromGameSettingINI(StaticFunctionTag* base, UInt32 iFile, BSFixedString s_IniSection, BSFixedString s_IniValue)
{
	_DMESSAGE("ReadINIFloatFromGameSettingINI starts");
	CSimpleIniA ini;
	ini.SetUnicode();


	string s_FilePathINI = f_GetMyDocumentsPath(iFile);

	auto errVal = ini.LoadFile(s_FilePathINI.c_str());

	if (errVal != SI_OK) {
		return -1;
	};
	float fValue = ini.GetDoubleValue(s_IniSection.c_str(), s_IniValue.c_str());
	return fValue;
}


BSFixedString cmd_ReadINIStringFromGameSettingINI(StaticFunctionTag* base, UInt32 iFile, BSFixedString s_IniSection, BSFixedString s_IniValue)
{
	_DMESSAGE("ReadINIStringFromGameSettingINI starts");
	CSimpleIniA ini;
	ini.SetUnicode();
	string s_FilePathINI = f_GetMyDocumentsPath(iFile);
	auto errVal = ini.LoadFile(s_FilePathINI.c_str());
	if (errVal != SI_OK) { return ""; };
	const char* sResult = ini.GetValue(s_IniSection.c_str(), s_IniValue.c_str());
	return sResult;
}




GameSettingValue cmd_GetGameINISettingValue(StaticFunctionTag* base, BSFixedString s_SectionValue)
{
	_DMESSAGE("GetGameINISettingValue starts");
	GameSettingValue result;
	result.Set<bool>("Success", false);

	Setting* MySetting = GetINISettingSUP(s_SectionValue.c_str());

	if (MySetting != NULL)
	{
		UInt32 SettingType = MySetting->GetType();
		_DMESSAGE("Setting type>>>%d", SettingType);

		if (SettingType == Setting::kType_Integer || SettingType == Setting::kType_Unknown)
		{
			result.Set<bool>("Success", true);
			result.Set<float>("fValue", MySetting->data.u32);
		}
		else if (SettingType == Setting::kType_Float)
		{
			result.Set<bool>("Success", true);
			result.Set<float>("fValue", MySetting->data.f32);
		}
		else if (SettingType == Setting::kType_Bool)
		{
			result.Set<bool>("Success", true);
			result.Set<float>("fValue", MySetting->data.u8);
		}
		else if (SettingType == Setting::kType_String)
		{
			result.Set<bool>("Success", true);
			BSFixedString TempString(MySetting->data.s);
			result.Set<BSFixedString>("sValue", MySetting->data.s);
		}

		return result;
		

	}
	else {
		_DMESSAGE("Setting %s doesn't exist", s_SectionValue.c_str());
		return result;

	}


}