
struct PassedCatchedMod {
	int CheckType;
	int NexusID;
	string GitHubPath;
	string OnlineVersion;
};

struct PassedGlobal {
	int exists;
	float data;
};

struct PassedForm {
	int exists;
	string name;
};




typedef int(*func_ext_int)();
typedef bool(*func_ext_bool)();
typedef float(*func_ext_float)();
typedef string(*func_ext_string)();

typedef bool(*func_ext_UpdateChecker_IsAPIKeyValid)();
typedef string(*func_ext_GetNexusModVersionAPI)(int iNexusID);


typedef vector<PassedCatchedMod>(*func_ext_UpdateChecker_PassCachedMods)();

func_ext_int f_SUPDater_GetVersion = NULL;
func_ext_int f_SUPDater_GetCurrentStage = NULL;
func_ext_int f_SUPDater_GetDoneStage = NULL;
func_ext_int f_SUPDater_GetInterfaceVer = NULL;


func_ext_UpdateChecker_IsAPIKeyValid f_SUPDater_IsAPIKeyValid = NULL;
func_ext_GetNexusModVersionAPI f_SUPDater_GetNexusModVersionAPI = NULL;
func_ext_UpdateChecker_PassCachedMods f_SUPDater_PassCachedMods = NULL;


HMODULE g_SUPDater_Module = NULL;






class CachedPlugin
{
public:

	int PluginType = -1; //1 for ESM or ESPS, 2 for SE plugins.
	int CheckType = -1;//1 for Nexus, 2 for Github
	string OnlineVersion{};
	string GitHubPath{}; // For GitHub
	int NexusID = -1; // For Nexus
};


vector<CachedPlugin*> g_CachedPlugins;
int iPluginsCached = 0;

#define t_SUPDateInterfaceVER 1

#define t_PluginType_ESM_or_ESP 1
#define t_PluginType_SE 2

#define t_CheckType_Nexus 1
#define t_CheckType_GitHub 2

#define t_OutputType_Default 0
#define t_OutputType_Console 1

int t_PluginStage_Done = 9999;













void f_UpdateChecker_AddCachedPlugin(int CheckType, int NexusID, string GitHubPath, string OnlineVersion)
{
	_DMESSAGE("f_AddCachedPlugin started with Checktype>>>>%d NexusID>>>%d GitHubPath>>>>%s OnlineVersion>>%s", CheckType, NexusID, GitHubPath.c_str(), OnlineVersion.c_str());
	int iFound = 0;

	for (auto it2 = g_CachedPlugins.begin(); it2 != g_CachedPlugins.end(); it2++)
	{
		if (Iter2->CheckType == CheckType)
		{
			if (CheckType == t_CheckType_Nexus)
			{
				if (NexusID == Iter2->NexusID)
				{
					iFound = 1;
					_DMESSAGE("Found in existing array");
				}
			}
			else if (CheckType == t_CheckType_GitHub)
			{
				if (GitHubPath == Iter2->GitHubPath)
				{
					iFound = 1;
					_DMESSAGE("Found in existing array");
				}

			}

		}

	}

	if (iFound)
	{
		return;
	}

	CachedPlugin* NewPlugin = NULL;

	NewPlugin = new CachedPlugin;
	NewPlugin->CheckType = CheckType;

	if (NewPlugin->CheckType == t_CheckType_Nexus)
	{
		NewPlugin->NexusID = NexusID;
		_DMESSAGE("Adding NEXUS mod to catched plugins with NexusID>>>%d", NexusID);

	}
	else if (NewPlugin->CheckType == t_CheckType_GitHub)
	{
		NewPlugin->GitHubPath = GitHubPath;
		_DMESSAGE("Adding GitHub mod to catched plugins with NexusID>>>%s", GitHubPath.c_str());
	}
	NewPlugin->OnlineVersion = OnlineVersion;
	g_CachedPlugins.push_back(NewPlugin);
}



int f_UpdateChecker_GetInterfaceVer()
{
	return t_SUPDateInterfaceVER;
}




bool f_UpdateChecker_ShowMessage(string s_Message, int iOutputType)
{


	if (iOutputType == t_OutputType_Console)
	{
		i_SUPDater_ShowMessage = 2;
		Console_Print("%s", s_Message.c_str());
		return true;
	}else {
		if (b_SUPDater_LoadingDone)
		{
			i_SUPDater_ShowMessage = 2;
			_DMESSAGE("f_UpdateChecker_InvokeMessage started, message>>%s", s_Message.c_str());
			//const char* ptrMes = "TEST MESSAGE BOX";
			BSFixedString g_Message;
			CALL_MEMBER_FN(&g_Message, Set)(s_Message.c_str());

			VMArray<VMVariable> arguments;
			VMValue args;
			VMVariable var1;
			var1.Set<BSFixedString>(&g_Message);
			arguments.Push(&var1);
			arguments.PackArray(&args, g_MyVirtMachine);

			BSFixedString g_ScriptName;
			BSFixedString g_FuncName;

			const char* ptr = "Debug";
			CALL_MEMBER_FN(&g_ScriptName, Set)(ptr);
			const char* ptr2 = "MessageBox"; //Notification
			CALL_MEMBER_FN(&g_FuncName, Set)(ptr2);

			_DMESSAGE("MessageBox Call 1");
			CallGlobalFunctionNoWait_Internal(g_MyVirtMachine, 0, 0, &g_ScriptName, &g_FuncName, &args);
			_DMESSAGE("MessageBox Call 2");
			return true;
		}
		else {
			i_SUPDater_ShowMessage = 1;
			s_SUPDater_MessageToShow = s_Message;
			_DMESSAGE("Loading not yet done - saving message");
		}

	}


}



bool f_UpdateChecker_IsModLoaded(string PluginFileName)
{
	_DMESSAGE("f_UpdateChecker_IsModLoaded started for %s", PluginFileName.c_str());
	UInt32 mod_Idx = (*g_dataHandler)->GetLoadedModIndex(PluginFileName.c_str());

	_DMESSAGE("f_UpdateChecker_ModIndex>>%d", mod_Idx);

	if (mod_Idx == 255)
	{
		_DMESSAGE("RETURN FALSE");
		return false;
	}
	else {
		_DMESSAGE("RETURN TRUE");
		return true;
	}
}




PassedGlobal f_UpdateChecker_GetGlobalVariable(string PluginFileName, string GlobalValueFormID)
{
	_DMESSAGE("GetGlobalFromMod startes with mod >>>>%s, formID is>>>%s", PluginFileName.c_str(), GlobalValueFormID.c_str());

	PassedGlobal result;
	result.exists = 0;
	result.data = -1;


	TESForm* form_Result = BuildRefFormFromSTR(GlobalValueFormID, PluginFileName, 0);



	//UInt32 mod_Idx = (*g_dataHandler)->GetLoadedModIndex(PluginFileName.c_str());

	//_DMESSAGE("Mod index is %d", mod_Idx);
	//if (mod_Idx == 255)
	//{
	//	_DMESSAGE("%s is not loaded", PluginFileName.c_str());
	//	return result;
	//}



	//UInt32 form_Dec;


	//try
	//{
	//	form_Dec = std::stoul(GlobalValueFormID, nullptr, 16);
	//}
	//catch (std::invalid_argument e)
	//{
	//	_DMESSAGE("HeX threw exception inv agr - skipping");
	//	return result;
	//}
	//catch (std::out_of_range  e)
	//{
	//	_DMESSAGE("HeX threw exception out of range - skipping");
	//	return result;
	//}

	//UInt32 referenceRes = ((mod_Idx & 0xFF) << 24) | (form_Dec & 0x00FFFFFF);

	//TESForm* form_Result = (TESForm*)LookupFormByID(referenceRes);

	if (!form_Result)
	{
		_DMESSAGE("Global variable not found");
		return result;
	}


	if (form_Result->formType != kFormType_GLOB)
	{
		_DMESSAGE("Gloval variable has different type, type is %d", form_Result->formType);
		return result;
	}


	TESGlobal* GlobalResult = (TESGlobal*)form_Result;


	result.exists = 1;
	result.data = GlobalResult->value;

	return result;
}




PassedForm f_UpdateChecker_GetForm(string PluginFileName, string GlobalValueFormID)
{
	_DMESSAGE("GetForm started with mod >>>>%s, formID is>>>%s", PluginFileName.c_str(), GlobalValueFormID.c_str());

	PassedForm result;
	result.exists = 0;
	result.name = "";


	TESForm* form_Result = BuildRefFormFromSTR(GlobalValueFormID, PluginFileName, 0);

	//UInt32 mod_Idx = (*g_dataHandler)->GetLoadedModIndex(PluginFileName.c_str());

	//_DMESSAGE("Mod index is %d", mod_Idx);
	//if (mod_Idx == 255)
	//{
	//	_DMESSAGE("%s is not loaded", PluginFileName.c_str());
	//	return result;
	//}



	//UInt32 form_Dec;


	//try
	//{
	//	form_Dec = std::stoul(GlobalValueFormID, nullptr, 16);
	//}
	//catch (std::invalid_argument e)
	//{
	//	_DMESSAGE("HeX threw exception inv agr - skipping");
	//	return result;
	//}
	//catch (std::out_of_range  e)
	//{
	//	_DMESSAGE("HeX threw exception out of range - skipping");
	//	return result;
	//}

	//UInt32 referenceRes = ((mod_Idx & 0xFF) << 24) | (form_Dec & 0x00FFFFFF);

	//TESForm* form_Result = (TESForm*)LookupFormByID(referenceRes);

	if (!form_Result){
		_DMESSAGE("Form not found");
		return result;
	}
	
	result.exists = 1;
	result.name = form_Result->GetFullName();
	_DMESSAGE("Form name is %s", result.name.c_str());

	return result;

}









bool f_UpdateChecker_InitSUPDater()
{
	_DMESSAGE("InitSUPDater starts");
	g_SUPDater_Module = GetModuleHandleA("SUPDateChecker.dll");


	if (!g_SUPDater_Module)
	{
		_DMESSAGE("SUPDATEChecker not loaded");
		return false;
	}

	f_SUPDater_GetVersion = (func_ext_int)GetProcAddress(g_SUPDater_Module, "fn_ext_GETVERSION");

	if (!f_SUPDater_GetVersion)
	{
		_DMESSAGE("SUPDATEChecker function get version  does not exist");
		return false;
	}

	if (f_SUPDater_GetVersion() <= 0)
	{
		_DMESSAGE("SUPDATEChecker version returns wrong values");
		return false;
	}

	f_SUPDater_GetInterfaceVer = (func_ext_int)GetProcAddress(g_SUPDater_Module, "f_UpdateChecker_GetInterfaceVer");

	if (!f_SUPDater_GetInterfaceVer)
	{
		_DMESSAGE("GetInterfaceVer function get version  does not exist");
		return false;
	}

	if (f_SUPDater_GetInterfaceVer() != t_SUPDateInterfaceVER)
	{
		_DMESSAGE("SUPDATEChecker not loaded, Interface is %d, SUP NVSE interface is %d", f_SUPDater_GetInterfaceVer(), t_SUPDateInterfaceVER);
		return false;
	}


	f_SUPDater_GetCurrentStage = (func_ext_int)GetProcAddress(g_SUPDater_Module, "f_UpdateChecker_GetCurrentStage");
	f_SUPDater_GetDoneStage = (func_ext_int)GetProcAddress(g_SUPDater_Module, "f_UpdateChecker_GetDoneStage");
	f_SUPDater_IsAPIKeyValid = (func_ext_UpdateChecker_IsAPIKeyValid)GetProcAddress(g_SUPDater_Module, "f_UpdateChecker_IsAPIKeyValid");
	f_SUPDater_GetNexusModVersionAPI = (func_ext_GetNexusModVersionAPI)GetProcAddress(g_SUPDater_Module, "f_UpdateChecker_GetNexusModVersionAPI");
	f_SUPDater_PassCachedMods = (func_ext_UpdateChecker_PassCachedMods)GetProcAddress(g_SUPDater_Module, "f_UpdateChecker_PassCachedMods");


	if (f_SUPDater_IsAPIKeyValid && f_SUPDater_GetCurrentStage && f_SUPDater_GetDoneStage && f_SUPDater_PassCachedMods)
	{
		bSUPdaterLoaded = true;
		t_PluginStage_Done = f_SUPDater_GetDoneStage();
		_DMESSAGE("SUPDATEChecker LOAD SUCCESS, DONE STAGE IS %d", t_PluginStage_Done);
	}
	else {
		_DMESSAGE("SUPDATEChecker not loaded - not all functions are found in SUP. Please report to mod author");
	}

}

