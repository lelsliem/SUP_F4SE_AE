#include "sup/util/string.hpp"




//bool cmd_IsMenuOpened(StaticFunctionTag*, BSFixedString menuName)
//{
//	_DMESSAGE("IsMenuOpen started with request of >>%s", menuName.c_str());
//	return (*g_ui)->IsMenuOpen(menuName.c_str());
//}





bool cmd_IsFormValid(StaticFunctionTag*, TESForm* thisForm)
{
	return f_IsFormValid(thisForm);
}

VMArray<TESForm*> cmd_FormListToArray(StaticFunctionTag* base, BGSListForm* formList)
{
	VMArray<TESForm*> result;


	if (!formList) return result;

	for (UInt32 i = 0; i < formList->arrayOfForms.size(); i++)
	{
		TESForm* form = formList->arrayOfForms[i];
		if (form)
			result.Push(&form);

	}

	return result;
}



void cmd_DebugPrintSUP(StaticFunctionTag* base, BSFixedString s_Message, UInt32 iRequest)// 0 for Console, 1 for File, 2 for Both 3 to debug file
{

    if (iRequest == 300)
    {
        _DMESSAGE("%s", s_Message.c_str());
        return;
    }


	if (iRequest == 0 || iRequest == 2)
	{
		Console_Print(s_Message.c_str());
	}


	if (iRequest == 1 || iRequest == 2)
	{
		ofstream myfile;
		myfile.open("aaSUPF4SEDebugPrint.txt", std::ios_base::app);

		if (!myfile.is_open())
			return;

		myfile << endl;

		myfile << s_Message.c_str();
		myfile.close();

	}

}





TESForm* cmd_GetSplineForm(StaticFunctionTag* base, TESForm* splineForm)
{
    if (!splineForm) {
        BGSDefaultObject* splineDefault = (*g_defaultObjectMap)->GetDefaultObject("WorkshopSplineObject");
        if (splineDefault) {
            splineForm = splineDefault->form;
        }
    }
    if (!splineForm)
    {
        return nullptr;
    }

    BGSBendableSpline* spline = DYNAMIC_CAST(splineForm, TESForm, BGSBendableSpline);

    return spline;
}



bool cmd_DrawSpline(StaticFunctionTag* base, TESObjectREFR* refA, TESObjectREFR* refB, TESObjectREFR* splineRef, float Thickness, float Slack)
{
 
   _DMESSAGE("DrawLineStarts");

    // No specified spline, no refs, refs are same item, or no 3D loaded
    if (!splineRef || !refA || !refB || refA == refB || !refA->Get3D() || !refB->Get3D()) {
        _DMESSAGE("DrawSpline::Check fail");
        if (!refA)
        {
            _DMESSAGE("refA not valid");
        }

        if (!refB)
        {
            _DMESSAGE("refB not valid");
        }

        if (refA == refB)
        {
            _DMESSAGE("refA == refB");
        }


        return false;
    }


    BGSBendableSpline* splineA = DYNAMIC_CAST(refA->GetObjectReference(), TESForm, BGSBendableSpline);
    BGSBendableSpline* splineB = DYNAMIC_CAST(refB->GetObjectReference(), TESForm, BGSBendableSpline);



    UInt32 nullHandle = *g_invalidRefHandle;

    TESObjectCELL* parentCell = splineRef->parentCell;

    TESWorldSpace* worldspace = splineRef->parentCell ? splineRef->parentCell->worldSpace : nullptr;

    NiPoint3 rot;
    NiPoint3 refAPos = refA->GetPosition();
    MoveRefrToPosition(splineRef, &nullHandle, parentCell, worldspace, &refAPos, &rot);

    Workshop::ContextData contextData(*g_player);

    sup::compat::SplineUtils::UpdateSpline(&contextData, splineRef, refB, 0, refA, 0);
    RE::SplineUtils::ConnectSpline(refA, 0, refB, 0, splineRef);

    ExtraBendableSplineParams* splineParams = (ExtraBendableSplineParams*)splineRef->extraDataList->GetByType(kExtraData_BendableSplineParams);
    if (splineParams) {
        // RE keeps ParamData members private; access via a layout-compatible mirror
        // (F4SE layout: slack @ 0x18, thickness @ 0x1C within the extra data).
        struct SUP_SplineParams { float slack; float thickness; };
        auto* sp = reinterpret_cast<SUP_SplineParams*>(&splineParams->data);
        sp->thickness = Thickness;
        sp->slack = Slack;
        //_DMESSAGE("extra data exists");
        //_DMESSAGE("unk18 is %f", splineParams->unk18); //unk18 is SLACK
       // _DMESSAGE("unk2Cis %f", splineParams->unk2C);
    }


    _DMESSAGE("SplineRef drawn");
    return true;
}








TESObjectREFR* cmd_GetWorldLocationMarkerRef(StaticFunctionTag* base, BGSLocation* Location)
{
    NiPointer<TESObjectREFR> refr;


    if (!Location)
        return NULL;

    if (!Location->worldLocMarker)
    {
        _DMESSAGE("Marker doesn't exist 1");
        return nullptr;
    }

    LookupREFRByHandle(Location->worldLocMarker, refr);

    if (!refr)
    {
        _DMESSAGE("Marker doesn't exist 2");
        return NULL;
    }

    return refr.get();
}



UInt32 f_IsMenuModeActive()
{
    if ((*g_ui) == NULL)
    {
        return 1;
    }
    _DMESSAGE("*g_ui exists");

    if ((*g_ui)->IsMenuOpen("DialogueMenu"))
    {
        return 1;
    }
    if ((*g_ui)->IsMenuOpen("BarterMenu"))
    {
        return 1;
    }
    if ((*g_ui)->IsMenuOpen("WorkshopMenu"))
    {
        return 1;
    }
    if ((*g_ui)->IsMenuOpen("LooksMenu"))
    {
        return 1;
    }
    if ((*g_ui)->IsMenuOpen("VATSMenu"))
    {
        return 1;
    }

   // _DMESSAGE("Returning (*g_ui)->numPauseGame() ");
    return (*g_ui)->numPauseGame();
}


UInt32 cmd_IsMenuModeActive(StaticFunctionTag* base)
{
    return f_IsMenuModeActive();
}


UInt32 cmd_IsGamePaused(StaticFunctionTag* base)
{
    if ((*g_ui) == NULL)
    {
        return 1;
    }

    return (*g_ui)->numPauseGame();
}


UInt32 cmd_IsNewGame(StaticFunctionTag* base)
{
    return g_BBLastNewGame;
}



ScreenPos cmd_WorldToScreen(StaticFunctionTag* base, float fPosX, float fPosY, float fPosZ, UInt32 iNormalize)
{
    _DMESSAGE("W2S starts, X::%f,Y::%f,Z::%f, Normalize is>>>%d", fPosX, fPosY, fPosZ, iNormalize);

    ScreenPos result;

    NiPoint3 pIN, pOUT;

    int iOnScreen = 1;

    pIN.x = fPosX;
    pIN.y = fPosY;
    pIN.z = fPosZ;

	WorldToScreen_Internal(&pIN, &pOUT);



    if (pOUT.x > 1)
    {
        if (iNormalize)
        {
            pOUT.x = 1;
        }

        iOnScreen = 0;
    }
    else if (pOUT.x < 0)
    {
        if (iNormalize)
        {
            pOUT.x = 0;
        }
        iOnScreen = 0;
    }

    if (pOUT.y > 1)
    {
        if (iNormalize)
        {
            pOUT.y = 1;
        }
        iOnScreen = 0;
    }
    else if (pOUT.y < 0)
    {
        if (iNormalize)
        {
            pOUT.y = 0;
        }
        iOnScreen = 0;
    }



    result.Set<float>("x", pOUT.x);
    result.Set<float>("y", pOUT.y);
    result.Set<UInt32>("OnScreen", iOnScreen);

    _DMESSAGE("W2S pos is X::%f,Y::%f,Z::%f", pOUT.x, pOUT.y, pOUT.z);
    return result;
}




VMArray <SUPFurnitureMarkerData> cmd_GetFurnitureMarkersFromFurniture(StaticFunctionTag*, TESForm* thisForm)
{
    _DMESSAGE("GetFurnitureMarkersFromFurniture starts");
    VMArray<SUPFurnitureMarkerData> result;


    if (!thisForm)
    {
        _DMESSAGE("GetFurnitureMarkersFromFurniture::non valid form");
        return result;
    }

    if (thisForm->formType == kFormType_FURN)
    {
        _DMESSAGE("Furniture");
    }
    else {
        _DMESSAGE("Not furniture");
        return result;
    }


    TESFurnitureSUP* pFurnitureSUP = static_cast<TESFurnitureSUP*>(thisForm);

    //_DMESSAGE("marker count>>>%d", pFurnitureSUP->markersArray.count);


    for (UInt64 i = 0; i < pFurnitureSUP->markersArray.count; i++)
    {
        _DMESSAGE("NEW MARKER");
        SUPFurnitureMarker MarkerInst;
        pFurnitureSUP->markersArray.GetNthItem(i, MarkerInst);
        _DMESSAGE("MarkerX>>%f", MarkerInst.pos.x);
        _DMESSAGE("MarkerY>>%f", MarkerInst.pos.y);
        _DMESSAGE("MarkerZ>>%f", MarkerInst.pos.z);
        _DMESSAGE("Marker ROT>>%f", MarkerInst.Rotation);
        _DMESSAGE("Marker ROT FIXED>>%f", f_Rad_To_Angle(MarkerInst.Rotation));
        _DMESSAGE("Marker flags>>%d", MarkerInst.allowedAnimations);

        SUPFurnitureMarkerData ResultElement;
        ResultElement.Set<UInt32>("ID", i);
        ResultElement.Set<float>("X", MarkerInst.pos.x);
        ResultElement.Set<float>("Y", MarkerInst.pos.y);
        ResultElement.Set<float>("Z", MarkerInst.pos.z);
        ResultElement.Set<float>("Rot", f_Rad_To_Angle(MarkerInst.Rotation));
        ResultElement.Set<UInt32>("EntryTypes", MarkerInst.allowedAnimations);
        ResultElement.Set<TESForm*>("FurnKeyWord", MarkerInst.Keyword);
        result.Push(&ResultElement);
    }


    return result;
}







TESForm * cmd_GetFormFromFileEx(StaticFunctionTag* base, BSFixedString sFormID, BSFixedString sPluginFileName, UInt32 iLookForESLFirst)
{
    _DMESSAGE("GetFormFromFileEX started with %s %s", sFormID.c_str(), sPluginFileName.c_str());
    return BuildRefFormFromBSSTR(sFormID,sPluginFileName,iLookForESLFirst);
}





UInt32 cmd_StringToDec(StaticFunctionTag* base, BSFixedString sFormID)
{

    string s_FormIDs = sFormID.c_str();

    UInt32 form_Dec = 0;

    try
    {
        form_Dec = std::stoul(s_FormIDs, nullptr, 16);
    }
    catch (std::invalid_argument e)
    {
        _DMESSAGE("HeX threw exception inv agr - skipping");
        return 0;
    }
    catch (std::out_of_range  e)
    {
        _DMESSAGE("HeX threw exception out of range - skipping");
        return 0;
    }

    return form_Dec;
}



//bool cmd_HasConsoleBeenOpened(StaticFunctionTag* base)
//{
//    BSFixedString MenuName("Console");
//    return (*g_ui)->GetMenu(MenuName);
//}


float cmd_GetGlobalTimeMultiplier(StaticFunctionTag* base)
{
    // The real game global (previously a local copy that never tracked the game).
    return RE::BSTimer::QGlobalTimeMultiplier();
}



bool cmd_SetGlobalTimeMultiplier(StaticFunctionTag* base, float fTimeMult)
{
    _DMESSAGE("SetGlobalTimeMultiplier started");

    // The game's own setter (the same one the "sgtm" console command drives), so the
    // timescale actually changes instead of shelling out through the console command.
    if (auto* timer = RE::BSTimer::GetSingleton()) {
        timer->SetGlobalTimeMultiplier(fTimeMult, true);
        return true;
    }
    return false;
}






bool cmd_IsScriptLoaded(StaticFunctionTag* base, BSFixedString ScriptName)
{
    string sMyScriptName = ScriptName.c_str();
    sup::string::to_lower(sMyScriptName);

    return AttachedScriptReader::IsScriptLoaded(sMyScriptName);
}

bool cmd_IsScriptLoadedForForm(StaticFunctionTag* base, BSFixedString ScriptName, TESForm* MyForm)
{
    if (!MyForm)
    {
        _DMESSAGE("Form is not valid");
        return false;
    }

    string sMyScriptName = ScriptName.c_str();
    sup::string::to_lower(sMyScriptName);

    auto result = AttachedScriptReader::IsScriptLoadedForForm(sMyScriptName, MyForm);

    return result.bSuccess;
}


VMArray<VMVariable> cmd_MergeArrays(StaticFunctionTag* tag, VMArray<VMVariable> arr1, VMArray<VMVariable> arr2)
{

    
    VMArray<VMVariable>  result;
    UInt32 argCount1 = arr1.Length();

    for (UInt32 i = 0; i < argCount1; i++)
    {
        VMVariable var;
        arr1.Get(&var, i);
        result.Push(&var);
    }

    UInt32 argCount2 = arr2.Length();

    for (UInt32 i = 0; i < argCount2; i++)
     {
       VMVariable var;
       arr2.Get(&var, i);
       result.Push(&var);
     }
    

    return result;
}





VMArray<float> cmd_GetAllRadioFrequencies(StaticFunctionTag* tag)
{
    VMArray<float> result;
    for (auto fValue : vCurrentRadioFrequencies) {
        result.Push(&fValue);
    }
    return result;
}




bool cmd_SetChargenPresetFlag(StaticFunctionTag* tag, TESForm* thisForm, bool bTrue)
{
    if (!thisForm)
    {
        _DMESSAGE("SetChargenPresetFlag::form is not valid");
        return false;
    }


    TESActorBaseData* pTESActorData = DYNAMIC_CAST(thisForm, TESForm, TESActorBaseData);

    if (!pTESActorData) {
        _DMESSAGE("SetChargenPresetFlag:: no TESActorBaseData for this ref.");
        return false;
    }

    auto& presetFlag = pTESActorData->actorData.actorBaseFlags;
    _DMESSAGE("Flags before are>>%d", presetFlag.underlying());

    if (bTrue)
    {
        presetFlag.set(ACTOR_BASE_DATA::Flag::kIsChargenFacePreset);
    }
    else {
        presetFlag.reset(ACTOR_BASE_DATA::Flag::kIsChargenFacePreset);
    }

    _DMESSAGE("Flags after are>>%d", presetFlag.underlying());



    return true;
}





//const ModInfo* f_LookupLoadedModByIndex(UInt8 iModIndex)
//{
//    for (UInt32 i = 0; i < (*g_dataHandler)->modList.loadedMods.count; i++) {
//        ModInfo* modInfo = (*g_dataHandler)->modList.loadedMods[i];
//        if ((modInfo->modIndex, iModIndex) == 0)
//            return modInfo;
//    }
//
//}
//
//const ModInfo* f_LookupLoadedLightModByIndex(UInt8 iModIndex)
//{
//    for (UInt32 i = 0; i < (*g_dataHandler)->modList.lightMods.count; i++) {
//        ModInfo* modInfo = (*g_dataHandler)->modList.lightMods[i];
//        if ((modInfo->modIndex, iModIndex) == 0)
//            return modInfo;
//    }

//}



VMArray<TESForm*> cmd_GetLoadedFormsByType(StaticFunctionTag* tag, UInt32 iType, BSFixedString PluginName)
{

    string sPluginName = PluginName.c_str();
    _DMESSAGE("GetLoadedFormsByType started, type is>>%d,PluginName>>>%s", iType, sPluginName.c_str());

    VMArray<TESForm*> result;


    if (iType > 158)
    {
        _DMESSAGE("Type exceeds max value");
        return result;
    }
    else if (iType < 1)
    {
        _DMESSAGE("Type  is less than 1");
        return result;
    }


    const TESFile* ChosenMod = NULL;

    if (sPluginName.length() > 0)
    {
        ChosenMod = (*g_dataHandler)->LookupLoadedModByName(sPluginName.c_str());

        if (!ChosenMod)
        {
            _DMESSAGE("Mod not found --- Looking for light mod");
            ChosenMod = (*g_dataHandler)->LookupLoadedLightModByName(sPluginName.c_str());
        }

        if (!ChosenMod)
        {
            _DMESSAGE("Specified mod is not loaded");
            return result;
        }
        _DMESSAGE("Mod loaded.");

    }

    BSTArray<TESForm*>& MyArr = (*g_dataHandler)->formArrays[iType];



    for (std::size_t i = 0; i < MyArr.size(); i++)
    {
        //_DMESSAGE("New Element, index is>>%d", i);
        TESForm* CurrentObject = MyArr[i];

        if (CurrentObject)
        {
            if (ChosenMod)
            {
                if (ChosenMod->IsFormInMod(CurrentObject->formID))
                {
                    result.Push(&CurrentObject);
                }

            }
            else {
                result.Push(&CurrentObject);
            }
        }

    }

    return result;

}


bool cmd_SetAmmoProjectile(StaticFunctionTag* tag, TESAmmo* MyAmmo, BGSProjectile* MyProjectile)
{
    _DMESSAGE("SetAmmoProjectile starts");
    if (!MyAmmo)
    {
        _DMESSAGE("SetAmmoProjectile::Ammo not valid");
        return false;
    }

    if (!MyProjectile)
    {
        _DMESSAGE("SetAmmoProjectile::Projectile not valid");
        return false;
    }

    MyAmmo->data.projectile = MyProjectile;
    return true;
}

BGSProjectile* cmd_GetAmmoProjectile(StaticFunctionTag* tag, TESAmmo* MyAmmo)
{
    _DMESSAGE("GetAmmoProjectile starts");
    if (!MyAmmo)
    {
        _DMESSAGE("SetAmmoProjectile::Ammo not valid");
        return NULL;
    }

    return MyAmmo->data.projectile;
}


BSFixedString cmd_GetAmmoShellCasingModelPath(StaticFunctionTag* tag, TESAmmo* MyAmmo)
{
    _DMESSAGE("GetAmmoShellCasingModelPath starts");

    if (!MyAmmo)
    {
        _DMESSAGE("GetAmmoShellCasingModelPath::Ammo not valid");
        return "";
    }

    return MyAmmo->shellCasing.GetModel();
}


bool cmd_SetAmmoShellCasingModelPath(StaticFunctionTag* tag, TESAmmo* MyAmmo, BSFixedString NewPath)
{
    _DMESSAGE("SetAmmoShellCasingModelPath starts");

    if (!MyAmmo)
    {
        _DMESSAGE("SetAmmoShellCasingModelPath::Ammo not valid");
        return "";
    }

    MyAmmo->shellCasing.SetModel(NewPath.c_str());
    return true;
}



bool cmd_FailQuest(StaticFunctionTag* tag, TESQuest* MyQuest)
{

    if (!MyQuest)
    {
        _DMESSAGE("Quest is not valid");
        return false;
    }

    // TESQuest::FailQuest is not modeled by CommonLibF4 (nor available in F4SE 0.7.7).
    _DMESSAGE("FailQuest: Function not available");
    return false;

}









bool cmd_IsSUPPatchActive(StaticFunctionTag* tag, UInt32 iRequest)
{
    return fn_ext_IsSUPPatchActive(iRequest);
}







BSFixedString cmd_GetRadioStationName(StaticFunctionTag* tag, float fFrequency)
{
    _DMESSAGE("GetRadioStationName starts");

    const char* RadioStatName = GetRadioStationName(fFrequency);

    if (RadioStatName)
    {
        return RadioStatName;
    }
    else {
        return "";
    }

}



TESForm* CheckForRefAndGetForm(TESForm* CurrentForm)
{

    if (!CurrentForm)
    {
        _DMESSAGE("Form doesn't exist");
        return NULL;;
    }

    TESObjectREFR* CurrentRef = (TESObjectREFR*)DYNAMIC_CAST(CurrentForm, TESForm, TESObjectREFR);

    if (CurrentRef)
    {
        if (CurrentRef->GetObjectReference())
        {
            return CurrentRef->GetObjectReference();
        }
        else {
            return NULL;
        }
    }
    else {
        return CurrentForm;
    }


}

BSFixedString cmd_GetOverridingMod(StaticFunctionTag* tag, TESForm* CurrentForm)
{
    _DMESSAGE("GetOverridingMod starts");

    CurrentForm = CheckForRefAndGetForm(CurrentForm);

    if (!CurrentForm)
    {
        _DMESSAGE("Form does not exists");
        return "";
    }

    auto* srcFiles = GetFormFileArray(CurrentForm);
    if (!srcFiles || srcFiles->empty())
    {
        _DMESSAGE("Form is not overriden by anything.");
        return "";
    }
    int iPos = static_cast<int>(srcFiles->size()) - 1;
    return (*srcFiles)[iPos]->filename;
}



VMArray<BSFixedString> cmd_GetOverridingMods(StaticFunctionTag* tag, TESForm* CurrentForm)
{
    VMArray<BSFixedString> result;
    _DMESSAGE("GetOverridingMods starts");

    CurrentForm = CheckForRefAndGetForm(CurrentForm);

    if (!CurrentForm)
    {
        _DMESSAGE("Form does not exists");
        return result;
    }

    auto* srcFiles = GetFormFileArray(CurrentForm);
    if (!srcFiles || srcFiles->empty())
    {
        _DMESSAGE("Form is not overriden by anything.");
        return result;
    }

    for (std::size_t i = 0; i < srcFiles->size(); i++)
    {
        BSFixedString sTmp((*srcFiles)[i]->filename);
        result.Push(&sTmp);
    }

    return result;
}


bool cmd_IsOverridingMod(StaticFunctionTag* tag, TESForm* CurrentForm, BSFixedString ModName)
{

    _DMESSAGE("IsOverridingMod starts");

    CurrentForm = CheckForRefAndGetForm(CurrentForm);

    if (!CurrentForm)
    {
        _DMESSAGE("Form does not exists");
        return false;
    }

    auto* srcFiles = GetFormFileArray(CurrentForm);
    if (!srcFiles || srcFiles->empty())
    {
        _DMESSAGE("Form is not overriden by anything.");
        return false;
    }

    int iPos = static_cast<int>(srcFiles->size()) - 1;
    return sup::string::iequals(ModName.c_str(), (*srcFiles)[iPos]->filename);
}







VMArray<TESForm*> cmd_GetInventoryItemsByType(StaticFunctionTag* tag, TESObjectREFR* ObjectRef, UInt32 type, bool bReturnMultiple)
{
    VMArray<TESForm*> result;
    _DMESSAGE("GetInventoryItemsByType starts");

    if (!ObjectRef)
    {
        _DMESSAGE("Object Ref not valid");
        return result;
    }

    if (!ObjectRef->inventoryList)
    {
        _DMESSAGE("Inventory list doesn't exist for this ref");
        return result;
    }

    if (!ObjectRef->inventoryList->data.size())
    {
        _DMESSAGE("Inventory list count is 0");
        return result;
    }

    // RE: BGSInventoryList lock is not modeled; reads are not externally locked.


    UInt32 iPos = 0;

    while (iPos < ObjectRef->inventoryList->data.size())
    {
        TESForm* CurrentForm = ObjectRef->inventoryList->data[iPos].object;

        if (CurrentForm && CurrentForm->formType.underlying() == type)
        {
            if (bReturnMultiple)
            {
                BGSInventoryItem::Stack* CurrentStack = ObjectRef->inventoryList->data[iPos].stackData.get();
                if (CurrentStack)
                {
                    for (auto i = 0; i < CurrentStack->count;i++)
                    {
                        i++;
                        result.Push(&CurrentForm);
                    }

                    
                    while (CurrentStack->nextStack)
                    {
                        CurrentStack = CurrentStack->nextStack.get();
                        for (auto i = 0; i < CurrentStack->count; i++)
                        {
                            i++;
                            result.Push(&CurrentForm);
                        }
                    }

                }
            }
            else {
                result.Push(&CurrentForm);
            }

        }
        iPos++;
    }

    // (RE: no modeled lock to release)

    _DMESSAGE("GetInventoryItemsByType ends");
    return result;
}



VMArray<TESForm*> cmd_GetInventoryItemsByTypes(StaticFunctionTag* tag, TESObjectREFR* ObjectRef, VMArray<UInt32> types, bool bReturnMultiple)
{
    VMArray<TESForm*> result;
    _DMESSAGE("GetInventoryItemsByType starts");

    if (!ObjectRef)
    {
        _DMESSAGE("Object Ref not valid");
        return result;
    }

    if (!ObjectRef->inventoryList)
    {
        _DMESSAGE("Inventory list doesn't exist for this ref");
        return result;
    }

    if (!ObjectRef->inventoryList->data.size())
    {
        _DMESSAGE("Inventory list count is 0");
        return result;
    }

    UInt32 iPos = 0;

    vector<UInt32> vTypes = ConvertVMArrayToVector(types);


    if (!vTypes.size())
    {
        _DMESSAGE("Types array is empty");
        return result;
    }

    // RE: BGSInventoryList lock is not modeled; reads are not externally locked.

    while (iPos < ObjectRef->inventoryList->data.size())
    {
        TESForm* CurrentForm = ObjectRef->inventoryList->data[iPos].object;

        for (auto it = vTypes.begin(); it != vTypes.end(); it++)
        {
            if (CurrentForm && CurrentForm->formType.underlying() == Iter)
            {
                if (bReturnMultiple)
                {
                    BGSInventoryItem::Stack* CurrentStack = ObjectRef->inventoryList->data[iPos].stackData.get();
                    if (CurrentStack)
                    {
                        for (auto i = 0; i < CurrentStack->count; i++)
                        {
                            i++;
                            result.Push(&CurrentForm);
                        }


                        while (CurrentStack->nextStack)
                        {
                            CurrentStack = CurrentStack->nextStack.get();
                            for (auto i = 0; i < CurrentStack->count; i++)
                            {
                                i++;
                                result.Push(&CurrentForm);
                            }
                        }

                    }
                }
                else {
                    result.Push(&CurrentForm);
                }
            }
        }


        iPos++;
    }

    // (RE: no modeled lock to release)

    _DMESSAGE("GetInventoryItemsByType ends");
    return result;
}
















bool __forceinline PositiveCheckforItemKeywords(BGSKeywordForm* pKeywords,TESForm* TempRef, vector<BGSKeyword*>& KeywordToInclude, vector<BGSKeyword*>& KeywordToExclude, bool bCheckForAnyIncluded, bool bCheckForAnyExcluded, int& iKeywordToExcludeSize)
{
    bool bFound = false;


    if (bCheckForAnyIncluded)
    {

            for (UInt32 i = 0; i < pKeywords->numKeywords; i++)
            {
                if (bFound)
                {
                    break;
                }

                BGSKeyword* CurrentKeyword = pKeywords->keywords[i];

                if (!CurrentKeyword)
                {
                    continue;
                }

                for (auto it = KeywordToInclude.begin(); it != KeywordToInclude.end(); it++) { 
                    if (CurrentKeyword->formID == Iter->formID) 
                    {
                        bFound = true;
                        break;
                    }
                }

            }



    }
    else {
        bFound = true;


        for (auto it = KeywordToInclude.begin(); it != KeywordToInclude.end(); it++) {
        
            bool bFoundinArr = false;

            for (UInt32 i = 0; i < pKeywords->numKeywords; i++)
            {
                BGSKeyword* CurrentKeyword = pKeywords->keywords[i];

                if (!CurrentKeyword)
                {
                    continue;
                }
                if (CurrentKeyword->formID == Iter->formID)
                {
                    _DMESSAGE("Found in passed keyword array.");
                    bFoundinArr = true;
                    break;
                }
            }

            if (!bFoundinArr)
            {
                _DMESSAGE("Keyword NOT FOUND - SKIPPING ITEM");
                bFound = false;
                break;
            }

        }


        //for (UInt32 i = 0; i < pKeywords->numKeywords; i++)
        //{
        //    BGSKeyword* CurrentKeyword = pKeywords->keywords[i];

        //   _DMESSAGE("Keyword in item array>>%x", CurrentKeyword->formID);
        //    if (!CurrentKeyword)
        //    {
        //        continue;
        //    }
        //    bool bFoundinArr = false;

        //    for (auto it = KeywordToInclude.begin(); it != KeywordToInclude.end(); it++) {
        //        if (CurrentKeyword->formID == Iter->formID)
        //        {
        //            _DMESSAGE("Found in passed keyword array.");
        //            bFoundinArr = true;
        //            break;
        //        }
        //    }

        //    if(!bFoundinArr)
        //    {
        //        _DMESSAGE("Keyword NOT FOUND - SKIPPING ITEM");
        //        bFound = false;
        //        break;
        //    }

        //}

    }

    if (!bFound)
    {
       _DMESSAGE("Item not found - returning to loop");
        return false;
    }

    if (!iKeywordToExcludeSize)
    {
        return true;
    }


    if (bCheckForAnyExcluded)
    {

        for (UInt32 i = 0; i < pKeywords->numKeywords; i++)
        {
            BGSKeyword* CurrentKeyword = pKeywords->keywords[i];

            if (!CurrentKeyword)
            {
                continue;
            }

            for (auto it = KeywordToExclude.begin(); it != KeywordToExclude.end(); it++) {
                if (CurrentKeyword->formID == Iter->formID)
                {
                    return false;
                }
            }
        }

    }
    else {
        int iCount = 0;
        for (auto it = KeywordToExclude.begin(); it != KeywordToExclude.end(); it++) {
        
            bool bFoundinArr = false;

            for (UInt32 i = 0; i < pKeywords->numKeywords; i++)
            {
                BGSKeyword* CurrentKeyword = pKeywords->keywords[i];

               if (!CurrentKeyword)
               {
                   continue;
               }

               if (CurrentKeyword->formID == Iter->formID)
               {
                    bFoundinArr = true;
                    break;
               }

            }

            if (!bFoundinArr)
            {
                iCount += 1;
            }

        }

        if (iCount != iKeywordToExcludeSize)
        {
            return false;
        }


    }





    return true;
}










VMArray<TESForm*> cmd_GetInventoryItemsByKeyword(StaticFunctionTag* tag, TESObjectREFR* ObjectRef, TESForm* KeywordToInclude, TESForm* KeywordToExclude, bool bCheckForAnyIncluded, bool bCheckForAnyExcluded, bool bReturnMultiple)
{
    VMArray<TESForm*> result;
    _DMESSAGE("GetInventoryItemsByKeyword starts");

    if (!ObjectRef)
    {
        _DMESSAGE("Object Ref not valid");
        return result;
    }

    if (!KeywordToInclude)
    {
        _DMESSAGE("KeywordToInclude not valid");
        return result;
    }

    if (!ObjectRef->inventoryList)
    {
        _DMESSAGE("Inventory list doesn't exist for this ref");
        return result;
    }

    if (!ObjectRef->inventoryList->data.size())
    {
        _DMESSAGE("Inventory list count is 0");
        return result;
    }


    vector<BGSKeyword*> vKeywordToInclude{};
    FillKeywordsArrayByForm(vKeywordToInclude, KeywordToInclude);
    
    if (bDebugMode)
    {
        for (auto it = vKeywordToInclude.begin(); it != vKeywordToInclude.end(); it++)
        {
            _DMESSAGE("Included keywords>>>%x", Iter->formID);
        }
    }





    if (vKeywordToInclude.size() == 0)
    {
        _DMESSAGE("keyword vector size is null");
        return result;
    }
    vector<BGSKeyword*> vKeywordToExclude{};
    if (KeywordToExclude)
    {
        FillKeywordsArrayByForm(vKeywordToExclude, KeywordToExclude);
    }

    int iKeywordToExclude = vKeywordToExclude.size();

    // RE: BGSInventoryList lock is not modeled; reads are not externally locked.


    UInt32 iPos = 0;

    while (iPos < ObjectRef->inventoryList->data.size())
    {
        TESForm* CurrentForm = ObjectRef->inventoryList->data[iPos].object;

        if (!CurrentForm)
        {
            iPos++;
            continue;
        }
        _DMESSAGE("Checking inventory item>>%x", CurrentForm->formID);

        BGSKeywordForm* pKeywords = DYNAMIC_CAST(CurrentForm, TESForm, BGSKeywordForm);

        if (!pKeywords) {
            iPos++;
            continue;
        }
            
        if (!pKeywords->numKeywords)
        {
            iPos++;
            continue;
        }

        if (PositiveCheckforItemKeywords(pKeywords, CurrentForm, vKeywordToInclude, vKeywordToExclude, bCheckForAnyIncluded, bCheckForAnyExcluded, iKeywordToExclude)) {
           _DMESSAGE("InevntoryItem passed keyworkd check - adding.");
            if (bReturnMultiple)
            {
                BGSInventoryItem::Stack* CurrentStack = ObjectRef->inventoryList->data[iPos].stackData.get();
                if (CurrentStack)
                {
                    for (auto i = 0; i < CurrentStack->count; i++)
                    {
                        i++;
                        result.Push(&CurrentForm);
                    }


                    while (CurrentStack->nextStack)
                    {
                        CurrentStack = CurrentStack->nextStack.get();
                        for (auto i = 0; i < CurrentStack->count; i++)
                        {
                            i++;
                            result.Push(&CurrentForm);
                        }
                    }
                }
            }
            else {
                result.Push(&CurrentForm);
            }

        }

        iPos++;
    }

    // (RE: no modeled lock to release)

    _DMESSAGE("GetInventoryItemsByKeyword ends");
    return result;
}





VMArray<TESForm*> cmd_GetInventoryItemsByKeywordAlt(StaticFunctionTag* tag, TESObjectREFR* ObjectRef, VMArray<BGSKeyword*> KeywordsToInclude, VMArray<BGSKeyword*> KeywordsToExclude, bool bCheckForAnyIncluded, bool bCheckForAnyExcluded, bool bReturnMultiple)
{
    VMArray<TESForm*> result;
    _DMESSAGE("GetInventoryItemsByKeyword starts");

    if (!ObjectRef)
    {
        _DMESSAGE("Object Ref not valid");
        return result;
    }

    if (!KeywordsToInclude.Length())
    {
        _DMESSAGE("KeywordToInclude not valid");
        return result;
    }

    if (!ObjectRef->inventoryList)
    {
        _DMESSAGE("Inventory list doesn't exist for this ref");
        return result;
    }

    if (!ObjectRef->inventoryList->data.size())
    {
        _DMESSAGE("Inventory list count is 0");
        return result;
    }



    vector<BGSKeyword*> vKeywordsToInclude = ConvertVMArrayToVectorCheckType(KeywordsToInclude, kFormType_KYWD);


    if (vKeywordsToInclude.size() == 0)
    {
        _DMESSAGE("keyword vector size is null");
        return result;
    }

    if (bDebugMode)
    {
        for (auto it = vKeywordsToInclude.begin(); it != vKeywordsToInclude.end(); it++)
        {
            _DMESSAGE("Included keywords>>>%x", Iter->formID);
        }
    }

    vector<BGSKeyword*> vKeywordsToExclude = ConvertVMArrayToVectorCheckType(KeywordsToExclude, kFormType_KYWD);
    int iKeywordToExclude = vKeywordsToExclude.size();



    // RE: BGSInventoryList lock is not modeled; reads are not externally locked.


    UInt32 iPos = 0;

    while (iPos < ObjectRef->inventoryList->data.size())
    {
        TESForm* CurrentForm = ObjectRef->inventoryList->data[iPos].object;

        if (!CurrentForm)
        {
            iPos++;
            continue;
        }
        //_DMESSAGE("Checking inventory item>>%x", CurrentForm->formID);

        BGSKeywordForm* pKeywords = DYNAMIC_CAST(CurrentForm, TESForm, BGSKeywordForm);

        if (!pKeywords) {
            iPos++;
            continue;
        }

        if (!pKeywords->numKeywords)
        {
            iPos++;
            continue;
        }

        if (PositiveCheckforItemKeywords(pKeywords, CurrentForm, vKeywordsToInclude, vKeywordsToExclude, bCheckForAnyIncluded, bCheckForAnyExcluded, iKeywordToExclude)) {
            //_DMESSAGE("InevntoryItem passed keyworkd check - adding.");
            if (bReturnMultiple)
            {
                BGSInventoryItem::Stack* CurrentStack = ObjectRef->inventoryList->data[iPos].stackData.get();
                if (CurrentStack)
                {
                    for (auto i = 0; i < CurrentStack->count; i++)
                    {
                        i++;
                        result.Push(&CurrentForm);
                    }


                    while (CurrentStack->nextStack)
                    {
                        CurrentStack = CurrentStack->nextStack.get();
                        for (auto i = 0; i < CurrentStack->count; i++)
                        {
                            i++;
                            result.Push(&CurrentForm);
                        }
                    }
                }
            }
            else {
                result.Push(&CurrentForm);
            }

        }

        iPos++;
    }

    // (RE: no modeled lock to release)

    _DMESSAGE("GetInventoryItemsByKeyword ends");
    return result;
}








UInt32 cmd_NotifyReferenceScripts(StaticFunctionTag* tag, VMArray<TESObjectREFR*> references, BSFixedString scriptname, BSFixedString callbackname, VMArray<VMVariable> arguments)
{

    UInt32 result = 0;

    if (!references.Length())
    {
        return -1;
    }

    VMValue argsToStore;

    bool bHasArgs = arguments.Length();

    if (bHasArgs) {
        VMArray<VMVariable> argumentsToUSE;

        UInt32 argCount = arguments.Length();
        if (argCount > 0) {
            VMVariable result;
            result.Set<VMArray<VMVariable>>(&arguments);
            argumentsToUSE.Push(&result);
        }

        argumentsToUSE.PackArray(&argsToStore, g_MyVirtMachine);
    }


    for (UInt32 i = 0; i < references.Length(); i++)
    {
        TESObjectREFR* ref;
        references.Get(&ref, i);
        if (ref)
        {
            auto VMIndentifier = AttachedScriptReader::GetVMIdentifierForForm(scriptname.c_str(), ref);
            
            if (!VMIndentifier)
            {
                _DMESSAGE("cound not find scriptname %s for %x", scriptname.c_str(), ref->formID);
                continue;
            }

            if (bHasArgs)
            {
                VMValue* NewArgs = new VMValue(argsToStore);
                CallFunctionNoWait_Internal(g_MyVirtMachine, 0, VMIndentifier, &callbackname, NewArgs);
            }
            else {
                CallFunctionNoWait_Internal(g_MyVirtMachine, 0, VMIndentifier, &callbackname, g_packedArgs);
            }

            result++;
        }

    }

    return result;
}











bool cmd_IsSUPPexMismatched(StaticFunctionTag* tag)
{
    return bPexVersionWrong;
}



bool cmd_AttachPapyrusScript(StaticFunctionTag* tag, TESObjectREFR* Caller, BSFixedString sScriptName, BSFixedString sFunctionName)
{

    if (!Caller)
    {
        _DMESSAGE("Caller not valid");
        return false;
    }


    std::stringstream ss;
    ss << std::hex << Caller->formID; // int decimal_value
    std::string res(ss.str());
    //Console_Print("HEX>>>%s", res.c_str());


    string sCommand = res + ".APS " + sScriptName.c_str() + " " + sFunctionName.c_str();
    OnConsoleCommand_Original(sCommand.c_str());
    return true;
}


BGSKeyword* cmd_GetTerminalLinkKeyword(StaticFunctionTag* tag, TESObjectREFR* REF)
{
    _DMESSAGE("GetTerminalLinkKeyword started");
    if (!REF)
    {
        _DMESSAGE("Ref not valid");
        return NULL;
    }

    return GetTerminalLinkKeyword_Hook(REF);

}



BGSKeyword* cmd_GetKeywordByName(StaticFunctionTag* tag, BSFixedString sKeywordName)
{
    _DMESSAGE("GetKeywordByName started>>%s", sKeywordName.c_str());

    VMArray<BGSKeyword*> result;

    BSTArray<TESForm*>& MyArr = (*g_dataHandler)->formArrays[static_cast<std::size_t>(kFormType_KYWD)];

    for (std::size_t i = 0; i < MyArr.size(); i++)
    {
        TESForm* CurrentObject = MyArr[i];
        BGSKeyword* CurrentKeyword = static_cast<BGSKeyword*>(CurrentObject);

        if (CurrentKeyword->formEditorID == sKeywordName)
        {
            return CurrentKeyword;
        }
    }
    return NULL;
}



//

ActorValueInfo* cmd_GetActorValueByName(StaticFunctionTag* tag, BSFixedString sKeywordName)
{
    _DMESSAGE("GetActorValueByName started>>%s", sKeywordName.c_str());

    VMArray<BGSKeyword*> result;

    BSTArray<TESForm*>& MyArr = (*g_dataHandler)->formArrays[static_cast<std::size_t>(kFormType_AVIF)];

    for (std::size_t i = 0; i < MyArr.size(); i++)
    {
        TESForm* CurrentObject = MyArr[i];
        ActorValueInfo* CurrentAV = static_cast<ActorValueInfo*>(CurrentObject);

        if (_stricmp(CurrentAV->formEditorID.c_str(), sKeywordName.c_str()) == 0)
        {
            return CurrentAV;
        }

    }

    return NULL;
}





bool cmd_IsLastGameLoadedFromMainMenu(StaticFunctionTag* tag)
{
    return g_LastSaveLoadedMainMenuWasOpen;
}



UInt32 cmd_GetInstanceNamingRulesType(StaticFunctionTag* tag, BGSInstanceNamingRules* MyForm)
{

    //Console_Print("Start");
    if (!MyForm)
    {
        //Console_Print("No form");
        return 0;
    }
    //Console_Print("Result>>%d", MyForm->unk20);

    return static_cast<UInt32>(MyForm->type);
}





UInt32 cmd_NotifyReferenceScriptsEx(StaticFunctionTag* tag, VMArray<TESObjectREFR*> references, VMArray <BSFixedString> scriptnames, BSFixedString callbackname, VMArray<VMVariable> arguments)
{
    _DMESSAGE("NotifyReferenceScriptsEx started");
    UInt32 result = 0;

    if (!references.Length())
    {
        _DMESSAGE("refs are empty");
        return -1;
    }

    if (!scriptnames.Length())
    {
        _DMESSAGE("scriptnames are empty");
        return -2;
    }

    if (references.Length() != scriptnames.Length())
    {
        _DMESSAGE("ref lenght %d is not equal to scriptname lenght %d.", references.Length(), scriptnames.Length());
        return -3;
    }



    VMValue argsToStore;

    bool bHasArgs = arguments.Length();

    if (bHasArgs) {
        VMArray<VMVariable> argumentsToUSE;

        UInt32 argCount = arguments.Length();
        if (argCount > 0) {
            VMVariable result;
            result.Set<VMArray<VMVariable>>(&arguments);
            argumentsToUSE.Push(&result);
        }

        argumentsToUSE.PackArray(&argsToStore, g_MyVirtMachine);
    }


    for (UInt32 i = 0; i < references.Length(); i++)
    {
        TESObjectREFR* ref;
        references.Get(&ref, i);
        if (ref)
        {

            BSFixedString CurrentScriptName;
            scriptnames.Get(&CurrentScriptName, i);

            auto VMIndentifier = AttachedScriptReader::GetVMIdentifierForForm(CurrentScriptName.c_str(), ref);

            if (!VMIndentifier)
            {
                _DMESSAGE("cound not find scriptname %s for %x", CurrentScriptName.c_str(), ref->formID);
                continue;
            }

            if (bHasArgs)
            {
                VMValue* NewArgs = new VMValue(argsToStore);
                CallFunctionNoWait_Internal(g_MyVirtMachine, 0, VMIndentifier, &callbackname, NewArgs);
            }
            else {
                CallFunctionNoWait_Internal(g_MyVirtMachine, 0, VMIndentifier, &callbackname, g_packedArgs);
            }

            result++;
        }

    }

    return result;
}

bool cmd_IsIPInTheSameSubnet(StaticFunctionTag* tag, UInt32 NetIP, UInt32 NetSubmask, UInt32 IPToCheck)
{
    _DMESSAGE("IsIPInTheSameSubnet starts>>%d,%d,%d", NetIP, NetSubmask, IPToCheck);

    if ((NetIP & NetSubmask) == (IPToCheck & NetSubmask))
        return true;
    else
        return false;
}


//bool bFound = false;
//
//std::string_view TempScript = CurrentScriptName;
//
//if (TempScript.length() > 4)
//{
//    if (TempScript.at(0) == '[' && TempScript.at(1) == '\"')
//    {
//        bFound = true;
//    }
//}
//
//
//if (!bFound)
//{
//    VMIndentifier = AttachedScriptReader::GetVMIdentifierForForm(CurrentScriptName.c_str(), ref);
//
//    if (!VMIndentifier)
//    {
//        _DMESSAGE("cound not find scriptname %s for %x", CurrentScriptName.c_str(), ref->formID);
//        continue;
//    }
//}
//else {
//    std::string sTemp = CurrentScriptName.c_str();
//}


//TESForm* TestForm = LookupFormByID(197510);
//
//auto OverridingMod = cmd_GetOverridingMod(NULL, TestForm);
//Console_Print("OverridingMod>>%s", OverridingMod.c_str());
//
//VMArray <BSFixedString> OverridingMods = cmd_GetOverridingMods(NULL, TestForm);
//
//
//
//for (auto i = 0; i < OverridingMods.Length(); i++)
//{
//    BSFixedString TempString;
//    OverridingMods.Get(&TempString, i);
//    Console_Print("Overriding mods from array>>%s", TempString.c_str());
//}


//TESObjectREFR* cmd_SUPGetCurrentConsoleReference(StaticFunctionTag* base)
//{
//    	UInt32 handle = (*g_consoleHandle);
//    	NiPointer<TESObjectREFR> refr;
//    	if (handle != 0 && handle != (*g_invalidRefHandle)) {
//    		LookupREFRByHandle(handle, refr);
//    		return refr;
//    	}
//    
//    	return nullptr;
//}


//TESObjectREFR* GetCurrentConsoleRef()
//{
//	UInt32 handle = (*g_consoleHandle);
//	NiPointer<TESObjectREFR> refr;
//	if (handle != 0 && handle != (*g_invalidRefHandle)) {
//		LookupREFRByHandle(handle, refr);
//		return refr;
//	}
//
//	return nullptr;
