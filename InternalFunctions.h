


bool f_Does_FolderExist(string name)
{

	if (std::filesystem::is_directory(name)) 
	{
		return true;
	}
	else {
		return false;
	}
}


bool f_Does_FileExist(string name)
{

	if (std::filesystem::exists(name))
	{
		return true;
	}
	else {
		return false;
	}
}




std::wstring f_Convert_String_to_StringW(std::string sIn)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
	std::wstring utf16String = convert.from_bytes(sIn);
	return utf16String;
}

std::string f_Convert_WString_to_string(wstring wIn)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
	std::string utf8String = convert.to_bytes(wIn);
	return utf8String;
}




LRESULT CALLBACK X_CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{

	if (nCode == HCBT_CREATEWND)
		g_HWND = (HWND)wParam;
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void CallXAndGetHWND()
{
	if (bCapturedHWND == false)
	{
		HHOOK hDllHook = SetWindowsHookEx(WH_CBT, X_CBTProc, NULL, GetCurrentThreadId());
		UnhookWindowsHookEx(hDllHook);
		bCapturedHWND = true;
	}

}


float GetDesktopResolution(int iRequest) // 0 for Width, 1 for Height
{
	_DMESSAGE("GetDesktopResolution starts, request is %d", iRequest);


	if (bResolutionCaptured)
	{
	}
	else {
		if (bCapturedHWND == false)
		{
			_DMESSAGE("g_HWND is NULL - GETTING");
			CallXAndGetHWND();
		}

		bResolutionCaptured = true;
		HMONITOR monitor = MonitorFromWindow(g_HWND, MONITOR_DEFAULTTONEAREST);
		MONITORINFO info{};
		info.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(monitor, &info);
		iScreenWidth = info.rcMonitor.right - info.rcMonitor.left;
		iScreenHeight = info.rcMonitor.bottom - info.rcMonitor.top;
		_DMESSAGE("Getting screen resolution, height is %d, width is %d", iScreenHeight, iScreenWidth);

	}



	if (iRequest)
	{
		return iScreenHeight;
	}
	else {
		return iScreenWidth;
	}

}



bool IsProcessRunning(const TCHAR* const executableName) {
	PROCESSENTRY32 entry{};
	entry.dwSize = sizeof(PROCESSENTRY32);

	const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (!Process32First(snapshot, &entry)) {
		CloseHandle(snapshot);
		return false;
	}

	do {
		if (!_tcsicmp(entry.szExeFile, executableName)) {
			CloseHandle(snapshot);
			return true;
		}
	} while (Process32Next(snapshot, &entry));

	CloseHandle(snapshot);
	return false;
}

bool IsDirectorySafe(const char* parent, const char* relPath) {
	char fullPath[MAX_PATH] = "";
	_DMESSAGE("IsDirectorySafe started, FULL path is %s, Relative path is %s", parent, relPath);


	if (!GetFullPathNameA(relPath, MAX_PATH, fullPath, nullptr)) {
		_DMESSAGE("Fail1");
		return false;
	}

	_DMESSAGE("Comparing %s and %s", fullPath, parent);


	if (strstr(fullPath, parent) != fullPath) {
		_DMESSAGE("Fail2");
		return false;
	}

	return true;
}


void f_Create_Folders_For_Path(string s_FilePath)
{

	auto FolderPath = std::filesystem::path(s_FilePath).parent_path();

	if (FolderPath.string().length() > 0)
	{
		std::filesystem::create_directories(FolderPath);
	}

}


std::vector<std::string> f_Split_String(std::string str, std::string delimeter)
{
	std::vector<std::string> splitStrings = {};
	size_t pos = 0;

	while ((pos = str.find(delimeter)) != std::string::npos)
	{
		std::string token = str.substr(0, pos);
		if (token.length() > 0)
			splitStrings.push_back(token);
		str.erase(0, pos + delimeter.length());
	}

	if (str.length() > 0)
		splitStrings.push_back(str);
	return splitStrings;
}


bool f_IsFormValid(TESForm* thisForm)
{
	if (!thisForm)
		return false;

	TESForm* CheckForm = NULL;
	CheckForm = (TESForm*)LookupFormByID(thisForm->formID);




	if (CheckForm)
	{
		return true;
	}
	else {
		return false;
	}

}

std::string MakeSavePathSUP(std::string name, const char* extension)
{
	wchar_t wpath[MAX_PATH];
	char path[MAX_PATH];
	ASSERT(SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, wpath)));
	WideCharToMultiByte(CP_UTF8, 0, wpath, -1, path, MAX_PATH, NULL, NULL);

	std::string	result = path;
	result += FO4_SavegamePath;
	Setting* localSavePath = GetINISetting("sLocalSavePath:General");
	if (localSavePath && (localSavePath->GetType() == Setting::kType_String))
		result += localSavePath->data.s;
	else
		result += "Saves\\";

	result += "\\";
	result += name;
	if (extension)
		result += extension;
	return result;
}


void DumpClassAlt(void* theClassPtr, UInt64 nIntsToDump)
{
	UInt64* basePtr = (UInt64*)theClassPtr;

	_MESSAGE("DumpClass: %016I64X", basePtr);

	gLog.Indent();

	if (!theClassPtr) return;
	for (UInt64 ix = 0; ix < nIntsToDump; ix++) {
		UInt64* curPtr = basePtr + ix;
		const char* curPtrName = NULL;
		UInt64 otherPtr = 0;
		float otherFloat1 = 0.0;
		float otherFloat2 = 0.0;
		const char* otherPtrName = NULL;
		if (curPtr) {
			curPtrName = GetObjectClassName((void*)curPtr);

			__try
			{
				otherPtr = *curPtr;
				UInt32 lowerFloat = otherPtr & 0xFFFFFFFF;
				UInt32 upperFloat = (otherPtr >> 32) & 0xFFFFFFFF;
				otherFloat1 = *(float*)&lowerFloat;
				otherFloat2 = *(float*)&upperFloat;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				//
			}

			if (otherPtr) {
				otherPtrName = GetObjectClassName((void*)otherPtr);
			}
		}

		_MESSAGE("%3d +%03X ptr: 0x%016I64X: %32s *ptr: 0x%016I64x | %f, %f: %32s", ix, ix * 8, curPtr, curPtrName, otherPtr, otherFloat2, otherFloat1, otherPtrName);
	}

	gLog.Outdent();
}

bool getBit(long x, int i) {
	return ((x >> i) & 1) != 0;
}

UInt16 setABit(int input, int n) {
	return input | (1 << n - 1);
}


UInt16 clearABit(int input, int n) {
	return input & ~(1 << n - 1);
}

int f_GetStatCodeResponse(HINTERNET* hData)
{
	DWORD statCodeLen = sizeof(DWORD);
	DWORD statCode;
	if (HttpQueryInfo((*hData), HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &statCode, &statCodeLen, NULL))
	{
		return statCode;
	}
}



const ModInfo* f_GetLoadedModNameByIndex(UInt32 iModIndex)
{
	for (UInt32 i = 0; i < (*g_dataHandler)->modList.loadedMods.count; i++) {
		ModInfo* modInfo = (*g_dataHandler)->modList.loadedMods[i];
		if (modInfo->modIndex == iModIndex)
			return modInfo;
	}

	return NULL;
}

const ModInfo* f_GetLightLoadedModNameByIndex(UInt32 iModIndex)
{
	for (UInt32 i = 0; i < (*g_dataHandler)->modList.lightMods.count; i++) {
		ModInfo* modInfo = (*g_dataHandler)->modList.lightMods[i];
		if (modInfo->modIndex == iModIndex)
			return modInfo;
	}

	return NULL;
}




bool f_BB_IsCellInterior(TESObjectCELL* Cell)
{
	if ((Cell->flags & TESObjectCELL::kFlag_IsInterior) != TESObjectCELL::kFlag_IsInterior)
	{
		return false;
	}
	else {
		return true;
	}

}

Setting* GetINISettingSUP(const char* name)
{
	Setting* setting = (*g_iniSettings)->Get(name);
	if (!setting)
		setting = (*g_iniPrefSettings)->Get(name);

	return setting;
}



void f_SetTextInputState(UInt8 iState)
{
	(*g_inputMgr)->allowTextInput = iState;
}



void f_ShowTextBoxMessage(string s_Message)
{
	_DMESSAGE("f_ShowTextBoxMessage started with message>>>%s", s_Message.c_str());
	BSFixedString g_Message;
	CALL_MEMBER_FN(&g_Message, Set)(s_Message.c_str());

	VMArray<VMVariable> arguments;
	VMValue args;
	VMVariable var1;
	var1.Set<BSFixedString>(&g_Message);
	arguments.Push(&var1);
	arguments.PackArray(&args, g_MyVirtMachine);

	BSFixedString g_ScriptName;
	BSFixedString g_FuncName;

	const char* ptr = "Debug";
	CALL_MEMBER_FN(&g_ScriptName, Set)(ptr);
	const char* ptr2 = "MessageBox"; //Notification
	CALL_MEMBER_FN(&g_FuncName, Set)(ptr2);

	_DMESSAGE("MessageBox Call 1");
	CallGlobalFunctionNoWait_Internal(g_MyVirtMachine, 0, 0, &g_ScriptName, &g_FuncName, &args);
	_DMESSAGE("MessageBox Call 2");
}


bool is_string_number(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

double f_Rad_To_Angle(float fRad)
{
	const double pi = 3.1415926535897932384626433832795;
	return 180 / pi * fRad;
}








TESForm* BuildRefFormFromDEC(UInt32 form_Dec, BSFixedString sPluginFileName, UInt32 iLookForESLFirst) {



	const ModInfo* MyModInfo = NULL;

	if (iLookForESLFirst)
	{
		MyModInfo = (*g_dataHandler)->LookupLoadedLightModByName(sPluginFileName.c_str());

		if (MyModInfo == NULL)
		{
			_DMESSAGE("Mod not found --- Looking for heavy mod");
			MyModInfo = (*g_dataHandler)->LookupLoadedModByName(sPluginFileName.c_str());
		}

	}
	else {
		MyModInfo = (*g_dataHandler)->LookupLoadedModByName(sPluginFileName.c_str());

		if (MyModInfo == NULL)
		{
			_DMESSAGE("Mod not found --- Looking for light mod");
			MyModInfo = (*g_dataHandler)->LookupLoadedLightModByName(sPluginFileName.c_str());
		}

	}

	if (MyModInfo == NULL)
	{
		_DMESSAGE("Mod not loaded");
		return NULL;
	}


	UInt32 referenceRes;
	_DMESSAGE("form_Dec is %d", form_Dec);
	_DMESSAGE("modIndex is %d", MyModInfo->modIndex);

	if (MyModInfo->IsLight())
	{
		_DMESSAGE("Mod is light");

		referenceRes = 0xFE << 24 | ((MyModInfo->lightIndex) << 12) | (form_Dec & 0x00000FFF);
	}
	else {
		referenceRes = ((MyModInfo->modIndex & 0xFF) << 24) | (form_Dec & 0x00FFFFFF);
	}

	//referenceRes = 4261417039;
	_DMESSAGE("ReferenceRes is %d", referenceRes);

	return (TESForm*)LookupFormByID(referenceRes);
}








TESForm* BuildRefFormFromBSSTR(BSFixedString sFormID, BSFixedString sPluginFileName, UInt32 iLookForESLFirst) {
	const ModInfo* MyModInfo = NULL;

	if (iLookForESLFirst)
	{
		MyModInfo = (*g_dataHandler)->LookupLoadedLightModByName(sPluginFileName.c_str());

		if (MyModInfo == NULL)
		{
			_DMESSAGE("Mod not found --- Looking for heavy mod");
			MyModInfo = (*g_dataHandler)->LookupLoadedModByName(sPluginFileName.c_str());
		}

	}
	else {
		MyModInfo = (*g_dataHandler)->LookupLoadedModByName(sPluginFileName.c_str());

		if (MyModInfo == NULL)
		{
			_DMESSAGE("Mod not found --- Looking for light mod");
			MyModInfo = (*g_dataHandler)->LookupLoadedLightModByName(sPluginFileName.c_str());
		}

	}



	if (MyModInfo == NULL)
	{
		_DMESSAGE("Mod not loaded");
		return NULL;
	}


	UInt32 form_Dec;
	string s_FormIDs = sFormID;

	try
	{
		form_Dec = std::stoul(s_FormIDs, nullptr, 16);
	}
	catch (std::invalid_argument e)
	{
		_DMESSAGE("HeX threw exception inv agr - skipping");
		return NULL;
	}
	catch (std::out_of_range  e)
	{
		_DMESSAGE("HeX threw exception out of range - skipping");
		return NULL;
	}

	UInt32 referenceRes;
	_DMESSAGE("form_Dec is %d", form_Dec);
	_DMESSAGE("modIndex is %d", MyModInfo->modIndex);

	if (MyModInfo->IsLight())
	{
		_DMESSAGE("Mod is light");
		referenceRes = 0xFE << 24 | ((MyModInfo->lightIndex) << 12) | (form_Dec & 0x00000FFF);
	}
	else {
		referenceRes = ((MyModInfo->modIndex & 0xFF) << 24) | (form_Dec & 0x00FFFFFF);
	}

	_DMESSAGE("ReferenceRes is %d", referenceRes);

	return (TESForm*)LookupFormByID(referenceRes);
}

TESForm* BuildRefFormFromSTR(std::string sFormID, std::string sPluginFileName, UInt32 iLookForESLFirst) {
	const ModInfo* MyModInfo = NULL;

	if (iLookForESLFirst)
	{
		MyModInfo = (*g_dataHandler)->LookupLoadedLightModByName(sPluginFileName.c_str());

		if (MyModInfo == NULL)
		{
			_DMESSAGE("Mod not found --- Looking for heavy mod");
			MyModInfo = (*g_dataHandler)->LookupLoadedModByName(sPluginFileName.c_str());
		}

	}
	else {
		MyModInfo = (*g_dataHandler)->LookupLoadedModByName(sPluginFileName.c_str());

		if (MyModInfo == NULL)
		{
			_DMESSAGE("Mod not found --- Looking for light mod");
			MyModInfo = (*g_dataHandler)->LookupLoadedLightModByName(sPluginFileName.c_str());
		}

	}



	if (MyModInfo == NULL)
	{
		_DMESSAGE("Mod not loaded");
		return NULL;
	}


	UInt32 form_Dec;
	string s_FormIDs = sFormID;

	try
	{
		form_Dec = std::stoul(s_FormIDs, nullptr, 16);
	}
	catch (std::invalid_argument e)
	{
		_DMESSAGE("HeX threw exception inv agr - skipping");
		return NULL;
	}
	catch (std::out_of_range  e)
	{
		_DMESSAGE("HeX threw exception out of range - skipping");
		return NULL;
	}

	UInt32 referenceRes;
	_DMESSAGE("form_Dec is %d", form_Dec);
	_DMESSAGE("modIndex is %d", MyModInfo->modIndex);

	if (MyModInfo->IsLight())
	{
		_DMESSAGE("Mod is light");
		referenceRes = 0xFE << 24 | ((MyModInfo->lightIndex) << 12) | (form_Dec & 0x00000FFF);
	}
	else {
		referenceRes = ((MyModInfo->modIndex & 0xFF) << 24) | (form_Dec & 0x00FFFFFF);
	}

	_DMESSAGE("ReferenceRes is %d", referenceRes);

	return (TESForm*)LookupFormByID(referenceRes);
}




struct JSONContainsResult //storing original key before lowering
{
	bool bSuccess = false;
	string sOriginalKey{};
};



JSONContainsResult f_JSON_iContains(nlohmann::json* jTempIter, string sKey)
{
	JSONContainsResult result;

	if (jTempIter->is_primitive())
	{
		_DMESSAGE("Object is primitive - not contains");
		return result;
	}
	else if (jTempIter->is_array())
	{
		_DMESSAGE("Object is array - not contains");
		return result;
	}

	for (auto it = jTempIter->begin(); it != jTempIter->end(); ++it)
	{

		string sKeyTemp = it.key();
		boost::to_lower(sKeyTemp);

		if (boost::equals(sKey, sKeyTemp))
		{
			//_DMESSAGE("EQUAL");
			result.bSuccess = true;
			result.sOriginalKey = it.key();
			return result;
		}


	}
	return result;
}






__forceinline bool IsFilePathValid(string& s_TempPathRelative) // affects input string
{
	boost::replace_all(s_TempPathRelative, "/", "\\");
	string s_FullPathRelative = g_FalloutFolderPath + s_TempPathRelative;

	if (IsDirectorySafe(s_FullPathRelative.c_str(), s_TempPathRelative.c_str()))
	{
		_DMESSAGE("Check Pass succesfull");
		return true;
	}
	else {
		_DMESSAGE("Check Pass failed")
		return false;
	}
}




Workshop::ExtraData* f_GetPowerGridExtraData(TESObjectREFR* WorkshopRef)
{

	BSExtraData* MyExtraDataTemp = WorkshopRef->extraDataList->GetByType(kExtraData_WorkshopExtraData);

	if (!MyExtraDataTemp)
	{
		_DMESSAGE("reference doesn't have workshop power grid extra data");
		return NULL;
	}

	return (Workshop::ExtraData*)MyExtraDataTemp;

}



struct PowerGridInstanceInternal {
	bool Success = false;
	UInt32 ID = -1;
	float Load = 0;
	float Capacity = 0;
};

PowerGridInstanceInternal f_GetPowerGridForObjectFromExtraData( Workshop::ExtraData* WorkshopData, TESObjectREFR* ObjectRef)
{
	PowerGridInstanceInternal result;

	if (!WorkshopData)
	{
		return result;
	}


	for (UInt64 i = 0; i < WorkshopData->powerGrid.count; i++)
	{
		PowerUtils::PowerGrid* pGrid;
		WorkshopData->powerGrid.GetNthItem(i, pGrid);

		if (pGrid)
		{
			for (auto it = pGrid->adjacencyMap.begin(); it != pGrid->adjacencyMap.end(); it++)
			{
				_DMESSAGE("adjacency map ref>>>%x", Iter.first);
			}



			auto FoundItem = pGrid->adjacencyMap.find(ObjectRef->formID);

			if (FoundItem != pGrid->adjacencyMap.end())
			{
				result.Success = true;
				result.ID = i;
				result.Load = pGrid->load;
				result.Capacity = pGrid->capacity;
				return result;
			}
			else {
				_DMESSAGE("ObjectRef does NOT exist");
				return result;
			}
		}
	}

	return result;
}

TESObjectREFR* f_GetCurrentConsoleReference()
{
    	if ((*g_consoleHandle) != 0 && (*g_consoleHandle) != (*g_invalidRefHandle)) {
			NiPointer<TESObjectREFR> refr;
    		LookupREFRByHandle((*g_consoleHandle), refr);
    		return refr;
    	}
    
    	return nullptr;
}




TESObjectREFR* f_GetPlayerMapMarker()
{
	// PlayerMapMarkerHandle no longer accessible in F4SE 0.7.7
	// This function needs to be reimplemented using Address Library
	// For now, return nullptr
	return nullptr;
}



template <typename T>
vector<T> ConvertVMArrayToVector(VMArray<T>& VMARR)
{



	vector<T> result;
	result.reserve(VMARR.Length());

	for (UInt32 i = 0; i < VMARR.Length(); i++)
	{
		T CurrentRef = NULL;
		VMARR.Get(&CurrentRef, i);
		if (CurrentRef)
		{
			result.push_back(CurrentRef);
		}
	}

	return result;
}


template <typename T, typename TYPE>
vector<T> ConvertVMArrayToVectorCheckType(VMArray<T>& VMARR, TYPE CurrentType)
{
	vector<T> result;
	result.reserve(VMARR.Length());
	for (UInt32 i = 0; i < VMARR.Length(); i++)
	{
		T CurrentRef;
		VMARR.Get(&CurrentRef, i);
		if (CurrentRef)
		{

			if (CurrentRef->formType == CurrentType)
			{
				result.push_back(CurrentRef);
			}
		}
	}

	return result;
}


void __forceinline CheckRefForKeywordsAndAddToArray(VMArray<TESObjectREFR*>& result, TESObjectREFR* TempRef, vector<BGSKeyword*>& KeywordToInclude, vector<BGSKeyword*>& KeywordToExclude, bool bCheckForAnyIncluded, bool bCheckForAnyExcluded, int& iKeywordToExcludeSize)
{
	bool bFound = false;

	if (bCheckForAnyIncluded)
	{
		for (auto it = KeywordToInclude.begin(); it != KeywordToInclude.end(); it++)
		{
			if (TempRef->baseForm && DYNAMIC_CAST(TempRef->baseForm, TESForm, BGSKeywordForm) &&
				((BGSKeywordForm*)TempRef->baseForm)->HasKeyword((void*)Iter))
			{
				bFound = true;
				break;

			}
		}

	}
	else {
		bFound = true;

		for (auto it = KeywordToInclude.begin(); it != KeywordToInclude.end(); it++)
		{
			bool hasKW = TempRef->baseForm && DYNAMIC_CAST(TempRef->baseForm, TESForm, BGSKeywordForm) &&
				((BGSKeywordForm*)TempRef->baseForm)->HasKeyword((void*)Iter);
			if (!hasKW)
			{
				bFound = false;
				break;

			}
		}

	}

	if (!bFound)
	{
		return;
	}

	if (!iKeywordToExcludeSize)
	{
		result.Push(&TempRef);
		return;
	}

	bool bFoundExcluded = false;

	if (bCheckForAnyExcluded)
	{
		for (auto it = KeywordToExclude.begin(); it != KeywordToExclude.end(); it++)
		{
			bool hasKW = TempRef->baseForm && DYNAMIC_CAST(TempRef->baseForm, TESForm, BGSKeywordForm) &&
				((BGSKeywordForm*)TempRef->baseForm)->HasKeyword((void*)Iter);
			if (hasKW)
			{
				bFoundExcluded = true;
				break;
			}
		}
	}
	else {
		//bFoundExcluded = true;
		int iCount = 0;

		for (auto it = KeywordToExclude.begin(); it != KeywordToExclude.end(); it++)
		{
			bool hasKW = TempRef->baseForm && DYNAMIC_CAST(TempRef->baseForm, TESForm, BGSKeywordForm) &&
				((BGSKeywordForm*)TempRef->baseForm)->HasKeyword((void*)Iter);
			if (!hasKW)
			{
				iCount += 1;
				continue;
			}
			else {
				break;
			}
		}

		if (iCount != iKeywordToExcludeSize)
		{
			bFoundExcluded = true;
		}

	}


	if (!bFoundExcluded)
	{
		result.Push(&TempRef);
	}

}



void f_AddObjectFromCellByKeyword(TESObjectCELL* Cell, VMArray<TESObjectREFR*>& result, vector<BGSKeyword*>& KeywordToInclude, vector<BGSKeyword*>& KeywordToExclude, bool bCheckForAnyIncluded, bool bCheckForAnyExcluded)
{

	TESObjectREFR* TempRef;


	int iKeywordToExcludeSize = KeywordToExclude.size();



	for (UInt64 i = 0; i < Cell->objectList.count; i++)
	{
		Cell->objectList.GetNthItem(i, TempRef);

		if (TempRef)
		{
			CheckRefForKeywordsAndAddToArray(result, TempRef, KeywordToInclude, KeywordToExclude, bCheckForAnyIncluded, bCheckForAnyExcluded, iKeywordToExcludeSize);

		}

	}
}



void FillKeywordsArrayByForm(vector<BGSKeyword*>& vArrToFill, TESForm* Keyword)
{
	if (Keyword->formType == kFormType_FLST)
	{

		BGSListForm* formList = (BGSListForm*)Keyword;

		for (UInt32 i = 0; i < formList->forms.count; i++)
		{
			TESForm* form = nullptr;
			formList->forms.GetNthItem(i, form);
			if (form && form->formType == kFormType_KYWD)
			{
				_DMESSAGE("Adding form %x to search list", form->formID);
				vArrToFill.push_back((BGSKeyword*)form);
			}

		}

	}
	else {
		if (Keyword->formType != kFormType_KYWD)
		{
			_DMESSAGE("Keyword is not a keyword type and not a formlist>>%d", Keyword->formType);
			return;
		}
		vArrToFill.push_back((BGSKeyword*)Keyword);
	}



}


bool f_ContainsOnlyASCII(const std::string& MyString) {

	for (auto c : MyString) {
		if (static_cast<unsigned char>(c) > 127) {
			return false;
		}
	}
	return true;
}



bool fn_IsSnappedConnection(TESObjectREFR* refr, TESObjectREFR* TargetRef)
{

	if (!refr)
		return false;

	NiNode* root = refr->GetObjectRootNode();
	if (!root)
		return false;

	BSConnectPoint::Parents* extraData = (BSConnectPoint::Parents*)Runtime_DynamicCast(root->GetExtraData("CPA"), RTTI_NiExtraData, RTTI_BSConnectPoint__Parents);
	if (extraData)
	{
		for (UInt32 i = 0; i < extraData->points.count; i++)
		{
			BSConnectPoint::Parents::ConnectPoint* connectPoint = extraData->points[i];
			if (connectPoint)
			{
				float yaw, pitch, roll;
				connectPoint->rot.GetEulerAngles(roll, pitch, yaw);
				yaw *= 180.0 / MATH_PI;
				pitch *= 180.0 / MATH_PI;
				roll *= 180.0 / MATH_PI;

				NiPoint3 localPos = connectPoint->pos;

				NiPoint3 worldPos = localPos;
				NiAVObject* parent = root;

				worldPos = parent->m_worldTransform.rot.Transpose() * localPos + parent->m_worldTransform.pos;


				if (refr->parentCell) {
					bhkWorld* world = CALL_MEMBER_FN(refr->parentCell, GetHavokWorld)();
					if (world) {
						TESObjectREFR* connected = GetObjectAtConnectPoint(*refr, worldPos, *world, 8.0f);
						if (connected && connected->formID == TargetRef->formID) {
							return true;
						}

					}
				}


			}
		}
	}

	return false;
}

void CheckAndRemoveBOM(std::ifstream& is)
{
	/* Read the first byte. */
	const char c0 = is.get();
	if (c0 != '\xEF') {
		is.putback(c0);
		return;
	}

	/* Read the second byte. */
	const char c1 = is.get();
	if (c1 != '\xBB') {
		is.putback(c1);
		is.putback(c0);
		return;
	}

	/* Peek the third byte. */
	const char c2 = is.peek();
	if (c2 != '\xBF') {
		is.putback(c1);
		is.putback(c0);
		return;
	}

	_DMESSAGE("file has UTF-8-BOM encoding - accounting.");
	is.ignore(1);
	return; // This file contains a BOM for UTF-8.
}