
#include "minhook/include/MinHook.h"

// Phase 4: the Address Library IDs for these hooks are unverified (fuzzy nearest-match; two
// were already proven to land on the wrong code). Hooking a wrong address corrupts the game,
// so the hooks are disabled until each ID is verified. Flip to 1 to re-enable them.
#define SUP_ENABLE_HOOKS 1


//.text:00000001403C10F0; static void __fastcall TESObjectCELL::UpdateSeenDataForPlayerLoc(const struct NiPoint3*, struct TESObjectCELL*)
typedef void(__fastcall* OnUpdateSeenDataForPlayerLoc)(const NiPoint3*, TESObjectCELL*);
RelocAddr <OnUpdateSeenDataForPlayerLoc> OnUpdateSeenDataForPlayerLocSUP(fn_Main_UpdateSeenDataForPlayerLoc);
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
RelocAddr <OnRadioUpdate> OnRadioUpdateSUP(fn_Main_RadioUpdate);
OnRadioUpdate OnRadioUpdateSUP_Original = NULL;



void OnRadioUpdate_MyHook(void* RManager)
{

	GameLoopTaskFunc();
	OnRadioUpdateSUP_Original(RManager);
}


//; void __fastcall PipboyRadioData::AddStation(PipboyArray **this, float)
typedef void(__fastcall* OnRadioAddStation)(void*,float Frequency);
RelocAddr <OnRadioAddStation> OnRadioAddStation_Hook(fn_Main_OnRadioAddStation);
OnRadioAddStation OnRadioAddStation_Original = NULL;
void OnRadioAddStation_MyHook(void** me, float Frequency)
{
	//Console_Print("RadioAdd hook works, frequ >>%f size is>>%d", Frequency, vCurrentRadioFrequencies.size());
	vCurrentRadioFrequencies.push_back(Frequency);
	OnRadioAddStation_Original(me, Frequency);
}



template <class T>
void SetHookSUP(RelocAddr<T>& pTargetReloc, LPVOID pDetour, LPVOID* ppOriginal, std::string HookName)
{
	const auto pTarget = reinterpret_cast<LPVOID>(pTargetReloc.address());

	// ID = 0 (disabled) or a raw un-migrated offset (< the image base 0x140000000) means we
	// have no valid address — skip instead of patching garbage.
	if (pTarget == nullptr || reinterpret_cast<std::uintptr_t>(pTarget) < 0x140000000ull)
	{
		_MESSAGE("%s hook SKIPPED: address not resolved (0x%llX)", HookName.c_str(), static_cast<unsigned long long>(reinterpret_cast<std::uintptr_t>(pTarget)));
		return;
	}

	// Real MinHook detour (vendored): MH_CreateHook builds a proper trampoline that preserves
	// whole instructions, so the returned ppOriginal is a valid "call the original" pointer.
	auto HookStatus = MH_CreateHook(pTarget, pDetour, ppOriginal);
	if (HookStatus != MH_OK)
	{
		_MESSAGE("Cannot hook %s >>>%d", HookName.c_str(), HookStatus);
		return;
	}

	HookStatus = MH_EnableHook(pTarget);
	if (HookStatus != MH_OK)
	{
		_MESSAGE("Cannot enable %s Hook >>>%d", HookName.c_str(), HookStatus);
		return;
	}

	_MESSAGE("%s hook enabled @ 0x%llX", HookName.c_str(), static_cast<unsigned long long>(reinterpret_cast<std::uintptr_t>(pTarget)));
}typedef void(__fastcall* OnNoteAdded)(void*, TESForm* CurrentNote);
RelocAddr <OnNoteAdded> OnNoteAdded_Hook(0xEB2D20);
OnNoteAdded OnNoteAdded_Original = NULL;

void OnNoteAdded_MyHook(void** me, TESForm* CurrentNote)
{
	Console_Print("NoteAdded");
	OnNoteAdded_Original(me,CurrentNote);
}


typedef void(__fastcall* OnQuestActive)(TESQuest* CurrentQuest, bool bActive);
RelocAddr <OnQuestActive> OnQuestActive_Hook(fn_Main_OnQuestActive);
OnQuestActive OnQuestActive_Original = NULL;

void OnQuestActive_MyHook(TESQuest* CurrentQuest, bool bActive)
{
	OnQuestActive_Original(CurrentQuest, bActive);
	OnQuestActiveEventHandler.ProcessEventOnQuestActive(bActive, CurrentQuest);
}

typedef void(__fastcall* OnQuestCompleted)(TESQuest* CurrentQuest, bool bUnk);
RelocAddr <OnQuestCompleted> OnQuestCompleted_Hook(fn_Main_OnQuestComplete);
OnQuestCompleted OnQuestCompleted_Original = NULL;

void OnQuestCompleted_MyHook(TESQuest* CurrentQuest, bool bUnk)
{
	OnQuestCompleted_Original(CurrentQuest, bUnk);
	OnQuestFinishedEventHandler.ProcessEventOnQuestFinished(true, CurrentQuest, bUnk);
}


typedef void(__fastcall* OnQuestFailed)(TESQuest* CurrentQuest, bool bUnk);
RelocAddr <OnQuestFailed> OnQuestFailed_Hook(fn_Main_OnQuestFailed);
OnQuestFailed OnQuestFailed_Original = NULL;

void OnQuestFailed_MyHook(TESQuest* CurrentQuest, bool bUnk)
{
	OnQuestFailed_Original(CurrentQuest, bUnk);
	OnQuestFinishedEventHandler.ProcessEventOnQuestFinished(false, CurrentQuest, bUnk);
}






typedef void(__fastcall* OnProcessResourceRecalculations)(void*);
RelocAddr <OnProcessResourceRecalculations> OnProcessResourceRecalculations_Hook(0x20E390);
OnProcessResourceRecalculations OnProcessResourceRecalculations_Original = NULL;


void OnProcessResourceRecalculations_MyHook(void** me)
{
	Console_Print("OnProcessResourceRecalculations");
	//_DMESSAGE("OnProcessResourceRecalculations");
	OnProcessResourceRecalculations_Original(me);
}



typedef void(__fastcall* OnUpdatePowerOnItems)(void*, BSTSet<TESObjectREFR*> Set1, BSTSet<TESObjectREFR*> Set2, TESObjectREFR* UnkRef);

RelocAddr <OnUpdatePowerOnItems> OnUpdatePowerOnItems_Hook(0x2019E0);
OnUpdatePowerOnItems OnUpdatePowerOnItems_Original = NULL;

void OnUpdatePowerOnItems_MyHook(void** me, BSTSet<TESObjectREFR*> Set1, BSTSet<TESObjectREFR*> Set2, TESObjectREFR* UnkRef)
{
	Console_Print("OnUpdatePowerOnItems");

	OnUpdatePowerOnItems_Original(me, Set1, Set2, UnkRef);
}




typedef void(*_OnUpdateReferencePower)(TESObjectREFR* akRef, bool bUnk);
RelocAddr<_OnUpdateReferencePower> OnUpdateReferencePower_Hook(0x1F6170);
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




// DISABLED (Phase 4): PowerGrid::CalculateCapacityAndLoad is INLINED in the NG binary
// (F4SE master: "this was inlined everywhere") — there is no function prologue to hook.
// The Papyrus function is served by sup::compat::CalculateCapacityAndLoad (reimplemented
// in C++ over the modeled PowerGrid fields). Kept out of the hook list below on purpose.
//typedef void(__fastcall* OnCalculateCapacityAndLoad)(void*);
//RelocAddr <OnCalculateCapacityAndLoad> OnCalculateCapacityAndLoad_Hook(fn_WorkShopExtra_CalculateCapacityAndLoad);
//OnCalculateCapacityAndLoad OnCalculateCapacityAndLoad_Original = NULL;



typedef void(__fastcall* OnQuestEnabled)(void*, bool bUnk);
RelocAddr <OnQuestEnabled> OnQuestEnabled_Hook(0x5D59C0);
OnQuestEnabled OnQuestEnabled_Original = NULL;

void OnQuestEnabled_MyHook(void** me, bool bUnk)
{
	Console_Print("QuestEnabled");
	_DMESSAGE("QuestAEnabled");
	OnQuestEnabled_Original(me, bUnk);
}


typedef void(__fastcall* OnRagdollActor)(void*, Actor* ActorRef);
RelocAddr <OnRagdollActor> OnRagdollActor_Hook(0xC8A50);
OnRagdollActor OnRagdollActor_Original = NULL;

void OnRagdollActor_MyHook(void** me, Actor* ActorRef)
{
	Console_Print("RagdollActor");
	_DMESSAGE("RagdollActor");
	OnRagdollActor_Original(me, ActorRef);
}


typedef int(__fastcall* OnActorJump)(Actor* ActorRef,float fUnk);
RelocAddr <OnActorJump> OnActorJump_Hook(0xD73770);
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
RelocAddr <OnActorFlee> OnActorFlee_Hook(0xE2AA30);
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
RelocAddr <OnCompileScript> OnCompileScript_Hook(fn_Main_OnCompileScript);
OnCompileScript OnCompileScript_Original = NULL;







bool OnCompileScript_MyHook(::Script* me, void *a1,void* a2, UInt32 a3, __int64 a4)
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
		BSFixedString TempString(Iter.c_str());
		Params.Push(&TempString);
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
RelocAddr <OnSetPlayerMapMarker> OnSetPlayerMapMarker_Hook(fn_Main_OnSePlayerMarker);
OnSetPlayerMapMarker OnSetPlayerMapMarker_Original = NULL;

__int64 OnSetPlayerMapMarker_MyHook(PlayerCharacter* PCChar, const NiPoint3* a2, TESForm* a3)
{
	__int64 result = OnSetPlayerMapMarker_Original(PCChar, a2, a3);

	//Console_Print("Event marker pos X>>%f,Y>>%f,Z>>%f", a2->x, a2->y, a2->z);

	OnPlayerMapMarkerStateChangeEventHandler.ProcessEventOnPlayerMapMarkerStateChangeEvent(true, (*a2),(TESWorldSpace*)a3);
	return result;
}


typedef __int64(__fastcall* OnRemovePlayerMapMarker)(PlayerCharacter* PCChar);
RelocAddr <OnRemovePlayerMapMarker> OnRemovePlayerMapMarker_Hook(fn_Main_OnRemovePlayerMapMarker);
OnRemovePlayerMapMarker OnRemovePlayerMapMarker_Original = NULL;


__int64 OnRemovePlayerMapMarker_MyHook(PlayerCharacter* PCChar)
{

	OnPlayerMapMarkerStateChangeEventHandler.ProcessEventOnPlayerMapMarkerStateChangeEvent(false, NiPoint3(), NULL);

	return OnRemovePlayerMapMarker_Original(PCChar);
}



typedef bool(__fastcall* OnKnockExplosion)(void* me, Actor* ActorRef, const NiPoint3* PosInfo, float force);
RelocAddr <OnKnockExplosion> OnKnockExplosion_Hook(fn_Main_OnKnockExplosion);
OnKnockExplosion OnKnockExplosion_Original = NULL;


bool OnKnockExplosion_MyHook(void* me, Actor* ActorRef, const NiPoint3* PosInfo, float force)
{
	//Console_Print("OnKnockExplosion works");

	//if (ActorRef)
	//{
	//	Console_Print("ActorRef>>>%x, X::%f Y::%f Z::%f Force::%f", ActorRef->formID,ActorRef->pos.x, ActorRef->pos.y, ActorRef->pos.z, force);
	//}

	if (sup::compat::ActorCanBeKnockedDown(ActorRef))
	{
		OnActorRagdollStartEventHandler.ProcessEventOnRagdollStart(ActorRef, (NiPoint3*)PosInfo, force);
	}
	
	return OnKnockExplosion_Original(me, ActorRef, PosInfo, force);
}





typedef void(__fastcall* OnPlayerRadioState)(::RadioManager* PCChar, UInt8 bEnable);
RelocAddr <OnPlayerRadioState> OnPlayerRadioState_Hook(fn_Main_OnPlayerRadioState);
OnPlayerRadioState OnPlayerRadioState_Original = NULL;

void OnPlayerRadioState_MyHook(::RadioManager* me, UInt8 bOnGamemode)
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
RelocAddr <OnGetUpFromKnock> OnGetUpFromKnock_Hook(fn_Main_OnGetUpFromKnock);
OnGetUpFromKnock OnGetUpFromKnock_Original = NULL;


void OnGetUpFromKnock_MyHook(void* me, Actor* ActorRef)
{
	OnGetUpFromKnock_Original(me, ActorRef);
	OnActorRagdollFinishEventHandler.ProcessEventOnActorRagdollFinish(ActorRef);
}











typedef void(__fastcall* OnProcessWorkshopSwitchEvent)(void* me, TESObjectREFR* Object1, TESObjectREFR* Object2, bool bUnk); 
RelocAddr <OnProcessWorkshopSwitchEvent> OnProcessWorkshopSwitchEvent_Hook(0xD5F310);
OnProcessWorkshopSwitchEvent OnProcessWorkshopSwitchEvent_Original = NULL;




typedef void(__fastcall* GatherPotentialConnectionsAroundRadiator)(TESObjectREFR* Radiator, BSTSet<TESObjectREFR*>& ArToFil);
RelocAddr <GatherPotentialConnectionsAroundRadiator> GatherPotentialConnectionsAroundRadiator_func(0x201610);

// --- AddConnection / RemoveConnection — fire the onpowerconnection SUP event ---
// Recreated from the pre-port build's hook pair (identity verified against the
// Workshop::ExtraData cluster, IDs 2194998/2194999 -> 0x3873B0/0x3878B0).
typedef void(__fastcall* OnAddGridConnection)(Workshop::ExtraData* WorkshopData, TESObjectREFR* akRef1, TESObjectREFR* akRef2, TESObjectREFR* akWireRef);
RelocAddr <OnAddGridConnection> OnAddGridConnection_Hook(fn_WorkShopExtra_AddGridConnection);
OnAddGridConnection OnAddGridConnection_Original = NULL;

typedef void(__fastcall* OnRemoveGridConnection)(Workshop::ExtraData* WorkshopData, TESObjectREFR* akRef1, TESObjectREFR* akRef2, TESObjectREFR* akWireRef, bool bCleanUp);
RelocAddr <OnRemoveGridConnection> OnRemoveGridConnection_Hook(fn_WorkShopExtra_RemoveGridConnection);
OnRemoveGridConnection OnRemoveGridConnection_Original = NULL;

void OnAddGridConnection_MyHook(Workshop::ExtraData* WorkshopData, TESObjectREFR* akRef1, TESObjectREFR* akRef2, TESObjectREFR* akWireRef)
{
	_DMESSAGE("OnAddGridConnection>>>akRef1>>%x,akRef2>>%x,akWireRef>>%x", akRef1 ? akRef1->formID : 0, akRef2 ? akRef2->formID : 0, akWireRef ? akWireRef->formID : 0);

	if (!akRef1 || !akRef2 || !WorkshopData)
	{
		OnAddGridConnection_Original(WorkshopData, akRef1, akRef2, akWireRef);
		return;
	}

	OnAddGridConnection_Original(WorkshopData, akRef1, akRef2, akWireRef);

	OnPowerConnectionEventHandler.ProcessEventOnPowerConnection(true, WorkshopData, akRef1, akRef2, akWireRef);
}

void OnRemoveGridConnection_MyHook(Workshop::ExtraData* WorkshopData, TESObjectREFR* akRef1, TESObjectREFR* akRef2, TESObjectREFR* akWireRef, bool bCleanUp)
{
	_DMESSAGE("OnRemoveGridConnection>>>akRef1>>%x,akRef2>>%x,akWireRef>>%x", akRef1 ? akRef1->formID : 0, akRef2 ? akRef2->formID : 0, akWireRef ? akWireRef->formID : 0);

	if (!akRef1 || !akRef2 || !WorkshopData)
	{
		OnRemoveGridConnection_Original(WorkshopData, akRef1, akRef2, akWireRef, bCleanUp);
		return;
	}

	OnPowerConnectionEventHandler.ProcessEventOnPowerConnection(false, WorkshopData, akRef1, akRef2, akWireRef);

	OnRemoveGridConnection_Original(WorkshopData, akRef1, akRef2, akWireRef, bCleanUp);
}

typedef void(__fastcall* OnConnectToRadiator)(TESObjectREFR* RefA, TESObjectREFR* RefB, Workshop::ExtraData* WorkshopData);
RelocAddr <OnConnectToRadiator> OnConnectToRadiator_Hook(fn_WorkShop_ConnectToRadiator);
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



	for (UInt64 i = 0; i < WorkshopData->powerGrid.size(); i++)
	{

		PowerUtils::PowerGrid* pGrid;
		sup::compat::GetNthItem(WorkshopData->powerGrid, i, pGrid);

		if (!pGrid)
		{
			continue;
		}

		if (pGrid)
		{
			Console_Print("new powerGrid>>%d, powered items count>>>%d", i, pGrid->currentlyPowered.size());

			for (UInt64 i = 0; i < pGrid->currentlyPowered.size(); i++)
			{
				UInt32 CurrentlyPoweredID;

				if (sup::compat::GetNthItem(pGrid->currentlyPowered, i, CurrentlyPoweredID))
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
RelocAddr <OnDestroyItem> OnDestroyItem_Hook(0x207B10);
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
RelocAddr <OnSetWantsDelete> OnSetWantsDelete_Hook(fn_Main_OnSetWantsDelete);
OnSetWantsDelete OnSetWantsDelete_Original = NULL;


int IsItemPresentInWorkshop(Workshop::ExtraData* WorkshopData, UInt32 FormID) { // 0 for NOT Exists, 1 for Object, 2 for wire

	for (UInt64 i = 0; i < WorkshopData->powerGrid.size(); i++)
	{
		PowerUtils::PowerGrid* pGrid;
		sup::compat::GetNthItem(WorkshopData->powerGrid, i, pGrid);

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

	for (UInt64 i = 0; i < WorkshopData->powerGrid.size(); i++)
	{
		PowerUtils::PowerGrid* pGrid;
		sup::compat::GetNthItem(WorkshopData->powerGrid, i, pGrid);

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

	for (UInt64 i = 0; i < WorkshopData->powerGrid.size(); i++)
	{
		PowerUtils::PowerGrid* pGrid;
		sup::compat::GetNthItem(WorkshopData->powerGrid, i, pGrid);

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
RelocAddr<_WorkshopDeleteWorkshopItem> WorkshopDeleteWorkshopItem(fn_WorkShop_DeleteWorkshopItem);



typedef void(__fastcall* OnWorkshopDeleteWorkshopItem)(void* me,TESObjectREFR* REF);
RelocAddr <OnWorkshopDeleteWorkshopItem> OnWorkshopDeleteWorkshopItem_Hook(fn_WorkShop_DeleteWorkshopItem);
OnWorkshopDeleteWorkshopItem OnWorkshopDeleteWorkshopItem_Original = NULL;


void OnWorkshopDeleteWorkshopItem_MyHook(void* me, TESObjectREFR* REF)
{
	//Console_Print("OnWorkshopDeleteWorkshopItem works>>%x", REF->formID);

	OnWorkshopDeleteWorkshopItem_Original(me, REF);
}








// Phase 3: f4se/GameMessages.h is not available under CommonLibF4. The only thing it
// provided here was g_TaskInterface, which is unused — removed.

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
					sup::compat::RemoveItem(WorkshopData, MyRef);
					for (auto it = AllWires.begin(); it != AllWires.end(); it++)
					{
						//Console_Print("Removing wire>>%x", Iter->formID);
						WorkshopDeleteWorkshopItem(Iter);
					}
					//Console_Print("Ref is present in extra data AFTER>>%d", IsItemPresentInWorkshop(WorkshopData, MyRef->formID));
				}
				else { 
					Workshop::ContextData contextData(*g_player);
					BSPointerHandle<TESObjectREFR> currentWorkshop = *Workshop::hCurrentWorkshop;
					Workshop::SetCurrentWorkshop(workshopRef);
					NiPointer<TESObjectREFR> refrPointer(MyRef);
					sup::compat::WorkshopScrapReference(&contextData, &refrPointer);
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


					//	sup::compat::RemoveConnection(WorkshopData, Iter.Connection1, Iter.Connection2, MyRef, false);

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

					//sup::compat::RemoveItem(WorkshopData, MyRef);
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
	//	sup::compat::RemoveConnection(WorkshopData, Iter.Connection1, Iter.Connection2, RefC, false);
	//	_DMESSAGE("removed");
	//	Console_Print("Removing wireless connection, Generator>>%x,Receiver>>%x", Iter.Connection1->formID, Iter.Connection2->formID);
	//}

	_DMESSAGE("OnConnectToRadiator finishes");
	Console_Print("OnConnectToRadiator finishes");
}









typedef void(*_UpdateMovingWirelessItem1)(TESObjectREFR* akRef, Workshop::ExtraData* workshopExtraData);
// PowerUtils::UpdateMovingWirelessItem — F4SE master hardcodes 0x0038E990 (REL::ID 2195088).
// (Old raw 0x00201B10 was the 1.10.163 flat address; wrong under NG ASLR.)
RelocAddr <_UpdateMovingWirelessItem1> UpdateMovingWirelessItem1(fn_PowerUtils_UpdateMovingWirelessItem);
_UpdateMovingWirelessItem1 OnUpdateMovingWirelessItem1_Original = NULL;


void _UpdateMovingWirelessItem1_MyHook(TESObjectREFR* akRef, Workshop::ExtraData* workshopExtraData)
{
	//Console_Print("_UpdateMovingWirelessItem1 works, ref>>%x", akRef->formID);

	//Console_Print("Item power received>>%d", ItemIsPowerReceiver(akRef));
	//Console_Print("Item is powered>>%d", sup::compat::IsPowered(workshopExtraData, akRef));
	//Console_Print("Item IsSwitchr>>%d", IsSwitch(akRef->GetObjectReference()));
	

	

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
RelocAddr <_GetActorByHandle> GetActorByHandle_Execute(fn_Main_GetActorByHandle);

typedef void(*_ApplyDismemberment)(void* me, const Dismemberment_Data& DismemberData);
RelocAddr <_ApplyDismemberment> OnApplyDismemberment_Hook(fn_Main_OnApplyDismemberment);
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
RelocAddr <_QueueSaveLoadTask> _QueueSaveLoadTask_Hook(0xCF1220);
_QueueSaveLoadTask _QueueSaveLoadTask_Original = NULL;


void _QueueSaveLoadTask_MyHook(void* BSSaveLoadManager, SInt32 request)
{
	Console_Print("SAVELOAD,REQUEST>>>%d", request);
	_QueueSaveLoadTask_Original(BSSaveLoadManager, request);
}

//bool __fastcall BGSSaveLoadManager::IsSavingAllowed(BGSSaveLoadManager *this)


void f_InitHooks()
{
#if !SUP_ENABLE_HOOKS
	_MESSAGE("C++ hooks DISABLED (SUP_ENABLE_HOOKS=0) - Address Library IDs pending verification");
	return;
#else
	// Real MinHook must be initialized before creating hooks.
	if (MH_Initialize() != MH_OK)
	{
		_MESSAGE("Cannot initialize MinHook");
		return;
	}

	// Hook targets are resolved via REL::ID (raw Address Library offsets). A clean prologue
	// alone is NOT enough — the AE-reference fuzzy IDs sometimes point at the *wrong*
	// function (e.g. SetWantsDelete's old ID resolved to a linked-list removal), so identity
	// matters as much as the prologue. The enabled set below is verified/canonical; the
	// disabled set has unverified identity and is left off rather than risk hooking the
	// wrong function.

	// DISABLED: 0x3C1180 is a lock-guarded container op (identical body to 0xCC1DF0 below),
	// not UpdateSeenDataForPlayerLoc. Signature/identity could not be confirmed.
	//SetHookSUP(OnUpdateSeenDataForPlayerLocSUP, &OnUpdateSeenDataForPlayerLoc_Hook, reinterpret_cast<LPVOID*>(&OnUpdateSeenDataForPlayerLoc_Original), "CellSeenData");
	// DISABLED: 0xC4C010 reads 4 args (rcx/rdx/r8/r9b) but the typedef declares 2 (void*,float).
	//SetHookSUP(OnRadioAddStation_Hook, &OnRadioAddStation_MyHook, reinterpret_cast<LPVOID*>(&OnRadioAddStation_Original), "OnRadioAdd");

	// Quest event dispatch cluster: sequential IDs 2204935/2204940/2204942 -> 0x5D66D0/0x5D69F0/0x5D6AA0
	// (adjacent big-stack-frame functions).
	SetHookSUP(OnQuestActive_Hook, &OnQuestActive_MyHook, reinterpret_cast<LPVOID*>(&OnQuestActive_Original), "OnQuestActive");
	SetHookSUP(OnQuestCompleted_Hook, &OnQuestCompleted_MyHook, reinterpret_cast<LPVOID*>(&OnQuestCompleted_Original), "OnQuestCompleted");
	SetHookSUP(OnQuestFailed_Hook, &OnQuestFailed_MyHook, reinterpret_cast<LPVOID*>(&OnQuestFailed_Original), "OnQuestFailed");
	// DISABLED: ID 4814165 -> 0x125B572 is a `FF 25` indirect-jump thunk, not a function.
	//SetHookSUP(OnConsoleCommand_Hook, &OnConsoleCommand_MyHook, reinterpret_cast<LPVOID*>(&OnConsoleCommand_Original), "OnConsoleCommand");
	// DISABLED: 0x4E2A30 iterates a container via only rcx (1 arg), typedef declares 5 args.
	//SetHookSUP(OnCompileScript_Hook, &OnCompileScript_MyHook, reinterpret_cast<LPVOID*>(&OnCompileScript_Original), "OnCompileScript");
	// DISABLED: identity unverified (ID 2238349 -> 0xEA7470).
	//SetHookSUP(OnSetPlayerMapMarker_Hook, &OnSetPlayerMapMarker_MyHook, reinterpret_cast<LPVOID*>(&OnSetPlayerMapMarker_Original), "OnSetPlayerMapMarker");
	// DISABLED: no ID (same address as OnSetPlayerMapMarker in the old DB).
	//SetHookSUP(OnRemovePlayerMapMarker_Hook, &OnRemovePlayerMapMarker_MyHook, reinterpret_cast<LPVOID*>(&OnRemovePlayerMapMarker_Original), "OnRemovePlayerMapMarker");
	SetHookSUP(OnKnockExplosion_Hook, &OnKnockExplosion_MyHook, reinterpret_cast<LPVOID*>(&OnKnockExplosion_Original), "OnKnockExplosion");
	// DISABLED: 0xCC1DF0 is a lock-guarded container op (identical body to 0x3C1180 above).
	//SetHookSUP(OnPlayerRadioState_Hook, &OnPlayerRadioState_MyHook, reinterpret_cast<LPVOID*>(&OnPlayerRadioState_Original), "OnPlayerRadioState");
	// DISABLED: 0xD5A0D0 reads a global + vector math; 2nd arg (Actor*) unused in the prologue.
	//SetHookSUP(OnGetUpFromKnock_Hook, &OnGetUpFromKnock_MyHook, reinterpret_cast<LPVOID*>(&OnGetUpFromKnock_Original), "OnGetUpFromKnock");
	// ScrapCrashFix — CommonLibF4 canonical ID for TESObjectREFR::SetWantsDelete.
	SetHookSUP(OnSetWantsDelete_Hook, &OnSetWantsDelete_MyHook, reinterpret_cast<LPVOID*>(&OnSetWantsDelete_Original), "OnSetWantsDelete");

	//SetHookSUP(OnHandleLimbReplacement_Hook, &OnHandleLimbReplacement_MyHook, reinterpret_cast<LPVOID*>(&OnHandleLimbReplacement_Original), "OnHandleLimbReplacement");

	SetHookSUP(OnApplyDismemberment_Hook, &OnApplyDismemberment_MyHook, reinterpret_cast<LPVOID*>(&OnApplyDismemberment_Original), "OnApplyDismemberment");


	//SetHookSUP(_QueueSaveLoadTask_Hook, &_QueueSaveLoadTask_MyHook, reinterpret_cast<LPVOID*>(&_QueueSaveLoadTask_Original), "QueueSaveLoadTask");

	// Power-grid hooks — fire the onpowerconnection SUP event. Addresses are the verified
	// Workshop::ExtraData cluster (REL::ID 2194998/2194999/2195090, runtime-verified).
	SetHookSUP(OnAddGridConnection_Hook, &OnAddGridConnection_MyHook, reinterpret_cast<LPVOID*>(&OnAddGridConnection_Original), "OnAddGridConnectionAdd");
	SetHookSUP(OnRemoveGridConnection_Hook, &OnRemoveGridConnection_MyHook, reinterpret_cast<LPVOID*>(&OnRemoveGridConnection_Original), "OnRemoveGridConnection");
	SetHookSUP(OnConnectToRadiator_Hook, &OnConnectToRadiator_MyHook, reinterpret_cast<LPVOID*>(&OnConnectToRadiator_Original), "OnConnectToRadiator");

	
	
	//SetHookSUP(UpdateMovingWirelessItem1, &_UpdateMovingWirelessItem1_MyHook, reinterpret_cast<LPVOID*>(&OnUpdateMovingWirelessItem1_Original), "UpdateMovingWirelessItem1");


	
	//SetHookSUP(OnProcessWorkshopSwitchEvent_Hook, &OnProcessWorkshopSwitch_MyHook, reinterpret_cast<LPVOID*>(&OnProcessWorkshopSwitchEvent_Original), "OnProcessWorkshopSwitch");
#endif



	


	

}








