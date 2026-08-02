#pragma comment(lib, "C:\\SUP_F4SEAE\\common\\build\\Release\\common.lib")
#pragma comment(lib, "C:\\SUP_F4SEAE\\f4se\\build\\f4se\\f4se_common\\Release\\f4se_common.lib")
#pragma comment(lib, "C:\\SUP_F4SEAE\\f4se\\build\\f4se\\Release\\f4se_1_11_221.lib")
//#define NOMINMAX

// Standard library includes needed by common library
#include <cstdio>

// Common library includes - must come first for type definitions
#include "common/IDebugLog.h"
#include "common/ITypes.h"

// F4SE 0.7.8 compatibility - include Address Library support
#include "SUP_AddressLib.h"
#include "Tomm_Addresses.h"

#define BBCheckPluginHash



int iVersion = 78;  // Version locked to F4SE 0.7.8
//#define DO_ADDITIONAL_LOG

#if defined DO_ADDITIONAL_LOG
#define _DMESSAGEADD(...) if (bDebugMode) { _DMESSAGESUP(__VA_ARGS__);};
#else
#define _DMESSAGEADD(...)  };
#endif


#include "f4se_common/f4se_version.h"
#include "f4se_common/Utilities.h" 
#include "f4se_common/Relocation.h"
#include "f4se_common/BranchTrampoline.h"
#include "f4se_common/SafeWrite.h"
#include <cassert>
#include <cstring> 
#include <shlobj.h>
#include <vector>
#include "common/IFileStream.h" 
#include "Hooks_ObScript.h"
#include "Hooks_Papyrus.h"
#include "Hooks_Scaleform.h"
#include "Hooks_Gameplay.h"
#include "Hooks_Memory.h"
#include "Hooks_GameData.h"
#include "Hooks_SaveLoad.h"
#include "Hooks_Input.h"
#include "Hooks_Debug.h"
#include "Hooks_Threads.h"
#include "Hooks_Camera.h"
#include "PluginManager.h"
#include "InternalSerialization.h"
#include <GameSettings.h>
#include "GameCamera.h"
#include "PapyrusVM.h"
#include <filesystem>
#include <thread>
#include <dshow.h>  // Include before using namespace std to avoid byte conflict
using namespace std;

#include "f4se/PluginAPI.h"
#include "f4se/PapyrusNativeFunctions.h"
#include <f4se/GameReferences.h>
#include "f4se/GameMenus.h"
#include "F4SE_Compat.h"  // Must be before GameObjects.h for TESFaction definition
#include <GameObjects.h>
#include "f4se/GameData.h"
#include <codecvt>
#include <tlhelp32.h>
#include <tchar.h>
#include <fstream>
#include <iostream>
#include "strsafe.h"

#include <regex>
#include "sha1.hpp"
#include "f4se/PapyrusEvents.h"
#include <GameExtraData.h>

#include "Decoding.h"
#include <GameStreams.h>
#include "NiNodes.h"
#include <ObScript.h>
#include "GameThreads.h"
#include <GameWorkshop.h>
#include <PapyrusScaleformAdapter.h>

#include "json.hpp"
#include "SimpleIni.h"
#include "NanoTimer.h"
#include <GameRTTI.h>
#include <boost/algorithm/string.hpp>

#include <sstream>
#include <fstream>
#include <exception>
#include <NiExtraData.h>
#include "Tomm_FunctionDeclarations.h"





#define Iter (*it)
#define Iter2 (*it2)
#define Iter3 (*it3)
#define Iter4 (*it4)
#define Iter5 (*it5)
#define Iter6 (*it6)
#define INPUTDEVICEMANAGER (*g_inputDeviceMgr)

#define FoundHashItem (*FoundItem)





char pluginName[] = { "SUP_F4SE" };
PluginHandle SUP_F4SE_Handle = kPluginHandle_Invalid;

#include "Tomm_PexFileSHA1.h"


const F4SEInterface* g_f4se = NULL;
F4SEPapyrusInterface* g_papyrus = NULL;
F4SEMessagingInterface* g_messaging = NULL;
F4SETaskInterface* g_taskInterface = NULL;
F4SETrampolineInterface* g_Trampoline = NULL;
F4SESerializationInterface* g_Serialization = NULL;




//GLOBALS


HWND g_HWND = NULL;
int bCapturedHWND = false;
bool bResolutionCaptured = false;
int iScreenWidth = 0;
int iScreenHeight = 0;

string g_FalloutFolderPath{};


bool bCleanUpDeletedReferenceFromPowerArrayFix = true;



bool bDebugMode = false;
bool bSkipPexCheck = false;
bool bScrapCrashFix = true;
bool bWirelessPowerFix = true;
bool bTermLinkFix = true;
bool bDisableAllHooks = false;

bool bEditorIDPatchLoaded = false;

string s_IniPath = "Data\\F4SE/Plugins\\SUP_F4SE.ini";
string s_PexPath = "Data\\Scripts\\SUP_F4SE.pex";
string s_ConfigPath = "Data\\Config\\";
string s_ScriptRunnerPath = "Data\\Scripts\\SUPScriptRunner";

VirtualMachine* g_MyVirtMachine = NULL;
IObjectHandlePolicy* g_ObjHandlerPolicy = NULL;

const char* FO4_SavegamePath = "\\My Games\\Fallout4\\";
char* g_SaveTempName = NULL;
string g_LastLoadedSaveName{};
bool g_LastLoadedSaveGameNameValid = false;
bool g_LastSaveLoadedMainMenuWasOpen = false;
bool g_LastSaveLoadedMainMenuWasOpenTemp = false;
bool bPexVersionWrong = false;

BGSKeyword* g_SUPWorkshopKeyword = nullptr;
BGSKeyword* g_WorkshopPowerConnection = nullptr;
ActorValueInfo* g_RadiationActorValue = NULL;




vector<float> vCurrentRadioFrequencies{};


#define _DMESSAGE(...) if (bDebugMode) { _DMESSAGESUP(__VA_ARGS__);};










inline void _DMESSAGESUP(const char* fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	gLog.Log(IDebugLog::kLevel_DebugMessage, fmt, args);
	va_end(args);
}

bool bSUPdaterLoaded = false;
bool b_SUPDater_LoadingDone = false;
int i_SUPDater_ShowMessage = 0; // 0 - not shown, 1 - Requested to Show, 2 - shown.
string s_SUPDater_MessageToShow{};


#include <windows.h>
#include <wininet.h>

#include <string>
#include <stdio.h>
#include <stdlib.h>


#pragma comment ( lib, "Wininet.lib" )
#include "InternalFunctions.h"
#include "AttachedScriptsReader.h"

#include "inputbox.h"

#include "Tomm_INIProcessing.h"



bool fn_ext_IsSUPPatchActive(int iRequest)
{
	if (iRequest == 1)
	{
		return bScrapCrashFix;
	}
	else if (iRequest == 2)
	{
		return bWirelessPowerFix;
	}
	else if (iRequest == 3)
	{
		return bTermLinkFix;
	}


	
	return false;
}


DECLARE_STRUCT(UserTimeStruct, "SUP_F4SE")
DECLARE_STRUCT(GetCalculatedPosRes, "SUP_F4SE")
DECLARE_STRUCT(FileDialogueBoxResult, "SUP_F4SE")
DECLARE_STRUCT(ModLocalDataElement, "SUP_F4SE")
DECLARE_STRUCT(NodeTranslation, "SUP_F4SE")
DECLARE_STRUCT(NodeRotation, "SUP_F4SE")
DECLARE_STRUCT(FileVersion, "SUP_F4SE")
DECLARE_STRUCT(PowerGridInstance, "SUP_F4SE")
DECLARE_STRUCT(JSONValue, "SUP_F4SE")
DECLARE_STRUCT(ConnectPointSUP, "SUP_F4SE")
DECLARE_STRUCT(ScreenPos, "SUP_F4SE")
DECLARE_STRUCT(RadioData, "SUP_F4SE")
DECLARE_STRUCT(SUPFurnitureMarkerData, "SUP_F4SE")
DECLARE_STRUCT(SUPConnectedObject, "SUP_F4SE")
DECLARE_STRUCT(SUPXYZ, "SUP_F4SE")
DECLARE_STRUCT(GameSettingValue, "SUP_F4SE")
DECLARE_STRUCT(GridConnection, "SUP_F4SE")
DECLARE_STRUCT(SUPReferenceInfo, "SUP_F4SE")
DECLARE_STRUCT(GridConnectionEx, "SUP_F4SE")





#include "Tomm_SUPDater_Funcs.h"
#include "Tomm_Events.h"
#include "Tomm_ConsoleCommands.h"

#include "Tomm_EventScriptCaller.h"
#include "Tomm_Gameloop.h"
#include "MP3_Player.h"

#include "Tomm_SteamAPI.h"
#include "Tomm_SUPPastebinReader.h"
#include "Tomm_BuildingBridges.h"

#include "Tomm_fn_ModLocalData.h"
#include "Tomm_fn_Utility.h"
#include "Tomm_fn_Screenshots.h"
#include "Tomm_fn_Math.h"
#include "Tomm_fn_INI.h"
#include "Tomm_fn_FileFunctions.h"
#include "Tomm_fn_Strings.h"
#include "Tomm_fn_Misc.h"


#include "Tomm_fn_Dialogue_Box.h"
#include "Tomm_fn_MP3.h"
#include "Tomm_fn_Nif.h"
#include "Tomm_fn_Excel.h"
#include "Tomm_fn_Online.h"
#include "Tomm_fn_ObjectReference.h"
#include "Tomm_fn_Cell.h"
//#include "Tomm_fn_Controller.h"
#include "Tomm_fn_Steam.h"
#include "Tomm_fn_BuildingBridges.h"
#include "Tomm_fn_JSON.h"
#include "Tomm_fn_XML.h"
#include "Tomm_fn_Events.h"
#include "Tomm_fn_Recipies.h"
#include "Tomm_fn_PowerGrid.h"
#include "Tomm_fn_Location.h"
#include "Tomm_fn_MapMarker.h"

#include "Tomm_RegisterFunctions.h"
#include "Tomm_Hooks.h"





void F_SUP_TEST_FUNCTION()
{
}

#include "Tomm_NativeEvents.h"


bool IsGetEditorIDPatchLoaded()
{
	if (GetModuleHandleA("BakaFramework.dll"))
	{
		if (f_Does_FileExist("Data\\F4SE\\Plugins\\BakaFramework.toml"))
		{
			ifstream aInputFile;
			aInputFile.open("Data\\F4SE\\Plugins\\BakaFramework.toml");
			string str{};
			while (getline(aInputFile, str))
			{
				boost::to_lower(str);
				if (str == "enableloadingeditorids = true")
				{
					return true;
				}
			}
		}
	}
	return false;
}


typedef int(__fastcall* OnBSPrint)(char* a1, size_t a2, const char* a3, uint64 pad);
RelocAddr <OnBSPrint> OnBSPrint_Hook(0x274B2F0);

int OnBSPrint_MyHook(char* a1, size_t a2, const char* a3, uint64 pad)
{
	return 0;
}



void f_SUP_ONDefferedInit()
{
	float fVersion = iVersion * 0.01;
	Console_Print("SUP F4SE V.%0.2lf", fVersion);

	BSFixedString MenuName("Console");
	CALL_MEMBER_FN(*g_uiMessageManager, SendUIMessage)(MenuName, kMessage_Open);
	CALL_MEMBER_FN(*g_uiMessageManager, SendUIMessage)(MenuName, kMessage_Close);

	g_ObjHandlerPolicy = g_MyVirtMachine->GetHandlePolicy(); //permanent address
		
	f_UpdateChecker_InitSUPDater();
	f_LoadSteam();
	EventRunner::CheckAndGiveHandleToLocalFunctions();
	//AttachedScriptReader::DumpALLScripts();
	//F_SUP_TEST_FUNCTION();
	f_BB_OnDeterredInit();
	BB_FilesHashMap.clear();
	if (g_BB_ShowWarningOnGameLoad == 1)
	{
		Console_Print("%s", g_BB_WarningMessageOnGameLoad.c_str());
	}


	g_packedArgs = new VMValue;
	g_packedArgs->type.value = VMValue::kType_None;
	g_packedArgs->SetNone();

	f_GameLoop_OnDeferredInit();
	if (!bDisableAllHooks) {
		f_InitHooks();
	} else {
		_MESSAGE("All hooks disabled via INI setting");
	}


	BGSDefaultObject* workshopItemDefault = (*g_defaultObjectMap)->GetDefaultObject("WorkshopItem");
	if (workshopItemDefault) {
		g_SUPWorkshopKeyword = DYNAMIC_CAST(workshopItemDefault->form, TESForm, BGSKeyword);
		_DMESSAGE("WorkshopItem keyword found");
	}
	else {
		_MESSAGE("WARNING::WorkshopItem keyword NOT found");
	}



	//g_WorkshopPowerConnection = (BGSKeyword*)LookupFormByID(347044);


	//if (g_WorkshopPowerConnection)
	//{
	//	_DMESSAGE("WorkshopPowerConnection keyword keyword found");
	//}
	//else {
	//	_DMESSAGE("WorkshopPowerConnection keyword keyword  NOT found");
	//}



	g_RadiationActorValue = (ActorValueInfo*)LookupFormByID(815);

	if (g_RadiationActorValue)
	{
		_DMESSAGE("RadiationActorValue keyword found");
	}
	else {
		_MESSAGE("WARNING::RadiationActorValue NOT found");
	}

	//GetEventDispatcher<TESPlacementEvent>()->AddEventSink(&g_SUPPLacementEvent);
	//GetEventDispatcher<TESActivateEvent>()->AddEventSink(&g_SUPActivateEvent);  // TermLinkFix - disabled, needs correct NG address


	
	//GetEventDispatcher<TESPowerOnEvent>()->AddEventSink(&g_SUPPowerOnEvent);
	//GetEventDispatcher<TESPowerOffEvent>()->AddEventSink(&g_SUPPowerOffEvent);

	//bEditorIDPatchLoaded = IsGetEditorIDPatchLoaded();
	//_MESSAGE("bEditorIDPatchLoaded>>%d", bEditorIDPatchLoaded);
	//auto TestForm = GetFormByEditorID("WorkshopPowerConnectionDUPLICATE000"); //WorkshopPowerConnectionDUPLICATE000  HoldupKeyword

	//if (TestForm)
	//{
	//	Console_Print("FORM FOUND!");
	//}
	//else {
	//	Console_Print("FORM NOT FOUND!");
	//}
	
	//GetEventDispatcher<TESOnPowerOffEvent>()->AddEventSink(&g_SUPPowerOffEventSink);
	//GetEventDispatcher<TESOnPowerOnEvent>()->AddEventSink(&g_SUPPowerOnEventSink);

}


void SetNativeEventsOnSaveLoad()
{
	CrosshairChangeHandler_SUP::Register();
}



void MessageCallback(F4SEMessagingInterface::Message* msg)
{
	switch (msg->type)
	{
	case F4SEMessagingInterface::kMessage_GameLoaded:
		break;


	case F4SEMessagingInterface::kMessage_PostSaveGame:

		if (OnGameSaveEventHandler.HasEvents())
		{
			OnGameSaveEventHandler.EnterLock();
			OnGameSaveEventHandler.RunEvents(NULL);
			OnGameSaveEventHandler.LeaveLock();
		}
		break;


	case F4SEMessagingInterface::kMessage_PreLoadGame:

		g_SaveTempName = NULL;
		g_SaveTempName = (char*)msg->data;

		if (g_SaveTempName)
		{
			g_LastLoadedSaveGameNameValid = true;
			g_LastLoadedSaveName = g_SaveTempName;
			_DMESSAGE("Latest save name is %s ", g_LastLoadedSaveName.c_str());
		}
		else {
			g_LastLoadedSaveGameNameValid = false;
		}


		if ((*g_ui)->IsMenuOpen("MainMenu"))
		{
			g_LastSaveLoadedMainMenuWasOpenTemp = true;
			//Console_Print("Main menu is open");
		}
		else {
			g_LastSaveLoadedMainMenuWasOpenTemp = false;
			//Console_Print("Main menu is NOT open");
		}


		break;


	case F4SEMessagingInterface::kMessage_PostLoadGame:
		if (!bDisableAllHooks) SetNativeEventsOnSaveLoad();
		if (!msg->data)
			return;

		if (!(bool)msg->data)
			return;

		g_LastSaveLoadedMainMenuWasOpen = g_LastSaveLoadedMainMenuWasOpenTemp;

		vCurrentRadioFrequencies.clear();
		b_SUPDater_LoadingDone = true;


		if (i_SUPDater_ShowMessage == 1)
		{
			_DMESSAGE("SUPDATER::Showing message on game load");
			i_SUPDater_ShowMessage = 2;
			f_UpdateChecker_ShowMessage(s_SUPDater_MessageToShow, 10);
		}

		i_PastebinReader_LoadingDone = 1;
		if (i_PastebinReader_CallFunctions == 1)
		{
			_DMESSAGE("PastebinReader::Calling functions on game load");
			i_PastebinReader_CallFunctions = 2;
			PastebinReader::CallFunctions();
		}

		
		f_Gameloop_OnGameLoad();
		f_BB_LoadFunctionFromMessageHandler(0);

		//CrosshairChangeHandler_LL::Register();


		


		//F_SUP_TEST_FUNCTION();

		//OnGameLoadTest();
		break;

	case F4SEMessagingInterface::kMessage_NewGame:
		if (!bDisableAllHooks) SetNativeEventsOnSaveLoad();
		f_Gameloop_OnGameLoad();
		EventRunner::RunOnSaveLoad();
		vCurrentRadioFrequencies.clear();

		i_PastebinReader_LoadingDone = 1;
		if (i_PastebinReader_CallFunctions == 1)
		{
			_DMESSAGE("PastebinReader::Calling functions on game load");
			i_PastebinReader_CallFunctions = 2;
			PastebinReader::CallFunctions();
		}

		g_BBLastNewGame = 1;
		g_BBLastNewGameBBExecuted = false;
		break;

	case F4SEMessagingInterface::kMessage_GameDataReady:
		f_SUP_ONDefferedInit();
		break;

	default:
		break;
	}

}













// F4SE 0.7.8+ uses declarative version checking - no code execution during version check
// This structure is read by F4SE to determine compatibility before loading the plugin
extern "C" {
__declspec(dllexport) F4SEPluginVersionData F4SEPlugin_Version =
{
	F4SEPluginVersionData::kVersion,					// dataVersion

	78,													// pluginVersion (0.7.8 - locked to F4SE version)
	"SUP_F4SE",											// plugin name
	"Tomm",												// author name

	// Version independence flags
	// Use kAddressIndependence_Signatures because we load Address Library database ourselves
	// This avoids F4SE's CheckAddressLibrary() which uses relative paths
	F4SEPluginVersionData::kAddressIndependence_Signatures,
	F4SEPluginVersionData::kStructureIndependence_1_11_137Layout,

	// Compatible game versions (zero-terminated list)
	// Scope locked to 1.11.221 (AE) only
	{ RUNTIME_VERSION_1_11_221, 0 },

	0,	// seVersionRequired - works with any F4SE version
	0,	// reservedNonBreaking
	0,	// reservedBreaking
};
}

// Legacy F4SEPlugin_Query kept for older F4SE versions (pre-0.7.0)
// This may be called by older F4SE builds
bool F4SEPlugin_Query(const F4SEInterface* f4se, PluginInfo* info) {
	// populate info structure
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = pluginName;
	info->version = iVersion;

	if (f4se->isEditor) {
		return false;
	}

	// For NG/Anniversary builds, require 1.10.980+
	if (f4se->runtimeVersion < RUNTIME_VERSION_1_10_980) {
		return false;
	}

	return true;
}


#include "Tomm_Serialization.h"



BranchTrampoline trampoline;


bool F4SEPlugin_Load(const F4SEInterface* f4se) {
	// Initialize logging (moved from Query since Query no longer executes in F4SE 0.7.0+)
	gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Fallout4\\F4SE\\SUP_F4SE.log");
	gLog.SetPrintLevel(IDebugLog::kLevel_Error);
	gLog.SetLogLevel(IDebugLog::kLevel_Message);

	float fVersion = iVersion * 0.01;
	_MESSAGE("SUP F4SE V.%0.2lf Loading (F4SE 0.7.8 compatible)", fVersion);

	// Initialize Address Library addresses
	if (!SUP_AddressLib::Initialize()) {
		_MESSAGE("ERROR: Failed to initialize Address Library. Plugin will not function correctly.");
		// Continue loading but features requiring addresses may not work
	}

	g_f4se = f4se;
	SUP_F4SE_Handle = f4se->GetPluginHandle();

	g_papyrus = (F4SEPapyrusInterface*)f4se->QueryInterface(kInterface_Papyrus);
	g_messaging = (F4SEMessagingInterface*)f4se->QueryInterface(kInterface_Messaging);
	//g_taskInterface = (F4SETaskInterface*)f4se->QueryInterface(kInterface_Task);
	g_Serialization = (F4SESerializationInterface*)f4se->QueryInterface(kInterface_Serialization);

	g_Serialization->SetSaveCallback(SUP_F4SE_Handle, SUPCosave::SaveCallback);
	g_Serialization->SetLoadCallback(SUP_F4SE_Handle, SUPCosave::LoadCallback);
	g_Serialization->SetUniqueID(SUP_F4SE_Handle, t_CosaveF4SEPluginID);

	//g_Trampoline = (F4SETrampolineInterface*)f4se->QueryInterface(kInterface_Trampoline);
	//void* Allocated = g_Trampoline->AllocateFromBranchPool(f4se->GetPluginHandle(), 14);
	//trampoline.Create(14, Allocated);
	//trampoline.Write5Call(OnBSPrint_Hook.GetUIntPtr(), (uintptr_t)OnBSPrint_MyHook);

	g_messaging->RegisterListener(SUP_F4SE_Handle, "F4SE", MessageCallback);


	std::filesystem::path GamePath = std::filesystem::current_path();
	g_FalloutFolderPath = GamePath.u8string() + "\\";




	if (g_papyrus->Register(RegisterFuncs))
		_MESSAGE("SUP F4SE Funcs Registered");


	CSimpleIniA ini;


	INIManagement::ProcessINIFile(ini);

	if (bDebugMode)
		gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);


	if (bSkipPexCheck != 1)
	{
		string s_Message{};

		if (f_Does_FileExist(s_PexPath))
		{
			int iPexVersion = -1;
			string SHA1 = SHA1::from_file(s_PexPath);
			_DMESSAGE("SHA1 is %s", SHA1.c_str());

			if (SHA1 != s_CurrentPEXHash)
			{
				bPexVersionWrong = true;
				s_Message = "File <" + s_PexPath + "> is damaged(or edited) or has been recompiled.You should reinstall FULL SUP F4SE package, otherwise mods may not work correctly.";

				if (GetModuleHandleA("usvfs_x64") != NULL || GetModuleHandleA("usvfs_x86") != NULL)
				{
					s_Message += "\nAlso make sure that SUP F4SE package is NOT in game folder and MO2 overwrite folder AT THE SAME TIME.";
				}
				_MESSAGE("PEX VERSION WRONG.");
				MessageBoxA(NULL, s_Message.c_str(), "Message from SUP F4SE", MB_ICONERROR);
			}


		}
		else {
			s_Message = "Required file <" + s_PexPath + "> doesn't exist. You should reinstall FULL SUP F4SE package, otherwise mods may not work correctly.";
			MessageBoxA(NULL, s_Message.c_str(), "Message from SUP F4SE", MB_ICONERROR);
			_MESSAGE("PEX FILE NOT FOUND."); 
		}

	}


	if (f_Does_FileExist("aaSUPF4SEDebugPrint.txt"))
		std::filesystem::remove("aaSUPF4SEDebugPrint.txt");


	EventRunner::PopulateArrayFromFiles();
	PastebinReader::PopulateFromFiles();

	PastebinReader::CheckPluginsForInvalid();

	if (iPastebinPasteCount)
	{
		PastebinReader::ReadPastes();
	}

	f_BB_OnBBPluginLoad(ini);

	float fVersionFinal = iVersion * 0.01;
	_MESSAGE("SUP F4SE loaded V.%0.2lf,bScrapCrashFix>>%d,bWirelessPowerFix>>%d,bTermLinkFix>>%d", fVersionFinal, bScrapCrashFix, bWirelessPowerFix, bTermLinkFix);
	return true;
}






extern "C" __declspec(dllexport) int fn_ext_GETVERSION()
{
	return  iVersion;
}

extern "C" __declspec(dllexport) double fn_ext_SUPDateCheckerGetVersion()
{
	return iVersion;
}


extern "C" __declspec(dllexport) int fn_ext_FixesPermitted(int iVersion, string sFixName)
{
	return 1;
}
