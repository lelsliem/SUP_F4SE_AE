#include "sup/util/string.hpp"
// Phase 3: common/ICriticalSection.h replaced by the compat ICriticalSection in F4SECompat.h
#include "sup/compat/F4SECompat.h"
#include <variant>

VMValue* g_packedArgs = NULL;

ICriticalSection	s_GameLoopLock;


struct PowerGridChanges {
	bool bChanged = false;
	vector<UInt32> AffectedPowerGrids{};
	vector<UInt32> DeletedPowerGrids{};
};







class SUPEvent
{
public:

	struct SUPEventFilters {

		enum FilterTypes {
			k_FilterFormID = 1,
			k_FilterFloat,
			k_FilterString,
		};

		enum FilterNumbers {
			k_FilterFirst = 0,
			k_FilterSecond,
			k_FilterThird,
			k_FilterFourth,
		};


		struct SUPSmartFilter{

			bool operator==(const SUPSmartFilter& rhs)
			{
				return Number == rhs.Number && type == rhs.type;
			}


			SUPSmartFilter(int NumberIN, int typeIN)
			{
				Number = NumberIN;
				type = typeIN;

				if (typeIN == k_FilterFormID)
				{
					List.emplace<vector<UInt32>>();
				}
				else if (typeIN == k_FilterFloat)
				{
					List.emplace<vector<float>>();
				}
				else if (typeIN == k_FilterString)
				{
					List.emplace<vector<string>>();
				}

			}

			int Number = 0;
			int type = 0;
			std::variant<vector<UInt32>,vector<string>, vector<float>> List;
		};

		vector<SUPSmartFilter> v_Filters;
	};

	void RunEventAlt(vector<SUPEvent::SUPEventFilters::SUPSmartFilter>& v_Filters, VMValue* args);

	string s_ScriptName{};
	string s_FunctionName{};
	UInt64 ScriptHandle = 0;
	bool bRemoveOnSaveLoad = false;
	bool bDontCheckPermanentEventOnSaveLoad = false;
	int iRunTimesCount  = 0;
	bool bPermanent = false;


	string sPluginName{}; // FOR SUP EVENT CALLER OPTIMIZE LATER
	string sFormID{}; // FOR SUP EVENT CALLER OPTIMIZE LATER

	SUPEventFilters CurrentFilters;
};



class SUPEventHandler
{
public:
	SUPEventHandler(string sEventName, int EventIDIn);

	string EventName{};
	int EventID = 0;
	ICriticalSection s_Lock;
	vector<SUPEvent*> v_EventList;


	struct AllowedFilters {
		AllowedFilters(int numberIn, int typeIn) {
			number = numberIn;
			type = typeIn;
		}
		int number = 0;
		int type = 0;

	};

	vector<AllowedFilters> vAllowedFilters{};


	bool IsFilterAllowed(int type, int number)
	{
		for (auto it = vAllowedFilters.begin(); it != vAllowedFilters.end(); it++)
		{
			if (type == Iter.type)
			{
				if (number == Iter.number)
				{
					return true;
				}
			}
		}
		return false;
	}


	void EnterLock()
	{
		s_Lock.Enter();
	}

	void LeaveLock()
	{
		s_Lock.Leave();
	}


	__forceinline bool HasEvents()
	{
		return v_EventList.size();
	}


	UInt32 PushBackNewEvent(SUPEvent* NewEvent, bool bSkipCheck = false)
	{
		EnterLock();

		if (!bSkipCheck)
		{
			auto MyEvent = FindEventInList(NewEvent->s_ScriptName, NewEvent->s_FunctionName, NewEvent->ScriptHandle, NewEvent->bPermanent);

			if (MyEvent)
			{
				delete NewEvent;
				_DMESSAGE("Event is already in list - cannot add");
				LeaveLock();
				return -1;
			}
		}
		

		v_EventList.push_back(NewEvent);

		LeaveLock();
		return 1;
	}

	bool RemoveEvent(string sScriptName,string sFunctionName, UInt64 handle, bool bPermanent)
	{
		EnterLock();
		bool bResult = false;

		auto it = FindEventInListIter(sScriptName, sFunctionName, handle, bPermanent);

		if (it != v_EventList.end())
		{
			delete (*it);
			v_EventList.erase(it);
			bResult = true;
		}

		LeaveLock();
		return bResult;
	}


	int RemoveAllEventByScriptName(string sScriptName, UInt64 handle, bool bPermanent)
	{
		EnterLock();
		int iRemoved = 0;

		for (auto it = v_EventList.begin(); it != v_EventList.end();)
		{
			if (Iter->ScriptHandle == handle && Iter->bPermanent == bPermanent && Iter->s_ScriptName == sScriptName)
			{
				it = v_EventList.erase(it);
				iRemoved++;
			}
			else {
				it++;
			}
		}

		LeaveLock();
		return iRemoved;
	}

	int RemoveAllEventByScriptNameAndFunction(string sScriptName, string sFunctionName, UInt64 handle, bool bPermanent)
	{
		EnterLock();
		int iRemoved = 0;

		for (auto it = v_EventList.begin(); it != v_EventList.end();)
		{
			if (Iter->ScriptHandle == handle && Iter->bPermanent == bPermanent && Iter->s_ScriptName == sScriptName && Iter->s_FunctionName == sFunctionName)
			{
				it = v_EventList.erase(it);
				iRemoved++;
			}
			else {
				it++;
			}
		}

		LeaveLock();
		return iRemoved;
	}




	void RunEventsAlt(VMValue* args, vector<SUPEvent::SUPEventFilters::SUPSmartFilter>& v_PassedFilters)
	{
		//_DMESSAGE("RunEventsAlt starts for >>>%s", EventName.c_str());
		VMValue* NewArg = NULL;

		for (auto it = v_EventList.begin(); it != v_EventList.end();)
		{
			//_DMESSAGE("New event list")
				if (args)
				{
					NewArg = new VMValue((*args));
				}

			Iter->RunEventAlt(v_PassedFilters, NewArg);


			if (Iter->iRunTimesCount > 0)
			{
				if ((Iter->iRunTimesCount - 1) == 0)
				{
					_DMESSAGE("Erasing event(Run once)>>%s", Iter->s_FunctionName.c_str());
					delete Iter;
					it = v_EventList.erase(it);
					continue;
				}
				else {
					Iter->iRunTimesCount--;
				}
			}

			++it;

		}
		//_DMESSAGE("RunEvents ends for >>>s", EventName.c_str());

	}






	void RunEvents(VMValue* args)
	{
		vector<SUPEvent::SUPEventFilters::SUPSmartFilter> v_PassedFilters;
		RunEventsAlt(args, v_PassedFilters);
	}


	void ProcessOnSaveLoad()
	{
		EnterLock();
		for (auto it = v_EventList.begin(); it != v_EventList.end();)
		{
			if (Iter->bRemoveOnSaveLoad || Iter->bPermanent)
			{
				_DMESSAGE("Erasing event on game load>>%s", Iter->s_FunctionName.c_str());
				delete Iter;
				it = v_EventList.erase(it);
			}
			else {
				++it;
			}
		}
		LeaveLock();
	}






	//AttachedScriptReader::HandlerResult GetObjectHandleForEvent(SUPEvent* CurrentEvent)
	//{

	//	AttachedScriptReader::HandlerResult result;

	//	TESForm* CurrentObject = BuildRefFormFromSTR(CurrentEvent->sFormID, CurrentEvent->sPluginName, 0);

	//	if (!CurrentObject)
	//	{
	//		_DMESSAGE("Cannot build form from event.");
	//		return result;
	//	}

	//	return AttachedScriptReader::IsScriptLoadedForForm(CurrentEvent->s_ScriptName, CurrentObject);

	//}




	void CheckAndStoreFunctions() // No need to lock, happens at the start of the game
	{
		for (auto it = v_EventList.begin(); it != v_EventList.end();)
		{

			auto result = GetObjectHandleForEvent(Iter);

			if (!result.bSuccess)
			{
				_MESSAGE("Cannot find Script>>> %s in the VM list - removing from SUP Script runner.", Iter->s_ScriptName.c_str());
				delete Iter;
				it = v_EventList.erase(it);
				continue;
			}

			Iter->sPluginName.clear();
			Iter->sFormID.clear();
			Iter->ScriptHandle = result.handle;
			_DMESSAGE("Found %s in the stored VMScripts array", Iter->s_ScriptName.c_str());
			it++;
		}
	
	}


	void ProcessEventOnQuestFinished(bool bCompleted, TESQuest* CurrentQuest, bool bUnk)
	{

		EnterLock();
		if (HasEvents())
		{
			if (!CurrentQuest)
			{
				_DMESSAGE("EventOnQuestFinished::Quest is not valid");
				LeaveLock();
				return;
			}

			VMArray<VMVariable> arguments;
			VMValue args;


			VMVariable var1;
			var1.Set<bool>(&bCompleted);
			arguments.Push(&var1);

			VMVariable var0;
			var0.Set<TESQuest*>(&CurrentQuest);
			arguments.Push(&var0);




			arguments.PackArray(&args, g_MyVirtMachine);

			vector<SUPEvent::SUPEventFilters::SUPSmartFilter> v_PassedFilters;

			SUPEvent::SUPEventFilters::SUPSmartFilter NewFilter(SUPEvent::SUPEventFilters::k_FilterFirst, SUPEvent::SUPEventFilters::k_FilterFormID);
			std::get<vector<UInt32>>(NewFilter.List).push_back(CurrentQuest->formID);

			SUPEvent::SUPEventFilters::SUPSmartFilter NewFilter2(SUPEvent::SUPEventFilters::k_FilterSecond, SUPEvent::SUPEventFilters::k_FilterFloat);
			std::get<vector<float>>(NewFilter2.List).push_back(bCompleted);

			v_PassedFilters.push_back(NewFilter);
			v_PassedFilters.push_back(NewFilter2);

			RunEventsAlt(&args, v_PassedFilters);

		}

		LeaveLock();
	}

	void fn_OnPowerConnectionEvent_SendEvent(bool bAdded, TESObjectREFR* workshopRef, TESObjectREFR* akRef1, TESObjectREFR* akRef2, TESObjectREFR* akWireRef, Workshop::ExtraData* WorkshopExtraData = NULL)
	{

		VMArray<VMVariable> arguments;
		VMValue args;

		VMVariable var0;
		var0.Set<bool>(&bAdded);
		arguments.Push(&var0);

		VMVariable var1;
		var1.Set<TESObjectREFR*>(&workshopRef);
		arguments.Push(&var1);

		VMVariable var2;
		var2.Set<TESObjectREFR*>(&akRef1);
		arguments.Push(&var2);

		VMVariable var3;
		var3.Set<TESObjectREFR*>(&akRef2);
		arguments.Push(&var3);

		VMVariable var4;
		var4.Set<TESObjectREFR*>(&akWireRef);
		arguments.Push(&var4);




		VMVariable GridA_ID, GridA_Load, GridA_Capacity;
		VMVariable GridB_ID, GridB_Load, GridB_Capacity;


		bool Grid1Set = false;
		bool Grid2Set = false;

		if (akRef1)
		{
			auto pGrid1 = sup::compat::GetPowerGridForTarget(WorkshopExtraData, akRef1);

			if (pGrid1)
			{
				for (UInt32 i = 0; i < WorkshopExtraData->powerGrid.size(); i++)
				{
					PowerUtils::PowerGrid* pGrid;
					sup::compat::GetNthItem(WorkshopExtraData->powerGrid, i, pGrid);

					if (pGrid)
					{
						if (pGrid == pGrid1)
						{
							Grid1Set = true;
							GridA_ID.Set<UInt32>(&i);
							GridA_Load.Set<float>(&pGrid1->load);
							GridA_Capacity.Set<float>(&pGrid1->capacity);
							break;
						}
					}
				}
			}

		}

		if (!Grid1Set)
		{
			UInt32 iTemp = -1;
			GridA_ID.Set<UInt32>(&iTemp);
			GridA_Load.Set<float>(0);
			GridA_Capacity.Set<float>(0);
		}


		if (akRef2)
		{
			auto pGrid2 = sup::compat::GetPowerGridForTarget(WorkshopExtraData, akRef2);

			if (pGrid2)
			{
				for (UInt32 i = 0; i < WorkshopExtraData->powerGrid.size(); i++)
				{
					PowerUtils::PowerGrid* pGrid;
					sup::compat::GetNthItem(WorkshopExtraData->powerGrid, i, pGrid);

					if (pGrid)
					{
						if (pGrid == pGrid2)
						{
							Grid2Set = true;
							GridB_ID.Set<UInt32>(&i);
							GridB_Load.Set<float>(&pGrid2->load);
							GridB_Capacity.Set<float>(&pGrid2->capacity);
							break;
						}
					}
				}
			}

		}

		if (!Grid2Set)
		{
			UInt32 iTemp = -1;
			GridB_ID.Set<UInt32>(&iTemp);
			GridB_Load.Set<float>(0);
			GridB_Capacity.Set<float>(0);
		}




		arguments.Push(&GridA_ID);
		arguments.Push(&GridA_Load);
		arguments.Push(&GridA_Capacity);

		arguments.Push(&GridB_ID);
		arguments.Push(&GridB_Load);
		arguments.Push(&GridB_Capacity);


		bool IsSnapped = false;
		if (akRef1 && akRef2)
		{
			IsSnapped = fn_IsSnappedConnection(akRef1, akRef2);
		}

		VMVariable var5;
		var5.Set<bool>(&IsSnapped);
		arguments.Push(&var5);







		arguments.PackArray(&args, g_MyVirtMachine);




		vector<SUPEvent::SUPEventFilters::SUPSmartFilter> v_PassedFilters;

		SUPEvent::SUPEventFilters::SUPSmartFilter NewFilter1(SUPEvent::SUPEventFilters::k_FilterFirst, SUPEvent::SUPEventFilters::k_FilterFormID);
		std::get<vector<UInt32>>(NewFilter1.List).push_back(workshopRef->formID);

		SUPEvent::SUPEventFilters::SUPSmartFilter NewFilter2(SUPEvent::SUPEventFilters::k_FilterSecond, SUPEvent::SUPEventFilters::k_FilterFormID);

		if (akRef1)
		{
			std::get<vector<UInt32>>(NewFilter2.List).push_back(akRef1->formID);
		}

		if (akRef2)
		{
			std::get<vector<UInt32>>(NewFilter2.List).push_back(akRef2->formID);
		}
		

		SUPEvent::SUPEventFilters::SUPSmartFilter NewFilter3(SUPEvent::SUPEventFilters::k_FilterThird, SUPEvent::SUPEventFilters::k_FilterFloat);

		if (akWireRef)
		{
			std::get<vector<float>>(NewFilter3.List).push_back(1);
		}
		else {
			if (IsSnapped)
			{
				std::get<vector<float>>(NewFilter3.List).push_back(2);
			}
			else {
				std::get<vector<float>>(NewFilter3.List).push_back(3);
			}
		}





		v_PassedFilters.push_back(NewFilter1);
		v_PassedFilters.push_back(NewFilter2);
		v_PassedFilters.push_back(NewFilter3);

		RunEventsAlt(&args, v_PassedFilters);
	}

	void ProcessEventOnPowerConnection(bool bAdded, Workshop::ExtraData* WorkshopExtraData, TESObjectREFR* akRef1, TESObjectREFR* akRef2, TESObjectREFR* akWireRef)
	{
		EnterLock();
		if (HasEvents())
		{
			if (!akRef1)
			{
				_DMESSAGE("Connection added, akRef1 is not valid");
				fn_OnPowerConnectionEvent_SendEvent(bAdded, NULL, akRef1, akRef2, akWireRef);
				LeaveLock();
				return;
			}

			TESObjectREFR* workshopRef = GetLinkedRef_Native(akRef1, g_SUPWorkshopKeyword);
			
			if (!workshopRef)
			{
				_DMESSAGE("Connection removed, workshopRef is not valid");
				fn_OnPowerConnectionEvent_SendEvent(bAdded, NULL, akRef1, akRef2, akWireRef);
				LeaveLock();
				return;
			}
			fn_OnPowerConnectionEvent_SendEvent(bAdded, workshopRef, akRef1, akRef2, akWireRef, WorkshopExtraData);
		}
		LeaveLock();
	}







	void ProcessEventOnQuestActive(bool bActive, TESQuest* CurrentQuest)
	{

		EnterLock();
		if (HasEvents())
		{
			if (!CurrentQuest)
			{
				_DMESSAGE("OnQuestActive::Quest is not valid");
				LeaveLock();
				return;
			}

			VMArray<VMVariable> arguments;
			VMValue args;


			VMVariable var1;
			var1.Set<bool>(&bActive);
			arguments.Push(&var1);

			VMVariable var0;
			var0.Set<TESQuest*>(&CurrentQuest);
			arguments.Push(&var0);

			arguments.PackArray(&args, g_MyVirtMachine);

			vector<SUPEvent::SUPEventFilters::SUPSmartFilter> v_PassedFilters;

			SUPEvent::SUPEventFilters::SUPSmartFilter NewFilter(SUPEvent::SUPEventFilters::k_FilterFirst, SUPEvent::SUPEventFilters::k_FilterFormID);
			std::get<vector<UInt32>>(NewFilter.List).push_back(CurrentQuest->formID);

			SUPEvent::SUPEventFilters::SUPSmartFilter NewFilter2(SUPEvent::SUPEventFilters::k_FilterSecond, SUPEvent::SUPEventFilters::k_FilterFloat);
			std::get<vector<float>>(NewFilter2.List).push_back(bActive);

			v_PassedFilters.push_back(NewFilter);
			v_PassedFilters.push_back(NewFilter2);

			RunEventsAlt(&args, v_PassedFilters);

		}

		LeaveLock();
	}






	void ProcessEventOnPlayerMapMarkerStateChangeEvent(bool bAdded,  NiPoint3 coords, TESWorldSpace* TargetWorldspace = NULL)
	{
		EnterLock();
		if (HasEvents())
		{

			if (!bAdded)
			{
				TESObjectREFR* MarkerRef = MarkerRef = f_GetPlayerMapMarker();
				if (MarkerRef)
				{
					coords.x = MarkerRef->GetPosition().x;
					coords.y= MarkerRef->GetPosition().y;
					coords.z = MarkerRef->GetPosition().z;
					if (MarkerRef->parentCell)
					{
						TargetWorldspace = MarkerRef->parentCell->worldSpace;
					}
				}
			}



			VMArray<VMVariable> arguments;
			VMValue args;

			VMVariable var0;
			var0.Set<bool>(&bAdded);
			arguments.Push(&var0);

			VMVariable var1;
			var1.Set<TESWorldSpace*>(&TargetWorldspace);
			arguments.Push(&var1);

			
			VMVariable var2;
			var2.Set<float>(&coords.x);
			arguments.Push(&var2);

			VMVariable var3;
			var3.Set<float>(&coords.y);
			arguments.Push(&var3);

			VMVariable var4;
			var4.Set<float>(&coords.z);
			arguments.Push(&var4);

			arguments.PackArray(&args, g_MyVirtMachine);


			vector<SUPEvent::SUPEventFilters::SUPSmartFilter> v_PassedFilters;
			SUPEvent::SUPEventFilters::SUPSmartFilter NewFilter(SUPEvent::SUPEventFilters::k_FilterFirst, SUPEvent::SUPEventFilters::k_FilterFloat);
			std::get<vector<float>>(NewFilter.List).push_back(bAdded);
			v_PassedFilters.push_back(NewFilter);

			RunEventsAlt(&args, v_PassedFilters);
		}
		LeaveLock();
	}





	void ProcessEventOnRagdollStart(Actor* ActorRef, NiPoint3* PosInfo, float force)
	{
		EnterLock();
		if (HasEvents())
		{

			if (!ActorRef)
			{
				_DMESSAGE("RagdollStart no Actor.");
				LeaveLock();
				return;

			}
			if (!PosInfo)
			{
				_DMESSAGE("RagdollStart no PosInfo.");
				LeaveLock();
				return;
			}


			VMArray<VMVariable> arguments;
			VMValue args;

			VMVariable var0;
			var0.Set<Actor*>(&ActorRef);
			arguments.Push(&var0);


			VMVariable var2;
			var2.Set<float>(&PosInfo->x);
			arguments.Push(&var2);

			VMVariable var3;
			var3.Set<float>(&PosInfo->y);
			arguments.Push(&var3);

			VMVariable var4;
			var4.Set<float>(&PosInfo->z);
			arguments.Push(&var4);

			VMVariable var5;
			var5.Set<float>(&force);
			arguments.Push(&var5);

			arguments.PackArray(&args, g_MyVirtMachine);


			vector<SUPEvent::SUPEventFilters::SUPSmartFilter> v_PassedFilters;


			SUPEvent::SUPEventFilters::SUPSmartFilter NewFilter(SUPEvent::SUPEventFilters::k_FilterFirst, SUPEvent::SUPEventFilters::k_FilterFormID);
			std::get<vector<UInt32>>(NewFilter.List).push_back(ActorRef->formID);


			SUPEvent::SUPEventFilters::SUPSmartFilter NewFilter2(SUPEvent::SUPEventFilters::k_FilterSecond, SUPEvent::SUPEventFilters::k_FilterFormID);
			std::get<vector<UInt32>>(NewFilter.List).push_back(ActorRef->GetObjectReference()->formID);


			v_PassedFilters.push_back(NewFilter);
			v_PassedFilters.push_back(NewFilter2);




			RunEventsAlt(&args, v_PassedFilters);
		}
		LeaveLock();
		
	}



	void ProcessEventOnActorRagdollFinish(Actor* ActorRef)
	{
		EnterLock();
		if (HasEvents())
		{

			if (!ActorRef)
			{
				_DMESSAGE("Ragdoll Finish no Actor.");
				LeaveLock();
				return;

			}


			VMArray<VMVariable> arguments;
			VMValue args;

			VMVariable var0;
			var0.Set<Actor*>(&ActorRef);
			arguments.Push(&var0);
			arguments.PackArray(&args, g_MyVirtMachine);


			vector<SUPEvent::SUPEventFilters::SUPSmartFilter> v_PassedFilters;
			SUPEvent::SUPEventFilters::SUPSmartFilter NewFilter(SUPEvent::SUPEventFilters::k_FilterFirst, SUPEvent::SUPEventFilters::k_FilterFormID);
			std::get<vector<UInt32>>(NewFilter.List).push_back(ActorRef->formID);
			v_PassedFilters.push_back(NewFilter);

			RunEventsAlt(&args, v_PassedFilters);
		}
		LeaveLock();

	}








	void ProcessEventUniversalOneBool(bool bValue)
	{
		EnterLock();
		if (HasEvents())
		{
			VMArray<VMVariable> arguments;
			VMValue args;
			VMVariable var0;
			var0.Set<bool>(&bValue);
			arguments.Push(&var0);
			arguments.PackArray(&args, g_MyVirtMachine);


			vector<SUPEvent::SUPEventFilters::SUPSmartFilter> v_PassedFilters;
			SUPEvent::SUPEventFilters::SUPSmartFilter NewFilter(SUPEvent::SUPEventFilters::k_FilterFirst, SUPEvent::SUPEventFilters::k_FilterFloat);
			std::get<vector<float>>(NewFilter.List).push_back(bValue);
			v_PassedFilters.push_back(NewFilter);

			RunEventsAlt(&args, v_PassedFilters);
		}
		LeaveLock();
	}




	void ProcessEventOnCrosshairRefChange(bool OnCrosshairOn,TESObjectREFR* NewCrosshairRef, UInt32 CrosshairType)
	{
		EnterLock();

		VMArray<VMVariable> arguments;
		VMValue args;

		VMVariable varNULL;
		varNULL.Set<bool>(&OnCrosshairOn);
		arguments.Push(&varNULL);

		VMVariable var0;
		var0.Set<TESObjectREFR*>(&NewCrosshairRef);
		arguments.Push(&var0);

		VMVariable var1;
		var1.Set<UInt32>(&CrosshairType);
		arguments.Push(&var1);

		arguments.PackArray(&args, g_MyVirtMachine);




		SUPEvent::SUPEventFilters::SUPSmartFilter NewFilter1(SUPEvent::SUPEventFilters::k_FilterFirst, SUPEvent::SUPEventFilters::k_FilterFloat);
		std::get<vector<float>>(NewFilter1.List).push_back(OnCrosshairOn);



		SUPEvent::SUPEventFilters::SUPSmartFilter NewFilter2(SUPEvent::SUPEventFilters::k_FilterSecond, SUPEvent::SUPEventFilters::k_FilterFormID);
		std::get<vector<UInt32>>(NewFilter2.List).push_back(NewCrosshairRef->formID);


		SUPEvent::SUPEventFilters::SUPSmartFilter NewFilter3(SUPEvent::SUPEventFilters::k_FilterThird, SUPEvent::SUPEventFilters::k_FilterFormID);
		SUPEvent::SUPEventFilters::SUPSmartFilter NewFilter4(SUPEvent::SUPEventFilters::k_FilterFourth, SUPEvent::SUPEventFilters::k_FilterFloat);

		if (NewCrosshairRef->GetObjectReference())
		{
			std::get<vector<UInt32>>(NewFilter3.List).push_back(NewCrosshairRef->GetObjectReference()->formID);
			std::get<vector<float>>(NewFilter4.List).push_back(static_cast<float>(NewCrosshairRef->GetObjectReference()->formType.underlying()));
		}
		else {
			std::get<vector<UInt32>>(NewFilter3.List).push_back(NULL);
			std::get<vector<float>>(NewFilter4.List).push_back(NULL);
		}



		

		vector<SUPEvent::SUPEventFilters::SUPSmartFilter> v_PassedFilters;
		v_PassedFilters.push_back(NewFilter1);
		v_PassedFilters.push_back(NewFilter2);
		v_PassedFilters.push_back(NewFilter3);
		v_PassedFilters.push_back(NewFilter4);

		RunEventsAlt(&args, v_PassedFilters);





		LeaveLock();
	}



	void ProcessEventOnWorkshopPowerStateChange(TESObjectREFR* ObjectRef,TESObjectREFR * workshopRef, PowerGridChanges& resultChanges, UInt32 iEvent, TESObjectREFR* akRef1, TESObjectREFR* akRef2, TESObjectREFR* akWireRef)
	{
		// iReason NOT USED
		// 1 - Power grid count changed
		// 2 - Power rating changed
		// 3 - Power grid capacity changed
		// 4 - Power grid load changed
		// 5 - Power grid  Currently powered items count changed

		// iEvent
		// 1 - AddConnection - contains 3 refs
		// 2 - RemoveConnection - contains 3 refs
		// 3 - OtherAction- contains 1 ref - akRef1.


		EnterLock();
		if (!ObjectRef && iEvent !=4)
		{
			_DMESSAGE("Crosshair is not valid");
			LeaveLock();
			return;
		}

		if (!workshopRef)
		{
			workshopRef = GetLinkedRef_Native(ObjectRef, g_SUPWorkshopKeyword);

			if (!workshopRef)
			{
				_DMESSAGE("Workshop is not valid");
				LeaveLock();
				return;
			}
		}



		VMArray<VMVariable> arguments;
		VMValue args;

		VMVariable var1;
		var1.Set<TESObjectREFR*>(&workshopRef);
		arguments.Push(&var1);

		VMVariable var2;
		var2.Set<UInt32>(&iEvent);
		arguments.Push(&var2);



		VMArray<UInt32> AffectedPowerGridsAr;
		for (auto it = resultChanges.AffectedPowerGrids.begin(); it != resultChanges.AffectedPowerGrids.end(); it++)
		{
			AffectedPowerGridsAr.Push(&Iter);
		}
		VMVariable AffectedPowerGridsVAR;
		AffectedPowerGridsVAR.Set<VMArray<UInt32>>(&AffectedPowerGridsAr);
		arguments.Push(&AffectedPowerGridsVAR);


		VMArray<UInt32> DeletedPowerGridsAr;
		for (auto it = resultChanges.DeletedPowerGrids.begin(); it != resultChanges.DeletedPowerGrids.end(); it++)
		{
			DeletedPowerGridsAr.Push(&Iter);
		}
		VMVariable DeletedPowerGridsVAR;
		DeletedPowerGridsVAR.Set<VMArray<UInt32>>(&DeletedPowerGridsAr);
		arguments.Push(&DeletedPowerGridsVAR);


		VMArray<TESObjectREFR*> PassedRefs;

		if (iEvent == 1 || iEvent == 2)
		{
			PassedRefs.Push(&akRef1);
			PassedRefs.Push(&akRef2);
			PassedRefs.Push(&akWireRef);
		}
		else if (iEvent == 3)
		{
			PassedRefs.Push(&ObjectRef);
		}



		VMVariable PassedRefsVAR;
		PassedRefsVAR.Set<VMArray<TESObjectREFR*>>(&PassedRefs);
		arguments.Push(&PassedRefsVAR);



		arguments.PackArray(&args, g_MyVirtMachine);


		vector<SUPEvent::SUPEventFilters::SUPSmartFilter> v_PassedFilters;
		SUPEvent::SUPEventFilters::SUPSmartFilter NewFilter(SUPEvent::SUPEventFilters::k_FilterFirst, SUPEvent::SUPEventFilters::k_FilterFormID);
		std::get<vector<UInt32>>(NewFilter.List).push_back(workshopRef->formID);
		v_PassedFilters.push_back(NewFilter);



		RunEventsAlt(&args, v_PassedFilters);

		LeaveLock();
	}


	void ProcessEventOnDismemberLimb(Actor* CurrentActor, UInt32 iLimbNum)
	{
		//  0 - "Torso"
		//	1 - "Head1"
		//	2 - "Eye"
		//	3 - "LookAt"
		//	4 - "Fly Grab"
		//	5 - "Head2"
		//	6 - "LeftArm1"
		//	7 - "LeftArm2"
		//	8 - "RightArm1"
		//	9 - "RightArm2"
		//	10 - "LeftLeg1"
		//	11 - "LeftLeg2"
		//	12 - "LeftLeg3"
		//	13 - "RightLeg1"
		//	14 - "RightLeg2"
		//	15 - "RightLeg3"
		//	16 - "Brain"
		//	17 - "Weapon"
		//	18 - "Root"
		//	19 - "COM"
		//	20 - "Pelvis"
		//	21 - "Camera"
		//	22 - "Offset Root"
		//	23 - "Left Foot"
		//	24 - "Right Foot"


		VMArray<VMVariable> arguments;
		VMValue args;

		VMVariable var1;
		var1.Set<Actor*>(&CurrentActor);
		arguments.Push(&var1);

		VMVariable var2;
		var2.Set<UInt32>(&iLimbNum);
		arguments.Push(&var2);

		arguments.PackArray(&args, g_MyVirtMachine);



		vector<SUPEvent::SUPEventFilters::SUPSmartFilter> v_PassedFilters;

		SUPEvent::SUPEventFilters::SUPSmartFilter NewFilter(SUPEvent::SUPEventFilters::k_FilterFirst, SUPEvent::SUPEventFilters::k_FilterFormID);
		std::get<vector<UInt32>>(NewFilter.List).push_back(CurrentActor->formID);

		SUPEvent::SUPEventFilters::SUPSmartFilter NewFilter2(SUPEvent::SUPEventFilters::k_FilterSecond, SUPEvent::SUPEventFilters::k_FilterFloat);
		std::get<vector<float>>(NewFilter2.List).push_back(iLimbNum);

		v_PassedFilters.push_back(NewFilter);
		v_PassedFilters.push_back(NewFilter2);



		RunEventsAlt(&args, v_PassedFilters);
	}


	bool IsEventHandlerInList(string sScriptName, string sFunctionName,  UInt64 handle, bool bPermanent)
	{
		return FindEventInListBool(sScriptName, sFunctionName, handle, bPermanent);
	}



	private:

		SUPEvent* FindEventInList(string sScriptName, string sFunctionName, UInt64 handle, bool bPermanent)
		{
			for (auto it = v_EventList.begin(); it != v_EventList.end(); it++)
			{
				if (Iter->ScriptHandle == handle && Iter->bPermanent == bPermanent && Iter->s_ScriptName == sScriptName && Iter->s_FunctionName == sFunctionName)
				{
					return Iter;
				}
			}
			return NULL;
		}

		bool FindEventInListBool(string sScriptName, string sFunctionName, UInt64 handle, bool bPermanent)
		{
			for (auto it = v_EventList.begin(); it != v_EventList.end(); it++)
			{
				if (Iter->ScriptHandle == handle && Iter->bPermanent == bPermanent && Iter->s_ScriptName == sScriptName && Iter->s_FunctionName == sFunctionName)
				{
					return true;
				}
			}
			return false;
		}


		vector<SUPEvent*>::iterator FindEventInListIter(string sScriptName, string sFunctionName, UInt64 handle, bool bPermanent)
		{
			for (auto it = v_EventList.begin(); it != v_EventList.end(); it++)
			{
				if (Iter->ScriptHandle == handle && Iter->bPermanent == bPermanent && Iter->s_ScriptName == sScriptName && Iter->s_FunctionName == sFunctionName)
				{
					return it;
				}
			}
			return v_EventList.end();
		}





};

vector<SUPEventHandler*> v_EventHandlers{};



namespace SUPEventHandlers {
	SUPEventHandler* FindSUPEventHandlerByEventName(string sEventName)
	{
		for (auto it = v_EventHandlers.begin(); it != v_EventHandlers.end(); it++)
		{
			if (sEventName == Iter->EventName)
			{
				return Iter;
			}
		}

		return NULL;
	}

	SUPEventHandler* FindSUPEventHandlerByType(int type)
	{
		for (auto it = v_EventHandlers.begin(); it != v_EventHandlers.end(); it++)
		{
			if (type == Iter->EventID)
			{
				return Iter;
			}
		}

		return NULL;
	}


	void CleanPermanentEventsOnGameLoad(){

	}





};









enum SUPEventTypes {
	ongameload = 1,
	oncellchange,
	oncellfirsttimevisit,
	ongamesave,
	onpowerconnection,
	onquestfinished,
	onquestactive,
	onconsolecommand,
	onplayermapmarkerstate,
	onactorragdollstart,
	onplayerradiostate,
	onactorragdollfinish,
	oncrosshairrefchange,
	onworkshoppowerstatechange,
	ondismemberlimb,
};


SUPEventHandler::SUPEventHandler(string sEventName, int EventIDIn) {
	EventName = sEventName;
	EventID = EventIDIn;


	if (EventID == SUPEventTypes::oncellchange || EventID == SUPEventTypes::oncellfirsttimevisit || EventID == SUPEventTypes::onworkshoppowerstatechange)
	{
		SUPEventHandler::AllowedFilters Filter1(SUPEvent::SUPEventFilters::k_FilterFirst, SUPEvent::SUPEventFilters::k_FilterFormID);
		vAllowedFilters.push_back(Filter1);
	}
	else if (EventID == SUPEventTypes::onpowerconnection)
	{
		SUPEventHandler::AllowedFilters Filter1(SUPEvent::SUPEventFilters::k_FilterFirst, SUPEvent::SUPEventFilters::k_FilterFormID);
		SUPEventHandler::AllowedFilters Filter2(SUPEvent::SUPEventFilters::k_FilterSecond, SUPEvent::SUPEventFilters::k_FilterFormID);
		SUPEventHandler::AllowedFilters Filter3(SUPEvent::SUPEventFilters::k_FilterThird, SUPEvent::SUPEventFilters::k_FilterFloat);

		vAllowedFilters.reserve(3);
		vAllowedFilters.push_back(Filter1);
		vAllowedFilters.push_back(Filter2);
		vAllowedFilters.push_back(Filter3);

	}
	else if (EventID == SUPEventTypes::onconsolecommand)
	{
		SUPEventHandler::AllowedFilters Filter1(SUPEvent::SUPEventFilters::k_FilterFirst, SUPEvent::SUPEventFilters::k_FilterFormID);
		SUPEventHandler::AllowedFilters Filter2(SUPEvent::SUPEventFilters::k_FilterSecond, SUPEvent::SUPEventFilters::k_FilterString);

		vAllowedFilters.reserve(2);
		vAllowedFilters.push_back(Filter1);
		vAllowedFilters.push_back(Filter2);
	}
	else if (EventID == SUPEventTypes::onplayermapmarkerstate || EventID == SUPEventTypes::onplayerradiostate)
	{
		SUPEventHandler::AllowedFilters Filter1(SUPEvent::SUPEventFilters::k_FilterFirst, SUPEvent::SUPEventFilters::k_FilterFloat);
		vAllowedFilters.push_back(Filter1);
	}
	else if (EventID == SUPEventTypes::oncrosshairrefchange)
	{
		SUPEventHandler::AllowedFilters Filter1(SUPEvent::SUPEventFilters::k_FilterFirst, SUPEvent::SUPEventFilters::k_FilterFloat); // Set or remove
		SUPEventHandler::AllowedFilters Filter2(SUPEvent::SUPEventFilters::k_FilterSecond, SUPEvent::SUPEventFilters::k_FilterFormID); // Base form ID
		SUPEventHandler::AllowedFilters Filter3(SUPEvent::SUPEventFilters::k_FilterThird, SUPEvent::SUPEventFilters::k_FilterFormID); // REF ID
		SUPEventHandler::AllowedFilters Filter4(SUPEvent::SUPEventFilters::k_FilterFourth, SUPEvent::SUPEventFilters::k_FilterFloat); // Type
		vAllowedFilters.reserve(4);
		vAllowedFilters.push_back(Filter1);
		vAllowedFilters.push_back(Filter2);
		vAllowedFilters.push_back(Filter3);
		vAllowedFilters.push_back(Filter4);
	}
	else if ( EventID == SUPEventTypes::ondismemberlimb || EventID == SUPEventTypes::onquestfinished || EventID == SUPEventTypes::onquestactive)
	{
		SUPEventHandler::AllowedFilters Filter1(SUPEvent::SUPEventFilters::k_FilterFirst, SUPEvent::SUPEventFilters::k_FilterFormID);
		SUPEventHandler::AllowedFilters Filter2(SUPEvent::SUPEventFilters::k_FilterSecond, SUPEvent::SUPEventFilters::k_FilterFloat);
		vAllowedFilters.reserve(2);
		vAllowedFilters.push_back(Filter1);
		vAllowedFilters.push_back(Filter2);
	}
	else if (EventID == SUPEventTypes::onactorragdollfinish || EventID == SUPEventTypes::onactorragdollstart) {
		SUPEventHandler::AllowedFilters Filter1(SUPEvent::SUPEventFilters::k_FilterFirst, SUPEvent::SUPEventFilters::k_FilterFormID); // Base form ID
		SUPEventHandler::AllowedFilters Filter2(SUPEvent::SUPEventFilters::k_FilterSecond, SUPEvent::SUPEventFilters::k_FilterFormID); // REF ID
		vAllowedFilters.reserve(2);
		vAllowedFilters.push_back(Filter1);
		vAllowedFilters.push_back(Filter2);
	}



	v_EventHandlers.push_back(this);
}





SUPEventHandler OnGameLoadEventHandler("ongameload", SUPEventTypes::ongameload); // No filters
SUPEventHandler OnCellChangeEventHandler("oncellchange", SUPEventTypes::oncellchange); //DONE
SUPEventHandler OnFirstTimeVisitEventHandler("oncellfirsttimevisit", SUPEventTypes::oncellfirsttimevisit);//DONE
SUPEventHandler OnGameSaveEventHandler("ongamesave", SUPEventTypes::ongamesave); // No filters
SUPEventHandler OnPowerConnectionEventHandler("onpowerconnection", SUPEventTypes::onpowerconnection); //DONE
SUPEventHandler OnQuestFinishedEventHandler("onquestfinished", SUPEventTypes::onquestfinished);//DONE
SUPEventHandler OnQuestActiveEventHandler("onquestactive", SUPEventTypes::onquestactive);//DONE
SUPEventHandler OnConsoleCommandEventHandler("onconsolecommand", SUPEventTypes::onconsolecommand); //DONE
SUPEventHandler OnPlayerMapMarkerStateChangeEventHandler("onplayermapmarkerstatechange", SUPEventTypes::onplayermapmarkerstate); //DONE
SUPEventHandler OnActorRagdollStartEventHandler("onactorragdollstart", SUPEventTypes::onactorragdollstart);//DONE
SUPEventHandler OnPlayerRadioStateEventHandler("onplayerradiostate", SUPEventTypes::onplayerradiostate); //DONE
SUPEventHandler OnActorRagdollFinishEventHandler("onactorragdollfinish", SUPEventTypes::onactorragdollfinish); //DONE
SUPEventHandler OnCrosshairRefChangeEventHandler("oncrosshairrefchange", SUPEventTypes::oncrosshairrefchange); //DONE
SUPEventHandler OnWorkshopPowerStateChangeEventHandler("onworkshoppowerstatechange", SUPEventTypes::onworkshoppowerstatechange); //DONE
SUPEventHandler OnDismemberLimbEventHandler("ondismemberlimb", SUPEventTypes::ondismemberlimb); //DONE

//vector<SUPEvent*> v_SUPEventOnCellChange;
//vector<SUPEvent*> g_EventScriptArrayOnLoad;






template <typename T> void __forceinline CompareEventLists(int& iMatchedFiltersCount,bool& bFound, vector<SUPEvent::SUPEventFilters::SUPSmartFilter>::iterator& it, vector<SUPEvent::SUPEventFilters::SUPSmartFilter>::iterator& FoundCurrentFilter)
{
	for (auto it3 = get<vector<T>>(Iter.List).begin(); it3 != get<vector<T>>(Iter.List).end(); it3++)
	{
		if (std::find(get<vector<T>>((*FoundCurrentFilter).List).begin(), get<vector<T>>((*FoundCurrentFilter).List).end(), Iter3) != get<vector<T>>((*FoundCurrentFilter).List).end())
		{
			iMatchedFiltersCount++;
			bFound = true;
			break;
		}
	}



}




void SUPEvent::RunEventAlt(vector<SUPEvent::SUPEventFilters::SUPSmartFilter>& v_PassedFilters, VMValue* args = NULL)
{
	_DMESSAGE("Running event script>>>%s named>>>> %s ", s_ScriptName.c_str(), s_FunctionName.c_str());


	//for (auto it = CurrentFilters.v_Filters.begin(); it != CurrentFilters.v_Filters.end();it++)
	//{
	//	_DMESSAGE("Current filter number>>%d,type>>%d", Iter.Number, Iter.type);
	//}

	//for (auto it = v_PassedFilters.begin(); it != v_PassedFilters.end(); it++)
	//{
	//	_DMESSAGE("Passed filter number>>%d,type>>%d", Iter.Number, Iter.type);
	//}

	int CurrentFiltersCount = CurrentFilters.v_Filters.size();

	if (CurrentFiltersCount)
	{
		int iMatchedFiltersCount = 0;
		
		for (auto it = v_PassedFilters.begin(); it != v_PassedFilters.end(); it++)
		{
			//_DMESSAGE("Passed filter number>>%d,type>>%d",Iter.Number,Iter.type);

			auto FoundCurrentFilter = std::find(CurrentFilters.v_Filters.begin(), CurrentFilters.v_Filters.end(), Iter);
			if (FoundCurrentFilter != CurrentFilters.v_Filters.end())
			{
				//_DMESSAGE("Filter found in current filters.")
				if (Iter.type == SUPEvent::SUPEventFilters::k_FilterFormID)
				{
					//_DMESSAGE("Filter type FORM ID");
					bool bFound = false;
					CompareEventLists<UInt32>(iMatchedFiltersCount, bFound,it, FoundCurrentFilter);

					if (!bFound)
					{
						iMatchedFiltersCount = -1;
						//_DMESSAGE("DOSROCHNO zavershit 1");
						break;
					}

				}else if (Iter.type == SUPEvent::SUPEventFilters::k_FilterFloat)
				{
					bool bFound = false;
					CompareEventLists<float>(iMatchedFiltersCount, bFound, it, FoundCurrentFilter);

					if (!bFound)
					{
						iMatchedFiltersCount = -1;
						//_DMESSAGE("DOSROCHNO zavershit 2");
						break;
					}
				}
				else if (Iter.type == SUPEvent::SUPEventFilters::k_FilterString)
				{
					//_DMESSAGE("COMPARING STRING");

					//for (auto it3 = get<vector<string>>((*FoundCurrentFilter).List).begin(); it3 != get<vector<string>>((*FoundCurrentFilter).List).end(); it3++) {
					//	_DMESSAGE("strings in current filter>>%s", Iter3.c_str());
					//}

					//for (auto it3 = get<vector<string>>(Iter.List).begin(); it3 != get<vector<string>>(Iter.List).end(); it3++)
					//{
					//	_DMESSAGE("strings in current iter>>%s", Iter3.c_str());
					//}


					bool bFound = false;
					CompareEventLists<string>(iMatchedFiltersCount, bFound, it, FoundCurrentFilter);

					if (!bFound)
					{
						iMatchedFiltersCount = -1;
						_DMESSAGE("DOSROCHNO zavershit 3");
						break;
					}
				}

			}

		}


		if (iMatchedFiltersCount != CurrentFiltersCount)
		{
			if (args)
				delete args;

			//_DMESSAGE("Doesn't pass filter for this event,iMatchedFiltersCount>>%d,CurrentFiltersCount>>%d", iMatchedFiltersCount,CurrentFiltersCount);
			return;
		}

	}
	else {
		//_DMESSAGE("filters don't exist.");
	}


	if (!args)
	{
		_DMESSAGE("No args");
		args = g_packedArgs; // NONE
	}



	//if (bFunctionIsLocal)
	//{
		VMIdentifier* identifier = NULL;
		if (!g_MyVirtMachine->GetObjectIdentifier(ScriptHandle, s_ScriptName.c_str(), 0, &identifier, 0))
		{
			if (args && args != g_packedArgs)
				delete args;


			_DMESSAGE("VM identifier NOT found");
			return;
		}

		//_DMESSAGE("VM identifier found");

		BSFixedString g_FuncName(s_FunctionName.c_str());


		CallFunctionNoWait_Internal(g_MyVirtMachine, 0, identifier, &g_FuncName, args);

		//_DMESSAGE("Calling local:::ScriptName is %s, FunctionName is %s", s_ScriptName.c_str(), s_FunctionName.c_str());
	//}
	//else {
	//	BSFixedString g_ScriptName(s_ScriptName.c_str());
	//	BSFixedString g_FuncName(s_FunctionName.c_str());
	//	CallGlobalFunctionNoWait_Internal(g_MyVirtMachine, 0, 0, &g_ScriptName, &g_FuncName, args);
	//	_DMESSAGE("Calling global:::ScriptName is %s, FunctionName is %s", s_ScriptName.c_str(), s_FunctionName.c_str());
	//}

}





namespace SUPEventFilterHelper {







	void GetFormIDFromVMValue(vector<UInt32>& vList, VMValue& mValue, VMVariable& var, int& iFilterInvalidNum)
	{

		if (!mValue.data.id)
		{
			iFilterInvalidNum = 1;
			return;
		}

		if ((*g_objectHandlePolicy)->IsType(kFormType_CELL, mValue.data.id->GetHandle()))
		{
			_DMESSAGE(" CURRENT VAR IS kFormType_CELL!!!!");
		}

		VMArray<VMVariable> varr;
		if (var.Get(&varr))
		{
			for (UInt32 i = 0; i < varr.Length(); i++)
			{
				VMVariable var;
				varr.Get(&var, i);

				TESObjectREFR* refr = nullptr;
				var.Get(&refr);
				if (refr)
					vList.push_back(refr->formID);
			}
			return;
		}



		if ((*g_objectHandlePolicy)->IsType(kFormType_REFR, mValue.data.id->GetHandle()))
		{
			// Resolve the handle to the live reference and store its form ID (the
			// raw handle is 64-bit and would be truncated in the UInt32 filter list).
			TESObjectREFR* refr = nullptr;
			if (var.Get(&refr) && refr)
			{
				vList.push_back(refr->formID);
			}
			else {
				iFilterInvalidNum = 1;
			}
			return;
		}

		BGSListForm* formList = nullptr;
		if (var.Get(&formList) && formList)
		{
			for (UInt32 i = 0; i < formList->arrayOfForms.size(); i++)
			{
				TESForm* form = formList->arrayOfForms[i];

				if (form)
				{
					vList.push_back(form->formID);
					_DMESSAGE("ADDING FROM FORM LIST>>%x", form->formID);
				}

			}
			return;
		}

		if ((*g_objectHandlePolicy)->IsType(0, mValue.data.id->GetHandle()))
		{
			TESForm* form = nullptr;
			if (var.Get(&form) && form)
			{
				vList.push_back(form->formID);
			}
			else {
				iFilterInvalidNum = 1;
			}
			return;
		}

	}




	void ProcessVarArrayElementAlt(SUPEvent::SUPEventFilters& result, VMVariable& var, SUPEventHandler* EventHandler, UInt32 iNumber, int& iFilterInvalidNum)
	{

		if (var.IsNone())
		{
			_DMESSAGE("Element is NONE");
			return;
		}

		int iAcceptedType = EventHandler->vAllowedFilters.at(iNumber).type;
		//_DMESSAGE("iAcceptedType>>%d", iAcceptedType);
		auto mValue = var.GetValue();
		UInt8 type = mValue.GetTypeEnum();
		//_DMESSAGE("TYPE>>%d", type);

		if (iAcceptedType == SUPEvent::SUPEventFilters::k_FilterString)
		{

			if (type == VMValue::kType_String)
			{
				BSFixedString MyString;
				if (var.Get(&MyString))
				{
					SUPEvent::SUPEventFilters::SUPSmartFilter NewFilter(iNumber, SUPEvent::SUPEventFilters::k_FilterString);
					string sLowerString(MyString.c_str());
					sup::string::to_lower(sLowerString);

					if (f_ContainsOnlyASCII(sLowerString))
					{
						std::get<vector<string>>(NewFilter.List).push_back(sLowerString.c_str());
						//_DMESSAGE("SINGULAR STRING ADDED>>%s", sLowerString.c_str());
					}
					else {
						_DMESSAGE("String contains non ASCII chars");
					}

				}
			}
			else if (type == VMValue::kType_VariableArray)
			{
				VMArray<VMVariable> varr;

				if (var.Get(&varr)) {


					SUPEvent::SUPEventFilters::SUPSmartFilter NewFilter(iNumber, SUPEvent::SUPEventFilters::k_FilterString);

					for (UInt32 i = 0; i < varr.Length(); i++) {

						VMVariable CurrentVar;
						varr.Get(&CurrentVar, i);

						BSFixedString MyString;

						if (CurrentVar.Get(&MyString)) {
							//_DMESSAGE("Current string from array>>%s", MyString.c_str());
							string sLowerString(MyString.c_str());
							sup::string::to_lower(sLowerString);
							if (f_ContainsOnlyASCII(sLowerString))
							{
								std::get<vector<string>>(NewFilter.List).push_back(sLowerString.c_str());
								//_DMESSAGE("SINGULAR STRING ADDED>>%s", sLowerString.c_str());
							}
							else {
								_DMESSAGE("String contains non ASCII chars");
							}
						}


					}

					if (std::get<vector<string>>(NewFilter.List).size())
					{
						result.v_Filters.push_back(NewFilter);
					}

				}
				else {
					_DMESSAGE("Cannot extract var array.");
				}



			}



		}
		else if (iAcceptedType == SUPEvent::SUPEventFilters::k_FilterFloat)
		{

			SUPEvent::SUPEventFilters::SUPSmartFilter NewFilter(iNumber, SUPEvent::SUPEventFilters::k_FilterFloat);



			if (type == VMValue::kType_Int)
			{
				std::get<vector<float>>(NewFilter.List).push_back(mValue.data.u);
				//_DMESSAGE("PUSING INT>>%d", mValue.data.u);
			}
			else if (type == VMValue::kType_Float)
			{
				std::get<vector<float>>(NewFilter.List).push_back(mValue.data.f);
				//_DMESSAGE("PUSING float>>%f", mValue.data.f);
			}
			else if (type == VMValue::kType_Bool)
			{
				std::get<vector<float>>(NewFilter.List).push_back(mValue.data.b);
				//_DMESSAGE("PUSING BOOL>>%d", mValue.data.b);
			}
			else if (type == VMValue::kType_VariableArray)
			{
				VMArray<VMVariable> varr;

				if (var.Get(&varr)) {

					for (UInt32 i = 0; i < varr.Length(); i++) {

						VMVariable CurrentVar;
						varr.Get(&CurrentVar, i);

						auto mCurrentValue = CurrentVar.GetValue();
						UInt8 type = mCurrentValue.GetTypeEnum();

						if (type == VMValue::kType_Int)
						{
							std::get<vector<float>>(NewFilter.List).push_back(mCurrentValue.data.u);
							//_DMESSAGE("PUSING INT 2>>%d", mCurrentValue.data.u);
						}
						else if (type == VMValue::kType_Float)
						{
							std::get<vector<float>>(NewFilter.List).push_back(mCurrentValue.data.f);
							//_DMESSAGE("PUSING float 2>>%f", mCurrentValue.data.f);
						}
						else if (type == VMValue::kType_Bool)
						{
							std::get<vector<float>>(NewFilter.List).push_back(mCurrentValue.data.b);
							//_DMESSAGE("PUSING BOOL 2>>%d", mCurrentValue.data.b);
						}

					}


				}

			}


			if (std::get<vector<float>>(NewFilter.List).size())
			{
				result.v_Filters.push_back(NewFilter);
			}

		}
		else if (iAcceptedType == SUPEvent::SUPEventFilters::k_FilterFormID)
		{

			if (!mValue.IsComplexType())
			{
				_DMESSAGE("Current value is not complex type.");
				return;
			}

			SUPEvent::SUPEventFilters::SUPSmartFilter NewFilter(iNumber, SUPEvent::SUPEventFilters::k_FilterFormID);

			GetFormIDFromVMValue(std::get<vector<UInt32>>(NewFilter.List), mValue, var, iFilterInvalidNum);

			if (std::get<vector<UInt32>>(NewFilter.List).size())
			{
				result.v_Filters.push_back(NewFilter);
			}

		}

		return;



		//if (mValue.IsComplexType()) {

		//	auto ComplexType = mValue.GetComplexType();

		//	if (ComplexType)
		//	{
		//		_DMESSAGE("ComplexType type>>%d", ComplexType->GetType());
		//		_DMESSAGE("ComplexType name>>%s", ComplexType->m_typeName.c_str());
		//		_DMESSAGE("ComplexType handle>>%x", mValue.data.id->GetHandle());

		//		if ((*g_objectHandlePolicy)->IsType(kFormType_ACHR, mValue.data.id->GetHandle()))
		//		{
		//			_DMESSAGE(" CURRENT VAR IS ACTOR!!!!");
		//		}

		//		if ((*g_objectHandlePolicy)->IsType(0, mValue.data.id->GetHandle()))
		//		{
		//			_DMESSAGE(" CURRENT VAR IS FORM!!!!");
		//		}

		//		if ((*g_objectHandlePolicy)->IsType(kFormType_CELL, mValue.data.id->GetHandle()))
		//		{
		//			_DMESSAGE(" CURRENT VAR IS kFormType_CELL!!!!");
		//		}

		//	}
		//}
		//else {
		//	_DMESSAGE("ValueType>>>%d", mValue.GetTypeEnum());

		//	if (mValue.GetTypeEnum() == VMValue::kType_String)
		//	{
		//		BSFixedString MyString;
		//		if (var.Get(&MyString))
		//		{
		//			_DMESSAGE("String>>%s", MyString.c_str());
		//		}
		//		else {
		//			_DMESSAGE("Can't get string");
		//		}

		//	}
		//}


	}









	SUPEvent::SUPEventFilters GetFiltersFromFiltersArray(VMArray<VMVariable>& varr, SUPEventHandler* EventHandler, int& iFilterInvalidNum)
	{

		//_DMESSAGE("SMART FILTERS START, var length>>%d", varr.Length());

		SUPEvent::SUPEventFilters result;


		for (UInt32 i = 0; i < varr.Length(); i++)
		{
			VMVariable var;
			varr.Get(&var, i);
			//_DMESSAGE("SMART Processing VAR>>%d", i);

			if (i >= EventHandler->vAllowedFilters.size())
			{
				_DMESSAGE("count more than allowed filters cound >> breaking loop");
				break;
			}

			ProcessVarArrayElementAlt(result, var, EventHandler, i, iFilterInvalidNum);
		}



		if (!result.v_Filters.size())
		{
			_DMESSAGE("Filters not added");
		}


		return result;
	}




}


