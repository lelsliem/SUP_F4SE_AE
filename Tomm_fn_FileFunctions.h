


UInt32 cmd_FileExists(StaticFunctionTag* base, BSFixedString name)
{

	string s_FileName = name;
	

	if (f_Does_FileExist(s_FileName))
	{

		if (std::filesystem::is_directory(s_FileName))
		{
			return 3;
		}
		else {
			return 1;
		}
	}
	else {
		_DMESSAGE("File not found - looking in binary streams");

		
		if (boost::icontains(s_FileName, "Data/"))
		{
				boost::replace_first(s_FileName, "Data/", "");
				_DMESSAGE("Removing Data folder from path>>%s", s_FileName.c_str());
				BSResourceNiBinaryStream fileStream(s_FileName.c_str());

				if (fileStream.IsValid())
				{
					return 2;
				}
				else {
					return 0;
				}

		}
		else {
			return 0;
		}

	}
}






float cmd_FileGetSize(StaticFunctionTag* base, BSFixedString name)
{
	string s_FilePath = name;

	if (f_Does_FileExist(s_FilePath))
	{
		return std::filesystem::file_size(s_FilePath);
	}
	else {
		return -1;
	}

}

VMArray<BSFixedString> cmd_FolderGetContents(StaticFunctionTag* base, BSFixedString name, UInt32 iRequest) // 0 - get files, 1 - get Folders, 2 - get ALL
{
	string s_FilePath = name;
	VMArray<BSFixedString> result;

	if (!IsFilePathValid(s_FilePath))
	{
		return result;
	}

	if (f_Does_FolderExist(s_FilePath))
	{
		for (auto const& dir_entry : std::filesystem::directory_iterator{ s_FilePath })
		{
			if (iRequest)
			{
				if (iRequest == 1)
				{
					if (dir_entry.is_directory())
						result.Push(&BSFixedString(dir_entry.path().filename().string().c_str()));
				}
				else if (iRequest == 2)
				{
					result.Push(&BSFixedString(dir_entry.path().filename().string().c_str()));
				}

			}
			else
			{
				if (!dir_entry.is_directory())
					result.Push(&BSFixedString(dir_entry.path().filename().string().c_str()));
			}

		}
	}

	return result;
}



UInt32 cmd_FileGetLinesCount(StaticFunctionTag* base, BSFixedString name)
{
	ifstream aInputFile(name.c_str());
	CheckAndRemoveBOM(aInputFile);
	string aLineStr;
	size_t n = 0; while (getline(aInputFile, aLineStr)) n++;
	return n;
}





BSFixedString cmd_ReadStringFromFile(StaticFunctionTag* base, BSFixedString FilePath, UInt32 iStartLine, UInt32 iLineCount)
{
	_DMESSAGE("ReadStringFromFile started with request of>> %s", FilePath.c_str());

	string s_TempPathRelative = FilePath;

	if (!IsFilePathValid(s_TempPathRelative))
	{
		return "";
	}

	ifstream aInputFile;
	aInputFile.open(s_TempPathRelative);

	CheckAndRemoveBOM(aInputFile);


	string str{};
	string s_res{};
	size_t n = 0;

	int iEndLine = iStartLine + iLineCount;
	_DMESSAGE("EndLine is >>%d", iEndLine);

	while (getline(aInputFile, str))
	{
		_DMESSAGE("Line is %s", str.c_str());
		if ((n >= iStartLine) && (n < iEndLine))
		{
			s_res += str;
			_DMESSAGE("line read CHOSEN is %s", str.c_str());
		}

		n++;
	}

	_DMESSAGE("Result line read is %s", s_res.c_str());


	return s_res.c_str();

}





VMArray<BSFixedString> cmd_FileGetFileTime(StaticFunctionTag* base, BSFixedString name, UInt32 iRequest)
{
	VMArray<BSFixedString> result;

	HANDLE hFile;
	char szBuf[MAX_PATH];

	hFile = CreateFileA(name.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		_DMESSAGE("%s %s", "INVALID FILE>>", name.c_str());
		return result;
	}





	FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stUTC, stLocal;


	// Retrieve the file times for the file.
	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
		return result;

	if (iRequest == 0)
		FileTimeToSystemTime(&ftCreate, &stUTC);
	else if (iRequest == 1)
		FileTimeToSystemTime(&ftAccess, &stUTC);
	else
		FileTimeToSystemTime(&ftWrite, &stUTC);

	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

	char tempchar[0x4000];
	StringCchPrintfA(tempchar, MAX_PATH, "%d", stLocal.wYear);
	result.Push(&(BSFixedString)tempchar); //Year
	StringCchPrintfA(tempchar, MAX_PATH, "%02d", stLocal.wMonth);
	result.Push(&(BSFixedString)tempchar); //Month
	StringCchPrintfA(tempchar, MAX_PATH, "%02d", stLocal.wDay);
	result.Push(&(BSFixedString)tempchar); //Day
	StringCchPrintfA(tempchar, MAX_PATH, "%02d", stLocal.wHour);
	result.Push(&(BSFixedString)tempchar); //Hour
	StringCchPrintfA(tempchar, MAX_PATH, "%02d", stLocal.wMinute);
	result.Push(&(BSFixedString)tempchar); //Minute


	return result;

}

#include <fstream>
#include <string>
#include <iostream>

bool cmd_WriteStringToFile(StaticFunctionTag* base, BSFixedString FilePath, BSFixedString String, UInt32 iAppend)
{
	_DMESSAGE("Writing to file %s", FilePath.c_str());


	string s_TempPathRelative = FilePath;

	if (!IsFilePathValid(s_TempPathRelative))
	{
		return false;
	}

	f_Create_Folders_For_Path(s_TempPathRelative);

	if (f_Does_FileExist(FilePath.c_str()))
	{
		string sExtension = std::filesystem::path(FilePath.c_str()).extension().string();

		if (sExtension == ".txt")
		{
		}
		else if (sExtension == ".ini")
		{
		}
		else if (sExtension == ".json")
		{
		}
		else {
			_DMESSAGE("File extension is on on the allowed list.");
			return false;
		}
	}



	



	ofstream myfile;

	if (iAppend)
	{
		myfile.open(s_TempPathRelative, std::ios_base::app);
	}
	else {
		myfile.open(s_TempPathRelative);
	}

	if (!myfile.is_open())
		return false;

	if (iAppend == 2)
	{
		myfile << endl;
	}


	myfile << String.c_str();
	
	myfile.close();
	_DMESSAGE("Writing successfull");
	return true;
}


VMArray<BSFixedString> cmd_ReadStringArrayFromFile(StaticFunctionTag* base, BSFixedString FilePath, BSFixedString delim)
{
	_DMESSAGE("Reading array from file %s", FilePath.c_str());

	VMArray<BSFixedString> result;


	string s_TempPathRelative = FilePath;

	if (!IsFilePathValid(s_TempPathRelative))
	{
		return result;
	}



	ifstream aInputFile(s_TempPathRelative);

	CheckAndRemoveBOM(aInputFile);

	string aLineStr;
	std::stringstream buffer;
	buffer << aInputFile.rdbuf();
	string s_Contents = buffer.str();

	vector<string> v_SplittedString = f_Split_String(s_Contents, delim.c_str());

	for (std::vector<string>::iterator it = v_SplittedString.begin(); it != v_SplittedString.end(); ++it)
	{
		if (Iter.length() > 0)
		{
			result.Push(&BSFixedString(Iter.c_str()));
		}
	}
	return result;
}



VMArray<TESForm*> cmd_ReadFormArrayFromFile(StaticFunctionTag* base, BSFixedString FilePath, BSFixedString delim)
{
	_DMESSAGE("Reading array from file %s", FilePath.c_str());

	VMArray<TESForm*> result;

	string s_TempPathRelative = FilePath;

	if (!IsFilePathValid(s_TempPathRelative))
	{
		return result;
	}


	ifstream aInputFile(s_TempPathRelative);
	CheckAndRemoveBOM(aInputFile);
	string aLineStr;
	std::stringstream buffer;
	buffer << aInputFile.rdbuf();
	string s_Contents = buffer.str();

	vector<string> v_SplittedString = f_Split_String(s_Contents, delim.c_str());

	std::size_t found;
	string s_ModName{};
	string s_Hex{};
	UInt32  mod_Idx, form_Dec;
	UInt32 referenceRes = 0;
	TESForm* form_Result = NULL;


	for (std::vector<string>::iterator it = v_SplittedString.begin(); it != v_SplittedString.end(); ++it)
	{
		if (Iter.length() > 0)
		{
			found = Iter.find(":");

			if (found != std::string::npos)
			{
				s_ModName = Iter.substr(0, found);
				s_Hex = Iter.substr(found + 1, Iter.length() - 1);


				_DMESSAGE("Mod name is>>%s,HeX is>>%s", s_ModName.c_str(), s_Hex.c_str());


				if (s_Hex.length() > 10)
				{
					_DMESSAGE("HeX is too long - skipping");
					continue;
				}

				form_Result = BuildRefFormFromBSSTR(s_Hex.c_str(), s_ModName.c_str(), 0);

				if (form_Result)
				{
					_DMESSAGE("Adding to array is>>%x", form_Result->formID);
					result.Push(&form_Result);
				}
				
			}
		}
	}

	return result;
}


float cmd_GetLastLoadedSaveSize(StaticFunctionTag* base, UInt32 iRequest) // 0 for fos, 1 for f4se
{
	string s_FilePath{};


	if (g_LastLoadedSaveGameNameValid)
	{

		if (iRequest)
		{
			s_FilePath = MakeSavePathSUP(g_LastLoadedSaveName, ".f4se");
			if (f_Does_FileExist(s_FilePath))
			{
				return std::filesystem::file_size(s_FilePath);
			}
			else {
				return -1;
			}

		}
		else {
			s_FilePath = MakeSavePathSUP(g_LastLoadedSaveName, ".fos");
			if (f_Does_FileExist(s_FilePath))
			{
				return std::filesystem::file_size(s_FilePath);
			}
			else {
				return -1;
			}

		}


	}
	else {
		return -1;
	}

}


BSFixedString cmd_GetLastLoadedSaveName(StaticFunctionTag* base, UInt32 iRequest) // 0 for fos, 1 for f4se
{
	string s_FilePath{};


	if (!g_LastLoadedSaveGameNameValid)
	{
		return "";
	}
	std::string sResult{};
	if (iRequest)
	{
		sResult = g_LastLoadedSaveName + ".f4se";
	}
	else {
		sResult = g_LastLoadedSaveName + ".fos";
	}
	return sResult.c_str();
}




UInt32 cmd_FileExistsAlt(StaticFunctionTag* base, BSFixedString name)
{

	string s_FileName = name;
	string s_FileNameAlt = "Data\\" + s_FileName;

	if (f_Does_FileExist(s_FileNameAlt))
	{

		if (std::filesystem::is_directory(s_FileNameAlt))
		{
			return 3;
		}
		else {
			return 1;
		}
	}
	else {
		_DMESSAGE("File not found - looking in binary streams");

		BSResourceNiBinaryStream fileStream(s_FileName.c_str());
		if (fileStream.IsValid())
		{
			return 2;
		}
		else {
			return 0;
		}
	}
}


void RemoveGamePathFromFilePath(std::string& s_FilePath)
{

	auto found = s_FilePath.find(g_FalloutFolderPath);

	_DMESSAGE("File path before path>>{}", s_FilePath);
	if (found != std::string_view::npos)
	{
		s_FilePath.erase(found,g_FalloutFolderPath.length());
	}
	_DMESSAGE("Cleared File path>>{}", s_FilePath);
}



VMArray<BSFixedString> cmd_FolderGetContentsAlt(StaticFunctionTag* base, BSFixedString name, UInt32 iRequest) // 0 - get files, 1 - get Folders, 2 - get ALL
{
	string s_FilePath = name;
	VMArray<BSFixedString> result;


	if (!IsFilePathValid(s_FilePath))
	{
		return result;
	}

	if (f_Does_FolderExist(s_FilePath))
	{
		for (auto const& dir_entry : std::filesystem::directory_iterator{ s_FilePath })
		{
			if (iRequest)
			{
				if (iRequest == 1)
				{
					if (dir_entry.is_directory())
					{
						auto TempPath = dir_entry.path().string();
						RemoveGamePathFromFilePath(TempPath);
						result.Push(&BSFixedString(TempPath.c_str()));
					}
						
				}
				else if (iRequest == 2)
				{
					auto TempPath = dir_entry.path().string();
					RemoveGamePathFromFilePath(TempPath);
					result.Push(&BSFixedString(TempPath.c_str()));
				}

			}
			else
			{
				if (!dir_entry.is_directory())
				{
					auto TempPath = dir_entry.path().string();
					RemoveGamePathFromFilePath(TempPath);
					result.Push(&BSFixedString(TempPath.c_str()));
				}

			}

		}
	}

	return result;
}