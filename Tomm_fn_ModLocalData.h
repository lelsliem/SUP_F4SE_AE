




class ModLocalData
{
public:

	enum ElementTypes
	{
		kType_None = 0,
		kType_Float,
		kType_Int,
		kType_Ref,
		kType_String,
		kType_RefArray,
	};

	class ArrayElement
	{
	public:

		string Name{};
		int Type = -1;
		string Val_String;
		TESObjectREFR* Val_Ref = NULL;
		int Val_int;
		float Val_float;
		vector<TESObjectREFR*> RefArray;

		void CleanValue()
		{
			Val_String.clear();
			Val_Ref = NULL;
			Val_int = 0;
			Val_float = 0;
			RefArray.clear();
		}

	};


	string ModName{};
	vector<ArrayElement*> Elements;



};

vector<ModLocalData*> g_MLDArray;
#define MLDElement ModLocalData::ArrayElement
#define MLDElementType ModLocalData::ElementTypes

int f_MLD_GetModIndex(string Modname)
{
	for (auto it = g_MLDArray.begin(); it != g_MLDArray.end(); ++it)
	{
		if (Iter->ModName == Modname)
		{
			return it - g_MLDArray.begin();
		}
	}

	return -1;
}


int f_MLD_GetElementIndex(int iModDataIndex, string Key)
{

	for (auto it = g_MLDArray[iModDataIndex]->Elements.begin(); it != g_MLDArray[iModDataIndex]->Elements.end();++it)
	{
		if (Iter->Name == Key)
			return it - g_MLDArray[iModDataIndex]->Elements.begin();
	}

	return -1;
}

int f_MLD_GetElementType(int iModDataIndex, int iElementIndex)
{
	return g_MLDArray[iModDataIndex]->Elements[iElementIndex]->Type;
}

bool f_MLD_RemoveKey(int iModDataIndex, int iElementIndex)
{
	delete g_MLDArray[iModDataIndex]->Elements[iElementIndex];
	g_MLDArray[iModDataIndex]->Elements.erase(g_MLDArray[iModDataIndex]->Elements.begin() +iElementIndex);
	return true;
}


bool f_MLD_RemoveAllKeys(int iModDataIndex)
{
	for (auto it = g_MLDArray[iModDataIndex]->Elements.begin(); it != g_MLDArray[iModDataIndex]->Elements.end(); ++it)
	{
		delete Iter;
	}

	g_MLDArray[iModDataIndex]->Elements.clear();

	delete g_MLDArray[iModDataIndex];


	g_MLDArray.erase(g_MLDArray.begin() + iModDataIndex);
	return true;
}

void f_MLD_DumpToConsole(int iModDataIndex)
{

	Console_Print("Dumping ModLocalData for>>>%s<<< STARTS", g_MLDArray[iModDataIndex]->ModName.c_str());
	Console_Print("Elements::");

	for (auto it = g_MLDArray[iModDataIndex]->Elements.begin(); it != g_MLDArray[iModDataIndex]->Elements.end(); ++it)
	{
		if (Iter->Type == MLDElementType::kType_Float)
		{
			Console_Print("--->>%s<<--, type is FLOAT, value is %f ", Iter->Name.c_str(), Iter->Val_float);
		}
		else if (Iter->Type == MLDElementType::kType_Int)
		{
			Console_Print("--->>%s<<--, type is INT, value is %d ", Iter->Name.c_str(), Iter->Val_int);
		}
		else if (Iter->Type == MLDElementType::kType_Ref)
		{
			if (f_IsFormValid(Iter->Val_Ref))
			{
				//if (Iter->Val_Ref->GetFullName())
				//{
					Console_Print("--->>%s<<--, type is REFERENCE, reference is %X ", Iter->Name.c_str(), Iter->Val_Ref->formID);
				//}
				//else {
					//Console_Print("--->>%s<<--, type is REFERENCE, Name cannot be detected", Iter->Name);
				//}
			}
			else {
				Console_Print("--->>%s<<--, type is REFERENCE, NOT VALID", Iter->Name.c_str());
			}
		}else if (Iter->Type == MLDElementType::kType_String)
		{
			Console_Print("--->>%s<<--, type is STRING, value is %s ", Iter->Name.c_str(), Iter->Val_String);
		}
		else if (Iter->Type == MLDElementType::kType_RefArray)
		{
			Console_Print("--->>%s<<--, type is Ref Array", Iter->Name.c_str());

			for (auto it2 = Iter->RefArray.begin(); it2 != Iter->RefArray.end();++it2)
			{
				Console_Print("Key is %d, reference is %X",it2 - Iter->RefArray.begin(), (*it2)->formID);
			}
		}
	}
	Console_Print("Dumping ModLocalData for>>>%s<<< ENDS", g_MLDArray[iModDataIndex]->ModName.c_str());
}




int f_MLD_CreateModData(string name)
{
	int iIndex = g_MLDArray.size();
	ModLocalData* MLDToAdd = new ModLocalData;
	MLDToAdd->ModName = name;
	g_MLDArray.push_back(MLDToAdd);
	return iIndex;
}

void f_MLD_CreateNewElementFloat(int iModDataIndex, string Key, float Val)
{
	MLDElement* Element = new MLDElement;
	Element->Type = MLDElementType::kType_Float;
	Element->Name = Key;
	Element->Val_float = Val;
	g_MLDArray[iModDataIndex]->Elements.push_back(Element);
}

void f_MLD_CreateNewElementInt(int iModDataIndex, string Key, int Val)
{
	MLDElement* Element = new MLDElement;
	Element->Type = MLDElementType::kType_Int;
	Element->Name = Key;
	Element->Val_int = Val;
	g_MLDArray[iModDataIndex]->Elements.push_back(Element);
}

void f_MLD_CreateNewElementRef(int iModDataIndex, string Key, TESObjectREFR* Val)
{
	MLDElement* Element = new MLDElement;
	Element->Type = MLDElementType::kType_Ref;
	Element->Name = Key;
	Element->Val_Ref = Val;
	g_MLDArray[iModDataIndex]->Elements.push_back(Element);
}

void f_MLD_CreateNewElementString(int iModDataIndex, string Key, string Val)
{
	MLDElement* Element = new MLDElement;
	Element->Type = MLDElementType::kType_String;
	Element->Name = Key;
	Element->Val_String = Val;
	g_MLDArray[iModDataIndex]->Elements.push_back(Element);
}



void f_MLD_ReplaceElementFloat(int iModDataIndex, int iElementIndex, float Val)
{
	MLDElement* Element = g_MLDArray[iModDataIndex]->Elements[iElementIndex];
	Element->CleanValue();
	Element->Val_float = Val;
	Element->Type = MLDElementType::kType_Float;
}


void f_MLD_ReplaceElementInt(int iModDataIndex, int iElementIndex, int Val)
{
	MLDElement* Element = g_MLDArray[iModDataIndex]->Elements[iElementIndex];
	Element->CleanValue();
	Element->Val_int = Val;
	Element->Type = MLDElementType::kType_Int;
}

void f_MLD_ReplaceElementRef(int iModDataIndex, int iElementIndex, TESObjectREFR* Val)
{
	MLDElement* Element = g_MLDArray[iModDataIndex]->Elements[iElementIndex];
	Element->CleanValue();
	Element->Val_Ref = Val;
	Element->Type = MLDElementType::kType_Ref;
}

void f_MLD_ReplaceElementString(int iModDataIndex, int iElementIndex, string Val)
{
	MLDElement* Element = g_MLDArray[iModDataIndex]->Elements[iElementIndex];
	Element->CleanValue();
	Element->Val_String = Val;
	Element->Type = MLDElementType::kType_String;
}




float f_MLD_GetElementFloat(int iModDataIndex, int iElementIndex)
{
	return g_MLDArray[iModDataIndex]->Elements[iElementIndex]->Val_float;
}

int f_MLD_GetElementInt(int iModDataIndex, int iElementIndex)
{
	return g_MLDArray[iModDataIndex]->Elements[iElementIndex]->Val_int;
}

TESObjectREFR* f_MLD_GetElementRef(int iModDataIndex, int iElementIndex)
{
	return g_MLDArray[iModDataIndex]->Elements[iElementIndex]->Val_Ref;
}

string f_MLD_GetElementString(int iModDataIndex, int iElementIndex)
{
	return g_MLDArray[iModDataIndex]->Elements[iElementIndex]->Val_String;
}


bool cmd_ModLocalDataExists(StaticFunctionTag* base, BSFixedString ModNameIN, BSFixedString KeyIN)
{
	string ModName = ModNameIN;
	string Key = KeyIN;
	int iModIndex = -1;
	int iCreated = 0;

	iModIndex = f_MLD_GetModIndex(ModName);
	if (iModIndex > -1)
	{
		int iElementIndex = f_MLD_GetElementIndex(iModIndex, Key);

		if (iElementIndex > -1)
		{
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

UInt32 cmd_ModLocalDataGetType(StaticFunctionTag* base, BSFixedString ModNameIN, BSFixedString KeyIN)
{
	string ModName = ModNameIN;
	string Key = KeyIN;
	int iModIndex = -1;
	int iCreated = 0;

	iModIndex = f_MLD_GetModIndex(ModName);
	if (iModIndex > -1)
	{
		int iElementIndex = f_MLD_GetElementIndex(iModIndex, Key);

		if (iElementIndex > -1)
		{
			return f_MLD_GetElementType(iModIndex, iElementIndex);
		}
		else {
			return -1;
		}
	}
	else {
		return -1;
	}
}




void cmd_ModLocalDataSetFloat(StaticFunctionTag* base, BSFixedString ModNameIN, BSFixedString KeyIN, float Val)
{
	string ModName = ModNameIN;
	string Key = KeyIN;
	int iModIndex = -1;
	int iCreated = 0;

	iModIndex = f_MLD_GetModIndex(ModName);
	if (iModIndex > -1)
	{}
	else {
		iModIndex = f_MLD_CreateModData(ModName);
		iCreated = 1;
	}

	if (iCreated)
	{
		f_MLD_CreateNewElementFloat(iModIndex, Key, Val);
	}
	else {

		int iElementIndex = f_MLD_GetElementIndex(iModIndex,Key);

		if (iElementIndex > -1)
		{
			f_MLD_ReplaceElementFloat(iModIndex, iElementIndex, Val);
		}
		else {
			f_MLD_CreateNewElementFloat(iModIndex, Key, Val);
		}
	}
}

float cmd_ModLocalDataGetFloat(StaticFunctionTag* base, BSFixedString ModNameIN, BSFixedString KeyIN)
{
	string ModName = ModNameIN;
	string Key = KeyIN;
	int iModIndex = -1;

	iModIndex = f_MLD_GetModIndex(ModName);
	if (iModIndex > -1)
	{
		int iElementIndex = f_MLD_GetElementIndex(iModIndex, Key);

		if (iElementIndex > -1)
		{
			return f_MLD_GetElementFloat(iModIndex, iElementIndex);
		}
		else {
			return 0;
		}

	}
	else {
		return 0;
	}
}



void cmd_ModLocalDataSetInt(StaticFunctionTag* base, BSFixedString ModNameIN, BSFixedString KeyIN, UInt32 Val)
{
	string ModName = ModNameIN;
	string Key = KeyIN;
	int iModIndex = -1;
	int iCreated = 0;

	iModIndex = f_MLD_GetModIndex(ModName);
	if (iModIndex > -1)
	{
	}
	else {
		iModIndex = f_MLD_CreateModData(ModName);
		iCreated = 1;
	}

	if (iCreated)
	{
		f_MLD_CreateNewElementInt(iModIndex, Key, Val);
	}
	else {

		int iElementIndex = f_MLD_GetElementIndex(iModIndex, Key);

		if (iElementIndex > -1)
		{
			f_MLD_ReplaceElementInt(iModIndex, iElementIndex, Val);
		}
		else {
			f_MLD_CreateNewElementInt(iModIndex, Key, Val);
		}
	}
}

UInt32 cmd_ModLocalDataGetInt(StaticFunctionTag* base, BSFixedString ModNameIN, BSFixedString KeyIN)
{
	string ModName = ModNameIN;
	string Key = KeyIN;
	int iModIndex = -1;

	iModIndex = f_MLD_GetModIndex(ModName);
	if (iModIndex > -1)
	{
		int iElementIndex = f_MLD_GetElementIndex(iModIndex, Key);

		if (iElementIndex > -1)
		{
			return f_MLD_GetElementInt(iModIndex, iElementIndex);
		}
		else {
			return 0;
		}

	}
	else {
		return 0;
	}
}

void cmd_ModLocalDataSetRef(StaticFunctionTag* base, BSFixedString ModNameIN, BSFixedString KeyIN, TESObjectREFR* Val)
{
	string ModName = ModNameIN;
	string Key = KeyIN;
	int iModIndex = -1;
	int iCreated = 0;

	iModIndex = f_MLD_GetModIndex(ModName);
	if (iModIndex > -1)
	{
	}
	else {
		iModIndex = f_MLD_CreateModData(ModName);
		iCreated = 1;
	}

	if (iCreated)
	{
		f_MLD_CreateNewElementRef(iModIndex, Key, Val);
	}
	else {

		int iElementIndex = f_MLD_GetElementIndex(iModIndex, Key);

		if (iElementIndex > -1)
		{
			f_MLD_ReplaceElementRef(iModIndex, iElementIndex, Val);
		}
		else {
			f_MLD_CreateNewElementRef(iModIndex, Key, Val);
		}
	}
}


TESObjectREFR* cmd_ModLocalDataGetRef(StaticFunctionTag* base, BSFixedString ModNameIN, BSFixedString KeyIN)
{
	string ModName = ModNameIN;
	string Key = KeyIN;
	int iModIndex = -1;

	iModIndex = f_MLD_GetModIndex(ModName);
	if (iModIndex > -1)
	{
		int iElementIndex = f_MLD_GetElementIndex(iModIndex, Key);

		if (iElementIndex > -1)
		{
			return f_MLD_GetElementRef(iModIndex, iElementIndex);
		}
		else {
			return NULL;
		}

	}
	else {
		return NULL;
	}
}



void cmd_ModLocalDataSetString(StaticFunctionTag* base, BSFixedString ModNameIN, BSFixedString KeyIN, BSFixedString ValIN)
{
	string ModName = ModNameIN;
	string Key = KeyIN;
	string Val = ValIN;
	int iModIndex = -1;
	int iCreated = 0;

	iModIndex = f_MLD_GetModIndex(ModName);
	if (iModIndex > -1)
	{
	}
	else {
		iModIndex = f_MLD_CreateModData(ModName);
		iCreated = 1;
	}

	if (iCreated)
	{
		f_MLD_CreateNewElementString(iModIndex, Key, Val);
	}
	else {

		int iElementIndex = f_MLD_GetElementIndex(iModIndex, Key);

		if (iElementIndex > -1)
		{
			f_MLD_ReplaceElementString(iModIndex, iElementIndex, Val);
		}
		else {
			f_MLD_CreateNewElementString(iModIndex, Key, Val);
		}
	}
}

BSFixedString cmd_ModLocalDataGetString(StaticFunctionTag* base, BSFixedString ModNameIN, BSFixedString KeyIN)
{
	string ModName = ModNameIN;
	string Key = KeyIN;
	int iModIndex = -1;


	iModIndex = f_MLD_GetModIndex(ModName);
	if (iModIndex > -1)
	{
		int iElementIndex = f_MLD_GetElementIndex(iModIndex, Key);

		if (iElementIndex > -1)
		{
			return f_MLD_GetElementString(iModIndex, iElementIndex).c_str();
		}
		else {
			return "";
		}

	}
	else {
		return "";
	}
}





bool cmd_ModLocalDataRemoveKey(StaticFunctionTag* base, BSFixedString ModNameIN, BSFixedString KeyIN)
{
	string ModName = ModNameIN;
	string Key = KeyIN;
	int iModIndex = -1;

	iModIndex = f_MLD_GetModIndex(ModName);
	if (iModIndex > -1)
	{
		int iElementIndex = f_MLD_GetElementIndex(iModIndex, Key);

		if (iElementIndex > -1)
		{
			return f_MLD_RemoveKey(iModIndex, iElementIndex);
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}


bool cmd_ModLocalDataRemoveAllKeys(StaticFunctionTag* base, BSFixedString ModNameIN)
{
	string ModName = ModNameIN;
	int iModIndex = -1;
	
	iModIndex = f_MLD_GetModIndex(ModName);
	if (iModIndex > -1)
	{
		return f_MLD_RemoveAllKeys(iModIndex);
	}
	else {
		return false;
	}
}

void cmd_ModLocalDataDumpToConsole(StaticFunctionTag* base, BSFixedString ModNameIN)
{
	string ModName = ModNameIN;
	int iModIndex = -1;


	iModIndex = f_MLD_GetModIndex(ModName);
	if (iModIndex > -1)
	{
		f_MLD_DumpToConsole(iModIndex);
	}
	else {
		Console_Print("ModLocalData %s doesn't exist.", ModName.c_str());
		return;
	}
}


VMArray<ModLocalDataElement> cmd_ModLocalDataDumpToArray(StaticFunctionTag* base, BSFixedString ModNameIN)
{
	VMArray<ModLocalDataElement> result;

	string ModName = ModNameIN;
	int iModIndex = -1;


	iModIndex = f_MLD_GetModIndex(ModName);

	if (iModIndex <= -1)
	{
		return result;
	}

	for (auto it = g_MLDArray[iModIndex]->Elements.begin(); it != g_MLDArray[iModIndex]->Elements.end(); ++it)
	{
		ModLocalDataElement Element;

		Element.Set<BSFixedString>("Name", Iter->Name.c_str());
		Element.Set<UInt32>("Type", Iter->Type);


		if (Iter->Type == MLDElementType::kType_Float)
		{
			Element.Set<float>("Val_float", Iter->Val_float);
		}
		else if (Iter->Type == MLDElementType::kType_Int)
		{
			Element.Set<UInt32>("Val_int", Iter->Val_int);
		}
		else if (Iter->Type == MLDElementType::kType_Ref)
		{
			Element.Set<TESObjectREFR*>("Val_Ref", Iter->Val_Ref);
		}
		else if (Iter->Type == MLDElementType::kType_String)
		{
			Element.Set<BSFixedString>("Val_String", Iter->Val_String.c_str());
		}

		result.Push(&Element);
	}

	return result;

}








bool f_MLD_CreateNewElementRefArray(int iModDataIndex, string Key, TESObjectREFR* Val)
{
	MLDElement* Element = new MLDElement;
	Element->Type = MLDElementType::kType_RefArray;
	Element->Name = Key;
	Element->RefArray.push_back(Val);
	g_MLDArray[iModDataIndex]->Elements.push_back(Element);
	return true;
}



bool f_MLD_AddElementRefArray(int iModDataIndex, int iElementIndex, TESObjectREFR* Val)
{
	MLDElement* Element = g_MLDArray[iModDataIndex]->Elements[iElementIndex];

	if (Element->Type == MLDElementType::kType_RefArray)
	{
		for (auto it = g_MLDArray[iModDataIndex]->Elements[iElementIndex]->RefArray.begin(); it != g_MLDArray[iModDataIndex]->Elements[iElementIndex]->RefArray.end(); ++it)
		{
			if (Iter->formID == Val->formID)
			{
				return false;
			}

		}

	}else
	{
		Element->CleanValue();
		Element->Type = MLDElementType::kType_Ref;
	}

	Element->RefArray.push_back(Val);
	return true;
}

VMArray<TESObjectREFR*> f_MLD_GetElementRefArray(int iModDataIndex, int iElementIndex)
{
	VMArray<TESObjectREFR*> result;
	for (auto it = g_MLDArray[iModDataIndex]->Elements[iElementIndex]->RefArray.begin(); it != g_MLDArray[iModDataIndex]->Elements[iElementIndex]->RefArray.end(); ++it)
	{
		result.Push(&Iter);
	}
	return result;
}



bool cmd_ModLocalDataRefArrayAddReference(StaticFunctionTag* base, BSFixedString ModNameIN, BSFixedString KeyIN, TESObjectREFR* Val)
{
	string ModName = ModNameIN;
	string Key = KeyIN;
	int iModIndex = -1;
	int iCreated = 0;

	iModIndex = f_MLD_GetModIndex(ModName);
	if (iModIndex > -1)
	{
	}
	else {
		iModIndex = f_MLD_CreateModData(ModName);
		iCreated = 1;
	}

	if (iCreated)
	{
		return f_MLD_CreateNewElementRefArray(iModIndex, Key, Val);
	}
	else {

		int iElementIndex = f_MLD_GetElementIndex(iModIndex, Key);

		if (iElementIndex > -1)
		{
			return f_MLD_AddElementRefArray(iModIndex, iElementIndex, Val);
		}
		else {
			return f_MLD_CreateNewElementRefArray(iModIndex, Key, Val);
		}
	}
}





VMArray<TESObjectREFR*> cmd_ModLocalDataRefArrayGet(StaticFunctionTag* base, BSFixedString ModNameIN, BSFixedString KeyIN)
{
	string ModName = ModNameIN;
	string Key = KeyIN;
	int iModIndex = -1;

	VMArray<TESObjectREFR*> result;

	iModIndex = f_MLD_GetModIndex(ModName);
	if (iModIndex > -1)
	{
		int iElementIndex = f_MLD_GetElementIndex(iModIndex, Key);

		if (iElementIndex > -1)
		{
			return f_MLD_GetElementRefArray(iModIndex, iElementIndex);
		}
		else {
			return result;
		}

	}
	else {
		return result;
	}
}




UInt32 f_MLD_GetElementRefArrayIndex(int iModDataIndex, int iElementIndex, TESObjectREFR* Val)
{
	UInt32 result = -1;

	for (auto it = g_MLDArray[iModDataIndex]->Elements[iElementIndex]->RefArray.begin(); it != g_MLDArray[iModDataIndex]->Elements[iElementIndex]->RefArray.end(); ++it)
	{
		if (Iter->formID == Val->formID)
		{
			result = it - g_MLDArray[iModDataIndex]->Elements[iElementIndex]->RefArray.begin();
			break;
		}

	}

	return result;
}



UInt32 cmd_ModLocalDataRefArrayGetIndex(StaticFunctionTag* base, BSFixedString ModNameIN, BSFixedString KeyIN, TESObjectREFR* Val)
{
	string ModName = ModNameIN;
	string Key = KeyIN;
	int iModIndex = -1;


	iModIndex = f_MLD_GetModIndex(ModName);
	if (iModIndex > -1)
	{
		int iElementIndex = f_MLD_GetElementIndex(iModIndex, Key);

		if (iElementIndex > -1)
		{
			return f_MLD_GetElementRefArrayIndex(iModIndex, iElementIndex, Val);
		}
		else {
			return -2;
		}

	}
	else {
		return -3;
	}
}










UInt32 f_MLD_RefArrayRemoveReference(int iModDataIndex, int iElementIndex, TESObjectREFR* Val)
{

	for (auto it = g_MLDArray[iModDataIndex]->Elements[iElementIndex]->RefArray.begin(); it != g_MLDArray[iModDataIndex]->Elements[iElementIndex]->RefArray.end(); ++it)
	{
		if (Iter->formID == Val->formID)
		{
			g_MLDArray[iModDataIndex]->Elements[iElementIndex]->RefArray.erase(it);
			return 1;
		}
	}

	
	return -1;
}



UInt32 cmd_ModLocalDataRefArrayRemoveReference(StaticFunctionTag* base, BSFixedString ModNameIN, BSFixedString KeyIN, TESObjectREFR* Val)
{
	string ModName = ModNameIN;
	string Key = KeyIN;
	int iModIndex = -1;


	iModIndex = f_MLD_GetModIndex(ModName);
	if (iModIndex > -1)
	{
		int iElementIndex = f_MLD_GetElementIndex(iModIndex, Key);

		if (iElementIndex > -1)
		{
			return f_MLD_RefArrayRemoveReference(iModIndex, iElementIndex, Val);
		}
		else {
			return -2;
		}

	}
	else {
		return -3;
	}
}