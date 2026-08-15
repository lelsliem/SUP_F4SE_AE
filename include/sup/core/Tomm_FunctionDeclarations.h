typedef TESObjectREFR* (*_DisconnectSpline)(TESObjectREFR* akRef);
RelocAddr<_DisconnectSpline> DisconnectSpline(fn_WorkShop_DisconnectSpline);


typedef void(__fastcall* WorkshopDestroyItem)(Workshop::ExtraData* MyData, TESObjectREFR** MyRef, bool bHasData, UInt8 iUnk, bool bGamemode);
RelocAddr <WorkshopDestroyItem> WorkshopDestroyItem_func(REL::ID(4808453)); // 0x207B10 -> 0x207B30 (nearest)


typedef bool (*_ItemIsWirelessPowerReceiver)(TESObjectREFR* akRef);
RelocAddr<_ItemIsWirelessPowerReceiver> ItemIsWirelessPowerReceiver(fn_Workshop_ItemIsWirelessPowerReceiver);

typedef bool (*_IsWithinRadiatorRange)(TESObjectREFR* RadiatorRef, TESObjectREFR* ObjectRef);
RelocAddr<_IsWithinRadiatorRange> IsWithinRadiatorRange(fn_Workshop_IsWithinRadiatorRange);




typedef bool (*_ConnectToRadiator)(TESObjectREFR* RadiatorRef, TESObjectREFR* ObjectRef, Workshop::ExtraData* MyData);
// PowerUtils::ConnectToRadiator — PDB-verified (alandtse):
//   "PowerUtils::ConnectToRadiator(TESObjectREFR&,TESObjectREFR&,Workshop::ExtraData&)"
// resolves to 0x38EFA0 via REL::ID 2195090 in the 1.11.221 Address Library.
RelocAddr<_ConnectToRadiator> ConnectToRadiator(REL::ID(2195090));

// TESObjectACTI::IsSwitch(TESForm*) — reimplemented in sup::compat (the game checks the
// "ActivatorIsSwitch" keyword). Old REL::ID(2193572) -> 0x328AB0 was a BSTHashMap helper that
// crashed when called with a TESForm* during item placement.
typedef bool (*_IsSwitch)(TESForm* ObjectRef);
inline bool IsSwitch(TESForm* ObjectRef) { return sup::compat::IsSwitch(ObjectRef); }


// BGSOpenCloseForm::GetOpenState(TESObjectREFR*) — sup::compat routes to CommonLibF4's canonical
// REL::ID 2192799. Old REL::ID(2178825) -> 0x143990 was a TLS accessor that crashed on placement.
typedef UINT64 (*_GetOpenState)(TESObjectREFR* ObjectRef);
inline UINT64 GetOpenState(TESObjectREFR* ObjectRef)
{
	return static_cast<UINT64>(sup::compat::GetOpenState(ObjectRef));
}


class RadioManager {
public:
	DEFINE_MEMBER_FN_0(IsPlayerRadioEnabled, bool, fn_Main_IsPlayerRadioEnabled); //_BOOL8 __fastcall RadioManager::QPlayerRadioEnabled(RadioManager *this)
};


typedef void(*_WorkshopCalculatePowerRating)(TESObjectREFR* akRef);
// ExtraData::CalculatePowerRating — behaviorally verified at 0x386490 (REL::ID 2194985):
// reads the refr's workshop extra data, sums per-grid (capacity-load) into powerRating and
// sets offGridItems. (Old fuzzy ID 4472827 -> 0x1F6160 was pre-NG; wrong in 1.11.221.)
RelocAddr<_WorkshopCalculatePowerRating> WorkshopCalculatePowerRating(fn_WorkShop_CalculatePowerRating);
_WorkshopCalculatePowerRating WorkshopCalculatePowerRating_Original = NULL;

typedef void(*_WorkshopRecalculateResources)(TESObjectREFR* akRef);
// NOTE: ID 7099885 resolves to 0x1F5D50 (`mov rax,rcx; ret`), a harmless no-op rather than
// the real recalc function. The likely real target is ID 4472821 -> 0x1F5D60 (clean prologue),
// but that function reads 3 args while SUP's typedef declares 1 (TESObjectREFR*), so it is
// NOT used — calling it with the wrong signature could crash. Left as the safe no-op.
RelocAddr<_WorkshopRecalculateResources> WorkshopRecalculateResources(REL::ID(7099885));
_WorkshopRecalculateResources WorkshopRecalculateResources_Original = NULL;


typedef bool (*_ItemIsPowerReceiver)(TESObjectREFR* akRef);
RelocAddr<_ItemIsPowerReceiver> ItemIsPowerReceiver(REL::ID(2195060)); // PowerUtils::ItemIsPowerReceiver (canonical)







typedef TESForm* (*_GetFormByEditorID)(const char * EditorID);
RelocAddr<_GetFormByEditorID> GetFormByEditorID(fn_Main_GetFormByEditorID);



typedef const char* (*_GetRadioStationName)(float fFrequency);
RelocAddr<_GetRadioStationName> GetRadioStationName(fn_Main_GetRadioStationName);

typedef void(__fastcall* OnConsoleCommand)(const char* Text);
RelocAddr <OnConsoleCommand> OnConsoleCommand_Hook(fn_Main_OnConsoleCommand); 
OnConsoleCommand OnConsoleCommand_Original = NULL;

typedef BGSKeyword* (__fastcall* GetTerminalLinkKeyword)(TESObjectREFR* REF);
RelocAddr <GetTerminalLinkKeyword> GetTerminalLinkKeyword_Hook(fn_Main_GetTerminalLinkKeyword);