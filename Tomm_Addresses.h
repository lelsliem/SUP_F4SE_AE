#pragma once

// SUP F4SE Address Definitions
//
// For F4SE 0.7.7+ and Fallout 4 1.10.980+ (Next Gen) / 1.11.x (Anniversary Edition):
// Addresses are now resolved at runtime via Address Library (see SUP_AddressLib.h)
// The macros fn_* are defined in SUP_AddressLib.h using Address Library lookups.
//
// The old hardcoded offsets below are preserved as reference for finding Address Library IDs.
// These offsets were valid for Fallout 4 version 1.10.163 (pre-Next Gen).

// =============================================================================
// LEGACY OFFSETS (1.10.163) - FOR REFERENCE ONLY
// Use these to find the corresponding Address Library IDs:
// 1. Look up the offset in the Address Library database for version 1.10.163
// 2. Find the ID that maps to that offset
// 3. Add that ID to SUP_AddressLib.h in the IDs namespace
// =============================================================================

/*
Legacy Offset Reference Table (Fallout 4 v1.10.163):

// Workshop/Power Grid Functions
fn_REFR_HasKeywordHelper              = 0x4120B0  // TESObjectREFR::HasKeywordHelper
fn_WorkShopExtra_RemoveItem           = 0x1F69C0  // Workshop::ExtraData::RemoveItem
fn_WorkShopExtra_AddGridConnection    = 0x1F6E20  // Workshop::ExtraData::AddConnection
fn_WorkShopExtra_RemoveGridConnection = 0x1F7440  // Workshop::ExtraData::RemoveConnection
fn_WorkShopExtra_AreDirectlyConnected = 0x1F77D0  // Workshop::ExtraData::AreDirectlyConnected
fn_WorkShopExtra_CalculateCapacityAndLoad = 0x240160  // PowerUtils::PowerGrid::CalculateCapacityAndLoad
fn_WorkShopExtra_GetPowerGridForTarget = 0x1F7950  // Workshop::ExtraData::GetPowerGrid
fn_WorkShopExtra_CleanUpEmptyPowerGrids = 0x1F8550  // Workshop::ExtraData::CleanUpEmptyPowerGrids
fn_WorkShop_DeleteWorkshopItem        = 0x207A30  // Workshop::DeleteWorkshopItem
fn_WorkShop_DisconnectSpline          = 0x200F50  // SplineUtils::DisconnectSpline
fn_Workshop_ItemInterruptsPower       = 0x1FC360  // PowerUtils::ItemInterruptsPower
fn_Workshop_ItemIsWirelessPowerReceiver = 0x1FC140  // Wireless power receiver check
fn_Workshop_IsWithinRadiatorRange     = 0x201930  // Radiator range check
fn_Workshop_IsPowered                 = 0x1F7840  // Workshop::ExtraData::IsPowered

// Radio Functions
fn_Main_RadioUpdate                   = 0xCC3450  // RadioManager::Update
fn_Main_OnRadioAddStation             = 0xC4BEE0  // PipboyRadioData::AddStation

// Quest Functions
fn_Main_OnQuestComplete               = 0x5D69B0  // TESQuest::SetCompleted
fn_Main_OnQuestActive                 = 0x5D6AC0  // TESQuest::SetActive
fn_Main_OnQuestFailed                 = 0x5D6690  // TESQuest::SetFailed

// Console/Script Functions
fn_Main_OnConsoleCommand              = 0x125B4A0 // Console::ExecuteCommand
fn_Main_OnCompileScript               = 0x4E2A60  // Script::CompilePartialScript

// Map Marker Functions
fn_Main_OnSePlayerMarker              = 0xEA7220  // PlayerCharacter::SetPlayerMapMarker
fn_Main_OnRemovePlayerMapMarker       = 0xEA7250  // PlayerCharacter::RemovePlayerMapMarker

// Combat/Physics Functions
fn_Main_OnKnockExplosion              = 0xE73F40  // AIProcess::KnockExplosion
fn_Main_OnGetUpFromKnock              = 0xD59F00  // TaskQueueInterface::QueueGetUpFromKnock

// Radio State Functions
fn_Main_OnPlayerRadioState            = 0xCC1B90  // RadioManager::EnablePlayerRadio
fn_Main_IsPlayerRadioEnabled          = 0xCC1CF0  // RadioManager::QPlayerRadioEnabled

// Cell/Reference Functions
fn_Main_UpdateSeenDataForPlayerLoc    = 0x3C10F0  // TESObjectCELL::UpdateSeenDataForPlayerLoc
fn_Main_OnSetWantsDelete              = 0x4115C0  // TESObjectREFR::SetWantsDelete
fn_TaskInterface_ProcessWorkshopSwitchEvent = 0xD5F310  // TaskQueueInterface::QueueProcessWorkshopSwitchEvent

// Actor Functions
fn_Actor_Jump                         = 0xD73770  // Actor::Jump
fn_Actor_IsInAir                      = 0xDA8AD0  // Actor::IsInAir
fn_Actor_IsDead                       = 0xD89D40  // Actor::IsDead
fn_Actor_CanBeKnockedDown             = 0xD89DD0  // Actor::CanKnockDown

// Utility Functions
fn_Main_GetFormByEditorID             = 0x152EB0  // TESForm::GetFormByEditorID
fn_Main_GetRadioStationName           = 0xCC1950  // RadioManager::GetRadioStationName
fn_Address_TimeMultiplier             = 0x38457D0 // BSTimer time multiplier address
fn_Main_GetActorByHandle              = 0x23870   // BSPointerHandleManagerInterface::GetSmartPointer
fn_Main_OnApplyDismemberment          = 0x2B04A0  // BGSDismembermentManager::ApplyDismemberment
fn_Main_GetTerminalLinkKeyword        = 0x204400  // TerminalUtils::GetTerminalLinkKeyword
*/

// =============================================================================
// The actual address definitions (fn_*) are now provided by SUP_AddressLib.h
// which resolves them at runtime via Address Library.
// =============================================================================

// Legacy member function definitions (commented out - use Address Library instead)
//DEFINE_MEMBER_FN_1(CanBeKnockedDown, bool, 0xD89DD0, bool);
//DEFINE_MEMBER_FN_1(Jump, int, fn_Actor_Jump, float fUnk);
//DEFINE_MEMBER_FN_0(IsInAir, bool, fn_Actor_IsInAir);
//DEFINE_MEMBER_FN_0(IsDead, bool, fn_Actor_IsDead);
