

#include "f4se/PapyrusDelayFunctors.h"


//DECLARE_DELAY_FUNCTOR(F4SEAttachWireFunctor, 2, cmd_DrawSplineLatent, TESObjectREFR, TESObjectREFR*, TESObjectREFR*, TESForm*);


bool RegisterFuncs(VirtualMachine* vm) {

	g_MyVirtMachine = vm;

	vm->RegisterFunction(new NativeFunction0 <StaticFunctionTag, UInt32>("GetSUPF4SEVersion", pluginName, cmd_GetSUPF4SEVersion, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, float, UInt32>("GetSystemTrait", pluginName, cmd_GetSystemTrait, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, UInt32, BSFixedString>("FileExists", pluginName, cmd_FileExists, vm));
	vm->RegisterFunction(new NativeFunction8<StaticFunctionTag, float, BSFixedString, BSFixedString, float, float, float, float, float, float>("CaptureScreenshotAlt", pluginName, cmd_CaptureScreenshotAlt, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, float, BSFixedString, float>("CaptureScreenshot", pluginName, cmd_CaptureScreenshot, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, BSFixedString>("IsProcessRunning", pluginName, cmd_IsProcessRunning, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, UInt32, UInt32>("GetMousePosition", pluginName, cmd_GetMousePosition, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, BSFixedString>("ClipboardToString", pluginName, cmd_ClipboardToString, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, BSFixedString>("StringToClipboard", pluginName, cmd_StringToClipboard, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, UInt32, BSFixedString, BSFixedString>("DeleteScreenshot", pluginName, cmd_DeleteScreenshot, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, BSFixedString>("GetFalloutRootFolder", pluginName, cmd_GetFalloutRootFolder, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, float, BSFixedString>("FileGetSize", pluginName, cmd_FileGetSize, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, VMArray<BSFixedString>, BSFixedString, UInt32>("FolderGetContents", pluginName, cmd_FolderGetContents, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, UInt32, float>("Round", pluginName, cmd_Round, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, BSFixedString>("IsDLLLoaded", pluginName, cmd_IsDLLLoaded, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, float, BSFixedString, BSFixedString, BSFixedString>("ReadINIFloatFromFile", pluginName, cmd_ReadINIFloatFromFile, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, BSFixedString, BSFixedString, BSFixedString, BSFixedString>("ReadINIStringFromFile", pluginName, cmd_ReadINIStringFromFile, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, VMArray<BSFixedString>, BSFixedString>("ReadINISectionsFromFile", pluginName, cmd_ReadINISectionsFromFile, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, VMArray<BSFixedString>, BSFixedString, BSFixedString>("ReadINISectionKeysFromFile", pluginName, cmd_ReadINISectionKeysFromFile, vm));
	vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, BSFixedString, BSFixedString, BSFixedString, BSFixedString, UInt32>("StringReplace", pluginName, cmd_StringReplace, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, BSFixedString, BSFixedString, UInt32, UInt32>("StringErase", pluginName, cmd_StringErase, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, bool, BSFixedString, BSFixedString>("StringCompareCaseInsensitive", pluginName, cmd_StringCompareCaseInsensitive, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, UInt32, BSFixedString>("StringGetLength", pluginName, cmd_StringGetLength, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, bool, BSFixedString, BSFixedString>("StringContains", pluginName, cmd_StringContains, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, BSFixedString, BSFixedString, BSFixedString, UInt32>("StringInsert", pluginName, cmd_StringInsert, vm));
	//vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, float, BSFixedString>("StringToFloat", pluginName, cmd_StringToFloat, vm));
	//vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, UInt32, BSFixedString>("StringToInt", pluginName, cmd_StringToInt, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, UInt32, BSFixedString>("FileGetLinesCount", pluginName, cmd_FileGetLinesCount, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, BSFixedString, BSFixedString, UInt32, UInt32>("ReadStringFromFile", pluginName, cmd_ReadStringFromFile, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, VMArray<BSFixedString>, BSFixedString, UInt32>("FileGetFileTime", pluginName, cmd_FileGetFileTime, vm));
	vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, UInt32, BSFixedString, BSFixedString, BSFixedString, float>("SetModINIFloat", pluginName, cmd_SetINIFloat, vm));
	vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, UInt32, BSFixedString, BSFixedString, BSFixedString, UInt32>("SetModINILong", pluginName, cmd_SetINILong, vm));
	vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, UInt32, BSFixedString, BSFixedString, BSFixedString, BSFixedString>("SetModINIString", pluginName, cmd_SetINIString, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, float, BSFixedString, BSFixedString, BSFixedString>("GetModINIFloat", pluginName, cmd_GetINIFloat, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, UInt32, BSFixedString, BSFixedString, BSFixedString>("GetModINILong", pluginName, cmd_GetINILong, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, BSFixedString, BSFixedString, BSFixedString, BSFixedString>("GetModINIString", pluginName, cmd_GetINIString, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, BSFixedString, BSFixedString>("GetSHA1FromFile", pluginName, cmd_GetSHA1FromFile, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, BSFixedString, BSFixedString>("GetSHA1FromString", pluginName, cmd_GetSHA1FromString, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, BSFixedString>("GetUserTimeStamp", pluginName, cmd_GetUserTimeStamp, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, VMArray<UInt32>>("GetUserTime", pluginName, cmd_GetUserTime, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, TESForm*>("IsFormValid", pluginName, cmd_IsFormValid, vm));

	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, bool, BSFixedString, BSFixedString, UInt32>("WriteStringToFile", pluginName, cmd_WriteStringToFile, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, VMArray<BSFixedString>, BSFixedString, BSFixedString>("ReadStringArrayFromFile", pluginName, cmd_ReadStringArrayFromFile, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, VMArray<TESForm*>, BSFixedString, BSFixedString>("ReadFormArrayFromFile", pluginName, cmd_ReadFormArrayFromFile, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, VMArray<TESForm*>, BGSListForm*>("FormListToArray", pluginName, cmd_FormListToArray, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, BSFixedString, UInt32>("NumberToString", pluginName, cmd_NumberToString, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, UserTimeStruct>("GetUserTimeStruct", pluginName, cmd_GetUserTimeStruct, vm));

	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, GetCalculatedPosRes, TESObjectREFR*,float,float>("GetCalculatedPos", pluginName, cmd_GetCalculatedPos, vm));
	vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, GetCalculatedPosRes, float, float, float, float>("GetCalculatedPosAlt", pluginName, cmd_GetCalculatedPosAlt, vm));
	vm->RegisterFunction(new NativeFunction6<StaticFunctionTag, float, float, float, float, float, float, float>("GetDistanceBetweenPoints", pluginName, cmd_GetDistanceBetweenPoints, vm));
	vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, float, TESObjectREFR*, float, float, float>("GetHeadingAngleAlt", pluginName, cmd_GetHeadingAngleAlt, vm));
	vm->RegisterFunction(new NativeFunction5<StaticFunctionTag, float, float, float, float, float, float>("GetHeadingAngleBetweenPoints", pluginName, cmd_GetHeadingAngleBetweenPoints, vm));
	//vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, BSFixedString, BSFixedString>("StringToUpper", pluginName, cmd_StringToUpper, vm));

	//V 2.00
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, void, BSFixedString, UInt32>("DebugPrintSUP", pluginName, cmd_DebugPrintSUP, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, FileDialogueBoxResult, BSFixedString>("FileDialogueBoxOpenFile", pluginName, cmd_FileDialogueBoxOpenFile, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, FileDialogueBoxResult>("FileDialogueBoxOpenFolder", pluginName, cmd_FileDialogueBoxOpenFolder, vm));

	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, void, BSFixedString, UInt32>("DebugPrintSUP", pluginName, cmd_DebugPrintSUP, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag,bool, BSFixedString, BSFixedString>("ModLocalDataExists", pluginName, cmd_ModLocalDataExists, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, UInt32, BSFixedString, BSFixedString>("ModLocalDataGetType", pluginName, cmd_ModLocalDataGetType, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, void, BSFixedString, BSFixedString, float>("ModLocalDataSetFloat", pluginName, cmd_ModLocalDataSetFloat, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, void, BSFixedString, BSFixedString, UInt32>("ModLocalDataSetInt", pluginName, cmd_ModLocalDataSetInt, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, void, BSFixedString, BSFixedString, TESObjectREFR*>("ModLocalDataSetRef", pluginName, cmd_ModLocalDataSetRef, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, void, BSFixedString, BSFixedString, BSFixedString>("ModLocalDataSetString", pluginName, cmd_ModLocalDataSetString, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, float, BSFixedString, BSFixedString>("ModLocalDataGetFloat", pluginName, cmd_ModLocalDataGetFloat, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, UInt32, BSFixedString, BSFixedString>("ModLocalDataGetInt", pluginName, cmd_ModLocalDataGetInt, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, TESObjectREFR*, BSFixedString, BSFixedString>("ModLocalDataGetRef", pluginName, cmd_ModLocalDataGetRef, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, BSFixedString, BSFixedString, BSFixedString>("ModLocalDataGetString", pluginName, cmd_ModLocalDataGetString, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, bool, BSFixedString, BSFixedString>("ModLocalDataRemoveKey", pluginName, cmd_ModLocalDataRemoveKey, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, BSFixedString>("ModLocalDataRemoveAllKeys", pluginName, cmd_ModLocalDataRemoveAllKeys, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, void, BSFixedString>("ModLocalDataDumpToConsole", pluginName, cmd_ModLocalDataDumpToConsole, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, VMArray<ModLocalDataElement>, BSFixedString>("ModLocalDataDumpToArray", pluginName, cmd_ModLocalDataDumpToArray, vm));

	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, bool, BSFixedString, BSFixedString, UInt32>("StringRegexMatch", pluginName, cmd_StringRegexMatch, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, VMArray<BSFixedString>, BSFixedString, BSFixedString, UInt32>("StringRegexSearch", pluginName, cmd_StringRegexSearch, vm));
	vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, BSFixedString, BSFixedString, BSFixedString, BSFixedString, UInt32>("StringRegexReplace", pluginName, cmd_StringRegexReplace, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, float,  UInt32>("GetLastLoadedSaveSize", pluginName, cmd_GetLastLoadedSaveSize, vm));

	//vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, BSFixedString>("MP3LoadFile", pluginName, cmd_MP3LoadFile, vm));
	//vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, bool>("MP3Play", pluginName, cmd_MP3Play, vm));
	//vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, bool>("MP3Pause", pluginName, cmd_MP3Pause, vm));
	//vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, bool>("MP3Stop", pluginName, cmd_MP3Stop, vm));
	//vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, bool>("MP3IsPlaying", pluginName, cmd_MP3IsPlaying, vm));
	//vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, UInt32>("MP3GetDuration", pluginName, cmd_MP3GetDuration, vm));
	//vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, UInt32>("MP3GetCurrentPosition", pluginName, cmd_MP3GetCurrentPosition, vm));
	//vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, bool>("MP3IsFileLoaded", pluginName, cmd_IsFileLoaded, vm));
	//vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, UInt32>("MP3GetVolume", pluginName, cmd_MP3GetVolume, vm));
	//vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, UInt32>("MP3SetVolume", pluginName, cmd_MP3SetVolume, vm));
	//vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, bool>("MP3HasFinishedPlaying", pluginName, cmd_MP3HasFinishedPlaying, vm));
	//vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, BSFixedString>("MP3GetCurrentFile", pluginName, cmd_MP3GetCurrentFile, vm));

	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, TESObjectCELL*>("IsCellVisited", pluginName, cmd_IsCellVisited, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, TESWorldSpace*, TESObjectCELL*>("GetCellWorldSpace", pluginName, cmd_GetCellWorldSpace, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, UInt32, BSFixedString>("FileExistsAlt", pluginName, cmd_FileExistsAlt, vm));



	vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, bool, TESObjectREFR*, BSFixedString, float, UInt32>("SetNodeScale", pluginName, cmd_SetNodeScale, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, float, TESObjectREFR*, BSFixedString, UInt32>("GetNodeScale", pluginName, cmd_GetNodeScale, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, VMArray<BSFixedString>, TESObjectREFR*, BSFixedString, UInt32>("GetNodeChildren", pluginName, cmd_GetNodeChildren, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, UInt32, TESObjectREFR*, BSFixedString, UInt32>("GetNodeChildrenCount", pluginName, cmd_GetNodeChildrenCount, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, UInt32, TESObjectREFR*, BSFixedString, UInt32>("GetNodeFlags", pluginName, cmd_GetNodeFlags, vm));
	vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, bool, TESObjectREFR*, BSFixedString, UInt32, UInt32>("SetNodeFlags", pluginName, cmd_SetNodeFlags, vm));
	vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, NodeTranslation, TESObjectREFR*, BSFixedString, UInt32, UInt32>("GetNodeTranslation", pluginName, cmd_GetNodeTranslation, vm));
	vm->RegisterFunction(new NativeFunction5<StaticFunctionTag, bool, TESObjectREFR*, BSFixedString, UInt32,NodeTranslation, UInt32>("SetNodeTranslation", pluginName, cmd_SetNodeTranslation, vm));
	vm->RegisterFunction(new NativeFunction6<StaticFunctionTag, bool, TESObjectREFR*, BSFixedString, UInt32, BSFixedString, float, UInt32>("SetNodeTranslationAxis", pluginName, cmd_SetNodeTranslationAxis, vm));
	vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, NodeRotation, TESObjectREFR*, BSFixedString, UInt32, UInt32>("GetNodeRotation", pluginName, cmd_GetNodeRotation, vm));
	vm->RegisterFunction(new NativeFunction5<StaticFunctionTag, bool, TESObjectREFR*, BSFixedString, UInt32, NodeRotation,UInt32>("SetNodeRotation", pluginName, cmd_SetNodeRotation, vm));
	vm->RegisterFunction(new NativeFunction6<StaticFunctionTag, bool, TESObjectREFR*, BSFixedString, UInt32, BSFixedString, float, UInt32>("SetNodeRotationAxis", pluginName, cmd_SetNodeRotationAxis, vm));


	vm->RegisterFunction(new NativeFunction5<StaticFunctionTag, bool, BSFixedString, BSFixedString, UInt32, UInt32, float>("ExcelWriteFloat", pluginName, cmd_ExcelWriteFloat, vm));
	vm->RegisterFunction(new NativeFunction5<StaticFunctionTag, bool, BSFixedString, BSFixedString, UInt32, UInt32, BSFixedString >("ExcelWriteString", pluginName, cmd_ExcelWriteString, vm));
	vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, float, BSFixedString, BSFixedString, UInt32, UInt32>("ExcelReadFloat", pluginName, cmd_ExcelReadFloat, vm));
	vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, BSFixedString, BSFixedString, BSFixedString, UInt32, UInt32>("ExcelReadString", pluginName, cmd_ExcelReadString, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, UInt32, BSFixedString>("ExcelGetWorkSheetCount", pluginName, cmd_ExcelGetWorkSheetCount, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, bool, BSFixedString, BSFixedString>("ExcelWorkSheetExists", pluginName, cmd_ExcelWorkSheetExists, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, UInt32, BSFixedString, BSFixedString>("ExcelGetRowCount", pluginName, cmd_ExcelGetRowCount, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, UInt32, BSFixedString, BSFixedString>("ExcelGetColumnCount", pluginName, cmd_ExcelGetColumnCount, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, bool, BSFixedString, BSFixedString, BSFixedString>("ExcelWorkSheetRename", pluginName, cmd_ExcelWorkSheetRename, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, VMArray<BSFixedString>, BSFixedString>("ExcelGetAllWorkSheets", pluginName, cmd_ExcelGetAllWorkSheets, vm));
	vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, bool, BSFixedString, BSFixedString, UInt32, UInt32>("ExcelEraseCell", pluginName, cmd_ExcelEraseCell, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, bool, BSFixedString, BSFixedString>("ExcelEraseSheet", pluginName, cmd_ExcelEraseSheet, vm));
	vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, UInt32, BSFixedString, BSFixedString, UInt32, UInt32>("ExcelGetCellValueType", pluginName, cmd_ExcelGetCellValueType, vm));
	vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, UInt32, BSFixedString, BSFixedString, UInt32, UInt32>("ExcelGetCellValueType", pluginName, cmd_ExcelGetCellValueType, vm));


	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, BSFixedString, BSFixedString>("GetGitHubLatestReleaseTag", pluginName, cmd_GetGitHubLatestReleaseTag, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, BSFixedString, UInt32>("GetNexusModVersionAPI", pluginName, cmd_GetNexusModVersionAPI, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, bool>("IsNexusAPIKeyValid", pluginName, cmd_IsNexusAPIKeyValid, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, UInt32>("GetSUPDateCheckerDoneStage", pluginName, cmd_GetSUPDateCheckerDoneStage, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, UInt32>("GetSUPDateCheckerCurrentStage", pluginName, cmd_GetSUPDateCheckerCurrentStage, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, void, UInt32>("OpenNexusModPage", pluginName, cmd_OpenNexusModPage, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, bool>("IsSUPDateCheckerLoaded", pluginName, cmd_IsSUPDateCheckerLoaded, vm));



	vm->RegisterFunction(new NativeFunction5<StaticFunctionTag, VMArray<float>, BSFixedString, BSFixedString, UInt32, UInt32, UInt32>("ExcelReadFloatArray", pluginName, cmd_ExcelReadFloatArray, vm));
	vm->RegisterFunction(new NativeFunction5<StaticFunctionTag, VMArray<BSFixedString>, BSFixedString, BSFixedString, UInt32, UInt32, UInt32>("ExcelReadStringArray", pluginName, cmd_ExcelReadStringArray, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, TESObjectREFR*, TESObjectREFR*>("GetAshPileSource", pluginName, cmd_GetAshPileSource, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, TESObjectREFR*, TESObjectREFR*>("GetItemDropper", pluginName, cmd_GetItemDropper, vm));
	
	//vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, float>("GetLeftStickX", pluginName, cmd_GetLeftStickX, vm));
	//vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, float>("GetRightStickX", pluginName, cmd_GetRightStickX, vm));
	//vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, float>("GetLeftStickY", pluginName, cmd_GetLeftStickY, vm));
	//vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, float>("GetRightStickY", pluginName, cmd_GetRightStickY, vm));
	//vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, float>("GetLeftTrigger", pluginName, cmd_GetLeftTrigger, vm));
	//vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, float>("GetRightTrigger", pluginName, cmd_GetRightTrigger, vm));
	//vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, UInt32>("IsGamePadButtonPressed", pluginName, cmd_IsGamePadButtonPressed, vm));

	
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, BSFixedString>("GetWindowsLanguage", pluginName, cmd_GetUserWindowsLanguage, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, FileVersion, BSFixedString>("GetFileVersion", pluginName, cmd_GetFileVersion, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, BSFixedString>("SteamGetGameLanguage", pluginName, cmd_SteamGetGameLanguage, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, BSFixedString>("SteamGetSteamLanguage", pluginName, cmd_SteamGetSteamLanguage, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, bool>("SteamIsLoaded", pluginName, cmd_SteamIsLoaded, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, bool>("SteamIsOverlayEnabled", pluginName, cmd_SteamIsOverlayEnabled, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, void>("SteamTriggerScreenshot", pluginName, cmd_SteamTriggerScreenshot, vm));


	vm->RegisterFunction(new NativeFunction5<StaticFunctionTag, bool, TESObjectREFR*, TESObjectREFR*, TESObjectREFR*, float, float>("DrawSpline", pluginName, cmd_DrawSpline, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, TESForm*, TESForm*>("GetSplineForm", pluginName, cmd_GetSplineForm, vm));

	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, TESObjectREFR*, BGSLocation*>("GetWorldLocationMarkerRef", pluginName, cmd_GetWorldLocationMarkerRef, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, TESObjectREFR*>("IsMapMarker", pluginName, cmd_IsMapMarker, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, bool, TESObjectREFR*, BSFixedString>("MapMarkerSetName", pluginName, cmd_MapMarkerSetName, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, BSFixedString, TESObjectREFR*>("MapMarkerGetName", pluginName, cmd_MapMarkerGetName, vm));


	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, TESObjectREFR*>("MapMarkerGetUseLocationName", pluginName, cmd_MapMarkerGetUseLocationName, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, bool, TESObjectREFR*, bool>("MapMarkerSetUseLocationName", pluginName, cmd_MapMarkerSetUseLocationName, vm));


	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, PowerGridInstance, TESObjectREFR*, TESObjectREFR*>("GetPowerGridForObject", pluginName, cmd_GetPowerGridForObject, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, UInt32, TESObjectREFR*>("IsWorkshopPowerGridOverloaded", pluginName, cmd_IsWorkshopPowerGridOverloaded, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, VMArray<PowerGridInstance>, TESObjectREFR*>("ListPowerGrids", pluginName, cmd_ListPowerGrids, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, VMArray<TESObjectREFR*>, TESObjectREFR*, UInt32>("GetPowerGridElements", pluginName, cmd_GetPowerGridElements, vm));

	
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, bool,BSFixedString, BSFixedString, TESObjectREFR*>("ModLocalDataRefArrayAddReference", pluginName, cmd_ModLocalDataRefArrayAddReference, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, VMArray<TESObjectREFR*>, BSFixedString, BSFixedString>("ModLocalDataRefArrayGet", pluginName, cmd_ModLocalDataRefArrayGet, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, UInt32, BSFixedString, BSFixedString, TESObjectREFR*>("ModLocalDataRefArrayGetIndex", pluginName, cmd_ModLocalDataRefArrayGetIndex, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, UInt32, BSFixedString, BSFixedString, TESObjectREFR*>("ModLocalDataRefArrayRemoveReference", pluginName, cmd_ModLocalDataRefArrayRemoveReference, vm));


	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, BSFixedString, BSFixedString>("GetPastebinRawTextString", pluginName, cmd_GetPastebinRawTextString, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, VMArray<BSFixedString>, BSFixedString>("GetPastebinRawTextArray", pluginName, cmd_GetPastebinRawTextArray, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, UInt32>("GetPastebinReaderDoneStage", pluginName, cmd_GetPastebinReaderDoneStage, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, UInt32>("GetPastebinReaderCurrentStage", pluginName, cmd_GetPastebinReaderCurrentStage, vm));



	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, VMArray<Actor*>, TESObjectCELL*, UInt32>("GetActorsInCell", pluginName, cmd_GetActorsInCell, vm));




	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, UInt32,UInt32, UInt32>("BBTextInputUpdate", pluginName, cmd_BBTextInputUpdate, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, UInt32>("BBGetTextInputOffsetPage", pluginName, cmd_BBGetTextInputOffsetPage, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, UInt32, UInt32>("BBSetTextInputCurrentPage", pluginName, cmd_BBSetTextInputCurrentPage, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, UInt32>("BBGetTextInputCurrentPage", pluginName, cmd_BBGetTextInputCurrentPage, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, UInt32>("BBAreMessagesFetched", pluginName, cmd_BBAreMessagesFetched, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, UInt32>("BBAPIKeyExists", pluginName, cmd_BBAPIKeyExists, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, UInt32>("BBNexusAPIKeyExists", pluginName, cmd_BBNexusAPIKeyExists, vm));

	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, UInt32>("BBGetBBKey", pluginName, cmd_BBGetBBKey, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, void>("BBSetLastServerMessageForMessageBox", pluginName, cmd_BBSetLastServerMessageForMessageBox, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, UInt32, UInt32, UInt32>("BBPostMessage", pluginName, cmd_BBPostMessage, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, void>("BBOnCellChange", pluginName, cmd_BBOnCellChange, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, UInt32>("BBHasCellChanged", pluginName, cmd_BBHasCellChanged, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, UInt32, UInt32>("BBGetCurrentMenuButtonFor", pluginName, cmd_BBGetCurrentMenuButtonFor, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, UInt32, TESObjectREFR*>("BBActivatorActivate", pluginName, cmd_BBActivatorActivate, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, void>("BBSetLastActivatedMessageForMessageBox", pluginName, cmd_BBSetLastActivatedMessageForMessageBox, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, UInt32, UInt32>("BBGetUserInfo", pluginName, cmd_BBGetUserInfo, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, UInt32>("BBSetMessageForMainMenu", pluginName, cmd_BBSetMessageForMainMenu, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, UInt32>("BBLikeMessage", pluginName, cmd_BBLikeMessage, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, UInt32>("BBGetLastActivatedMessageRating", pluginName, cmd_BBGetLastActivatedMessageRating, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, UInt32>("BBGetUserMessages", pluginName, cmd_BBGetUserMessages, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, UInt32, UInt32>("BBSetManageMessagesCurrentPage", pluginName, cmd_BBSetManageMessagesCurrentPage, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, UInt32>("BBGetManageMessagesCurrentPage", pluginName, cmd_BBGetManageMessagesCurrentPage, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, UInt32, UInt32, UInt32>("BBManageMenuUpdate", pluginName, cmd_BBManageMenuUpdate, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, UInt32>("BBDeleteUserMessage", pluginName, cmd_BBDeleteUserMessage, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, UInt32>("BBHideMessage", pluginName, cmd_BBHideMessage, vm));
	
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, UInt32>("IsMenuModeActive", pluginName, cmd_IsMenuModeActive, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, UInt32>("IsGamePaused", pluginName, cmd_IsGamePaused, vm));
	
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, UInt32, UInt32, UInt32, UInt32>("BBGetPressedKeyResult", pluginName, cmd_BBGetPressedKeyResult, vm));
	
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, void, UInt32>("BBSetMenuStage", pluginName, cmd_BBSetMenuStage, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, UInt32>("BBGetMenuStage", pluginName, cmd_BBGetMenuStage, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, void>("BBClearTextInputState", pluginName, cmd_BBClearTextInputState, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, void>("BBTextInputFreeTextUpdate", pluginName, cmd_BBTextInputFreeTextUpdate, vm));

	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, UInt32, UInt32>("BBCheckLaunchValue", pluginName, cmd_BBCheckLaunchValue, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, UInt32>("BBReportMessage", pluginName, cmd_BBReportMessage, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, UInt32>("IsNewGame", pluginName, cmd_IsNewGame, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, void>("BBOnNewGameLoad", pluginName, cmd_BBOnNewGameLoad, vm));


	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, TESObjectREFR*>("UpdateMovingWirelessItem", pluginName, cmd_UpdateMovingWirelessItem, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, TESObjectREFR*>("EstablishTerminalLinks", pluginName, cmd_EstablishTerminalLinks, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, bool, TESObjectREFR*, TESObjectREFR*, TESObjectREFR*>("UpdateSpline", pluginName, cmd_UpdateSpline, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, VMArray<TESObjectREFR*>, TESObjectREFR*>("UpdateWires", pluginName, cmd_UpdateWires, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, JSONValue, BSFixedString, BSFixedString, UInt32>("JSONGetValue", pluginName, cmd_JSONGetValue, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, VMArray<JSONValue>, BSFixedString, BSFixedString, UInt32>("JSONGetValueArray", pluginName, cmd_JSONGetValueArray, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, UInt32, BSFixedString, UInt32>("JSONCacheFile", pluginName, cmd_JSONCacheFile, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, UInt32, BSFixedString, UInt32, BSFixedString>("JSONCloseFile", pluginName, cmd_JSONCloseFile, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, UInt32, BSFixedString,  BSFixedString>("JSONCacheFileFromString", pluginName, cmd_JSONCacheFileFromString, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, UInt32, BSFixedString>("JSONIsFileCached", pluginName, cmd_JSONIsFileCached, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, UInt32, BSFixedString, BSFixedString, UInt32>("JSONGetValueType", pluginName, cmd_JSONGetValueType, vm));
	vm->RegisterFunction(new NativeFunction5<StaticFunctionTag, UInt32, BSFixedString, BSFixedString,float, UInt32, UInt32>("JSONSetValueFloat", pluginName, cmd_JSONSetValueFloat, vm));
	vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, UInt32, BSFixedString, BSFixedString, BSFixedString, UInt32>("JSONSetValueString", pluginName, cmd_JSONSetValueString, vm));
	vm->RegisterFunction(new NativeFunction5<StaticFunctionTag, UInt32, BSFixedString, BSFixedString, float, UInt32,UInt32>("JSONAppendValueFloat", pluginName, cmd_JSONAppendValueFloat, vm));
	vm->RegisterFunction(new NativeFunction5<StaticFunctionTag, UInt32, BSFixedString, BSFixedString, BSFixedString, UInt32, UInt32>("JSONAppendValueString", pluginName, cmd_JSONAppendValueString, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, UInt32, BSFixedString, BSFixedString,  UInt32>("JSONClearKey", pluginName, cmd_JSONClearKey, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, UInt32, BSFixedString, BSFixedString, UInt32>("JSONEraseKey", pluginName, cmd_JSONEraseKey, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, BSFixedString, BSFixedString>("JSONToString", pluginName, cmd_JSONToString, vm));
	vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, UInt32, BSFixedString, BSFixedString, UInt32,  UInt32>("JSONEraseArrayElement", pluginName, cmd_JSONEraseArrayElement, vm));

	vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, UInt32, BSFixedString, BSFixedString, UInt32, UInt32>("SUPStringFind", pluginName, cmd_SUPStringFind, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, BSFixedString, BSFixedString, UInt32, UInt32>("StringFindSubString", pluginName, cmd_StringFindSubString, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, UInt32, BSFixedString>("JSONIsFileValid", pluginName, cmd_JSONIsFileValid, vm));


	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, VMArray<ConnectPointSUP>, TESObjectREFR*>("SUPGetConnectPoints", pluginName, cmd_SUPGetConnectPoints, vm));

	vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, float, TESObjectREFR*, BSFixedString, BSFixedString, UInt32>("GetDistanceBetweenNodes", pluginName, cmd_GetDistanceBetweenNodes, vm));
	vm->RegisterFunction(new NativeFunction6<StaticFunctionTag, float, TESObjectREFR*, BSFixedString, UInt32, TESObjectREFR*, BSFixedString, UInt32 >("GetDistanceBetweenNodesTwoRefs", pluginName, cmd_GetDistanceBetweenNodesTwoRefs, vm));

	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, VMArray<TESObjectCELL*>, TESObjectCELL*, UInt32>("GetNearCells", pluginName, cmd_GetNearCells, vm));

	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, VMArray<TESObjectREFR*>, TESObjectCELL*, UInt32, UInt32>("GetReferencesInCell", pluginName, cmd_GetReferencesInCell, vm));
	vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, ScreenPos, float, float, float, UInt32>("WorldToScreen", pluginName, cmd_WorldToScreen, vm));

	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, TESObjectREFR*>("IsRadio", pluginName, cmd_IsRadio, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, RadioData, TESObjectREFR*>("GetRadioData", pluginName, cmd_GetRadioData, vm));


	vm->RegisterFunction(new NativeFunction5<StaticFunctionTag, TESObjectREFR*,TESObjectREFR*, TESObjectREFR*, TESObjectREFR*, UInt32, UInt32>("AttachWireEX", pluginName, cmd_AttachWireEX, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, VMArray <SUPFurnitureMarkerData>, TESForm*>("GetFurnitureMarkersFromFurniture", pluginName, cmd_GetFurnitureMarkersFromFurniture, vm));

	vm->RegisterFunction(new NativeFunction5<StaticFunctionTag, bool, TESObjectREFR*, TESObjectREFR*, TESObjectREFR*, UInt32, UInt32>("UpdateSplineEx", pluginName, cmd_UpdateSplineEx, vm));

	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, VMArray<SUPConnectedObject>, TESObjectREFR*>("GetConnectedObjectsEx", pluginName, cmd_GetConnectedObjectsEx, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, UInt32, TESObjectREFR*, UInt32>("RemovePowerGrid", pluginName, cmd_RemovePowerGrid, vm));


	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, VMArray<TESObjectREFR*>>("GetAllRadios", pluginName, cmd_GetAllRadios, vm));

	vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, float, TESObjectREFR*, BSFixedString, TESObjectREFR*, UInt32>("GetDistanceBetweenNodeAndRef", pluginName, cmd_GetDistanceBetweenNodeAndRef, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, VMArray<TESObjectREFR*>, TESObjectCELL*, TESForm*, UInt32>("GetReferencesInCellByBaseForm", pluginName, cmd_GetReferencesInCellByBaseForm, vm));

	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, VMArray<BSFixedString>, BSFixedString, BSFixedString, UInt32>("SplitStringEx", pluginName, cmd_SplitStringEx, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, TESObjectREFR*>("IsRadioReceiver", pluginName, cmd_IsRadioReceiver, vm));
	vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, SUPXYZ, TESObjectREFR*, BSFixedString, BSFixedString, UInt32>("GetDistanceBetweenNodesEx", pluginName, cmd_GetDistanceBetweenNodesEx, vm));


	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, TESForm*, BSFixedString, BSFixedString, UInt32>("GetFormFromFileEx", pluginName, cmd_GetFormFromFileEx, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, UInt32, BSFixedString>("StringToDec", pluginName, cmd_StringToDec, vm));

	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, float>("GetGlobalTimeMultiplier", pluginName, cmd_GetGlobalTimeMultiplier, vm));


	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, SUPXYZ, TESObjectREFR*>("GetStartingLocation", pluginName, cmd_GetStartingLocation, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, SUPXYZ, TESObjectREFR*>("GetStartingAngle", pluginName, cmd_GetStartingAngle, vm));


	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, TESObjectREFR*>("TransmitConnectedPowerLatentEx", pluginName, cmd_TransmitConnectedPowerLatentEx, vm));

	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, float>("SetGlobalTimeMultiplier", pluginName, cmd_SetGlobalTimeMultiplier, vm));



	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, VMArray<VMVariable>, VMArray<VMVariable>, VMArray<VMVariable>>("MergeArrays", pluginName, cmd_MergeArrays, vm));


	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, VMArray<float>>("GetAllRadioFrequencies", pluginName, cmd_GetAllRadioFrequencies, vm));



	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, BSFixedString, BSFixedString, bool>("XMLToJSONString", pluginName, cmd_XMLToJSONString, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, UInt32, BSFixedString, bool, BSFixedString>("XMLToJSONCachedFile", pluginName, cmd_XMLToJSONCachedFile, vm));



	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, bool, TESForm*,bool>("SetChargenPresetFlag", pluginName, cmd_SetChargenPresetFlag, vm));

	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, VMArray<BGSConstructibleObject*>, TESForm*>("GetAllConstructibleObjectsFromForm", pluginName, cmd_GetAllConstructibleObjectsFromForm, vm));

	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, VMArray<TESForm*>, UInt32,BSFixedString>("GetLoadedFormsByType", pluginName, cmd_GetLoadedFormsByType, vm));


	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, float, UInt32, BSFixedString, BSFixedString>("ReadINIFloatFromGameSettingINI", pluginName, cmd_ReadINIFloatFromGameSettingINI, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, BSFixedString, UInt32, BSFixedString, BSFixedString>("ReadINIStringFromGameSettingINI", pluginName, cmd_ReadINIStringFromGameSettingINI, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, GameSettingValue, BSFixedString>("GetGameINISettingValue", pluginName, cmd_GetGameINISettingValue, vm));

	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, VMArray<TESFaction*>, TESObjectREFR*>("GetAllActorFactions", pluginName, cmd_GetAllActorFactions, vm));


	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, bool, TESAmmo*, BGSProjectile*>("SetAmmoProjectile", pluginName, cmd_SetAmmoProjectile, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, BGSProjectile*, TESAmmo*>("GetAmmoProjectile", pluginName, cmd_GetAmmoProjectile, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, BSFixedString, TESAmmo*>("GetAmmoShellCasingModelPath", pluginName, cmd_GetAmmoShellCasingModelPath, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, bool, TESAmmo*, BSFixedString>("SetAmmoShellCasingModelPath", pluginName, cmd_SetAmmoShellCasingModelPath, vm));


	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, bool, TESObjectREFR* ,TESObjectREFR*, TESObjectREFR*>("AreDirectlyConnected", pluginName, cmd_AreDirectlyConnected, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, TESObjectREFR*>("OutputPowerGrid", pluginName, cmd_OutputPowerGrid, vm));

	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, VMArray<TESObjectREFR*>, TESObjectREFR*, UInt32>("ScanPowerGridForFaults", pluginName, cmd_ScanPowerGridForFaults, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, bool, TESObjectREFR*, TESObjectREFR*, TESObjectREFR*>("IsDirectGridConnection", pluginName, cmd_IsDirectGridConnection, vm));


	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, VMArray<GridConnection>, TESObjectREFR*, TESObjectREFR*>("GetDirectGridConnections", pluginName, cmd_GetDirectGridConnections, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, bool, TESObjectREFR*, TESObjectREFR*, TESObjectREFR*>("IsSnappedGridConnection", pluginName, cmd_IsSnappedGridConnection, vm));


	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, TESObjectREFR*>("WorkshopCalculatePowerRating", pluginName, cmd_WorkshopCalculatePowerRating, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, TESObjectREFR*>("WorkshopRecalculateResources", pluginName, cmd_WorkshopRecalculateResources, vm));


	vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, bool, TESObjectREFR*, TESObjectREFR*, TESObjectREFR*, TESObjectREFR*>("RemoveGridConnection", pluginName, cmd_RemoveGridConnection, vm));
	vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, bool, TESObjectREFR*, TESObjectREFR*, TESObjectREFR*, TESObjectREFR*>("AddGridConnection", pluginName, cmd_AddGridConnection, vm));



	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, UInt32, Actor*>("ActorJump", pluginName, cmd_ActorJump, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, Actor*>("ActorIsInAir", pluginName, cmd_ActorIsInAir, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, Actor*>("ActorIsSwimming", pluginName, cmd_ActorIsSwimming, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, TESQuest*>("FailQuest", pluginName, cmd_FailQuest, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, BGSLocation*>("IsLocationEverCleared", pluginName, cmd_IsLocationEverCleared, vm));

	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, bool>("GetPlayerHasMapMarker", pluginName, cmd_GetPlayerHasMapMarker, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, SUPReferenceInfo>("GetPlayerMapMarkerInfo", pluginName, cmd_GetPlayerMapMarkerInfo, vm));

	vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, bool, TESWorldSpace*, float,float,float>("SetPlayerMapMarker", pluginName, cmd_SetPlayerMapMarker, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, bool>("RemovePlayerMapMarker", pluginName, cmd_RemovePlayerMapMarker, vm));

	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, UInt32, TESObjectREFR*, UInt32>("ScanPowerGridForFaultsEx", pluginName, cmd_ScanPowerGridForFaultsEx, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, UInt32, TESObjectREFR*, UInt32>("ScanPowerGridForFaultsAndFix", pluginName, cmd_ScanPowerGridForFaultsAndFix, vm));

	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, TESObjectREFR*>("IsObjectWirelessPowerReceiver", pluginName, cmd_IsObjectWirelessPowerReceiver, vm));
	

	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, UInt32>("IsSUPPatchActive", pluginName, cmd_IsSUPPatchActive, vm));
	
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, BSFixedString, float>("GetRadioStationName", pluginName, cmd_GetRadioStationName, vm));


	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, BSFixedString, TESForm*>("GetOverridingMod", pluginName, cmd_GetOverridingMod, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, VMArray<BSFixedString>, TESForm*>("GetOverridingMods", pluginName, cmd_GetOverridingMods, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, bool, TESForm*, BSFixedString>("IsOverridingMod", pluginName, cmd_IsOverridingMod, vm));

	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, TESObjectREFR*>("WorkshopCalculateCapacityAndLoad", pluginName, cmd_WorkshopCalculateCapacityAndLoad, vm));



	vm->RegisterFunction(new NativeFunction6<StaticFunctionTag, VMArray<TESObjectREFR*>, TESObjectCELL*, TESForm*, TESForm*,bool,bool, UInt32>("GetReferencesInCellWithKeyword", pluginName, cmd_GetReferencesInCellWithKeyword, vm));
	vm->RegisterFunction(new NativeFunction6<StaticFunctionTag, VMArray<TESObjectREFR*>, TESObjectCELL*, VMArray<BGSKeyword*>, VMArray<BGSKeyword*>, bool, bool, UInt32>("GetReferencesInCellWithKeywordAlt", pluginName, cmd_GetReferencesInCellWithKeywordAlt, vm));

	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, bool, TESObjectREFR*, TESObjectREFR*>("WorkshopGenerateFakeReferenceEvent", pluginName, cmd_WorkshopGenerateFakeReferenceEvent, vm));



	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, VMArray<TESObjectREFR*>, VMArray<TESObjectREFR*>, VMArray<ActorValueInfo*>, VMArray<ActorValueInfo*>>("FilterRefArrayByActorValues", pluginName, cmd_FilterRefArrayByActorValues, vm));
	vm->RegisterFunction(new NativeFunction8<StaticFunctionTag, VMArray<TESObjectREFR*>, TESObjectCELL*, UInt32, BSFixedString, UInt32, VMArray<BGSKeyword*>, VMArray<BGSKeyword*>,bool,bool>("GetReferencesInCellByType", pluginName, cmd_GetReferencesInCellByType, vm));

	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, VMArray<TESForm*>, TESObjectREFR*, UInt32, bool>("GetInventoryItemsByType", pluginName, cmd_GetInventoryItemsByType, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, VMArray<TESForm*>, TESObjectREFR*, VMArray<UInt32>, bool>("GetInventoryItemsByTypes", pluginName, cmd_GetInventoryItemsByTypes, vm));




	vm->RegisterFunction(new NativeFunction9<StaticFunctionTag, UInt32, BSFixedString, TESForm*, BSFixedString, BSFixedString, bool, bool, bool, UInt32, VMArray<VMVariable>>("RegisterForSUPEvent", pluginName, cmd_RegisterForSUPEvent, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, BSFixedString>("IsScriptLoaded", pluginName, cmd_IsScriptLoaded, vm));
	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, bool, BSFixedString, TESForm*>("IsScriptLoadedForForm", pluginName, cmd_IsScriptLoadedForForm, vm));

	vm->RegisterFunction(new NativeFunction10<StaticFunctionTag, UInt32, TESForm* ,BSFixedString, BSFixedString, bool,  float, UInt32, bool, UInt32, VMArray<VMVariable>, bool>("RegisterForGameMainLoopCallback", pluginName, cmd_RegisterForGameMainLoopCallback, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, UInt32>("RemoveFromGameMainLoopCallback", pluginName, cmd_RemoveFromGameMainLoopCallback, vm));

	vm->RegisterFunction(new NativeFunction10<StaticFunctionTag, UInt32,TESForm*, UInt32, UInt32, UInt32, UInt32, UInt32, UInt32, UInt32, UInt32, UInt32>("IsAnyOfTypes", pluginName, cmd_IsAnyOfTypes, vm));
	vm->RegisterFunction(new NativeFunction8<StaticFunctionTag, UInt32, BSFixedString, BSFixedString, BSFixedString, bool, bool, bool, UInt32, VMArray<VMVariable>>("SetEventHandler", pluginName, cmd_SetEventHandler, vm));


	vm->RegisterFunction(new NativeFunction6<StaticFunctionTag, VMArray<TESForm*>,TESObjectREFR*, TESForm*, TESForm*,bool,bool,bool>("GetInventoryItemsByKeyword", pluginName, cmd_GetInventoryItemsByKeyword, vm));
	vm->RegisterFunction(new NativeFunction6<StaticFunctionTag, VMArray<TESForm*>, TESObjectREFR*, VMArray<BGSKeyword*>, VMArray<BGSKeyword*>, bool, bool, bool>("GetInventoryItemsByKeywordAlt", pluginName, cmd_GetInventoryItemsByKeywordAlt, vm));


	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, bool, TESObjectREFR*, TESObjectREFR*, bool>("RadiatorEstablishConnections", pluginName, cmd_RadiatorEstablishConnections, vm));
	vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, UInt32, VMArray<TESObjectREFR*>, BSFixedString, BSFixedString, VMArray<VMVariable>>("NotifyReferenceScripts", pluginName, cmd_NotifyReferenceScripts, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, TESObjectREFR*>("WorkshopCleanUpEmptyPowerGrids", pluginName, cmd_WorkshopCleanUpEmptyPowerGrids, vm));
	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, bool>("IsSUPPexMismatched", pluginName, cmd_IsSUPPexMismatched, vm));



	vm->RegisterFunction(new NativeFunction5<StaticFunctionTag, bool, BSFixedString,TESForm*, BSFixedString, BSFixedString,bool>("IsSUPEventRegistered", pluginName, cmd_IsSUPEventRegistered, vm));

	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, bool, TESObjectREFR*, BSFixedString, BSFixedString>("AttachPapyrusScript", pluginName, cmd_AttachPapyrusScript, vm));

	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, BGSKeyword*, TESObjectREFR*>("GetTerminalLinkKeyword", pluginName, cmd_GetTerminalLinkKeyword, vm));

	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, TESObjectREFR*>("ReestablishTerminalLinks", pluginName, cmd_ReestablishTerminalLinks, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, BGSKeyword*, BSFixedString>("GetKeywordByName", pluginName, cmd_GetKeywordByName, vm));
	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, ActorValueInfo*, BSFixedString>("GetActorValueByName", pluginName, cmd_GetActorValueByName, vm));

	vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, bool>("IsLastGameLoadedFromMainMenu", pluginName, cmd_IsLastGameLoadedFromMainMenu, vm));

	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, BSFixedString, BSFixedString>("StringRemoveWhiteSpace", pluginName, cmd_StringRemoveWhiteSpace, vm));

	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, BSFixedString, UInt32>("GetLastLoadedSaveName", pluginName, cmd_GetLastLoadedSaveName, vm));
	vm->RegisterFunction(new NativeFunction5<StaticFunctionTag, UInt32, BSFixedString, TESForm*, bool, BSFixedString, BSFixedString>("UnregisterForAllSUPEvents", pluginName, cmd_UnregisterForAllSUPEvents, vm));

	vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, UInt32, BGSInstanceNamingRules*>("GetInstanceNamingRulesType", pluginName, cmd_GetInstanceNamingRulesType, vm));

	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, VMArray<TESObjectREFR*>, VMArray<TESObjectREFR*>, VMArray<TESObjectREFR*>>("FilterObjectRefArrayByRefs", pluginName, cmd_FilterObjectRefArrayByRefs, vm));
	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, VMArray<TESObjectREFR*>, VMArray<TESObjectREFR*>, TESObjectREFR*, UInt32>("FilterObjectRefArrayBySharedPowergrid", pluginName, cmd_FilterObjectRefArrayBySharedPowergrid, vm));


	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, VMArray<TESObjectREFR*>, TESObjectREFR*, TESObjectREFR*>("GetDirectGridConnectionsRefs", pluginName, cmd_GetDirectGridConnectionsRefs, vm));




	vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, UInt32, VMArray<TESObjectREFR*>, VMArray<BSFixedString>, BSFixedString, VMArray<VMVariable>>("NotifyReferenceScriptsEx", pluginName, cmd_NotifyReferenceScriptsEx, vm));

	vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, bool, UInt32, UInt32, UInt32>("IsIPInTheSameSubnet", pluginName, cmd_IsIPInTheSameSubnet, vm));


	vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, VMArray<BSFixedString>, BSFixedString, UInt32>("FolderGetContentsAlt", pluginName, cmd_FolderGetContentsAlt, vm));

	//vm->RegisterFunction(new NativeFunction8<StaticFunctionTag, UInt32, BSFixedString, BSFixedString, BSFixedString,bool,bool,bool, UInt32, VMArray<VMVariable>>("SetEventHandler", pluginName, cmd_SetEventHandler, vm));




	//vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, bool, TESObjectREFR*, TESObjectREFR*>("RemoveConnectedObject", pluginName, cmd_RemoveConnectedObject, vm));

	//vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, TESObjectREFR*>("SUPGetCurrentConsoleReference", pluginName, cmd_SUPGetCurrentConsoleReference, vm));


	return true;
}