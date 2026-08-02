




class MyThread {
public:
	std::atomic<int> stage = 0;
	std::atomic<HINTERNET> hData = 0;
	plf::nanotimer timer;

	int CheckType = 0;

	void Execute()
	{
		_DMESSAGE("Thread starts");


		while (stage < 100)
		{

			if (stage == 5)
			{
				timer.start();
				_DMESSAGE("Reset timer");
				stage = 10;
			}
			else if (stage == 10)
			{
				if (timer.get_elapsed_ms() > 2999)
				{
					_DMESSAGE("Timer elapsed");
					InternetCloseHandle(hData);
					stage = 120;
				}

			}

		}

		_DMESSAGE("Thread finishes, stage is %d", (int)stage);
	}
};







void f_UpdateChecker_PopulateCachedArrayFromSUPDater()
{
	_DMESSAGE("f_UpdateChecker_PopulateCachedArrayFromSUPDater() starts");
	iPluginsCached = 1;
	vector<PassedCatchedMod> g_OnlineMods = f_SUPDater_PassCachedMods();

	int iFound = 0;
	CachedPlugin* NewPlugin = NULL;



	_DMESSAGE("g_OnlineMods array count is %d", g_OnlineMods.size());

	for (auto it = g_OnlineMods.begin(); it != g_OnlineMods.end(); it++)
	{
		iFound = 0;

		for (auto it2 = g_CachedPlugins.begin(); it2 != g_CachedPlugins.end(); it2++)
		{
			if (Iter2->CheckType == Iter.CheckType)
			{
				if (Iter.CheckType == t_CheckType_Nexus)
				{
					if (Iter.NexusID == Iter2->NexusID)
					{
						iFound = 1;
						_DMESSAGE("Found in existing array");
					}
				}
				else if (Iter.CheckType == t_CheckType_GitHub)
				{
					if (Iter.GitHubPath == Iter2->GitHubPath)
					{
						iFound = 1;
						_DMESSAGE("Found in existing array");
					}

				}

			}

		}

		if (iFound)
		{
			continue;
		}



		NewPlugin = new CachedPlugin;
		NewPlugin->CheckType = Iter.CheckType;

		if (NewPlugin->CheckType == t_CheckType_Nexus)
		{
			NewPlugin->NexusID = Iter.NexusID;
			_DMESSAGE("Adding NEXUS mod to catched plugins with NexusID>>>%d", Iter.NexusID);

		}
		else if (NewPlugin->CheckType == t_CheckType_GitHub)
		{
			NewPlugin->GitHubPath = Iter.GitHubPath;
			_DMESSAGE("Adding GitHub mod to catched plugins with NexusID>>>%s", Iter.GitHubPath.c_str());
		}
		NewPlugin->OnlineVersion = Iter.OnlineVersion;
		g_CachedPlugins.push_back(NewPlugin);

	}



	_DMESSAGE("f_UpdateChecker_PopulateCachedArrayFromSUPDater() ends");
}



CachedPlugin* f_NexusPluginCatched(int iNumber)
{
	if (bSUPdaterLoaded && iPluginsCached == 0 && f_SUPDater_GetCurrentStage() >= t_PluginStage_Done)
	{
		f_UpdateChecker_PopulateCachedArrayFromSUPDater();
	}

	_DMESSAGE("g_CachedPlugins count is>>> %d", g_CachedPlugins.size());

	for (auto it = g_CachedPlugins.begin(); it != g_CachedPlugins.end(); it++)
	{
		if (Iter->CheckType != t_CheckType_Nexus)
			continue;

		_DMESSAGE("Comparing array ID %d with Request %d", Iter->NexusID, iNumber);

		if (Iter->NexusID == iNumber)
		{
			_DMESSAGE("Found cached plugin");
			return Iter;
		}
	}

	return NULL;
}






CachedPlugin* f_GitHubPluginCatched(string GitHubPath)
{
	if (bSUPdaterLoaded && iPluginsCached == 0 && f_SUPDater_GetCurrentStage() >= t_PluginStage_Done)
	{
		f_UpdateChecker_PopulateCachedArrayFromSUPDater();
	}

	_DMESSAGE("g_CachedPlugins count is>>> %d", g_CachedPlugins.size());

	for (auto it = g_CachedPlugins.begin(); it != g_CachedPlugins.end(); it++)
	{
		if (Iter->CheckType != t_CheckType_GitHub)
			continue;

		_DMESSAGE("Comparing array GithubPath>>> %s with Request>>> %s", Iter->GitHubPath.c_str(), GitHubPath.c_str());

		if (Iter->GitHubPath == GitHubPath)
		{
			_DMESSAGE("Found cached plugin");
			return Iter;
		}
	}

	return NULL;
}






void f_AddCachedPlugin(int CheckType, int NexusID, string GitHubPath, string OnlineVersion)
{
	_DMESSAGE("f_AddCachedPlugin started with Checktype>>>>%d NexusID>>>%d GitHubPath>>>>%s OnlineVersion>>%s", CheckType, NexusID, GitHubPath.c_str(), OnlineVersion.c_str());
	int iFound = 0;

	for (auto it2 = g_CachedPlugins.begin(); it2 != g_CachedPlugins.end(); it2++)
	{
		if (Iter2->CheckType == CheckType)
		{
			if (CheckType == t_CheckType_Nexus)
			{
				if (NexusID == Iter2->NexusID)
				{
					iFound = 1;
					_DMESSAGE("Found in existing array");
				}
			}
			else if (CheckType == t_CheckType_GitHub)
			{
				if (GitHubPath == Iter2->GitHubPath)
				{
					iFound = 1;
					_DMESSAGE("Found in existing array");
				}

			}

		}

	}

	if (iFound)
	{
		return;
	}

	CachedPlugin* NewPlugin = NULL;

	NewPlugin = new CachedPlugin;
	NewPlugin->CheckType = CheckType;

	if (NewPlugin->CheckType == t_CheckType_Nexus)
	{
		NewPlugin->NexusID = NexusID;
		_DMESSAGE("Adding NEXUS mod to catched plugins with NexusID>>>%d", NexusID);

	}
	else if (NewPlugin->CheckType == t_CheckType_GitHub)
	{
		NewPlugin->GitHubPath = GitHubPath;
		_DMESSAGE("Adding GitHub mod to catched plugins with NexusID>>>%s", GitHubPath.c_str());
	}
	NewPlugin->OnlineVersion = OnlineVersion;
	g_CachedPlugins.push_back(NewPlugin);
}








string f_GetGitHubLatestReleaseTag(string s_strArgTemp)
{
	HINTERNET hInternet = InternetOpenA("InetURL/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	HINTERNET hConnection = InternetConnectA(hInternet, "api.github.com", 80, " ", " ", INTERNET_SERVICE_HTTP, 0, 0);

	string s_RequestMod{};
	s_RequestMod = "/repos/";
	s_RequestMod += s_strArgTemp;
	s_RequestMod += "/releases/latest";


	HINTERNET hData = HttpOpenRequestA(hConnection, "GET", s_RequestMod.c_str(), NULL, NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION, 0);
	HttpAddRequestHeaders(hData, _T("accept: application/json"), -1, HTTP_ADDREQ_FLAG_ADD);

	char buf[2048 + 1];

	MyThread MyNewThread;
	MyNewThread.CheckType = t_CheckType_Nexus;

	thread NewThread(&MyThread::Execute, std::ref(MyNewThread));
	NewThread.detach();



	MyNewThread.stage = 5;
	MyNewThread.hData = hData;
	HttpSendRequestA(hData, NULL, 0, NULL, 0);
	MyNewThread.stage = 110;


	int iStatCode = f_GetStatCodeResponse(&hData);
	_DMESSAGE("iStatCode is %d", iStatCode);
	if (iStatCode == 0)
	{
		_DMESSAGE("Server not responding");
		return "ERROR";
	}

	DWORD bytesRead = 0;
	DWORD totalBytesRead = 0;

	string s_resultRead{};

	while (InternetReadFile(hData, buf, 2048, &bytesRead) && bytesRead != 0)
	{
		s_resultRead.append(buf, bytesRead);
		totalBytesRead += bytesRead;
	}
	InternetCloseHandle(hData);
	InternetCloseHandle(hConnection);
	InternetCloseHandle(hInternet);

	nlohmann::json j;
	j = nlohmann::json::parse(s_resultRead, 0, 0);

	if (!j.is_structured())
	{
		return "ERROR";
	}


	if (j["tag_name"].is_string())
	{

		string s_res = j["tag_name"].get<string>();

		f_AddCachedPlugin(t_CheckType_GitHub, -1, s_strArgTemp, s_res);

		return s_res;
	}
	else {
		return "ERROR";
	}
}






BSFixedString cmd_GetGitHubLatestReleaseTag(StaticFunctionTag* base, BSFixedString s_requestIN)
{
	_DMESSAGE("GetGitHubLatestReleaseTag started with request>>>%s", s_requestIN.c_str());
	string s_request = s_requestIN;

	CachedPlugin* TempPlugin = f_GitHubPluginCatched(s_request);

	if (TempPlugin)
	{
		_DMESSAGE("Plugin catched");
		return TempPlugin->OnlineVersion.c_str();
	}

	return f_GetGitHubLatestReleaseTag(s_request).c_str();
}



BSFixedString cmd_GetNexusModVersionAPI(StaticFunctionTag* base, UInt32 iNumber)
{
	_DMESSAGE("GetNexusModVersionAPI started with request>>>%d", iNumber);


	CachedPlugin* TempPlugin = f_NexusPluginCatched(iNumber);

	if (TempPlugin)
	{
		_DMESSAGE("Plugin catched");
		return TempPlugin->OnlineVersion.c_str();
	}


	if (bSUPdaterLoaded)
	{
		return f_SUPDater_GetNexusModVersionAPI(iNumber).c_str();
	}
	else {
		return "ERROR";
	}

}







bool cmd_IsNexusAPIKeyValid(StaticFunctionTag* base)
{

	if (bSUPdaterLoaded)
	{
		return f_SUPDater_IsAPIKeyValid();
	}
	else {
		return -1;
	}


	return true;
}



UInt32 cmd_GetSUPDateCheckerDoneStage(StaticFunctionTag* base)
{

	if (bSUPdaterLoaded)
	{
		return f_SUPDater_GetDoneStage();
	}
	else {
		return -1;
	}

	return true;
}



UInt32 cmd_GetSUPDateCheckerCurrentStage(StaticFunctionTag* base)
{

	if (bSUPdaterLoaded)
	{
		return f_SUPDater_GetCurrentStage();
	}
	else {
		return -1;
	}

	return true;
}




void cmd_OpenNexusModPage(StaticFunctionTag* base, UInt32 iNumber)
{
	string url = "https://www.nexusmods.com/fallout4/mods/" + to_string(iNumber);
	string op = string("start ").append(url);
	system(op.c_str());
}


bool cmd_IsSUPDateCheckerLoaded(StaticFunctionTag* base)
{
	return bSUPdaterLoaded;
}





BSFixedString cmd_GetPastebinRawTextString(StaticFunctionTag* base, BSFixedString s_URL)
{
	string s_strArgTemp2 = s_URL;
	PastebinPaste* TempPaste = PastebinReader::f_IsPasteInArray(s_strArgTemp2);

	if (TempPaste)
	{
		if (TempPaste->Stage == t_Pastebin_Result_Success)
		{
			_DMESSAGE("Paste already catched");
			return TempPaste->sText.c_str();
		}
	}


	HINTERNET hInternet = InternetOpenA("InetURL/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	HINTERNET hConnection = InternetConnectA(hInternet, "pastebin.com", 80, " ", " ", INTERNET_SERVICE_HTTP, 0, 0);

	string s_RequestMod = "/raw/" + s_strArgTemp2 + "?Cache-Control: no-cache";

	HINTERNET hData = HttpOpenRequestA(hConnection, "GET", s_RequestMod.c_str(), NULL, NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION, 0);

	char buf[2048 + 1];

	MyThread MyNewThread;
	MyNewThread.CheckType = t_CheckType_Nexus;

	thread NewThread(&MyThread::Execute, std::ref(MyNewThread));
	NewThread.detach();



	MyNewThread.stage = 5;
	MyNewThread.hData = hData;
	HttpSendRequestA(hData, NULL, 0, NULL, 0);
	MyNewThread.stage = 110;


	int iStatCode = f_GetStatCodeResponse(&hData);
	_DMESSAGE("iStatCode is %d", iStatCode);
	if (iStatCode == 0)
	{
		_DMESSAGE("Server not responding");
		return "ERROR";
	}
	else if (iStatCode == 404)
	{
		_DMESSAGE("Paste %s doesn't exist", s_strArgTemp2.c_str());
		return "";
	}

	DWORD bytesRead = 0;
	DWORD totalBytesRead = 0;

	string s_resultRead{};

	while (InternetReadFile(hData, buf, 2048, &bytesRead) && bytesRead != 0)
	{
		_DMESSAGE("buf is %s", buf);
		s_resultRead.append(buf, bytesRead);
		totalBytesRead += bytesRead;
	}
	InternetCloseHandle(hData);
	InternetCloseHandle(hConnection);
	InternetCloseHandle(hInternet);

	return s_resultRead.c_str();
}



VMArray<BSFixedString> cmd_GetPastebinRawTextArray(StaticFunctionTag* base, BSFixedString s_URL)
{
	string s_strArgTemp2 = s_URL;
	PastebinPaste* TempPaste = PastebinReader::f_IsPasteInArray(s_strArgTemp2);

	int iAlreadyCached = 0;

	VMArray<BSFixedString> result;

	if (TempPaste)
	{
		if (TempPaste->Stage == t_Pastebin_Result_Success)
		{
			_DMESSAGE("Paste already cached");
			iAlreadyCached = 1;
		}
	}

	string s_resultRead{};

	if (iAlreadyCached)
	{
		s_resultRead = TempPaste->sText;
	}
	else {
		HINTERNET hInternet = InternetOpenA("InetURL/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
		HINTERNET hConnection = InternetConnectA(hInternet, "pastebin.com", 80, " ", " ", INTERNET_SERVICE_HTTP, 0, 0);

		string s_RequestMod = "/raw/" + s_strArgTemp2 + "?Cache-Control: no-cache";

		HINTERNET hData = HttpOpenRequestA(hConnection, "GET", s_RequestMod.c_str(), NULL, NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION, 0);

		char buf[2048 + 1];

		MyThread MyNewThread;
		MyNewThread.CheckType = t_CheckType_Nexus;

		thread NewThread(&MyThread::Execute, std::ref(MyNewThread));
		NewThread.detach();



		MyNewThread.stage = 5;
		MyNewThread.hData = hData;
		HttpSendRequestA(hData, NULL, 0, NULL, 0);
		MyNewThread.stage = 110;


		int iStatCode = f_GetStatCodeResponse(&hData);
		_DMESSAGE("iStatCode is %d", iStatCode);
		if (iStatCode == 0)
		{
			_DMESSAGE("Server not responding");
			return result;
		}
		else if (iStatCode == 404)
		{
			_DMESSAGE("Paste %s doesn't exist", s_strArgTemp2.c_str());
			return result;
		}

		DWORD bytesRead = 0;
		DWORD totalBytesRead = 0;

		while (InternetReadFile(hData, buf, 2048, &bytesRead) && bytesRead != 0)
		{
			_DMESSAGE("buf is %s", buf);
			s_resultRead.append(buf, bytesRead);
			totalBytesRead += bytesRead;
		}
		InternetCloseHandle(hData);
		InternetCloseHandle(hConnection);
		InternetCloseHandle(hInternet);

	}

	vector<string> Splitted = f_Split_String(s_resultRead, "\n");

	if (Splitted.size() > 0)
	{
		for (auto it = Splitted.begin(); it != Splitted.end(); ++it)
		{
			result.Push(&(BSFixedString)Iter.c_str());
			_DMESSAGE("ADDING LINE TO ARRAY RESULT");
		}
	}

	return result;
}



UInt32 cmd_GetPastebinReaderDoneStage(StaticFunctionTag* base)
{
	return iPastebinReaderDoneStage;
}



UInt32 cmd_GetPastebinReaderCurrentStage(StaticFunctionTag* base)
{
	return iPastebinReaderStage;
}