
class SUP_ExtraDataRef : public BSExtraData
{
public:
	UInt32	handle;
};



class SUP_ExtraDataKeyword : public BSExtraData
{
public:
	BSTArray<BGSKeyword*> keywords;
};





class SUP_ExtraDataRadioRef : public BSExtraData
{
public:
	float Frequency;
	float MinWeakDistance;
	float MaxWeakDistance;
	UInt32 IgnoresDistanceCheck;
};




TESObjectREFR* f_GetItemDropper(TESObjectREFR* thisForm)
{

	if (!thisForm)
		return NULL;

	ExtraDataList* pExtraData = thisForm->extraDataList.get();

	if (!pExtraData)
	{
		_DMESSAGE("No extra data list");
		return NULL;
	}



	BSExtraData* MyExtraDataTemp = pExtraData->GetByType(kExtraData_ItemDropper);

	if (!MyExtraDataTemp)
	{

		_DMESSAGE("No extra data Item dropper");
		return NULL;
	}


	SUP_ExtraDataRef* ExtraDataAshPileRef = (SUP_ExtraDataRef*)MyExtraDataTemp;

	if (!ExtraDataAshPileRef)
	{
		_DMESSAGE("No SUP extra data - shoult not happen");
		return NULL;
	}

	NiPointer<TESObjectREFR> refr;
	LookupREFRByHandle(ExtraDataAshPileRef->handle, refr);

	if (!refr)
	{
		_DMESSAGE("Source reference doesn't exist");
		return NULL;
	}

	return refr.get();
}





TESObjectREFR* f_GetAshPileSource(TESObjectREFR* thisForm)
{

	if (!thisForm)
		return NULL;

	//if (thisForm->GetObjectReference()->formID != kFormType_ACTI)
	//{
	//	_DMESSAGE("Form is not an activator, type is %d", thisForm->formType);
	//	return NULL;
	//}

	ExtraDataList* pExtraData = thisForm->extraDataList.get();

	if (!pExtraData)
	{
		_DMESSAGE("No extra data list");
		return NULL;
	}



	BSExtraData* MyExtraDataTemp = pExtraData->GetByType(kExtraData_AshPileRef);

	if (!MyExtraDataTemp)
	{

		_DMESSAGE("No extra data pile ref");
		return NULL;
	}


	SUP_ExtraDataRef* ExtraDataAshPileRef = (SUP_ExtraDataRef*)MyExtraDataTemp;

	if (!ExtraDataAshPileRef)
	{
		_DMESSAGE("No SUP extra data - shoult not happen");
		return NULL;
	}

	NiPointer<TESObjectREFR> refr;
	LookupREFRByHandle(ExtraDataAshPileRef->handle, refr);

	if (!refr)
	{
		_DMESSAGE("Source reference doesn't exist");
		return NULL;
	}

	return refr.get();
}







TESObjectREFR* cmd_GetAshPileSource(StaticFunctionTag* base, TESObjectREFR* thisForm)
{
	return f_GetAshPileSource(thisForm);
}

TESObjectREFR* cmd_GetItemDropper(StaticFunctionTag* base, TESObjectREFR* thisForm)
{
	return f_GetItemDropper(thisForm);
}
















struct CellScanInfoSUP
{
	TESObjectCELL* cell;						//player's current cell
	TESObjectCELL* curCell;					//cell currently being scanned
	TESWorldSpace* world;
	SInt8	OrigX =0;
	SInt8	OrigY = 0;
	SInt8	curX = 0;										//offsets of curCell from player's cell
	SInt8	curY = 0;
	vector<TESObjectCELL*> v_FoundCells;
	vector<TESObjectCELL*> v_BB_CellMapTemp;
	int iDepth = 0;



	void f_ScanForCell(int iOffsetX, int iOffsetY)
	{
		int iCount = 0;
		int iFound = 0;
		int iAlreadyInArray = 0;

		curCell = NULL;
		curX = OrigX + (iOffsetX);
		curY = OrigY + (iOffsetY);


		_DMESSAGE("Looking for Cell with coord X::%d, Y:%d", curX, curY);



		for (auto it = v_BB_CellMapTemp.begin(); it != v_BB_CellMapTemp.end(); it++)
		{
			if (Iter->GetCoordinates())
			{
				if ((Iter->GetCoordinates()->cellX == curX) && (Iter->GetCoordinates()->cellY == curY))
				{
					v_FoundCells.push_back(Iter);
					//_DMESSAGE("Cell found>>%x", Iter->formID);
					v_BB_CellMapTemp.erase(it);
					return;
				}
			}

		}
	}


	void GetWorldSpaceCell()
	{
		_DMESSAGE("Cell map size>>%d", world->cellMap.size());
		_DMESSAGE("Cell map max size>>%d", world->cellMap.size());



		for (auto it = world->cellMap.begin(); it != world->cellMap.end(); it++) {
			if (Iter.second)
			{
				v_BB_CellMapTemp.push_back(Iter.second);
			}
		}

	}



	CellScanInfoSUP() {}
	CellScanInfoSUP(int iDepthIn,TESObjectCELL* _cell)
		: curCell(NULL), cell(_cell), world(NULL), curX(0), curY(0)
	{

		iDepth = iDepthIn;
		world = cell->worldSpace;
		v_FoundCells.push_back(cell);
		if (world)		//exterior, cell depth > 0
		{
			v_BB_CellMapTemp.clear();
			GetWorldSpaceCell();

			OrigX = cell->GetCoordinates()->cellX;
			OrigY = cell->GetCoordinates()->cellY;

			_DMESSAGE("Adding original X::%d,Y::%d cell>>>%x", OrigX, OrigY, cell->formID);

			if (iDepth >=1)
			{
				for (int i = 1; i <= iDepth; i++)
				{
					_DMESSAGE("Looking for coordinates with depth>>%d", i);
					int iCoord = 1 * i;
					int iCoordNegative = iCoord * -1;

					_DMESSAGE("Coord is>>%d,Negative coord is>>%d ", iCoord, iCoordNegative);

					f_ScanForCell(iCoordNegative, iCoord);
					f_ScanForCell(0, iCoord);
					f_ScanForCell(iCoord, iCoord);
					f_ScanForCell(iCoord, 0);
					f_ScanForCell(iCoord, iCoordNegative);
					f_ScanForCell(0, iCoordNegative);
					f_ScanForCell(iCoordNegative, iCoordNegative);
					f_ScanForCell(iCoordNegative, 0);
				}

			}


		}
		else {
			v_FoundCells.push_back(cell);
			_DMESSAGE("Cell scanner did not find cell worldspace");
		}

	}



};



void f_AddNPCFromCell(TESObjectCELL* Cell, VMArray<Actor*>* result)
{

	TESObjectREFR* TempRef;

	for (UInt64 i = 0; i < Cell->references.size(); i++)
	{
		TempRef = Cell->references[i].get();


		if (TempRef && TempRef->GetObjectReference())
		{
			if (TempRef->GetObjectReference()->formType == kFormType_NPC)
			{
				Actor* MyActor = (Actor*)TempRef;
				result->Push(&MyActor);
				_DMESSAGE("FOUND IN CELL >>> %x>>-------%x", TempRef->formID, Cell->formID);
				//_DMESSAGE("Found %s in cell %x.", TempRef->GetObjectReference()->GetFullName(),Cell->formID);
			}

		}

	}


}


VMArray<Actor*> cmd_GetActorsInCell(StaticFunctionTag* base, TESObjectCELL* Cell, UInt32 iDepth)
{
	_DMESSAGE("GetActorsInCell starts with Depth>>%d", iDepth);
	VMArray<Actor*> result;


	if (!Cell)
	{
		_DMESSAGE("Chosen cell is NULL");
		return result;
	}

	int iToCompare = iDepth;
	_DMESSAGE("iToCompare>>%d", iToCompare);

	if (-1 > iToCompare)
	{
		_DMESSAGE("Setting iDepth to -1");
		iDepth = -1;
	}




	int iInterior = f_BB_IsCellInterior(Cell);


	if (iInterior == 0)
	{
		if (iDepth == -1)
		{
			Setting* MySetting = GetINISettingSUP("uGridsToLoad:General");
			if (MySetting != NULL)
			{
				iDepth = MySetting->GetUInt();
			}
			else {
				iDepth = 5;
				_DMESSAGE("uGridsToLoad:General doesn't exist");
			}
		}
	}


	_DMESSAGE("Cell scan Depth is %d", iDepth);


	CellScanInfoSUP info(iDepth, Cell);


	for (auto it = info.v_FoundCells.begin(); it != info.v_FoundCells.end(); it++)
	{
		f_AddNPCFromCell(Iter, &result);
	}

	return result;
}






void f_AddObjectFromCell(TESObjectCELL* Cell, VMArray<TESObjectREFR*>* result, int iType)
{

	TESObjectREFR* TempRef;

	if (iType == -1)
	{
		for (UInt64 i = 0; i < Cell->references.size(); i++)
		{
			TempRef = Cell->references[i].get();

			if (TempRef && TempRef->GetObjectReference())
			{
				TESObjectREFR* MyActor = (TESObjectREFR*)TempRef;
				result->Push(&MyActor);
				_DMESSAGE("FOUND IN CELL >>> %x>>-------%x", TempRef->formID, Cell->formID);
			}

		}

	}
	else {
		for (UInt64 i = 0; i < Cell->references.size(); i++)
		{
			TempRef = Cell->references[i].get();

			if (TempRef && TempRef->GetObjectReference())
			{
				if (TempRef->GetObjectReference()->formType.underlying() == iType)
				{
					TESObjectREFR* MyActor = (TESObjectREFR*)TempRef;
					result->Push(&MyActor);
					_DMESSAGE("FOUND IN CELL >>> %x>>-------%x", TempRef->formID, Cell->formID);
				}

			}

		}

	}

}




VMArray<TESObjectREFR*> cmd_GetReferencesInCell(StaticFunctionTag* base, TESObjectCELL* Cell, UInt32 iType, UInt32 iDepth)
{
	_DMESSAGE("GetReferencesInCell starts with Depth>>%d", iDepth);
	VMArray<TESObjectREFR*> result;


	if (Cell == NULL)
	{
		_DMESSAGE("Chosen cell is NULL");
		return result;
	}

	int iTypeToLook = iType;
	int iToCompare = iDepth;
	_DMESSAGE("iToCompare>>%d", iToCompare);

	if (-1 > iToCompare)
	{
		_DMESSAGE("Setting iDepth to -1");
		iDepth = -1;
	}




	int iInterior = f_BB_IsCellInterior(Cell);


	if (iInterior == 0)
	{
		if (iDepth == -1)
		{
			Setting* MySetting = GetINISettingSUP("uGridsToLoad:General");
			if (MySetting != NULL)
			{
				iDepth = MySetting->GetUInt();
			}
			else {
				iDepth = 5;
				_DMESSAGE("uGridsToLoad:General doesn't exist");
			}
		}
	}


	_DMESSAGE("Cell scan Depth is %d", iDepth);


	CellScanInfoSUP info(iDepth, Cell);


	for (auto it = info.v_FoundCells.begin(); it != info.v_FoundCells.end(); it++)
	{
		f_AddObjectFromCell(Iter, &result, iTypeToLook);
	}

	return result;
}






void f_AddObjectFromCelByFormList(TESObjectCELL* Cell, VMArray<TESObjectREFR*>* result, vector<TESForm*> v_BaseForms)
{

	TESObjectREFR* TempRef = NULL;


	for (UInt64 i = 0; i < Cell->references.size(); i++)
	{
		TempRef = Cell->references[i].get();

		if (TempRef == NULL)
		{
			continue;
		}

		for (auto it = v_BaseForms.begin(); it != v_BaseForms.end(); it++)
		{
			if(TempRef->GetObjectReference() == Iter)
			{
				TESObjectREFR* MyActor = (TESObjectREFR*)TempRef;
				result->Push(&MyActor);
				//_DMESSAGE("FOUND IN CELL >>> %x>>-------%x", TempRef->formID, Cell->formID);
			}
		}
	}
}






void f_AddObjectFromCelByBaseForm(TESObjectCELL* Cell, VMArray<TESObjectREFR*>* result, TESForm* MyBaseForm)
{

	TESObjectREFR* TempRef;

		for (UInt64 i = 0; i < Cell->references.size(); i++)
		{
			TempRef = Cell->references[i].get();

			if (TempRef && TempRef->GetObjectReference() == MyBaseForm)
			{
				TESObjectREFR* MyActor = (TESObjectREFR*)TempRef;
				result->Push(&MyActor);
				//_DMESSAGE("FOUND IN CELL >>> %x>>-------%x", TempRef->formID, Cell->formID);
			}

		}
}










VMArray<TESObjectREFR*> cmd_GetReferencesInCellByBaseForm(StaticFunctionTag* base, TESObjectCELL* Cell, TESForm* MyBaseForm, UInt32 iDepth)
{
	_DMESSAGE("GetReferencesInCell starts with Depth>>%d", iDepth);
	VMArray<TESObjectREFR*> result;

	if (MyBaseForm == NULL)
	{
		_DMESSAGE("Chosen base form is NULL");
		return result;
	}


	if (Cell == NULL)
	{
		_DMESSAGE("Chosen cell is NULL");
		return result;
	}

	int iInterior = f_BB_IsCellInterior(Cell);


	if (iInterior == 0)
	{
		if (iDepth == -1)
		{
			Setting* MySetting = GetINISettingSUP("uGridsToLoad:General");
			if (MySetting != NULL)
			{
				iDepth = MySetting->GetUInt();
			}
			else {
				iDepth = 5;
				_DMESSAGE("uGridsToLoad:General doesn't exist");
			}
		}
	}


	_DMESSAGE("Cell scan Depth is %d", iDepth);


	CellScanInfoSUP info(iDepth, Cell);


	if (MyBaseForm->formType == kFormType_FLST)
	{

		BGSListForm* formList = (BGSListForm*)MyBaseForm;

		vector<TESForm*> v_BaseForms{};


		for (UInt32 i = 0; i < formList->arrayOfForms.size(); i++)
		{
			TESForm* form = nullptr;
			form = formList->arrayOfForms[i];
			if (form)
			{
				_DMESSAGE("Adding form %x to search list", form->formID);
				v_BaseForms.push_back(form);
			}

		}

		if (v_BaseForms.size() == 0)
		{
			_DMESSAGE("form list size is null");
			return result;
		}


		for (auto it = info.v_FoundCells.begin(); it != info.v_FoundCells.end(); it++)
		{
			f_AddObjectFromCelByFormList(Iter, &result, v_BaseForms);
		}

	}
	else {
		for (auto it = info.v_FoundCells.begin(); it != info.v_FoundCells.end(); it++)
		{
			f_AddObjectFromCelByBaseForm(Iter, &result, MyBaseForm);
		}

	}



	return result;
}
























bool cmd_UpdateMovingWirelessItem(StaticFunctionTag* base, TESObjectREFR* refA)
{

	if (refA == NULL)
	{
		_DMESSAGE("Chosen ref is NULL");
		return false;
	}

	Workshop::ExtraData* extraDataWorkshop = static_cast<Workshop::ExtraData*>(refA->extraDataList->GetByType(RE::EXTRA_DATA_TYPE::kWorkshop));
	if (!extraDataWorkshop) {
		return false;
	}

	sup::compat::UpdateMovingWirelessItem(refA, extraDataWorkshop);
	return true;
}

bool cmd_EstablishTerminalLinks(StaticFunctionTag* base, TESObjectREFR* refA)
{
	if (refA == NULL)
	{
		_DMESSAGE("Chosen ref is NULL");
		return false;
	}

	sup::compat::EstablishTerminalLinks(refA);
	return true;
}


bool f_UpdateSpline(TESObjectREFR* refA, TESObjectREFR* refB, TESObjectREFR* splineRef, UInt32 linkType1 = 0, UInt32 linkType2 = 0)
{
	if (refA == NULL)
	{
		_DMESSAGE("refA is NULL");
		return false;
	}

	if (refB == NULL)
	{
		_DMESSAGE("refB is NULL");
		return false;
	}

	if (splineRef == NULL)
	{
		_DMESSAGE("splineRefis NULL");
		return false;
	}

	Workshop::ContextData contextData(*g_player);

	sup::compat::SplineUtils::UpdateSpline(&contextData, splineRef, refB, linkType2, refA, linkType1);
	return true;

}




bool cmd_UpdateSpline(StaticFunctionTag* base, TESObjectREFR* refA, TESObjectREFR* refB, TESObjectREFR* splineRef)
{
	return f_UpdateSpline(refA, refB, splineRef);
}

bool cmd_UpdateSplineEx(StaticFunctionTag* base, TESObjectREFR* refA, TESObjectREFR* refB, TESObjectREFR* splineRef,UInt32 linkType1, UInt32 linkType2)
{
	return f_UpdateSpline(refA, refB, splineRef, linkType1, linkType2);
}


vector<TESObjectREFR*> f_GetConnectedSplines(TESObjectREFR* thisRef)
{
	vector<TESObjectREFR*> result;
	if (!thisRef)
		return result;

	ExtraDataList* extraDataList = thisRef->extraDataList.get();
	if (extraDataList)
	{
		if (extraDataList->HasType(kExtraData_PowerLinks))
		{
			ExtraPowerLinks* powerLinks = (ExtraPowerLinks*)extraDataList->GetByType(kExtraData_PowerLinks);
			if (powerLinks)
			{
				for (const auto& link : powerLinks->powerLinks)
				{
					TESForm* form = LookupFormByID(link.formID);
					if (form)
					{
						TESObjectREFR* refr = DYNAMIC_CAST(form, TESForm, TESObjectREFR);
						if (refr)
							result.push_back(refr);
					}
				}
			}
		}
	}

	return result;
}




VMArray<TESObjectREFR*> cmd_UpdateWires(StaticFunctionTag* base, TESObjectREFR* refObjectWithWires)
{
	VMArray<TESObjectREFR*> result;


	if (!refObjectWithWires)
	{
		_DMESSAGE("refObjectWithWires not valid");
		return result;
	}

	vector<TESObjectREFR*> Splines = f_GetConnectedSplines(refObjectWithWires);

	if (Splines.size() <= 0)
	{
		_DMESSAGE("No splines for this reference");
		return result;
	}

	Workshop::ContextData contextData(*g_player);

	for (auto it = Splines.begin(); it != Splines.end(); it++)
	{
		_DMESSAGE("Checking Spline>>> %x", Iter->formID);
		vector<TESObjectREFR*> splineConnections = f_GetConnectedSplines(Iter);
		TESObjectREFR* connectedObject = NULL;

		for (auto it2 = splineConnections.begin(); it2 != splineConnections.end(); it2++)
		{
			if (Iter2 != refObjectWithWires)
			{
				connectedObject = Iter2;
			}

			if (connectedObject)
			{
				f_UpdateSpline(refObjectWithWires, connectedObject, Iter);
				result.Push(&connectedObject);
				_DMESSAGE("Updating connected object>>> %x", connectedObject->formID);
			}
		}
	}
	return result;
}




bool cmd_IsRadio(StaticFunctionTag* base, TESObjectREFR* thisForm)
{
	_DMESSAGE("IsRadio started");
	if (!thisForm)
		return false;


	if (!thisForm->extraDataList)
	{
		_DMESSAGE("No extra data list");
		return false;
	}


	BSExtraData* MyExtraDataTemp = thisForm->extraDataList->GetByType(kExtraData_RadioData);

	if (!MyExtraDataTemp)
	{
		_DMESSAGE("No extra data Radio Ref");
		return false;
	}

	return true;

}


RadioData cmd_GetRadioData(StaticFunctionTag* base, TESObjectREFR* thisForm)
{
	_DMESSAGE("GetRadioData started");

	RadioData result;

	if (!thisForm)
		return result;

	if (!thisForm->extraDataList)
	{
		_DMESSAGE("No extra data list");
		return result;
	}

	SUP_ExtraDataRadioRef* ExtraDataRef = (SUP_ExtraDataRadioRef*)thisForm->extraDataList->GetByType(kExtraData_RadioData);

	if (!ExtraDataRef)
	{
		_DMESSAGE("No extra data Radio Ref");
		return result;
	}

	_DMESSAGE("0");
	result.Set<UInt32>("IsRadio", 1);
	_DMESSAGE("1");
	result.Set<float>("Frequency", ExtraDataRef->Frequency);
	_DMESSAGE("2");
	result.Set<float>("MinWeakDistance", ExtraDataRef->MinWeakDistance);
	_DMESSAGE("3");
	result.Set<float>("MaxWeakDistance", ExtraDataRef->MaxWeakDistance);
	_DMESSAGE("4");
	result.Set<UInt32>("IgnoresDistanceCheck", ExtraDataRef->IgnoresDistanceCheck);
	_DMESSAGE("5");
	return result;

}






TESObjectREFR* cmd_AttachWireEX(StaticFunctionTag* base, TESObjectREFR* refA, TESObjectREFR* refB, TESObjectREFR* wireRef, UInt32 linkType1, UInt32 linkType2)
{
	
	//VirtualMachine* vm = (*g_gameVM)->m_virtualMachine;

	if (!refA || !refB || !wireRef)
	{
		_DMESSAGE("AttachWireEX::One of the incoming refs is invalid");
		return NULL;
	}


	// No specified spline, no refs, refs are same item, or no 3D loaded
	if (!refA || !refB || refA == refB || !refA->Get3D() || !refB->Get3D()) {
		return nullptr;
	}

	// See if the two references are already linked by the same wire i.e. they have the same entry in their PowerLinks listing
	std::set<UInt64> linkedWires;
	ExtraDataList* extraDataRefA = refA->extraDataList.get();
	ExtraDataList* extraDataRefB = refB->extraDataList.get();
	if (extraDataRefA && extraDataRefB)
	{
		ExtraPowerLinks* powerLinksA = static_cast<ExtraPowerLinks*>(extraDataRefA->GetByType(kExtraData_PowerLinks));
		ExtraPowerLinks* powerLinksB = static_cast<ExtraPowerLinks*>(extraDataRefB->GetByType(kExtraData_PowerLinks));
		if (powerLinksA && powerLinksB) // Both items must have power links to check
		{
			const auto* connectionPopulate = &powerLinksA->powerLinks;
			const auto* connectionSearch = &powerLinksB->powerLinks;
			if (connectionPopulate->size() > connectionSearch->size()) // Pick the smaller list to be the set
			{
				std::swap(connectionPopulate, connectionSearch);
			}

			// Add the items from the smaller list to the set
			for (const auto& link : *connectionPopulate)
			{
				linkedWires.insert(link.formID);
			}

			// Search the other listing for items that exist in the set
			for (const auto& link : *connectionSearch)
			{
				// This wire exists in the other list, it is invalid to wire the same objects twice
				if (linkedWires.find(link.formID) != linkedWires.end()) {
					return nullptr;
				}
			}
		}
	}

	BGSBendableSpline* splineA = DYNAMIC_CAST(refA->GetObjectReference(), TESForm, BGSBendableSpline);
	BGSBendableSpline* splineB = DYNAMIC_CAST(refB->GetObjectReference(), TESForm, BGSBendableSpline);

	BGSKeyword* keyword = nullptr;
	BGSDefaultObject* workshopItemDefault = (*g_defaultObjectMap)->GetDefaultObject("WorkshopItem");
	if (workshopItemDefault) {
		keyword = DYNAMIC_CAST(workshopItemDefault->form, TESForm, BGSKeyword);
	}

	// No workshop keyword is bad
	// Connecting a wire to another wire or connecting a non-wire is invalid
	if (!keyword || splineA || splineB) {
		return nullptr;
	}

	// Get the workshop by keyword
	TESObjectREFR* workshopRef = GetLinkedRef_Native(refA, keyword);
	if (!workshopRef) {
		return nullptr;
	}

	// Workshop ref isn't a workshop!
	Workshop::ExtraData* extraDataWorkshop = static_cast<Workshop::ExtraData*>(workshopRef->extraDataList->GetByType(RE::EXTRA_DATA_TYPE::kWorkshop));
	if (!extraDataWorkshop) {
		return nullptr;
	}

	// Create our wire instance


	UInt32 nullHandle = *g_invalidRefHandle;
	TESObjectCELL* parentCell = wireRef->parentCell;
	TESWorldSpace* worldspace = wireRef->parentCell ? wireRef->parentCell->worldSpace : nullptr;

	NiPoint3 rot;
	NiPoint3 refAPos = refA->GetPosition();
	MoveRefrToPosition(wireRef, &nullHandle, parentCell, worldspace, &refAPos, &rot);

	// Set the wire's linked ref to the workshop
	sup::compat::SetLinkedRef_Native(wireRef, workshopRef, keyword);

	BSPointerHandle<TESObjectREFR> currentWorkshop = *Workshop::hCurrentWorkshop;

	Workshop::SetCurrentWorkshop(workshopRef);

	Workshop::ContextData contextData(*g_player);
	sup::compat::SplineUtils::UpdateSpline(&contextData, wireRef, refB, linkType2, refA, linkType1);
	RE::SplineUtils::ConnectSpline(refA, linkType1, refB, linkType2, wireRef);

	ExtraBendableSplineParams* splineParams = (ExtraBendableSplineParams*)wireRef->extraDataList->GetByType(kExtraData_BendableSplineParams);
	if (splineParams) {
		// RE keeps ParamData members private; access via a layout-compatible mirror.
		struct SUP_SplineParams { float slack; float thickness; };
		auto* sp = reinterpret_cast<SUP_SplineParams*>(&splineParams->data);
		sp->thickness = 1.5f;
	}

	sup::compat::AddItem(extraDataWorkshop, wireRef);
	sup::compat::AddConnection(extraDataWorkshop, refA, refB, wireRef);
	sup::compat::UpdateMovingWirelessItem(refA, extraDataWorkshop);
	sup::compat::UpdateMovingWirelessItem(refB, extraDataWorkshop);
	sup::compat::EstablishTerminalLinks(wireRef);

	*Workshop::hCurrentWorkshop = currentWorkshop;
	return wireRef;
}



VMArray<SUPConnectedObject> cmd_GetConnectedObjectsEx(StaticFunctionTag* base, TESObjectREFR* thisRef)
{
	_DMESSAGE("GetConnectedObjectsEx starts");
	VMArray<SUPConnectedObject> result;
	if (!thisRef)
	{
		_DMESSAGE("GetConnectedObjectsEx:: ref is invalid");
		return result;
	}


	ExtraDataList* extraDataList = thisRef->extraDataList.get();
	if (extraDataList)
	{
		if (extraDataList->HasType(kExtraData_PowerLinks))
		{
			_DMESSAGE("Ref has extra data.");
			ExtraPowerLinksSUP* powerLinks = (ExtraPowerLinksSUP*)extraDataList->GetByType(kExtraData_PowerLinks);
			if (powerLinks)
			{
				_DMESSAGE("powerLinks count >>%d", powerLinks->connections.count);
				for (int i = 0; i < powerLinks->connections.count; i++)
				{
					//_DMESSAGE("Connection>>%d", i);
					ExtraPowerLinksSUP::Element CurrentElement;
					//_DMESSAGE("1");
					powerLinks->connections.GetNthItem(i, CurrentElement);
					//_DMESSAGE("2");
					TESObjectREFR* TempRef = (TESObjectREFR*)LookupFormByID(CurrentElement.formID);

					//TESForm* form = LookupFormByID(CurrentElement.formID);
					//_DMESSAGE("3");
					if (TempRef)
					{
						_DMESSAGE("Form ID exists>>>%x", TempRef);
						//_DMESSAGE("refr exists");
						SUPConnectedObject ResultElement;
						ResultElement.Set<TESObjectREFR*>("ObjectRef", TempRef);
						ResultElement.Set<UInt32>("LinkType", CurrentElement.linkType);
						result.Push(&ResultElement);
					}
					else {
						_DMESSAGE("Form ID doesn't exist>>>");
					}
				}
			}
		}
	}

	_DMESSAGE("Return result");
	return result;
}
















void f_GetRadioRefsFromCell(TESObjectCELL* Cell, VMArray<TESObjectREFR*>* Ar_Result)
{
	_DMESSAGE("GetRadioRefsFromCell starts");

	if (!Cell)
	{
		_DMESSAGE("Chosen cell is NULL");
		return;
	}

	TESObjectREFR* TempRef;


	for (UInt64 i = 0; i < Cell->references.size(); i++)
	{
		TempRef = Cell->references[i].get();

		if (TempRef && TempRef->GetObjectReference())
		{
			if (TempRef->GetObjectReference()->formType == kFormType_ACTI)
			{
				if (TempRef->extraDataList && TempRef->extraDataList->GetByType(kExtraData_RadioData))
				{
					Ar_Result->Push(&TempRef);
					_DMESSAGE("Adding radio ref to array,%x", TempRef->formID);
				}
			}

		}

	}

}







VMArray<TESObjectREFR*> cmd_GetAllRadios(StaticFunctionTag* base)
{
	VMArray<TESObjectREFR*> result;

	auto& cellList = (*g_dataHandler)->interiorCells;
	for (std::size_t i = 0; i < cellList.size(); i++) {
		TESObjectCELL* MyCell = cellList[i];
		if (MyCell)
		{
			f_GetRadioRefsFromCell(MyCell, &result);
		}
	}

	
	return result;
}



bool cmd_IsRadioReceiver(StaticFunctionTag* base, TESObjectREFR* thisForm)
{
	_DMESSAGE("IsReceiver started");
	if (!thisForm)
		return false;


	if (!thisForm->extraDataList)
	{
		_DMESSAGE("No extra data list");
		return false;
	}


	BSExtraData* MyExtraDataTemp = thisForm->extraDataList->GetByType(kExtraData_RadioReceiver);

	if (!MyExtraDataTemp)
	{
		_DMESSAGE("No extra data Receiver");
		return false;
	}

	return true;
}



SUPXYZ cmd_GetStartingLocation(StaticFunctionTag* base, TESObjectREFR* thisForm)
{
	_DMESSAGE("GetStartingLocation starts");
	SUPXYZ result;
	if (!thisForm)
	{
		_DMESSAGE("GetStartingLocation - form invalid");
		return result;
	}



	NiPoint3 p = thisForm->GetStartingLocation();

	result.Set<UInt32>("Success", 1);
	result.Set<float>("x", p.x);
	result.Set<float>("y", p.y);
	result.Set<float>("z", p.z);

	_DMESSAGE("Success is %d,%f,%f,%f", 1, p.x, p.y, p.z);
	return result;
}




SUPXYZ cmd_GetStartingAngle(StaticFunctionTag* base, TESObjectREFR* thisForm)
{
	_DMESSAGE("GetStartingAngle starts");
	SUPXYZ result;
	if (!thisForm)
	{
		_DMESSAGE("GetStartingAngle - form invalid");
		return result;
	}


	NiPoint3 p = thisForm->GetStartingAngle();



	result.Set<UInt32>("Success", 1);
	result.Set<float>("x", p.x * 180.0 / MATH_PI);
	result.Set<float>("y", p.y * 180.0 / MATH_PI);
	result.Set<float>("z", p.z * 180.0 / MATH_PI);
	return result;
}





bool cmd_TransmitConnectedPowerLatentEx(StaticFunctionTag* base,TESObjectREFR* refr)
{
	if (!refr)
		return false;

	NiNode* root = sup::compat::GetObjectRootNode(refr);
	if (!root) {
		return false;
	}

	BGSKeyword* keyword = nullptr;
	BGSDefaultObject* workshopItemDefault = (*g_defaultObjectMap)->GetDefaultObject("WorkshopItem");
	if (workshopItemDefault) {
		keyword = DYNAMIC_CAST(workshopItemDefault->form, TESForm, BGSKeyword);
	}

	// No workshop keyword is bad
	if (!keyword) {
		return false;
	}

	// Get the workshop by keyword
	TESObjectREFR* workshopRef = GetLinkedRef_Native(refr, keyword);
	if (!workshopRef) {
		return false;
	}

	// Workshop ref isn't a workshop!
	Workshop::ExtraData* extraDataWorkshop = static_cast<Workshop::ExtraData*>(workshopRef->extraDataList->GetByType(RE::EXTRA_DATA_TYPE::kWorkshop));
	if (!extraDataWorkshop) {
		return false;
	}

	auto* extraData = sup::compat::GetExtraData(root, "CPA");
	auto* parents = reinterpret_cast<sup::compat::BSConnectPointParents*>(extraData);
	if (!parents) {
		return false;
	}

	BSPointerHandle<TESObjectREFR> previousWorkshop = *Workshop::hCurrentWorkshop;

	Workshop::SetCurrentWorkshop(workshopRef);

	for (UInt32 i = 0; i < parents->points.count; i++)
	{
		sup::compat::BSConnectPointParents::ConnectPoint* connectPoint = parents->points[i];
		if (!connectPoint)
			continue;

		NiPoint3 localPos = connectPoint->pos;
		NiAVObject* parent = nullptr;
		//if (connectPoint->parent == "")
		//	parent = root;
		//else
		//{
		//	NiAVObject* child = root->GetObjectByName(&connectPoint->parent);
		//	if (child)
		//		parent = child;
		//}

		NiPoint3 worldPos = localPos;
		parent = root;
		if (parent) {
			worldPos = parent->world.rotate.Transpose() * localPos + parent->world.translate;
		}

		float scale = connectPoint->scale;
		if (parent != root && refr->parentCell) {
			bhkWorld* world = sup::compat::GetHavokWorld(refr->parentCell);
			if (world) {
				TESObjectREFR* connected = GetObjectAtConnectPoint(refr, &worldPos, world, 8.0f);
				if (connected) {
					try // Probably wont make a difference but doesnt hurt to try
					{
						sup::compat::AddConnection(extraDataWorkshop, refr, connected, nullptr);
						sup::compat::UpdateMovingWirelessItem(connected, extraDataWorkshop);
					}
					catch (...)
					{
						_MESSAGE("Power link error!");
					}
				}
			}
		}
	}

	sup::compat::UpdateMovingWirelessItem(refr, extraDataWorkshop);

	*Workshop::hCurrentWorkshop = previousWorkshop;
	return true;
}








VMArray<TESFaction*> cmd_GetAllActorFactions(StaticFunctionTag* base, TESObjectREFR* TempRef)
{
	VMArray<TESFaction*> result;


	if (!TempRef)
	{
		_DMESSAGE("GetAllActorFactions - actor is invalid");
		return result;
	}

	BSTArray<TESForm*>& MyArr = (*g_dataHandler)->formArrays[14];


	Actor* ActorRef = (Actor*)TempRef;

	for (std::size_t i = 0; i < MyArr.size(); i++)
	{
		TESForm* CurrentObject = MyArr[i];

		if (CurrentObject)
		{
			TESFaction* MyFaction = ((TESFaction*)CurrentObject);

			if (ActorRef->IsInFaction(MyFaction))
			{
				result.Push(&MyFaction);
			}


		}

	}



	return result;
}





UInt32 cmd_ActorJump(StaticFunctionTag* base, Actor* TempRef)
{
	_DMESSAGE("ActorJump starts");
	if (!TempRef)
	{
		_DMESSAGE("Actor not valid");
		return -1;
	}

	RE::bhkCharacterController* CharController = ActorGetCharController(TempRef);
	if (!CharController)
	{
		_DMESSAGE("no CharController");
		return -1;
	}

	CharController->Jump(-3.4028235e38);
	return 0;
}


bool cmd_ActorIsInAir(StaticFunctionTag* base, Actor* TempRef)
{
	_DMESSAGE("ActorIsInAir starts");
	if (!TempRef)
	{
		_DMESSAGE("Actor not valid");
		return -1;
	}

	return ActorIsInAir(TempRef);
}


bool cmd_ActorIsSwimming(StaticFunctionTag* base, Actor* TempRef)
{
	_DMESSAGE("ActorIsSwimming( starts");
	if (!TempRef)
	{
		_DMESSAGE("Actor not valid");
		return false;
	}


	return TempRef->IsSwimming();
}










VMArray<TESObjectREFR*> cmd_GetReferencesInCellWithKeyword(StaticFunctionTag* base, TESObjectCELL* Cell, TESForm* KeywordToInclude, TESForm* KeywordToExclude, bool bCheckForAnyIncluded, bool bCheckForAnyExcluded, UInt32 iDepth)
{
	_DMESSAGE("GetReferencesInCellWithKeyword with Depth>>%d", iDepth);


	VMArray<TESObjectREFR*> result;

	if (Cell == NULL)
	{
		_DMESSAGE("Chosen cell is NULL");
		return result;
	}

	if (KeywordToInclude == NULL)
	{
		_DMESSAGE("Chosen Keyword is NULL");
		return result;
	}



	vector<BGSKeyword*> vKeywordToInclude{};
	vector<BGSKeyword*> vKeywordToExclude{};

	FillKeywordsArrayByForm(vKeywordToInclude, KeywordToInclude);

	if (vKeywordToInclude.size() == 0)
	{
		_DMESSAGE("form list size is null");
		return result;
	}

	if (KeywordToExclude)
	{
		FillKeywordsArrayByForm(vKeywordToExclude, KeywordToExclude);
	}



	int iInterior = f_BB_IsCellInterior(Cell);


	if (iInterior == 0)
	{
		if (iDepth == -1)
		{
			Setting* MySetting = GetINISettingSUP("uGridsToLoad:General");
			if (MySetting != NULL)
			{
				iDepth = MySetting->GetUInt();
			}
			else {
				iDepth = 5;
				_DMESSAGE("uGridsToLoad:General doesn't exist");
			}
		}
	}

	_DMESSAGE("Cell scan Depth is %d", iDepth);

	CellScanInfoSUP info(iDepth, Cell);
	for (auto it = info.v_FoundCells.begin(); it != info.v_FoundCells.end(); it++)
	{
		f_AddObjectFromCellByKeyword(Iter, result, vKeywordToInclude, vKeywordToExclude, bCheckForAnyIncluded, bCheckForAnyExcluded);
	}


	return result;
}







VMArray<TESObjectREFR*> cmd_GetReferencesInCellWithKeywordAlt(StaticFunctionTag* base, TESObjectCELL* Cell, VMArray<BGSKeyword*> KeywordsToInclude, VMArray<BGSKeyword*> KeywordsToExclude, bool bCheckForAnyIncluded, bool bCheckForAnyExcluded, UInt32 iDepth)
{
	_DMESSAGE("GetReferencesInCellWithKeyword with Depth>>%d", iDepth);


	VMArray<TESObjectREFR*> result;

	if (Cell == NULL)
	{
		_DMESSAGE("Chosen cell is NULL");
		return result;
	}

	if (!KeywordsToInclude.Length())
	{
		_DMESSAGE("Chosen Keyword array is 0");
		return result;
	}



	vector<BGSKeyword*> vKeywordToInclude{};
	vector<BGSKeyword*> vKeywordToExclude{};


	for (UInt32 i = 0; i < KeywordsToInclude.Length(); i++)
	{
		BGSKeyword* var;
		KeywordsToInclude.Get(&var, i);
		if (var)
		{
			if (var->formType == kFormType_KYWD)
			{
				vKeywordToInclude.push_back(var);
			}
			else {
				_DMESSAGE("Keyword  INCL is not a keyword type and not a formlist>>%d", var->formType);
			}
		
		}

	}


	if (vKeywordToInclude.size() == 0)
	{
		_DMESSAGE("form list size is null");
		return result;
	}


	for (UInt32 i = 0; i < KeywordsToExclude.Length(); i++)
	{
		BGSKeyword* var;
		KeywordsToExclude.Get(&var, i);
		if (var)
		{
			if (var->formType == kFormType_KYWD)
			{
				vKeywordToExclude.push_back(var);
			}
			else {
				_DMESSAGE("Keyword ECLD is not a keyword type and not a formlist>>%d", var->formType);
			}
		}
	}



	int iInterior = f_BB_IsCellInterior(Cell);


	if (iInterior == 0)
	{
		if (iDepth == -1)
		{
			Setting* MySetting = GetINISettingSUP("uGridsToLoad:General");
			if (MySetting != NULL)
			{
				iDepth = MySetting->GetUInt();
			}
			else {
				iDepth = 5;
				_DMESSAGE("uGridsToLoad:General doesn't exist");
			}
		}
	}

	_DMESSAGE("Cell scan Depth is %d", iDepth);

	CellScanInfoSUP info(iDepth, Cell);
	for (auto it = info.v_FoundCells.begin(); it != info.v_FoundCells.end(); it++)
	{
		f_AddObjectFromCellByKeyword(Iter, result, vKeywordToInclude, vKeywordToExclude, bCheckForAnyIncluded, bCheckForAnyExcluded);
	}


	return result;
}














void f_AddObjectFromCelByType(TESObjectCELL* Cell, VMArray<TESObjectREFR*>& result, UInt32 Type, const ModInfo* ChosenMod, vector<BGSKeyword*>& KeywordsToInclude, vector<BGSKeyword*>& KeywordsToExclude, bool bCheckForAnyIncluded, bool bCheckForAnyExcluded)
{

	TESObjectREFR* TempRef = NULL;


	int iKeywordToExcludeSize = KeywordsToExclude.size();
	bool bCheckForKeywords = KeywordsToInclude.size();

	for (UInt64 i = 0; i < Cell->references.size(); i++)
	{
		TempRef = Cell->references[i].get();

		if (TempRef == NULL)
		{
			continue;
		}


		if ((UInt32)TempRef->GetObjectReference()->formType.underlying() == Type)
		{
			if (ChosenMod)
			{
				if (ChosenMod->IsFormInMod(TempRef->formID))
				{
					if (bCheckForKeywords)
					{
						CheckRefForKeywordsAndAddToArray(result, TempRef, KeywordsToInclude, KeywordsToExclude, bCheckForAnyIncluded, bCheckForAnyExcluded, iKeywordToExcludeSize);
					}
					else {
						result.Push(&TempRef);
					}



				}

			}
			else {
				if (bCheckForKeywords)
				{
					CheckRefForKeywordsAndAddToArray(result, TempRef, KeywordsToInclude, KeywordsToExclude, bCheckForAnyIncluded, bCheckForAnyExcluded, iKeywordToExcludeSize);
				}
				else {
					result.Push(&TempRef);
				}
			}
			
		}
		
	}
}








VMArray<TESObjectREFR*> cmd_GetReferencesInCellByType(StaticFunctionTag* base, TESObjectCELL* Cell, UInt32 Type, BSFixedString PluginName, UInt32 iDepth, VMArray<BGSKeyword*> KeywordsToInclude, VMArray<BGSKeyword*> KeywordsToExclude, bool bCheckForAnyIncluded, bool bCheckForAnyExcluded)
{
	_DMESSAGE("GetReferencesInCellByTypestarts with Depth>>%d, Type>>%d", iDepth,Type);
	VMArray<TESObjectREFR*> result;



	if (!Cell)
	{
		_DMESSAGE("Chosen cell is NULL");
		return result;
	}


	if (Type > 158)
	{
		_DMESSAGE("Type exceeds max value");
		return result;
	}
	else if (Type < 1)
	{
		_DMESSAGE("Type  is less than 1");
		return result;
	}

	string sPluginName = PluginName.c_str();


	const ModInfo* ChosenMod = NULL;

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
	}


	vector< BGSKeyword*> vKeywordsToInclude = ConvertVMArrayToVectorCheckType(KeywordsToInclude, kFormType_KYWD);
	vector< BGSKeyword*> vKeywordsToExclude = ConvertVMArrayToVectorCheckType(KeywordsToExclude, kFormType_KYWD);




	int iInterior = f_BB_IsCellInterior(Cell);


	if (iInterior == 0)
	{
		if (iDepth == -1)
		{
			Setting* MySetting = GetINISettingSUP("uGridsToLoad:General");
			if (MySetting != NULL)
			{
				iDepth = MySetting->GetUInt();
			}
			else {
				iDepth = 5;
				_DMESSAGE("uGridsToLoad:General doesn't exist");
			}
		}
	}


	_DMESSAGE("Cell scan Depth is %d", iDepth);


	CellScanInfoSUP info(iDepth, Cell);


	for (auto it = info.v_FoundCells.begin(); it != info.v_FoundCells.end(); it++)
	{
		f_AddObjectFromCelByType(Iter, result, Type, ChosenMod, vKeywordsToInclude, vKeywordsToExclude,  bCheckForAnyIncluded, bCheckForAnyExcluded);
	}



	return result;
}






VMArray<TESObjectREFR*> cmd_FilterRefArrayByActorValues(StaticFunctionTag* base, VMArray<TESObjectREFR*> sourceArray, VMArray<ActorValueInfo*> AVIncluded, VMArray<ActorValueInfo*> AVExcluded)
{
	VMArray<TESObjectREFR*> result;

	if (!AVIncluded.Length())
	{
		_DMESSAGE("Chosen AV array is 0");
		return result;
	}

	vector<ActorValueInfo*> vAVIncluded = ConvertVMArrayToVector(AVIncluded);

	if (!vAVIncluded.size())
	{
		_DMESSAGE("No actor values included provided");
		return result;
	}


	vector<ActorValueInfo*> vAVExcluded = ConvertVMArrayToVector(AVExcluded);



	for (UInt32 i = 0; i < sourceArray.Length(); i++)
	{
		TESObjectREFR* CurrentRef;
		sourceArray.Get(&CurrentRef, i);
		if (CurrentRef)
		{

			bool bFound = false;
			for (auto it = vAVIncluded.begin(); it != vAVIncluded.end(); it++)
			{
				if (ActorValueFromRefr(CurrentRef, Iter))
				{
					bFound = true;
					break;
				}

			}

			if (!bFound)
			{
				continue;
			}


			bFound = false;

			for (auto it = vAVExcluded.begin(); it != vAVExcluded.end(); it++)
			{
				if (ActorValueFromRefr(CurrentRef, Iter))
				{
					bFound = true;
					break;
				}

			}

			if (!bFound)
			{
				result.Push(&CurrentRef);
			}

		}

	}


	return result;
}




UInt32 cmd_IsAnyOfTypes(StaticFunctionTag* base, TESForm* CurrentObject, UInt32 Type1, UInt32 Type2, UInt32 Type3, UInt32 Type4, UInt32 Type5, UInt32 Type6, UInt32 Type7, UInt32 Type8, UInt32 Type9)
{
	if (!CurrentObject)
	{
		_DMESSAGE("Object is not valid");
		return -1;
	}

	UInt8 Type = (UInt8)CurrentObject->formType.underlying();

	//Console_Print("TYPE OF OBJECT>>%d", Type);

	if (Type == Type1)
	{
		return 1;
	}
	else if (Type == Type2){

		return 2;
	}
	else if (Type == Type3) {

		return 3;
	}
	else if (Type == Type4) {

		return 4;
	}
	else if (Type == Type5) {

		return 5;
	}
	else if (Type == Type6) {

		return 6;
	}
	else if (Type == Type7) {

		return 7;
	}
	else if (Type == Type8) {

		return 8;
	}
	else if (Type == Type9) {

		return 9;
	}


	return 0;
}






VMArray<TESObjectREFR*> cmd_FilterObjectRefArrayByRefs(StaticFunctionTag* tag, VMArray<TESObjectREFR*> sourceArray, VMArray<TESObjectREFR*> excludeRefs)
{
	VMArray<TESObjectREFR*> result;
	_DMESSAGE("FilterObjectRefArrayByRefs starts");


	if (!sourceArray.Length())
	{
		_DMESSAGE("sourceArray doesn't have any refs");
		return result;
	}

	if (!excludeRefs.Length())
	{
		_DMESSAGE("excludeRefs doesn't have any refs");
		return result;
	}


	vector<TESObjectREFR*> vExcludeRefs = ConvertVMArrayToVector(excludeRefs);


	for (UInt32 i = 0; i < sourceArray.Length(); i++)
	{
		bool bFound = false;
		TESObjectREFR* CurrentRef;
		sourceArray.Get(&CurrentRef, i);
		if (CurrentRef)
		{
			for (auto it = vExcludeRefs.begin(); it != vExcludeRefs.end(); it++)
			{
				if (Iter->formID == CurrentRef->formID)
				{
					bFound = true;
					break;
				}
			}

			if (!bFound)
			{
				result.Push(&CurrentRef);
			}

		}
	}



	_DMESSAGE("FilterObjectRefArrayByRefs ends");
	return result;
}












//bool cmd_RemoveConnectedObject(StaticFunctionTag* base, TESObjectREFR* thisRef, TESObjectREFR* refToRemove)
//{
//
//	if (!thisRef)
//		return false;
//
//	if (!refToRemove)
//		return false;
//
//	ExtraDataList* extraDataList = thisRef->extraDataList.get();
//
//
//	if (!extraDataList || !extraDataList->HasType(kExtraData_PowerLinks))
//	{
//		return false;
//	}
//
//
//
//	ExtraPowerLinks* powerLinks = (ExtraPowerLinks*)extraDataList->GetByType(kExtraData_PowerLinks);
//	if (!powerLinks)
//	{
//		return false;
//	}
//
//	_DMESSAGE("Checks passed, connection count>>%d", powerLinks->connections.count);
//	for (int i = 0; i < powerLinks->connections.count; i++)
//	{
//		UInt64 formID = 0;
//		powerLinks->connections.GetNthItem(i, formID);
//
//		TESForm* form = LookupFormByID(formID);
//		if (form)
//		{
//			_DMESSAGE("Checking form>>%x", form->formID);
//			TESObjectREFR* refr = DYNAMIC_CAST(form, TESForm, TESObjectREFR);
//			if (refr->formID == refToRemove->formID)
//			{
//				_DMESSAGE("removing form from connections");
//				return powerLinks->connections.Remove(i);
//			}
//				
//		}
//	}
//
//	return false;
//}










//if any, GetWorkshopPowerGridCapacity(ObjectReference workshop, Int powerGridId) and GetWorkshopPowerGridLoad(ObjectReference workshop, Int powerGridID) where powerGridID < 0 means all


//ActorValueInfo* g_HealthValueInfo = NULL;
//			g_HealthValueInfo = (ActorValueInfo*)LookupFormByID(724);
//float fCurrentHealth = -1;
//fCurrentHealth = (*g_player)->actorValueOwner.GetValue(g_HealthValueInfo);


//IsOnPowerGrid(ObjRef workshop, Int powergridId, ObjRef referenceToTest)
//ListPowerGrids()


//set mm visible
			//MiscComponent comp;
			//components.Get(&comp, i);

			//UInt32 count;
			//TESObjectMISC::Component cp;
			//comp.Get("object", &cp.component);
			//comp.Get("count", &count);
			//cp.count = count;
			//thisObject->components->Push(cp);