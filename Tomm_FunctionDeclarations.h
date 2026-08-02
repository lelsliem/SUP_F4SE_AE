typedef TESObjectREFR* (*_DisconnectSpline)(TESObjectREFR* akRef);
RelocAddr<_DisconnectSpline> DisconnectSpline(fn_WorkShop_DisconnectSpline);


typedef void(__fastcall* WorkshopDestroyItem)(Workshop::ExtraData* MyData, TESObjectREFR** MyRef, bool bHasData, UInt8 iUnk, bool bGamemode);
RelocAddr <WorkshopDestroyItem> WorkshopDestroyItem_func(0x207B10);


typedef bool (*_ItemInterruptsPower)(TESObjectREFR* akRef);
RelocAddr<_ItemInterruptsPower> ItemInterruptsPower(fn_Workshop_ItemInterruptsPower);


typedef bool (*_ItemIsWirelessPowerReceiver)(TESObjectREFR* akRef);
RelocAddr<_ItemIsWirelessPowerReceiver> ItemIsWirelessPowerReceiver(fn_Workshop_ItemIsWirelessPowerReceiver);

typedef bool (*_IsWithinRadiatorRange)(TESObjectREFR* RadiatorRef, TESObjectREFR* ObjectRef);
RelocAddr<_IsWithinRadiatorRange> IsWithinRadiatorRange(fn_Workshop_IsWithinRadiatorRange);




typedef bool (*_ConnectToRadiator)(TESObjectREFR* RadiatorRef, TESObjectREFR* ObjectRef, Workshop::ExtraData* MyData);
RelocAddr<_ConnectToRadiator> ConnectToRadiator(0x203C80);

typedef bool (*_IsSwitch)(TESForm* ObjectRef);
RelocAddr<_IsSwitch> IsSwitch(0x328A80);



typedef UINT64 (*_GetOpenState)(TESObjectREFR* ObjectRef);
RelocAddr<_GetOpenState> GetOpenState(0x1439B0);


class RadioManager {
public:
	// Runtime function pointer instead of DEFINE_MEMBER_FN (Address Library compatible)
	bool IsPlayerRadioEnabled() {
		typedef bool(__fastcall *FN)(void*);
		FN fn = (FN)fn_Main_IsPlayerRadioEnabled;
		return fn ? fn(this) : false;
	}
};


typedef void(*_WorkshopCalculatePowerRating)(TESObjectREFR* akRef);
RelocAddr<_WorkshopCalculatePowerRating> WorkshopCalculatePowerRating(0x1F6170);
_WorkshopCalculatePowerRating WorkshopCalculatePowerRating_Original = NULL;

typedef void(*_WorkshopRecalculateResources)(TESObjectREFR* akRef);
RelocAddr<_WorkshopRecalculateResources> WorkshopRecalculateResources(0x1F5D40);
_WorkshopRecalculateResources WorkshopRecalculateResources_Original = NULL;


typedef bool (*_ItemIsPowerReceiver)(TESObjectREFR* akRef);
RelocAddr<_ItemIsPowerReceiver> ItemIsPowerReceiver(0x1FC140);


typedef bool (*_ItemContainsNode)(TESObjectREFR* akRef);
RelocAddr<_ItemContainsNode> ItemContainsNode(0x241910);





typedef TESForm* (*_GetFormByEditorID)(const char * EditorID);
RelocAddr<_GetFormByEditorID> GetFormByEditorID(fn_Main_GetFormByEditorID);



typedef const char* (*_GetRadioStationName)(float fFrequency);
RelocAddr<_GetRadioStationName> GetRadioStationName(fn_Main_GetRadioStationName);

typedef void(__fastcall* OnConsoleCommand)(const char* Text);
RelocAddr <OnConsoleCommand> OnConsoleCommand_Hook(fn_Main_OnConsoleCommand); 
OnConsoleCommand OnConsoleCommand_Original = NULL;

typedef void(__fastcall* OnWorkshopHandleReferenceEvent)(TESObjectREFR* REF, TESObjectREFR* REF2);
RelocAddr <OnWorkshopHandleReferenceEvent> OnWorkshopHandleReferenceEvent_Hook(0x1F44D0); //7FF7D0B944D0
OnWorkshopHandleReferenceEvent OnWorkshopHandleReferenceEvent_Original = NULL;

typedef BGSKeyword* (__fastcall* GetTerminalLinkKeyword)(TESObjectREFR* REF);
RelocAddr <GetTerminalLinkKeyword> GetTerminalLinkKeyword_Hook(fn_Main_GetTerminalLinkKeyword);