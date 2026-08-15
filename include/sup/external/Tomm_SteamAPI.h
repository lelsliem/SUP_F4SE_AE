#include "third_party/steam_api.h"


bool iSteamLoaded = false;


HMODULE g_Steam_Module = NULL;

ISteamFriends* SteamAPI_SteamFriends = NULL;
ISteamUtils* SteamAPI_SteamUtils = NULL;
ISteamApps* SteamAPI_SteamApps = NULL;
ISteamScreenshots* SteamAPI_SteamScreenshots = NULL;



func_ext_bool f_SteamAPI_Init = NULL;


typedef ISteamFriends* (*func_ext_SteamFriendsGetClass)();
typedef ISteamUtils* (*func_ext_SteamUtilsGetClass)();
typedef ISteamApps* (*func_ext_SteamAppsGetClass)();
typedef ISteamScreenshots* (*func_ext_SteamScreenshotsGetClass)();


//SteamUtils

bool f_LoadSteam()
{

	func_ext_SteamFriendsGetClass SteamAPI_SteamFriendsClassGetFunction = NULL;
	func_ext_SteamUtilsGetClass SteamAPI_SteamUtilsClassGetFunction = NULL;
	func_ext_SteamAppsGetClass SteamAPI_SteamAppsClassGetFunction = NULL;
	func_ext_SteamScreenshotsGetClass SteamAPI_SteamScreenshotsClassGetFunction = NULL;


	g_Steam_Module = GetModuleHandleA("steam_api64.dll");


	if (!g_Steam_Module)
	{
		_DMESSAGE("Steam DLL NOT LOADED");
		return false;
	}

	_DMESSAGE("Steam DLL LOADED");


	f_SteamAPI_Init = (func_ext_bool)GetProcAddress(g_Steam_Module, "SteamAPI_Init");


	if (!f_SteamAPI_Init)
	{
		_DMESSAGE("SteamAPI_Init NOT LOADED");
		return false;
	}

	_DMESSAGE("SteamAPI_Init LOADED");
	

	//SteamAPI_SteamFriendsClassGetFunction = (func_ext_SteamFriendsGetClass)GetProcAddress(g_Steam_Module, "SteamFriends");

	//if (!SteamAPI_SteamFriendsClassGetFunction)
	//{
	//	_DMESSAGE("Steam Friends class Getter NOT LOADED");
	//	return false;
	//}
	//else {
	//	_DMESSAGE("Steam Friends class Getter LOADED");
	//	SteamAPI_SteamFriends = SteamAPI_SteamFriendsClassGetFunction();
	//	if (!SteamAPI_SteamFriends)
	//	{
	//		_DMESSAGE("Steam Friends Class NOT LOADED");
	//		return false;
	//	}
	//	else {
	//		_DMESSAGE("Steam Friends ClassLOADED");
	//	}
	//}



	SteamAPI_SteamUtilsClassGetFunction = (func_ext_SteamUtilsGetClass)GetProcAddress(g_Steam_Module, "SteamUtils");

	if (!SteamAPI_SteamUtilsClassGetFunction)
	{
		_DMESSAGE("Steam Utils class Getter NOT LOADED");
		return false;
	}

	_DMESSAGE("Steam Utils class Getter LOADED");
	SteamAPI_SteamUtils = SteamAPI_SteamUtilsClassGetFunction();
	if (!SteamAPI_SteamUtils)
	{
		_DMESSAGE("Steam Utils Class NOT LOADED");
		return false;
	}

	_DMESSAGE("Steam Utils ClassLOADED");
	



	SteamAPI_SteamAppsClassGetFunction = (func_ext_SteamAppsGetClass)GetProcAddress(g_Steam_Module, "SteamApps");

	if (!SteamAPI_SteamAppsClassGetFunction)
	{
		_DMESSAGE("Steam Apps class Getter NOT LOADED");
		return false;
	}

		_DMESSAGE("Steam Apps class Getter LOADED");
		SteamAPI_SteamApps = SteamAPI_SteamAppsClassGetFunction();
		if (!SteamAPI_SteamApps)
		{
			_DMESSAGE("Steam Apps Class NOT LOADED");
			return false;
		}
		else {
			_DMESSAGE("Steam Apps ClassLOADED");
		}
	


	SteamAPI_SteamScreenshotsClassGetFunction = (func_ext_SteamScreenshotsGetClass)GetProcAddress(g_Steam_Module, "SteamScreenshots");

	if (!SteamAPI_SteamScreenshotsClassGetFunction)
	{
		_DMESSAGE("Steam Screenshots class Getter NOT LOADED");
		return false;
	}

		_DMESSAGE("Steam Screenshots class Getter LOADED");
		SteamAPI_SteamScreenshots = SteamAPI_SteamScreenshotsClassGetFunction();
		if (!SteamAPI_SteamScreenshots)
		{
			_DMESSAGE("Steam Screenshots Class NOT LOADED");
			return false;
		}
		else {
			_DMESSAGE("Steam Screenshots ClassLOADED");
		}
	

	iSteamLoaded = true;
	_MESSAGE("Steam API Loaded");

	return true;

}