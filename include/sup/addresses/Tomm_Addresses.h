// SUP F4SE — version-independent addresses (Phase 4).
//
// These were hardcoded 1.10.163 offsets (e.g. `#define fn_X 0x1F69C0`). Under ASLR a raw
// offset used as an absolute address is wrong, and it breaks on any game update. They now
// resolve through CommonLibF4's REL::ID (the F4SE Address Library, `version-*.bin`), so the
// same ID maps to the correct offset for whatever runtime is installed.
//
// IDs come from the AE reference plugin (SUP_AddressLib.h). A few are "fuzzy" (nearest-match
// in the NG database) and some pairs share an ID — these are flagged below; re-verify against
// the 1.11.221 database before relying on them.
//
// Usage is unchanged at call sites: `RelocAddr<T> fn(fn_WorkShopExtra_RemoveItem);` now
// resolves through the Address Library at first use instead of an absolute address.

#pragma once

#include "REL/ID.h"

// Workshop / Power Grid
//
// Phase 4 (resolved 2026-08-13): the old fuzzy 4472xxx IDs pointed at the pre-NG 0x1F6xxx
// address range, which in the 1.11.221 binary holds unrelated code (list allocators,
// linked-list teardown, BSTArray helpers). The real Workshop::ExtraData cluster relocated to
// 0x386xxx-0x389xxx under IDs 2194996-2195016, verified three independent ways:
//   1. F4SE master GameWorkshop.h hardcodes AddItem=0x386D70 / AddConnection=0x3873B0.
//   2. ConnectToRadiator (0x38EFA0) calls 0x3873B0 with (ExtraData*, ref1, ref2, wire).
//   3. The alandtse PDB names + pre-NG ordering match the NG cluster one-for-one.
#define fn_REFR_HasKeywordHelper REL::ID( 2196779 )                   // 0x4120B0 -> 0x412090
#define fn_WorkShopExtra_RemoveItem REL::ID( 2194997 )                // ExtraData::RemoveItem (0x386EA0)
#define fn_WorkShopExtra_AddGridConnection REL::ID( 2194998 )         // ExtraData::AddConnection (0x3873B0, F4SE-master-verified)
#define fn_WorkShopExtra_RemoveGridConnection REL::ID( 2194999 )      // ExtraData::RemoveConnection (0x3878B0)
#define fn_WorkShopExtra_AreDirectlyConnected REL::ID( 2195000 )      // ExtraData::AreDirectlyConnected (0x387DC0)
// PowerGrid::CalculateCapacityAndLoad was inlined by the NG compiler (no standalone symbol
// in the 1.11.221 address library; the old ID 4475969 -> 0x240140 was a BSTArray helper).
// The compat helper reimplements it in C++ over the modeled PowerGrid fields.
#define fn_WorkShopExtra_CalculateCapacityAndLoad REL::ID( 0 )        // inlined in NG — reimplemented in C++
#define fn_WorkShopExtra_GetPowerGridForTarget REL::ID( 2195002 )     // ExtraData::GetPowerGrid (0x387EB0)
#define fn_WorkShopExtra_CleanUpEmptyPowerGrids REL::ID( 2195016 )    // ExtraData::CleanUpEmptyPowerGrids (0x389240)
#define fn_WorkShop_DeleteWorkshopItem REL::ID( 4808450 )             // 0x207A30 -> 0x207A20
#define fn_WorkShop_DisconnectSpline REL::ID( 2195074 )               // CommonLibF4 canonical (0x38DDC0, clean)
#define fn_Workshop_ItemIsWirelessPowerReceiver REL::ID( 2195060 )    // PowerUtils::ItemIsPowerReceiver (CommonLibF4 canonical; 4473100 -> 0x1FC0D0 was a vector op)
#define fn_WorkShop_CalculatePowerRating REL::ID( 2194985 )           // ExtraData::CalculatePowerRating (0x386490, behaviorally verified: sums per-grid capacity-load, writes powerRating+offGridItems)
#define fn_PowerUtils_UpdateMovingWirelessItem REL::ID( 2195088 )     // PowerUtils::UpdateMovingWirelessItem (0x38E990, F4SE-master hardcode + 2-arg signature verified) canonical; 4473100 -> 0x1FC0D0 was a vector op)
#define fn_Workshop_IsWithinRadiatorRange REL::ID( 2195085 )          // PowerUtils::IsWithinRadiatorRange (PDB-verified, 0x38E7B0)
#define fn_Workshop_IsPowered REL::ID( 2195001 )                      // ExtraData::IsPowered (0x387E30)
#define fn_WorkShop_ConnectToRadiator REL::ID( 2195090 )              // PowerUtils::ConnectToRadiator (0x38EFA0, runtime-verified: resolves clean)

// Radio
#define fn_Main_RadioUpdate REL::ID( 0 )                              // DISABLED — no ID (0xCC3450)
#define fn_Main_OnRadioAddStation REL::ID( 2229305 )                  // 0xC4BEE0 -> 0xC4BE80

// Quest
#define fn_Main_OnQuestComplete REL::ID( 2204940 )                    // 0x5D69B0 -> 0x5D69E0
#define fn_Main_OnQuestActive REL::ID( 2204942 )                      // 0x5D6AC0 -> 0x5D6A90
#define fn_Main_OnQuestFailed REL::ID( 2204935 )                      // 0x5D6690 -> 0x5D66C0

// Console / Script
#define fn_Main_OnConsoleCommand REL::ID( 4814165 )                   // 0x125B4A0 -> 0x125B4A2
#define fn_Main_OnCompileScript REL::ID( 2200532 )                    // 0x4E2A60 -> 0x4E2A20

// Map marker
#define fn_Main_OnSePlayerMarker REL::ID( 2238349 )                   // 0xEA7220 -> 0xEA72D0
#define fn_Main_OnRemovePlayerMapMarker REL::ID( 0 )                  // DISABLED — same addr as set

// Combat / physics
#define fn_Main_OnKnockExplosion REL::ID( 2237514 )                   // 0xE73F40 -> 0xE73F00
#define fn_Main_OnGetUpFromKnock REL::ID( 2232921 )                   // 0xD59F00 -> 0xD59F40

// Radio state
#define fn_Main_OnPlayerRadioState REL::ID( 2230851 )                 // 0xCC1B90 -> 0xCC1C60
// SUSPICIOUS: shares ID 2230851 with OnPlayerRadioState — re-verify.
#define fn_Main_IsPlayerRadioEnabled REL::ID( 2230851 )               // 0xCC1CF0 -> 0xCC1C60

// Cell / reference
#define fn_Main_UpdateSeenDataForPlayerLoc REL::ID( 2195601 )         // 0x3C10F0 -> 0x3C1170
#define fn_Main_OnSetWantsDelete REL::ID( 2201199 )                   // CommonLibF4 TESObjectREFR::SetWantsDelete (0x519420)
#define fn_TaskInterface_ProcessWorkshopSwitchEvent REL::ID( 2232980 ) // 0xD5F310 -> 0xD5EFF0 (fuzzy)

// Actor
#define fn_Actor_Jump REL::ID( 2233155 )                              // 0xD73770 -> 0xD73620 (fuzzy)
#define fn_Actor_IsInAir REL::ID( 2233999 )                           // 0xDA8AD0 -> 0xDA8C50 (fuzzy)
#define fn_Actor_IsDead REL::ID( 2233624 )                            // 0xD89D40 -> 0xD89D50
#define fn_Actor_CanBeKnockedDown REL::ID( 0 )                        // DISABLED — ID 2233626 -> 0xD89F70 (NG) is a flag-check family, not bool CanKnockDown(bool); reimplemented over ActorState::knockState in F4SECompat.h

// Utility
#define fn_Main_GetFormByEditorID REL::ID( 2179550 )                  // 0x152EB0 -> 0x152E80
#define fn_Main_GetRadioStationName REL::ID( 2230850 )                // 0xCC1950 -> 0xCC19D0
// TimeMultiplier resolved via CommonLibF4 RE::BSTimer (QGlobalTimeMultiplier=2666307, SetGlobalTimeMultiplier=2267970); see Decoding.h
#define fn_Main_GetActorByHandle REL::ID( 2168321 )                   // 0x23870 -> 0x23780
#define fn_Main_OnApplyDismemberment REL::ID( 2191382 )               // 0x2B04A0 -> 0x2B0530
#define fn_Main_GetTerminalLinkKeyword REL::ID( 4473787 )             // 0x204400 (exact)
