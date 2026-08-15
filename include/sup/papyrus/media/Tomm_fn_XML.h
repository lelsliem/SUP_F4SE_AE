#include "sup/util/string.hpp"


#include "third_party/xml2json.hpp"

BSFixedString cmd_XMLToJSONString(StaticFunctionTag* base, BSFixedString FilePath, bool bAddPrefix)
{
	_DMESSAGE("XMLToJSONString started with request of>> %s", FilePath.c_str());

	string s_TempPathRelative = FilePath.c_str();
	if (!IsFilePathValid(s_TempPathRelative))
	{
		return "";
	}

	ifstream aInputFile(s_TempPathRelative);
	CheckAndRemoveBOM(aInputFile);
	std::stringstream buffer;
	buffer << aInputFile.rdbuf();
	const auto json_str = xml2json(buffer.str().c_str(), bAddPrefix);
	return json_str.c_str();
}



UInt32 cmd_XMLToJSONCachedFile(StaticFunctionTag* base, BSFixedString FilePath, bool bAddPrefix, BSFixedString JSONFilePath)
{
	_DMESSAGE("XMLToJSONCachedFile started with request of>> %s", FilePath.c_str());

	string s_TempPathRelative = FilePath.c_str();

	if (!IsFilePathValid(s_TempPathRelative))
	{
		return t_JSON_OpenFile_WrongDirectory;
	}


	ifstream aInputFile(s_TempPathRelative);
	CheckAndRemoveBOM(aInputFile);
	std::stringstream buffer;
	buffer << aInputFile.rdbuf();
	const auto json_str = xml2json(buffer.str().c_str(), bAddPrefix);


	string sFilePathTemp = JSONFilePath.c_str();
	sup::string::to_lower(sFilePathTemp);
	sup::string::replace_all(sFilePathTemp, "/", "\\");
	return JSONCacheFileFromString(sFilePathTemp, json_str);

}
