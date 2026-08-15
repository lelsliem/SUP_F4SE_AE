//#define NOMINMAX


// Phase 3: the legacy F4SE headers (f4se/*, f4se_common/*, common/*) are gone — the
// CommonLibF4 compat bridge (include/sup/compat/F4SECompat.h) provides their API surface
// on top of RE:: / REL:: / F4SE::. The huge precompiled F4SE header is no longer needed.
// Phase 1: logging routed to spdlog (see sup/util/Log.h) — _MESSAGE/_DMESSAGE/_ERROR
#include "sup/util/Log.h"
// Phase 2: boost::string helpers replaced by sup::string (see sup/util/string.hpp)
#include "sup/util/string.hpp"
// Phase 3: F4SE → CommonLibF4 compatibility bridge
#include "sup/compat/F4SECompat.h"
// Phase 3: SUP-specific structures not modeled by CommonLibF4 (furniture markers, message boxes, …)
#include "sup/util/Decoding.h"

#include "sup/addresses/Tomm_Addresses.h"

#define BBCheckPluginHash



int iVersion = 1170;


#include <cassert>
#include <cstring> 
#include <shlobj.h>
#include <vector>
#include <filesystem>
#include <thread>
#include <codecvt>
#include <tlhelp32.h>
#include <tchar.h>
#include <fstream>
#include <iostream>
#include <regex>
#include <dshow.h>
#include <sstream>
#include <exception>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#include "third_party/sha1.hpp"
#include "third_party/json.hpp"
#include "third_party/SimpleIni.h"
#include "third_party/NanoTimer.h"

#include "sup/core/Tomm_FunctionDeclarations.h"





#define Iter (*it)
#define Iter2 (*it2)
#define Iter3 (*it3)
#define Iter4 (*it4)
#define Iter5 (*it5)
#define Iter6 (*it6)
#define INPUTDEVICEMANAGER (*g_inputDeviceMgr)

#define FoundHashItem (*FoundItem)





char pluginName[] = { "SUP_F4SE" };

#include "sup/core/Tomm_PexFileSHA1.h"


// Phase 3: the F4SE interface globals (g_f4se, g_papyrus, g_messaging, g_Serialization, ...)
// are gone — the load path in main.cpp fetches interfaces via F4SE::Get*Interface().




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
bool bTermLinkFix = true;

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
#include "sup/util/InternalFunctions.h"
#include "sup/util/AttachedScriptsReader.h"

#include "sup/util/inputbox.h"

#include "sup/core/Tomm_INIProcessing.h"



bool fn_ext_IsSUPPatchActive(int iRequest)
{
	if (iRequest == 1)
	{
		return bScrapCrashFix;
	}
	else if (iRequest == 3)
	{
		return bTermLinkFix;
	}


	
	return false;
}


DECLARE_STRUCT(UserTimeStruct, SUP_F4SE)
DECLARE_STRUCT(GetCalculatedPosRes, SUP_F4SE)
DECLARE_STRUCT(FileDialogueBoxResult, SUP_F4SE)
DECLARE_STRUCT(ModLocalDataElement, SUP_F4SE)
DECLARE_STRUCT(NodeTranslation, SUP_F4SE)
DECLARE_STRUCT(NodeRotation, SUP_F4SE)
DECLARE_STRUCT(FileVersion, SUP_F4SE)
DECLARE_STRUCT(PowerGridInstance, SUP_F4SE)
DECLARE_STRUCT(JSONValue, SUP_F4SE)
DECLARE_STRUCT(ConnectPointSUP, SUP_F4SE)
DECLARE_STRUCT(ScreenPos, SUP_F4SE)
DECLARE_STRUCT(RadioData, SUP_F4SE)
DECLARE_STRUCT(SUPFurnitureMarkerData, SUP_F4SE)
DECLARE_STRUCT(SUPConnectedObject, SUP_F4SE)
DECLARE_STRUCT(SUPXYZ, SUP_F4SE)
DECLARE_STRUCT(GameSettingValue, SUP_F4SE)
DECLARE_STRUCT(GridConnection, SUP_F4SE)
DECLARE_STRUCT(SUPReferenceInfo, SUP_F4SE)





#include "sup/external/Tomm_SUPDater_Funcs.h"
#include "sup/events/Tomm_Events.h"
#include "sup/core/Tomm_ConsoleCommands.h"

#include "sup/events/Tomm_EventScriptCaller.h"
#include "sup/core/Tomm_Gameloop.h"

#include "sup/external/Tomm_SteamAPI.h"
#include "sup/external/Tomm_SUPPastebinReader.h"
#include "sup/buildingbridges/Tomm_BuildingBridges.h"

#include "sup/papyrus/general/Tomm_fn_ModLocalData.h"
#include "sup/papyrus/general/Tomm_fn_Utility.h"
#include "sup/papyrus/media/Tomm_fn_Screenshots.h"
#include "sup/papyrus/general/Tomm_fn_Math.h"
#include "sup/papyrus/general/Tomm_fn_INI.h"
#include "sup/papyrus/general/Tomm_fn_FileFunctions.h"
#include "sup/papyrus/general/Tomm_fn_Strings.h"
#include "sup/papyrus/general/Tomm_fn_Misc.h"


#include "sup/papyrus/media/Tomm_fn_Dialogue_Box.h"
#include "sup/papyrus/world/Tomm_fn_Nif.h"
#include "sup/papyrus/media/Tomm_fn_Excel.h"
#include "sup/papyrus/online/Tomm_fn_Online.h"
#include "sup/papyrus/world/Tomm_fn_ObjectReference.h"
#include "sup/papyrus/world/Tomm_fn_Cell.h"
#include "sup/papyrus/online/Tomm_fn_Steam.h"
#include "sup/buildingbridges/Tomm_fn_BuildingBridges.h"
#include "sup/papyrus/media/Tomm_fn_JSON.h"
#include "sup/papyrus/media/Tomm_fn_XML.h"
#include "sup/papyrus/events/Tomm_fn_Events.h"
#include "sup/papyrus/world/Tomm_fn_Recipies.h"
#include "sup/papyrus/world/Tomm_fn_PowerGrid.h"
#include "sup/papyrus/world/Tomm_fn_Location.h"
#include "sup/papyrus/world/Tomm_fn_MapMarker.h"

#include "sup/core/Tomm_RegisterFunctions.h"
#include "sup/hooks/Tomm_Hooks.h"





void F_SUP_TEST_FUNCTION()
{
}

#include "sup/events/Tomm_NativeEvents.h"


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
				sup::string::to_lower(str);
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
	g_uiMessageManager->SendUIMessageImpl(MenuName, kMessage_Open);
	g_uiMessageManager->SendUIMessageImpl(MenuName, kMessage_Close);

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
	f_InitHooks();

	// Phase 4: log the resolved workshop/power-grid helper addresses once, so a game
	// update that shifts the binary (or a stale Address Library) shows up here as a
	// missing/zero address instead of a silent crash later.
	sup::compat::VerifyWorkshopIDs();


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

	if (auto* source = RE::TESActivateEvent::GetEventSource()) {
		source->RegisterSink(&g_SUPActivateEvent);
	}


	
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
	// Phase 3: re-sync the legacy singleton globals (they no longer point into the exe)
	sup::compat::RefreshSingletons();

	switch (msg->type)
	{
	case kMessage_GameLoaded:
		break;


	case kMessage_PostSaveGame:

		if (OnGameSaveEventHandler.HasEvents())
		{
			OnGameSaveEventHandler.EnterLock();
			OnGameSaveEventHandler.RunEvents(NULL);
			OnGameSaveEventHandler.LeaveLock();
		}
		break;


	case kMessage_PreLoadGame:

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


	case kMessage_PostLoadGame:
		SetNativeEventsOnSaveLoad();
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

	case kMessage_NewGame:
		SetNativeEventsOnSaveLoad();
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

	case kMessage_GameDataReady:
		f_SUP_ONDefferedInit();
		break;

	default:
		break;
	}

}













// Phase 3: F4SEPlugin_Query is replaced by the CommonLibF4 plugin-version data generated
// by the commonlibf4.plugin rule (src/main.cpp + commonlibf4-plugin.cpp.in).


#include "sup/core/Tomm_Serialization.h"



// Phase 3: adapter between the CommonLibF4 Papyrus interface callback (raw IVirtualMachine*)
// and the legacy RegisterFuncs(VirtualMachine*). The wrapper is static so g_MyVirtMachine
// stays valid for the whole process lifetime.
bool SUP_RegisterPapyrusFunctions(BSScript::IVirtualMachine* a_vm)
{
	// The wrapper is static so g_MyVirtMachine stays valid for the whole process lifetime.
	static VirtualMachine wrapper(a_vm);

	// Pre-load the SUP_F4SE script object so every struct type is registered before the
	// NativeFunction constructors resolve their struct-typed signatures. The real fix for the
	// "failed to get type info for structure" errors is in CommonLibF4's BSScriptUtil.h
	// (structure_wrapper::name was not null-terminated); this load is a belt-and-braces guard.
	{
		RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo> objType;
		a_vm->GetScriptObjectType(RE::BSFixedString("SUP_F4SE"), objType);
	}

	return RegisterFuncs(&wrapper);
}


// Phase 3: F4SEPlugin_Load is replaced by this function, called from main.cpp after F4SE::Init.
// The F4SE interface globals are gone; interfaces are fetched here via F4SE::Get*Interface().
bool SUP_F4SE_PluginLoad()
{

	const auto* serialization = F4SE::GetSerializationInterface();
	if (serialization)
	{
		serialization->SetUniqueID(t_CosaveF4SEPluginID);
		serialization->SetSaveCallback(SUPCosave::SaveCallback);
		serialization->SetLoadCallback(SUPCosave::LoadCallback);
	}
	else {
		_MESSAGE("ERROR::Failed to get serialization interface");
	}

	const auto* papyrus = F4SE::GetPapyrusInterface();
	if (papyrus)
	{
		if (papyrus->Register(SUP_RegisterPapyrusFunctions))
			_MESSAGE("SUP F4SE Funcs Registered");
	}
	else {
		_MESSAGE("ERROR::Failed to get papyrus interface");
	}


	std::filesystem::path GamePath = std::filesystem::current_path();
	g_FalloutFolderPath = GamePath.string() + "\\";



	CSimpleIniA ini;


	INIManagement::ProcessINIFile(ini);

	if (bDebugMode)
		sup::log::set_debug_enabled(true);


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

	float fVersion = iVersion * 0.01;
	_MESSAGE("SUP F4SE loaded V.%0.2lf,bScrapCrashFix>>%d,bTermLinkFix>>%d", fVersion, bScrapCrashFix, bTermLinkFix);
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
