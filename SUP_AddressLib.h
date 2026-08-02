#pragma once

// SUP F4SE Address Library Integration
// This header provides version-independent address resolution using the Address Library database
// Compatible with F4SE 0.7.7 and Fallout 4 1.10.980+ (Next Gen) / 1.11.x (Anniversary Edition)

#include <cstdint>
#include <cstdio>
#include <map>
#include <string>
#include <Windows.h>

// Include common types for UInt32, etc.
#include "common/ITypes.h"
#include "common/IDebugLog.h"
#include "f4se_common/f4se_version.h"
#include "f4se_common/Relocation.h"

// Use the _MESSAGE macro from IDebugLog.h for logging
// It's already linked to gLog via the common library

// VersionDb class for reading Address Library database files
// Based on the Address Library format by meh321
class VersionDb
{
public:
	VersionDb() : m_baseAddr(0), m_loaded(false) {}
	~VersionDb() { Clear(); }

	void Clear()
	{
		m_data.clear();
		m_loaded = false;
	}

	bool Load()
	{
		__try
		{
			// Get game executable version
			char fileName[MAX_PATH];
			memset(fileName, 0, sizeof(fileName));
			UInt32 version = CURRENT_RELEASE_RUNTIME;

			_MESSAGE("Address Library: Loading for runtime version 0x%08X (v%d.%d.%d)",
				version,
				GET_EXE_VERSION_MAJOR(version),
				GET_EXE_VERSION_MINOR(version),
				GET_EXE_VERSION_BUILD(version));

			// Get the absolute path to the game directory from the executable module
			char exePath[MAX_PATH];
			memset(exePath, 0, sizeof(exePath));

			HMODULE hModule = GetModuleHandle(NULL);

			if (hModule)
			{
				DWORD result = GetModuleFileNameA(hModule, exePath, MAX_PATH);

				if (result > 0)
				{
					// Remove the executable filename to get the directory
					char* lastSlash = strrchr(exePath, '\\');
					if (lastSlash)
					{
						*lastSlash = '\0';
					}

					_snprintf_s(fileName, sizeof(fileName),
						"%s\\Data\\F4SE\\Plugins\\version-%d-%d-%d-%d.bin",
						exePath,
						GET_EXE_VERSION_MAJOR(version),
						GET_EXE_VERSION_MINOR(version),
						GET_EXE_VERSION_BUILD(version),
						0);
				}
				else
				{
					_MESSAGE("Address Library: GetModuleFileNameA failed, using relative path");
					_snprintf_s(fileName, sizeof(fileName),
						"Data\\F4SE\\Plugins\\version-%d-%d-%d-%d.bin",
						GET_EXE_VERSION_MAJOR(version),
						GET_EXE_VERSION_MINOR(version),
						GET_EXE_VERSION_BUILD(version),
						0);
				}
			}
			else
			{
				_MESSAGE("Address Library: GetModuleHandle failed, using relative path");
				// Fallback to relative path
				_snprintf_s(fileName, sizeof(fileName),
					"Data\\F4SE\\Plugins\\version-%d-%d-%d-%d.bin",
					GET_EXE_VERSION_MAJOR(version),
					GET_EXE_VERSION_MINOR(version),
					GET_EXE_VERSION_BUILD(version),
					0);
			}

			return LoadFromFile(fileName);
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			_MESSAGE("Address Library: Exception caught in Load()!");
			return false;
		}
	}

	bool LoadFromFile(const char* filePath)
	{
		_MESSAGE("Address Library: Attempting to load database from: %s", filePath);

		FILE* file = nullptr;
		fopen_s(&file, filePath, "rb");
		if (!file)
		{
			_MESSAGE("Address Library: Failed to open file - errno=%d", errno);
			return false;
		}

		// Read header - NG format may have different structure
		int header[4];
		if (fread(header, sizeof(int), 4, file) != 4)
		{
			_MESSAGE("Address Library: Failed to read header");
			fclose(file);
			return false;
		}

		_MESSAGE("Address Library: Header bytes: %d %d %d %d", header[0], header[1], header[2], header[3]);

		int format = 0;
		int addrCount = 0;

		// Try to detect format:
		// Old format: first int is format (1 or 2)
		// NG format might have address count first
		if (header[0] == 1 || header[0] == 2)
		{
			format = header[0];
			_MESSAGE("Address Library: Detected old format version: %d", format);
		}
		else
		{
			// NG Address Library format (F4SE 0.7.x / 1.10.980+)
			// Format: 8-byte count, then pairs of (8-byte ID, 8-byte offset)
			_MESSAGE("Address Library: Trying NG simple format...");

			// Re-read count as 8-byte value from start of file
			fseek(file, 0, SEEK_SET);
			uint64_t count64;
			if (fread(&count64, sizeof(count64), 1, file) != 1)
			{
				_MESSAGE("Address Library: Failed to read count");
				fclose(file);
				return false;
			}

			addrCount = static_cast<int>(count64);
			_MESSAGE("Address Library: NG format - count=%d", addrCount);

			m_data.clear();

			// Read (ID, offset) pairs - each is 16 bytes total
			for (int i = 0; i < addrCount; i++)
			{
				uint64_t id, offset;
				if (fread(&id, sizeof(id), 1, file) != 1 ||
					fread(&offset, sizeof(offset), 1, file) != 1)
				{
					_MESSAGE("Address Library: Failed to read entry %d", i);
					break;
				}

				// Offset is stored directly (no scaling needed)
				m_data[id] = offset;
			}

			fclose(file);
			m_baseAddr = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));
			m_loaded = true;
			_MESSAGE("Address Library: Successfully loaded %zu addresses (NG format), base addr: 0x%llX", m_data.size(), m_baseAddr);
			return true;
		}

		// Old format - continue with version info
		// Read version info
		int verMajor, verMinor, verBuild, verSub;
		fread(&verMajor, sizeof(verMajor), 1, file);
		fread(&verMinor, sizeof(verMinor), 1, file);
		fread(&verBuild, sizeof(verBuild), 1, file);
		fread(&verSub, sizeof(verSub), 1, file);

		// Read module name length and name
		int nameLen;
		fread(&nameLen, sizeof(nameLen), 1, file);

		char* name = new char[nameLen + 1];
		fread(name, 1, nameLen, file);
		name[nameLen] = 0;
		delete[] name;

		// Read pointer size (should be 8 for 64-bit)
		int ptrSize;
		fread(&ptrSize, sizeof(ptrSize), 1, file);

		// Read address count
		fread(&addrCount, sizeof(addrCount), 1, file);

		// Read addresses using delta encoding
		m_data.clear();

		uint64_t prevId = 0;
		uint64_t prevOffset = 0;

		for (int i = 0; i < addrCount; i++)
		{
			uint8_t type;
			fread(&type, sizeof(type), 1, file);

			uint8_t lo = type & 0xF;
			uint8_t hi = type >> 4;

			uint64_t id, offset;

			// Decode ID
			switch (lo)
			{
			case 0: { uint64_t v; fread(&v, sizeof(v), 1, file); id = v; } break;
			case 1: id = prevId + 1; break;
			case 2: { uint8_t v; fread(&v, sizeof(v), 1, file); id = prevId + v; } break;
			case 3: { int8_t v; fread(&v, sizeof(v), 1, file); id = prevId - v; } break;
			case 4: { uint16_t v; fread(&v, sizeof(v), 1, file); id = prevId + v; } break;
			case 5: { int16_t v; fread(&v, sizeof(v), 1, file); id = prevId - v; } break;
			case 6: { uint16_t v; fread(&v, sizeof(v), 1, file); id = v; } break;
			case 7: { uint32_t v; fread(&v, sizeof(v), 1, file); id = v; } break;
			default: id = 0; break;
			}

			// Decode offset
			switch (hi)
			{
			case 0: { uint64_t v; fread(&v, sizeof(v), 1, file); offset = v; } break;
			case 1: offset = prevOffset + 1; break;
			case 2: { uint8_t v; fread(&v, sizeof(v), 1, file); offset = prevOffset + v; } break;
			case 3: { int8_t v; fread(&v, sizeof(v), 1, file); offset = prevOffset - v; } break;
			case 4: { uint16_t v; fread(&v, sizeof(v), 1, file); offset = prevOffset + v; } break;
			case 5: { int16_t v; fread(&v, sizeof(v), 1, file); offset = prevOffset - v; } break;
			case 6: { uint16_t v; fread(&v, sizeof(v), 1, file); offset = v; } break;
			case 7: { uint32_t v; fread(&v, sizeof(v), 1, file); offset = v; } break;
			default: offset = 0; break;
			}

			if (format == 2)
			{
				// Format 2 uses relative virtual addresses
				offset *= 16;  // Addresses are stored as offset / 16
			}

			m_data[id] = offset;
			prevId = id;
			prevOffset = offset;
		}

		fclose(file);
		m_baseAddr = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));
		m_loaded = true;
		_MESSAGE("Address Library: Successfully loaded %zu addresses, base addr: 0x%llX", m_data.size(), m_baseAddr);
		return true;
	}

	bool FindOffsetById(uint64_t id, uint64_t& result) const
	{
		auto it = m_data.find(id);
		if (it != m_data.end())
		{
			result = it->second;
			return true;
		}
		return false;
	}

	uintptr_t FindAddressById(uint64_t id) const
	{
		auto it = m_data.find(id);
		if (it != m_data.end())
		{
			return m_baseAddr + it->second;
		}
		return 0;
	}

	bool IsLoaded() const { return m_loaded; }
	uintptr_t GetBaseAddr() const { return m_baseAddr; }

private:
	std::map<uint64_t, uint64_t> m_data;
	uintptr_t m_baseAddr;
	bool m_loaded;
};

// Global Address Library database instance
extern VersionDb g_versionDb;

// SUP Address Library namespace - contains all resolved addresses
namespace SUP_AddressLib
{
	// Address IDs from Address Library database
	// NOTE: These IDs need to be looked up from the Address Library database
	// The database maps function names/signatures to IDs, which are then used
	// to look up the correct offset for each game version
	//
	// To find the correct IDs:
	// 1. Use the AddressLibDecoder tool from CommonLibF4
	// 2. Or dump the database and search for the function by name/offset
	// 3. Or use the online Address Library database viewer

	namespace IDs
	{
		// NG Address Library IDs for Fallout 4 v1.11.191 (Next Gen)
		// CORRECTED 2025-12-21 by fuzzy-matching offsets to NG database

		// Workshop/Power Grid functions
		constexpr uint64_t REFR_HasKeywordHelper = 2196779;              // 0x4120B0 -> 0x412090
		constexpr uint64_t WorkShopExtra_RemoveItem = 4472849;           // 0x1F69C0 -> 0x1F6A70
		constexpr uint64_t WorkShopExtra_AddGridConnection = 4472860;    // 0x1F6E20 -> 0x1F6CF0 (nearest match)
		constexpr uint64_t WorkShopExtra_RemoveGridConnection = 4472871; // 0x1F7440 -> 0x1F7430
		constexpr uint64_t WorkShopExtra_AreDirectlyConnected = 4472881; // 0x1F77D0 -> 0x1F7780
		constexpr uint64_t WorkShopExtra_CalculateCapacityAndLoad = 4475969; // 0x240160 -> 0x240140
		constexpr uint64_t WorkShopExtra_GetPowerGridForTarget = 4472882; // 0x1F7950 -> 0x1F7A50 (nearest match)
		constexpr uint64_t WorkShopExtra_CleanUpEmptyPowerGrids = 4472898; // 0x1F8550 -> 0x1F84B0
		constexpr uint64_t WorkShop_DeleteWorkshopItem = 4808450;        // 0x207A30 -> 0x207A20
		constexpr uint64_t WorkShop_DisconnectSpline = 4473497;          // 0x200F50 -> 0x200F60
		constexpr uint64_t Workshop_ItemInterruptsPower = 4473103;       // 0x1FC360 (exact match)
		constexpr uint64_t Workshop_ItemIsWirelessPowerReceiver = 4473100; // 0x1FC140 -> 0x1FC0D0
		constexpr uint64_t Workshop_IsWithinRadiatorRange = 4473543;     // 0x201930 -> 0x2018F0
		constexpr uint64_t Workshop_IsPowered = 4472881;                 // 0x1F7840 -> 0x1F7780

		// Radio functions
		constexpr uint64_t Main_RadioUpdate = 0; // DISABLED - testing                   // 0xCC3450 -> 0xCC3460
		constexpr uint64_t Main_OnRadioAddStation = 2229305;             // 0xC4BEE0 -> 0xC4BE80

		// Quest functions
		constexpr uint64_t Main_OnQuestComplete = 2204940;               // 0x5D69B0 -> 0x5D69E0
		constexpr uint64_t Main_OnQuestActive = 2204942;                 // 0x5D6AC0 -> 0x5D6A90
		constexpr uint64_t Main_OnQuestFailed = 2204935;                 // 0x5D6690 -> 0x5D66C0

		// Console/Script functions
		constexpr uint64_t Main_OnConsoleCommand = 4814165;              // 0x125B4A0 -> 0x125B4A2
		constexpr uint64_t Main_OnCompileScript = 2200532;               // 0x4E2A60 -> 0x4E2A20

		// Map Marker functions
		constexpr uint64_t Main_OnSePlayerMarker = 2238349;              // 0xEA7220 -> 0xEA72D0
		constexpr uint64_t Main_OnRemovePlayerMapMarker = 0;             // DISABLED - same addr       // 0xEA7250 -> 0xEA72D0

		// Combat/Physics functions
		constexpr uint64_t Main_OnKnockExplosion = 2237514;              // 0xE73F40 -> 0xE73F00
		constexpr uint64_t Main_OnGetUpFromKnock = 2232921;              // 0xD59F00 -> 0xD59F40

		// Radio state functions
		constexpr uint64_t Main_OnPlayerRadioState = 2230851;            // 0xCC1B90 -> 0xCC1C60
		constexpr uint64_t Main_IsPlayerRadioEnabled = 2230851;          // 0xCC1CF0 -> 0xCC1C60

		// Cell/Reference functions
		constexpr uint64_t Main_UpdateSeenDataForPlayerLoc = 2195601;    // 0x3C10F0 -> 0x3C1170
		constexpr uint64_t Main_OnSetWantsDelete = 2196766;              // 0x4115C0 -> 0x4115B0
		constexpr uint64_t TaskInterface_ProcessWorkshopSwitchEvent = 2232980; // 0xD5F310 -> 0xD5EFF0 (nearest match)

		// Actor functions
		constexpr uint64_t Actor_Jump = 2233155;                         // 0xD73770 -> 0xD73620 (nearest match)
		constexpr uint64_t Actor_IsInAir = 2233999;                      // 0xDA8AD0 -> 0xDA8C50 (nearest match)
		constexpr uint64_t Actor_IsDead = 2233624;                       // 0xD89D40 -> 0xD89D50
		constexpr uint64_t Actor_CanBeKnockedDown = 2233626;             // 0xD89DD0 (exact match)

		// Utility functions
		constexpr uint64_t Main_GetFormByEditorID = 2179550;             // 0x152EB0 -> 0x152E80
		constexpr uint64_t Main_GetRadioStationName = 2230850;           // 0xCC1950 -> 0xCC19D0
		constexpr uint64_t Address_TimeMultiplier = 0;                   // NOT FOUND in NG - disabled
		constexpr uint64_t Main_GetActorByHandle = 2168321;              // 0x23870 -> 0x23780
		constexpr uint64_t Main_OnApplyDismemberment = 2191382;          // 0x2B04A0 -> 0x2B0530
		constexpr uint64_t Main_GetTerminalLinkKeyword = 4473787;        // 0x204400 (exact match)
	}

	// Resolved addresses - initialized at plugin load
	extern uintptr_t addr_REFR_HasKeywordHelper;
	extern uintptr_t addr_WorkShopExtra_RemoveItem;
	extern uintptr_t addr_WorkShopExtra_AddGridConnection;
	extern uintptr_t addr_WorkShopExtra_RemoveGridConnection;
	extern uintptr_t addr_WorkShopExtra_AreDirectlyConnected;
	extern uintptr_t addr_WorkShopExtra_CalculateCapacityAndLoad;
	extern uintptr_t addr_WorkShopExtra_GetPowerGridForTarget;
	extern uintptr_t addr_WorkShopExtra_CleanUpEmptyPowerGrids;
	extern uintptr_t addr_WorkShop_DeleteWorkshopItem;
	extern uintptr_t addr_WorkShop_DisconnectSpline;
	extern uintptr_t addr_Workshop_ItemInterruptsPower;
	extern uintptr_t addr_Workshop_ItemIsWirelessPowerReceiver;
	extern uintptr_t addr_Workshop_IsWithinRadiatorRange;
	extern uintptr_t addr_Workshop_IsPowered;
	extern uintptr_t addr_Main_RadioUpdate;
	extern uintptr_t addr_Main_OnRadioAddStation;
	extern uintptr_t addr_Main_OnQuestComplete;
	extern uintptr_t addr_Main_OnQuestActive;
	extern uintptr_t addr_Main_OnQuestFailed;
	extern uintptr_t addr_Main_OnConsoleCommand;
	extern uintptr_t addr_Main_OnCompileScript;
	extern uintptr_t addr_Main_OnSePlayerMarker;
	extern uintptr_t addr_Main_OnRemovePlayerMapMarker;
	extern uintptr_t addr_Main_OnKnockExplosion;
	extern uintptr_t addr_Main_OnGetUpFromKnock;
	extern uintptr_t addr_Main_OnPlayerRadioState;
	extern uintptr_t addr_Main_IsPlayerRadioEnabled;
	extern uintptr_t addr_Main_UpdateSeenDataForPlayerLoc;
	extern uintptr_t addr_Main_OnSetWantsDelete;
	extern uintptr_t addr_TaskInterface_ProcessWorkshopSwitchEvent;
	extern uintptr_t addr_Actor_Jump;
	extern uintptr_t addr_Actor_IsInAir;
	extern uintptr_t addr_Actor_IsDead;
	extern uintptr_t addr_Actor_CanBeKnockedDown;
	extern uintptr_t addr_Main_GetFormByEditorID;
	extern uintptr_t addr_Main_GetRadioStationName;
	extern uintptr_t addr_Address_TimeMultiplier;
	extern uintptr_t addr_Main_GetActorByHandle;
	extern uintptr_t addr_Main_OnApplyDismemberment;
	extern uintptr_t addr_Main_GetTerminalLinkKeyword;

	// Initialize all addresses from Address Library
	// Call this once during plugin load (F4SEPlugin_Load)
	// Returns true if successful, false if Address Library database could not be loaded
	bool Initialize();

	// Check if addresses are initialized
	bool IsInitialized();

	// Helper to resolve an address by ID (for custom lookups)
	uintptr_t ResolveAddress(uint64_t id);
}

// Backwards compatibility macros
// These redirect the old #define addresses to the new resolved addresses
// Eventually, code should be updated to use SUP_AddressLib::addr_* directly

#define fn_REFR_HasKeywordHelper SUP_AddressLib::addr_REFR_HasKeywordHelper
#define fn_WorkShopExtra_RemoveItem SUP_AddressLib::addr_WorkShopExtra_RemoveItem
#define fn_WorkShopExtra_AddGridConnection SUP_AddressLib::addr_WorkShopExtra_AddGridConnection
#define fn_WorkShopExtra_RemoveGridConnection SUP_AddressLib::addr_WorkShopExtra_RemoveGridConnection
#define fn_WorkShopExtra_AreDirectlyConnected SUP_AddressLib::addr_WorkShopExtra_AreDirectlyConnected
#define fn_WorkShopExtra_CalculateCapacityAndLoad SUP_AddressLib::addr_WorkShopExtra_CalculateCapacityAndLoad
#define fn_WorkShopExtra_GetPowerGridForTarget SUP_AddressLib::addr_WorkShopExtra_GetPowerGridForTarget
#define fn_WorkShopExtra_CleanUpEmptyPowerGrids SUP_AddressLib::addr_WorkShopExtra_CleanUpEmptyPowerGrids
#define fn_WorkShop_DeleteWorkshopItem SUP_AddressLib::addr_WorkShop_DeleteWorkshopItem
#define fn_WorkShop_DisconnectSpline SUP_AddressLib::addr_WorkShop_DisconnectSpline
#define fn_Workshop_ItemInterruptsPower SUP_AddressLib::addr_Workshop_ItemInterruptsPower
#define fn_Workshop_ItemIsWirelessPowerReceiver SUP_AddressLib::addr_Workshop_ItemIsWirelessPowerReceiver
#define fn_Workshop_IsWithinRadiatorRange SUP_AddressLib::addr_Workshop_IsWithinRadiatorRange
#define fn_Workshop_IsPowered SUP_AddressLib::addr_Workshop_IsPowered
#define fn_Main_RadioUpdate SUP_AddressLib::addr_Main_RadioUpdate
#define fn_Main_OnRadioAddStation SUP_AddressLib::addr_Main_OnRadioAddStation
#define fn_Main_OnQuestComplete SUP_AddressLib::addr_Main_OnQuestComplete
#define fn_Main_OnQuestActive SUP_AddressLib::addr_Main_OnQuestActive
#define fn_Main_OnQuestFailed SUP_AddressLib::addr_Main_OnQuestFailed
#define fn_Main_OnConsoleCommand SUP_AddressLib::addr_Main_OnConsoleCommand
#define fn_Main_OnCompileScript SUP_AddressLib::addr_Main_OnCompileScript
#define fn_Main_OnSePlayerMarker SUP_AddressLib::addr_Main_OnSePlayerMarker
#define fn_Main_OnRemovePlayerMapMarker SUP_AddressLib::addr_Main_OnRemovePlayerMapMarker
#define fn_Main_OnKnockExplosion SUP_AddressLib::addr_Main_OnKnockExplosion
#define fn_Main_OnGetUpFromKnock SUP_AddressLib::addr_Main_OnGetUpFromKnock
#define fn_Main_OnPlayerRadioState SUP_AddressLib::addr_Main_OnPlayerRadioState
#define fn_Main_IsPlayerRadioEnabled SUP_AddressLib::addr_Main_IsPlayerRadioEnabled
#define fn_Main_UpdateSeenDataForPlayerLoc SUP_AddressLib::addr_Main_UpdateSeenDataForPlayerLoc
#define fn_Main_OnSetWantsDelete SUP_AddressLib::addr_Main_OnSetWantsDelete
#define fn_TaskInterface_ProcessWorkshopSwitchEvent SUP_AddressLib::addr_TaskInterface_ProcessWorkshopSwitchEvent
#define fn_Actor_Jump SUP_AddressLib::addr_Actor_Jump
#define fn_Actor_IsInAir SUP_AddressLib::addr_Actor_IsInAir
#define fn_Actor_IsDead SUP_AddressLib::addr_Actor_IsDead
#define fn_Actor_CanBeKnockedDown SUP_AddressLib::addr_Actor_CanBeKnockedDown
#define fn_Main_GetFormByEditorID SUP_AddressLib::addr_Main_GetFormByEditorID
#define fn_Main_GetRadioStationName SUP_AddressLib::addr_Main_GetRadioStationName
#define fn_Address_TimeMultiplier SUP_AddressLib::addr_Address_TimeMultiplier
#define fn_Main_GetActorByHandle SUP_AddressLib::addr_Main_GetActorByHandle
#define fn_Main_OnApplyDismemberment SUP_AddressLib::addr_Main_OnApplyDismemberment
#define fn_Main_GetTerminalLinkKeyword SUP_AddressLib::addr_Main_GetTerminalLinkKeyword
