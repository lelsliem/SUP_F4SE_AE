
///MAKE FUNCTION FOR NISTON >>>>>>>>>>SetMessageButtonText  MessageBoxForm N "Text",


string g_CurrentFilterRequest{};
string sCurrentMessage_ToShow{};
vector<int> v_CurrentMessage{};
string sCurrentMessage_ToSend{};

std::map<int, string> g_BB_MapKeyArray = {
std::make_pair(65,"a"),
std::make_pair(66,"b"),
std::make_pair(67,"c"),
std::make_pair(68,"d"),
std::make_pair(69,"e"),
std::make_pair(70,"f"),
std::make_pair(71,"g"),
std::make_pair(72,"h"),
std::make_pair(73,"i"),
std::make_pair(74,"j"),
std::make_pair(75,"k"),
std::make_pair(76,"l"),
std::make_pair(77,"m"),
std::make_pair(78,"n"),
std::make_pair(79,"o"),
std::make_pair(80,"p"),
std::make_pair(81,"q"),
std::make_pair(82,"r"),
std::make_pair(83,"s"),
std::make_pair(84,"t"),
std::make_pair(85,"u"),
std::make_pair(86,"v"),
std::make_pair(87,"w"),
std::make_pair(88,"x"),
std::make_pair(89,"y"),
std::make_pair(90,"z"),
std::make_pair(565,"A"),
std::make_pair(566,"B"),
std::make_pair(567,"C"),
std::make_pair(568,"D"),
std::make_pair(569,"E"),
std::make_pair(570,"F"),
std::make_pair(571,"G"),
std::make_pair(572,"H"),
std::make_pair(573,"I"),
std::make_pair(574,"J"),
std::make_pair(575,"K"),
std::make_pair(576,"L"),
std::make_pair(577,"M"),
std::make_pair(578,"N"),
std::make_pair(579,"O"),
std::make_pair(580,"P"),
std::make_pair(581,"Q"),
std::make_pair(582,"R"),
std::make_pair(583,"S"),
std::make_pair(584,"T"),
std::make_pair(585,"U"),
std::make_pair(586,"V"),
std::make_pair(587,"W"),
std::make_pair(588,"X"),
std::make_pair(589,"Y"),
std::make_pair(590,"Z"),
std::make_pair(48,"0"),
std::make_pair(49,"1"),
std::make_pair(50,"2"),
std::make_pair(51,"3"),
std::make_pair(52,"4"),
std::make_pair(53,"5"),
std::make_pair(54,"6"),
std::make_pair(55,"7"),
std::make_pair(56,"8"),
std::make_pair(57,"9"),
std::make_pair(188,","),
std::make_pair(189,"-"),
std::make_pair(190,".") //dot
};



UInt32 cmd_BBHideMessage(StaticFunctionTag* base)
{
	_DMESSAGE("BBHideMessage started");
	UInt32 iMessageID = g_CurrentMessageID;


	int iFileExists = f_Does_FileExist(s_BB_HiddenNotesPath);

	_DMESSAGE("SUP_BB::File exists>>%d", iFileExists);

	fstream f(s_BB_HiddenNotesPath);
	int iFoundMessage = 0;

	if (iFileExists)
	{
		if (f.is_open() == false)
		{
			g_BBLastServerMessage = "Cannot open " + s_BB_HiddenNotesPath + " file. Please manually delete it for this feature to work.";
			_DMESSAGE("SUP_BB::Cannot open file.");
			return -1;
		}

		nlohmann::json j;
		f >> j;

		if (j.is_structured() == false)
		{
			f.close();
			g_BBLastServerMessage = "Wrong JSON structure in " + s_BB_HiddenNotesPath + " file. Please manually delete it for this feature to work.";
			_DMESSAGE("SUP_BB::Hidden file is not structured");
			return -1;
		}

		for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it) {

			if (Iter.is_number_integer())
			{
				if (Iter.get<int>() == iMessageID)
				{
					iFoundMessage = 1;
					g_BBLastServerMessage = "Message is already hidden.";
					_DMESSAGE("SUP_BB::Message is already hidden.");
					break;
				}


			}
			else {
				_DMESSAGE("SUP_BB:: array value is not integrer");
			}

		}

		f.close();

		if (iFoundMessage == 1)
		{
			return 0;
		}
		else {
			j.push_back(iMessageID);
			std::ofstream o(s_BB_HiddenNotesPath);
			if (o.is_open() == false)
			{
				g_BBLastServerMessage = "Cannot write to file: " + s_BB_HiddenNotesPath + " file. Please try manually deleting it and try again.";
				return -2;
			}
			else {
				o << std::setw(4) << j << std::endl;
				_DMESSAGE("SUP_BB::Writing to file.");
			}

		}


	}
	else {
		nlohmann::json j;
		j.push_back(iMessageID);

		std::ofstream o(s_BB_HiddenNotesPath);
		if (o.is_open() == false)
		{
			g_BBLastServerMessage = "Cannot write to file: " + s_BB_HiddenNotesPath + " file. Please try manually deleting it and try again.";
			return -2;
		}
		else {
			o << std::setw(4) << j << std::endl;
			_DMESSAGE("SUP_BB::Writing to file.");
		}

	}

	if (f.is_open())
	{
		f.close();
	}
	f_BB_RemoveMessageByID(iMessageID);
	return 1;
}









int f_BB_GetUserMessages()
{
	if (iBBAPIKeyLoaded == 0)
	{
		_DMESSAGE("SUP_BB::Nexus API key not present>>");
		g_BBLastServerMessage = "Nexus API key not present.";
		return 0;
	}


	for (auto it = g_UserMessagesTemp.begin(); it != g_UserMessagesTemp.end(); it++)
	{
		delete Iter;
	}
	g_UserMessagesTemp.clear();


	HINTERNET hInternet = InternetOpenA("InetURL/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	HINTERNET hConnection = InternetConnectA(hInternet, "Tomminfinite.pythonanywhere.com", 80, " ", " ", INTERNET_SERVICE_HTTP, 0, 0);
	HINTERNET hData = HttpOpenRequestA(hConnection, "POST", "/FO4MessagesGetUserMessages", NULL, NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION, 0);
	HttpAddRequestHeadersA(hData, "content-type: application/json", -1, HTTP_ADDREQ_FLAG_ADD);


	char buf[2048 + 1];

	MyThreadBB MyNewThread;
	MyNewThread.CheckType = t_CheckType_Nexus;

	thread NewThread(&MyThreadBB::Execute, std::ref(MyNewThread));
	NewThread.detach();


	MyNewThread.stage = 5;
	MyNewThread.hData = hData;

	string s_BodyTemp = R"({"apikey" : ")" + s_BBApiKey + R"(", "APIVersion" : )" + to_string(t_BB_API_VER) + R"(})";

	HttpSendRequestA(hData, NULL, 0, (LPVOID)s_BodyTemp.c_str(), s_BodyTemp.length());
	MyNewThread.stage = 110;


	int iStatCode = f_GetStatCodeResponse(&hData);
	_DMESSAGE("SUP_BB::iStatCode is %d", iStatCode);
	if (iStatCode == 0)
	{
		_DMESSAGE("SUP_BB::Server not responding");
		g_BBLastServerMessage = "Server is not responding or internet connection is absent";
		return -1;
	}
	else if (iStatCode == 404)
	{
		_DMESSAGE("SUP_BB::Page does not exist");
		g_BBLastServerMessage = "Page on server doesn't exist.";
		return -1;
	}
	else if (iStatCode == 500)
	{
		_DMESSAGE("SUP_BB::stat code 500");
		g_BBLastServerMessage = "Internal server error. Please take screenshot of this message and send to TommInfinite in Direct Messages as it contains your API key>>" + s_BodyTemp;
		return -1;
	}
	else if (iStatCode != 200)
	{
		_DMESSAGE("SUP_BB::statcode unidentified>>>%d", iStatCode);
		g_BBLastServerMessage = "Problem connecting to server. Please try again later.>>";
		return -1;
	}



	DWORD bytesRead = 0;
	DWORD totalBytesRead = 0;

	string s_resultRead{};

	while (InternetReadFile(hData, buf, 2048, &bytesRead) && bytesRead != 0)
	{
		_DMESSAGE("SUP_BB::buf is %s", buf);
		s_resultRead.append(buf, bytesRead);
		totalBytesRead += bytesRead;
	}
	InternetCloseHandle(hData);
	InternetCloseHandle(hConnection);
	InternetCloseHandle(hInternet);


	auto j = nlohmann::json::parse(s_resultRead, 0, 0);

	if (!j.is_structured())
	{
		_DMESSAGE("SUP_BB::JSON is not structured");
		g_BBLastServerMessage = "Internal server error - JSON not structured. Please take screenshot of this message and send to TommInfinite in Direct Messages as it contains your API key>>" + s_resultRead;
		return 0;
	}

	int iSuccess = 0;

	if (j.find("success") != j.end())
	{
		iSuccess = j["success"].get<int>();
		if (iSuccess == 1)
		{
			_DMESSAGE("SUP_BB::No messages for user");
			return 0;
		}

	}



	if (j.find("message") != j.end())
	{
		string s_temp_Message = j["message"].get<string>();
		g_BBLastServerMessage = s_temp_Message;
		_DMESSAGE("SUP_BB::Answer is>>%s", s_temp_Message.c_str());
		return -1;
	}


	string message{};
	string PluginName{};
	int CellFormID{};
	int id = -1;
	string CoordinatesXYZ;
	int iInterior = 0;
	TESObjectCELL* Cell = NULL;
	int iRating = 0;
	vector<int> vCurrentMessage{};
	string message_text{};
	int iMessageType;
	for (auto& [key, val] : j.items())
	{
		//_DMESSAGE("SUP_BB::New Element");
		vCurrentMessage.clear();

		if (val.find("id") != val.end())
		{
			id = val["id"].get<int>();
		}
		else {
			_DMESSAGE("SUP_BB::id is not found in JSON file");
			continue;
		}



		if (val.find("messageContents") != val.end())
		{
			if (val["messageContents"].is_string())
			{

				message = val["messageContents"].get<std::string>();
				//_DMESSAGE("message is>>>%s", message.c_str());

				nlohmann::json jMessage = nlohmann::json::parse(message.c_str(), 0, 0);


				if (!jMessage.is_structured())
				{
					_DMESSAGE("SUP_BB::JSON MESSAGE is not structured");
					continue;
				}

				if (jMessage.find("Text") != jMessage.end())
				{
					iMessageType = t_BB_MessageTypeFreeText;
					if (jMessage["Text"].is_string() == 0)
					{
						_DMESSAGE("SUP_BB::JSON Text is not a string");
						continue;
					}
					else {
						message_text = jMessage["Text"].get<std::string>();
						//_DMESSAGE("BB::Message text is>>>%s", message_text.c_str());
					}
				}
				else {
					iMessageType = t_BB_MessageTypeSimple;
					int iFoundError = 0;


					for (auto it : jMessage.items())
					{
						if (Iter.value().is_number() == 0)
						{
							_DMESSAGE("SUP_BB::JSON MESSAGE object is invalid - not number");
							iFoundError = 1;
							break;
						}
						else {
							//_DMESSAGE("Adding %d to message", Iter.value().get<int>());
							vCurrentMessage.push_back(Iter.value().get<int>());
						}
					}

					if (iFoundError)
					{
						continue;
					}
				}




			}
			else {
				_DMESSAGE("SUP_BB::message is not a string");
				continue;
			}

		}
		else {
			_DMESSAGE("SUP_BB::message is not found in JSON file");
			continue;
		}


		if (val.find("rating") != val.end())
		{
			iRating = val["rating"].get<int>();
		}
		else {
			_DMESSAGE("SUP_BB::rating is not found in JSON file");
			continue;
		}



		OnlineMessage* TempMessage = new OnlineMessage;

		TempMessage->MessageID = id;
		TempMessage->Type = iMessageType;
		if (iMessageType == t_BB_MessageTypeFreeText)
		{
			TempMessage->s_Message = message_text;
		}
		else {
			TempMessage->v_Message = vCurrentMessage;
		}


		TempMessage->Rating = iRating;

		_DMESSAGE("New user messages added, ID>>>%d", TempMessage->MessageID);
		g_UserMessagesTemp.push_back(TempMessage);

	}


	g_ManageMessagesMaxMessages = g_UserMessagesTemp.size();
	_DMESSAGE("Message count>>>%d", g_ManageMessagesMaxMessages);
	return g_ManageMessagesMaxMessages;

}




int f_BB_DeleteUserMessage()
{
	_DMESSAGE("SUP_BB::BBDeleteUserMessage starts");



	OnlineMessage* MessageTemp = g_UserMessagesTemp.at(g_ManageMessagesCurrentMessage);

	if (MessageTemp == NULL)
	{
		_DMESSAGE("SUP_BB::Message doesn't exists locally.");
		g_BBLastServerMessage = "Message doesn't exists locally.";
		return 0;
	}

	int iMessageID = MessageTemp->MessageID;


	if (iBBAPIKeyLoaded == 0)
	{
		_DMESSAGE("SUP_BB::iBBAPIKeyLoaded not loaded>>");
		g_BBLastServerMessage = "BB key is not loaded";
		return 0;
	}



	HINTERNET hInternet = InternetOpenA("InetURL/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	HINTERNET hConnection = InternetConnectA(hInternet, "Tomminfinite.pythonanywhere.com", 80, " ", " ", INTERNET_SERVICE_HTTP, 0, 0);
	HINTERNET hData = HttpOpenRequestA(hConnection, "POST", "/FO4MessagesRemoveUserMessage", NULL, NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION, 0);
	HttpAddRequestHeadersA(hData, "content-type: application/json", -1, HTTP_ADDREQ_FLAG_ADD);

	char buf[2048 + 1];

	MyThreadBB MyNewThread;
	MyNewThread.CheckType = t_CheckType_Nexus;

	thread NewThread(&MyThreadBB::Execute, std::ref(MyNewThread));
	NewThread.detach();


	MyNewThread.stage = 5;
	MyNewThread.hData = hData;


	string s_BodyTemp = R"({"apikey" : ")" + s_BBApiKey + R"(", "APIVersion" : )" + to_string(t_BB_API_VER) + R"(, "message_id" : )" + to_string(iMessageID) + R"(})";

	_DMESSAGE("JSON REQUEST IS>>>%s", s_BodyTemp.c_str());
	HttpSendRequestA(hData, NULL, 0, (LPVOID)s_BodyTemp.c_str(), s_BodyTemp.length());
	MyNewThread.stage = 110;

	int iStatCode = f_GetStatCodeResponse(&hData);
	_DMESSAGE("SUP_BB::iStatCode is %d", iStatCode);
	if (iStatCode == 0)
	{
		_DMESSAGE("SUP_BB::Server not responding");
		g_BBLastServerMessage = "Server is not responding or internet connection is absent";
		return 0;
	}
	else if (iStatCode == 404)
	{
		_DMESSAGE("SUP_BB::Page does not exist");
		g_BBLastServerMessage = "Page on server doesn't exist.";
		return 0;
	}
	else if (iStatCode == 500)
	{
		_DMESSAGE("SUP_BB::stat code 500");
		g_BBLastServerMessage = "Internal server error. Please take screenshot of this message and send to TommInfinite in Direct Messages as it contains your API key>>" + s_BodyTemp;
		return 0;
	}
	else if (iStatCode != 200)
	{
		_DMESSAGE("SUP_BB::statcode unedentified>>>%d", iStatCode);
		g_BBLastServerMessage = "Problem connecting to server. Please try again later.>>";
		return 0;
	}


	DWORD bytesRead = 0;
	DWORD totalBytesRead = 0;

	string s_resultRead{};

	while (InternetReadFile(hData, buf, 2048, &bytesRead) && bytesRead != 0)
	{
		_DMESSAGE("SUP_BB::buf is %s", buf);
		s_resultRead.append(buf, bytesRead);
		totalBytesRead += bytesRead;
	}
	InternetCloseHandle(hData);
	InternetCloseHandle(hConnection);
	InternetCloseHandle(hInternet);



	auto j = nlohmann::json::parse(s_resultRead, 0, 0);

	if (!j.is_structured())
	{
		_DMESSAGE("SUP_BB::JSON is not structured");
		g_BBLastServerMessage = "Internal server error - JSON not structured. Please take screenshot of this message and send to TommInfinite in Direct Messages as it contains your API key>>" + s_resultRead;
		return 0;
	}

	string s_Message_result;
	int iDeletionSuccess = 0;



	if (j.find("success") != j.end())
	{
		iDeletionSuccess = j["success"].get<int>();

	}
	else {
		_DMESSAGE("SUP_BB::JSON does not have success key");
		g_BBLastServerMessage = "Internal server error - JSON does not have success key. Please take screenshot of this message and send to TommInfinite in Direct Messages as it contains your API key>>" + s_resultRead;
		return 0;
	}

	if (j.find("message") != j.end())
	{
		s_Message_result = j["message"].get<string>();
	}
	else {
		_DMESSAGE("SUP_BB::JSON does not have message key");
		g_BBLastServerMessage = "Internal server error - JSON does not have message key. Please take screenshot of this message and send to TommInfinite in Direct Messages as it contains your API key>>" + s_resultRead;
		return 0;
	}


	if (iDeletionSuccess == 0)
	{
		_DMESSAGE("SUP_BB::JSON deletion success key is 0");
		g_BBLastServerMessage = s_Message_result;
		return 0;
	}

	if (iDeletionSuccess == 1)
	{

		g_BBLastServerMessage = s_Message_result;
		f_BB_RemoveMessageByID(iMessageID);
	}


	_DMESSAGE("BBDeleteUserMessage ends");
	return 1;
}




UInt32 cmd_BBDeleteUserMessage(StaticFunctionTag* base)
{
	return f_BB_DeleteUserMessage();
}






int f_BB_RefreshManageMenuMessage(UInt32 iButton, UInt32 iCurrentPage)
{

	_DMESSAGE("f_BB_RefreshManageMenuMessage started, iButton>>>%d,CurrentPage>>>>%d", iButton, iCurrentPage);

	string s_FinalMessage{};

	OnlineMessage* MessageTemp = NULL;
	int iMessageCount = 0;

	if (iCurrentPage == -1)
	{

		if (iButton == -100) // Init
		{
			g_ManageMessagesCurrentMessage = 0; 
		}
		else if (iButton == 0) // Next
		{
			if ((g_ManageMessagesCurrentMessage + 1) >= g_ManageMessagesMaxMessages)
			{
			}
			else {
				g_ManageMessagesCurrentMessage += 1;
			}

		}else if(iButton == 1) //Previous
		{

			if ((g_ManageMessagesCurrentMessage -1) < 0)
			{
			}
			else {
				g_ManageMessagesCurrentMessage -= 1;
			}

		}
		else if (iButton == 2) //Delete
		{
			g_ManageMessagesCurrentPage = -5;
		}
		//else if (iButton == 3) //Exit
		//{
		//	g_ManageMessagesCurrentPage = -10;
		//	Console_Print("Setting current page to -10");
		//}

	}
	//else if (iCurrentPage == -5)
	//{
		//g_ManageMessagesCurrentPage = -1;
		//if (iButton == 0)
		//{
			//f_BB_DeleteUserMessage();
			//g_ManageMessagesMaxMessages = f_BB_GetUserMessages();
			///g_ManageMessagesCurrentMessage = 0;
		//}

	//}


	if (g_ManageMessagesMaxMessages > 0)
	{
		MessageTemp = g_UserMessagesTemp.at(g_ManageMessagesCurrentMessage);

		if (MessageTemp)
		{
			s_FinalMessage += "Showing message#" + to_string(g_ManageMessagesCurrentMessage + 1) + " out of " + to_string(g_ManageMessagesMaxMessages) + ". Rating: " + to_string(MessageTemp->Rating) + "." + '\n' + '\n';
			s_FinalMessage += f_BB_GetStringFromMessage(MessageTemp);
		}
		else {
			s_FinalMessage = "Error occured - no message.";
			g_ManageMessagesCurrentPage = -20;
		}
	}
	else {
		s_FinalMessage = "You have no more messages.";
		g_ManageMessagesCurrentPage = -20;
	}


	f_BB_SetMessageBodyText(s_FinalMessage, 1);
	return g_ManageMessagesMaxMessages;
}


UInt32 cmd_BBManageMenuUpdate(StaticFunctionTag* base, UInt32 iButton, UInt32 iCurrentPage)
{
	_DMESSAGE("BBManageMenuUpdate started");
	int iCurrentButtonCount = f_BB_RefreshManageMenuMessage(iButton, iCurrentPage);
	_DMESSAGE("Button count>>>%d", iCurrentButtonCount);
	return iCurrentButtonCount;
}







UInt32 cmd_BBGetManageMessagesCurrentPage(StaticFunctionTag* base)
{
	return g_ManageMessagesCurrentPage;
}

UInt32 cmd_BBSetManageMessagesCurrentPage(StaticFunctionTag* base, UInt32 iValue)
{
	g_ManageMessagesCurrentPage = iValue;
	_DMESSAGE("Setting TextInputCurrentPage to >>>%d", iValue);
	return 1;
}









UInt32 cmd_BBGetUserMessages(StaticFunctionTag* base)
{
	return f_BB_GetUserMessages();
}







bool f_BB_LikeLocalByID(int currentID)
{
	_DMESSAGE("SUP_BB::f_BB_LikeLocalByID starts, ID is>>>%d", currentID);



	for (auto& [key, val] : g_InteriorCellMap)
	{

		for (auto it = val.begin(); it != val.end(); it++)
		{
			if (Iter->MessageID == currentID)
			{
				Iter->Rating = Iter->Rating + 1;
				return true;
			}

		}

	}

	for (auto& [key, val] : g_ExteriorCellMap)
	{

		for (auto it = val.begin(); it != val.end(); it++)
		{
			if (Iter->MessageID == currentID)
			{
				Iter->Rating = Iter->Rating + 1;
				return true;
			}

		}

	}

	return false;
	_DMESSAGE("SUPBB::ENDS");
}







UInt32 cmd_BBLikeMessage(StaticFunctionTag* base)
{

	UInt32 iMessageID = g_CurrentMessageID;
	

	if (iBBAPIKeyLoaded == 0)
	{
		_DMESSAGE("SUP_BB::iBBAPIKeyLoaded not loaded>>");
		g_BBLastServerMessage = "BB key is not loaded.";
		return 0;
	}


	HINTERNET hInternet = InternetOpenA("InetURL/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	HINTERNET hConnection = InternetConnectA(hInternet, "Tomminfinite.pythonanywhere.com", 80, " ", " ", INTERNET_SERVICE_HTTP, 0, 0);
	HINTERNET hData = HttpOpenRequestA(hConnection, "POST", "/FO4MessagesLikeMessage", NULL, NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION, 0);
	HttpAddRequestHeadersA(hData, "content-type: application/json", -1, HTTP_ADDREQ_FLAG_ADD);

	char buf[2048 + 1];

	MyThreadBB MyNewThread;
	MyNewThread.CheckType = t_CheckType_Nexus;

	thread NewThread(&MyThreadBB::Execute, std::ref(MyNewThread));
	NewThread.detach();


	MyNewThread.stage = 5;
	MyNewThread.hData = hData;


	string s_BodyTemp = R"({"apikey" : ")" + s_BBApiKey + R"(", "APIVersion" : )" + to_string(t_BB_API_VER) + R"(, "message_id" : )" + to_string(iMessageID) + R"(})";
	_DMESSAGE("JSON REQUEST IS>>>%s", s_BodyTemp.c_str());
	HttpSendRequestA(hData, NULL, 0, (LPVOID)s_BodyTemp.c_str(), s_BodyTemp.length());
	MyNewThread.stage = 110;

	int iStatCode = f_GetStatCodeResponse(&hData);
	_DMESSAGE("SUP_BB::iStatCode is %d", iStatCode);
	if (iStatCode == 0)
	{
		_DMESSAGE("SUP_BB::Server not responding");
		g_BBLastServerMessage = "Server is not responding or internet connection is absent";
		return 0;
	}
	else if (iStatCode == 404)
	{
		_DMESSAGE("SUP_BB::Page does not exist");
		g_BBLastServerMessage = "Page on server doesn't exist.";
		return 0;
	}
	else if (iStatCode == 500)
	{
		_DMESSAGE("SUP_BB::stat code 500");
		g_BBLastServerMessage = "Internal server error. Please take screenshot of this message and send to TommInfinite in Direct Messages as it contains your API key>>" + s_BodyTemp;
		return 0;
	}
	else if (iStatCode != 200)
	{
		_DMESSAGE("SUP_BB::statcode unedentified>>>%d", iStatCode);
		g_BBLastServerMessage = "Problem connecting to server. Please try again later.>>";
		return 0;
	}


	DWORD bytesRead = 0;
	DWORD totalBytesRead = 0;

	string s_resultRead{};

	while (InternetReadFile(hData, buf, 2048, &bytesRead) && bytesRead != 0)
	{
		_DMESSAGE("SUP_BB::buf is %s", buf);
		s_resultRead.append(buf, bytesRead);
		totalBytesRead += bytesRead;
	}
	InternetCloseHandle(hData);
	InternetCloseHandle(hConnection);
	InternetCloseHandle(hInternet);



	auto j = nlohmann::json::parse(s_resultRead, 0, 0);

	if (!j.is_structured())
	{
		_DMESSAGE("SUP_BB::JSON is not structured");
		g_BBLastServerMessage = "Internal server error - JSON not structured. Please take screenshot of this message and send to TommInfinite in Direct Messages as it contains your API key>>" + s_resultRead;
		return 0;
	}

	string s_Message_result;
	int iLikeSuccess = 0;



	if (j.find("success") != j.end())
	{
		iLikeSuccess = j["success"].get<int>();

	}
	else {
		_DMESSAGE("SUP_BB::JSON does not have success key");
		g_BBLastServerMessage = "Internal server error - JSON does not have success key. Please take screenshot of this message and send to TommInfinite in Direct Messages as it contains your API key>>" + s_resultRead;
		return 0;
	}

	if (j.find("message") != j.end())
	{
		s_Message_result = j["message"].get<string>();
	}
	else {
		_DMESSAGE("SUP_BB::JSON does not have message key");
		g_BBLastServerMessage = "Internal server error - JSON does not have message key. Please take screenshot of this message and send to TommInfinite in Direct Messages as it contains your API key>>" + s_resultRead;
		return 0;
	}


	if (iLikeSuccess == 0)
	{
		_DMESSAGE("SUP_BB::JSON success key is 0");
		g_BBLastServerMessage = s_Message_result;
		return 0;
	}

	if (iLikeSuccess == 1)
	{
		g_BBLastServerMessage = s_Message_result;
		f_BB_LikeLocalByID(iMessageID);
		return 1;
	}


	_DMESSAGE("SUP_BB::BBLikeMessage ends");
	return true;
}



















UInt32 cmd_BBGetUserInfo(StaticFunctionTag* base, UInt32 iRequest)
{

	if (iRequest == 0)
	{
		return g_BBPlayerLevel;
	}
	else if (iRequest == 1)
	{
		return g_BBPlayerRating;
	}
	else if (iRequest == 2)
	{
		return g_BBPlayerMaxMessages;
	}
	else if (iRequest == 3)
	{
		return g_BBPlayerMaxWords;
	}
	else if (iRequest == 4)
	{
		return g_BBPlayerCanEnterText;
	}
	else if (iRequest == 5)
	{
		return g_BBPlayerMaxSymbols;
	}

	return true;
}






OnlineMessage* f_BB_GetMessageFromFormID(int currentID)
{

	_DMESSAGE("SUP_BB::f_BB_GetMessageFromFormID starts");

	for (auto const& [key, val] : g_InteriorCellMap)
	{

		for (auto it = val.begin(); it != val.end(); it++)
		{
			if (Iter->MessageID == currentID)
			{
				_DMESSAGE("SUP_BB::MESSAGE FOUND!!");
				return Iter;
			}

		}

	}

	for (auto const& [key, val] : g_ExteriorCellMap)
	{

		for (auto it = val.begin(); it != val.end(); it++)
		{
			if (Iter->MessageID == currentID)
			{
				_DMESSAGE("SUP_BB::MESSAGE FOUND!!");
				return Iter;
			}

		}

	}

	return NULL;
	_DMESSAGE("SUP_BB::ENDS");
}












UInt32 cmd_BBActivatorActivate(StaticFunctionTag* base, TESObjectREFR* reference)
{
	_DMESSAGE("BBActivatorActivate started");
	if (reference == NULL)
	{
		_DMESSAGE("SUP_BB::reference not valid");
		return 0;
	}

	for (auto it = g_Activators.begin(); it != g_Activators.end(); it++)
	{
		_DMESSAGE("Comparing>>>%x with >>>%x", Iter->reference->formID, reference->formID);
		if (Iter->reference->formID == reference->formID)
		{
			_DMESSAGE("SUP_BB::Found activator in array");
			OnlineMessage* MessageTemp = f_BB_GetMessageFromFormID(Iter->currentID);
			if (MessageTemp)
			{
				_DMESSAGE("SUP_BB::Found message, ID>>>%d, type>>>%d", MessageTemp->MessageID, MessageTemp->Type);

				g_BBLastActivatedMessageString = f_BB_GetStringFromMessage(MessageTemp);


				
				_DMESSAGE("message text>>>%s", g_BBLastActivatedMessageString);

				g_CurrentMessageID = MessageTemp->MessageID;
				g_CurrentMessageRating = MessageTemp->Rating;
				return 1;
			}
		}
	}

	return 0;
}



void f_BB_BBOnCellChange()
{
	TESObjectCELL* Cell = (*g_player)->parentCell;

	if (Cell == NULL)
	{
		_DMESSAGE("Player cell doesn't exist - probably on New game.");
		return;
	}

	int iInterior = f_BB_IsCellInterior(Cell);
	_DMESSAGE("SUP_BB::f_BB_BBOnCellChange starts, Interior::%d, refID>>%x", iInterior, Cell->formID);
	int iWasInterior = -1;

	if (g_PreviousCell != NULL)
	{
		iWasInterior = f_BB_IsCellInterior(g_PreviousCell);
	}

	g_PreviousCell = Cell;

	ActivatorInCell.clear();
	TempMessages.clear();
	g_ActivatorsTEMP = g_Activators;
	_DMESSAGE("g_ActivatorsTEMP count is %d", g_ActivatorsTEMP.size());

	double fPosXFinal, fPosYFinal, fPosZFinal;
	float fPosXMarker, fPosYMarker, fPosZMarker;

	Activator_Message* TempActivator = NULL;

	vector<UInt32> CurrentCells{};

	int iExcluded = 0;
	int iTempFound = 0;

	if (iInterior)
	{

		try
		{
			CurrentCells.push_back(Cell->formID);
		}
		catch (...)
		{
			_DMESSAGE("SUP:BB::Error occured when reading from CELL form - INTERIOR");
			return;
		}



		auto it = g_InteriorCellMap.find(Cell);
		if (it != g_InteriorCellMap.end())
		{
			_DMESSAGE("SUP_BB::INTERIOR CELL IS IN ARRAY");

			for (auto it2 = Iter.second.begin(); it2 != Iter.second.end(); it2++)
			{
				TempMessages.push_back(Iter2);
			}



			for (auto it3 = g_ActivatorsTEMP.begin(); it3 != g_ActivatorsTEMP.end();) // looking for refs with ID set to exclude them.
			{
				iExcluded = 0;

				for (auto it2 = TempMessages.begin(); it2 != TempMessages.end(); it2++)
				{
					if (Iter3->currentID == Iter2->MessageID)
					{

						_DMESSAGE("SUP_BB::Assigning activator>>%X to message>>%d, Same reference by ID.", Iter3->reference->formID, Iter2->MessageID);
						it3 = g_ActivatorsTEMP.erase(it3);
						it2 = TempMessages.erase(it2);
						iExcluded = 1;
						break;
					}

				}

				if (iExcluded != 1)
				{
					for (auto it4 = CurrentCells.begin(); it4 != CurrentCells.end(); it4++)
					{

						try
						{
							if (Iter3->reference->parentCell && Iter3->reference->parentCell->formID == Iter4)
							{
								ActivatorInCell.push_back(Iter3);
							}
						}
						catch (...)
						{
							_DMESSAGE("SUP:BB::Error occured when filling ActivatorInCell");
						}
					}
					it3++;
				}


			}




			int iTempActionsCount = TempMessages.size();

			if (iTempActionsCount != 0)
			{

				int iActivatorsInCellCount = ActivatorInCell.size();

				for (auto it2 = TempMessages.begin(); it2 != TempMessages.end();) // Excluding refs in current cell - setting new pos to them
				{
					if (ActivatorInCell.size() == 0)
					{
						_DMESSAGE("SUP_BB::Activators in cell are no longer present");
						break;
					}

					iTempFound = 0;

					for (auto it3 = ActivatorInCell.begin(); it3 != ActivatorInCell.end(); it3++)
					{

						std::vector<Activator_Message*>::iterator iterTemp = std::find(g_ActivatorsTEMP.begin(), g_ActivatorsTEMP.end(), Iter3);

						if (iterTemp != g_ActivatorsTEMP.end())
						{
							(*iterTemp)->currentID = Iter2->MessageID;
							g_ActivatorsTEMP.erase(iterTemp);
						}
						else {
							_DMESSAGE("SUP_BB::WARNING - ACTIVATOR IN CELL WAS NOT IN TEMP ARRAY");
						}

						_DMESSAGE("SUP_BB::Assigning activator>>%X to message>>%d, Same cell.", Iter3->reference->formID, Iter2->MessageID);
						f_BB_Ref_ChangePosInCell(Iter3, Iter2->PosX, Iter2->PosY, Iter2->PosZ);
						ActivatorInCell.erase(it3);
						it2 = TempMessages.erase(it2);
						iTempFound = 1;
						break;
					}


					if (iTempFound == 0)
					{
						it2++;
					}
				}


				for (auto it2 = TempMessages.begin(); it2 != TempMessages.end();) // now only refs in other cell remains - we move them
				{

					iTempFound = 0;

					for (auto it3 = g_ActivatorsTEMP.begin(); it3 != g_ActivatorsTEMP.end(); it3++)
					{
						_DMESSAGE("SUP_BB::Assigning activator>>%X to message>>%d, Same cell.", Iter3->reference->formID, Iter2->MessageID);
						Iter3->currentID = Iter2->MessageID;
						f_BB_Ref_MoveToCell(Iter3, Iter2->PosX, Iter2->PosY, Iter2->PosZ);
						g_ActivatorsTEMP.erase(it3);
						it2 = TempMessages.erase(it2);
						iTempFound = 1;
						break;
					}
					if (iTempFound == 0)
					{
						it2++;
					}

				}



			}

		}

		f_BB_Move_ALL_Back_To_DefaultCell(CurrentCells);


	}
	else {


		CurrentCells = f_BB_GetNearCells(Cell); ////////////////

		bool bCellsFound = false;


		for (auto it2 = CurrentCells.begin(); it2 != CurrentCells.end(); it2++)
		{

			map<UInt32, vector<OnlineMessage*>>::iterator it;

			it = g_ExteriorCellMap.find(Iter2);

			if (it != g_ExteriorCellMap.end())
			{
				_DMESSAGE("SUP_BB::Cell found>> %x", Iter2);
				bCellsFound = true;

				for (auto it3 = Iter.second.begin(); it3 != Iter.second.end(); it3++)
				{
					TempMessages.push_back(Iter3);
				}
			}
		}


		if (bCellsFound)
		{
			_DMESSAGE("SUP_BB::EXTERIOR CELL IS IN ARRAY");

			for (auto it3 = g_ActivatorsTEMP.begin(); it3 != g_ActivatorsTEMP.end();) // looking for refs with ID set to exclude them.
			{
				iExcluded = 0;

				for (auto it2 = TempMessages.begin(); it2 != TempMessages.end(); it2++)
				{
					if (Iter3->currentID == Iter2->MessageID)
					{
						_DMESSAGE("SUP_BB::Assigning to message>>%d, Same reference by ID.",  Iter2->MessageID);
						it3 = g_ActivatorsTEMP.erase(it3);
						it2 = TempMessages.erase(it2);
						iExcluded = 1;
						break;
					}

				}

				if (iExcluded != 1)
				{
					for (auto it4 = CurrentCells.begin(); it4 != CurrentCells.end(); it4++){

						try
						{
							if (Iter3->reference && Iter3->reference->parentCell && Iter3->reference->parentCell->formID == Iter4) {
								ActivatorInCell.push_back(Iter3);
							}
						}
						catch (...)
						{
							_DMESSAGE("SUP:BB::Error occured when filling ActivatorInCell - EXTERIOR");
						}

					}
					it3++;
				}


			}

			int iTempActionsCount = TempMessages.size();
			_DMESSAGE("SUP_BB::Messages remaining to assign>>>%d", iTempActionsCount);
			if (iTempActionsCount != 0)
			{

				int iActivatorsInCellCount = ActivatorInCell.size();

				for (auto it2 = TempMessages.begin(); it2 != TempMessages.end();) // Excluding refs in current cell - setting new pos to them
				{
					if (ActivatorInCell.size() == 0){
						_DMESSAGE("SUP_BB::Activators in cell are no longer present");
						break;
					}

					iTempFound = 0;

					for (auto it3 = ActivatorInCell.begin(); it3 != ActivatorInCell.end(); it3++)
					{
						std::vector<Activator_Message*>::iterator iterTemp = std::find(g_ActivatorsTEMP.begin(), g_ActivatorsTEMP.end(), Iter3);

						if (iterTemp != g_ActivatorsTEMP.end())
						{
							(*iterTemp)->currentID = Iter2->MessageID;
							g_ActivatorsTEMP.erase(iterTemp);
						}
						else {
							_DMESSAGE("SUP_BB::WARNING - ACTIVATOR IN CELL WAS NOT IN TEMP ARRAY");
						}

						_DMESSAGE("SUP_BB::Assigning activator>>%X to message>>%d, Change pos in Cell.", Iter3->reference->formID, Iter2->MessageID);
						f_BB_Ref_ChangePosInCell(Iter3, Iter2->PosX, Iter2->PosY, Iter2->PosZ);
						ActivatorInCell.erase(it3);
						it2 = TempMessages.erase(it2);
						iTempFound = 1;
						break;
					}

					if (iTempFound == 0)
					{
						it2++;
					}
				}


				for (auto it2 = TempMessages.begin(); it2 != TempMessages.end();) // now only refs in other cell remains - we move them
				{

					iTempFound = 0;

					for (auto it3 = g_ActivatorsTEMP.begin(); it3 != g_ActivatorsTEMP.end(); it3++)
					{
						_DMESSAGE("SUP_BB::Assigning activator>>%X to message>>%d.", Iter3->reference->formID, Iter2->MessageID);
						Iter3->currentID = Iter2->MessageID;

						f_BB_Ref_MoveToCell(Iter3, Iter2->PosX, Iter2->PosY, Iter2->PosZ);


						g_ActivatorsTEMP.erase(it3);
						it2 = TempMessages.erase(it2);
						iTempFound = 1;

						break;
					}
					if (iTempFound == 0)
					{
						it2++;
					}

				}
				_DMESSAGE("Temp Messages left>>>%d", TempMessages.size());

			}
		}
		f_BB_Move_ALL_Back_To_DefaultCell(CurrentCells);
	}
}





void cmd_BBOnCellChange(StaticFunctionTag* base)
{
	f_BB_BBOnCellChange();
}




















UInt32 cmd_BBPostMessage(StaticFunctionTag* base, UInt32 iMessageType, UInt32 iLaunchValue)
{

	if (iBBAPIKeyLoaded == 0)
	{
		_DMESSAGE("SUP_BB::iBBAPIKeyLoaded not loaded>>");
		g_BBLastServerMessage = "BB key is not loaded";
		return 0;
	}


	if (iLaunchValue != g_S_Value)
	{
		_DMESSAGE("SUP_BB::Launch value wrong >>");
		g_BBLastServerMessage = "Launch value is wrong. Please restart the game. If it doesn't help - reinstall the mod package.";
		return 0;
	}



	nlohmann::json jMessage;



	if (iMessageType == t_BB_MessageTypeSimple)
	{
		if (v_CurrentMessage.size() == 0)
		{
			_DMESSAGE("SUP_BB::Message is empty");
			g_BBLastServerMessage = "You're trying to send an empty message";
			return 0;
		}


		for (auto it = v_CurrentMessage.begin(); it != v_CurrentMessage.end(); it++)
		{
			jMessage.push_back(Iter);
		}

	}
	else {

		jMessage["Text"] = sCurrentMessage_ToSend;
	}



	_DMESSAGE("SUP_BB::Message to send>>>>%s", jMessage.dump());





	TESObjectCELL* ChosenCell = (*g_player)->parentCell;

	if (ChosenCell == NULL)
	{
		_DMESSAGE("SUP_BB::Chosen Cell doesn't exist.");
		g_BBLastServerMessage = "Parent Cell doesn't exist.";
		return 0;
	}

	float fPosX = (*g_player)->pos.x;
	float fPosY = (*g_player)->pos.y;
	float fPosZ = (*g_player)->pos.z;

	int iInterior = f_BB_IsCellInterior(ChosenCell);

	_DMESSAGE("Cell interior>>>%d", iInterior);

	UInt8 CellSourcePluginID = (UInt8)(ChosenCell->formID >> 24);
	
	_DMESSAGE("1");
	const ModInfo* ChosenMod = f_GetLoadedModNameByIndex(CellSourcePluginID);
	_DMESSAGE("2");
	if (ChosenMod == NULL)
	{
		_DMESSAGE("SUP_BB::Chosen mod is NULL");
		g_BBLastServerMessage = "The cell you're in comes from an ELS plugin. BB doesn't support ELS plugin at the moment.";
		return 0;
	}
	//_DMESSAGE("3");
	const char* CellSourcePluginName = ChosenMod->name;
	string s_CellSourcePluginName{};
	_DMESSAGE("4");
	if (CellSourcePluginName)
	{
		_DMESSAGE("s_CellSourcePluginName is>>>%s", s_CellSourcePluginName.c_str());
		s_CellSourcePluginName = CellSourcePluginName;
	}
	else
	{
		_DMESSAGE("SUP_BB::Cannot find Cell source plugin name>>");
		g_BBLastServerMessage = "Cannot find Cell source plugin name - that is a rare case - please report to TommInfnite..";
		return 0;
	}

	_DMESSAGE("5");
	_DMESSAGE("SUP_BB::Cell FORM ID is>> %d", ChosenCell->formID & 0xFFFFFF);
	int i_CellFormID = ChosenCell->formID & 0xFFFFFF;
	_DMESSAGE("6");
	nlohmann::json jCellInfo;
	_DMESSAGE("7");
	jCellInfo["x"] = fPosX;
	jCellInfo["y"] = fPosY;
	jCellInfo["z"] = fPosZ;
	jCellInfo["is_interior"] = iInterior;
	jCellInfo["cell_formID"] = i_CellFormID;
	jCellInfo["cell_pluginname"] = s_CellSourcePluginName;

	//_DMESSAGE("0");
	/*if (iInterior == 0)
	{
		_DMESSAGE("1");
		if (ChosenCell->worldSpace)
		{
			_DMESSAGE("2");
			UInt8 WorldSpaceSourcePluginID = (UInt8)(ChosenCell->worldSpace->formID >> 24);

			const ModInfo* ChosenModWRLDSPACE = f_GetLoadedModNameByIndex(WorldSpaceSourcePluginID);

			if (ChosenModWRLDSPACE == NULL)
			{
				_DMESSAGE("SUP_BB::Chosen mod WRLDSPPACE is NULL");
				g_BBLastServerMessage = "Unable to extract plugin from worldspace form ID.";
				return 0;
			}


			const char* WorldSpaceSourcePluginName = ChosenMod->name;
			string s_WorldSpaceSourcePluginName{};

			if (WorldSpaceSourcePluginName)
			{
				_DMESSAGE("3");
				s_WorldSpaceSourcePluginName = WorldSpaceSourcePluginName;
				_DMESSAGE("s_WorldSpaceSourcePluginName is>>>%s", s_WorldSpaceSourcePluginName.c_str());

				int i_WorldSpaceFormID = ChosenCell->worldSpace->formID & 0xFFFFFF;
				jCellInfo["worldspace_formID"] = i_WorldSpaceFormID;
				jCellInfo["worldspace_pluginname"] = s_WorldSpaceSourcePluginName;
			}
			else
			{
				_DMESSAGE("SUP_BB::Cannot find WorldSpacesource plugin name>>");
				g_BBLastServerMessage = "Cannot find WorldSpace source plugin name - that is a rare case - please report to TommInfnite..";
				return 0;
			}

		}
		else {
			_DMESSAGE("SUP_BB::Cannot find Cell worldspace");
			g_BBLastServerMessage = "Cannot find Cell worldspace - that is a rare case - please report to TommInfnite.";
			return 0;
		}

	}*/







	nlohmann::json jIN;
	jIN["apikey"] = s_BBApiKey;
	jIN["message"] = jMessage.dump();
	jIN["CellInfo"] = jCellInfo;
	jIN["APIVersion"] = t_BB_API_VER;
	jIN["content-type"] = "application/json";
	string s_RequestFINAL = jIN.dump();



	HINTERNET hInternet = InternetOpenA("InetURL/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	HINTERNET hConnection = InternetConnectA(hInternet, "Tomminfinite.pythonanywhere.com", 80, " ", " ", INTERNET_SERVICE_HTTP, 0, 0);
	HINTERNET hData = HttpOpenRequestA(hConnection, "POST", "/FO4MessagesPOST", NULL, NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION, 0);
	HttpAddRequestHeadersA(hData, "content-type: application/json", -1, HTTP_ADDREQ_FLAG_ADD);


	char buf[2048 + 1];

	MyThreadBB MyNewThread;
	MyNewThread.CheckType = t_CheckType_Nexus;

	thread NewThread(&MyThreadBB::Execute, std::ref(MyNewThread));
	NewThread.detach();


	MyNewThread.stage = 5;
	MyNewThread.hData = hData;

	_DMESSAGE("SUP_BB::SENDING>>>%s", s_RequestFINAL.c_str());
	HttpSendRequestA(hData, NULL, 0, (LPVOID)s_RequestFINAL.c_str(), s_RequestFINAL.length());
	MyNewThread.stage = 110;


	int iStatCode = f_GetStatCodeResponse(&hData);
	_DMESSAGE("SUP_BB::iStatCode is %d", iStatCode);
	if (iStatCode == 0)
	{
		_DMESSAGE("SUP_BB::Server not responding");
		g_BBLastServerMessage = "Server is not responding or internet connection is absent";
		return 0;
	}
	else if (iStatCode == 404)
	{
		_DMESSAGE("SUP_BB::Page does not exist");
		g_BBLastServerMessage = "Page on server doesn't exist.";
		return 0;
	}
	else if (iStatCode == 500)
	{
		_DMESSAGE("SUP_BB::stat code 500");
		g_BBLastServerMessage = "Internal server error. Please take screenshot of this message and send to TommInfinite in Direct Messages as it contains your API key>>" + s_RequestFINAL;
		return 0;
	}
	else if (iStatCode != 200)
	{
		_DMESSAGE("SUP_BB::statcode unedentified>>>%d", iStatCode);
		g_BBLastServerMessage = "Problem connecting to server. Please try again later.>>";
		return 0;
	}




	DWORD bytesRead = 0;
	DWORD totalBytesRead = 0;

	string s_resultRead{};

	while (InternetReadFile(hData, buf, 2048, &bytesRead) && bytesRead != 0)
	{
		_DMESSAGE("SUP_BB::buf is %s", buf);
		s_resultRead.append(buf, bytesRead);
		totalBytesRead += bytesRead;
	}
	InternetCloseHandle(hData);
	InternetCloseHandle(hConnection);
	InternetCloseHandle(hInternet);



	auto j = nlohmann::json::parse(s_resultRead, 0, 0);

	if (!j.is_structured())
	{
		_DMESSAGE("SUP_BB::JSON is not structured");
		g_BBLastServerMessage = "Internal server error - JSON not structured. Please take screenshot of this message and send to TommInfinite in Direct Messages as it contains your API key>>" + s_resultRead;
		return 0;
	}

	string s_Message_result;
	int iMessageID = -1;
	int iInsertionSuccess = 0;




	if (j.find("success") != j.end())
	{
		iInsertionSuccess = j["success"].get<int>();

	}
	else {
		_DMESSAGE("SUP_BB::JSON does not have success key");
		g_BBLastServerMessage = "Internal server error - JSON does not have success key. Please take screenshot of this message and send to TommInfinite in Direct Messages as it contains your API key>>" + s_resultRead;
		return 0;
	}

	if (j.find("message") != j.end())
	{
		s_Message_result = j["message"].get<string>();
	}
	else {
		_DMESSAGE("SUP_BB::JSON does not have message key");
		g_BBLastServerMessage = "Internal server error - JSON does not have message key. Please take screenshot of this message and send to TommInfinite in Direct Messages as it contains your API key>>" + s_resultRead;
		return 0;
	}


	if (iInsertionSuccess == 0)
	{
		_DMESSAGE("SUP_BB::JSON success key is 0");
		g_BBLastServerMessage = s_Message_result;
		return 0;
	}


	if (j.find("id") != j.end())
	{
		iMessageID = j["id"].get<int>();
	}
	else {
		_DMESSAGE("SUP_BB::JSON does not have apikey key");
		g_BBLastServerMessage = "Internal server error - JSON does not have apikey ID key. Please take screenshot of this message and send to TommInfinite in Direct Messages as it contains your API key>>" + s_resultRead;
		return 0;
	}


	if (iInsertionSuccess == 1)
	{
		g_BBLastServerMessage = s_Message_result;
		fPosZ = fPosZ + 3;

		if (iMessageType == t_BB_MessageTypeSimple)
		{
			f_BB_AddMessageToLocalArray(iMessageID, v_CurrentMessage, fPosX, fPosY, fPosZ, iInterior, ChosenCell, ChosenCell->formID, 0);
		}
		else {
			f_BB_AddMessageToLocalArray_FreeText(iMessageID, sCurrentMessage_ToSend, fPosX, fPosY, fPosZ, iInterior, ChosenCell, ChosenCell->formID, 0);
			sCurrentMessage_ToSend.clear();
		}



		_DMESSAGE("SUP_BB::Result is>>>> %s", s_resultRead.c_str());
		f_BB_BBOnCellChange();
		return 1;
	}

	_DMESSAGE("SUP_BB::Result is>>>> %s", s_resultRead.c_str());
	g_BBLastServerMessage = "Unindentified error";
	return 0;
}



UInt32 cmd_BBSetMessageForMainMenu(StaticFunctionTag* base)
{

	if (iMessagesFetched)
	{
		string s_temp = "Connected, fetched " + to_string(iMessagesFetchedCount) + " messages.";
		f_BB_SetMessageBodyText(s_temp, 1);

	}
	else {

		if (g_BB_NeedToRestartTheGame)
		{
			f_BB_SetMessageBodyText("YOU NEED TO RESTART THE GAME IN ORDER FOR BUILDING BRIDGES TO WORK.", 1);
		}
		else if (iBBStage < iBBDoneStage)
		{
			f_BB_SetMessageBodyText("Connecting to fetch messages - please bring this menu up in 10-15 seconds", 1);
		}
		else {
			if (g_BBShowError)
			{
				f_BB_SetMessageBodyText(s_BB_Error, 1);
			}
			else
			{
				f_BB_SetMessageBodyText("Offline, unidentified reason - please reinstall whole BB package.", 1);
			}

		}

	}

	TESObjectCELL* Cell = (*g_player)->parentCell;

	if (f_BB_IsCellInterior(Cell))
	{

		const char* FullName = Cell->GetFullName();

		if (FullName)
		{
			f_BB_SetMessageBodyText(FullName, 2);
		}
		else {
			f_BB_SetMessageBodyText("NoName", 2);
		}
	}
	else {
		std::stringstream ss;
		ss << std::hex << Cell->formID; // int decimal_value
		std::string res(ss.str());

		f_BB_SetMessageBodyText(res, 2);
	}

	if (g_BB_HideServerMessage == 1)
	{
		if (iServerMessageFound)
		{
			f_BB_SetMessageBodyText("Exists but hidden.", 3);
		}
		else {
			f_BB_SetMessageBodyText("None.", 3);
		}

	}
	else {
		if (iServerMessageFound)
		{
			f_BB_SetMessageBodyText(s_Server_Message, 3);
		}
		else {
			f_BB_SetMessageBodyText("None.", 3);
		}
	}





	return 1;
}






void cmd_BBSetLastServerMessageForMessageBox(StaticFunctionTag* base)
{
	f_BB_SetMessageBodyText(g_BBLastServerMessage,1);
	return;
}


void cmd_BBSetLastActivatedMessageForMessageBox(StaticFunctionTag* base)
{
	f_BB_SetMessageBodyText(g_BBLastActivatedMessageString,1);
	return;
}

UInt32 cmd_BBGetLastActivatedMessageRating(StaticFunctionTag* base)
{
	return g_CurrentMessageRating;
}



UInt32 cmd_BBGetBBKey(StaticFunctionTag* base)
{

	if (iBBNexusAPIKeyLoaded == 0)
	{
		_DMESSAGE("SUP_BB::Nexus API key not present>>");
		g_BBLastServerMessage = "Nexus API key not present.";
		return 0;
	}

	//if (iBBKeyRequested)
	//{
	//	_DMESSAGE("You already requsted a new BB key");
	//	g_BBLastServerMessage = "You already requsted a new BB key.";
	//	return 0;
	//}





	HINTERNET hInternet = InternetOpenA("InetURL/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	HINTERNET hConnection = InternetConnectA(hInternet, "Tomminfinite.pythonanywhere.com", 80, " ", " ", INTERNET_SERVICE_HTTP, 0, 0);
	HINTERNET hData = HttpOpenRequestA(hConnection, "POST", "/FO4MessagesFirstTimeLogin", NULL, NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION, 0);
	HttpAddRequestHeadersA(hData, "content-type: application/json", -1, HTTP_ADDREQ_FLAG_ADD);


	char buf[2048 + 1];

	MyThreadBB MyNewThread;
	MyNewThread.CheckType = t_CheckType_Nexus;

	thread NewThread(&MyThreadBB::Execute, std::ref(MyNewThread));
	NewThread.detach();


	MyNewThread.stage = 5;
	MyNewThread.hData = hData;


	string s_BodyTemp = R"({"NexusApiKey" : ")" + s_BBNexusAPIKey + R"(", "APIVersion" : )" + to_string(t_BB_API_VER) + R"(})";


	_DMESSAGE("SUP_BB::SENDING>>>%s", s_BodyTemp.c_str());
	HttpSendRequestA(hData, NULL, 0, (LPVOID)s_BodyTemp.c_str(), s_BodyTemp.length());
	MyNewThread.stage = 110;


	int iStatCode = f_GetStatCodeResponse(&hData);
	_DMESSAGE("SUP_BB::iStatCode is %d", iStatCode);
	if (iStatCode == 0)
	{
		_DMESSAGE("SUP_BB::Server not responding");
		g_BBLastServerMessage = "Server is not responding or internet connection is absent";
		return 0;
	}
	else if (iStatCode == 404)
	{
		_DMESSAGE("SUP_BB::Page does not exist");
		g_BBLastServerMessage = "Page on server doesn't exist.";
		return 0;
	}
	else if (iStatCode == 500)
	{
		_DMESSAGE("SUP_BB::stat code 500");
		g_BBLastServerMessage = "Internal server error. Please take screenshot of this message and send to TommInfinite in Direct Messages as it contains your API key>>" + s_BodyTemp;
		return 0;
	}
	else if (iStatCode != 200)
	{
		_DMESSAGE("SUP_BB::statcode unedentified>>>%d", iStatCode);
		g_BBLastServerMessage = "Problem connecting to server. Please try again later.>>";
		return 0;
	}






	DWORD bytesRead = 0;
	DWORD totalBytesRead = 0;

	string s_resultRead{};

	while (InternetReadFile(hData, buf, 2048, &bytesRead) && bytesRead != 0)
	{
		_DMESSAGE("SUP_BB::buf is %s", buf);
		s_resultRead.append(buf, bytesRead);
		totalBytesRead += bytesRead;
	}
	InternetCloseHandle(hData);
	InternetCloseHandle(hConnection);
	InternetCloseHandle(hInternet);



	auto j = nlohmann::json::parse(s_resultRead, 0, 0);

	if (!j.is_structured())
	{
		_DMESSAGE("SUP_BB::JSON is not structured");
		g_BBLastServerMessage = "Internal server error - JSON not structured. Please take screenshot of this message and send to TommInfinite in Direct Messages as it contains your API key>>" + s_resultRead;
		return 0;
	}

	string s_Message_result;
	int iNewUser = 0;
	string s_Apikey_Result{};
	int iInsertionSuccess = 0;


	if (j.find("message") != j.end())
	{
		s_Message_result = j["message"].get<string>();
	}
	else {
		_DMESSAGE("SUP_BB::JSON does not have message key");
		g_BBLastServerMessage = "Internal server error - JSON does not have message key. Please take screenshot of this message and send to TommInfinite in Direct Messages as it contains your API key>>" + s_resultRead;
		return 0;
	}



	if (j.find("success") != j.end())
	{
		iInsertionSuccess = j["success"].get<int>();

		if (iInsertionSuccess == 0)
		{
			_DMESSAGE("SUP_BB::JSON success key is 0");
			g_BBLastServerMessage = s_Message_result;
			return 0;
		}



	}
	else {
		_DMESSAGE("SUP_BB::JSON does not have success key");
		g_BBLastServerMessage = "Internal server error - JSON does not have success key. Please take screenshot of this message and send to TommInfinite in Direct Messages as it contains your API key>>" + s_resultRead;
		return 0;
	}









	if (j.find("apikey") != j.end())
	{
		s_Apikey_Result = j["apikey"].get<string>();
	}
	else {

		_DMESSAGE("SUP_BB::JSON does not have apikey key");
		g_BBLastServerMessage = "Internal server error - JSON does not have apikey ID key. Please take screenshot of this message and send to TommInfinite in Direct Messages as it contains your API key>>" + s_resultRead;
		return 0;
	}



	if (j.find("new_user") != j.end())
	{

	}
	else {
		_DMESSAGE("SUP_BB::JSON does not have new_user key");
		g_BBLastServerMessage = "Internal server error - JSON does not have new_user key. Please take screenshot of this message and send to TommInfinite in Direct Messages as it contains your API key>>" + s_resultRead;
		return 0;
	}


	iNewUser = j["new_user"].get<int>();

	g_BBLastServerMessage = s_Message_result;

	f_BB_Store_BB_KEY(s_Apikey_Result.c_str());
	iBBKeyRequested = 1;

	//_DMESSAGE("SUP_BB::Result is>>>> %s", s_resultRead.c_str());
	//Console_Print("SUP_BB::Result is>>>> %s", s_resultRead.c_str());

	g_BB_NeedToRestartTheGame = 1;
	if (iNewUser == 1)
	{
		return 1;
	}
	else {
		return 2;
	}
	
}















void f_BB_SetMessageButtons(vector<string> vStrings)
{
	_DMESSAGE("f_BB_SetMessageButtons started");
	g_BBCurrentCantFindWordsByRequest = 0;
	BSSimpleList<MESSAGEBOX_BUTTON*>* IterationList = &g_TextInputMessage->buttonList;

	if (vStrings.size() == 0)
	{
		IterationList->elem->text = "SOMETHING WENT WRONG - NO BUTTONS TO ASSIGN";
		return;
	}



	IterationList->elem->text = vStrings.at(0).c_str();
	_DMESSAGE("Assigning>> %s to button>>>%d", vStrings.at(0).c_str(), 0);
	int iButtonCount = 0;

	if (vStrings.size() == 1) // Cannot find words by request
	{
		g_BBCurrentCantFindWordsByRequest = 1;
		return;
	}

	int iTemp;

	while (IterationList = IterationList->next)
	{
		iButtonCount += 1;
		_DMESSAGE("New iteration, button count is %d", iButtonCount);
		iTemp = iButtonCount + 1;
		if (iTemp > vStrings.size())
		{
			_DMESSAGE("Max array size reached");
			break;
		}
		else {
			_DMESSAGE("Assigning>> %s to button>>>%d", vStrings.at(iButtonCount).c_str(), iButtonCount);
			IterationList->elem->text = vStrings.at(iButtonCount).c_str();
		}

	}

}



BBWordType* f_BB_GetBBWordClassByKey(int iArrayKey)
{
	for (auto it = g_BBAllWords.begin(); it != g_BBAllWords.end(); it++)
	{
		if (Iter->StartKey == iArrayKey)
		{
			return Iter;
		}

	}

	return NULL;
}



vector<string> f_BBGetCurrentVerbForms()
{
	vector<string> result;
	for (auto it = g_CurrentMultiWord->vWords.begin(); it != g_CurrentMultiWord->vWords.end(); it++)
	{
		if (Iter.second.length() > 0)
		{
			result.push_back(Iter.second);//0
		}
	}

	//if (g_CurrentMultiWord->iType == t_BB_WordTypeNoun)
	//{
	//	
	//	result.push_back(g_CurrentMultiWord->vWords.at(1).second);//0

	//}
	//else {
	//	result.push_back(g_CurrentMultiWord->vWords.at(0).second);//0
	//	result.push_back(g_CurrentMultiWord->vWords.at(1).second);//0
	//	result.push_back(g_CurrentMultiWord->vWords.at(2).second);//0
	//	result.push_back(g_CurrentMultiWord->vWords.at(3).second);//0
	//}






	//result.push_back(g_CurrentVerb->Word);//0
	//result.push_back(g_CurrentVerb->VerbTP);//1
	//result.push_back(g_CurrentVerb->VerbPast);//2
	//result.push_back(g_CurrentVerb->VerbGerund);//3
	
	return result;
}



string f_BB_GetTextEntryCurrentLetters(vector<string> ArrayIn)
{
	vector<char> FirstLettersArray;
	string result{};
	int iCount = 0;

	for (auto it = ArrayIn.begin(); it != ArrayIn.end(); ++it) 
	{
		if (iCount < 3)
		{
			iCount += 1;
			continue;
		}

		if (Iter.length() > 0)
		{
			if (std::find(FirstLettersArray.begin(), FirstLettersArray.end(), Iter.at(0)) != FirstLettersArray.end())
			{
			}
			else
			{
				FirstLettersArray.push_back(Iter.at(0));
			}

		}

	}

	result += "(";

	for (auto it = FirstLettersArray.begin(); it != FirstLettersArray.end(); ++it)
	{
		result += toupper((Iter));

		if (it != std::prev(FirstLettersArray.end()))
		{
			result += "-";
		}

	}


	result += ")";
	return result;


	//if (result.size() > 3)
	//{
	//	if (result.at(3).length() > 0)
	//	{
	//		s_TextEntryCurrentPageLetterStart = toupper(result.at(3).at(0));
	//	}
	//	if (result.back().length() > 0)
	//	{
	//		s_TextEntryCurrentPageLetterFinish = toupper(result.back().at(0));
	//	}

	//}



	//s_TextEntryCurrentLetters

}


vector<string> f_BBGetFilteredStringsFromPool(int iArrayKey)
{

	vector<string> result;

	for (auto it = g_FilteredWords->vWords.begin(); it != g_FilteredWords->vWords.end(); it++)
	{
		delete Iter;
	}

	g_FilteredWords->vWords.clear();
	_DMESSAGE("g_CurrentFilterRequest is %s", g_CurrentFilterRequest.c_str());
	//Console_Print("g_CurrentFilterRequest is %s", g_CurrentFilterRequest.c_str());

	if (g_CurrentFilterRequest.length() == 0)
	{
		_DMESSAGE("Request is empty.");
		result.push_back("YOUR REQUEST IS EMPTY");
		return result;
	}




	for (auto it = g_BBAllWords.begin(); it != g_BBAllWords.end(); it++)
	{
		for (auto it2 = Iter->vWords.begin(); it2 != Iter->vWords.end(); it2++)
		{
			//_DMESSAGE("1");
			for (auto it3 = Iter2->vWords.begin(); it3 != Iter2->vWords.end(); it3++)
			{
				//_DMESSAGE("2");
				//_DMESSAGE("Comparing>>%s", Iter3.second.c_str());
				bool b_res = boost::icontains(Iter3.second.c_str(), g_CurrentFilterRequest.c_str());

				//if (b_res == 0)
				//{
				//	b_res = (Iter3.second == g_CurrentFilterRequest);
				//}


				//_DMESSAGE("3");
				if (b_res)
				{
					_DMESSAGE("Found %s in %s", g_CurrentFilterRequest.c_str(), Iter3.second.c_str());

					if (std::find(g_FilteredWords->vWords.begin(), g_FilteredWords->vWords.end(), Iter2) == g_FilteredWords->vWords.end()) {

						BBWord* NewWord = new BBWord;
						NewWord->vWords = Iter2->vWords;
						NewWord->iType = Iter2->iType;
						_DMESSAGE("ADDING WORD TO FILTERED VECTOR");
						g_FilteredWords->vWords.push_back(NewWord);
						//_DMESSAGE("3.5");
					}

					//_DMESSAGE("4");
					break;
				}
			}
		}
	}



	int iCount = 0;
	_DMESSAGE("Sorting.");

	for (auto it2 = g_FilteredWords->vWords.begin(); it2 != g_FilteredWords->vWords.end(); ++it2) {
		Iter2->iKeySorted = iCount + g_FilteredWords->StartKey;
		iCount += 1;
	}



	_DMESSAGE("f_BBGetFilteredStringsFromPool starts with request>>>%d", iArrayKey);

	BBWordType* WordArray = f_BB_GetBBWordClassByKey(iArrayKey);
	if (WordArray == NULL)
	{
		_DMESSAGE("Can't find words by request");
		result.push_back("CANNOT FIND WORDS BY REQUEST");
		return result;
	}

	int iArraySize = WordArray->vWords.size();

	_DMESSAGE("Original array size is>>>%d", iArraySize);

	if (iArraySize == 0)
	{
		_DMESSAGE("Can't find words by filtered request");
		result.push_back("CANNOT FIND WORDS BY YOUR REQUEST");
		return result;
	}


	float fTemp = iArraySize / float(17);
	_DMESSAGE("fTemp->>>%f", fTemp);
	g_TextInputOffsetPageMax = ceil(fTemp);
	_DMESSAGE("Max pages>>> %d", g_TextInputOffsetPageMax);



	g_TextInputCurrentNextButtonNumber = 0;
	result.push_back("[Next]");
	g_TextInputCurrentPreviousButtonNumber = g_TextInputCurrentNextButtonNumber + 1;
	result.push_back("[Previous]");
	g_TextInputCurrentBackButtonNumber = g_TextInputCurrentNextButtonNumber + 2;
	result.push_back("[Back]");

	_DMESSAGE("Current Next Button number>>>%d", g_TextInputCurrentNextButtonNumber);
	_DMESSAGE("Current Previous Button number>>>%d", g_TextInputCurrentPreviousButtonNumber);
	_DMESSAGE("Current Back Button number>>>%d", g_TextInputCurrentBackButtonNumber);


	int iMaxRequest = iArrayKey + iArraySize;
	_DMESSAGE("Max array size>>>> %d", iMaxRequest);

	int iRequestFrom = iArrayKey + (g_TextInputOffsetPage * 17);
	int iRequestTo = iRequestFrom + 16;

	if (iRequestTo > iMaxRequest)
	{
		iRequestTo = iRequestTo - (iRequestTo - iMaxRequest);
		_DMESSAGE("RequestTo is exceeding iMaxRequest - new value is %d", iRequestTo);
	}
	_DMESSAGE("iRequestFROM is %d, RequestTO is %d", iRequestFrom, iRequestTo);

	for (auto it2 = WordArray->vWords.begin(); it2 != WordArray->vWords.end(); ++it2) {

		if (Iter2->iKeySorted >= iRequestFrom && Iter2->iKeySorted <= iRequestTo)
		{
			_DMESSAGE("FOUND KEY>>>%d, value is %s", Iter2->iKeySorted, Iter2->vWords.at(0).second.c_str());
			result.push_back(Iter2->vWords.at(0).second);
		}

	}


	s_TextEntryCurrentLetters = f_BB_GetTextEntryCurrentLetters(result);


	return result;
}







vector<string> f_BBGetStringsFromPool(int iArrayKey)
{
	_DMESSAGE("f_BBGetStringsFromPool starts with request>>>%d", iArrayKey);
	vector<string> result;
	BBWordType* WordArray = f_BB_GetBBWordClassByKey(iArrayKey);
	if (WordArray == NULL)
	{
		_DMESSAGE("Can't find words by request");
		result.push_back("CANNOT FIND WORDS BY REQUEST");
		return result;
	}

	int iArraySize = WordArray->vWords.size();
	_DMESSAGE("Original array size is>>>%d", iArraySize);
	float fTemp = iArraySize / float(17);
	_DMESSAGE("fTemp->>>%f", fTemp);
	g_TextInputOffsetPageMax = ceil(fTemp);
	_DMESSAGE("Max pages>>> %d", g_TextInputOffsetPageMax);



	g_TextInputCurrentNextButtonNumber = 0;
	result.push_back("[Next]");
	g_TextInputCurrentPreviousButtonNumber = g_TextInputCurrentNextButtonNumber + 1;
	result.push_back("[Previous]");
	g_TextInputCurrentBackButtonNumber = g_TextInputCurrentNextButtonNumber + 2;
	result.push_back("[Back]");

	_DMESSAGE("Current Next Button number>>>%d", g_TextInputCurrentNextButtonNumber);
	_DMESSAGE("Current Previous Button number>>>%d", g_TextInputCurrentPreviousButtonNumber);
	_DMESSAGE("Current Back Button number>>>%d", g_TextInputCurrentBackButtonNumber);


	int iMaxRequest = iArrayKey + iArraySize;
	_DMESSAGE("Max array size>>>> %d", iMaxRequest);

	int iRequestFrom = iArrayKey + (g_TextInputOffsetPage * 17);
	int iRequestTo = iRequestFrom + 16;

	if (iRequestTo > iMaxRequest)
	{
		iRequestTo = iRequestTo - (iRequestTo - iMaxRequest);
		_DMESSAGE("RequestTo is exceeding iMaxRequest - new value is %d", iRequestTo);
	}
	_DMESSAGE("iRequestFROM is %d, RequestTO is %d", iRequestFrom, iRequestTo);

	for (auto it2 = WordArray->vWords.begin(); it2 != WordArray->vWords.end(); ++it2) {

		if (Iter2->iKeySorted >= iRequestFrom && Iter2->iKeySorted <= iRequestTo)
		{
			_DMESSAGE("FOUND KEY>>>%d, value is %s", Iter2->iKeySorted, Iter2->vWords.at(0).second.c_str());
			result.push_back(Iter2->vWords.at(0).second);
		}

	}


	s_TextEntryCurrentLetters = f_BB_GetTextEntryCurrentLetters(result);


	return result;
}


void f_BB_AddChosenWordToCurrentMessage(BBWord* ChosenWord, int iRequest)
{
	_DMESSAGE("f_BB_AddChosenWordToCurrentMessage started, iRequest is>>>%d", iRequest);

	int iTemp = iRequest + 1;

	if (ChosenWord->vWords.size() >= iTemp)
	{

	}
	else {
		_DMESSAGE("SUPBB::Add chosen word Request maxed");
		return;
	}


	std::pair<int, string> TempPair = ChosenWord->vWords.at(iRequest);
	_DMESSAGE("TempPair>>%s, key is >>%d ", TempPair.second, TempPair.first);


	if (ChosenWord->iType == t_BB_WordTypePunctuation)
	{
		sCurrentMessage_ToShow = sCurrentMessage_ToShow + TempPair.second; // Adding word to current string
	}
	else {
		sCurrentMessage_ToShow = sCurrentMessage_ToShow + " " + TempPair.second; // Adding word to current string
	}




	v_CurrentMessage.push_back(TempPair.first); // Storing actual key to array


	g_CurrentFilterRequest.clear();
	g_BBForceResetTextEntryMenu = 1;





	//if (iRequest == 0) // Main word
	//{
	//	sCurrentMessage = sCurrentMessage + " " + ChosenWord->Word; // Adding word to current string
	//	v_CurrentMessage.push_back(ChosenWord->iKey); // Storing actual key to array
	//}
	//else if (iRequest == 1)
	//{
	//	sCurrentMessage = sCurrentMessage + " " + ChosenWord->VerbTP; // Adding word to current string
	//	v_CurrentMessage.push_back(ChosenWord->iVerbTPKey); // Storing actual key to array
	//}
	//else if (iRequest == 2)
	//{
	//	sCurrentMessage = sCurrentMessage + " " + ChosenWord->VerbPast; // Adding word to current string
	//	v_CurrentMessage.push_back(ChosenWord->iVerbPastKey); // Storing actual key to array
	//}
	//else if (iRequest == 3)
	//{
	//	sCurrentMessage = sCurrentMessage + " " + ChosenWord->VerbGerund; // Adding word to current string
	//	v_CurrentMessage.push_back(ChosenWord->iVerbGerundKey); // Storing actual key to array
	//}
}



int f_BB_ProcessWordByButtonPress(int iButton)
{
	_DMESSAGE("f_BB_ProcessWordByButtonPress starts with button>>>%d", iButton);

	BBWordType* WordArray = f_BB_GetBBWordClassByKey(g_TextInputCurrentWordTypeKey);
	if (WordArray == NULL)
	{
		_DMESSAGE("Can't find words by request");
		return -1;
	}

	_DMESSAGE("g_TextInputCurrentWordTypeKey>>>%d", g_TextInputCurrentWordTypeKey);
	_DMESSAGE("g_TextInputOffsetPage>>>%d", g_TextInputOffsetPage);

	iButton = g_TextInputCurrentWordTypeKey + g_TextInputOffsetPage * 17  + iButton -3; // -3 because first 3 buttons are for navigating menu
	_DMESSAGE("Final key to look is>>>>%d", iButton);


	BBWord* ChosenWord = NULL;
	for (auto it = WordArray->vWords.begin(); it != WordArray->vWords.end(); ++it) {
		if (Iter->iKeySorted == iButton)
		{
			ChosenWord = Iter;
			break;
		}

	}


	if (ChosenWord == NULL)
	{
		_DMESSAGE("Can't find words by request");
		return -1;
	}

	//if (ChosenWord->iType == t_BB_WordTypeVerb || ChosenWord->iType == t_BB_WordTypeNoun)
	if (ChosenWord->vWords.size() > 1)
	{
		g_CurrentMultiWord = ChosenWord;
		return 2;
	}
	else {
		//Console_Print("Word chosen>>>%s", ChosenWord->vWords.at(0).second.c_str());
		_DMESSAGE("Word chosen>>>%s", ChosenWord->vWords.at(0).second.c_str());
		f_BB_AddChosenWordToCurrentMessage(ChosenWord,0);
		return 1;
	}


}


void f_BB_ClearCurrentMessage()
{
	sCurrentMessage_ToShow.clear();
	v_CurrentMessage.clear();
}

void f_BB_RemoveLastWordFromMessage()
{
	_DMESSAGE("f_BB_RemoveLastWordFromMessage() started");

	_DMESSAGE("1");

	int iCurrentKey = -1;

	if (v_CurrentMessage.size() > 0)
	{
		_DMESSAGE("2");
		iCurrentKey = v_CurrentMessage.back();
		v_CurrentMessage.pop_back();
	}



	if (iCurrentKey != 1)
	{

		int iArrayPos;
		BBWord* TempWord = f_BB_GetWordFromInt(iCurrentKey, iArrayPos);

		if (TempWord)
		{
			string s_Text = TempWord->vWords.at(iArrayPos).second;
			//_DMESSAGE("3");
			const auto pos = sCurrentMessage_ToShow.find_last_of(s_Text);
			_DMESSAGE("TempWord->iType is %d,word is %s", TempWord->iType, s_Text.c_str());
			_DMESSAGE("Current message>>>%s", sCurrentMessage_ToShow.c_str());

			_DMESSAGE("pos is >>%d", pos);
			if (TempWord->iType == t_BB_WordTypePunctuation)
			{
				sCurrentMessage_ToShow = sCurrentMessage_ToShow.substr(0, pos - s_Text.length() + 1);
			}
			else {
				sCurrentMessage_ToShow = sCurrentMessage_ToShow.substr(0, pos - s_Text.length());
			}
			_DMESSAGE("Current message after >>>%s", sCurrentMessage_ToShow.c_str());
			_DMESSAGE("5");

		}
		else {
			_DMESSAGE("Can't find word by key");
		}



	}

	//if (sCurrentMessage.size() > 0)
	//{
	//	_DMESSAGE("5.5");
	//	sCurrentMessage = sCurrentMessage.substr(0, sCurrentMessage.size() - 1); //
	//}

	_DMESSAGE("6");
}


vector<string> f_BB_BuildTextInputMenuMain()
{
	g_TextInputCurrentWordTypeKey = -1;
	g_CurrentFilterRequest.clear();


	vector<string> result;

	result.push_back("[Send Message]");
	g_TextInputCurrentPostMessageButtonNumber = 0;

	int iCount = 0;
	for (auto it = g_BBAllWords.begin(); it != g_BBAllWords.end(); it++)
	{
		if (Iter->iDontInclude == false) 
		{
			iCount += 1;
			result.push_back(Iter->WordTypeName);
			Iter->iMenuButton = iCount; //it - g_BBAllWords.begin()

			_DMESSAGE("Assigning Words>>%s to menu button>>> %d", Iter->WordTypeName.c_str(), Iter->iMenuButton);
		}
	}

	result.push_back("[Remove word]");
	g_TextInputCurrentRemoveWordButtonNumber = iCount + 1; ; // was  g_BBAllWords.size() +1 before Filtered words
	_DMESSAGE("Assigning [Remove word] to menu button>>> %d", g_TextInputCurrentRemoveWordButtonNumber);

	result.push_back("[Clear message]");
	g_TextInputCurrentClearMessageButtonNumber = g_TextInputCurrentRemoveWordButtonNumber + 1;

	_DMESSAGE("Assigning [Clear message] to menu button>>> %d", g_TextInputCurrentClearMessageButtonNumber);

	result.push_back("[Exit]");
	g_TextInputCurrentExitButtonNumber = g_TextInputCurrentClearMessageButtonNumber + 1;

	_DMESSAGE("Assigning [Exit] to menu button>>> %d", g_TextInputCurrentExitButtonNumber);
	return result;
}


int f_BB_RefreshTextInputMessage(UInt32 iButton, UInt32 iCurrentPage)
{
	_DMESSAGE("f_BB_RefreshTextInputMessage started, iButton>>>%d,CurrentPage>>>>%d, Current message>>>%s", iButton, iCurrentPage, sCurrentMessage_ToShow.c_str());
	int iCurrentButtonCount = 0;

	int iMaxWordsExceededMessage = 0;

	vector<string> vStrings{};

	string s_FinalMessage{};



	if (iCurrentPage == -1)
	{
		g_BBForceResetTextEntryMenu = 0;
		g_TextInputOffsetPage = 0;
		
		_DMESSAGE("BUTTON IS %d, g_TextInputCurrentRemoveWordButtonNumber is %d", iButton, g_TextInputCurrentRemoveWordButtonNumber);
		_DMESSAGE("BUTTON IS %d, g_TextInputCurrentExitButtonNumberis %d", iButton, g_TextInputCurrentExitButtonNumber);

		

		if (iButton == -100) // Main
		{
			f_SetTextInputState(1);
			g_CurrentMultiWord = NULL;
			iCurrentButtonCount = 1;

			vStrings = f_BB_BuildTextInputMenuMain();
		}
		else if (iButton == g_TextInputCurrentRemoveWordButtonNumber) 
		{
			_DMESSAGE("Removing last word.");
			f_BB_RemoveLastWordFromMessage();
			vStrings = f_BB_BuildTextInputMenuMain();
		}
		else if (iButton == g_TextInputCurrentClearMessageButtonNumber)
		{
			_DMESSAGE("Clearing message.");
			vStrings = f_BB_BuildTextInputMenuMain();
			f_BB_ClearCurrentMessage();
		}
		else if (iButton == g_TextInputCurrentExitButtonNumber)
		{
			f_SetTextInputState(0);
			_DMESSAGE("Exiting.");
			return 0;
		}
		else if (iButton == g_TextInputCurrentPostMessageButtonNumber)
		{
			f_SetTextInputState(0);
			_DMESSAGE("Posting message(from plugin(");
			return 0;
		}
		else {

			for (auto it = g_BBAllWords.begin(); it != g_BBAllWords.end(); it++)
			{
				if (Iter->iMenuButton == iButton)
				{
					g_TextInputCurrentWordTypeKey = Iter->StartKey;
					g_TextInputCurrentPage = 100;
					_DMESSAGE("Found Iter menu button for %s", Iter->WordTypeName.c_str());
					if (iButton == -200) // Filling filtered words array
					{

						vStrings = f_BBGetFilteredStringsFromPool(g_TextInputCurrentWordTypeKey);
					}
					else {
						

						vStrings = f_BBGetStringsFromPool(g_TextInputCurrentWordTypeKey);
					}





				}

			}


		}


		f_BB_SetMessageButtons(vStrings);


	}
	else if (iCurrentPage == 100)
	{
		if (iButton == 0 && g_BBCurrentCantFindWordsByRequest)
		{
			g_TextInputCurrentPage = -1;
			_DMESSAGE("iButton == 0 && g_BBCurrentCantFindWordsByRequest");
			vStrings = f_BB_BuildTextInputMenuMain();
			f_BB_SetMessageButtons(vStrings);
		}
		else if (iButton == g_TextInputCurrentNextButtonNumber)
		{
			_DMESSAGE("iButton = g_TextInputCurrentNextButtonNumber");
			if (g_TextInputOffsetPage + 1 >= g_TextInputOffsetPageMax)
			{
				_DMESSAGE("Max page exceded - staying on current page");
			}
			else {
				g_TextInputOffsetPage = g_TextInputOffsetPage + 1;
			}
			vStrings = f_BBGetStringsFromPool(g_TextInputCurrentWordTypeKey);
			f_BB_SetMessageButtons(vStrings);
		}
		else if (iButton == g_TextInputCurrentPreviousButtonNumber)
		{ 
			_DMESSAGE("iButton = g_TextInputCurrentPreviousButtonNumber");
			if (g_TextInputOffsetPage -1 < 0)
			{
				_DMESSAGE("Min page exceeded - staying on current page");
			}
			else {
				g_TextInputOffsetPage = g_TextInputOffsetPage - 1;
			}
			vStrings = f_BBGetStringsFromPool(g_TextInputCurrentWordTypeKey);
			f_BB_SetMessageButtons(vStrings);
		}
		else if (iButton == g_TextInputCurrentBackButtonNumber)
		{
			g_TextInputCurrentPage = -1;
			_DMESSAGE("iButton = g_TextInputCurrentBackButtonNumber");
			vStrings = f_BB_BuildTextInputMenuMain();
			f_BB_SetMessageButtons(vStrings);
		}
		else {

			if (v_CurrentMessage.size() >= g_BBPlayerMaxWords)
			{

				iMaxWordsExceededMessage = 1;
				vStrings = f_BBGetStringsFromPool(g_TextInputCurrentWordTypeKey);
				f_BB_SetMessageButtons(vStrings);
			}
			else {
				int iResult = f_BB_ProcessWordByButtonPress(iButton);

				if (iResult == 2) // Chosen word is found and multiple
				{
					g_TextInputCurrentPage = 200;
					vStrings = f_BBGetCurrentVerbForms();
				}
				else {
					vStrings = f_BBGetStringsFromPool(g_TextInputCurrentWordTypeKey);
				}
				f_BB_SetMessageButtons(vStrings);
			}



		}

	}
	else if (iCurrentPage == 200)
	{
		f_BB_AddChosenWordToCurrentMessage(g_CurrentMultiWord, iButton);
		g_TextInputCurrentPage = 100;
		vStrings = f_BBGetStringsFromPool(g_TextInputCurrentWordTypeKey);
		f_BB_SetMessageButtons(vStrings);
	}


	if (g_BBForceResetTextEntryMenu)
	{
		g_BBForceResetTextEntryMenu = 0;
		_DMESSAGE("g_BBForceResetTextEntryMenu == 1");
		g_BBForceResetTextEntryMenu = 0;
		g_TextInputCurrentPage = -1;
		vStrings = f_BB_BuildTextInputMenuMain();
		f_BB_SetMessageButtons(vStrings);
	}



	//if (iMaxWordsExceededMessage)
	//{
	//	s_FinalMessage += "MAXIMUM WORDS COUNT EXCEEDED." + '\n' + '\n';

	//}


	if (g_TextInputCurrentWordTypeKey == t_BB_WordTypeStartKey_FilteredWords)
	{
		s_FinalMessage += "Searching for >>> " + g_CurrentFilterRequest + '\n' + '\n';
	}



	if (g_TextInputCurrentPage == 100)
	{
		if (g_TextInputCurrentWordTypeKey == t_BB_WordTypeStartKeyMisc)
		{
			s_FinalMessage += "Page " + to_string(g_TextInputOffsetPage + 1) + " out of " + to_string(g_TextInputOffsetPageMax) + '\n' + '\n';
		}
		else {
			//s_FinalMessage += "Page " + to_string(g_TextInputOffsetPage + 1) + " out of " + to_string(g_TextInputOffsetPageMax) + "(" + s_TextEntryCurrentPageLetterStart + "-" + s_TextEntryCurrentPageLetterFinish + ")" + '\n' + '\n';
			s_FinalMessage += "Page " + to_string(g_TextInputOffsetPage + 1) + " out of " + to_string(g_TextInputOffsetPageMax) + s_TextEntryCurrentLetters + '\n' + '\n';
		}

	}

	s_FinalMessage += "Your message:" + sCurrentMessage_ToShow + '\n' + '\n';

	s_FinalMessage += "Words left:" + to_string(g_BBPlayerMaxWords - v_CurrentMessage.size()) + '\n';


	if (g_TextInputCurrentPage == 100)
	{
		s_FinalMessage += "\nTIP:You can search for words by typing on your keyboard.\n";
		s_FinalMessage += "Please choose words to add from below:";
	}
	else if (g_TextInputCurrentPage == -1)
	{
		s_FinalMessage += "\nTIP:You can search for words by typing on your keyboard.\n";
		s_FinalMessage += "Please choose what type of word you want to add: ";


	}else if (g_TextInputCurrentPage == 200)
	{

		if (g_CurrentMultiWord)
		{
			s_FinalMessage = "Choose between Word forms:";
		}

	}




	f_BB_SetMessageBodyText(s_FinalMessage,1);
	iCurrentButtonCount = vStrings.size() - 1;

	return iCurrentButtonCount;
}


UInt32 cmd_BBGetTextInputOffsetPage(StaticFunctionTag* base)
{
	return g_TextInputOffsetPage;
}

UInt32 cmd_BBGetTextInputCurrentPage(StaticFunctionTag* base)
{
	return g_TextInputCurrentPage;
}

UInt32 cmd_BBSetTextInputCurrentPage(StaticFunctionTag* base, UInt32 iValue)
{
	g_TextInputCurrentPage = iValue;
	_DMESSAGE("Setting TextInputCurrentPage to >>>%d", iValue);
	return 1;
}


UInt32 cmd_BBGetCurrentNextButtonNumber(StaticFunctionTag* base)
{
	return g_TextInputCurrentNextButtonNumber;
}



UInt32 cmd_BBTextInputUpdate(StaticFunctionTag* base, UInt32 iButton, UInt32 iCurrentPage)
{
	_DMESSAGE("BBTextInputUpdate started");
	int iCurrentButtonCount = f_BB_RefreshTextInputMessage(iButton,iCurrentPage);
	_DMESSAGE("Button count>>>%d", iCurrentButtonCount);
	return iCurrentButtonCount;
}

UInt32 cmd_BBAreMessagesFetched(StaticFunctionTag* base)
{
	return iMessagesFetched;
}

UInt32 cmd_BBAPIKeyExists(StaticFunctionTag* base)
{
	return iBBAPIKeyLoaded;
}

UInt32 cmd_BBNexusAPIKeyExists(StaticFunctionTag* base)
{
	_DMESSAGE("BBNexusAPIKeyExists started>>>%d", iBBNexusAPIKeyLoaded);
	return iBBNexusAPIKeyLoaded;
}



UInt32 cmd_BBHasCellChanged(StaticFunctionTag* base)
{
	_DMESSAGE("BBHasCellChanged started");
	TESObjectCELL* CurrentCell = (*g_player)->parentCell;

	if (CurrentCell == NULL)
	{
		_DMESSAGE("Cell is not valid");
		return 0;
	}

	if (g_BBPreviousCell != CurrentCell)
	{
		g_BBPreviousCell = CurrentCell;
		_DMESSAGE("Changing cells");
		//Console_Print("Changing cells, new cell>>%x", CurrentCell->formID);
		f_BB_BBOnCellChange();
		return 1;
	}


	return 0;
}


UInt32 cmd_BBGetCurrentMenuButtonFor(StaticFunctionTag* base, UInt32 iRequest)
{
	if (iRequest == 0)
	{
		return g_TextInputCurrentPostMessageButtonNumber;
	}
	else if (iRequest == 1)
	{
		return g_TextInputCurrentExitButtonNumber;
	}

}




bool f_BB_AddLetterToFilterRequest(UInt32 iKey, string* s_ToAdd)
{



	if (s_ToAdd == NULL)
	{
		_DMESSAGE("string to add is NULL - should not happen");
		return false;
	}

	_DMESSAGE("BACKSPACE - InputString is %s", (*s_ToAdd).c_str());
	
	if (iKey == 8) //Backspace
	{


		if (s_ToAdd == &sCurrentMessage_ToSend)
		{
			if (g_BBLeftShiftPressed)
			{
				sCurrentMessage_ToSend.clear();
				return true;
			}
		}


		if (s_ToAdd->length() != 0)
		{
			s_ToAdd->pop_back();
			_DMESSAGE("BACKSPACE - InputString is %s", (*s_ToAdd).c_str());
			return true;
		}
		else {

			if (s_ToAdd == &g_CurrentFilterRequest)
			{
				g_TextInputCurrentPage = -1;
			}
			_DMESSAGE("Current InputString length is 0>>>%s", (*s_ToAdd).c_str());
			return false;
		}
	}


	if (s_ToAdd == &sCurrentMessage_ToSend)
	{
		if (sCurrentMessage_ToSend.length() >= g_BBPlayerMaxSymbols)
		{
			_DMESSAGE("SUP_BB::More symbols than allowed - skipping input.");
			return false;
		}

	}



	if (iKey == 32)//Space
	{

		if (g_BBLeftShiftPressed && (s_ToAdd == &sCurrentMessage_ToSend))
		{
			(*s_ToAdd) = (*s_ToAdd) + '\n';
		}
		else {
			(*s_ToAdd) = (*s_ToAdd) + ' ';
		}
		return true;
	}

	if (g_BBLeftShiftPressed)
	{

		if (iKey == 191)// "/" 
		{
			(*s_ToAdd) = (*s_ToAdd) + "?";
			return true;
		}


		if (iKey == 48)// 0
		{
			(*s_ToAdd) = (*s_ToAdd) + ")";
			return true;
		}

		if (iKey == 49)// 1
		{
			(*s_ToAdd) = (*s_ToAdd) + "!";
			return true;
		}

		if (iKey == 50)// 2
		{
			(*s_ToAdd) = (*s_ToAdd) + "@";
			return true;
		}


		if (iKey == 51)// 3
		{
			(*s_ToAdd) = (*s_ToAdd) + "#";
			return true;
		}

		if (iKey == 52)// 4
		{
			(*s_ToAdd) = (*s_ToAdd) + "$";
			return true;
		}

		if (iKey == 53)// 5
		{
			(*s_ToAdd) = (*s_ToAdd) + "%";
			return true;
		}

		if (iKey == 54)// 6
		{
			(*s_ToAdd) = (*s_ToAdd) + "^";
			return true;
		}

		if (iKey == 55)// 7
		{
			(*s_ToAdd) = (*s_ToAdd) + "&";
			return true;
		}

		if (iKey == 56)// 8
		{
			(*s_ToAdd) = (*s_ToAdd) + "*";
			return true;
		}

		if (iKey == 57)// 9
		{
			(*s_ToAdd) = (*s_ToAdd) + "(";
			return true;
		}



		if (iKey >= 65 && iKey <= 90 && (s_ToAdd == &sCurrentMessage_ToSend))
		{
			iKey = iKey + 500;
			_DMESSAGE("Changing key to UPPER");
		}

	}




	_DMESSAGE("f_BB_AddLetterToFilterRequest started with key>>%d", iKey);
	auto it = g_BB_MapKeyArray.find(iKey);
	if (it == g_BB_MapKeyArray.end())
	{
		return false;
	}

	(*s_ToAdd) = (*s_ToAdd) + Iter.second;
	_DMESSAGE("g_CurrentFilterRequest is %s", (*s_ToAdd).c_str());
	return true;
}




void f_BB_CallReshowMessage()
{
	_DMESSAGE("f_BB_CallReshowMessage() started");
	VMArray<VMVariable> arguments;
	VMValue args;

	BSFixedString g_ScriptName;
	BSFixedString g_FuncName;

	const char* ptr = "TommBuildingBridges:questScript";
	CALL_MEMBER_FN(&g_ScriptName, Set)(ptr);
	const char* ptr2 = "ReshowTextInputMessage";
	CALL_MEMBER_FN(&g_FuncName, Set)(ptr2);
	CallGlobalFunctionNoWait_Internal(g_MyVirtMachine, 0, 0, &g_ScriptName, &g_FuncName, &args);
	_DMESSAGE("f_BB_CallReshowMessage() ended");
}

//void f_BB_CallReshowMessage_FreeText()
//{
//	_DMESSAGE("f_BB_CallReshowMessage_FreeText() started");
//	VMArray<VMVariable> arguments;
//	VMValue args;
//
//	BSFixedString g_ScriptName;
//	BSFixedString g_FuncName;
//
//	const char* ptr = "TommBuildingBridges:questScript";
//	CALL_MEMBER_FN(&g_ScriptName, Set)(ptr);
//	const char* ptr2 = "ReshowTextInputMessageFreeText";
//	CALL_MEMBER_FN(&g_FuncName, Set)(ptr2);
//
//	CallGlobalFunctionNoWait_Internal(g_MyVirtMachine, 0, 0, &g_ScriptName, &g_FuncName, &args);
//	_DMESSAGE("f_BB_CallReshowMessage_FreeText() ended");
//}




int f_BB_RefreshTextInputFreeTextMessage(int iNew)
{
	_DMESSAGE("f_BB_RefreshTextInputFreeTextMessage() started");

	string s_FinalMessage{};
	s_FinalMessage += "Please type in your message below(" + to_string(sCurrentMessage_ToSend.length()) + "/" + to_string(g_BBPlayerMaxSymbols) + ")\n" + sCurrentMessage_ToSend + '\n' + '\n';

	//s_FinalMessage += "Symbols left:" + to_string(g_BBPlayerMaxSymbols - sCurrentMessage_ToSend.length()) + '\n';
	s_FinalMessage += "\n\n Press LShift + SPACE for a new line.\n Press LShift + Backspace to clear the current message.";
	if (iNew)
		f_BB_SetMessageBodyText(s_FinalMessage, 1);
	else {
		f_BB_Refresh_MessageMenu(s_FinalMessage);
	}

	_DMESSAGE("f_BB_RefreshTextInputFreeTextMessage() ended");
	return 1;
}





UInt32 cmd_BBGetPressedKeyResult(StaticFunctionTag* base, UInt32 iKey, UInt32 iKeyUP, UInt32 iLaunchValue) //1 bring up menu
{
	_DMESSAGE("BBGetPressedKeyResult started with key>>>%d", iKey);



	if (iLaunchValue != g_S_Value)
	{
		_DMESSAGE("SUP_BB::Key result - Launch value wrong.");
		return 0;
	}



	if (iKey == 160)
	{
		if (iKeyUP)
		{
			g_BBLeftShiftPressed = 0;
		}
		else {
			g_BBLeftShiftPressed = 1;
		}

		return 0;
	}


	if (iKeyUP == 0)
	{

		if (f_IsMenuModeActive())
		{
			_DMESSAGE("Menu mode is active");
			return 0;
		}

		if (g_BBMenuStage == 0 && iKey == g_BB_Hotkey)
		{
			return 1;
		}
	}
	else {

		if ((*g_ui)->IsMenuOpen("Console"))
		{
			_DMESSAGE("Console is open");
			return 0;
		}

		if ((*g_ui)->numPauseGame == 0)
		{
			//Console_Print("Game is not paused");
			_DMESSAGE("Game is not paused");
			return 0;

		}



		//
		_DMESSAGE("g_BBMenuStage is >>>%d", g_BBMenuStage);
		if (g_BBMenuStage == 100)
		{
			f_BB_AddLetterToFilterRequest(iKey, &g_CurrentFilterRequest);
			
			_DMESSAGE("Key pressed for search>>>>>%d", iKey);

			if (g_CurrentFilterRequest.length() == 0)
			{
				_DMESSAGE("Current request is empty.");
				g_TextInputCurrentPage = -250; // for papyrus processing which will set this value to -1 and button to -200
				g_CurrentFilterRequest.clear();
			}
			else {
				g_TextInputCurrentPage = -200; // for papyrus processing which will set this value to -1 and button to -200
			}



			string sTemp = "MessageBoxMenu";
			BSFixedString bs_MenuName;
			CALL_MEMBER_FN(&bs_MenuName, Set)(sTemp.c_str());
			CALL_MEMBER_FN(*g_uiMessageManager, SendUIMessage)(bs_MenuName, kMessage_Refresh);
			f_BB_CallReshowMessage();

			return 2;
			
		}
		else if (g_BBMenuStage == 150)
		{

			f_BB_AddLetterToFilterRequest(iKey,&sCurrentMessage_ToSend);
			f_BB_RefreshTextInputFreeTextMessage(0);
			//string sTemp = "MessageBoxMenu";
			//BSFixedString bs_MenuName;
			//CALL_MEMBER_FN(&bs_MenuName, Set)(sTemp.c_str());
			//CALL_MEMBER_FN(*g_uiMessageManager, SendUIMessage)(bs_MenuName, kMessage_Refresh);


			//f_BB_CallReshowMessage_FreeText();
		}
	}




	return 0;

}

void cmd_BBSetMenuStage(StaticFunctionTag* base, UInt32 iValue) //1 bring up menu
{
	g_BBMenuStage = iValue;


	if (g_BBMenuStage == 150)
	{
		_DMESSAGE("Clearing message to send.");
		sCurrentMessage_ToSend.clear();
	}
	else if (g_BBMenuStage == 100)
	{
		_DMESSAGE("Clearing message for simple text input.");
		f_BB_ClearCurrentMessage();
	}
}


UInt32 cmd_BBGetMenuStage(StaticFunctionTag* base) //1 bring up menu
{
	return g_BBMenuStage;
}


void cmd_BBClearTextInputState(StaticFunctionTag* base)
{
	f_SetTextInputState(0);
}








void cmd_BBTextInputFreeTextUpdate(StaticFunctionTag* base)
{
	_DMESSAGE("BBTextInputFreeTextUpdate started");
	f_SetTextInputState(1);
	f_BB_RefreshTextInputFreeTextMessage(1);
	_DMESSAGE("BBTextInputFreeTextUpdate ended");
}




UInt32 cmd_BBCheckLaunchValue(StaticFunctionTag* base, UInt32 iLaunchValue)
{
	if (g_LaunchValueChecked)
	{
		_DMESSAGE("Unendetified error reading S Value");
		return 0;
	}


	g_LaunchValueChecked = true;

	if (iLaunchValue == g_S_Value)
	{
		return 1;
	}
	else {
		return 0;
	}

}










UInt32 cmd_BBReportMessage(StaticFunctionTag* base)
{
	_DMESSAGE("SUP_BB::BReportMessage started");


	UInt32 iMessageID = g_CurrentMessageID;

	OnlineMessage* TempMessage = f_BB_GetMessageFromFormID(iMessageID);

	if (TempMessage)
	{
		if (TempMessage->Type == t_BB_MessageTypeSimple)
		{
			_DMESSAGE("SUP_BB::Can't report simple messages");
			g_BBLastServerMessage = "You can't report messages made by preplaced words.";
			return 0;
		}


	}
	else {
		_DMESSAGE("SUP_BB::Can't find message by ID.");
		g_BBLastServerMessage = "Can't find local message by ID.";
		return 0;
	}


	if (iBBAPIKeyLoaded == 0)
	{
		_DMESSAGE("SUP_BB::iBBAPIKeyLoaded not loaded>>");
		g_BBLastServerMessage = "BB key is not loaded.";
		return 0;
	}

	nlohmann::json jIN;
	jIN["apikey"] = s_BBApiKey;
	jIN["message_id"] = iMessageID;
	jIN["APIVersion"] = t_BB_API_VER;
	string s_RequestFINAL = jIN.dump();

	HINTERNET hInternet = InternetOpenA("InetURL/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	HINTERNET hConnection = InternetConnectA(hInternet, "Tomminfinite.pythonanywhere.com", 80, " ", " ", INTERNET_SERVICE_HTTP, 0, 0);
	HINTERNET hData = HttpOpenRequestA(hConnection, "POST", "/FO4MessagesReportMessage", NULL, NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION, 0);
	HttpAddRequestHeadersA(hData, "content-type: application/json", -1, HTTP_ADDREQ_FLAG_ADD);


	char buf[2048 + 1];

	MyThreadBB MyNewThread;
	MyNewThread.CheckType = t_CheckType_Nexus;

	thread NewThread(&MyThreadBB::Execute, std::ref(MyNewThread));
	NewThread.detach();


	MyNewThread.stage = 5;
	MyNewThread.hData = hData;

	_DMESSAGE("SUP_BB::SENDING>>>%s", s_RequestFINAL.c_str());
	HttpSendRequestA(hData, NULL, 0, (LPVOID)s_RequestFINAL.c_str(), s_RequestFINAL.length());
	MyNewThread.stage = 110;


	int iStatCode = f_GetStatCodeResponse(&hData);
	_DMESSAGE("SUP_BB::iStatCode is %d", iStatCode);
	if (iStatCode == 0)
	{
		_DMESSAGE("SUP_BB::Server not responding");
		g_BBLastServerMessage = "Server is not responding or internet connection is absent";
		return 0;
	}
	else if (iStatCode == 404)
	{
		_DMESSAGE("SUP_BB::Page does not exist");
		g_BBLastServerMessage = "Page on server doesn't exist.";
		return 0;
	}
	else if (iStatCode == 500)
	{
		_DMESSAGE("SUP_BB::stat code 500");
		g_BBLastServerMessage = "Internal server error. Please take screenshot of this message and send to TommInfinite in Direct Messages as it contains your API key>>" + s_RequestFINAL;
		return 0;
	}
	else if (iStatCode != 200)
	{
		_DMESSAGE("SUP_BB::statcode unedentified>>>%d", iStatCode);
		g_BBLastServerMessage = "Problem connecting to server. Please try again later.>>";
		return 0;
	}




	DWORD bytesRead = 0;
	DWORD totalBytesRead = 0;

	string s_resultRead{};

	while (InternetReadFile(hData, buf, 2048, &bytesRead) && bytesRead != 0)
	{
		_DMESSAGE("SUP_BB::buf is %s", buf);
		s_resultRead.append(buf, bytesRead);
		totalBytesRead += bytesRead;
	}
	InternetCloseHandle(hData);
	InternetCloseHandle(hConnection);
	InternetCloseHandle(hInternet);



	auto j = nlohmann::json::parse(s_resultRead, 0, 0);

	if (!j.is_structured())
	{
		_DMESSAGE("SUP_BB::JSON is not structured");
		g_BBLastServerMessage = "Internal server error - JSON not structured. Please take screenshot of this message and send to TommInfinite in Direct Messages as it contains your API key>>" + s_resultRead;
		return 0;
	}

	string s_Message_result;
	int iReportSuccess = 0;


	if (j.find("success") != j.end())
	{
		iReportSuccess = j["success"].get<int>();
		_DMESSAGE("SUP_BB::iReportSuccess is %d", iReportSuccess);

	}
	else {
		_DMESSAGE("SUP_BB::JSON does not have success key");
		g_BBLastServerMessage = "Internal server error - JSON does not have success key. Please take screenshot of this message and send to TommInfinite in Direct Messages as it contains your API key>>" + s_resultRead;
		return 0;
	}

	if (j.find("message") != j.end())
	{
		s_Message_result = j["message"].get<string>();
	}
	else {
		_DMESSAGE("SUP_BB::JSON does not have message key");
		g_BBLastServerMessage = "Internal server error - JSON does not have message key. Please take screenshot of this message and send to TommInfinite in Direct Messages as it contains your API key>>" + s_resultRead;
		return 0;
	}


	if (iReportSuccess == 0)
	{
		_DMESSAGE("SUP_BB::report success key is 0");
		g_BBLastServerMessage = s_Message_result;
		return 0;
	}

	if (iReportSuccess == 1)
	{
		g_BBLastServerMessage = s_Message_result;
		return 1;
	}

	_DMESSAGE("SUP_BB::Result is>>>> %s", s_resultRead.c_str());
	return true;
}



void cmd_BBOnNewGameLoad(StaticFunctionTag* base)
{
	_DMESSAGE("BBOnNewGameLoad started,g_BBLastNewGame>>%d,g_BBLastNewGameBBExecuted>>%d ", g_BBLastNewGame, g_BBLastNewGameBBExecuted);
	if (g_BBLastNewGame)
	{
		if (!g_BBLastNewGameBBExecuted)
		{
			g_BBLastNewGameBBExecuted = true;
			f_BB_LoadFunctionFromMessageHandler(1);
		}

	}
}