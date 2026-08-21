
PowerGridInstance cmd_GetPowerGridForObject(StaticFunctionTag* base, TESObjectREFR* WorkshopRef, TESObjectREFR* ObjectRef)
{
	PowerGridInstance result;
	result.Set<UInt32>("ID", -1);


	if (!WorkshopRef)
	{
		_DMESSAGE("Workshop not valid");
		return result;
	}
	if (!ObjectRef)
	{
		_DMESSAGE("ObjectRef not valid");
		return result;
	}

	Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(WorkshopRef);

	if (!WorkshopData)
	{
		_DMESSAGE("No workshop extra data");
		return result;
	}


	for (UInt64 i = 0; i < WorkshopData->powerGrid.size(); i++)
	{
		PowerUtils::PowerGrid* pGrid = NULL;
		sup::compat::GetNthItem(WorkshopData->powerGrid, i, pGrid);

		if (pGrid)
		{
			auto FoundItem = pGrid->adjacencyMap.find(ObjectRef->formID);

			if (FoundItem != pGrid->adjacencyMap.end())
			{
				TESObjectREFR* TempRef = (TESObjectREFR*)LookupFormByID(FoundHashItem.first);
				if (TempRef)
				{
					result.Set<UInt32>("ID", i);
					result.Set<float>("Load", pGrid->load);
					result.Set<float>("Capacity", pGrid->capacity);
					return result;
				}
				else {
					_DMESSAGE("ObjectRef does NOT exist");
					return result;
				}
			}
		}
	}

	return result;

}



UInt32 cmd_IsWorkshopPowerGridOverloaded(StaticFunctionTag* base, TESObjectREFR* WorkshopRef)
{
	if (!WorkshopRef)
	{
		_DMESSAGE("Workshop not valid");
		return -3;
	}

	Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(WorkshopRef);

	if (!WorkshopData)
	{
		_DMESSAGE("No workshop extra data");
		return -2;
	}

	for (UInt64 i = 0; i < WorkshopData->powerGrid.size(); i++)
	{
		PowerUtils::PowerGrid* pGrid = NULL;
		sup::compat::GetNthItem(WorkshopData->powerGrid, i, pGrid);

		if (pGrid)
		{

			if (pGrid->load > pGrid->capacity)
			{
				return i;
			}
		}

	}

	return -1;

}


VMArray<PowerGridInstance> cmd_ListPowerGrids(StaticFunctionTag* base, TESObjectREFR* WorkshopRef)
{

	VMArray<PowerGridInstance> result;

	if (!WorkshopRef)
	{
		_DMESSAGE("Workshop not valid");
		return result;
	}

	Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(WorkshopRef);

	if (!WorkshopData)
	{
		_DMESSAGE("No workshop extra data");
		return result;
	}


	for (UInt64 i = 0; i < WorkshopData->powerGrid.size(); i++)
	{
		PowerUtils::PowerGrid* pGrid = NULL;
		sup::compat::GetNthItem(WorkshopData->powerGrid, i, pGrid);
		if (pGrid)
		{
			PowerGridInstance TempPGrip;
			TempPGrip.Set<UInt32>("ID", i);
			TempPGrip.Set<float>("Load", pGrid->load);
			TempPGrip.Set<float>("Capacity", pGrid->capacity);
			result.Push(&TempPGrip);
		}
	}

	return result;
}


VMArray<TESObjectREFR*> cmd_GetPowerGridElements(StaticFunctionTag* base, TESObjectREFR* WorkshopRef, UInt32 iGridID)
{
	_DMESSAGE("GetPowerGridElements starts");
	VMArray<TESObjectREFR*> result;

	if (!WorkshopRef)
	{
		_DMESSAGE("Workshop not valid");
		return result;
	}

	Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(WorkshopRef);

	if (!WorkshopData)
	{
		_DMESSAGE("No workshop extra data");
		return result;
	}

	PowerUtils::PowerGrid* pGrid = NULL;
	sup::compat::GetNthItem(WorkshopData->powerGrid, iGridID, pGrid);
	if (pGrid)
	{
		for (auto it = pGrid->adjacencyMap.begin(); it != pGrid->adjacencyMap.end(); it++)
		{

			TESObjectREFR* TempRef = (TESObjectREFR*)LookupFormByID(Iter.first);

			if (TempRef)
			{
				result.Push(&TempRef);
			}
			else {
				_DMESSAGE("ObjectRef does NOT exist");
			}
		}
	}
	return result;
}







bool cmd_AreDirectlyConnected(StaticFunctionTag* base, TESObjectREFR* WorkshopRef, TESObjectREFR* TempRefA, TESObjectREFR* TempRefB)
{
	_DMESSAGE("SUP::AreDirectlyConnected starts");


	if (!TempRefA)
	{
		_DMESSAGE("TempRefA not valid");
		return false;
	}

	if (!TempRefB)
	{
		_DMESSAGE("TempRefB not valid");
		return false;
	}

	if (!WorkshopRef)
	{
		_DMESSAGE("Workshop not valid");
		return false;
	}

	Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(WorkshopRef);

	if (!WorkshopData)
	{
		_DMESSAGE("No workshop extra data");
		return false;
	}

	_DMESSAGE("ALL valid - calling");
	return sup::compat::AreDirectlyConnected(WorkshopData, TempRefA, TempRefB);
}



UInt32 cmd_RemovePowerGrid(StaticFunctionTag* base, TESObjectREFR* WorkshopRef, UInt32 iIndexToRemove)
{

	VMArray<PowerGridInstance> result;

	if (!WorkshopRef)
	{
		_DMESSAGE("Workshop not valid");
		return -3;
	}

	Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(WorkshopRef);

	if (!WorkshopData)
	{
		_DMESSAGE("No workshop extra data");
		return -2;
	}

	if (iIndexToRemove > WorkshopData->powerGrid.size() - 1)
	{
		_DMESSAGE("Index out of range");
		return -1;
	}

	if (sup::compat::RemoveAt(WorkshopData->powerGrid, iIndexToRemove))
	{
		return 1;
	}
	else {
		return 0;
	}
}




void OutputPowerGridHelper(Workshop::ExtraData* WorkshopData)
{
	_DMESSAGE("OutputPowerGridHelper starts");
	if (!WorkshopData->currentPowerGrid)
	{
		_DMESSAGE("No current powerGrid");
	}
	else {
		_DMESSAGE("Current powerGrid");
	}

	_DMESSAGE("Iterating adjacency map ");



	for (UInt64 i = 0; i < WorkshopData->powerGrid.size(); i++)
	{
		
		PowerUtils::PowerGrid* pGrid = NULL;
		sup::compat::GetNthItem(WorkshopData->powerGrid, i, pGrid);

		if (pGrid)
		{
			_DMESSAGE("new powerGrid>>%d, adjacency map count>>>%d", i, pGrid->adjacencyMap.size());

			for (auto it = pGrid->adjacencyMap.begin(); it != pGrid->adjacencyMap.end(); it++)
			{
				_DMESSAGE("New adjacency map,Key---->%x", Iter.first);


				for (auto it2 = Iter.second->begin(); it2 != Iter.second->end(); it2++)
				{
					_DMESSAGE("Grid connection::Connector>>%x,Connection>>%x", Iter2.connector, Iter2.connection);

					//TESObjectREFR* connector = (TESObjectREFR*)LookupFormByID(Iter2.connector);

					//if (connector)
					//{
					//	_DMESSAGE("Connector valid,type is>>>%d", connector->formType);
		
					//	//if (connector->GetObjectReference())
					//	//{
					//	//	_DMESSAGE("Connector base form>>%x", connector->GetObjectReference()->formID);
					//	//}
					//	//else {
					//	//	_DMESSAGE("Connector has no base form");
					//	//}
					//}
					//else {
					//	//_DMESSAGE("Connector is not valid");
					//}

				}
			}


		}
	}


	_DMESSAGE("OutputPowerGridHelper finishes");
}



bool cmd_OutputPowerGrid(StaticFunctionTag* base, TESObjectREFR* WorkshopRef)
{
	_DMESSAGE("SUP::OutputPowerGrid starts");

	if (!WorkshopRef)
	{
		_DMESSAGE("Workshop not valid");
		return false;
	}

	Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(WorkshopRef);

	if (!WorkshopData)
	{
		_DMESSAGE("No workshop extra data");
		return false;
	}

	_DMESSAGE("WorkshopData valid ");
	OutputPowerGridHelper(WorkshopData);
	_DMESSAGE("SUP::OutputPowerGrid finished");
	return true;

}







bool cmd_IsDirectGridConnection(StaticFunctionTag* base, TESObjectREFR* WorkshopRef, TESObjectREFR* TempRefA, TESObjectREFR* TempRefB)
{
	_DMESSAGE("SUP::IsDirectGridConnection starts");


	if (!TempRefA)
	{
		_DMESSAGE("TempRefA not valid");
		return false;
	}

	if (!TempRefB)
	{
		_DMESSAGE("TempRefB not valid");
		return false;
	}

	if (!WorkshopRef)
	{
		_DMESSAGE("Workshop not valid");
		return false;
	}

	Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(WorkshopRef);

	if (!WorkshopData)
	{
		_DMESSAGE("No workshop extra data");
		return false;
	}


	for (UInt64 i = 0; i < WorkshopData->powerGrid.size(); i++)
	{
		PowerUtils::PowerGrid* pGrid = NULL;
		sup::compat::GetNthItem(WorkshopData->powerGrid, i, pGrid);

		if (pGrid)
		{
			for (auto it = pGrid->adjacencyMap.begin(); it != pGrid->adjacencyMap.end(); it++)
			{
				if (TempRefA->formID == Iter.first)
				{
					for (auto it2 = Iter.second->begin(); it2 != Iter.second->end(); it2++)
					{
						if (Iter2.connection == TempRefB->formID)
						{
							_DMESSAGE("SUP::IsDirectGridConnection end - true");
							return true;
						}
					}
				}

			}

		}
	}

	_DMESSAGE("SUP::IsDirectGridConnection end - false");
	return false;
}

VMArray<TESObjectREFR*>  cmd_ScanPowerGridForFaults(StaticFunctionTag* base, TESObjectREFR* WorkshopRef, UInt32 iPowerGridID)
{
	_DMESSAGE("SUP::ScanPowerGridForFaults starts");
	
	VMArray<TESObjectREFR*> result;

	if (!WorkshopRef)
	{
		_DMESSAGE("Workshop not valid");
		return result;
	}
	Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(WorkshopRef);

	if (!WorkshopData)
	{
		_DMESSAGE("No workshop extra data");
		return result;
	}


	PowerUtils::PowerGrid* pGrid = NULL;
	sup::compat::GetNthItem(WorkshopData->powerGrid, iPowerGridID, pGrid);


	if (!pGrid)
	{
		_DMESSAGE("Cannot extract power grid by specified id, count>>%d", WorkshopData->powerGrid.size());
		return result;
	}


	for (auto it = pGrid->adjacencyMap.begin(); it != pGrid->adjacencyMap.end(); it++)
	{


		for (auto it2 = Iter.second->begin(); it2 != Iter.second->end(); it2++)
		{
			TESObjectREFR* CurrentObject = (TESObjectREFR*)LookupFormByID(Iter.first);

			if (Iter2.connector)
			{
				auto Connector = (TESObjectREFR*)LookupFormByID(Iter2.connector);

				if (!Connector)
				{
					if (!CurrentObject) {
						_DMESSAGE("Adjacency map key not valid>>>%x", Iter.first);
						break;
					}

					result.Push(&CurrentObject);
					break;
				}

			}


			auto Connection = (TESObjectREFR*)LookupFormByID(Iter2.connection);

			if (!Connection)
			{
				TESObjectREFR* CurrentObject = (TESObjectREFR*)LookupFormByID(Iter.first);
				if (!CurrentObject) {
					_DMESSAGE("Adjacency map key not valid>>>%x", Iter.first);
					break;
				}

				result.Push(&CurrentObject);
				break;
			}
			
		}
	}

	return result;
}





vector<TESObjectREFR*> fn_GetConnectPoints( TESObjectREFR* refr)
{
	vector<TESObjectREFR*> results;
	if (!refr)
		return results;

	NiNode* root = sup::compat::GetObjectRootNode(refr);
	if (!root)
		return results;

	// Phase 3: BSConnectPoint::Parents is modeled in sup::compat (F4SE layout); the "CPA"
	// extra data on a snap parent is always this type, so the RTTI check is dropped.
	auto* extraData = static_cast<sup::compat::BSConnectPointParents*>(sup::compat::GetExtraData(root, "CPA"));
	if (extraData)
	{
		for (UInt32 i = 0; i < extraData->points.count; i++)
		{
			auto* connectPoint = extraData->points[i];
			if (connectPoint)
			{
				float yaw, pitch, roll;
				sup::compat::GetEulerAngles(connectPoint->rot, &roll, &pitch, &yaw);
				yaw *= 180.0 / MATH_PI;
				pitch *= 180.0 / MATH_PI;
				roll *= 180.0 / MATH_PI;

				NiPoint3 localPos = connectPoint->pos;

				NiPoint3 worldPos = localPos;
				NiAVObject* parent = root;

				const auto& worldTransform = parent->GetWorldTransform();
				worldPos = worldTransform.rotate.Transpose() * localPos + worldTransform.translate;


				if (refr->parentCell) {
					bhkWorld* world = sup::compat::GetHavokWorld(refr->parentCell);
					if (world) {
						TESObjectREFR* connected = sup::compat::GetObjectAtConnectPoint(refr, &worldPos, world, 8.0f);
						if (connected) {
							results.push_back(connected);
							_DMESSAGE("New Connect points for Target is>>%x", connected->formID);
						}

					}
				}


			}
		}
	}

	return results;
}















VMArray<GridConnection> cmd_GetDirectGridConnections(StaticFunctionTag* base, TESObjectREFR* WorkshopRef, TESObjectREFR* TempRefA)
{
	_DMESSAGE("SUP::GetDirectGridConnections starts");
	VMArray<GridConnection> result;

	if (!TempRefA)
	{
		_DMESSAGE("TempRefA not valid");
		return result;
	}


	if (!WorkshopRef)
	{
		_DMESSAGE("Workshop not valid");
		return result;
	}

	Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(WorkshopRef);

	if (!WorkshopData)
	{
		_DMESSAGE("GDPG::No workshop extra data");
		return result;
	}

	
	for (UInt64 i = 0; i < WorkshopData->powerGrid.size(); i++)
	{
		PowerUtils::PowerGrid* pGrid = NULL;
		sup::compat::GetNthItem(WorkshopData->powerGrid, i, pGrid);

		if (pGrid)
		{
			auto FoundItem = pGrid->adjacencyMap.find(TempRefA->formID);

			if (FoundItem != pGrid->adjacencyMap.end())
			{
				//_DMESSAGE("Found item in adjacency map");
				if (TempRefA->formID == FoundHashItem.first)
				{
					//_DMESSAGE("Checking>>%x", FoundHashItem.first);


					for (auto it2 = FoundHashItem.second->begin(); it2 != FoundHashItem.second->end(); it2++)
					{
						GridConnection NewConnection;
						NewConnection.Set<bool>("IsSnappedConnection", false);
						NewConnection.Set<TESObjectREFR*>("connector", nullptr);


						//_DMESSAGE("GDPG::Checking new connection(%x)-connector pair(%x)", Iter2.connection, Iter2.connector);
						auto ConnectionRef = (TESObjectREFR*)LookupFormByID(Iter2.connection);

						if (!ConnectionRef)
						{
							_DMESSAGE("GDPG::Cannot extract connection reference");
							continue;
						}
						NewConnection.Set<TESObjectREFR*>("connection", ConnectionRef);
						//_DMESSAGE("GDPG::Setting connection>>>%x", ConnectionRef->formID);

						TESObjectREFR* ConnectorRef = nullptr;

						if (Iter2.connector)
						{
							ConnectorRef = (TESObjectREFR*)LookupFormByID(Iter2.connector);

							if (ConnectorRef)
							{
								NewConnection.Set<TESObjectREFR*>("connector", ConnectorRef);
								//_DMESSAGE("GDPG::Setting connector>>>%x", ConnectorRef->formID);
							}
						}

						if (!ConnectorRef)
						{
							if (fn_IsSnappedConnection(TempRefA, ConnectionRef))
							{
								//_DMESSAGE("GDPG::Setting snap connection>>>true");
								NewConnection.Set<bool>("IsSnappedConnection", true);
							}
						}


						result.Push(&NewConnection);
					}


					return result;
				}


			}
			else {
				_DMESSAGE("GDPG::Did not found item in adj map - continuing.");
				continue;
			}
		}
	}

	_DMESSAGE("SUP::GetDirectGridConnections end");
	return result;
}









bool cmd_IsSnappedGridConnection(StaticFunctionTag* base, TESObjectREFR* WorkshopRef, TESObjectREFR* TempRefA, TESObjectREFR* TempRefB)
{
	_DMESSAGE("SUP::IsSnappedGridConnection starts");


	if (!TempRefA)
	{
		_DMESSAGE("TempRefA not valid");
		return false;
	}

	if (!TempRefB)
	{
		_DMESSAGE("TempRefB not valid");
		return false;
	}

	if (!WorkshopRef)
	{
		_DMESSAGE("Workshop not valid");
		return false;
	}

	Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(WorkshopRef);

	if (!WorkshopData)
	{
		_DMESSAGE("No workshop extra data");
		return false;
	}


	for (UInt64 i = 0; i < WorkshopData->powerGrid.size(); i++)
	{
		PowerUtils::PowerGrid* pGrid = NULL;
		sup::compat::GetNthItem(WorkshopData->powerGrid, i, pGrid);

		if (pGrid)
		{
			for (auto it = pGrid->adjacencyMap.begin(); it != pGrid->adjacencyMap.end(); it++)
			{
				if (TempRefA->formID == Iter.first)
				{

					for (auto it2 = Iter.second->begin(); it2 != Iter.second->end(); it2++)
					{
						if (Iter2.connection == TempRefB->formID)
						{
							return fn_IsSnappedConnection(TempRefA, TempRefB);
						}
					}
				}

			}

		}
	}

	_DMESSAGE("SUP::IsSnappedGridConnection end - false");
	return false;
}


bool cmd_WorkshopCalculateCapacityAndLoad(StaticFunctionTag* base, TESObjectREFR* WorkshopRef)
{
	_DMESSAGE("SUP::WorkshopCalculateCapacityAndLoad starts");


	if (!WorkshopRef)
	{
		_DMESSAGE("Workshop not valid");
		return false;
	}

	Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(WorkshopRef);

	if (!WorkshopData)
	{
		_DMESSAGE("No workshop extra data");
		return false;
	}

	_DMESSAGE("ALL valid - calling");
	sup::compat::CalculateCapacityAndLoad(WorkshopData);
	return true;
}



struct WireConnection {
	TESObjectREFR* Connection1 = NULL;
	TESObjectREFR* Connection2 = NULL;
};


bool IsInWireConnectionArray(vector<WireConnection>& result, UInt32& FormID1, UInt32 FormID2)
{
	for (auto it3 = result.begin(); it3 != result.end(); it3++)
	{
		if (Iter3.Connection1->formID == FormID1 && Iter3.Connection2->formID == FormID2)
		{
			return true;
		}
		else if (Iter3.Connection1->formID == FormID2 && Iter3.Connection2->formID == FormID1)
		{
			return true;
		}

	}

	return false;

}


vector<WireConnection> GetWirelessConnectionsForItem(Workshop::ExtraData* WorkshopData, TESObjectREFR* RadiatorRef) {

	_DMESSAGE("NISTON::GETW::GetWirelessConnectionsForItem starts");
	vector<WireConnection> result;

	UInt32 FormID = RadiatorRef->formID;

	for (UInt64 i = 0; i < WorkshopData->powerGrid.size(); i++)
	{
		PowerUtils::PowerGrid* pGrid;
		sup::compat::GetNthItem(WorkshopData->powerGrid, i, pGrid);

		if (!pGrid)
		{
			continue;
		}
		_DMESSAGE("NISTON::GETW::Processing powergrid>>%d", i);

		for (auto it = pGrid->adjacencyMap.begin(); it != pGrid->adjacencyMap.end(); it++)
		{
			

			if (Iter.first == FormID)
			{
				_DMESSAGE("NISTON::GETW::New key in adjacency map>>%x - RADIATOR", Iter.first);
				for (auto it2 = Iter.second->begin(); it2 != Iter.second->end(); it2++)
				{
					_DMESSAGE("NISTON::GETW::Checking connection>>%x", Iter2.connection);
					if (Iter2.connector)
					{
						_DMESSAGE("NISTON::GETW::CONNECTOR(WIRE) EXISTS - SKIP");
						continue;
					}


					if (Iter2.connection)
					{
						_DMESSAGE("NISTON::GETW::Connection exists");
						TESObjectREFR* ConnectionRef = (TESObjectREFR*)LookupFormByID(Iter2.connection);

						if (!ConnectionRef)
						{
							_DMESSAGE("NISTON::GETW::Cannot build connection ref - SKIP");
							continue;
						}

						if (!IsInWireConnectionArray(result, FormID, Iter2.connection))
						{
							WireConnection NewWireConnection;
							NewWireConnection.Connection1 = RadiatorRef;
							NewWireConnection.Connection2 = ConnectionRef;
							result.push_back(NewWireConnection);
							_DMESSAGE("NISTON::GETW::Adding new Wirless connection, Connection1>>%x,Connection2>>%x", Iter.first, Iter2.connection);
						}
						else {
							_DMESSAGE("NISTON::GETW::Adding IS ALREADY IN RESULT ARRAY, Connection1>>%x,Connection2>>%x", Iter.first, Iter2.connection);
						}
					}
				}
			}
			else {
				_DMESSAGE("NISTON::GETW::New key in adjacency map>>%x - NON RADIATOR", Iter.first);
				for (auto it2 = Iter.second->begin(); it2 != Iter.second->end(); it2++)
				{
					_DMESSAGE("NISTON::GETW::Checking connection>>%x", Iter2.connection);
					if (Iter2.connector)
					{
						_DMESSAGE("NISTON::GETW::CONNECTOR(WIRE) EXISTS - SKIP");
						continue;
					}

					if (Iter2.connection == FormID)
					{
						_DMESSAGE("NISTON::GETW::%x CONNECTED TO RADITOR", Iter2.connection);
						TESObjectREFR* KeyRef = (TESObjectREFR*)LookupFormByID(Iter.first);

						if (!KeyRef)
						{
							_DMESSAGE("NISTON::GETW::Cannot build key ref - SKIP");
							continue;
						}

						if (!IsInWireConnectionArray(result, FormID, Iter.first))
						{
							WireConnection NewWireConnection;
							NewWireConnection.Connection1 = RadiatorRef;
							NewWireConnection.Connection2 = KeyRef;
							result.push_back(NewWireConnection);
							_DMESSAGE("NISTON::GETW::Adding new Wirless connection, Connection1>>%x,Connection2>>%x", Iter.first, Iter2.connection);
						}
						else {
							_DMESSAGE("NISTON::GETW::Adding IS ALREADY IN RESULT ARRAY, Connection1>>%x,Connection2>>%x", Iter.first, Iter2.connection);
						}

					}
				}
			}
		}
		
	}

	_DMESSAGE("GetWirelessConnectionsForItem ends");
	return result;
}

ExtraLinkedChildrenSUP* f_GetLinkedChildrenExtraData(TESObjectREFR* WorkshopRef)
{

	BSExtraData* MyExtraDataTemp = WorkshopRef->extraDataList->GetByType(kExtraData_LinkedRefChildren);

	if (!MyExtraDataTemp)
	{
		_DMESSAGE("reference doesn't have linked refs extra data");
		return NULL;
	}

	return (ExtraLinkedChildrenSUP*)MyExtraDataTemp;

}

vector<TESObjectREFR*> f_GetWirelessObjectsInRangeForRadiator(TESObjectREFR* WorkshopRef, TESObjectREFR* RadiatorRef, Workshop::ExtraData* WorkshopData)
{
	vector<TESObjectREFR*> result;


	auto LinkedRefsExtra = f_GetLinkedChildrenExtraData(WorkshopRef);

	if (!LinkedRefsExtra)
	{
		_DMESSAGE("No LinkedRefsExtra");
		return result;
	}


		for (UInt32 i = 0; i < LinkedRefsExtra->LinkedChildren.count; i++)
		{
			ExtraLinkedChildrenSUP::LinkedRef pLink;
			LinkedRefsExtra->LinkedChildren.GetNthItem(i, pLink);

			if (pLink.keyword == g_SUPWorkshopKeyword)
			{

				NiPointer<TESObjectREFR> refr;
				LookupREFRByHandle(pLink.refr, refr);

				TESObjectREFR* CurrentRef = refr.get();
				if (CurrentRef && RadiatorRef->formID != CurrentRef->formID)
				{
					//_DMESSAGE("CHECKING REF>>%x", CurrentRef->formID);
					if (!ItemIsWirelessPowerReceiver(CurrentRef))
					{
						continue;
					}

					if (sup::compat::IsPowered(WorkshopData, CurrentRef))
					{
						//_DMESSAGE("%x is already powered!!>%x", CurrentRef->formID);
						continue;
					}

					if (IsWithinRadiatorRange(RadiatorRef, CurrentRef)) 
					{
					
						result.push_back(CurrentRef);
					}
				}
			}

		}

	return result;
}

void RadiatorFixProcessPowerOnOffEvent(bool bPoweredON, TESObjectREFR* Radiator, TESObjectREFR* WorkshopRef, Workshop::ExtraData* WorkshopData)
{
	if (!Radiator || !WorkshopRef || !WorkshopData || !g_RadiationActorValue)
	{
		return;
	}


	//Console_Print("Workshop>>%x", WorkshopRef->formID);
	//Console_Print("Switch>>%x", Radiator->formID);
	float fRadiationValue = ActorValueFromRefr(Radiator, g_RadiationActorValue);

	if (!fRadiationValue)
	{
		//Console_Print("Not radiator!");
		return;
	}

	_DMESSAGE("RadiatorFixProcessPowerOnOffEvent works for radiator >>%x, bPowerOn>>%d", Radiator->formID, bPoweredON);

	//Console_Print("Radiator!,value>>%f", fRadiationValue);

	//Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(WorkshopRef);

	//if (!WorkshopData)
	//{
	//	_DMESSAGE("Workshop doesn't have workshop data??>>%x", WorkshopRef->formID);
	//	return;
	//}

	_DMESSAGE("NISTON::Radiator %x powered>>>%d", Radiator->formID, bPoweredON);


	if (bPoweredON)
	{
		//Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(WorkshopRef);

		//if (!sup::compat::IsPowered(WorkshopData, Radiator))
		//{
		//	Console_Print("Current switch not powered.");
		//	return;

		//}

		auto ItemsToConnect = f_GetWirelessObjectsInRangeForRadiator(WorkshopRef, Radiator, WorkshopData);

		for (auto it = ItemsToConnect.begin(); it != ItemsToConnect.end(); it++)
		{
			_DMESSAGE("New connection to radiator is>>>%x, Radiator>>>%x", Iter->formID, Radiator->formID);

			// PowerUtils::ConnectToRadiator (REL::ID 2195090 -> 0x38EFA0), PDB-verified against
			// "PowerUtils::ConnectToRadiator(TESObjectREFR&,TESObjectREFR&,Workshop::ExtraData&)".
			ConnectToRadiator(Radiator, Iter, WorkshopData);
		}

	}
	else {


		auto WirelessConnections = GetWirelessConnectionsForItem(WorkshopData, Radiator);

		auto ConnectPoints = fn_GetConnectPoints(Radiator);


		for (auto it = WirelessConnections.begin(); it != WirelessConnections.end(); it++)
		{
			//_DMESSAGE("Removing wireless connection, Generator>>%x,Receiver>>%x", Iter.Connection1->formID, Iter.Connection2->formID);

			bool bFound = false;
			for (auto it3 = ConnectPoints.begin(); it3 != ConnectPoints.end(); it3++)
			{
				if (Iter3->formID == Iter.Connection2->formID)
				{
					_DMESSAGE("NISTON::Snapped connection>>%x   %s  found for radiator - NOT removing it.", Iter3->formID, sup::compat::GetFormFullName(Iter3));
					bFound = true;
					break;
				}

			}
			if (!bFound)
			{


				sup::compat::RemoveConnection(WorkshopData, Iter.Connection1, Iter.Connection2, (TESObjectREFR*)NULL, false);
				_DMESSAGE("NISTON::Removing wireless connection, Radiator>>%x %s,Receiver>>%x %s", Iter.Connection1->formID, sup::compat::GetFormFullName(Iter.Connection1), Iter.Connection2->formID, sup::compat::GetFormFullName(Iter.Connection2));
			}

		}







	}

}bool cmd_WorkshopCalculatePowerRating(StaticFunctionTag* base, TESObjectREFR* WorkshopRef)
{
	_DMESSAGE("SUP::WorkshopCalculatePowerRating starts");


	if (!WorkshopRef)
	{
		_DMESSAGE("Workshop not valid");
		return false;
	}

	Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(WorkshopRef);

	if (!WorkshopData)
	{
		_DMESSAGE("No workshop extra data");
		return false;
	}

	_DMESSAGE("ALL valid - calling");
	WorkshopCalculatePowerRating(WorkshopRef);
	return true;
}

bool cmd_WorkshopRecalculateResources(StaticFunctionTag* base, TESObjectREFR* WorkshopRef)
{
	_DMESSAGE("SUP::WorkshopRecalculateResourcesstarts");


	if (!WorkshopRef)
	{
		_DMESSAGE("Workshop not valid");
		return false;
	}

	Workshop::ExtraData* WorkshopExtraData = f_GetPowerGridExtraData(WorkshopRef);

	if (!WorkshopExtraData)
	{
		_DMESSAGE("No workshop extra data");
		return false;
	}

	_DMESSAGE("ALL valid - calling");


	//if (OnWorkshopPowerStateChangeEventHandler.HasEvents())
	//{
	//	WorkshopStatus WorkshopStatusBefore = GetWorkShopStatus(WorkshopExtraData, true, false);
	//	WorkshopRecalculateResources(WorkshopRef);

	//	PowerGridChanges resultChanges = HasWorkshopChanged(WorkshopStatusBefore, WorkshopExtraData);
	//	if (resultChanges.bChanged)
	//	{
	//		_DMESSAGE("P GRID CHANGED - RAISING EVENT");
	//		OnWorkshopPowerStateChangeEventHandler.ProcessEventOnWorkshopPowerStateChange(NULL, WorkshopRef, resultChanges, 4, NULL, NULL, NULL);
	//	}
	//	else {
	//		_DMESSAGE("P GRID NOT CHANGED - RAISING EVENT");
	//	}


	//}
	//else {
		WorkshopRecalculateResources(WorkshopRef);
	//}
	


	
	return true;
}







bool cmd_RemoveGridConnection(StaticFunctionTag* base, TESObjectREFR* WorkshopRef, TESObjectREFR* TempRefA, TESObjectREFR* TempRefB, TESObjectREFR* refSpline)
{
	_DMESSAGE("SUP::RemoveGridConnection starts");


	if (!TempRefA)
	{
		_DMESSAGE("TempRefA not valid");
		return false;
	}

	if (!TempRefB)
	{
		_DMESSAGE("TempRefB not valid");
		return false;
	}

	if (!WorkshopRef)
	{
		_DMESSAGE("Workshop not valid");
		return false;
	}

	Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(WorkshopRef);

	if (!WorkshopData)
	{
		_DMESSAGE("No workshop extra data");
		return false;
	}

	sup::compat::RemoveConnection(WorkshopData, TempRefA, TempRefB, refSpline, false);


	_DMESSAGE("SUP::RemoveGridConnectionend - false");
	return false;
}


bool cmd_AddGridConnection(StaticFunctionTag* base, TESObjectREFR* WorkshopRef, TESObjectREFR* TempRefA, TESObjectREFR* TempRefB, TESObjectREFR* refSpline)
{
	_DMESSAGE("SUP::AddGridConnection starts");


	if (!TempRefA)
	{
		_DMESSAGE("TempRefA not valid");
		return false;
	}

	if (!TempRefB)
	{
		_DMESSAGE("TempRefB not valid");
		return false;
	}

	if (!WorkshopRef)
	{
		_DMESSAGE("Workshop not valid");
		return false;
	}

	Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(WorkshopRef);

	if (!WorkshopData)
	{
		_DMESSAGE("No workshop extra data");
		return false;
	}

	sup::compat::AddConnection(WorkshopData, TempRefA, TempRefB, refSpline);
	_DMESSAGE("SUP::AddGridConnection - end");
	return false;
}





UInt32  cmd_ScanPowerGridForFaultsEx(StaticFunctionTag* base, TESObjectREFR* WorkshopRef, UInt32 iPowerGridID)
{
	_DMESSAGE("SUP::ScanPowerGridForFaults starts");

	UInt32 result = 0;

	if (!WorkshopRef)
	{
		_DMESSAGE("Workshop not valid");
		return result;
	}
	Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(WorkshopRef);

	if (!WorkshopData)
	{
		_DMESSAGE("No workshop extra data");
		return result;
	}


	PowerUtils::PowerGrid* pGrid = NULL;
	sup::compat::GetNthItem(WorkshopData->powerGrid, iPowerGridID, pGrid);


	if (!pGrid)
	{
		_DMESSAGE("Cannot extract power grid by specified id, count>>%d", WorkshopData->powerGrid.size());
		return result;
	}

	for (auto it = pGrid->adjacencyMap.begin(); it != pGrid->adjacencyMap.end(); it++)
	{
		for (auto it2 = Iter.second->begin(); it2 != Iter.second->end(); it2++)
		{
			TESObjectREFR* CurrentObject = (TESObjectREFR*)LookupFormByID(Iter.first);

			if (!CurrentObject) {
				_DMESSAGE("Adjacency map key not valid>>>%x", Iter.first);
				result += 1;
				continue;
			}

			if (Iter2.connector)
			{
				auto Connector = (TESObjectREFR*)LookupFormByID(Iter2.connector);

				if (!Connector)
				{
					_DMESSAGE("Connector not valid>>%x", Iter2.connector);
					result += 1;
					continue;
				}
			}

			auto Connection = (TESObjectREFR*)LookupFormByID(Iter2.connection);

			if (!Connection)
			{
				_DMESSAGE("Connection not valid>>%x", Iter2.connection);
				result += 1;
				continue;
			}

		}
	}
	return result;
}


//
//constexpr std::uint32_t GenerateCRC32(tcb::span<const std::uint8_t> a_data) noexcept
//{
//	constexpr std::array<std::uint32_t, 256> table{
//		0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
//		0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988, 0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
//		0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
//		0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
//		0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172, 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
//		0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
//		0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
//		0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924, 0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
//		0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
//		0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
//		0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E, 0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
//		0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
//		0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
//		0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0, 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
//		0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
//		0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
//		0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A, 0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
//		0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
//		0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
//		0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC, 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
//		0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
//		0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
//		0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236, 0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
//		0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
//		0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
//		0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38, 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
//		0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
//		0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
//		0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2, 0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
//		0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
//		0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
//		0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94, 0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
//	};
//
//	std::uint32_t hash = 0;
//	for (const auto byte : a_data) {
//		hash = (hash >> 8) ^ table[(hash ^ byte) & 0xFF];
//	}
//	return hash;
//}



//template <>
//struct BSHash::CRC32Hash<PowerUtils::GridConnection>
//{
//public:
//	UInt32 operator()(const PowerUtils::GridConnection a_key) const
//	{
//		return GenerateCRC32(tcb::span(reinterpret_cast<const std::uint8_t*>(std::addressof(a_key)), sizeof(PowerUtils::GridConnection)));
//	}
//};




UInt32  cmd_ScanPowerGridForFaultsAndFix(StaticFunctionTag* base, TESObjectREFR* WorkshopRef, UInt32 iPowerGridID)
{
	_DMESSAGE("SUP::ScanPowerGridForFaults starts");

	UInt32 result = 0;

	if (!WorkshopRef)
	{
		_DMESSAGE("Workshop not valid");
		return result;
	}
	Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(WorkshopRef);

	if (!WorkshopData)
	{
		_DMESSAGE("No workshop extra data");
		return result;
	}


	PowerUtils::PowerGrid* pGrid = NULL;
	sup::compat::GetNthItem(WorkshopData->powerGrid, iPowerGridID, pGrid);


	if (!pGrid)
	{
		_DMESSAGE("Cannot extract power grid by specified id, count>>%d", WorkshopData->powerGrid.size());
		return result;
	}

	vector<UInt32> keysToRemoveFromAdjMap;
	
	int iAdjArraysRemoved = 0;
	int iConnectionsRemoved = 0;

	for (auto it = pGrid->adjacencyMap.begin(); it != pGrid->adjacencyMap.end();it++)
	{
		TESObjectREFR* CurrentObject = (TESObjectREFR*)LookupFormByID(Iter.first);
		
		if (!CurrentObject) {
			_DMESSAGE("Adjacency map key not valid>>>%x", Iter.first);
			keysToRemoveFromAdjMap.push_back(Iter.first);
			continue;
		}

		vector<PowerUtils::GridConnection*> ConnectionsToRemove;

		for (auto it2 = Iter.second->begin(); it2 != Iter.second->end();it2++)
		{
			
			if (Iter2.connector)
			{
				auto Connector = (TESObjectREFR*)LookupFormByID(Iter2.connector);

				if (!Connector)
				{
					_DMESSAGE("Connector not valid>>%x", Iter2.connector);
					ConnectionsToRemove.push_back(&Iter2);
					continue;
				}
			}

			auto Connection = (TESObjectREFR*)LookupFormByID(Iter2.connection);

			if (!Connection)
			{
				_DMESSAGE("Connection not valid>>%x", Iter2.connection);
				ConnectionsToRemove.push_back(&Iter2);
				continue;
			}
		}

		for (auto it3 = ConnectionsToRemove.begin(); it3 != ConnectionsToRemove.end(); it3++)
		{
			_DMESSAGE("Removing Connection from ajdacency map>>%x. Connector>>%x,Connection>>%x", Iter.first,Iter3->connector, Iter3->connection);
			//Iter.second->erase((*Iter3));
			iConnectionsRemoved += 1;
		}

		if (Iter.second->size() == 0)
		{
			_DMESSAGE("Adjacency map is empty after cleaning - remove later>>>%x", Iter.first);
			keysToRemoveFromAdjMap.push_back(Iter.first);
		}

	}

	
	for (auto it = keysToRemoveFromAdjMap.begin(); it != keysToRemoveFromAdjMap.end(); it++)
	{
		_DMESSAGE("Removing key from ajdacency map>>%x", Iter);
		//pGrid->adjacencyMap.erase(Iter);
		iAdjArraysRemoved += 1;
	}

	_DMESSAGE("AdjArrays removed>>%d,Connections removed>>%d", iAdjArraysRemoved, iConnectionsRemoved);

	_DMESSAGE("SUP::ScanPowerGridForFaults ends");
	return result;
}





bool  cmd_IsObjectWirelessPowerReceiver(StaticFunctionTag* base, TESObjectREFR* ObjectRef)
{
	_DMESSAGE("IsObjectPowerReceiver starts");
	if (!ObjectRef)
	{
		_DMESSAGE("ObjectRef not valid");
		return false;
	}

	bool bResult = ItemIsWirelessPowerReceiver(ObjectRef);
	//Console_Print("result Power Receiver>>%d", bResult);
	return bResult;
}



// --- Wireless fix: full power-grid diff engine (ported from Tommy's original) ---
// The simplified version below only handled the case where the event source itself is the
// radiator. Tommy's original snapshotted the whole workshop BEFORE the game's handler ran,
// then diffed AFTER, calling RadiatorFixProcessPowerOnOffEvent for EVERY item whose power
// state changed. That is what actually catches a NISTRON light being connected/disconnected
// by a radiator when the radiator (not the light) is the event source.

struct WorkshopStatus {
	struct PowerGrid {
		float capacity = 0;
		float load = 0;
		int iPoweredItemsCount = 0;
		UInt32 ID = 0;
		PowerUtils::PowerGrid* Pointer = NULL;
	};
	vector<PowerGrid> PowerGrids;
	vector<UInt32> PoweredItems;
	UInt32 PowerRating = 0;
};

WorkshopStatus GetWorkShopStatus(Workshop::ExtraData* WorkshopData, bool bGetWorkshopStatus, bool bGetPoweredItems)
{
	WorkshopStatus result;

	if (bGetWorkshopStatus)
	{
		result.PowerGrids.reserve(WorkshopData->powerGrid.size());
		result.PowerRating = WorkshopData->powerRating;
	}

	for (UInt64 i = 0; i < WorkshopData->powerGrid.size(); i++)
	{
		PowerUtils::PowerGrid* pGrid = NULL;
		sup::compat::GetNthItem(WorkshopData->powerGrid, i, pGrid);
		if (!pGrid)
		{
			continue;
		}

		if (bGetWorkshopStatus)
		{
			WorkshopStatus::PowerGrid NewPowerGrid;
			NewPowerGrid.capacity = pGrid->capacity;
			NewPowerGrid.load = pGrid->load;
			NewPowerGrid.iPoweredItemsCount = static_cast<int>(pGrid->currentlyPowered.size());
			NewPowerGrid.Pointer = pGrid;
			NewPowerGrid.ID = static_cast<UInt32>(i);
			result.PowerGrids.push_back(NewPowerGrid);
		}

		if (bGetPoweredItems)
		{
			result.PoweredItems.reserve(result.PoweredItems.capacity() + pGrid->currentlyPowered.size());

			for (UInt64 j = 0; j < pGrid->currentlyPowered.size(); j++)
			{
				UInt32 CurrentlyPoweredID = 0;

				if (sup::compat::GetNthItem(pGrid->currentlyPowered, j, CurrentlyPoweredID))
				{
					result.PoweredItems.push_back(CurrentlyPoweredID);
				}
			}
		}
	}

	return result;
}

PowerGridChanges HasWorkshopChanged(WorkshopStatus& WorkshopStatusBefore, Workshop::ExtraData* WorkshopData)
{
	PowerGridChanges result;

	if (WorkshopData->powerGrid.size() != WorkshopStatusBefore.PowerGrids.size())
	{
		_DMESSAGE("Power grid count changed");
		result.bChanged = true;
	}

	if (WorkshopData->powerRating != WorkshopStatusBefore.PowerRating)
	{
		_DMESSAGE("Power rating changed, old>>%d,new>>%d", WorkshopStatusBefore.PowerRating, WorkshopData->powerRating);
		result.bChanged = true;
	}

	for (UInt64 i = 0; i < WorkshopData->powerGrid.size(); i++)
	{
		PowerUtils::PowerGrid* pGrid = NULL;
		sup::compat::GetNthItem(WorkshopData->powerGrid, i, pGrid);

		if (!pGrid)
		{
			continue;
		}

		for (auto it = WorkshopStatusBefore.PowerGrids.begin(); it != WorkshopStatusBefore.PowerGrids.end(); it++)
		{
			if (Iter.Pointer == pGrid)
			{
				if (pGrid->capacity != Iter.capacity)
				{
					result.AffectedPowerGrids.push_back(Iter.ID);
					WorkshopStatusBefore.PowerGrids.erase(it);
					result.bChanged = true;
					break;
				}

				if (pGrid->load != Iter.load)
				{
					result.AffectedPowerGrids.push_back(Iter.ID);
					WorkshopStatusBefore.PowerGrids.erase(it);
					result.bChanged = true;
					break;
				}

				if (pGrid->currentlyPowered.size() != static_cast<std::size_t>(Iter.iPoweredItemsCount))
				{
					result.AffectedPowerGrids.push_back(Iter.ID);
					WorkshopStatusBefore.PowerGrids.erase(it);
					result.bChanged = true;
					break;
				}

				WorkshopStatusBefore.PowerGrids.erase(it);
				break;
			}
		}
	}

	if (result.AffectedPowerGrids.size() || WorkshopStatusBefore.PowerGrids.size())
	{
		result.bChanged = true;
	}

	for (auto it = WorkshopStatusBefore.PowerGrids.begin(); it != WorkshopStatusBefore.PowerGrids.end(); it++)
	{
		result.DeletedPowerGrids.push_back(Iter.ID);
	}

	return result;
}

// Diff engine: every item whose power state changed since the snapshot gets the radiator fix.
void WirelessFixProcessChanges(WorkshopStatus& WorkshopStatusBefore, Workshop::ExtraData* WorkshopData, TESObjectREFR* WorkshopRef, UInt32 SelfFormID, int& iCheckForSelf, bool OnlyRemove)
{
	// Items that are powered NOW but were NOT in the before-snapshot -> switched ON.
	for (UInt64 i = 0; i < WorkshopData->powerGrid.size(); i++)
	{
		PowerUtils::PowerGrid* pGrid = NULL;
		sup::compat::GetNthItem(WorkshopData->powerGrid, i, pGrid);

		if (!pGrid)
		{
			continue;
		}

		for (UInt64 j = 0; j < pGrid->currentlyPowered.size(); j++)
		{
			UInt32 CurrentlyPoweredID = 0;

			if (!sup::compat::GetNthItem(pGrid->currentlyPowered, j, CurrentlyPoweredID))
			{
				continue;
			}

			bool bFound = false;
			for (auto it = WorkshopStatusBefore.PoweredItems.begin(); it != WorkshopStatusBefore.PoweredItems.end(); it++)
			{
				if (Iter == CurrentlyPoweredID)
				{
					bFound = true;
					WorkshopStatusBefore.PoweredItems.erase(it);
					break;
				}
			}

			if (!bFound && !OnlyRemove)
			{
				// Item switched ON.
				TESObjectREFR* CurrentObject = (TESObjectREFR*)LookupFormByID(CurrentlyPoweredID);

				if (CurrentObject)
				{
					if (SelfFormID == CurrentlyPoweredID)
					{
						iCheckForSelf = 0;
					}

					_DMESSAGE("Wireless processchanged calls RadiatorFixProcessPowerOnOffEvent TRUE>>%x", CurrentObject->formID);
					RadiatorFixProcessPowerOnOffEvent(true, CurrentObject, WorkshopRef, WorkshopData);
				}
			}
		}
	}

	// Items in the before-snapshot that are no longer powered -> switched OFF.
	for (auto it = WorkshopStatusBefore.PoweredItems.begin(); it != WorkshopStatusBefore.PoweredItems.end(); it++)
	{
		TESObjectREFR* CurrentObject = (TESObjectREFR*)LookupFormByID(Iter);
		if (CurrentObject)
		{
			if (SelfFormID == Iter)
			{
				iCheckForSelf = 0;
			}
			_DMESSAGE("Wireless processchanged calls RadiatorFixProcessPowerOnOffEvent FALSE>>%x", CurrentObject->formID);
			RadiatorFixProcessPowerOnOffEvent(false, CurrentObject, WorkshopRef, WorkshopData);
		}
	}
}

bool ProcessWirelessReferenceEvent(TESObjectREFR* WorkshopRef, TESObjectREFR* SourceRef)
{
	if (!bWirelessPowerFix || !WorkshopRef || !SourceRef || !g_RadiationActorValue)
	{
		return false;
	}

	Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(WorkshopRef);
	if (!WorkshopData)
	{
		return false;
	}

	const bool powered = sup::compat::IsPowered(WorkshopData, SourceRef);
	RadiatorFixProcessPowerOnOffEvent(powered, SourceRef, WorkshopRef, WorkshopData);

	if (OnWorkshopPowerStateChangeEventHandler.HasEvents())
	{
		PowerGridChanges changes;
		changes.bChanged = true;
		OnWorkshopPowerStateChangeEventHandler.ProcessEventOnWorkshopPowerStateChange(SourceRef, WorkshopRef, changes, 3, nullptr, nullptr, nullptr);
	}

	return true;
}

bool  cmd_WorkshopGenerateFakeReferenceEvent(StaticFunctionTag* base, TESObjectREFR* WorkshopRef, TESObjectREFR* SourceRef)
{
	_DMESSAGE("GenerateFakeWorkshopHandleReferenceEvent starts");
	if (!WorkshopRef)
	{
		_DMESSAGE("Workshop not valid");
		return false;
	}

	if (!SourceRef)
	{
		_DMESSAGE("SourceRef not valid");
		return false;
	}

	// Guard: only process if the source ref is still alive and valid.
	// During device reset/deletion, NISTRON may call this with a ref being removed.
	if (SourceRef->IsDeleted() || SourceRef->IsDisabled())
	{
		_DMESSAGE("GenerateFakeEvent: SourceRef deleted/disabled, skipping");
		return false;
	}

	return ProcessWirelessReferenceEvent(WorkshopRef, SourceRef);
}







bool  cmd_RadiatorEstablishConnections(StaticFunctionTag* base, TESObjectREFR* WorkshopRef, TESObjectREFR* SourceRef, bool bPowerOn)
{
	_DMESSAGE("RadiatorEstablishConnections>>%d",bPowerOn);
	if (!WorkshopRef)
	{
		_DMESSAGE("WOrkshop not valid");
		return false;
	}

	if (!SourceRef)
	{
		_DMESSAGE("SourceRef not valid");
		return false;
	}


	Workshop::ExtraData* WorkshopExtraData = f_GetPowerGridExtraData(WorkshopRef);

	if (!WorkshopExtraData)
	{
		_DMESSAGE("No workshop extra data");
		return false;
	}

	_DMESSAGE("RadiatorEstablishConnections calls RadiatorFixProcessPowerOnOffEven");
	RadiatorFixProcessPowerOnOffEvent(bPowerOn, SourceRef, WorkshopRef, WorkshopExtraData);
	return true;
}







bool cmd_WorkshopCleanUpEmptyPowerGrids(StaticFunctionTag* base, TESObjectREFR* WorkshopRef)
{
	_DMESSAGE("SUP::WorkshopCleanUpEmptyPowerGrids starts");


	if (!WorkshopRef)
	{
		_DMESSAGE("Workshop not valid");
		return false;
	}

	Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(WorkshopRef);

	if (!WorkshopData)
	{
		_DMESSAGE("No workshop extra data");
		return false;
	}

	_DMESSAGE("ALL valid - calling");
	sup::compat::CleanUpEmptyPowerGrids(WorkshopData);
	return true;
}






bool ReestablishTerminalLinks(TESObjectREFR* refTerminal)
{
	TESObjectREFR* workshopRef = GetLinkedRef_Native(refTerminal, g_SUPWorkshopKeyword);

	if (!workshopRef)
	{
		_DMESSAGE("No workshop ref");
		return false;
	}

	Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(workshopRef);

	if (!WorkshopData)
	{
		_DMESSAGE("No workshop extra data");
		return false;
	}


	for (UInt64 i = 0; i < WorkshopData->powerGrid.size(); i++)
	{
		PowerUtils::PowerGrid* pGrid = NULL;
		sup::compat::GetNthItem(WorkshopData->powerGrid, i, pGrid);

		if (!pGrid)
		{
			continue;
		}
		auto FoundItem = pGrid->adjacencyMap.find(refTerminal->formID);

		if (FoundItem != pGrid->adjacencyMap.end())
		{
			for (auto it = pGrid->adjacencyMap.begin(); it != pGrid->adjacencyMap.end(); it++)
			{
				if (Iter.first == refTerminal->formID || !Iter.first)
				{
					continue;
				}

				auto CurrentObject = (TESObjectREFR*)LookupFormByID(Iter.first);


				if (!CurrentObject)
				{
					continue;
				}

				auto MyKeyword = GetTerminalLinkKeyword_Hook(CurrentObject);


				if (MyKeyword)
				{
					SetLinkedRef_Native(CurrentObject, refTerminal, MyKeyword); //
				}

			}
			break;
		}


	}

	return false;
}


bool cmd_ReestablishTerminalLinks(StaticFunctionTag* tag, TESObjectREFR* REF)
{
	_DMESSAGE("ReestablishTerminalLinks started");

	if (!REF)
	{
		_DMESSAGE("Ref is not valid");
		return false;
	}


	if (REF->GetObjectReference() && REF->GetObjectReference()->formType != kFormType_TERM)
	{
		_DMESSAGE("Ref is not terminal");
		return false;
	}

	ReestablishTerminalLinks(REF);
	return true;
}









VMArray<TESObjectREFR*> cmd_FilterObjectRefArrayBySharedPowergrid(StaticFunctionTag* base, VMArray<TESObjectREFR*> refsToFilter, TESObjectREFR* WorkshopRef, UInt32 PowerGridID)
{
	_DMESSAGE("FilterObjectRefArrayBySharedPowergrid starts");
	VMArray<TESObjectREFR*> result;

	if (!WorkshopRef)
	{
		_DMESSAGE("Workshop not valid");
		return result;
	}

	Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(WorkshopRef);

	if (!WorkshopData)
	{
		_DMESSAGE("No workshop extra data");
		return result;
	}

	if (!refsToFilter.Length())
	{
		_DMESSAGE("refsToFilter is empty");
		return result;
	}



	for (UInt64 i = 0; i < WorkshopData->powerGrid.size(); i++)
	{
		PowerUtils::PowerGrid* pGrid = NULL;
		sup::compat::GetNthItem(WorkshopData->powerGrid, i, pGrid);
		if (pGrid && i == PowerGridID)
		{
			for (UInt32 i = 0; i < refsToFilter.Length(); i++)
			{
				bool bFound = false;
				TESObjectREFR* CurrentRef;
				refsToFilter.Get(&CurrentRef, i);
				if (CurrentRef)
				{
					auto FoundItem = pGrid->adjacencyMap.find(CurrentRef->formID);

					if (FoundItem != pGrid->adjacencyMap.end())
					{
						result.Push(&CurrentRef);
					}
				}
			}
		}
	}


	_DMESSAGE("FilterObjectRefArrayBySharedPowergrid end");
	return result;
}








VMArray<TESObjectREFR*> cmd_GetDirectGridConnectionsRefs(StaticFunctionTag* base, TESObjectREFR* WorkshopRef, TESObjectREFR* TempRefA)
{
	_DMESSAGE("SUP::GetDirectGridConnections starts");
	VMArray<TESObjectREFR*> result;

	if (!TempRefA)
	{
		_DMESSAGE("TempRefA not valid");
		return result;
	}

	if (!WorkshopRef)
	{
		_DMESSAGE("Workshop not valid");
		return result;
	}

	Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(WorkshopRef);

	if (!WorkshopData)
	{
		_DMESSAGE("GDPG::No workshop extra data");
		return result;
	}


	for (UInt64 i = 0; i < WorkshopData->powerGrid.size(); i++)
	{
		PowerUtils::PowerGrid* pGrid = NULL;
		sup::compat::GetNthItem(WorkshopData->powerGrid, i, pGrid);

		if (pGrid)
		{
			auto FoundItem = pGrid->adjacencyMap.find(TempRefA->formID);

			if (FoundItem != pGrid->adjacencyMap.end())
			{
				//_DMESSAGE("Found item in adjacency map");
				if (TempRefA->formID == FoundHashItem.first)
				{
					//_DMESSAGE("Checking>>%x", FoundHashItem.first);
					for (auto it2 = FoundHashItem.second->begin(); it2 != FoundHashItem.second->end(); it2++)
					{
						auto ConnectionRef = (TESObjectREFR*)LookupFormByID(Iter2.connection);

						if (ConnectionRef)
						{
							result.Push(&ConnectionRef);
						}
						
					}

					return result;
				}

			}
			else {
				_DMESSAGE("GDPG::Did not found item in adj map - continuing.");
				continue;
			}
		}
	}

	_DMESSAGE("SUP::GetDirectGridConnections end");
	return result;
}











//for (UInt64 i = 0; i < WorkshopData->powerGrid.size(); i++)
//{
//	PowerUtils::PowerGrid* pGrid;
//	sup::compat::GetNthItem(WorkshopData->powerGrid, i, pGrid);
//
//	if (pGrid)
//	{
//		//auto FoundItem = pGrid->adjacencyMap.find(TempRefA->formID);
//
//		//if (FoundItem != pGrid->adjacencyMap.end())
//		//{
//
//
//
//		//}
//
//
//		//(*MyRes).
//
//		_DMESSAGE("Scanning adjacency map of new power grid>>%d", i);
//		for (auto it = pGrid->adjacencyMap.begin(); it != pGrid->adjacencyMap.end(); it++)
//		{
//			_DMESSAGE("New adjacency map iteration");
//			if (TempRefA->formID == Iter.first)
//			{
//				_DMESSAGE("Checking>>%x", Iter.first);
//				auto TargetRef = (TESObjectREFR*)LookupFormByID(Iter.first);
//
//				if (!TargetRef)
//				{
//					_DMESSAGE("Cannot extract target reference");
//					return result;
//				}
//				_DMESSAGE("TargetRef found");
//
//
//				for (auto it2 = Iter.second->begin(); it2 != Iter.second->end(); it2++)
//				{
//					GridConnection NewConnection;
//					NewConnection.Set<bool>("IsSnappedConnection", false);
//					NewConnection.Set<TESObjectREFR*>("connector", nullptr);
//
//
//					_DMESSAGE("Checking new connection(%x)-connector pair(%x)", Iter2.connection, Iter2.connector);
//					auto ConnectionRef = (TESObjectREFR*)LookupFormByID(Iter2.connection);
//
//					if (!ConnectionRef)
//					{
//						_DMESSAGE("Cannot extract connection reference")
//							continue;
//					}
//					NewConnection.Set<TESObjectREFR*>("connection", ConnectionRef);
//					_DMESSAGE("Setting connection>>>%x", ConnectionRef->formID);
//
//
//
//					if (Iter2.connector)
//					{
//						auto ConnectorRef = (TESObjectREFR*)LookupFormByID(Iter2.connector);
//
//						if (ConnectorRef)
//						{
//							NewConnection.Set<TESObjectREFR*>("connector", ConnectorRef);
//							_DMESSAGE("Setting connector>>>%x", ConnectorRef->formID);
//						}
//					}
//
//					auto ConnectPoints = fn_GetConnectPoints(TargetRef);
//
//					for (auto it3 = ConnectPoints.begin(); it3 != ConnectPoints.end(); it3++)
//					{
//						_DMESSAGE("Comparing connect point>>%x", Iter3->formID);
//						if (Iter3->formID == ConnectionRef->formID)
//						{
//							_DMESSAGE("Setting snap connection");
//							NewConnection.Set<bool>("IsSnappedConnection", true);
//							break;
//						}
//
//					}
//
//
//					result.Push(&NewConnection);
//				}
//
//
//				return result;
//			}
//
//		}
//
//	}
//}




//int HasWorkshopChanged(WorkshopStatus& WorkshopStatusBefore, Workshop::ExtraData* WorkshopData) {
//
//
//	int Result = 0;
//
//	if (WorkshopData->powerGrid.size() != WorkshopStatusBefore.PowerGrids.size())
//	{
//		_DMESSAGE("Power grid count changed");
//		return 1;
//	}
//
//	if (WorkshopData->powerRating != WorkshopStatusBefore.PowerRating)
//	{
//		_DMESSAGE("Power rating changed, old>>%d,new>>%d", WorkshopStatusBefore.PowerRating, WorkshopData->powerRating);
//		return 2;
//	}
//
//	bool bTrue = false;
//
//	for (UInt64 i = 0; i < WorkshopData->powerGrid.size(); i++)
//	{
//		PowerUtils::PowerGrid* pGrid = NULL;
//		sup::compat::GetNthItem(WorkshopData->powerGrid, i, pGrid);
//
//		if (!pGrid)
//		{
//			continue;
//		}
//
//
//
//		if (pGrid->capacity != WorkshopStatusBefore.PowerGrids.at(i).capacity)
//		{
//			_DMESSAGE("Power grid#%d has capacity changed, old capacity>>%f, new capacity>>%f", i, WorkshopStatusBefore.PowerGrids.at(i).capacity, pGrid->capacity);
//			return 3;
//		}
//
//		if (pGrid->load != WorkshopStatusBefore.PowerGrids.at(i).load)
//		{
//			_DMESSAGE("Power grid#%d has load changed, old load>>%f, new load>>%f", i, WorkshopStatusBefore.PowerGrids.at(i).load, pGrid->load);
//			return 4;
//		}
//
//		if (pGrid->currentlyPowered.size() != WorkshopStatusBefore.PowerGrids.at(i).iPoweredItemsCount)
//		{
//			_DMESSAGE("Power grid#%d has currentlyPowered count changed, old load>>%d, new load>>%d", i, WorkshopStatusBefore.PowerGrids.at(i).iPoweredItemsCount, pGrid->currentlyPowered.size());
//			return 5;
//		}
//
//	}
//
//	return 0;
//}