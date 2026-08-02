// SUP F4SE Address Library Implementation
// This file implements the Address Library integration for version-independent address resolution

#include "SUP_AddressLib.h"

// Global Address Library database instance
VersionDb g_versionDb;

namespace SUP_AddressLib
{
	// Resolved addresses - initialized to 0, set by Initialize()
	uintptr_t addr_REFR_HasKeywordHelper = 0;
	uintptr_t addr_WorkShopExtra_RemoveItem = 0;
	uintptr_t addr_WorkShopExtra_AddGridConnection = 0;
	uintptr_t addr_WorkShopExtra_RemoveGridConnection = 0;
	uintptr_t addr_WorkShopExtra_AreDirectlyConnected = 0;
	uintptr_t addr_WorkShopExtra_CalculateCapacityAndLoad = 0;
	uintptr_t addr_WorkShopExtra_GetPowerGridForTarget = 0;
	uintptr_t addr_WorkShopExtra_CleanUpEmptyPowerGrids = 0;
	uintptr_t addr_WorkShop_DeleteWorkshopItem = 0;
	uintptr_t addr_WorkShop_DisconnectSpline = 0;
	uintptr_t addr_Workshop_ItemInterruptsPower = 0;
	uintptr_t addr_Workshop_ItemIsWirelessPowerReceiver = 0;
	uintptr_t addr_Workshop_IsWithinRadiatorRange = 0;
	uintptr_t addr_Workshop_IsPowered = 0;
	uintptr_t addr_Main_RadioUpdate = 0;
	uintptr_t addr_Main_OnRadioAddStation = 0;
	uintptr_t addr_Main_OnQuestComplete = 0;
	uintptr_t addr_Main_OnQuestActive = 0;
	uintptr_t addr_Main_OnQuestFailed = 0;
	uintptr_t addr_Main_OnConsoleCommand = 0;
	uintptr_t addr_Main_OnCompileScript = 0;
	uintptr_t addr_Main_OnSePlayerMarker = 0;
	uintptr_t addr_Main_OnRemovePlayerMapMarker = 0;
	uintptr_t addr_Main_OnKnockExplosion = 0;
	uintptr_t addr_Main_OnGetUpFromKnock = 0;
	uintptr_t addr_Main_OnPlayerRadioState = 0;
	uintptr_t addr_Main_IsPlayerRadioEnabled = 0;
	uintptr_t addr_Main_UpdateSeenDataForPlayerLoc = 0;
	uintptr_t addr_Main_OnSetWantsDelete = 0;
	uintptr_t addr_TaskInterface_ProcessWorkshopSwitchEvent = 0;
	uintptr_t addr_Actor_Jump = 0;
	uintptr_t addr_Actor_IsInAir = 0;
	uintptr_t addr_Actor_IsDead = 0;
	uintptr_t addr_Actor_CanBeKnockedDown = 0;
	uintptr_t addr_Main_GetFormByEditorID = 0;
	uintptr_t addr_Main_GetRadioStationName = 0;
	uintptr_t addr_Address_TimeMultiplier = 0;
	uintptr_t addr_Main_GetActorByHandle = 0;
	uintptr_t addr_Main_OnApplyDismemberment = 0;
	uintptr_t addr_Main_GetTerminalLinkKeyword = 0;

	static bool s_initialized = false;

	// Helper function to resolve address and log if not found
	static uintptr_t ResolveAndLog(uint64_t id, const char* name)
	{
		if (id == 0)
		{
			// ID not yet configured - using placeholder
			_MESSAGE("  %s: ID=0 (not configured)", name);
			return 0;
		}

		uintptr_t addr = g_versionDb.FindAddressById(id);
		if (addr == 0)
		{
			// Address not found in database
			_MESSAGE("  %s: ID=%llu NOT FOUND", name, id);
		}
		else
		{
			_MESSAGE("  %s: ID=%llu -> 0x%llX", name, id, addr);
		}
		return addr;
	}

	bool Initialize()
	{
		if (s_initialized)
			return true;

		// Load the Address Library database
		if (!g_versionDb.Load())
		{
			// Failed to load - this is a critical error
			// Addresses will remain 0 and features using them will not work
			return false;
		}

		// Resolve all addresses from their IDs
		// NOTE: IDs are currently set to 0 (placeholders)
		// You need to fill in the correct IDs from the Address Library database
		//
		// To find the IDs:
		// 1. Use AddressLibDecoder from CommonLibF4 to dump the database
		// 2. Search for the function by name or known offset
		// 3. Update the IDs in SUP_AddressLib.h
		//
		// Example: If you know TESObjectREFR::HasKeywordHelper was at offset 0x4120B0 in 1.10.163,
		// look up that offset in the 1.10.163 database to find its ID, then use that ID here.

		_MESSAGE("Resolving Address Library IDs:");
		addr_REFR_HasKeywordHelper = ResolveAndLog(IDs::REFR_HasKeywordHelper, "REFR_HasKeywordHelper");
		addr_WorkShopExtra_RemoveItem = ResolveAndLog(IDs::WorkShopExtra_RemoveItem, "WorkShopExtra_RemoveItem");
		addr_WorkShopExtra_AddGridConnection = ResolveAndLog(IDs::WorkShopExtra_AddGridConnection, "WorkShopExtra_AddGridConnection");
		addr_WorkShopExtra_RemoveGridConnection = ResolveAndLog(IDs::WorkShopExtra_RemoveGridConnection, "WorkShopExtra_RemoveGridConnection");
		addr_WorkShopExtra_AreDirectlyConnected = ResolveAndLog(IDs::WorkShopExtra_AreDirectlyConnected, "WorkShopExtra_AreDirectlyConnected");
		addr_WorkShopExtra_CalculateCapacityAndLoad = ResolveAndLog(IDs::WorkShopExtra_CalculateCapacityAndLoad, "WorkShopExtra_CalculateCapacityAndLoad");
		addr_WorkShopExtra_GetPowerGridForTarget = ResolveAndLog(IDs::WorkShopExtra_GetPowerGridForTarget, "WorkShopExtra_GetPowerGridForTarget");
		addr_WorkShopExtra_CleanUpEmptyPowerGrids = ResolveAndLog(IDs::WorkShopExtra_CleanUpEmptyPowerGrids, "WorkShopExtra_CleanUpEmptyPowerGrids");
		addr_WorkShop_DeleteWorkshopItem = ResolveAndLog(IDs::WorkShop_DeleteWorkshopItem, "WorkShop_DeleteWorkshopItem");
		addr_WorkShop_DisconnectSpline = ResolveAndLog(IDs::WorkShop_DisconnectSpline, "WorkShop_DisconnectSpline");
		addr_Workshop_ItemInterruptsPower = ResolveAndLog(IDs::Workshop_ItemInterruptsPower, "Workshop_ItemInterruptsPower");
		addr_Workshop_ItemIsWirelessPowerReceiver = ResolveAndLog(IDs::Workshop_ItemIsWirelessPowerReceiver, "Workshop_ItemIsWirelessPowerReceiver");
		addr_Workshop_IsWithinRadiatorRange = ResolveAndLog(IDs::Workshop_IsWithinRadiatorRange, "Workshop_IsWithinRadiatorRange");
		addr_Workshop_IsPowered = ResolveAndLog(IDs::Workshop_IsPowered, "Workshop_IsPowered");
		addr_Main_RadioUpdate = ResolveAndLog(IDs::Main_RadioUpdate, "Main_RadioUpdate");
		addr_Main_OnRadioAddStation = ResolveAndLog(IDs::Main_OnRadioAddStation, "Main_OnRadioAddStation");
		addr_Main_OnQuestComplete = ResolveAndLog(IDs::Main_OnQuestComplete, "Main_OnQuestComplete");
		addr_Main_OnQuestActive = ResolveAndLog(IDs::Main_OnQuestActive, "Main_OnQuestActive");
		addr_Main_OnQuestFailed = ResolveAndLog(IDs::Main_OnQuestFailed, "Main_OnQuestFailed");
		addr_Main_OnConsoleCommand = ResolveAndLog(IDs::Main_OnConsoleCommand, "Main_OnConsoleCommand");
		addr_Main_OnCompileScript = ResolveAndLog(IDs::Main_OnCompileScript, "Main_OnCompileScript");
		addr_Main_OnSePlayerMarker = ResolveAndLog(IDs::Main_OnSePlayerMarker, "Main_OnSePlayerMarker");
		addr_Main_OnRemovePlayerMapMarker = ResolveAndLog(IDs::Main_OnRemovePlayerMapMarker, "Main_OnRemovePlayerMapMarker");
		addr_Main_OnKnockExplosion = ResolveAndLog(IDs::Main_OnKnockExplosion, "Main_OnKnockExplosion");
		addr_Main_OnGetUpFromKnock = ResolveAndLog(IDs::Main_OnGetUpFromKnock, "Main_OnGetUpFromKnock");
		addr_Main_OnPlayerRadioState = ResolveAndLog(IDs::Main_OnPlayerRadioState, "Main_OnPlayerRadioState");
		addr_Main_IsPlayerRadioEnabled = ResolveAndLog(IDs::Main_IsPlayerRadioEnabled, "Main_IsPlayerRadioEnabled");
		addr_Main_UpdateSeenDataForPlayerLoc = ResolveAndLog(IDs::Main_UpdateSeenDataForPlayerLoc, "Main_UpdateSeenDataForPlayerLoc");
		addr_Main_OnSetWantsDelete = ResolveAndLog(IDs::Main_OnSetWantsDelete, "Main_OnSetWantsDelete");
		addr_TaskInterface_ProcessWorkshopSwitchEvent = ResolveAndLog(IDs::TaskInterface_ProcessWorkshopSwitchEvent, "TaskInterface_ProcessWorkshopSwitchEvent");
		addr_Actor_Jump = ResolveAndLog(IDs::Actor_Jump, "Actor_Jump");
		addr_Actor_IsInAir = ResolveAndLog(IDs::Actor_IsInAir, "Actor_IsInAir");
		addr_Actor_IsDead = ResolveAndLog(IDs::Actor_IsDead, "Actor_IsDead");
		addr_Actor_CanBeKnockedDown = ResolveAndLog(IDs::Actor_CanBeKnockedDown, "Actor_CanBeKnockedDown");
		addr_Main_GetFormByEditorID = ResolveAndLog(IDs::Main_GetFormByEditorID, "Main_GetFormByEditorID");
		addr_Main_GetRadioStationName = ResolveAndLog(IDs::Main_GetRadioStationName, "Main_GetRadioStationName");
		addr_Address_TimeMultiplier = ResolveAndLog(IDs::Address_TimeMultiplier, "Address_TimeMultiplier");
		addr_Main_GetActorByHandle = ResolveAndLog(IDs::Main_GetActorByHandle, "Main_GetActorByHandle");
		addr_Main_OnApplyDismemberment = ResolveAndLog(IDs::Main_OnApplyDismemberment, "Main_OnApplyDismemberment");
		addr_Main_GetTerminalLinkKeyword = ResolveAndLog(IDs::Main_GetTerminalLinkKeyword, "Main_GetTerminalLinkKeyword");

		s_initialized = true;
		return true;
	}

	bool IsInitialized()
	{
		return s_initialized;
	}

	uintptr_t ResolveAddress(uint64_t id)
	{
		if (!g_versionDb.IsLoaded())
			return 0;
		return g_versionDb.FindAddressById(id);
	}
}
