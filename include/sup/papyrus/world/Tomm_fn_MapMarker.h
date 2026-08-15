
// F4SE TESFullName is a plain { BSFixedString name; } component � RE's TESFullName is
// abstract, so the map-marker struct uses a layout-compatible stand-in.
struct SUP_TESFullNameCompat
{
	BSFixedString name; // 00
};

class SUP_MapMarkerData // from CommonLibSSE (FO4 layout)
{
public:
	SUP_TESFullNameCompat	locationName; //00
	UInt8					flags;			// 08
	UInt8					type;			// 09
	UInt16					pad;			// 0A
	SUP_TESFullNameCompat	MMName;			// 0C
};

class SUP_ExtraDataMapMarkerRef : public BSExtraData // from CommonLibSSE
{
public:
	SUP_MapMarkerData* mapData;
};




bool cmd_IsMapMarker(StaticFunctionTag* base, TESObjectREFR* thisForm)
{
	if (!thisForm)
		return false;


	if (!thisForm->extraDataList)
	{
		_DMESSAGE("No extra data");
		return false;
	}

	BSExtraData* MyExtraDataTemp = thisForm->extraDataList->GetByType(kExtraData_MapMarker);
	if (!MyExtraDataTemp)
	{
		_DMESSAGE("No extra data Map Marker");
		return false;
	}
	SUP_ExtraDataMapMarkerRef* ExtraMapMarker = (SUP_ExtraDataMapMarkerRef*)MyExtraDataTemp;

	if (ExtraMapMarker->mapData)
	{
		return true;
	}
	else {
		return false;
	}
}

bool cmd_MapMarkerSetName(StaticFunctionTag* base, TESObjectREFR* thisForm, BSFixedString StringIN)
{
	//thisForm = (TESObjectREFR*)LookupFormByID(117464);

	if (!thisForm)
		return false;

	if (!thisForm->extraDataList)
	{
		_DMESSAGE("No extra data");
		return false;
	}

	BSExtraData* MyExtraDataTemp = thisForm->extraDataList->GetByType(kExtraData_MapMarker);
	if (!MyExtraDataTemp)
	{
		_DMESSAGE("No extra data Map Marker");
		return false;
	}
	SUP_ExtraDataMapMarkerRef* ExtraMapMarker = (SUP_ExtraDataMapMarkerRef*)MyExtraDataTemp;

	if (ExtraMapMarker->mapData)
	{
		ExtraMapMarker->mapData->locationName.name = StringIN;

		//ExtraMapMarker->mapData->MMName.name = StringIN;


		//TESFullName* pFullName = DYNAMIC_CAST(thisForm, TESForm, TESFullName);
		//if (pFullName) {
		//	pFullName->name = StringIN;
		//}

		return true;
	}
	else {
		return false;
	}
}

BSFixedString cmd_MapMarkerGetName(StaticFunctionTag* base, TESObjectREFR* thisForm)
{
	//thisForm = (TESObjectREFR*)LookupFormByID(456148);

	if (!thisForm)
		return "";

	if (!thisForm->extraDataList)
	{
		_DMESSAGE("No extra data");
		return "";
	}

	BSExtraData* MyExtraDataTemp = thisForm->extraDataList->GetByType(kExtraData_MapMarker);
	if (!MyExtraDataTemp)
	{
		_DMESSAGE("No extra data Map Marker");
		return "";
	}
	SUP_ExtraDataMapMarkerRef* ExtraMapMarker = (SUP_ExtraDataMapMarkerRef*)MyExtraDataTemp;

	if (ExtraMapMarker->mapData)
	{

		return ExtraMapMarker->mapData->locationName.name;
	}
	else {
		return "";
	}
}



bool cmd_MapMarkerGetUseLocationName(StaticFunctionTag* base, TESObjectREFR* thisForm)
{
	//thisForm = (TESObjectREFR*)LookupFormByID(140840);
	if (!thisForm)
		return false;

	if (!thisForm->extraDataList)
	{
		_DMESSAGE("No extra data");
		return false;
	}

	BSExtraData* MyExtraDataTemp = thisForm->extraDataList->GetByType(kExtraData_MapMarker);
	if (!MyExtraDataTemp)
	{
		_DMESSAGE("No extra data Map Marker");
		return false;
	}
	SUP_ExtraDataMapMarkerRef* ExtraMapMarker = (SUP_ExtraDataMapMarkerRef*)MyExtraDataTemp;

	if (ExtraMapMarker->mapData)
	{

		if (bDebugMode)
		{
			int iCount = 0;

			while (iCount < 20)
			{
				//_DMESSAGE("Bit>>%s set>>>%d", to_string(iCount), getBit(ExtraMapMarker->mapData->flags, iCount));
				iCount += 1;
			}
		}


		return getBit(ExtraMapMarker->mapData->flags, 11);


	}
	else {
		return false;
	}
}






bool cmd_MapMarkerSetUseLocationName(StaticFunctionTag* base, TESObjectREFR* thisForm, bool bBool)
{
	//thisForm = (TESObjectREFR*)LookupFormByID(140840);

	if (!thisForm)
		return false;

	if (!thisForm->extraDataList)
	{
		_DMESSAGE("No extra data");
		return false;
	}

	BSExtraData* MyExtraDataTemp = thisForm->extraDataList->GetByType(kExtraData_MapMarker);
	if (!MyExtraDataTemp)
	{
		_DMESSAGE("No extra data Map Marker");
		return false;
	}
	SUP_ExtraDataMapMarkerRef* ExtraMapMarker = (SUP_ExtraDataMapMarkerRef*)MyExtraDataTemp;

	if (ExtraMapMarker->mapData)
	{

		if (bDebugMode)
		{
			_DMESSAGE("Flags were>>>%d", ExtraMapMarker->mapData->flags);
			int iCount = 0;

			while (iCount < 20)
			{
				_DMESSAGE("Bit>>%s set>>>%d", to_string(iCount).c_str(), getBit(ExtraMapMarker->mapData->flags, iCount));
				iCount += 1;
			}
		}




		if (bBool)
		{
			ExtraMapMarker->mapData->flags = setABit(ExtraMapMarker->mapData->flags, 12);
		}
		else
		{
			ExtraMapMarker->mapData->flags = clearABit(ExtraMapMarker->mapData->flags, 12);
		}
		//_DMESSAGE("Flags became >>>%d", ExtraMapMarker->mapData->flags);


		if (bDebugMode)
		{
			int iCount = 0;

			while (iCount < 20)
			{
				//_DMESSAGE("Bit>>%s set>>>%d", to_string(iCount), getBit(ExtraMapMarker->mapData->flags, iCount));
				iCount += 1;
			}
		}



		return true;
	}
	else {
		return false;
	}
}








bool cmd_GetPlayerHasMapMarker(StaticFunctionTag* base)
{
	return (bool)f_GetPlayerMapMarker();
}


SUPReferenceInfo cmd_GetPlayerMapMarkerInfo(StaticFunctionTag* base)
{

	SUPReferenceInfo result;

	auto MapMarker = f_GetPlayerMapMarker();

	if (!MapMarker)
	{
		return result;
	}


	result.Set<bool>("exists", true);
	result.Set<float>("x", MapMarker->GetPosition().x);
	result.Set<float>("y", MapMarker->GetPosition().y);
	result.Set<float>("z", MapMarker->GetPosition().z);

	if (MapMarker->parentCell)
	{
		result.Set<TESWorldSpace*>("MarkerWorldSpace", MapMarker->parentCell->worldSpace);
	}

	
	return result;
}





bool cmd_SetPlayerMapMarker(StaticFunctionTag* base, TESWorldSpace* TargetWorldSpace, float fPosX, float fPosY, float fPosZ)
{
	_DMESSAGE("SetPlayerMapMarker vstarts");
	if (!TargetWorldSpace)
	{
		_DMESSAGE("Worldspace not valid");
		return false;
	}


	NiPoint3 CoordsToPass(fPosX,fPosY,fPosZ);

	//Console_Print("Coords to set from function,X>>%f,Y>>%f,Z>>%f", CoordsToPass.x, CoordsToPass.y, CoordsToPass.z);

	sup::compat::SetPlayerMapMarker(*g_player, &CoordsToPass, TargetWorldSpace);
	return true;
}


bool cmd_RemovePlayerMapMarker(StaticFunctionTag* base)
{

	if (!f_GetPlayerMapMarker())
	{
		_DMESSAGE("Marker doesn't exist");
		return false;
	}


	sup::compat::RemovePlayerMapMarker(*g_player);
	return true;
}