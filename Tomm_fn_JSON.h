


#define t_JSON_OpenFile_ParsedJSONNotStructured -14
#define t_JSON_OpenFile_NoSavePath -13
#define t_JSON_OpenFile_CantWriteToFile -12
#define t_JSON_OpenFile_PosInArrayOutOfRange -11
#define t_JSON_OpenFile_NotAnArray -10


#define t_JSON_OpenFile_WrongFileExtension -6
#define t_JSON_OpenFile_WrongDirectory -5
#define t_JSON_OpenFile_CantOpen -4
#define t_JSON_OpenFile_NotStructured -3
#define t_JSON_OpenFile_KeyNotFound -2
#define t_JSON_OpenFile_SuccessOtherType -1
#define t_JSON_OpenFile_SuccessNULL 0
#define t_JSON_OpenFile_SuccessObject 1
#define t_JSON_OpenFile_SuccessArray 2
#define t_JSON_OpenFile_SuccessString 3
#define t_JSON_OpenFile_SuccessBool 4
#define t_JSON_OpenFile_SuccessNumber 5


#define t_JSON_Param_Cache 1
#define t_JSON_Param_JSONString 2


class CachedJSON
{
public:

	nlohmann::json j;
	string sFilePath{};
	bool iHasSavePath = true;
};


vector<CachedJSON*> v_CachedJSONs{};


struct JSONOpenResult
{
	int iOpened = 0;
	nlohmann::json j;
	nlohmann::json* pointerj = NULL;
	string sFilePath{};
	int iCached = 0;
};


struct KeyFoundResult
{
	int iFoundResult = 0;
	nlohmann::json* jTempIter = NULL;
};






bool f_JSON_CHECK_OPENED_OKAY_TO_CONTINUE(int iOpened, int iAllowCorruptJSON)
{
	if (iOpened == t_JSON_OpenFile_WrongDirectory)
	{
		return false;
	}
	else if (iOpened == t_JSON_OpenFile_WrongFileExtension)
	{
		return false;
	}

	if (iAllowCorruptJSON == 0 && iOpened == t_JSON_OpenFile_NotStructured)
	{
		return false;
	}

	return true;
}





KeyFoundResult f_JSON_GetOnlyKey(vector<string> SplittedPath, JSONOpenResult* OpenResult, int iRoot = 0)
{
	_DMESSAGE("f_JSON_GetOnlyKey starts");

	KeyFoundResult result;

	nlohmann::json* jTempIter = NULL;

	if (OpenResult->pointerj)
	{
		_DMESSAGE("getting value from pointer");
		jTempIter = OpenResult->pointerj;
	}
	else {
		_DMESSAGE("getting value from stack");
		jTempIter = &OpenResult->j;
	}

	_DMESSAGE("JSON IS>>>%s", jTempIter->dump().c_str());

	if (iRoot)
	{
		_DMESSAGE("Length is zero - returning JSON root");
		result.jTempIter = jTempIter;
		return result;
	}


	if (SplittedPath.size() == 1)
	{
		_DMESSAGE("SplittedPath.at(0) is %s", SplittedPath.at(0).c_str());
		JSONContainsResult Result = f_JSON_iContains(jTempIter, SplittedPath.at(0));

		if (Result.bSuccess == true)
		{
			_DMESSAGE("JSON contains");
			result.jTempIter = &(*jTempIter)[Result.sOriginalKey];
			return result;
		}
		else {
			_DMESSAGE("JSON NOT contain");
			result.iFoundResult = t_JSON_OpenFile_KeyNotFound;
			return result;
		}

	}

	for (auto it = SplittedPath.begin(); it != SplittedPath.end(); it++)
	{
		_DMESSAGE("checking if JSON contains  %s", Iter.c_str());

		if (jTempIter->is_array() && is_string_number(Iter))
		{
			_DMESSAGE("Current JSON is an array and the key is number");
			int iPos = stoi(Iter);

			if (iPos >= jTempIter->size())
			{
				_DMESSAGE("Key not found in array - out of range");
				result.iFoundResult = t_JSON_OpenFile_PosInArrayOutOfRange;
				return result;
			}

			if (it != SplittedPath.end() && it == --SplittedPath.end())
			{
				result.jTempIter = &(*jTempIter).at(iPos);
				return result;
			}
			else {
				jTempIter = &jTempIter->at(iPos);
				continue;
			}

		}
		auto Result = f_JSON_iContains(jTempIter, Iter);

		if (Result.bSuccess == true)
		{
			if (it != SplittedPath.end() && it == --SplittedPath.end())
			{
				result.jTempIter = &(*jTempIter)[Result.sOriginalKey];
				return result;
			}
			else {
				jTempIter = &jTempIter->at(Result.sOriginalKey);
				continue;
			}
		}
		else {
			result.iFoundResult = t_JSON_OpenFile_KeyNotFound;
			return result;
		}

	}

}





KeyFoundResult f_JSON_GetOrCreateKey(vector<string> SplittedPath, JSONOpenResult* OpenResult, int iArray)
{
	_DMESSAGE("f_JSON_GetOrCreateKey started");
	KeyFoundResult result;
	nlohmann::json* jTempIter = NULL;

	if (OpenResult->pointerj)
	{
		_DMESSAGE("Using heap");
		jTempIter = OpenResult->pointerj;
	}
	else {
		_DMESSAGE("Using stack");
		jTempIter = &OpenResult->j;
	}

	_DMESSAGE("JSON IS >>>%s", jTempIter->dump().c_str());

	if (SplittedPath.size() == 1)
	{
		result.jTempIter = &(*jTempIter)[SplittedPath.at(0)];
		return result;
	}

	for (auto it = SplittedPath.begin(); it != SplittedPath.end(); it++)
	{
		_DMESSAGE("Checking if JSON contains  %s", Iter.c_str());



		if (jTempIter->is_array() && is_string_number(Iter))
		{
			_DMESSAGE("Current JSON is an array and the key is number");
			int iPos = stoi(Iter);

			if (iPos >= jTempIter->size())
			{
				_DMESSAGE("Key not found in array - out of range");
				result.iFoundResult = t_JSON_OpenFile_PosInArrayOutOfRange;
				return result;
			}

			if (it != SplittedPath.end() && it == --SplittedPath.end())
			{
				result.jTempIter = &(*jTempIter).at(iPos);
				return result;

			}
			else {
				jTempIter = &jTempIter->at(iPos);
				continue;
			}

		}

		auto Result = f_JSON_iContains(jTempIter, Iter);

		if (Result.bSuccess == true)
		{
			if (it != SplittedPath.end() && it == --SplittedPath.end())
			{
				result.jTempIter = &(*jTempIter)[Result.sOriginalKey];
				return result;
			}
			else {
				jTempIter = &jTempIter->at(Result.sOriginalKey);
				continue;
			}
		}
		else {
			_DMESSAGE("DOESNT CONTAIN");

			if (it != SplittedPath.end() && it == --SplittedPath.end())
			{
				_DMESSAGE("Last element 1");
				if (iArray)
				{
					_DMESSAGE("Last element 1.1");
					if ((*jTempIter).is_array() == 0 && (*jTempIter).is_primitive())
					{
						_DMESSAGE("Specified key cannot contain an array because it's not an array and is primitive");
						result.iFoundResult = t_JSON_OpenFile_NotAnArray;
						return result;
					}


					(*jTempIter)[Iter] = nlohmann::json::array();
				}

				_DMESSAGE("Last element 2");

				result.jTempIter = &(*jTempIter)[Iter];
				return result;
			}
			else {
				_DMESSAGE("NOT LAST ELEMENT 1");
				(*jTempIter)[Iter] = nlohmann::json({});
				_DMESSAGE("NOT LAST ELEMENT 1.1");
				jTempIter = &jTempIter->at(Iter);
				_DMESSAGE("NOT LAST ELEMENT 10");
				continue;
			}
		}

	}

}









int f_JSON_SaveJSON(int iCache, string s_LoweredFilePath, nlohmann::json jTemp, nlohmann::json* jTempPointer)
{
	if (iCache == 0)
	{
		f_Create_Folders_For_Path(s_LoweredFilePath.c_str());
		std::ofstream o(s_LoweredFilePath.c_str());
		if (o.is_open() == false)
		{
			_DMESSAGE("Can't write to specified path");
			return t_JSON_OpenFile_CantWriteToFile;
		}
		o << std::setw(4) << jTemp << std::endl;
	}
	else {
		//*(jTempPointer) = jTemp;
	}

	return 1;
}



JSONOpenResult f_JSON_OpenFile(string sFilePath, UInt32 iCache, int iWriting)
{
	nlohmann::json j;
	JSONOpenResult result;
	boost::replace_all(sFilePath, "/", "\\");

	if (iCache == t_JSON_Param_JSONString && iWriting == 0)
	{
		j = nlohmann::json::parse(sFilePath, 0, 0);

		if (!j.is_structured())
		{
			result.iOpened = t_JSON_OpenFile_NotStructured;
			_DMESSAGE("JSON is not structured");
			return result;
		}
		else {
			result.j = j;
			return result;
		}

	}
	else if (iCache == t_JSON_Param_Cache)
	{
		for (auto it = v_CachedJSONs.begin(); it != v_CachedJSONs.end(); it++)
		{
			if (Iter->sFilePath == sFilePath)
			{
				result.j = Iter->j;
				result.pointerj = &Iter->j;
				result.iCached = 1;
				return result;
			}
		}

		result.iOpened = t_JSON_OpenFile_CantOpen;
		_DMESSAGE("JSON not found on cached files");
		return result;
	}
	else
	{
		string s_TempPathRelative = sFilePath;

		string s_TempPathRelativeForCheck = s_TempPathRelative; // because we don't want replaced \-// to affect stored string.
		if (!IsFilePathValid(s_TempPathRelativeForCheck))
		{
			result.iOpened = t_JSON_OpenFile_WrongDirectory; 
			return result;
		}



		if (s_TempPathRelative.length() < 6)
		{
			result.iOpened = t_JSON_OpenFile_WrongFileExtension;
			_DMESSAGE("File can't be opened - length is too short");
			return result;
		}

		if (s_TempPathRelative.substr(s_TempPathRelative.size() - 5) != ".json")
		{
			result.iOpened = t_JSON_OpenFile_WrongFileExtension;
			_DMESSAGE("File can't be opened - extension is not JSON");
			return result;
		}

		std::ifstream t(sFilePath.c_str());

		if (!t.is_open())
		{
			result.iOpened = t_JSON_OpenFile_CantOpen;
			_DMESSAGE("File can't be opened");
			return result;
		}

		std::stringstream buffer;
		buffer << t.rdbuf();
		t.close();
		j = nlohmann::json::parse(buffer, 0, 0);

		if (!j.is_structured())
		{
			result.iOpened = t_JSON_OpenFile_NotStructured;
			_DMESSAGE("JSON is not structured");
			return result;
		}

		result.j = j;
		result.sFilePath = sFilePath;
		result.iOpened = 1;
		return result;
	}
}




UInt32 cmd_JSONIsFileCached(StaticFunctionTag* base, BSFixedString sFilePath)
{

	string sFilePathTemp = (string)sFilePath;
	boost::to_lower(sFilePathTemp);
	boost::replace_all(sFilePathTemp, "/", "\\");

	for (auto it = v_CachedJSONs.begin(); it != v_CachedJSONs.end(); it++)
	{
		if (Iter->sFilePath == sFilePathTemp)
		{
			_DMESSAGE("File with that name already exists");
			return 1;
		}
	}

	return 0;
}


__forceinline UInt32 JSONCacheFileFromString(string sFilePathTemp, string sJSONString)
{

	for (auto it = v_CachedJSONs.begin(); it != v_CachedJSONs.end(); it++)
	{
		if (Iter->sFilePath == sFilePathTemp)
		{
			_DMESSAGE("File with that name already exists");
			return -2;
		}
	}
	nlohmann::json j;

	j = nlohmann::json::parse(sJSONString, 0, 0);

	if (!j.is_structured())
	{
		_DMESSAGE("JSON is not structured");
		return t_JSON_OpenFile_NotStructured;
	}
	else {

		CachedJSON* NewJSON = new CachedJSON;
		NewJSON->j = j;
		NewJSON->iHasSavePath = false;
		NewJSON->sFilePath = sFilePathTemp;
		v_CachedJSONs.push_back(NewJSON);
		_DMESSAGE("Added JSON to cached files from string.");
		return 1;
	}

}


UInt32 cmd_JSONCacheFileFromString(StaticFunctionTag* base, BSFixedString sFilePath, BSFixedString sJSONString)
{
	string sFilePathTemp = sFilePath;
	boost::to_lower(sFilePathTemp);
	boost::replace_all(sFilePathTemp, "/", "\\");
	return JSONCacheFileFromString(sFilePathTemp, sJSONString.c_str());
}



UInt32 cmd_JSONCacheFile(StaticFunctionTag* base, BSFixedString sFilePath, UInt32 iCreateNew)
{
	string s_TempPathRelative = (string)sFilePath;
	boost::to_lower(s_TempPathRelative);
	boost::replace_all(s_TempPathRelative, "/", "\\");

	for (auto it = v_CachedJSONs.begin(); it != v_CachedJSONs.end(); it++)
	{
		if (Iter->sFilePath == s_TempPathRelative)
		{
			_DMESSAGE("File already opened");
			return 2;
		}
	}

	if (!IsFilePathValid(s_TempPathRelative))
	{
		return t_JSON_OpenFile_WrongDirectory;
	}



	nlohmann::json j;

	if (iCreateNew == 1)
	{
		CachedJSON* NewJSON = new CachedJSON;
		NewJSON->j = j;
		NewJSON->sFilePath = s_TempPathRelative;
		v_CachedJSONs.push_back(NewJSON);
		_DMESSAGE("Created cached JSON named >>>%s", sFilePath.c_str());
		return 1;
	}
	else {

		if (s_TempPathRelative.length() < 6)
		{
			_DMESSAGE("File can't be opened - length is too short");
			return t_JSON_OpenFile_WrongFileExtension;
		}

		if (s_TempPathRelative.substr(s_TempPathRelative.size() - 5) != ".json")
		{
			_DMESSAGE("File can't be opened - extension is not JSON");
			return t_JSON_OpenFile_WrongFileExtension;
		}


		std::ifstream t(sFilePath.c_str());

		if (!t.is_open())
		{
			_DMESSAGE("File can't be opened"); 
			return t_JSON_OpenFile_CantOpen;
		}

		std::stringstream buffer;
		buffer << t.rdbuf();
		t.close();
		j = nlohmann::json::parse(buffer, 0, 0);

		if (!j.is_structured())
		{
			_DMESSAGE("JSON is not structured");
			return t_JSON_OpenFile_NotStructured;
		}


		CachedJSON* NewJSON = new CachedJSON;
		NewJSON->j = j;
		NewJSON->sFilePath = s_TempPathRelative;
		v_CachedJSONs.push_back(NewJSON);
		_DMESSAGE("Added JSON to cached files>>>%s", sFilePath.c_str());
		return 1;
	}

}

UInt32 cmd_JSONCloseFile(StaticFunctionTag* base, BSFixedString sFilePath, UInt32 iSave, BSFixedString sFilePathToSave)
{
	_DMESSAGE("JSONCloseFile started");
	string sFilePathTemp = sFilePath;
	boost::to_lower(sFilePathTemp);
	boost::replace_all(sFilePathTemp, "/", "\\");


	string sFilePathToSaveTemp = sFilePathToSave;

	if (sFilePathToSaveTemp.length() > 0)
	{
		if (!IsFilePathValid(sFilePathToSaveTemp))
		{
			return t_JSON_OpenFile_WrongDirectory;
		}


		if (sFilePathToSaveTemp.length() < 6)
		{
			_DMESSAGE("File can't be saved - length is too short");
			return t_JSON_OpenFile_WrongFileExtension;
		}

		if (sFilePathToSaveTemp.substr(sFilePathToSaveTemp.size() - 5) != ".json")
		{
			_DMESSAGE("File can't be saved - extension is not JSON");
			return t_JSON_OpenFile_WrongFileExtension;
		}


	}




	for (auto it = v_CachedJSONs.begin(); it != v_CachedJSONs.end(); it++)
	{
		_DMESSAGE("Comparing %s with %s", Iter->sFilePath.c_str(), sFilePathTemp.c_str());

		if (Iter->sFilePath == sFilePathTemp)
		{
			_DMESSAGE("File found");
			if (iSave)
			{
				if (sFilePathToSaveTemp.length() > 0)
				{
					_DMESSAGE("Saving to specified path>>%s", sFilePathToSaveTemp.c_str());
					f_Create_Folders_For_Path(sFilePathToSaveTemp);
					std::ofstream o(sFilePathToSaveTemp);
					if (o.is_open() == false)
					{
						_DMESSAGE("Can't write to specified path");
						return t_JSON_OpenFile_CantWriteToFile;
					}

					o << std::setw(4) << Iter->j << std::endl;
				}
				else {

					if (Iter->iHasSavePath == false)
					{
						_DMESSAGE("JSON doesn't have specified PATH - it was created from string. ");
						return t_JSON_OpenFile_NoSavePath;
					}

					_DMESSAGE("Saving to original path>>%s", Iter->sFilePath.c_str());
					f_Create_Folders_For_Path(Iter->sFilePath);
					std::ofstream o(Iter->sFilePath);
					if (o.is_open() == false)
					{
						_DMESSAGE("Can't write to specified path");
						return t_JSON_OpenFile_CantWriteToFile;
					}
					o << std::setw(4) << Iter->j << std::endl;
				}

			}

			Iter->j.clear();
			delete Iter;
			v_CachedJSONs.erase(it);
			_DMESSAGE("Removed cached JSON");
			return 1;
		}
	}

	return -2;
}




UInt32 f_JSON_VALUE_GET_TYPE_FROM_JSON(nlohmann::json* jTemp)
{

	if (jTemp->is_number())
	{
		_DMESSAGE("JSON VALUE is number - setting");
		return t_JSON_OpenFile_SuccessNumber;
	}
	else if (jTemp->is_string())
	{
		_DMESSAGE("JSON VALUE is string - setting");
		return t_JSON_OpenFile_SuccessString;
	}
	else if (jTemp->is_object())
	{
		return t_JSON_OpenFile_SuccessObject;
	}
	else if (jTemp->is_array())
	{
		return t_JSON_OpenFile_SuccessArray;
	}
	else if (jTemp->is_null())
	{
		return t_JSON_OpenFile_SuccessNULL;
	}
	else if (jTemp->is_boolean())
	{
		return t_JSON_OpenFile_SuccessBool;
	}
	else
	{
		return t_JSON_OpenFile_SuccessOtherType;
	}




}




void f_JSON_VALUE_SET_FROM_JSON_ARRAY_OR_OBJECT(nlohmann::detail::iter_impl<nlohmann::json> it2, VMArray<JSONValue>* result, int iAddKeys)
{
	JSONValue resultValueTemp;
	if (Iter2.is_number())
	{
		_DMESSAGE("Iter2.is_number()1");
		resultValueTemp.Set<UInt32>("JSONSuccess", t_JSON_OpenFile_SuccessNumber);
		_DMESSAGE("Iter2.is_number()2");
		resultValueTemp.Set<float>("JSONfValue", Iter2.get<float>());

		if (iAddKeys)
		{
			resultValueTemp.Set<BSFixedString>("JSONkey", it2.key().c_str());
		}


		_DMESSAGE("Iter2.is_number()2.1");
		result->Push(&resultValueTemp);
	}
	else if (Iter2.is_string())
	{
		_DMESSAGE("Iter2.is_string()1");
		resultValueTemp.Set<UInt32>("JSONSuccess", t_JSON_OpenFile_SuccessString);
		_DMESSAGE("Iter2.is_string()2");
		resultValueTemp.Set<BSFixedString>("JSONsValue", Iter2.get<string>().c_str());
		_DMESSAGE("Iter2.is_string()3");
		if (iAddKeys)
		{
			resultValueTemp.Set<BSFixedString>("JSONkey", it2.key().c_str());
		}
		_DMESSAGE("Iter2.is_string()4");
		result->Push(&resultValueTemp);
	}
	else if (Iter2.is_object())
	{
		resultValueTemp.Set<UInt32>("JSONSuccess", t_JSON_OpenFile_SuccessObject);
		if (iAddKeys)
		{
			resultValueTemp.Set<BSFixedString>("JSONkey", it2.key().c_str());
		}
		result->Push(&resultValueTemp);
	}
	else if (Iter2.is_array())
	{

		resultValueTemp.Set<UInt32>("JSONSuccess", t_JSON_OpenFile_SuccessArray);
		if (iAddKeys)
		{
			resultValueTemp.Set<BSFixedString>("JSONkey", it2.key().c_str());
		}
		result->Push(&resultValueTemp);
	}
	else if (Iter2.is_null())
	{
		resultValueTemp.Set<UInt32>("JSONSuccess", t_JSON_OpenFile_SuccessNULL);
		if (iAddKeys)
		{
			resultValueTemp.Set<BSFixedString>("JSONkey", it2.key().c_str());
		}
		result->Push(&resultValueTemp);
	}
	else if (Iter2.is_boolean())
	{
		resultValueTemp.Set<UInt32>("JSONSuccess", t_JSON_OpenFile_SuccessBool);
		resultValueTemp.Set<float>("JSONfValue", Iter2.get<bool>());
		if (iAddKeys)
		{
			resultValueTemp.Set<BSFixedString>("JSONkey", it2.key().c_str());
		}
		result->Push(&resultValueTemp);
	}
	else {
		resultValueTemp.Set<UInt32>("JSONSuccess", t_JSON_OpenFile_SuccessOtherType);
		if (iAddKeys)
		{
			resultValueTemp.Set<BSFixedString>("JSONkey", it2.key().c_str());
		}
		result->Push(&resultValueTemp);
	}

}










JSONValue f_JSON_VALUE_SET_FROM_JSON(nlohmann::json* jTemp)
{
	JSONValue result;

	if (jTemp->is_number())
	{
		_DMESSAGE("JSON VALUE is number - setting");
		result.Set<UInt32>("JSONSuccess", t_JSON_OpenFile_SuccessNumber);
		result.Set<float>("JSONfValue", jTemp->get<float>());
		return result;
	}
	else if (jTemp->is_string())
	{
		_DMESSAGE("JSON VALUE is string - setting");
		result.Set<UInt32>("JSONSuccess", t_JSON_OpenFile_SuccessString);
		result.Set<BSFixedString>("JSONsValue", jTemp->get<string>().c_str());
		_DMESSAGE("JSON VALUE is string - setting DONE");
		return result;
	}
	else if (jTemp->is_object())
	{
		_DMESSAGE("JSON VALUE is object");
		result.Set<UInt32>("JSONSuccess", t_JSON_OpenFile_SuccessObject);
		return result;
	}
	else if (jTemp->is_array())
	{
		_DMESSAGE("JSON VALUE is array");
		result.Set<UInt32>("JSONSuccess", t_JSON_OpenFile_SuccessArray);
		return result;
	}
	else if (jTemp->is_null())
	{
		_DMESSAGE("JSON VALUE is null");
		result.Set<UInt32>("JSONSuccess", t_JSON_OpenFile_SuccessNULL);
		return result;
	}
	else if (jTemp->is_boolean())
	{
		_DMESSAGE("JSON VALUE is bool");
		result.Set<UInt32>("JSONSuccess", t_JSON_OpenFile_SuccessBool);
		result.Set<float>("JSONfValue", jTemp->get<bool>());
		return result;
	}
	else
	{
		_DMESSAGE("JSON VALUE is other type");
		result.Set<UInt32>("JSONSuccess", t_JSON_OpenFile_SuccessOtherType);
		return result;
	}


}






JSONValue cmd_JSONGetValue(StaticFunctionTag* base, BSFixedString sFilePath, BSFixedString key, UInt32 iCache)
{
	string s_LoweredKey = key;
	boost::to_lower(s_LoweredKey);

	string s_LoweredFilePath = sFilePath;
	boost::to_lower(s_LoweredFilePath);

	_DMESSAGE("JSONGetValue starts,path is %s, key is %s", s_LoweredFilePath.c_str(), s_LoweredKey.c_str());
	JSONValue result;

	JSONOpenResult OpenResult = f_JSON_OpenFile(s_LoweredFilePath.c_str(), iCache,0);

	if (OpenResult.iOpened < 0)
	{
		result.Set<UInt32>("JSONSuccess", OpenResult.iOpened);
		_DMESSAGE("JSONGetValue file not opened");
		return result;
	}


	std::vector<std::string> SplittedPath = f_Split_String(s_LoweredKey,"\\");
	_DMESSAGE("key is %s", s_LoweredKey.c_str());

	KeyFoundResult FoundKey = f_JSON_GetOnlyKey(SplittedPath, &OpenResult,0);

	if (FoundKey.iFoundResult < 0)
	{
		_DMESSAGE("iFoundResult>>%d", FoundKey.iFoundResult);
		result.Set<UInt32>("JSONSuccess", FoundKey.iFoundResult);
		return result;
	}


	_DMESSAGE("Type of value is %d", FoundKey.jTempIter->type());

	return f_JSON_VALUE_SET_FROM_JSON(FoundKey.jTempIter);
}



VMArray<JSONValue> cmd_JSONGetValueArray(StaticFunctionTag* base, BSFixedString sFilePath, BSFixedString key, UInt32 iCache)
{
	_DMESSAGE("JSONGetValueArray starts, iCache is %d", iCache);
	VMArray<JSONValue> result;
	JSONValue resultValue;

	string s_LoweredFilePath = sFilePath;
	boost::to_lower(s_LoweredFilePath);


	JSONOpenResult OpenResult = f_JSON_OpenFile(s_LoweredFilePath.c_str(), iCache,0);

	if (OpenResult.iOpened < 0)
	{
		resultValue.Set<UInt32>("JSONSuccess", OpenResult.iOpened);
		result.Push(&resultValue);
		_DMESSAGE("JSONGetValueArray file not opened");
		return result;
	}

	string sKeyTemp = key;
	boost::to_lower(sKeyTemp);
	std::vector<std::string> SplittedPath = f_Split_String(sKeyTemp.c_str(), "\\");
	_DMESSAGE("key is %s", sKeyTemp.c_str());

	int iRoot = 0;
	if (sKeyTemp.length() == 0) 
	{
		iRoot = 1;
	}

	KeyFoundResult FoundKey = f_JSON_GetOnlyKey(SplittedPath, &OpenResult, iRoot);

	if (FoundKey.iFoundResult < 0)
	{
		_DMESSAGE("iFoundResult>>%d", FoundKey.iFoundResult);
		resultValue.Set<UInt32>("JSONSuccess", FoundKey.iFoundResult);
		result.Push(&resultValue);
		return result;
	}


	_DMESSAGE("jTempIter is valid.");

	if (FoundKey.jTempIter->is_number())
	{
		_DMESSAGE("JSON VALUE is number - setting");
		resultValue.Set<UInt32>("JSONSuccess", t_JSON_OpenFile_SuccessNumber);
		resultValue.Set<float>("JSONfValue", FoundKey.jTempIter->get<float>());
		result.Push(&resultValue);
		return result;
	}
	else if (FoundKey.jTempIter->is_string())
	{
		_DMESSAGE("JSON VALUE is string - setting");
		resultValue.Set<UInt32>("JSONSuccess", t_JSON_OpenFile_SuccessString);
		resultValue.Set<BSFixedString>("JSONsValue", FoundKey.jTempIter->get<string>().c_str());
		result.Push(&resultValue);
		_DMESSAGE("JSON VALUE is string - setting DONE");
		return result;
	}
	else if (FoundKey.jTempIter->is_object())
	{
		_DMESSAGE("jTemp.at(Iter).is_object()");

		for (auto it2 = FoundKey.jTempIter->begin(); it2 != FoundKey.jTempIter->end(); ++it2)
		{
			_DMESSAGE("New iteration");
			f_JSON_VALUE_SET_FROM_JSON_ARRAY_OR_OBJECT(it2, &result, 1);
		}

		return result;

	}
	else if (FoundKey.jTempIter->is_array())
	{
		_DMESSAGE("jTemp.at(Iter).is_array()");
		for (auto it2 = FoundKey.jTempIter->begin(); it2 != FoundKey.jTempIter->end(); ++it2)
		{
			_DMESSAGE("New iteration");
			f_JSON_VALUE_SET_FROM_JSON_ARRAY_OR_OBJECT(it2, &result,0);
		}
		return result;
	}



}




UInt32 cmd_JSONGetValueType(StaticFunctionTag* base, BSFixedString sFilePath, BSFixedString key, UInt32 iCache)
{
	_DMESSAGE("JSONGetValueType starts");

	string s_LoweredFilePath = sFilePath;
	boost::to_lower(s_LoweredFilePath);

	JSONOpenResult OpenResult = f_JSON_OpenFile(s_LoweredFilePath.c_str(), iCache,0);

	if (OpenResult.iOpened < 0)
	{
		_DMESSAGE("JSONGetValue file not opened");
		return OpenResult.iOpened;
	}
	string s_LoweredKey = key;
	boost::to_lower(s_LoweredKey);

	std::vector<std::string> SplittedPath = f_Split_String(s_LoweredKey.c_str(), "\\");
	_DMESSAGE("key is %s", s_LoweredKey.c_str());


	KeyFoundResult FoundKey = f_JSON_GetOnlyKey(SplittedPath, &OpenResult,0);

	if (FoundKey.iFoundResult < 0)
	{
		_DMESSAGE("iFoundResult>>%d", FoundKey.iFoundResult);
		return FoundKey.iFoundResult;
	}

	return f_JSON_VALUE_GET_TYPE_FROM_JSON(FoundKey.jTempIter);
}



UInt32 cmd_JSONSetValueFloat(StaticFunctionTag* base, BSFixedString sFilePath, BSFixedString key, float Value, UInt32 iCache, UInt32 iValueIsBool)
{
	_DMESSAGE("JSONSetValueFloat starts");

	string s_LoweredFilePath = sFilePath;
	boost::to_lower(s_LoweredFilePath);

	JSONOpenResult OpenResult = f_JSON_OpenFile(s_LoweredFilePath.c_str(), iCache, 1);

	if (OpenResult.iOpened < 0)
	{
		if (iCache == 0 && f_JSON_CHECK_OPENED_OKAY_TO_CONTINUE(OpenResult.iOpened,0))
		{
			_DMESSAGE("JSONGetValue file not opened - should create");
		}
		else {
			_DMESSAGE("JSONGetValue file not opened");
			return OpenResult.iOpened;
		}

	}

	string s_LoweredKey = key;
	boost::to_lower(s_LoweredKey);

	std::vector<std::string> SplittedPath = f_Split_String(s_LoweredKey.c_str(), "\\");
	_DMESSAGE("key is %s", s_LoweredKey.c_str());



	KeyFoundResult FoundKey = f_JSON_GetOrCreateKey(SplittedPath, &OpenResult,0);

	if (FoundKey.iFoundResult < 0)
	{
		_DMESSAGE("iFoundResult>>%d", FoundKey.iFoundResult);
		return FoundKey.iFoundResult;
	}



	if (iValueIsBool)
	{
		if (Value)
		{
			(*FoundKey.jTempIter) = true;
		}
		else {
			(*FoundKey.jTempIter) = false;
		}

	}
	else {
		(*FoundKey.jTempIter) = Value;
	}

	return f_JSON_SaveJSON(iCache, s_LoweredFilePath, OpenResult.j, OpenResult.pointerj);
}


UInt32 cmd_JSONSetValueString(StaticFunctionTag* base, BSFixedString sFilePath, BSFixedString key, BSFixedString Value, UInt32 iCache)
{
	_DMESSAGE("JSONSetValueString starts");

	string s_LoweredFilePath = sFilePath;
	boost::to_lower(s_LoweredFilePath);


	JSONOpenResult OpenResult = f_JSON_OpenFile(s_LoweredFilePath.c_str(), iCache, 1);

	if (OpenResult.iOpened < 0)
	{
		if (iCache == 0 && f_JSON_CHECK_OPENED_OKAY_TO_CONTINUE(OpenResult.iOpened,0))
		{
			_DMESSAGE("JSONGetValue file not opened - should create");
		}
		else {
			_DMESSAGE("JSONGetValue file not opened");
			return OpenResult.iOpened;
		}

	}

	string s_LoweredKey = key;
	boost::to_lower(s_LoweredKey);


	std::vector<std::string> SplittedPath = f_Split_String(s_LoweredKey.c_str(), "\\");
	_DMESSAGE("key is %s", s_LoweredKey.c_str());

	KeyFoundResult FoundKey = f_JSON_GetOrCreateKey(SplittedPath, &OpenResult,0);

	if (FoundKey.iFoundResult < 0)
	{
		_DMESSAGE("iFoundResult>>%d", FoundKey.iFoundResult);
		return FoundKey.iFoundResult;
	}


	(*FoundKey.jTempIter) = Value.c_str();
	return f_JSON_SaveJSON(iCache, s_LoweredFilePath, OpenResult.j, OpenResult.pointerj);
}




UInt32 cmd_JSONAppendValueFloat(StaticFunctionTag* base, BSFixedString sFilePath, BSFixedString key, float Value, UInt32 iCache, UInt32 iValueIsBool)
{
	_DMESSAGE("JSONAppendValueFloat starts, iCache is %d", iCache);
	string s_LoweredKey = key;
	boost::to_lower(s_LoweredKey);

	string s_LoweredFilePath = sFilePath;
	boost::to_lower(s_LoweredFilePath);

	_DMESSAGE("JSONAppendValueFloat starts, filepath>>%s, key>>%s, value>>%f", s_LoweredFilePath.c_str(), s_LoweredKey.c_str(), Value);

	JSONOpenResult OpenResult = f_JSON_OpenFile(s_LoweredFilePath.c_str(), iCache, 1);

	if (OpenResult.iOpened < 0)
	{
		if (iCache == 0 && f_JSON_CHECK_OPENED_OKAY_TO_CONTINUE(OpenResult.iOpened,0))
		{
			_DMESSAGE("JSONGetValue file not opened - should create");
		}
		else {
			_DMESSAGE("JSONGetValue file not opened");
			return OpenResult.iOpened;
		}

	}
	std::vector<std::string> SplittedPath = f_Split_String(s_LoweredKey.c_str(), "\\");
	_DMESSAGE("key is %s", s_LoweredKey.c_str());


	KeyFoundResult FoundKey = f_JSON_GetOrCreateKey(SplittedPath, &OpenResult,1);

	if (FoundKey.iFoundResult < 0)
	{
		_DMESSAGE("iFoundResult>>%d", FoundKey.iFoundResult);
		return FoundKey.iFoundResult;
	}


	if ((*FoundKey.jTempIter).is_array() == false)
	{
		_DMESSAGE("Key is not an array>>%s", key.c_str());
		return t_JSON_OpenFile_NotAnArray;
	}
	_DMESSAGE("2");

	if (iValueIsBool)
	{
		if (Value)
		{
			(*FoundKey.jTempIter).push_back(true);
		}
		else {
			(*FoundKey.jTempIter).push_back(false);
		}
	}
	else {
		(*FoundKey.jTempIter).push_back(Value);
	}

	_DMESSAGE("3");
	return f_JSON_SaveJSON(iCache, s_LoweredFilePath, OpenResult.j, OpenResult.pointerj);

}




UInt32 cmd_JSONAppendValueString(StaticFunctionTag* base, BSFixedString sFilePath, BSFixedString key, BSFixedString Value, UInt32 iCache, UInt32 iValueisObject)
{
	_DMESSAGE("JSONAppendValueFloat starts");


	
	nlohmann::json jObjectValue;
	if (iValueisObject)
	{
		jObjectValue = nlohmann::json::parse(Value.c_str(), 0, 0);

		if (!jObjectValue.is_structured())
		{
			_DMESSAGE("JSON object is not structured");
			return t_JSON_OpenFile_ParsedJSONNotStructured;
		}

	}

	string s_LoweredFilePath = sFilePath;
	boost::to_lower(s_LoweredFilePath);

	JSONOpenResult OpenResult = f_JSON_OpenFile(s_LoweredFilePath.c_str(), iCache, 1);

	if (OpenResult.iOpened < 0)
	{
		if (iCache == 0 && f_JSON_CHECK_OPENED_OKAY_TO_CONTINUE(OpenResult.iOpened,0))
		{
			_DMESSAGE("JSONGetValue file not opened - should create");
		}
		else {
			_DMESSAGE("JSONGetValue file not opened");
			return OpenResult.iOpened;
		}

	}

	string s_LoweredKey = key;
	boost::to_lower(s_LoweredKey);


	std::vector<std::string> SplittedPath = f_Split_String(s_LoweredKey.c_str(), "\\");
	_DMESSAGE("key is %s", s_LoweredKey.c_str());


	KeyFoundResult FoundKey = f_JSON_GetOrCreateKey(SplittedPath, &OpenResult,1);


	if (FoundKey.iFoundResult < 0)
	{
		_DMESSAGE("iFoundResult>>%d", FoundKey.iFoundResult);
		return FoundKey.iFoundResult;
	}

	if ((*FoundKey.jTempIter).is_array() == false)
	{
		_DMESSAGE("Key is not an array>>%s", key.c_str());
		return t_JSON_OpenFile_NotAnArray;
	}

	if (iValueisObject)
	{
		(*FoundKey.jTempIter).push_back(jObjectValue);
	}
	else {
		(*FoundKey.jTempIter).push_back(Value.c_str());
	}

	return f_JSON_SaveJSON(iCache, s_LoweredFilePath, OpenResult.j, OpenResult.pointerj);
}





UInt32 cmd_JSONClearKey(StaticFunctionTag* base, BSFixedString sFilePath, BSFixedString key, UInt32 iCache)
{

	string s_LoweredKey = key;
	boost::to_lower(s_LoweredKey);


	_DMESSAGE("ClearKey starts>>>%s", s_LoweredKey.c_str());

	string s_LoweredFilePath = sFilePath;
	boost::to_lower(s_LoweredFilePath);
	JSONOpenResult OpenResult = f_JSON_OpenFile(s_LoweredFilePath.c_str(), iCache, 1);

	if (OpenResult.iOpened < 0)
	{
		if (iCache == 0 && f_JSON_CHECK_OPENED_OKAY_TO_CONTINUE(OpenResult.iOpened,0))
		{
			_DMESSAGE("JSONGetValue file not opened - should create");
		}
		else {
			_DMESSAGE("JSONGetValue file not opened");
			return OpenResult.iOpened;
		}

	}
	std::vector<std::string> SplittedPath = f_Split_String(s_LoweredKey.c_str(), "\\");
	_DMESSAGE("key is %s", s_LoweredKey.c_str());



	KeyFoundResult FoundKey = f_JSON_GetOnlyKey(SplittedPath, &OpenResult,0);

	if (FoundKey.iFoundResult < 0)
	{
		_DMESSAGE("iFoundResult>>%d", FoundKey.iFoundResult);
		return FoundKey.iFoundResult;
	}

	(*FoundKey.jTempIter).clear();
	return f_JSON_SaveJSON(iCache, s_LoweredFilePath, OpenResult.j, OpenResult.pointerj);
}




UInt32 cmd_JSONEraseKey(StaticFunctionTag* base, BSFixedString sFilePath, BSFixedString key, UInt32 iCache)
{
	string s_LoweredKey = key;
	boost::to_lower(s_LoweredKey);

	_DMESSAGE("JSONEraseKey starts>>>%s", s_LoweredKey.c_str());

	string s_LoweredFilePath = sFilePath;
	boost::to_lower(s_LoweredFilePath);
	JSONOpenResult OpenResult = f_JSON_OpenFile(s_LoweredFilePath.c_str(), iCache, 1);

	if (OpenResult.iOpened < 0)
	{
		if (iCache == 0 && f_JSON_CHECK_OPENED_OKAY_TO_CONTINUE(OpenResult.iOpened,1))
		{
			_DMESSAGE("JSONErase file not opened - should create");
		}
		else {
			_DMESSAGE("JSONErase file not opened");
			return OpenResult.iOpened;
		}

	}


	
	nlohmann::json* jTempIter = &OpenResult.j;

	string sKeyTemp = key;
	if (sKeyTemp.length() == 0)
	{
		OpenResult.j.clear();

		if (OpenResult.pointerj)
		{
			OpenResult.pointerj->clear();
		}

		return f_JSON_SaveJSON(iCache, s_LoweredFilePath, OpenResult.j, OpenResult.pointerj);
	}

	if (OpenResult.iOpened == t_JSON_OpenFile_NotStructured)
	{
		_DMESSAGE("JSON is not structured - cannot erase specified key.");
		return t_JSON_OpenFile_NotStructured;
	}


	std::vector<std::string> SplittedPath = f_Split_String(s_LoweredKey.c_str(), "\\");
	_DMESSAGE("key is %s", s_LoweredKey.c_str());


	KeyFoundResult FoundKey = f_JSON_GetOnlyKey(SplittedPath, &OpenResult,0);

	if (FoundKey.iFoundResult < 0)
	{
		_DMESSAGE("iFoundResult>>%d", FoundKey.iFoundResult);
		return FoundKey.iFoundResult;
	}

	jTempIter = FoundKey.jTempIter;
	(*jTempIter).erase(SplittedPath.back());
	return f_JSON_SaveJSON(iCache, s_LoweredFilePath, OpenResult.j, OpenResult.pointerj);
}




BSFixedString cmd_JSONToString(StaticFunctionTag* base, BSFixedString sFilePath)
{
	_DMESSAGE("JSONToString started");
	string sFilePathTemp = (string)sFilePath;
	boost::to_lower(sFilePathTemp);
	boost::replace_all(sFilePathTemp, "/", "\\");

	for (auto it = v_CachedJSONs.begin(); it != v_CachedJSONs.end(); it++)
	{
		if (Iter->sFilePath == sFilePathTemp)
		{
			_DMESSAGE("Found file -returning string>>>%s", Iter->j.dump().c_str());
			return Iter->j.dump().c_str();
		}
	}

	return "";
}





UInt32 cmd_JSONEraseArrayElement(StaticFunctionTag* base, BSFixedString sFilePath, BSFixedString key, UInt32 iPos,  UInt32 iCache)
{

	string s_LoweredKey = key;
	boost::to_lower(s_LoweredKey);
	string s_LoweredFilePath = sFilePath;
	boost::to_lower(s_LoweredFilePath);
	_DMESSAGE("JSONEraseArrayElement starts, filepath>>%s, key>>%s, startPos>>%d", s_LoweredFilePath.c_str(), s_LoweredKey.c_str(), iPos);

	JSONOpenResult OpenResult = f_JSON_OpenFile(s_LoweredFilePath.c_str(), iCache, 1);

	if (OpenResult.iOpened < 0)
	{
		if (iCache == 0 && f_JSON_CHECK_OPENED_OKAY_TO_CONTINUE(OpenResult.iOpened,0))
		{
			_DMESSAGE("JSONEraseArrayElement file not opened - should create");
		}
		else {
			_DMESSAGE("JSONEraseArrayElement file not opened");
			return OpenResult.iOpened;
		}

	}
	std::vector<std::string> SplittedPath = f_Split_String(s_LoweredKey.c_str(), "\\");
	_DMESSAGE("key is %s", s_LoweredKey.c_str());


	KeyFoundResult FoundKey = f_JSON_GetOnlyKey(SplittedPath, &OpenResult,0);

	if (FoundKey.iFoundResult < 0)
	{
		_DMESSAGE("iFoundResult>>%d", FoundKey.iFoundResult);
		return FoundKey.iFoundResult;
	}


	if (iPos > (*FoundKey.jTempIter).size() - 1)
	{
		_DMESSAGE("Erase position it our of range");
		return t_JSON_OpenFile_PosInArrayOutOfRange;
	}

	(*FoundKey.jTempIter).erase(iPos);
	return f_JSON_SaveJSON(iCache, s_LoweredFilePath, OpenResult.j, OpenResult.pointerj);
}




UInt32 cmd_JSONIsFileValid(StaticFunctionTag* base, BSFixedString sFilePath)
{

	string s_LoweredFilePath = sFilePath;
	boost::to_lower(s_LoweredFilePath);

	_DMESSAGE("JSONIsJSONFileValid starts,path is %s, key is %s", s_LoweredFilePath.c_str());

	JSONOpenResult OpenResult = f_JSON_OpenFile(s_LoweredFilePath.c_str(), 0, 0);

	return OpenResult.iOpened;
}