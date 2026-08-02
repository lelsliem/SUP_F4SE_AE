

string s_PastebinReaderPath = "Data\\f4se\\plugins\\SUPPastebinReader";



#define t_Pastebin_ReturnType_NonValid -1
#define t_Pastebin_ReturnType_String 0
#define t_Pastebin_ReturnType_Array 1



#define t_Pastebin_Result_FailNoPasteExists -2
#define t_Pastebin_Result_FailNoConnection -1
#define t_Pastebin_Result_Default 0
#define t_Pastebin_Result_Success 1


class PastebinPaste
{
public:

	int Stage = t_Pastebin_Result_Default;
	string sURL{};
	string sText{};
	int ReturnType = t_Pastebin_ReturnType_NonValid;

	string ScriptName{};
	string FunctionName{};
};


vector<PastebinPaste*> g_PastebinPastes;
int iPastebinReaderStage = 0;
int iPastebinReaderDoneStage = 100;

int i_PastebinReader_LoadingDone = 0;
int i_PastebinReader_CallFunctions = 0;


int iPastebinPasteCount = 0;
thread* PastebinReaderThread;


class MyThreadPastebin {
public:
	std::atomic<int> stage = 0;
	std::atomic<HINTERNET> hData = 0;
	plf::nanotimer timer;

	void Execute()
	{
		_DMESSAGE("SUP_PASTEBIN::Thread starts");


		while (stage < 100)
		{

			if (stage == 5)
			{
				timer.start();
				_DMESSAGE("SUP_PASTEBIN::Reset timer");
				stage = 10;
			}
			else if (stage == 10)
			{
				if (timer.get_elapsed_ms() > 2999)
				{
					_DMESSAGE("SUP_PASTEBIN::Timer elapsed");
					InternetCloseHandle(hData);
					stage = 120;
				}

			}

		}

		_DMESSAGE("SUP_PASTEBIN::Thread finishes, stage is %d", (int)stage);
	}
};



namespace PastebinReader {


	void CheckPluginsForInvalid()
	{
		iPastebinReaderStage = 30;
		for (auto it = g_PastebinPastes.begin(); it != g_PastebinPastes.end();)
		{

			//if (f_PastebinReader_IsPasteValid(*it))
			//{
			//	_DMESSAGE("Pastbin Paste valid>>%s", Iter->sURL.c_str());
			iPastebinPasteCount += 1;
			++it;
			//}
			//else {
			//	_DMESSAGE("Erasing>>%s", Iter->sURL.c_str());
			//	it = g_PastebinPastes.erase(it);
			//}
		}

		_DMESSAGE("SUP_PASTEBIN::iPastebinPasteCount>>>%d", iPastebinPasteCount);
	}





	void ReadPastebinPastes()
	{
		_DMESSAGE("SUP_PASTEBIN::<<f_PastebinReader_ReadPastebinPastes() starts>>");
		HINTERNET hInternet = InternetOpenA("InetURL/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);


		HINTERNET hConnection = InternetConnectA(hInternet, "pastebin.com", 80, " ", " ", INTERNET_SERVICE_HTTP, 0, 0);
		HINTERNET hData;
		string s_RequestMod{};

		DWORD bytesRead = 0;
		DWORD totalBytesRead = 0;
		char* FindRes = NULL;
		std::size_t found;

		string s_res{};
		char buf[2048 + 1];

		int iClearI = 0;


		HINTERNET* hDataTemp = NULL;
		string s_resultRead{};



		MyThreadPastebin MyNewThread;


		thread NewThread(&MyThreadPastebin::Execute, std::ref(MyNewThread));
		NewThread.detach();

		for (auto it = g_PastebinPastes.begin(); it != g_PastebinPastes.end(); it++)
		{
			s_resultRead.clear();
			bytesRead = 0;
			totalBytesRead = 0;
			string s_RequestMod = "/raw/" + Iter->sURL + "?Cache-Control: no-cache";
			_DMESSAGE("SUP_PASTEBIN::Sending request");

			hData = HttpOpenRequestA(hConnection, "GET", s_RequestMod.c_str(), NULL, NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION, 0);

			MyNewThread.stage = 5;
			MyNewThread.hData = hData;
			HttpSendRequestA(hData, NULL, 0, NULL, 0);
			MyNewThread.stage = 110;



			string s_resultRead{};

			while (InternetReadFile(hData, buf, 2048, &bytesRead) && bytesRead != 0)
			{
				s_resultRead.append(buf, bytesRead);
			}


			int iStatCode = f_GetStatCodeResponse(&hData);
			_DMESSAGE("SUP_PASTEBIN::iStatCode is %d", iStatCode);
			if (iStatCode == 0)
			{
				Iter->Stage = t_Pastebin_Result_FailNoConnection;
				_DMESSAGE("SUP_PASTEBIN::Server not responding");
				continue;
			}
			else if (iStatCode == 404)
			{
				_DMESSAGE("SUP_PASTEBIN::Paste %s doesn't exist", Iter->sURL.c_str());
				Iter->Stage = t_Pastebin_Result_FailNoPasteExists;
				continue;
			}


			InternetCloseHandle(hData);
			Iter->Stage = t_Pastebin_Result_Success;
			Iter->sText = s_resultRead;
		}


		InternetCloseHandle(hConnection);
		InternetCloseHandle(hInternet);
		_DMESSAGE("SUP_PASTEBIN::Pastebin reader Function ends");
		MyNewThread.stage = 110;

	}






	void CallFunction(PastebinPaste* NewPaste)
	{
		_DMESSAGE("f_PastebinReader_CallFunctionString started for %s", NewPaste->sURL.c_str());
		BSFixedString g_Message;
		CALL_MEMBER_FN(&g_Message, Set)(NewPaste->sText.c_str());
		BSFixedString g_ScriptName;
		CALL_MEMBER_FN(&g_ScriptName, Set)(NewPaste->ScriptName.c_str());
		BSFixedString g_FuncName;
		CALL_MEMBER_FN(&g_FuncName, Set)(NewPaste->FunctionName.c_str());


		VMArray<VMVariable> arguments;
		VMValue args;

		UInt32 myStage = NewPaste->Stage;

		VMVariable var0;
		var0.Set<UInt32>(&myStage);
		arguments.Push(&var0);


		VMVariable var1;
		var1.Set<BSFixedString>(&g_Message);
		arguments.Push(&var1);

		VMArray<BSFixedString> Array_String;
		if (NewPaste->ReturnType == t_Pastebin_ReturnType_Array)
		{
			vector<string> Splitted = f_Split_String(NewPaste->sText, "\n");
			for (auto it = Splitted.begin(); it != Splitted.end(); ++it)
			{
				Array_String.Push(&(BSFixedString)(*it).c_str());
			}
		}

		VMVariable var2;
		var2.Set<VMArray<BSFixedString>>(&Array_String);
		arguments.Push(&var2);

		arguments.PackArray(&args, g_MyVirtMachine);
		CallGlobalFunctionNoWait_Internal(g_MyVirtMachine, 0, 0, &g_ScriptName, &g_FuncName, &args);
	}







	void CallFunctions()
	{
		_DMESSAGE("SUP_PASTEBIN::f_PastebinReader_CallFunctions() started");


		if (i_PastebinReader_LoadingDone)
		{
			for (auto it = g_PastebinPastes.begin(); it != g_PastebinPastes.end(); it++)
			{
				CallFunction(Iter);
			}

		}
		else {
			_DMESSAGE("SUP_PASTEBIN::Loading not yet done");
			i_PastebinReader_CallFunctions = 1;
		}




	}



	void ReadPastesTHREAD()
	{
		iPastebinReaderStage = 50;
		plf::nanotimer mytimer;
		mytimer.start();
		_DMESSAGE("SUP_PASTEBIN::Pastebin Reader thread started");
		(*PastebinReaderThread).detach();

		ReadPastebinPastes();


		_DMESSAGE("SUP_PASTEBIN::Pastebin reader thread ended fine,Thread time elapsed:: %f", mytimer.get_elapsed_ms());


		iPastebinReaderStage = 80;
		CallFunctions();

		delete PastebinReaderThread;
	}







	void ReadPastes()
	{
		iPastebinReaderStage = 40;
		_DMESSAGE("SUP_PASTEBIN::f_PastebinReader_ReadPastes() started");
		PastebinReaderThread = new thread(ReadPastesTHREAD);
	}





	PastebinPaste* f_IsPasteInArray(string sURL)
	{

		for (auto it = g_PastebinPastes.begin(); it != g_PastebinPastes.end(); it++)
		{
			if (Iter->sURL == sURL)
			{
				return Iter;
			}
		}

		return NULL;
	}










	bool ProcessFile(string sFilePath)
	{

		_DMESSAGE("SUP_PASTEBIN::f_Pastebin_ProcessFile started for %s", sFilePath.c_str());
		nlohmann::json j;

		std::ifstream t(sFilePath);

		if (!t.is_open())
		{
			_DMESSAGE("SUP_PASTEBIN::File can't be opened");
			return false;
		}
		else {
			_DMESSAGE("SUP_PASTEBIN::File opened");
		}

		std::stringstream buffer;
		buffer << t.rdbuf();
		t.close();
		j = nlohmann::json::parse(buffer, 0, 0);

		if (!j.is_structured())
		{
			_DMESSAGE("SUP_PASTEBIN::JSON is not structured");
			return false;
		}
		else {
			_DMESSAGE("SUP_PASTEBIN::JSON is structured");
		}


		string sURL{};
		string ScriptName{};
		string FunctionName{};
		int ReturnType;

		if (j.find("PasteURL") != j.end())
		{
			sURL = j["PasteURL"].get<std::string>();
		}
		else {
			_DMESSAGE("SUP_PASTEBIN::PasteURL is not found in JSON file");
			return false;
		}


		PastebinPaste* PastebinTempCheck = f_IsPasteInArray(sURL);

		if (PastebinTempCheck)
		{
			_DMESSAGE("SUP_PASTEBIN::Paste already exists in array, name is %s. This is a dublicate", PastebinTempCheck->sURL.c_str());
			return false;
		}


		if (j.find("ReturnType") != j.end())
		{
			ReturnType = j["ReturnType"].get<int>();
		}
		else {
			_DMESSAGE("SUP_PASTEBIN::ReturnType is not found in JSON file");
			return false;
		}


		if (j.find("ScriptName") != j.end())
		{
			ScriptName = j["ScriptName"].get<std::string>();
		}
		else {
			_DMESSAGE("SUP_PASTEBIN::ScriptName is not found in JSON file");
			return false;
		}

		if (j.find("FunctionName") != j.end())
		{
			FunctionName = j["FunctionName"].get<std::string>();
		}
		else {
			_DMESSAGE("SUP_PASTEBIN::FunctionName is not found in JSON file");
			return false;
		}

		PastebinPaste* NewPaste = new PastebinPaste;
		NewPaste->sURL = sURL;
		NewPaste->ReturnType = ReturnType;
		NewPaste->ScriptName = ScriptName;
		NewPaste->FunctionName = FunctionName;

		_DMESSAGE("SUP_PASTEBIN::New Paste Added>>>>%s, ReturnType is>>>%d", NewPaste->sURL.c_str(), NewPaste->ReturnType);

		g_PastebinPastes.push_back(NewPaste);
		return true;
	}






	void PopulateFromFiles()
	{
		if (f_Does_FolderExist(s_PastebinReaderPath))
		{
			for (auto const& dir_entry : std::filesystem::directory_iterator{ s_PastebinReaderPath })
			{
				if (!dir_entry.is_directory())
					ProcessFile(dir_entry.path().u8string());
			}
		}
		else {
			_DMESSAGE("SUP_PASTEBIN::Folder doesn't exist.");
		}
	}


};

