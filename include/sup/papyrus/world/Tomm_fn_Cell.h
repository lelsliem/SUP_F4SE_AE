


bool __forceinline f_IsCellVisited(TESObjectCELL* Cell)
{

	if (!Cell->extraDataList) {

		return false;
	}

	if (!Cell->extraDataList->GetByType(kExtraData_SeenData))
	{
		return false;
	}

	return true;
}



bool cmd_IsCellVisited(StaticFunctionTag* base, TESObjectCELL* Cell)
{
	if (!Cell)
		return false;

	return f_IsCellVisited(Cell);
}

TESWorldSpace* cmd_GetCellWorldSpace(StaticFunctionTag* base, TESObjectCELL* Cell)
{

	if (!Cell)
		return NULL;

	return Cell->worldSpace;
}





VMArray<TESObjectCELL*> cmd_GetNearCells(StaticFunctionTag* base, TESObjectCELL* Cell, UInt32 iDepth)
{
	_DMESSAGE("GetNearCells starts with Depth>>%d", iDepth);
	VMArray<TESObjectCELL*> result;


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

	result = VMArray<TESObjectCELL*>();
	for (auto& CellRef : info.v_FoundCells)
	{
		result.push_back(CellRef);
	}
	return result;
}