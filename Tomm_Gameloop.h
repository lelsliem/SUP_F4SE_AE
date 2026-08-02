


plf::nanotimer g_timer;
float g_TimeElapsed = 0;

TESObjectCELL* g_MainLoopPreviousCell = NULL;



void __forceinline GameLoopTaskFunc()
{

	if (g_MainLoopPreviousCell != (*g_player)->parentCell && (*g_player)->parentCell)
	{
		g_MainLoopPreviousCell = (*g_player)->parentCell;
		if (OnCellChangeEventHandler.HasEvents())
		{
			OnCellChangeEventHandler.EnterLock();
			VMArray<VMVariable> arguments;
			VMValue args;
			VMVariable var1;
			var1.Set<TESObjectCELL*>(&(*g_player)->parentCell);
			arguments.Push(&var1);
			arguments.PackArray(&args, g_MyVirtMachine);

			vector<SUPEvent::SUPEventFilters::SUPSmartFilter> v_PassedFilters;

			SUPEvent::SUPEventFilters::SUPSmartFilter NewFilter(SUPEvent::SUPEventFilters::k_FilterFirst, SUPEvent::SUPEventFilters::k_FilterFormID);
			std::get<vector<UInt32>>(NewFilter.List).push_back((*g_player)->parentCell->formID);
			v_PassedFilters.push_back(NewFilter);


			OnCellChangeEventHandler.RunEventsAlt(&args, v_PassedFilters);
			OnCellChangeEventHandler.LeaveLock();
		}
		
	}

	if (!g_EventScriptArrayOnGameLoop.size())
	{
		return;
	}


	s_GameLoopLock.Enter();

	float elapsedTime = g_timer.get_elapsed_ms() - g_TimeElapsed;
	g_TimeElapsed = g_timer.get_elapsed_ms();
	//_DMESSAGE("ELAPSED TIME>>%f", elapsedTime);

	for (auto it = g_EventScriptArrayOnGameLoop.begin(); it != g_EventScriptArrayOnGameLoop.end();)
	{
		if (Iter->fCurTimer < 0)
		{
			Iter->fCurTimer = Iter->fCallInterval;

			Iter->RunEvent();

			if (Iter->iRunTimesCount > 0)
			{
				if ((Iter->iRunTimesCount - 1) == 0)
				{
					_DMESSAGE("Erasing gameloop event ID>>%d", Iter->iID);
					delete Iter;
					it = g_EventScriptArrayOnGameLoop.erase(it);
					continue;
				}
				else {
					Iter->iRunTimesCount--;
				}
			}
			it++;
		}
		else {

			if (Iter->iCallMode)
			{
				if (Iter->iCallMode == t_ScriptRunnerCallModeOnlyGamemode)
				{
					if (!(*g_ui)->numPauseGame)
					{
						Iter->fCurTimer -= elapsedTime;
					}
				}
				else {
					if ((*g_ui)->numPauseGame)
					{
						Iter->fCurTimer -= elapsedTime;
					}
				}
			}
			else {
				Iter->fCurTimer -= elapsedTime;
			}

			it++;
		}



	}

	s_GameLoopLock.Leave();
	//_DMESSAGE("10");
}





class GameLoopTask : public ITaskDelegate
{
public:
	virtual void Run() override
	{
		GameLoopTaskFunc();
	}

};









void f_GameLoop_OnDeferredInit()
{
	g_timer.start();
	g_TimeElapsed = g_timer.get_elapsed_ms();

	bGameLoopTimerStarted = true;
}



void f_Gameloop_OnGameLoad()
{
	g_MainLoopPreviousCell = NULL;

	if (bGameLoopTimerStarted == false)
	{

		//g_taskInterface->AddTaskPermanent(new GameLoopTask);
		//_DMESSAGE("Starting and storing timer for gameloop.");
	}

}



//Not used
//typedef bool (*MessageQueueProcessHook)(void*, float, UInt32);
//MessageQueueProcessHook MessageQueueProcessHook_F4SE_Original = NULL;
//bool MessageQueueProcessTask_MyHook(void* messageQueue, float timeout, UInt32 unk1)
//{
//	//f_DebugPring_SUP("My hook works");
//	bool bresult = MessageQueueProcessHook_F4SE_Original(messageQueue, timeout, unk1);
//
//	return bresult;
//}
//RelocAddr <MessageQueueProcessHook> MessageQueueProcessTaskSUP(0x00D57FA0);

//class TestTask : public ITaskDelegate
//{
//public:
//	virtual void Run() override
//	{
//		f_DebugPring_SUP("UI TASK FROM F4SE WORKS!!!!!!");
//	}
//
//};


