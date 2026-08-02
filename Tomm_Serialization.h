
#define t_CosaveF4SEPluginID 'SUPP'
#define t_CosaveF4SERecordEvents 'SUPE'


#define t_CosaveMainMemberVer "v"
#define t_CosaveMainMemberEvents "e"
#define t_CosaveMainMemberMapScriptName "m"
#define t_CosaveMainMemberMapFunctionName "n"


#define t_CosaveEventMemberType "t"
#define t_CosaveEventMemberScriptName "s"
#define t_CosaveEventMemberFuncName "p"
#define t_CosaveEventMemberHandle "h"
#define t_CosaveEventMemberFilters "f"
#define t_CosaveEventMemberFlags "b"

#define t_CosaveEventFiltersList "l"
#define t_CosaveEventFiltersType "t"
#define t_CosaveEventFiltersNumber "n"
#define t_CosaveEventFiltersRunCount "c"




namespace SUPCosave {

	// int EventType
	//string s_ScriptName{};
	//string s_FunctionName{};
	//UInt64 ScriptHandle = 0;
	//Filters


	enum EventFlags
	{
		DontCheckOnLoad = 1,
	};


	SUPEvent::SUPEventFilters GetFiltersFromJSON(nlohmann::json& j)
	{
		_DMESSAGE("GetFiltersFromJSON start.");
		SUPEvent::SUPEventFilters result;

		if (j.contains(t_CosaveEventMemberFilters))
		{
			for (auto it = j[t_CosaveEventMemberFilters].begin(); it != j[t_CosaveEventMemberFilters].end(); it++)
			{
				
				int iType = Iter[t_CosaveEventFiltersType].get<int>();

				SUPEvent::SUPEventFilters::SUPSmartFilter NewFilter(Iter[t_CosaveEventFiltersNumber].get<int>(), iType);


				//Console_Print("Adding new event, type>>%d,Number>>%d", iType, Iter[t_CosaveEventFiltersNumber].get<int>());

				if (iType == SUPEvent::SUPEventFilters::k_FilterFormID)
				{
					for (auto it2 = Iter[t_CosaveEventFiltersList].begin(); it2 != Iter[t_CosaveEventFiltersList].end(); it2++)
					{
						std::get<vector<UInt32>>(NewFilter.List).push_back(Iter2.get<UInt32>());
					}
				}
				else if (iType == SUPEvent::SUPEventFilters::k_FilterFloat)
				{
					for (auto it2 = Iter[t_CosaveEventFiltersList].begin(); it2 != Iter[t_CosaveEventFiltersList].end(); it2++)
					{
						std::get<vector<float>>(NewFilter.List).push_back(Iter2.get<float>());
					}
				}
				else if (iType == SUPEvent::SUPEventFilters::k_FilterString)
				{
					for (auto it2 = Iter[t_CosaveEventFiltersList].begin(); it2 != Iter[t_CosaveEventFiltersList].end(); it2++)
					{
						//Console_Print("ADDING FILTER STRING FROM COSAVE>>%s", Iter2.get<string>().c_str());
						std::get<vector<string>>(NewFilter.List).push_back(Iter2.get<string>());
					}
				}

				result.v_Filters.push_back(NewFilter);

			}

		}



		if (!result.v_Filters.size())
		{
			_DMESSAGE("Filters not added");
		}

		return result;
	}


	void RestoreEventTableFromJSON(char* NewChar, const F4SESerializationInterface* intfc) {

		_DMESSAGE("RestoreEventTableFromJSON starts");
		nlohmann::json j = nlohmann::json::parse(NewChar, 0, 0);


		delete[] NewChar;
		if (!j.is_structured())
		{
			_DMESSAGE("JSON is not structured");
			f_ShowTextBoxMessage("SUP F4SE COULD NOT LOAD FROM COSAVE FILE.Please report to author attaching Cosave file for this savegame. It is advised to try and load this savegame again in order to avoid issues with mods.");

			return;
		}

		if (!j.contains(t_CosaveMainMemberVer))
		{
			_MESSAGE("SUPCosave::Cosave doesn't contain v");
			return;
		}

		if (j.contains(t_CosaveMainMemberEvents))
		{
			_DMESSAGE("Event array present");

			vector<string> vScriptNameArray{};
			vScriptNameArray.reserve(j[t_CosaveMainMemberMapScriptName].size());
			for (auto it = j[t_CosaveMainMemberMapScriptName].begin(); it != j[t_CosaveMainMemberMapScriptName].end(); it++)
			{
				vScriptNameArray.push_back(Iter.get<string>());
			}


			vector<string> vFunctionNameArray{};
			vFunctionNameArray.reserve(j[t_CosaveMainMemberMapFunctionName].size());
			for (auto it = j[t_CosaveMainMemberMapFunctionName].begin(); it != j[t_CosaveMainMemberMapFunctionName].end(); it++)
			{
				vFunctionNameArray.push_back(Iter.get<string>());
			}



			for (auto it = j[t_CosaveMainMemberEvents].begin(); it != j[t_CosaveMainMemberEvents].end(); it++)
			{
				_DMESSAGE("New event");

				UInt64 ScriptHandle = Iter[t_CosaveEventMemberHandle].get<UInt64>();
				UInt64 newHandle = 0;

				if (!intfc->ResolveHandle(ScriptHandle, &newHandle))
				{
					_DMESSAGE("Cannot resolve handle, old handle>>>%x", ScriptHandle);
					continue;
				}


				auto EventHandler = SUPEventHandlers::FindSUPEventHandlerByType(Iter[t_CosaveEventMemberType].get<int>());

				if (!EventHandler)
				{
					_DMESSAGE("No event exists with type of>>%d", Iter[t_CosaveEventMemberType].get<int>());
					continue;
				}


				int iPos = Iter[t_CosaveEventMemberScriptName].get<int>();

				if (!Iter.contains(t_CosaveEventMemberFlags))
				{
					auto MyForm = LookupFormByID(newHandle);

					if (!MyForm)
					{
						_DMESSAGE("Form for script %s is not found for object>>%x(OLD), %x(NEW) ", vScriptNameArray.at(iPos).c_str(), ScriptHandle, newHandle);
						//Console_Print("Form for script %s is not found for object>>%x(OLD), %x(NEW) ", vScriptNameArray.at(iPos).c_str(), ScriptHandle, newHandle);
						continue;
					}

					VMIdentifier* identifier = NULL;
					if (!g_MyVirtMachine->GetObjectIdentifier(newHandle, vScriptNameArray.at(iPos).c_str(), 0, &identifier, 0))
					{
						_DMESSAGE("Attached script %s is not found for object>>%x(OLD), %x(NEW) ", vScriptNameArray.at(iPos).c_str(), ScriptHandle, newHandle);
						//Console_Print("Attached script %s is not found for object>>%x(OLD), %x(NEW) ", vScriptNameArray.at(iPos).c_str(), ScriptHandle, newHandle);
						continue;
					}


				}


				SUPEvent* NewEvent = new SUPEvent;
				NewEvent->s_ScriptName = vScriptNameArray.at(iPos);

				iPos = Iter[t_CosaveEventMemberFuncName].get<int>();
				NewEvent->s_FunctionName = vFunctionNameArray.at(iPos);

				NewEvent->ScriptHandle = newHandle;
				NewEvent->bPermanent = true;



				if (j.contains(t_CosaveEventFiltersRunCount))
				{
					NewEvent->iRunTimesCount = Iter[t_CosaveEventFiltersRunCount].get<int>();
				}




				if (EventHandler->vAllowedFilters.size())
				{
					NewEvent->CurrentFilters = GetFiltersFromJSON(Iter);
				}

				_DMESSAGE("Adding new event from cosave ScriptName>>%s, FunctionName>>%s,Form>>%x", NewEvent->s_ScriptName.c_str(), NewEvent->s_FunctionName.c_str(), NewEvent->ScriptHandle);
				EventHandler->PushBackNewEvent(NewEvent, true);

				//Console_Print("Added permanent event from cosave!");


				//_DMESSAGE("New event, handle is>>>%x", );

			}
		}

		//_DMESSAGE("VER>>%d", j[t_CosaveMainMemberVer].get<int>());  
		_DMESSAGE("RestoreEventTableFromJSON ends");

	}




	nlohmann::json PlaceCurrentEventTableToJSON(bool& bWrite)
	{
		nlohmann::json j;
		
		vector<string> vScriptNameArray{};
		vector<string> vFunctionNameArray{};


		for (auto it = v_EventHandlers.begin(); it != v_EventHandlers.end(); it++)
		{
			for (auto it2 = Iter->v_EventList.begin(); it2 != Iter->v_EventList.end(); it2++)
			{
				if (Iter2->bPermanent)
				{
					_DMESSAGE("WRITING PERMANENT EVENT TO COSAVE of type>>%d", Iter->EventID); 
					nlohmann::json jNewEvent;
					jNewEvent[t_CosaveEventMemberType] = Iter->EventID;


					


					if (Iter2->bDontCheckPermanentEventOnSaveLoad)
					{
						UInt32 flags = 0;
						flags |= DontCheckOnLoad;
						jNewEvent[t_CosaveEventMemberFlags] = flags;
					}


					int iCurrentPos = 0;

					bool bFound = false;
					for (auto it3 = vScriptNameArray.begin(); it3 != vScriptNameArray.end(); it3++)
					{
						if (Iter3 == Iter2->s_ScriptName)
						{
							bFound = true;
							iCurrentPos = it3 - vScriptNameArray.begin();
							break;
						}
					}

					if (!bFound)
					{


						vScriptNameArray.push_back(Iter2->s_ScriptName);
						iCurrentPos = vScriptNameArray.size() - 1;
					}

					jNewEvent[t_CosaveEventMemberScriptName] = iCurrentPos;



					iCurrentPos = 0;

					bFound = false;
					for (auto it3 = vFunctionNameArray.begin(); it3 != vFunctionNameArray.end(); it3++)
					{
						if (Iter3 == Iter2->s_FunctionName)
						{
							bFound = true;
							iCurrentPos = it3 - vFunctionNameArray.begin();
							break;
						}
					}

					if (!bFound)
					{
						vFunctionNameArray.push_back(Iter2->s_FunctionName);
						iCurrentPos = vFunctionNameArray.size() - 1;
					}


					jNewEvent[t_CosaveEventMemberFuncName] = iCurrentPos;




					jNewEvent[t_CosaveEventMemberHandle] = Iter2->ScriptHandle;

					if (Iter2->iRunTimesCount)
					{
						jNewEvent[t_CosaveEventFiltersRunCount] = Iter2->iRunTimesCount;
					}

					//jNewEvent[t_CosaveEventMemberFilters] = nlohmann::json::array();


					for (auto it3 = Iter2->CurrentFilters.v_Filters.begin(); it3 != Iter2->CurrentFilters.v_Filters.end(); it3++)
					{

						nlohmann::json jNewFilter;
						jNewFilter[t_CosaveEventFiltersType] = Iter3.type;
						jNewFilter[t_CosaveEventFiltersNumber] = Iter3.Number;





						if (Iter3.type == SUPEvent::SUPEventFilters::k_FilterFormID)
						{
							for (auto it4 = std::get<vector<UInt32>>(Iter3.List).begin(); it4 != std::get<vector<UInt32>>(Iter3.List).end(); it4++) {
								jNewFilter[t_CosaveEventFiltersList].push_back(Iter4);
							}
						}
						else if (Iter3.type == SUPEvent::SUPEventFilters::k_FilterFloat)
						{
							for (auto it4 = std::get<vector<float>>(Iter3.List).begin(); it4 != std::get<vector<float>>(Iter3.List).end(); it4++) {
								jNewFilter[t_CosaveEventFiltersList].push_back(Iter4);
							}
						}
						else if (Iter3.type == SUPEvent::SUPEventFilters::k_FilterString) {
							for (auto it4 = std::get<vector<string>>(Iter3.List).begin(); it4 != std::get<vector<string>>(Iter3.List).end(); it4++) {
								jNewFilter[t_CosaveEventFiltersList].push_back(Iter4.c_str());
							}
						}
						
						jNewEvent[t_CosaveEventMemberFilters].push_back(jNewFilter);

					}


					j[t_CosaveMainMemberEvents].push_back(jNewEvent);
					bWrite = true;
				}
			}
		}

		if (!bWrite) {
			return j;
		}



		for (auto it = vScriptNameArray.begin(); it != vScriptNameArray.end(); it++)
		{
			j[t_CosaveMainMemberMapScriptName].push_back(Iter);
		}

		for (auto it = vFunctionNameArray.begin(); it != vFunctionNameArray.end(); it++)
		{
			j[t_CosaveMainMemberMapFunctionName].push_back(Iter);
		}

		j[t_CosaveMainMemberVer] = iVersion;



		return j;
	}









	void SaveCallback(const F4SESerializationInterface* intfc)
	{
		bool bWrite = false;

		string sTest = PlaceCurrentEventTableToJSON(bWrite).dump();
		if (bWrite)
		{
			intfc->WriteRecord(t_CosaveF4SERecordEvents, 1, sTest.c_str(), sTest.size());
		}

	}

	void LoadCallback(const F4SESerializationInterface* intfc)
	{
		EventRunner::RunOnSaveLoad();
		char* NewChar = NULL;

		UInt32 type;
		UInt32 version;
		UInt32 length;
		while (intfc->GetNextRecordInfo(&type, &version, &length)) {

			switch (type) {
			case t_CosaveF4SERecordEvents:

				NewChar = new char[length + 1];

				_DMESSAGE("length is>>%d", length)

				if (!intfc->ReadRecordData(NewChar, length)) {
					_DMESSAGE("Failed to load string!");
				}
				else {
					NewChar[length] = '\0'; //NULL TERMINATE THIS FUCKER
					RestoreEventTableFromJSON(NewChar, intfc);
				}

				break;

			default:
				Console_Print("SUP::Cosave::Unrecognized signature type??????????????????????????????");
				break;
			}


		}

	}

}
