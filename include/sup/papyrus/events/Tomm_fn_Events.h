#include "sup/util/string.hpp"



//for (auto it = g_EventScriptArrayOnGameLoop.begin(); it != g_EventScriptArrayOnGameLoop.end();)
//{
//	if (Iter->s_ScriptName == sScriptName && Iter->s_FunctionName == sFunctionName)
//	{
//		_DMESSAGE("Script and functions are already in MainGameLoopCallback");
//		s_GameLoopLock.Leave();
//		return -1;
//	}

//}

//union SUPEventParam
//{
//	vector<TESObjectREFR*> vFiltered;
//	vector<TESForm*> vFiltered;
//};


//bool ConvertVariable(VMValue* src, SUPEventParam* ParamToFill)
//{
//	Console_Print("Converting value");
//
//	switch (src->GetTypeEnum())
//	{
//	case VMValue::kType_String:
//	{
//		BSFixedString* str = src->data.GetStr();
//		Console_Print("Value is string");
//		return true;
//	}
//	break;
//	case VMValue::kType_Int:
//		//dest->SetInt(src->data.i);
//		Console_Print("Value is INT");
//		return true;
//	case VMValue::kType_Float:
//		Console_Print("Value is float");
//		return true;
//	case VMValue::kType_Bool:
//		//dest->SetBool(src->data.b);
//		Console_Print("Value is bool");
//		return true;
//	case VMValue::kType_Identifier:
//	{
//		VMObjectTypeInfo* objectType = static_cast<VMObjectTypeInfo*>(src->GetComplexType());
//		VMIdentifier* id = src->data.id;
//		if (objectType && id) {
//			UInt64 handle = id->GetHandle();
//			BSFixedString typeName = objectType->m_typeName;
//			Console_Print("Value is Inentifier, handle is>>%d, typename is %s", handle, objectType->m_typeName.c_str());
//
//			//root->CreateObject(dest);
//			//dest->SetMember("__handleHigh__", &GFxValue((UInt32)(handle >> 32)));
//			//dest->SetMember("__handleLow__", &GFxValue((UInt32)(handle & 0xFFFFFFFF)));
//
//			//GFxValue type;
//			//root->CreateString(&type, typeName.c_str());
//
//			//dest->SetMember("__type__", &type);
//			return true;
//		}
//	}
//	break;
//	case VMValue::kType_Struct:
//	{
//		Console_Print("Value is struct");
//		/*VMStructTypeInfo* structType = static_cast<VMStructTypeInfo*>(src->GetComplexType());
//		VMValue::StructData* structData = src->data.strct;
//		if (structType && structData) {
//			root->CreateObject(dest);
//			GFxValue gStructObject;
//			root->CreateObject(&gStructObject);
//			dest->SetMember("__struct__", &gStructObject);
//
//			GFxValue gStructName;
//			root->CreateString(&gStructName, structType->m_typeName.c_str());
//			gStructObject.SetMember("__type__", &gStructName);
//
//			GFxValue gStructPairs;
//			root->CreateObject(&gStructPairs);
//			gStructObject.SetMember("__data__", &gStructPairs);
//
//			structType->m_members.ForEach([&gStructPairs, &structData, &root](VMStructTypeInfo::MemberItem* item)
//				{
//					GFxValue value;
//			ConvertPapyrusValue(&value, &structData->GetStruct()[item->index], root);
//			gStructPairs.SetMember(item->name.c_str(), &value);
//			return true;
//				});
//			return true;
//		}*/
//	}
//	break;
//	case VMValue::kType_Variable:
//	{
//		Console_Print("Value is VAR");
//		//VMValue* source = src->data.var;
//		//if (source) {
//		//	root->CreateObject(dest);
//		//	GFxValue type;
//		//	ConvertPapyrusValue(&type, source, root);
//		//	dest->SetMember("__var__", &type);
//		//	return true;
//		//}
//	}
//	break;
//	case VMValue::kType_StringArray:
//	case VMValue::kType_IntArray:
//	case VMValue::kType_FloatArray:
//	case VMValue::kType_BoolArray:
//	case VMValue::kType_IdentifierArray:
//	case VMValue::kType_StructArray:
//	case VMValue::kType_VariableArray:
//	{
//
//		Console_Print("Value is Array");
//
//
//		VMValue::ArrayData* arrayData = src->data.arr;
//
//		for (UInt32 i = 0; i < arrayData->arr.count; i++) {
//			VMValue vmValue;
//			arrayData->arr.GetNthItem(i, vmValue);
//			ConvertVariable(&vmValue, NULL);
//		}
//	
//
//		//if (arrayData) {
//		//	root->CreateArray(dest);
//
//		//	for (UInt32 i = 0; i < arrayData->arr.count; i++) {
//		//		VMValue vmValue;
//		//		arrayData->arr.GetNthItem(i, vmValue);
//
//		//		GFxValue value;
//		//		ConvertPapyrusValue(&value, &vmValue, root);
//		//		dest->PushBack(&value);
//		//	}
//
//		//	return true;
//		//}
//	}
//	break;
//	}
//
//	return false;
//}









UInt32 cmd_RegisterForGameMainLoopCallback(StaticFunctionTag* base, TESForm* CurrentObject, BSFixedString ScriptName, BSFixedString FunctionName, bool bSet,  float CallInterval, UInt32 CallMode, bool bRemoveOnGameLoad, UInt32 iRunTimesCount, VMArray<VMVariable> arguments, bool PassCallIDForFirstParam)
{
	_DMESSAGE("RegisterForGameMainLoopCallback started, funcname>>>%s", FunctionName.c_str());

	if (!CurrentObject)
	{
		_DMESSAGE("Object is not valid");
		return -4;
	}



	std::string sScriptName = ScriptName.c_str();
	std::string sFunctionName = FunctionName.c_str();
	sup::string::to_lower(sScriptName);
	sup::string::to_lower(sFunctionName);


	auto HandlerResult = AttachedScriptReader::GetHandlerForForm(CurrentObject);

	if (!HandlerResult.bSuccess)
	{
		return -15;
	}

	VMIdentifier* identifier = NULL;

	if (!g_MyVirtMachine->GetObjectIdentifier(HandlerResult.handle, sScriptName.c_str(), 0, &identifier, 0))
	{
		_DMESSAGE("VM identifier NOT found");
		return -2;
	}

	if (bSet)
	{
		if (CallMode < 0 || CallMode >t_ScriptRunnerCallModeOnlyMenumode)
		{
			_MESSAGE("Wrong CallMode specified>>%d - please read tutorial.", CallMode);
			return -3;
		}




		s_GameLoopLock.Enter();

		SUPEventGameLoop* NewEventRunner = new SUPEventGameLoop;
		NewEventRunner->s_ScriptName = sScriptName;
		NewEventRunner->s_FunctionName = sFunctionName;
		NewEventRunner->fCallInterval = CallInterval * 1000;
		NewEventRunner->fCurTimer = NewEventRunner->fCallInterval;
		NewEventRunner->iCallMode = CallMode;
		//NewEventRunner->bFunctionIsLocal = true;

		if (iRunTimesCount < 0)
		{
			iRunTimesCount = 0;
		}

		NewEventRunner->iRunTimesCount = iRunTimesCount;
		NewEventRunner->ScriptHandle = HandlerResult.handle;
		NewEventRunner->bRemoveOnSaveLoad = bRemoveOnGameLoad;


		iGameLoopFunctionCount++;
		NewEventRunner->iID = iGameLoopFunctionCount;


		if (g_EventScriptArrayOnGameLoop.size() == 0) {
			g_TimeElapsed = g_timer.get_elapsed_ms();

		}

		g_EventScriptArrayOnGameLoop.push_back(NewEventRunner);


		VMValue argsToStore;


		if (PassCallIDForFirstParam)
		{
			UInt32 iIDToSet = NewEventRunner->iID;

			VMArray<VMVariable> argumentsToUSE;
			VMVariable var1;
			var1.Set<UInt32>(&iIDToSet);
			argumentsToUSE.Push(&var1);

			UInt32 argCount = arguments.Length();
			if (argCount > 0) {
				VMVariable result;
				result.Set<VMArray<VMVariable>>(&arguments);
				argumentsToUSE.Push(&result);
			}



			argumentsToUSE.PackArray(&argsToStore, g_MyVirtMachine);
			NewEventRunner->args = argsToStore;
			NewEventRunner->bHasArgs = true;
			_DMESSAGE("SettingID>>%d, Stored %d args", iIDToSet, 100);
		}
		else {

			UInt32 argCount = arguments.Length();
			if (argCount > 0) {
				
				VMVariable result;
				result.Set<VMArray<VMVariable>>(&arguments);
				VMArray<VMVariable> argumentsToUSE; 
				argumentsToUSE.Push(&result);

				argumentsToUSE.PackArray(&argsToStore, g_MyVirtMachine);
				NewEventRunner->args = argsToStore;
				NewEventRunner->bHasArgs = true;
				_DMESSAGE("Stored %d args", argCount);
			}

		}

		_DMESSAGE("Stored new function, Scriptname>> %s, Functionname>> %s, ID>>%d", NewEventRunner->s_ScriptName.c_str(), NewEventRunner->s_FunctionName.c_str(), NewEventRunner->iID);
		s_GameLoopLock.Leave();
		return NewEventRunner->iID;

	}
	else {

		bool bFound = false;

		s_GameLoopLock.Enter();
		for (auto it = g_EventScriptArrayOnGameLoop.begin(); it != g_EventScriptArrayOnGameLoop.end();)
		{
			if (Iter->ScriptHandle == HandlerResult.handle && Iter->s_ScriptName == sScriptName && Iter->s_FunctionName == sFunctionName)
			{
				_DMESSAGE("Removing scriptname>>%s and function>>%s from callback, handler>>%x", sScriptName.c_str(), sFunctionName.c_str(), HandlerResult.handle);

				delete Iter;
				it = g_EventScriptArrayOnGameLoop.erase(it);
				bFound = true;
				continue;
			}
			it++;
		}
		s_GameLoopLock.Leave();
		return bFound;

	}


}

bool cmd_RemoveFromGameMainLoopCallback(StaticFunctionTag* base, UInt32 iID)
{
	_DMESSAGE("RemoveFromGameMainLoopCallback started, ID >>>%d", iID);
	s_GameLoopLock.Enter();
	for (auto it = g_EventScriptArrayOnGameLoop.begin(); it != g_EventScriptArrayOnGameLoop.end(); it++)
	{
		if (Iter->iID == iID)
		{
			delete Iter;
			g_EventScriptArrayOnGameLoop.erase(it);
			s_GameLoopLock.Leave();
			_DMESSAGE("Callback removed");
			return true;
		}
	}
	s_GameLoopLock.Leave();
	return false;

}











//VMVariable var
//























//UInt32 cmd_SetEventHandler (StaticFunctionTag* base, BSFixedString EventName, BSFixedString ScriptName, BSFixedString FunctionName, bool bSet, bool bFunctionIsLocal, bool bRemoveOnGameLoad, UInt32 iRunTimesCount, VMArray<VMVariable> filters)
//{
//	_DMESSAGE("SetEventHandler started>>%s", EventName.c_str());
//	string sEventName = EventName;
//	sup::string::to_lower(sEventName);
//
//
//	SUPEventHandler* EventHandler = NULL;
//	for (auto it = v_EventHandlers.begin(); it != v_EventHandlers.end(); it++)
//	{
//		if (sEventName == Iter->EventName)
//		{
//			EventHandler = Iter;
//			break;
//		}
//	}
//
//	if (!EventHandler) {
//
//		_DMESSAGE("Cannot find specified event in the list>>%s", sEventName.c_str());
//		return -3;
//	}
//
//	string sScriptName = ScriptName;
//	sup::string::to_lower(sScriptName);
//
//
//
//	if (bSet)
//	{
//		auto it_MyScript = VMScripts.find(sScriptName);
//		if (it_MyScript == VMScripts.end())
//		{
//			return -2;
//		}
//
//
//		string sFunctionName = FunctionName;
//		sup::string::to_lower(sFunctionName);
//
//		SUPEvent* NewEvent = new SUPEvent;
//		NewEvent->bFunctionIsLocal = bFunctionIsLocal;
//		NewEvent->bRemoveOnSaveLoad = bRemoveOnGameLoad;
//
//		if (iRunTimesCount <0)
//		{
//			iRunTimesCount = 0;
//		}
//
//		NewEvent->iRunTimesCount = iRunTimesCount;
//
//
//		NewEvent->s_ScriptName = sScriptName;
//		NewEvent->s_FunctionName = sFunctionName;
//		NewEvent->ScriptHandle = (*it_MyScript).second;
//
//		if (EventHandler->vAllowedFilters.size())
//		{
//			NewEvent->CurrentFilters = f_GetFiltersFromFiltersArray(filters, EventHandler);
//		}
//
//		return EventHandler->PushBackNewEvent(NewEvent);
//	}
//	else {
//		string sFunctionName = FunctionName;
//		sup::string::to_lower(sFunctionName);
//		return EventHandler->RemoveEvent(sScriptName, sFunctionName,0);
//	}
//}







UInt32 cmd_RegisterForSUPEvent(StaticFunctionTag* base, BSFixedString EventName, TESForm* CurrentObject, BSFixedString ScriptName, BSFixedString FunctionName, bool bSet, bool bPermanent, bool bRemoveOnGameLoad, UInt32 iRunTimesCount, VMArray<VMVariable> filters)
{
	_DMESSAGE("RegisterForSUPEvent started>>%s", EventName.c_str());

	if (!CurrentObject)
	{
		_DMESSAGE("Object is not valid");
		return -4;
	}

	string sEventName = EventName.c_str();
	sup::string::to_lower(sEventName);

	SUPEventHandler* EventHandler = NULL;
	for (auto it = v_EventHandlers.begin(); it != v_EventHandlers.end(); it++)
	{
		if (sEventName == Iter->EventName)
		{
			EventHandler = Iter;
			break;
		}
	}

	if (!EventHandler) {

		_DMESSAGE("Cannot find specified event in the list>>%s", sEventName.c_str());
		return -3;
	}

	string sScriptName = ScriptName.c_str();
	sup::string::to_lower(sScriptName);

	if (!f_ContainsOnlyASCII(sScriptName))
	{
		_DMESSAGE("Scriptname contains non ASCII chars");
		return -5;
	}
	string sFunctionName = FunctionName.c_str();
	sup::string::to_lower(sFunctionName);

	if (!f_ContainsOnlyASCII(sFunctionName))
	{
		_DMESSAGE("FunctionName contains non ASCII chars");
		return -5;
	}

	auto HandlerResult = AttachedScriptReader::GetHandlerForForm(CurrentObject);
	
	if (!HandlerResult.bSuccess)
	{
		return -15;
	}


	if (bSet)
	{

		VMIdentifier* identifier = NULL;

		if (!g_MyVirtMachine->GetObjectIdentifier(HandlerResult.handle, sScriptName.c_str(), 0, &identifier, 0))
		{
			_DMESSAGE("VM identifier NOT found");
			return -2;
		}

		SUPEvent::SUPEventFilters TempFilters;
		if (EventHandler->vAllowedFilters.size())
		{
			
			int iFilterInvalidNum = 0;
			TempFilters = SUPEventFilterHelper::GetFiltersFromFiltersArray(filters, EventHandler, iFilterInvalidNum);

			if (iFilterInvalidNum)
			{
				_DMESSAGE("Cannot extract from filters");
				return -6;
			}
		}


		SUPEvent* NewEvent = new SUPEvent;

		if (bPermanent)
		{
			NewEvent->bDontCheckPermanentEventOnSaveLoad = bRemoveOnGameLoad;
		}
		else {
			NewEvent->bRemoveOnSaveLoad = bRemoveOnGameLoad;
		}



		
		if (iRunTimesCount < 0)
		{
			iRunTimesCount = 0;
		}
		NewEvent->iRunTimesCount = iRunTimesCount;
		NewEvent->s_ScriptName = sScriptName;
		NewEvent->s_FunctionName = sFunctionName;
		NewEvent->ScriptHandle = HandlerResult.handle;
		NewEvent->bPermanent = bPermanent;
		NewEvent->CurrentFilters = std::move(TempFilters);
		//NewEvent->type = EventHandler->EventID;
		//NewEvent->


		return EventHandler->PushBackNewEvent(NewEvent, false);
	}
	else {
		string sFunctionName = FunctionName.c_str();
		sup::string::to_lower(sFunctionName);
		return EventHandler->RemoveEvent(sScriptName, sFunctionName, HandlerResult.handle, bPermanent);
	}
}



UInt32 cmd_SetEventHandler(StaticFunctionTag* base, BSFixedString EventName, BSFixedString ScriptName, BSFixedString FunctionName, bool bSet, bool bFunctionIsLocal, bool bRemoveOnGameLoad, UInt32 iRunTimesCount, VMArray<VMVariable> filters)
{
	f_ShowTextBoxMessage("You see this messages because one of the mods dependent on SUP F4SE plugin has called a function which is currently removed from the plugin(SetEventHandler). Please update this mod.");
	return -1;
}







bool cmd_IsSUPEventRegistered(StaticFunctionTag* base, BSFixedString EventName, TESForm* CurrentObject, BSFixedString ScriptName, BSFixedString FunctionName, bool bPermanent)
{
	_DMESSAGE("IsSUPEventRegistered>>%s", EventName.c_str());

	if (!CurrentObject)
	{
		_DMESSAGE("Object is not valid");
		return false;
	}

	string sEventName = EventName.c_str();
	sup::string::to_lower(sEventName);

	SUPEventHandler* EventHandler = NULL;
	for (auto it = v_EventHandlers.begin(); it != v_EventHandlers.end(); it++)
	{
		if (sEventName == Iter->EventName)
		{
			EventHandler = Iter;
			break;
		}
	}

	if (!EventHandler) {

		_DMESSAGE("Cannot find specified event in the list>>%s", sEventName.c_str());
		return false;
	}

	string sScriptName = ScriptName.c_str();
	sup::string::to_lower(sScriptName);

	if (!f_ContainsOnlyASCII(sScriptName))
	{
		_DMESSAGE("Scriptname contains non ASCII chars");
		return false;
	}
	string sFunctionName = FunctionName.c_str();
	sup::string::to_lower(sFunctionName);

	if (!f_ContainsOnlyASCII(sFunctionName))
	{
		_DMESSAGE("FunctionName contains non ASCII chars");
		return false;
	}

	auto HandlerResult = AttachedScriptReader::GetHandlerForForm(CurrentObject);

	if (!HandlerResult.bSuccess)
	{
		return false;
	}


	EventHandler->EnterLock();
	bool bResult = EventHandler->IsEventHandlerInList(sScriptName, sFunctionName, HandlerResult.handle, bPermanent);
	EventHandler->LeaveLock();
	return bResult;
}






UInt32 cmd_UnregisterForAllSUPEvents(StaticFunctionTag* base, BSFixedString EventName, TESForm* CurrentObject, bool bPermanent, BSFixedString ScriptName, BSFixedString FunctionName)
{
	_DMESSAGE("UnRegisterForSUPEventstarted>>%s", EventName.c_str());

	if (!CurrentObject)
	{
		_DMESSAGE("Object is not valid");
		return -2;
	}

	string sEventName = EventName.c_str();
	sup::string::to_lower(sEventName);

	SUPEventHandler* EventHandler = NULL;
	for (auto it = v_EventHandlers.begin(); it != v_EventHandlers.end(); it++)
	{
		if (sEventName == Iter->EventName)
		{
			EventHandler = Iter;
			break;
		}
	}

	if (!EventHandler) {

		_DMESSAGE("Cannot find specified event in the list>>%s", sEventName.c_str());
		return -1;
	}


	auto HandlerResult = AttachedScriptReader::GetHandlerForForm(CurrentObject);

	if (!HandlerResult.bSuccess)
	{
		return -15;
	}


	string sScriptName = ScriptName.c_str();
	sup::string::to_lower(sScriptName);

	if (!f_ContainsOnlyASCII(sScriptName))
	{
		_DMESSAGE("Scriptname contains non ASCII chars");
		return -3;
	}
	string sFunctionName = FunctionName.c_str();
	sup::string::to_lower(sFunctionName);

	if (!f_ContainsOnlyASCII(sFunctionName))
	{
		_DMESSAGE("FunctionName contains non ASCII chars");
		return -3;
	}

	if (!sScriptName.length())
	{
		_DMESSAGE("ScriptName is empty");
		return -4;
	}



	if (sFunctionName.length()) // Remove only by ScriptName
	{
		return EventHandler->RemoveAllEventByScriptNameAndFunction(sScriptName, sFunctionName, HandlerResult.handle, bPermanent);
	}
	else {
		return EventHandler->RemoveAllEventByScriptName(sScriptName, HandlerResult.handle, bPermanent);
	}
}






//if (!arguments.IsNone())
//{
//	f_ProcessVarArray(arguments);
//}
//else {
//	Console_Print("Var aray is none");
//}


