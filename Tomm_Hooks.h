
#include "MinHook.h"
#include "F4SE_Compat.h"

// Note: For F4SE 0.7.7 compatibility with Address Library, hook addresses are resolved at runtime.
// The fn_* macros now point to runtime-resolved addresses from SUP_AddressLib.h
// Helper macro to cast runtime addresses to function pointers for hooks
#define HOOK_ADDR(fn_addr) reinterpret_cast<LPVOID>(fn_addr)

//.text:00000001403C10F0; static void __fastcall TESObjectCELL::UpdateSeenDataForPlayerLoc(const struct NiPoint3*, struct TESObjectCELL*)
typedef void(__fastcall* OnUpdateSeenDataForPlayerLoc)(const NiPoint3*, TESObjectCELL*);
// Address resolved at runtime via Address Library - see f_InitHooks() for usage
OnUpdateSeenDataForPlayerLoc OnUpdateSeenDataForPlayerLoc_Original = NULL;

void __fastcall OnUpdateSeenDataForPlayerLoc_Hook(const NiPoint3* MyPoint, TESObjectCELL* MyCell)
{

	if (OnFirstTimeVisitEventHandler.HasEvents() && MyCell && f_BB_IsCellInterior(MyCell) && !f_IsCellVisited(MyCell))
	{
		OnFirstTimeVisitEventHandler.EnterLock();
		VMArray<VMVariable> arguments;
		VMValue args;
		VMVariable var1;
		var1.Set<TESObjectCELL*>(&MyCell);
		arguments.Push(&var1);
		arguments.PackArray(&args, g_MyVirtMachine);

		vector<SUPEvent::SUPEventFilters::SUPSmartFilter> v_PassedFilters;
		SUPEvent::SUPEventFilters::SUPSmartFilter NewFilter(SUPEvent::SUPEventFilters::k_FilterFirst, SUPEvent::SUPEventFilters::k_FilterFormID);
		std::get<vector<UInt32>>(NewFilter.List).push_back(MyCell->formID);
		v_PassedFilters.push_back(NewFilter);

		OnFirstTimeVisitEventHandler.RunEventsAlt(&args, v_PassedFilters);
		OnFirstTimeVisitEventHandler.LeaveLock();

	}

	OnUpdateSeenDataForPlayerLoc_Original(MyPoint, MyCell);
}


//.text:0000000140CC3450 ; void __fastcall __noreturn RadioManager::Update(RadioManager *__hidden this)
typedef void(__fastcall* OnRadioUpdate)(void* RadioManager);
// Address resolved at runtime via Address Library
OnRadioUpdate OnRadioUpdateSUP_Original = NULL;



void OnRadioUpdate_MyHook(void* RManager)
{

	GameLoopTaskFunc();
	OnRadioUpdateSUP_Original(RManager);
}


//; void __fastcall PipboyRadioData::AddStation(PipboyArray **this, float)
typedef void(__fastcall* OnRadioAddStation)(void*,float Frequency);
// Address resolved at runtime via Address Library
OnRadioAddStation OnRadioAddStation_Original = NULL;
void OnRadioAddStation_MyHook(void** me, float Frequency)
{
	//Console_Print("RadioAdd hook works, frequ >>%f size is>>%d", Frequency, vCurrentRadioFrequencies.size());
	vCurrentRadioFrequencies.push_back(Frequency);
	OnRadioAddStation_Original(me, Frequency);
}



void SetHookSUP(LPVOID pTarget, LPVOID pDetour, LPVOID* ppOriginal, std::string HookName) // x can resolve to type T, and y can resolve to type U
{
	// Safety guard: skip hooks for unresolved Address Library addresses
	if (pTarget == nullptr || pTarget == reinterpret_cast<LPVOID>(0))
	{
		_MESSAGE("SKIPPING hook %s - address not resolved (Address Library ID not found)", HookName.c_str());
		return;
	}

	auto HookStatus = MH_CreateHook(pTarget, pDetour, ppOriginal);

	if (HookStatus != MH_OK)
	{
		_MESSAGE("Cannot hook %s >>>%d", HookName.c_str(), HookStatus);
		Console_Print("Cannot hook %s >>>%d", HookName.c_str(), HookStatus);
		return;
	}

	HookStatus = MH_EnableHook(pTarget);

	if (HookStatus != MH_OK)
	{
		_MESSAGE("Cannot enable %s Hook >>>%d", HookName.c_str(), HookStatus);
		Console_Print("Cannot enable %s Hook >>>%d", HookName.c_str(), HookStatus);
		return;
	}

	_MESSAGE("%s hook enabled", HookName.c_str());

}







typedef void(__fastcall* OnAddGridConnection)(Workshop::ExtraData* WorkshopExtraData, TESObjectREFR* akRef1, TESObjectREFR* akRef2, TESObjectREFR* akWireRef);
// Address resolved at runtime via Address Library
OnAddGridConnection OnAddGridConnection_Original = NULL;


typedef void(__fastcall* OnRemoveGridConnection)(Workshop::ExtraData* WorkshopExtraData, TESObjectREFR* akRef1, TESObjectREFR* akRef2, TESObjectREFR* akWireRef, bool bUnk);
// Address resolved at runtime via Address Library
OnRemoveGridConnection OnRemoveGridConnection_Original = NULL;





void OnAddGridConnection_MyHook(Workshop::ExtraData* WorkshopExtraData, TESObjectREFR* akRef1, TESObjectREFR* akRef2, TESObjectREFR* akWireRef)
{
	_DMESSAGE("OnAddGridConnection>>>akRef1>>%x,akRef2>>%x", akRef1->formID, akRef2->formID);

	bool bHasEvents = OnWorkshopPowerStateChangeEventHandler.HasEvents();


	if (bHasEvents )
	{

		WorkshopStatus WorkshopStatusBefore = GetWorkShopStatus(WorkshopExtraData, bHasEvents, false);

		OnAddGridConnection_Original(WorkshopExtraData, akRef1, akRef2, akWireRef);


		PowerGridChanges resultChanges = HasWorkshopChanged(WorkshopStatusBefore, WorkshopExtraData);

		if (resultChanges.bChanged)
		{
			OnWorkshopPowerStateChangeEventHandler.ProcessEventOnWorkshopPowerStateChange(akRef1,NULL, resultChanges, 1,akRef1,akRef2,akWireRef);
		}



	}
	else {
		OnAddGridConnection_Original(WorkshopExtraData, akRef1, akRef2, akWireRef);
	}


		OnPowerConnectionEventHandler.ProcessEventOnPowerConnection(true, WorkshopExtraData, akRef1, akRef2, akWireRef);
	
}



void OnRemoveGridConnection_MyHook(Workshop::ExtraData* WorkshopExtraData, TESObjectREFR* akRef1, TESObjectREFR* akRef2, TESObjectREFR* akWireRef, bool bUnk)
{
	_DMESSAGE("OnRemoveGridConnection>>>akRef1>>%x,akRef2>>%x", akRef1->formID, akRef2->formID);

	OnPowerConnectionEventHandler.ProcessEventOnPowerConnection(false, WorkshopExtraData, akRef1, akRef2, akWireRef);

	bool bHasEvents = OnWorkshopPowerStateChangeEventHandler.HasEvents();
	if (bHasEvents || bWirelessPowerFix)
	{
		WorkshopStatus WorkshopStatusBefore =  GetWorkShopStatus(WorkshopExtraData, bHasEvents, bWirelessPowerFix);
		OnRemoveGridConnection_Original(WorkshopExtraData, akRef1, akRef2, akWireRef, bUnk);


		PowerGridChanges resultChanges = HasWorkshopChanged(WorkshopStatusBefore, WorkshopExtraData);

		if (resultChanges.bChanged)
		{
			OnWorkshopPowerStateChangeEventHandler.ProcessEventOnWorkshopPowerStateChange(akRef1, NULL, resultChanges, 2,akRef1,akRef2,akWireRef);
		}

		int iCheckForSelf = 0;
		if (bWirelessPowerFix)
		{
			_DMESSAGE("Launching WirelessFixProcessChanges from RemoveGridConnection");
			WirelessFixProcessChanges(WorkshopStatusBefore, WorkshopExtraData, NULL, -1, iCheckForSelf, true);
		}

		//int pad = 0;

		//if (bWirelessPowerFix)
		//{
		//	//WirelessFixProcessChanges(WorkshopStatusBefore, WorkshopExtraData, NULL, 0, pad,true);
		//}


	}
	else {
		OnRemoveGridConnection_Original(WorkshopExtraData, akRef1, akRef2, akWireRef, bUnk);
	}


	


}



typedef void(__fastcall* OnNoteAdded)(void*, TESForm* CurrentNote);
// Hardcoded offset - needs Address Library ID lookup for NG compatibility
constexpr uintptr_t addr_OnNoteAdded = 0xEB2D20;
OnNoteAdded OnNoteAdded_Original = NULL;

void OnNoteAdded_MyHook(void** me, TESForm* CurrentNote)
{
	Console_Print("NoteAdded");
	OnNoteAdded_Original(me,CurrentNote);
}


typedef void(__fastcall* OnQuestActive)(TESQuest* CurrentQuest, bool bActive);
// Address resolved at runtime via Address Library
OnQuestActive OnQuestActive_Original = NULL;

void OnQuestActive_MyHook(TESQuest* CurrentQuest, bool bActive)
{
	OnQuestActive_Original(CurrentQuest, bActive);
	OnQuestActiveEventHandler.ProcessEventOnQuestActive(bActive, CurrentQuest);
}

typedef void(__fastcall* OnQuestCompleted)(TESQuest* CurrentQuest, bool bUnk);
// Address resolved at runtime via Address Library
OnQuestCompleted OnQuestCompleted_Original = NULL;

void OnQuestCompleted_MyHook(TESQuest* CurrentQuest, bool bUnk)
{
	OnQuestCompleted_Original(CurrentQuest, bUnk);
	OnQuestFinishedEventHandler.ProcessEventOnQuestFinished(true, CurrentQuest, bUnk);
}


typedef void(__fastcall* OnQuestFailed)(TESQuest* CurrentQuest, bool bUnk);
// Address resolved at runtime via Address Library
OnQuestFailed OnQuestFailed_Original = NULL;

void OnQuestFailed_MyHook(TESQuest* CurrentQuest, bool bUnk)
{
	OnQuestFailed_Original(CurrentQuest, bUnk);
	OnQuestFinishedEventHandler.ProcessEventOnQuestFinished(false, CurrentQuest, bUnk);
}






typedef void(__fastcall* OnProcessResourceRecalculations)(void*);
// Hardcoded offset - needs Address Library ID lookup for NG compatibility
constexpr uintptr_t addr_OnProcessResourceRecalculations = 0x20E390;
OnProcessResourceRecalculations OnProcessResourceRecalculations_Original = NULL;


void OnProcessResourceRecalculations_MyHook(void** me)
{
	Console_Print("OnProcessResourceRecalculations");
	//_DMESSAGE("OnProcessResourceRecalculations");
	OnProcessResourceRecalculations_Original(me);
}



typedef void(__fastcall* OnUpdatePowerOnItems)(void*, BSTSet<TESObjectREFR*> Set1, BSTSet<TESObjectREFR*> Set2, TESObjectREFR* UnkRef);
// Hardcoded offset - needs Address Library ID lookup for NG compatibility
constexpr uintptr_t addr_OnUpdatePowerOnItems = 0x2019E0;
OnUpdatePowerOnItems OnUpdatePowerOnItems_Original = NULL;

void OnUpdatePowerOnItems_MyHook(void** me, BSTSet<TESObjectREFR*> Set1, BSTSet<TESObjectREFR*> Set2, TESObjectREFR* UnkRef)
{
	Console_Print("OnUpdatePowerOnItems");

	OnUpdatePowerOnItems_Original(me, Set1, Set2, UnkRef);
}




typedef void(*_OnUpdateReferencePower)(TESObjectREFR* akRef, bool bUnk);
// Hardcoded offset - needs Address Library ID lookup for NG compatibility
constexpr uintptr_t addr_OnUpdateReferencePower = 0x1F6170;
_OnUpdateReferencePower OnUpdateReferencePower_Original = NULL;


void OnUpdateReferencePower_MyHook( TESObjectREFR* akRef, bool bUnk)
{


	if (akRef)
	{
		Console_Print("OnUpdateReferencePower,Ref>>%x", akRef->formID);
	}
	else {
		Console_Print("OnUpdateReferencePower,Ref is not valid");
	}


	OnUpdateReferencePower_Original(akRef, bUnk);
}




typedef void(__fastcall* OnCalculateCapacityAndLoad)(void*);
// Address resolved at runtime via Address Library
OnCalculateCapacityAndLoad OnCalculateCapacityAndLoad_Original = NULL;

void OnCalculateCapacityAndLoad_MyHook(void** me)
{
	Console_Print("OnCalculateCapacityAndLoad works");

	OnCalculateCapacityAndLoad_Original(me);
}



typedef void(__fastcall* OnQuestEnabled)(void*, bool bUnk);
// Hardcoded offset - needs Address Library ID lookup for NG compatibility
constexpr uintptr_t addr_OnQuestEnabled = 0x5D59C0;
OnQuestEnabled OnQuestEnabled_Original = NULL;

void OnQuestEnabled_MyHook(void** me, bool bUnk)
{
	Console_Print("QuestEnabled");
	_DMESSAGE("QuestAEnabled");
	OnQuestEnabled_Original(me, bUnk);
}


typedef void(__fastcall* OnRagdollActor)(void*, Actor* ActorRef);
// Hardcoded offset - needs Address Library ID lookup for NG compatibility
constexpr uintptr_t addr_OnRagdollActor = 0xC8A50;
OnRagdollActor OnRagdollActor_Original = NULL;

void OnRagdollActor_MyHook(void** me, Actor* ActorRef)
{
	Console_Print("RagdollActor");
	_DMESSAGE("RagdollActor");
	OnRagdollActor_Original(me, ActorRef);
}


typedef int(__fastcall* OnActorJump)(Actor* ActorRef,float fUnk);
// Hardcoded offset - needs Address Library ID lookup for NG compatibility
constexpr uintptr_t addr_OnActorJump = 0xD73770;
OnActorJump OnActorJump_Original = NULL;

int64_t OnActorJump_MyHook(Actor* ActorRef, float fUnk)
{
	Console_Print("OnActorJump>>%f", fUnk);

	if (ActorRef)
	{
		if (!ActorIsInAir(ActorRef))
		{
			//Console_Print("ActorID>>%x", ActorRef->formID);
		}


	}

	int64_t iResult = OnActorJump_Original(ActorRef, fUnk);

	Console_Print("Jump result>>%d", iResult);
	return iResult;
}


//typedef bool(__fastcall* OnActorSwimming)(Actor* ActorRef, bool bUnk);
//RelocAddr <OnActorSwimming> OnActorSwimming_Hook(0xDB80D0);
//OnActorSwimming OnActorSwimming_Original = NULL;
//
//
//bool OnActorSwimming_MyHook(Actor* ActorRef, bool bUnk)
//{
//	Console_Print("OnActorSwimming>>%d", bUnk);
//
//	if (ActorRef)
//	{
//		Console_Print("ActorID>>%x", ActorRef->formID);
//	}
//
//	return OnActorSwimming_Original(ActorRef, bUnk);
//}



typedef int(__fastcall* OnActorFlee)(Actor* ActorRef);
// Hardcoded offset - needs Address Library ID lookup for NG compatibility
constexpr uintptr_t addr_OnActorFlee = 0xE2AA30;
OnActorFlee OnActorFlee_Original = NULL;

int OnActorFlee_MyHook(Actor* ActorRef)
{
	Console_Print("OnActorFlee");

	if (ActorRef)
	{
		Console_Print("ActorID>>%x", ActorRef->formID);
	}

	return OnActorFlee_Original(ActorRef);
}












void OnConsoleCommand_MyHook(char* Text)
{

	if (!SUPConsoleCommands::ProcessCommandAndTellIfContinue(Text))
	{
		return;
	}
	







	OnConsoleCommand_Original(Text);
}


class Script : public TESForm
{
public:
	struct ScriptInfo
	{
		UInt32	unk0;		// 00 (18)
		UInt32	numRefs;	// 04 (1C)
		UInt32	dataLength;	// 08 (20)
		UInt32	varCount;	// 0C (24)
		UInt16	type;		// 10 (28)
		bool	compiled;	// 12 (2A)
		UInt8   unk13;      // 13 (2B)
	};
	ScriptInfo		info;
	char* text;
};



typedef bool(__fastcall* OnCompileScript)(void* me, void* a1, void* a2, UInt32 a3, __int64 a4);
// Address resolved at runtime via Address Library
OnCompileScript OnCompileScript_Original = NULL;







bool OnCompileScript_MyHook(Script* me, void *a1,void* a2, UInt32 a3, __int64 a4)
{
	_DMESSAGE("OnCompileScript TEST");
	//Console_Print("OnCompileScriptTEST");

	bool bResult = OnCompileScript_Original(me,a1,a2,a3,a4);
	//Console_Print("RESULT>>%d", bResult);


	OnConsoleCommandEventHandler.EnterLock();


	if (!SUPConsoleCommands::bProcessConsoleCommand)
	{
		_DMESSAGE("Script is not from console");
		OnConsoleCommandEventHandler.LeaveLock();
		return bResult;
	}

	SUPConsoleCommands::bProcessConsoleCommand = false;

	if (!bResult)
	{
		_DMESSAGE("Script not compiled");
		OnConsoleCommandEventHandler.LeaveLock();
		return bResult;
	}


	if (!me->text)
	{
		_DMESSAGE("No text in script");
		OnConsoleCommandEventHandler.LeaveLock();
		return bResult;
	}



	//Console_Print("Text>>%s", me->text);
	string sText = me->text;


	if (SUPConsoleCommands::sCurrentConsoleText != sText)
	{
		_DMESSAGE("Text is different");
		OnConsoleCommandEventHandler.LeaveLock();
		return bResult;
	}

	SUPConsoleCommands::DecodedConsoleCommand result = SUPConsoleCommands::GetConsoleCommandByText(sText);

	if (!result.bSuccess) {
		OnConsoleCommandEventHandler.LeaveLock();
		return bResult;
	}


	VMArray<VMVariable> arguments;
	VMValue args;



	BSFixedString BSCallingRef(result.sCallingRef.c_str());
	VMVariable var0;
	var0.Set<BSFixedString>(&BSCallingRef);
	arguments.Push(&var0);


	VMVariable var1;
	var1.Set<TESObjectREFR*>(&result.CallingRef);
	arguments.Push(&var1);


	BSFixedString BSFunction(result.sFunction.c_str());
	VMVariable var2;
	var2.Set<BSFixedString>(&BSFunction);
	arguments.Push(&var2);

	VMArray<BSFixedString> Params;


	for (auto it = result.sParams.begin(); it != result.sParams.end(); it++)
	{
		Params.Push(&BSFixedString(Iter.c_str()));
	}

	VMVariable var3;
	var3.Set<VMArray<BSFixedString>>(&Params);
	arguments.Push(&var3);

	arguments.PackArray(&args, g_MyVirtMachine);

	vector<SUPEvent::SUPEventFilters::SUPSmartFilter> v_PassedFilters;

	SUPEvent::SUPEventFilters::SUPSmartFilter NewFilter1(SUPEvent::SUPEventFilters::k_FilterFirst, SUPEvent::SUPEventFilters::k_FilterFormID);

	if (result.CallingRef)
	{
		std::get<vector<UInt32>>(NewFilter1.List).push_back(result.CallingRef->formID);
	}
	else {
		std::get<vector<UInt32>>(NewFilter1.List).push_back(NULL);
	}


	SUPEvent::SUPEventFilters::SUPSmartFilter NewFilter2(SUPEvent::SUPEventFilters::k_FilterSecond, SUPEvent::SUPEventFilters::k_FilterString);
	std::get<vector<string>>(NewFilter2.List).push_back(result.sFunction.c_str());

	v_PassedFilters.push_back(NewFilter1);
	v_PassedFilters.push_back(NewFilter2);
	OnConsoleCommandEventHandler.RunEventsAlt(&args, v_PassedFilters);



	OnConsoleCommandEventHandler.LeaveLock();
	return bResult;
}








typedef __int64(__fastcall* OnSetPlayerMapMarker)(PlayerCharacter* PCChar, const NiPoint3* a2, TESForm* a3);
// Address resolved at runtime via Address Library
OnSetPlayerMapMarker OnSetPlayerMapMarker_Original = NULL;

__int64 OnSetPlayerMapMarker_MyHook(PlayerCharacter* PCChar, const NiPoint3* a2, TESForm* a3)
{
	__int64 result = OnSetPlayerMapMarker_Original(PCChar, a2, a3);

	//Console_Print("Event marker pos X>>%f,Y>>%f,Z>>%f", a2->x, a2->y, a2->z);

	OnPlayerMapMarkerStateChangeEventHandler.ProcessEventOnPlayerMapMarkerStateChangeEvent(true, (*a2),(TESWorldSpace*)a3);
	return result;
}


typedef __int64(__fastcall* OnRemovePlayerMapMarker)(PlayerCharacter* PCChar);
// Address resolved at runtime via Address Library
OnRemovePlayerMapMarker OnRemovePlayerMapMarker_Original = NULL;


__int64 OnRemovePlayerMapMarker_MyHook(PlayerCharacter* PCChar)
{

	OnPlayerMapMarkerStateChangeEventHandler.ProcessEventOnPlayerMapMarkerStateChangeEvent(false, NiPoint3(), NULL);

	return OnRemovePlayerMapMarker_Original(PCChar);
}



typedef bool(__fastcall* OnKnockExplosion)(void* me, Actor* ActorRef, const NiPoint3* PosInfo, float force);
// Address resolved at runtime via Address Library
OnKnockExplosion OnKnockExplosion_Original = NULL;


bool OnKnockExplosion_MyHook(void* me, Actor* ActorRef, const NiPoint3* PosInfo, float force)
{
	//Console_Print("OnKnockExplosion works");

	//if (ActorRef)
	//{
	//	Console_Print("ActorRef>>>%x, X::%f Y::%f Z::%f Force::%f", ActorRef->formID,ActorRef->pos.x, ActorRef->pos.y, ActorRef->pos.z, force);
	//}

	if (ActorCanBeKnockedDown(ActorRef))
	{
		OnActorRagdollStartEventHandler.ProcessEventOnRagdollStart(ActorRef, (NiPoint3*)PosInfo, force);
	}
	
	return OnKnockExplosion_Original(me, ActorRef, PosInfo, force);
}





typedef void(__fastcall* OnPlayerRadioState)(RadioManager* PCChar, UInt8 bEnable);
// Address resolved at runtime via Address Library
OnPlayerRadioState OnPlayerRadioState_Original = NULL;

void OnPlayerRadioState_MyHook(RadioManager* me, UInt8 bOnGamemode)
{

	bool bPreviousState = me->IsPlayerRadioEnabled();


	 OnPlayerRadioState_Original(me, bOnGamemode);

	 if (!bOnGamemode) // 0 is passed on save load, always 1 is for gamemode.
	 {
		 return;
	 }

	 bool bNewState = me->IsPlayerRadioEnabled();

	 if (bPreviousState != bNewState)
	 {
		 OnPlayerRadioStateEventHandler.ProcessEventUniversalOneBool(bNewState);
	 }

	 
}


//typedef __int64(__fastcall* OnTunePlayerRadio)(float fFrequency);
//RelocAddr <OnTunePlayerRadio> OnTunePlayerRadio_Hook(0xCC19D0);
//OnTunePlayerRadio OnTunePlayerRadio_Original = NULL;
//
//__int64 OnTunePlayerRadio_MyHook( float fFrequency)
//{
//
//	Console_Print("OnTunePlayerRadio, frequency>>>%f", fFrequency);
//	__int64 result = OnTunePlayerRadio_Original(fFrequency);
//	//Console_Print("result>>%d", result);
//	return result;
//}
//









typedef void(__fastcall* OnGetUpFromKnock)(void* me, Actor* ActorRef); // void __fastcall TaskQueueInterface::QueueGetUpFromKnock(TaskQueueInterface *this, struct Actor *a2)
// Address resolved at runtime via Address Library
OnGetUpFromKnock OnGetUpFromKnock_Original = NULL;


void OnGetUpFromKnock_MyHook(void* me, Actor* ActorRef)
{
	OnGetUpFromKnock_Original(me, ActorRef);
	OnActorRagdollFinishEventHandler.ProcessEventOnActorRagdollFinish(ActorRef);
}











typedef void(__fastcall* OnProcessWorkshopSwitchEvent)(void* me, TESObjectREFR* Object1, TESObjectREFR* Object2, bool bUnk);
// Hardcoded offset - needs Address Library ID lookup for NG compatibility
constexpr uintptr_t addr_OnProcessWorkshopSwitchEvent = 0xD5F310;
OnProcessWorkshopSwitchEvent OnProcessWorkshopSwitchEvent_Original = NULL;




typedef void(__fastcall* GatherPotentialConnectionsAroundRadiator)(TESObjectREFR* Radiator, BSTSet<TESObjectREFR*>& ArToFil);
// Hardcoded offset - needs Address Library ID lookup for NG compatibility
constexpr uintptr_t addr_GatherPotentialConnectionsAroundRadiator = 0x201610;

typedef void(__fastcall* OnConnectToRadiator)(TESObjectREFR* RefA, TESObjectREFR* RefB, Workshop::ExtraData* WorkshopData);
// Hardcoded offset - needs Address Library ID lookup for NG compatibility
constexpr uintptr_t addr_OnConnectToRadiator = 0x203c80;
OnConnectToRadiator OnConnectToRadiator_Original = NULL;


//typedef void*(__fastcall* AllocateBSTSET)(UInt32 Cou
// nt);
//RelocAddr <AllocateBSTSET> AllocateBSTSET_func(0x1B138A0);
//
//
//class ScrapHeapTOMM {
//public:
//	DEFINE_MEMBER_FN_2(Allocate, void*, 0x1B13F70, void**, UInt64);
//
//
//};





//template <>
//struct BSHash::CRC32Hash<TESObjectREFR*>
//{
//public:
//	UInt32 operator()(const TESObjectREFR* a_key) const
//	{
//		UInt32 crc32;
//		CalculateCRC32_32(&crc32, (UInt32)a_key, 0);
//		return crc32;
//	}
//};






typedef UInt64(__fastcall* GetBSTObjectArenaScrapAllocBase)();
RelocAddr <GetBSTObjectArenaScrapAllocBase> GetBSTObjectArenaScrapAllocBase_func(0x1B13830);







void OutputPoweredItems(Workshop::ExtraData* WorkshopData)
{
	Console_Print("Outputting powered items starts");



	for (UInt64 i = 0; i < WorkshopData->powerGrid.count; i++)
	{

		PowerUtils::PowerGrid* pGrid;
		WorkshopData->powerGrid.GetNthItem(i, pGrid);

		if (!pGrid)
		{
			continue;
		}

		if (pGrid)
		{
			Console_Print("new powerGrid>>%d, powered items count>>>%d", i, pGrid->currentlyPowered.count);

			for (UInt64 i = 0; i < pGrid->currentlyPowered.count; i++)
			{
				UInt32 CurrentlyPoweredID;

				if (pGrid->currentlyPowered.GetNthItem(i, CurrentlyPoweredID))
				{
					Console_Print("Currently powered>>>%x", CurrentlyPoweredID);
				}
			}
		}
	}
	Console_Print("Outputting powered items ends");
}








void OnProcessWorkshopSwitch_MyHook(void* me, TESObjectREFR* WorkshopRef, TESObjectREFR* Switch, bool bSwitchOff)
{

	//RadiatorFixProcessPowerOnOffEvent(bSwitchOff, Switch, WorkshopRef);
	//OnProcessWorkshopSwitchEvent_Original(me, WorkshopRef, Switch, bSwitchOff);
	//RadiatorFixProcessPowerOnOffEvent(bSwitchOff, Switch, WorkshopRef);
}





typedef void(__fastcall* OnDestroyItem)(void * me, TESObjectREFR** MyRef, bool bHasData, UInt8 iUnk, bool bGamemode);
// Hardcoded offset - needs Address Library ID lookup for NG compatibility
constexpr uintptr_t addr_OnDestroyItem = 0x207B10;
OnDestroyItem OnDestroyItem_Original = NULL;


void OnDestroyItem_MyHook(void* me, TESObjectREFR** Spline, bool bHasData, UInt8 iUnk, bool bGamemode)
{
	Console_Print("DestroyItem works!");


	if ((*Spline))
	{
		Console_Print("ref>>>%x", (*Spline)->formID);
	}
	else {
		Console_Print("Ref not valid");
	}

	Console_Print("bHasData>>%d", bHasData);
	Console_Print("iUnk>>%d", iUnk);
	Console_Print("bGamemode>>%d", bGamemode);

	OnDestroyItem_Original(me, Spline,bHasData,iUnk,bGamemode);
}







typedef void(__fastcall* OnSetWantsDelete)(TESObjectREFR* MyRef, bool bUnk);
// Address resolved at runtime via Address Library
OnSetWantsDelete OnSetWantsDelete_Original = NULL;


int IsItemPresentInWorkshop(Workshop::ExtraData* WorkshopData, UInt32 FormID) { // 0 for NOT Exists, 1 for Object, 2 for wire

	for (UInt64 i = 0; i < WorkshopData->powerGrid.count; i++)
	{
		PowerUtils::PowerGrid* pGrid;
		WorkshopData->powerGrid.GetNthItem(i, pGrid);

		if (pGrid)
		{
			for (auto it = pGrid->adjacencyMap.begin(); it != pGrid->adjacencyMap.end(); it++)
			{
				if (Iter.first == FormID)
				{
					return 1;
				}

				for (auto it2 = Iter.second->begin(); it2 != Iter.second->end(); it2++)
				{
					if (Iter2.connection == FormID)
					{
						return 1;
					}

					if (Iter2.connector == FormID)
					{
						return 2;
					}

				}

			}

		}
	}

	return 0;
}





vector<WireConnection> GetConnectionsForWire(Workshop::ExtraData* WorkshopData, UInt32 FormID) {

	vector<WireConnection> result;

	for (UInt64 i = 0; i < WorkshopData->powerGrid.count; i++)
	{
		PowerUtils::PowerGrid* pGrid;
		WorkshopData->powerGrid.GetNthItem(i, pGrid);

		if (pGrid)
		{
			for (auto it = pGrid->adjacencyMap.begin(); it != pGrid->adjacencyMap.end(); it++)
			{

				//Console_Print("Found item, in adj map");
				for (auto it2 = Iter.second->begin(); it2 != Iter.second->end(); it2++)
				{
					//Console_Print("New connection, connector>>%x", Iter2.connection);
					if (Iter2.connector == FormID)
					{
						bool bFound = false;
						for (auto it3 = result.begin(); it3 != result.end(); it3++)
						{
							if (Iter3.Connection1->formID == Iter.first && Iter3.Connection2->formID == Iter2.connection)
							{
								bFound = true;
								break;
							}
							else if (Iter3.Connection1->formID == Iter2.connection && Iter3.Connection2->formID == Iter.first)
							{
								bFound = true;
								break;
							}

						}


						TESObjectREFR* pConnection1 = (TESObjectREFR*)LookupFormByID(Iter.first);

						if (!pConnection1)
						{
							continue;
						}

						TESObjectREFR* pConnection2 = (TESObjectREFR*)LookupFormByID(Iter2.connection);

						if (!pConnection2)
						{
							continue;
						}


						if (!bFound)
						{
							WireConnection NewWireConnection;
							NewWireConnection.Connection1 = pConnection1;
							NewWireConnection.Connection2 = pConnection2;
							result.push_back(NewWireConnection);
							//Console_Print("Adding new Wire connection, Connection1>>%x,Connection2>>%x", Iter.first, Iter2.connection);
						}

						//TESObjectREFR* CurrentObject = (TESObjectREFR*)LookupFormByID(Iter2.connector);

						//if (CurrentObject) {
						//	result.push_back(CurrentObject);
						//}
					}

				}
				
			}
		}
	}

	return result;
}










vector<TESObjectREFR*> GetWiresForReference(Workshop::ExtraData* WorkshopData, UInt32 FormID) {

	vector<TESObjectREFR*> result;

	for (UInt64 i = 0; i < WorkshopData->powerGrid.count; i++)
	{
		PowerUtils::PowerGrid* pGrid;
		WorkshopData->powerGrid.GetNthItem(i, pGrid);

		if (pGrid)
		{
			for (auto it = pGrid->adjacencyMap.begin(); it != pGrid->adjacencyMap.end(); it++)
			{
				if (Iter.first == FormID)
				{
					//Console_Print("Found item, in adj map");
					for (auto it2 = Iter.second->begin(); it2 != Iter.second->end(); it2++)
					{
						//Console_Print("New connection, connector>>%x", Iter2.connector);
						if (Iter2.connector)
						{
							TESObjectREFR* CurrentObject = (TESObjectREFR*)LookupFormByID(Iter2.connector);

							if (CurrentObject) {
								result.push_back(CurrentObject);
							}
						}

					}
				}
			}
		}
	}

	return result;
}



typedef void(*_WorkshopDeleteWorkshopItem)(TESObjectREFR* akRef);
// Address resolved at runtime via Address Library
// Usage: reinterpret_cast<_WorkshopDeleteWorkshopItem>(fn_WorkShop_DeleteWorkshopItem)(akRef)



typedef void(__fastcall* OnWorkshopDeleteWorkshopItem)(void* me,TESObjectREFR* REF);
// Address resolved at runtime via Address Library
OnWorkshopDeleteWorkshopItem OnWorkshopDeleteWorkshopItem_Original = NULL;


void OnWorkshopDeleteWorkshopItem_MyHook(void* me, TESObjectREFR* REF)
{
	//Console_Print("OnWorkshopDeleteWorkshopItem works>>%x", REF->formID);

	OnWorkshopDeleteWorkshopItem_Original(me, REF);
}








#include "f4se/GameMessages.h"
RelocPtr <BSTCommonScrapHeapMessageQueue<BSPackedTask>*> g_TaskInterface(0x05AC64F0); // From F4SE

void OnSetWantsDelete_MyHook(TESObjectREFR* MyRef, bool bUnk)
{
	//Console_Print("SetWantsDeleteworks!>>>%x", MyRef->formID);

	if (!bScrapCrashFix)
	{
		OnSetWantsDelete_Original(MyRef, bUnk);
		return;
	}



	TESObjectREFR* workshopRef = GetLinkedRef_Native(MyRef, g_SUPWorkshopKeyword);

	if (workshopRef)
	{
		//Console_Print("Item removed by DELETE() found!>>%x", MyRef->formID);
		Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(workshopRef);

		if (WorkshopData)
		{
			//Console_Print("Manually removing item from workshop");
			int iPresent = IsItemPresentInWorkshop(WorkshopData, MyRef->formID);
			//Console_Print("Ref is present in extra data BEFORE>>%d", iPresent);

			if (iPresent)
			{
				if (iPresent == 1) // Object
				{
					auto AllWires = GetWiresForReference(WorkshopData, MyRef->formID);
					WorkshopCompat::RemoveItem(WorkshopData, MyRef);
					for (auto it = AllWires.begin(); it != AllWires.end(); it++)
					{
						//Console_Print("Removing wire>>%x", Iter->formID);
						WorkshopDeleteWorkshopItem(*it);
					}
					//Console_Print("Ref is present in extra data AFTER>>%d", IsItemPresentInWorkshop(WorkshopData, MyRef->formID));
				}
				else { 
					Workshop::ContextData contextData(*g_player);
					BSPointerHandle<TESObjectREFR> currentWorkshop = *Workshop::hCurrentWorkshop;
					*Workshop::hCurrentWorkshop = workshopRef;
					NiPointer<TESObjectREFR> refrPointer(MyRef);
					Workshop::ScrapReference(&contextData, &refrPointer, nullptr);
					*Workshop::hCurrentWorkshop = currentWorkshop;
					
					
					//Wire
					//Console_Print("Ref is wire - remove it");
					//_DMESSAGE("1");

					////WorkshopDestroyItem_func(WorkshopData,&MyRef, 1, 1, 1);
					//_DMESSAGE("2");
					//auto WireConnections = GetConnectionsForWire(WorkshopData, MyRef->formID);

					//for (auto it = WireConnections.begin(); it != WireConnections.end(); it++)
					//{
					//	Console_Print("Removing Wire connection, Connection1>>%x,Connection2>>%x", Iter.Connection1, Iter.Connection2);
					//	

					//	//FakeRef* FakeRefA = new FakeRef;
					//	//FakeRefA->formID = Iter.Connection1;

					//	//FakeRef* FakeRefB = new FakeRef;
					//	//FakeRefB->formID = Iter.Connection2;

					//	//FakeRef* FakeSplineRef = new FakeRef;
					//	//FakeSplineRef->formID = MyRef->formID;
					//	//_DMESSAGE("REMOVE TEST 1");


					//	bool bUpdateConnection1 = false;
					//	bool bUpdateConnection2 = false;
					//	if (!ItemInterruptsPower(Iter.Connection1))
					//	{
					//		Console_Print("Connection 1 NOT interrupts power - update it");
					//		bUpdateConnection1 = true;
					//	}

					//	if (!ItemInterruptsPower(Iter.Connection2))
					//	{
					//		Console_Print("Connection 2  NOT interrupts power - update it");
					//		bUpdateConnection2 = true;
					//	}

					//	WorkshopData->RemoveConnection(Iter.Connection1, Iter.Connection2, MyRef, false);

					//	if (bUpdateConnection1)
					//	{
					//		(*g_TaskInterface)->ProcessSwitchEvent(workshopRef, Iter.Connection1, 0);
					//	}

					//	if (bUpdateConnection2)
					//	{
					//		(*g_TaskInterface)->ProcessSwitchEvent(workshopRef, Iter.Connection2, 0);
					//	}

					//	_DMESSAGE("REMOVE TEST 2");
					//}
					//
					////DisconnectSpline(MyRef);
					//WorkshopCalculatePowerRating(workshopRef);
					//WorkshopRecalculateResources(workshopRef);

					//WorkshopData->RemoveItem(MyRef);
				}
			}

		}
		else {
			//Console_Print("Workshop has no extra data.");
		}

	}

	OnSetWantsDelete_Original(MyRef,bUnk);
}
















void OnConnectToRadiator_MyHook( TESObjectREFR* GeneratorRef, TESObjectREFR* RefB, Workshop::ExtraData* WorkshopData)
{

	Console_Print("OnConnectToRadiator works");
	_DMESSAGE("OnConnectToRadiator works starts");


	if (GeneratorRef)
	{
		Console_Print("GeneratorRef->%x", GeneratorRef->formID);

		_DMESSAGE("GeneratorRef->%x", GeneratorRef->formID);
	}
	else {
		Console_Print("GeneratorRef does not exists");
	}

	if (RefB)
	{
		Console_Print("Receiver-->%x", RefB->formID);
		_DMESSAGE("Receiver-->%x", RefB->formID);
	}
	else {
		Console_Print("Receiver does not exists");
	}



	//auto WirelessConnections = GetConnectionsForRadiator(WorkshopData, GeneratorRef);

	//TESObjectREFR* RefC = NULL;



	OnConnectToRadiator_Original(GeneratorRef, RefB, WorkshopData);


	OutputPowerGridHelper(WorkshopData);
	//for (auto it = WirelessConnections.begin(); it != WirelessConnections.end(); it++)
	//{
	//	_DMESSAGE("Removing wireless connection, Generator>>%x,Receiver>>%x", Iter.Connection1->formID, Iter.Connection2->formID);
	//	WorkshopData->RemoveConnection(Iter.Connection1, Iter.Connection2, RefC, false);
	//	_DMESSAGE("removed");
	//	Console_Print("Removing wireless connection, Generator>>%x,Receiver>>%x", Iter.Connection1->formID, Iter.Connection2->formID);
	//}

	_DMESSAGE("OnConnectToRadiator finishes");
	Console_Print("OnConnectToRadiator finishes");
}









void OnWorkshopHandleReferenceEvent_MyHook(TESObjectREFR* WorkshopRef, TESObjectREFR* SourceRef)
{

	_DMESSAGE("OnWorkshopHandleReferenceEvent starts>> %x %x", WorkshopRef->formID, SourceRef->formID);
	//Console_Print("OnWorkshopHandleReferenceEvent starts");

	bool bHasEvents = OnWorkshopPowerStateChangeEventHandler.HasEvents();


	if (bWirelessPowerFix || bHasEvents)
	{
		Workshop::ExtraData* WorkshopExtraData = f_GetPowerGridExtraData(WorkshopRef);

		if (!WorkshopExtraData)
		{
			_DMESSAGE("OnWorkshopHandleReferenceEvent -workshop has not extra data - calling original function.");
			OnWorkshopHandleReferenceEvent_Original(WorkshopRef, SourceRef);
			return;
		}

		WorkshopStatus WorkshopStatusBefore = GetWorkShopStatus(WorkshopExtraData, bHasEvents, bWirelessPowerFix);

		int iCheckForSelf = 0;

		if (bWirelessPowerFix)
		{
			if (IsSwitch(SourceRef->baseForm))
			{
				UINT64 iTemp = GetOpenState(SourceRef);

				_DMESSAGE("Source ref open state>>%d", iTemp);

				if (iTemp == 1)
				{
					iCheckForSelf = 1; //Switched OFF
				}
				else if (iTemp == 3) {
					iCheckForSelf = 2; //Switched ON
				}
			}
		}

		//Console_Print("ISPOWERED BEFORE %x >>>%d", SourceRef->formID, WorkshopExtraData->IsPowered(SourceRef));


		OnWorkshopHandleReferenceEvent_Original(WorkshopRef, SourceRef);

		//Console_Print("ISPOWERED AFTER %x >>>%d", SourceRef->formID, WorkshopExtraData->IsPowered(SourceRef));

		if (bHasEvents)
		{
			PowerGridChanges resultChanges = HasWorkshopChanged(WorkshopStatusBefore, WorkshopExtraData);
			if (resultChanges.bChanged)
			{
				OnWorkshopPowerStateChangeEventHandler.ProcessEventOnWorkshopPowerStateChange(SourceRef, WorkshopRef, resultChanges, 3,NULL,NULL,NULL);
			}
		}

		if (bWirelessPowerFix)
		{
			WirelessFixProcessChanges(WorkshopStatusBefore, WorkshopExtraData, WorkshopRef, SourceRef->formID, iCheckForSelf, false);

			if (iCheckForSelf == 1)
			{
				if (!WorkshopCompat::IsPowered(WorkshopExtraData, SourceRef))
				{
					_DMESSAGE("Checking event source on Power OFF>>%x", SourceRef->formID);
					RadiatorFixProcessPowerOnOffEvent(false, SourceRef, WorkshopRef, WorkshopExtraData);
				}

			}
			else if (iCheckForSelf == 2) {

				if (WorkshopCompat::IsPowered(WorkshopExtraData, SourceRef))
				{
					_DMESSAGE("Checking event source on Power ON>>%x", SourceRef->formID);
					RadiatorFixProcessPowerOnOffEvent(true, SourceRef, WorkshopRef, WorkshopExtraData);
				}
			}
		}


		_DMESSAGE("OnWorkshopHandleReferenceEvent ends");
	}
	else {
		OnWorkshopHandleReferenceEvent_Original(WorkshopRef, SourceRef);
	}

}


typedef void(*_UpdateMovingWirelessItem1)(TESObjectREFR* akRef, Workshop::ExtraData* workshopExtraData);
RelocAddr <_UpdateMovingWirelessItem1> UpdateMovingWirelessItem1(0x00201B10); // Usually paired with LinkPower
_UpdateMovingWirelessItem1 OnUpdateMovingWirelessItem1_Original = NULL;


void _UpdateMovingWirelessItem1_MyHook(TESObjectREFR* akRef, Workshop::ExtraData* workshopExtraData)
{
	//Console_Print("_UpdateMovingWirelessItem1 works, ref>>%x", akRef->formID);

	//Console_Print("Item power received>>%d", ItemIsPowerReceiver(akRef));
	//Console_Print("Item is powered>>%d", workshopExtraData->IsPowered(akRef));
	//Console_Print("Item ItemInterruptsPower>>%d", ItemInterruptsPower(akRef));
	//Console_Print("Item IsSwitchr>>%d", IsSwitch(akRef->baseForm));
	

	

	//Console_Print("Item power powered>>%d", ispowered(akRef));


	OnUpdateMovingWirelessItem1_Original(akRef, workshopExtraData);
}

struct Dismemberment_Data {
	UInt32 pad[16];
	UInt32 ActorHandler;
	UInt32 pad2[18];
	UInt32 LimbToDismember;

};
STATIC_ASSERT(offsetof(Dismemberment_Data, ActorHandler) == 0x40);
STATIC_ASSERT(offsetof(Dismemberment_Data, LimbToDismember) == 0x8C);


typedef bool(*_GetActorByHandle)(UInt32* ActorHandler,  Actor** ActorRef);
// Address resolved at runtime via Address Library
// Usage: reinterpret_cast<_GetActorByHandle>(fn_Main_GetActorByHandle)(&handle, &actor)

typedef void(*_ApplyDismemberment)(void* me, const Dismemberment_Data& DismemberData);
// Address resolved at runtime via Address Library
_ApplyDismemberment OnApplyDismemberment_Original = NULL;


void OnApplyDismemberment_MyHook(void* me, Dismemberment_Data& DismemberData)
{
	if (OnDismemberLimbEventHandler.HasEvents())
	{
		OnDismemberLimbEventHandler.EnterLock();
		Actor* CurrentActor = NULL;
		GetActorByHandle_Execute(&DismemberData.ActorHandler, &CurrentActor);

		if (CurrentActor)
		{
			OnDismemberLimbEventHandler.ProcessEventOnDismemberLimb(CurrentActor, DismemberData.LimbToDismember);
		}
		OnDismemberLimbEventHandler.LeaveLock();
	}
	OnApplyDismemberment_Original(me, DismemberData);
}




typedef void(*_QueueSaveLoadTask)(void* BSSaveLoadManager, SInt32 request);
// Hardcoded offset - needs Address Library ID lookup for NG compatibility
constexpr uintptr_t addr_QueueSaveLoadTask = 0xCF1220;
_QueueSaveLoadTask _QueueSaveLoadTask_Original = NULL;


void _QueueSaveLoadTask_MyHook(void* BSSaveLoadManager, SInt32 request)
{
	Console_Print("SAVELOAD,REQUEST>>>%d", request);
	_QueueSaveLoadTask_Original(BSSaveLoadManager, request);
}

//bool __fastcall BGSSaveLoadManager::IsSavingAllowed(BGSSaveLoadManager *this)


void f_InitHooks()
{
	auto HookStatus = MH_Initialize();

	if (HookStatus != MH_OK)
	{
		_MESSAGE("Cannot initialize MinHook >>>%d", HookStatus);
		return;
	}

	// Note: All addresses are now resolved at runtime via Address Library (SUP_AddressLib.h)
	// The fn_* macros expand to runtime-resolved addresses

	SetHookSUP(HOOK_ADDR(fn_Main_RadioUpdate), &OnRadioUpdate_MyHook, reinterpret_cast<LPVOID*>(&OnRadioUpdateSUP_Original), "Gameloop");
	SetHookSUP(HOOK_ADDR(fn_Main_UpdateSeenDataForPlayerLoc), &OnUpdateSeenDataForPlayerLoc_Hook, reinterpret_cast<LPVOID*>(&OnUpdateSeenDataForPlayerLoc_Original), "CellSeenData");
	SetHookSUP(HOOK_ADDR(fn_Main_OnRadioAddStation), &OnRadioAddStation_MyHook, reinterpret_cast<LPVOID*>(&OnRadioAddStation_Original), "OnRadioAdd");

	SetHookSUP(HOOK_ADDR(fn_Main_OnQuestActive), &OnQuestActive_MyHook, reinterpret_cast<LPVOID*>(&OnQuestActive_Original), "OnQuestActive");
	SetHookSUP(HOOK_ADDR(fn_Main_OnQuestComplete), &OnQuestCompleted_MyHook, reinterpret_cast<LPVOID*>(&OnQuestCompleted_Original), "OnQuestCompleted");
	SetHookSUP(HOOK_ADDR(fn_Main_OnQuestFailed), &OnQuestFailed_MyHook, reinterpret_cast<LPVOID*>(&OnQuestFailed_Original), "OnQuestFailed");
	SetHookSUP(HOOK_ADDR(fn_Main_OnConsoleCommand), &OnConsoleCommand_MyHook, reinterpret_cast<LPVOID*>(&OnConsoleCommand_Original), "OnConsoleCommand");
	SetHookSUP(HOOK_ADDR(fn_Main_OnCompileScript), &OnCompileScript_MyHook, reinterpret_cast<LPVOID*>(&OnCompileScript_Original), "OnCompileScript");
	SetHookSUP(HOOK_ADDR(fn_Main_OnSePlayerMarker), &OnSetPlayerMapMarker_MyHook, reinterpret_cast<LPVOID*>(&OnSetPlayerMapMarker_Original), "OnSetPlayerMapMarker");
	SetHookSUP(HOOK_ADDR(fn_Main_OnRemovePlayerMapMarker), &OnRemovePlayerMapMarker_MyHook, reinterpret_cast<LPVOID*>(&OnRemovePlayerMapMarker_Original), "OnRemovePlayerMapMarker");
	SetHookSUP(HOOK_ADDR(fn_Main_OnKnockExplosion), &OnKnockExplosion_MyHook, reinterpret_cast<LPVOID*>(&OnKnockExplosion_Original), "OnKnockExplosion");
	SetHookSUP(HOOK_ADDR(fn_Main_OnPlayerRadioState), &OnPlayerRadioState_MyHook, reinterpret_cast<LPVOID*>(&OnPlayerRadioState_Original), "OnPlayerRadioState");
	SetHookSUP(HOOK_ADDR(fn_Main_OnGetUpFromKnock), &OnGetUpFromKnock_MyHook, reinterpret_cast<LPVOID*>(&OnGetUpFromKnock_Original), "OnGetUpFromKnock");
	SetHookSUP(HOOK_ADDR(fn_Main_OnSetWantsDelete), &OnSetWantsDelete_MyHook, reinterpret_cast<LPVOID*>(&OnSetWantsDelete_Original), "OnSetWantsDelete");


	SetHookSUP(HOOK_ADDR(fn_WorkShopExtra_AddGridConnection), &OnAddGridConnection_MyHook, reinterpret_cast<LPVOID*>(&OnAddGridConnection_Original), "OnAddGridConnectionAdd");
	SetHookSUP(HOOK_ADDR(fn_WorkShopExtra_RemoveGridConnection), &OnRemoveGridConnection_MyHook, reinterpret_cast<LPVOID*>(&OnRemoveGridConnection_Original), "OnRemoveGridConnection");
	SetHookSUP(HOOK_ADDR(fn_TaskInterface_ProcessWorkshopSwitchEvent), &OnWorkshopHandleReferenceEvent_MyHook, reinterpret_cast<LPVOID*>(&OnWorkshopHandleReferenceEvent_Original), "OnWorkshopHandleReferenceEvent");

	//SetHookSUP(OnHandleLimbReplacement_Hook, &OnHandleLimbReplacement_MyHook, reinterpret_cast<LPVOID*>(&OnHandleLimbReplacement_Original), "OnHandleLimbReplacement");

	SetHookSUP(HOOK_ADDR(fn_Main_OnApplyDismemberment), &OnApplyDismemberment_MyHook, reinterpret_cast<LPVOID*>(&OnApplyDismemberment_Original), "OnApplyDismemberment");


	//SetHookSUP(_QueueSaveLoadTask_Hook, &_QueueSaveLoadTask_MyHook, reinterpret_cast<LPVOID*>(&_QueueSaveLoadTask_Original), "QueueSaveLoadTask");

	//SetHookSUP(OnConnectToRadiator_Hook, &OnConnectToRadiator_MyHook, reinterpret_cast<LPVOID*>(&OnConnectToRadiator_Original), "OnConnectToRadiator");

	
	
	//SetHookSUP(UpdateMovingWirelessItem1, &_UpdateMovingWirelessItem1_MyHook, reinterpret_cast<LPVOID*>(&OnUpdateMovingWirelessItem1_Original), "UpdateMovingWirelessItem1");


	
	//SetHookSUP(OnProcessWorkshopSwitchEvent_Hook, &OnProcessWorkshopSwitch_MyHook, reinterpret_cast<LPVOID*>(&OnProcessWorkshopSwitchEvent_Original), "OnProcessWorkshopSwitch");



	


	

}








