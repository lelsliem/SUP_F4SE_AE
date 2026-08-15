#include "sup/util/string.hpp"


#define t_ScriptRunnerCallModeAll 0
#define t_ScriptRunnerCallModeOnlyGamemode 1
#define t_ScriptRunnerCallModeOnlyMenumode 2



#define t_ScriptRunnerEventTypeInvalid 0
#define t_ScriptRunnerEventTypeOnGameOther 1
#define t_ScriptRunnerEventTypeOnMainGameLoop 2




//struct GlobalVarInfo {
//	string sGlobalVarFileName{};
//	string sGlobalVarFormID{};
//	float fGlobalVarExpectedValue = 0;
//};
//GlobalVarInfo* GlobVarInfo = NULL;





class SUPEventGameLoop : public SUPEvent
{
public:
	void RunEvent();


	int iID = 0;
	float fCurTimer = 0;
	float fCallInterval= 0.1;
	int iCallMode = t_ScriptRunnerCallModeAll;
	VMValue args; // Store on stack because each time I have to create new.
	bool bHasArgs = false;
};


bool bGameLoopTimerStarted = false;
int iGameLoopFunctionCount = 0;


vector<SUPEventGameLoop*> g_EventScriptArrayOnGameLoop;


// Phase 3: the F4SE tHashSet<VirtualMachine::IdentifierItem, UInt64>::Dump() debug
// specialization was removed — it referenced F4SE VM internals that CommonLibF4 does not
// model, and nothing in the build calls Dump().


void SUPEventGameLoop::RunEvent()
{
	_DMESSAGE("Running func  from script>>>%s named>>>> %s, ID>>>%d", s_ScriptName.c_str(), s_FunctionName.c_str(), iID);

	//if (bFunctionIsLocal)
	//{
		VMIdentifier* identifier = NULL;
		if (!g_MyVirtMachine->GetObjectIdentifier(ScriptHandle, s_ScriptName.c_str(), 0, &identifier, 0))
		{
			_DMESSAGE("VM identifier NOT found, handler is>>>%d", ScriptHandle);
			return;
		}

		_DMESSAGE("VM identifier found");

		BSFixedString g_FuncName(s_FunctionName.c_str());

		if (bHasArgs)
		{
			VMValue* NewArgs = new VMValue(args);


			CallFunctionNoWait_Internal(g_MyVirtMachine, 0, identifier, &g_FuncName, NewArgs);
		}
		else {
			CallFunctionNoWait_Internal(g_MyVirtMachine, 0, identifier, &g_FuncName, g_packedArgs);
		}


		//_DMESSAGE("Calling local:::ScriptName is %s, FunctionName is %s", s_ScriptName.c_str(), s_FunctionName.c_str());
	//}
	//else {
	//	BSFixedString g_ScriptName(s_ScriptName.c_str());
	//	BSFixedString g_FuncName(s_FunctionName.c_str());
	//	if (bHasArgs)
	//	{
	//		VMValue* NewArgs = new VMValue(args);
	//		CallGlobalFunctionNoWait_Internal(g_MyVirtMachine, 0, 0, &g_ScriptName, &g_FuncName, NewArgs);
	//	}
	//	else{
	//		CallGlobalFunctionNoWait_Internal(g_MyVirtMachine, 0, 0, &g_ScriptName, &g_FuncName, g_packedArgs);
	//	}


	//	_DMESSAGE("Calling global:::ScriptName is %s, FunctionName is %s", s_ScriptName.c_str(), s_FunctionName.c_str());
	//}

}

namespace EventRunner {

	void RunOnSaveLoad()
	{
		OnGameLoadEventHandler.RunEvents(NULL); // calling OnGameLoadEventHandler

		for (auto it = v_EventHandlers.begin(); it != v_EventHandlers.end(); it++) // Removing  Event handlers on game load which are permanent or need to be removed on game load.
		{
			Iter->ProcessOnSaveLoad();
		}


		s_GameLoopLock.Enter();
		for (auto it = g_EventScriptArrayOnGameLoop.begin(); it != g_EventScriptArrayOnGameLoop.end();)
		{

			if (Iter->bRemoveOnSaveLoad)
			{
				_DMESSAGE("Erasing  gameloop function to call from the list>>%s", Iter->s_FunctionName.c_str());

				delete Iter;
				it = g_EventScriptArrayOnGameLoop.erase(it);
			}
			else {
				++it;
			}
		}
		s_GameLoopLock.Leave();

	}




	void ProcessFile(string sFilePath)
	{
		_DMESSAGE("f_EventRunner_ProcessFile started for %s", sFilePath.c_str());
		nlohmann::json j;

		std::ifstream t(sFilePath);

		if (!t.is_open())
		{
			_MESSAGE("Problem reading SUP ScriptRunnerFile>>%s::File can't be opened", sFilePath.c_str());
			return;
		}


		std::stringstream buffer;
		buffer << t.rdbuf();
		t.close();
		j = nlohmann::json::parse(buffer, 0, 0);

		if (!j.is_structured())
		{
			_MESSAGE("Problem reading SUP ScriptRunnerFile>>%s::JSON is not sructured", sFilePath.c_str());
			return;
		}




		for (nlohmann::json::iterator it = j["Scripts"].begin(); it != j["Scripts"].end(); ++it) {

			_DMESSAGE("For each JSON Script array iterator");


			if (!Iter.contains("ScriptName") || !Iter["ScriptName"].is_string())
			{
				_MESSAGE("Problem reading SUP ScriptRunnerFile>>%s::Entry doesn't contain ScriptName or it's not a string.", sFilePath.c_str());
				continue;
			}

			if (!Iter.contains("FunctionName") || !Iter["FunctionName"].is_string())
			{
				_MESSAGE("Problem reading SUP ScriptRunnerFile>>%s::Entry doesn't contain Function name or it's not a string.", sFilePath.c_str());
				continue;
			}

			if (!Iter.contains("Event") || !Iter["Event"].is_string())
			{
				_MESSAGE("Problem reading SUP ScriptRunnerFile>>%s::Entry doesn't contain Event or it's not a string.", sFilePath.c_str());
				continue;
			}

			if (!Iter.contains("Caller") || !Iter["Caller"].is_string())
			{
				_MESSAGE("Problem reading SUP ScriptRunnerFile>>%s::Entry doesn't contain Caller or it's not a string.", sFilePath.c_str());
				continue;
			}


			vector<string> sCallerValue = f_Split_String(Iter["Caller"].get<std::string>(), "|");

			if (sCallerValue.size() != 2)
			{
				_MESSAGE("Problem reading SUP ScriptRunnerFile>>%s::Cannot split caller in two parts>>%s", sFilePath.c_str(), Iter["Caller"].get<std::string>().c_str());
				continue;
			}





			int iType = t_ScriptRunnerEventTypeInvalid;
			SUPEventGameLoop NewEventRunner;
			string sEvent = Iter["Event"].get<string>();

			SUPEventHandler* ChosenEventHandler = NULL;


			if (sup::string::iequals(sEvent, "OnMainGameloop")) {

				if (Iter.contains("CallInterval"))
				{
					if (!Iter["CallInterval"].is_number())
					{
						_MESSAGE("Problem reading SUP ScriptRunnerFile>>%s::Entry contains CallInterval but it's not a number.", sFilePath.c_str());
						continue;
					}


					NewEventRunner.fCallInterval = Iter["CallInterval"].get<float>() * 1000;
					_DMESSAGE("Call interval is>>>%f ms", NewEventRunner.fCallInterval);


					if (Iter.contains("CallMode"))
					{
						if (Iter["CallMode"].is_number())
						{
							int iCallMode = Iter["CallMode"].get<int>();

							if (iCallMode < 0 || iCallMode >t_ScriptRunnerCallModeOnlyMenumode)
							{
								_MESSAGE("Problem reading SUP ScriptRunnerFile>>%s::Wrong CallMode specified>>%d - please read tutorial.", sFilePath.c_str(), iCallMode);
								continue;
							}

							NewEventRunner.iCallMode = iCallMode;

							_DMESSAGE("Call mode is>>%d", iCallMode);

						}
					}

				}

				iType = t_ScriptRunnerEventTypeOnMainGameLoop;


			}
			else {

				sup::string::to_lower(sEvent);
				ChosenEventHandler = SUPEventHandlers::FindSUPEventHandlerByEventName(sEvent);

				if (!ChosenEventHandler)
				{
					_MESSAGE("Problem reading SUP ScriptRunnerFile>>%s::Event %s doesn't correspond with any existing event.", sFilePath.c_str(), sEvent.c_str());
					continue;
				}


				iType = t_ScriptRunnerEventTypeOnGameOther;
			}


			NewEventRunner.s_ScriptName = Iter["ScriptName"].get<std::string>();
			sup::string::to_lower(NewEventRunner.s_ScriptName);
			NewEventRunner.s_FunctionName = Iter["FunctionName"].get<std::string>();
			sup::string::to_lower(NewEventRunner.s_FunctionName);

			NewEventRunner.sFormID = sCallerValue.at(0);
			NewEventRunner.sPluginName = sCallerValue.at(1);




			if (Iter.contains("RemoveOnSaveLoad"))
			{
				if (Iter["RemoveOnSaveLoad"].is_number())
				{
					NewEventRunner.bRemoveOnSaveLoad = (bool)Iter["RemoveOnSaveLoad"].get<int>();
					_DMESSAGE("RemoveOnSaveLoad  is %d", NewEventRunner.bRemoveOnSaveLoad);
				}
			}



			//if (Iter.contains("Local") && Iter["Local"].is_number())
			//{
			//	NewEventRunner.bFunctionIsLocal = (bool)Iter["Local"].get<int>();
			//	_DMESSAGE("Local is %d", NewEventRunner.bFunctionIsLocal);
			//}


			if (Iter.contains("RunTimesCount"))
			{
				if (Iter["RunTimesCount"].is_number())
				{
					NewEventRunner.iRunTimesCount = Iter["RunTimesCount"].get<int>();
					_DMESSAGE("RunTimesCount  is %d", NewEventRunner.iRunTimesCount);
				}
			}


			//if (Iter.contains("GlobalVarFileName") && Iter.contains("GlobalVarFormID") && Iter.contains("GlobalVarExpectedValue"))
			//{
			//	if (Iter["GlobalVarFileName"].is_string() && Iter["GlobalVarFormID"].is_string() && Iter["GlobalVarExpectedValue"].is_number())
			//	{
			//		EventScriptRunner::GlobalVarInfo* NewGloBalVarInfo = new EventScriptRunner::GlobalVarInfo;
			//		NewGloBalVarInfo->sGlobalVarFileName = Iter["GlobalVarFileName"].get<string>();
			//		NewGloBalVarInfo->sGlobalVarFormID = Iter["GlobalVarFormID"].get<string>();
			//		NewGloBalVarInfo->fGlobalVarExpectedValue = Iter["GlobalVarExpectedValue"].get<float>();
			//		_DMESSAGE("Storing new global variable to set");
			//	}
			//}



			if (iType == t_ScriptRunnerEventTypeOnMainGameLoop)
			{
				iGameLoopFunctionCount++;
				NewEventRunner.iID = iGameLoopFunctionCount;
				g_EventScriptArrayOnGameLoop.push_back(new SUPEventGameLoop(NewEventRunner));
				_DMESSAGE("Setting ID>>%d", NewEventRunner.iID);
			}
			else {
				ChosenEventHandler->PushBackNewEvent(new SUPEvent(NewEventRunner));
			}


			_DMESSAGE("Added ScriptName>>>%s, FunctionName>>>%s, type>>%d", NewEventRunner.s_ScriptName.c_str(), NewEventRunner.s_FunctionName.c_str(), iType);


		}
	}







	void PopulateArrayFromFiles()
	{
		if (f_Does_FolderExist(s_ScriptRunnerPath))
		{
			for (auto const& dir_entry : std::filesystem::directory_iterator{ s_ScriptRunnerPath })
			{
				if (!dir_entry.is_directory())
					ProcessFile(dir_entry.path().string());
			}
		}

	}

	void CheckAndGiveHandleToLocalFunctions()
	{

		for (auto it = v_EventHandlers.begin(); it != v_EventHandlers.end(); it++)
		{
			Iter->CheckAndStoreFunctions();
		}

		for (auto it = g_EventScriptArrayOnGameLoop.begin(); it != g_EventScriptArrayOnGameLoop.end();)
		{
			auto result = GetObjectHandleForEvent(Iter);

			if (!result.bSuccess)
			{
				_MESSAGE("Cannot find Script>>> %s in the VM list - removing from SUP Script runner.", Iter->s_ScriptName.c_str());
				delete Iter;
				it = g_EventScriptArrayOnGameLoop.erase(it);
				continue;
			}


			Iter->sPluginName.clear();
			Iter->sFormID.clear();
			Iter->ScriptHandle = result.handle;
			_DMESSAGE("Found %s in the stored VMScripts array", Iter->s_ScriptName.c_str());
			it++;
		}
	}
};

















