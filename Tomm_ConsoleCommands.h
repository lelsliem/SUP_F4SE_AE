

namespace SUPConsoleCommands {



	struct ConsoleCommand {

		enum {
			kTypeInvalid = 0,
			kTypeSUP,
			kTypePapyrus
		};



		string name{};
		int Type = kTypeInvalid;
		bool bAcceptsCallingRefs = false;
		bool bRequiresCallingRef = false;


		string FunctionName{};
		string ScriptName{};
		UInt64 handle = 0;

	};




	struct DecodedConsoleCommand {
		bool bSuccess = false;
		string sCallingRef{};
		TESObjectREFR* CallingRef = NULL;
		string sFunction{};
		vector<string> sParams{};
	};

	bool bProcessConsoleCommand = false;
	TESObjectREFR* rCurrentConsoleReference = nullptr;
	string sCurrentConsoleText{};

	int iCurrentSUPDEbug = 0;

	
	//for (ObScriptCommand* iter = g_firstConsoleCommand; iter->opcode < (kObScript_NumConsoleCommands + kObScript_ConsoleOpBase); ++iter)
	//{
	//	if (!strcmp(iter->longName, "ForceRSXCrash"))
	//	{
	//		s_hijackedCommand = iter;
	//		break;
	//	}
	//}


	bool ProcessCommandAndTellIfContinue(char* Text)
	{


		if (Text)
		{




				//if (boost::iequals(Text, "checkinv"))
				//{

				//	auto MyKeyword = LookupFormByID(1002218);

				//	auto result = cmd_GetInventoryItemsByKeyword(NULL, (*g_player), (BGSKeyword*)MyKeyword, NULL, false, false, false);
				//	Console_Print("inv items result>>%d", result.Length());


				//	return;
				//}




				//if (boost::iequals(Text, "getstateopen"))
				//{
				//	rCurrentConsoleReference = f_GetCurrentConsoleReference();

				//	if (!rCurrentConsoleReference)
				//	{
				//		Console_Print("NO REF");
				//		return;
				//	}
				//	Console_Print("GetOpenState(SourceRef)>>%d", GetOpenState(rCurrentConsoleReference));

				//}


			//if (boost::iequals(Text, "outputgrid"))
			//{
			//	SUPConsoleCommands::rCurrentConsoleReference = f_GetCurrentConsoleReference();

			//	if (!SUPConsoleCommands::rCurrentConsoleReference)
			//	{
			//		Console_Print("NO REF");
			//		return false;
			//	}

			//	TESObjectREFR* workshopRef = GetLinkedRef_Native(SUPConsoleCommands::rCurrentConsoleReference, g_SUPWorkshopKeyword);

			//	if (!workshopRef)
			//	{
			//		Console_Print("NO workshop");
			//		return false;
			//	}

			//	Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(workshopRef);

			//	if (!WorkshopData)
			//	{
			//		_DMESSAGE("No workshop extra data");
			//		return false;
			//	}

			//	OutputPowerGridHelper(WorkshopData);
			//	Console_Print("gridoutputted");
			//	return false;
			//}

			//if (boost::iequals(Text, "supdebug"))
			//{
			//	_DMESSAGE("DEBUG COMMAND>>%d", iCurrentSUPDEbug);
			//	_DMESSAGE("DEBUG COMMAND>>%d", iCurrentSUPDEbug);
			//	_DMESSAGE("DEBUG COMMAND>>%d", iCurrentSUPDEbug);

			//	Console_Print("DEBUG COMMAND>>%d", iCurrentSUPDEbug);
			//	iCurrentSUPDEbug++;
			//	return false;
			//}






			//if (boost::iequals(Text, "supopenlog"))
			//{
			//	Console_Print("OPENING LOG");
			//	CHAR my_documents[MAX_PATH];
			//	HRESULT result = SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, my_documents);
			//	string sTemp = my_documents;
			//	sTemp += "\\My Games\\Fallout4\\F4SE\\SUP_F4SE.log";


			//	ShellExecute(NULL, _T("explore"), sTemp.c_str(), NULL, NULL, SW_SHOW);
			//}


			//if (boost::iequals(Text, "getsupversion"))
			//{
			//	float fVersion = iVersion * 0.01;
			//	Console_Print("SUP F4SE V.%0.2lf", fVersion);
			//	return;
			//}

			//string sTemp = Text;
			//if (sTemp == "setpers")
			//{
			//	TESForm* MyForm = LookupFormByID(1583606);

			//	if (MyForm)
			//	{
			//		MyForm->flags |= TESForm::kFlag_Persistent;
			//		Console_Print("set persistent");
			//	}
			//	else {
			//		Console_Print("object not exists");
			//	}


			//	//rCurrentConsoleReference = f_GetCurrentConsoleReference();

			//	//if (rCurrentConsoleReference)
			//	//{
			//	//	rCurrentConsoleReference->flags |= TESForm::kFlag_Persistent;
			//	//	Console_Print("set persistent");
			//	//}

			//	//Console_Print("Script dumped!");
			//	//AttachedScriptReader::DumpALLScripts();
			//}

			if (OnConsoleCommandEventHandler.HasEvents())
			{
				SUPConsoleCommands::bProcessConsoleCommand = true;
				SUPConsoleCommands::rCurrentConsoleReference = f_GetCurrentConsoleReference();
				SUPConsoleCommands::sCurrentConsoleText = Text;
			}

		}



		return true;
	}












	DecodedConsoleCommand GetConsoleCommandByText(string& sText){
	
		DecodedConsoleCommand result;


		if (sText.length() == 0)
		{
			_DMESSAGE("Console command text is empty");
			return result;
		}

		_DMESSAGE("Console command text>>>%s", sText.c_str());

		//Console_Print("OnConsoleExecuteCommand>>%s", sText.c_str());

		auto Splitted = f_Split_String(sText, " ");

		if (Splitted.size() == 0) {
			_DMESSAGE("Text is not splitted - strange.");
			return result;
		}







		auto SplittedFirstWord = f_Split_String(Splitted.at(0), ".");

		if (SplittedFirstWord.size() == 0) {
			_DMESSAGE("SplittedFirstWord is not splitted - strange.");
			return result;
		}


		if (SplittedFirstWord.size() == 1)
		{
			result.sFunction = SplittedFirstWord.at(0);
		}
		else {
			result.sCallingRef = SplittedFirstWord.at(0);
			result.sFunction = SplittedFirstWord.at(1);
		}


		if (Splitted.size() > 1)
		{
			for (auto it = std::next(Splitted.begin()); it != Splitted.end(); it++)
			{
				result.sParams.push_back(Iter);
			}
		}





		if (result.sCallingRef.length() == 0)
		{
			result.CallingRef = rCurrentConsoleReference;
		}
		else if (boost::iequals(result.sCallingRef.c_str(), "player"))
		{
			_DMESSAGE("Ref is player");
			result.CallingRef = (TESObjectREFR*)LookupFormByID(20);
		}
		else {
			_DMESSAGE("Building custom Ref");
			UInt32 form_Dec;

			try
			{
				form_Dec = std::stoul(result.sCallingRef, nullptr, 16);
			}
			catch (std::invalid_argument e)
			{
				_DMESSAGE("HeX threw exception inv agr - skipping");
				return result;
			}
			catch (std::out_of_range  e)
			{
				_DMESSAGE("HeX threw exception out of range - skipping");
				return result;
			}

			result.CallingRef = (TESObjectREFR*)LookupFormByID(form_Dec);


		}

		result.bSuccess = true;
		return result;
	}










}