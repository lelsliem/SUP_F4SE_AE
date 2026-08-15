



bool cmd_SteamIsLoaded(StaticFunctionTag* base)
{
	return iSteamLoaded;
}



BSFixedString cmd_SteamGetGameLanguage(StaticFunctionTag* base)
{
	if (!iSteamLoaded || !SteamAPI_SteamApps)
		return "";

	const char* result = SteamAPI_SteamApps->GetCurrentGameLanguage();

	if (result)
	{
		return result;
	}
	else {
		return "";
	}
}


BSFixedString cmd_SteamGetSteamLanguage(StaticFunctionTag* base) 
{
	if (!iSteamLoaded || !SteamAPI_SteamUtils)
		return "";

	const char* result = SteamAPI_SteamUtils->GetSteamUILanguage();

	if (result)
	{
		return result;
	}
	else {
		return "";
	}

}


bool cmd_IsSteamRunningOnSteamDeck(StaticFunctionTag* base) // Fallout 4 DLL not updated for this yet
{
	if (!iSteamLoaded || !SteamAPI_SteamUtils)
		return "";

	return SteamAPI_SteamUtils->IsSteamRunningOnSteamDeck();
}

bool cmd_SteamIsOverlayEnabled(StaticFunctionTag* base)
{
	if (!iSteamLoaded || !SteamAPI_SteamUtils)
		return "";

	return SteamAPI_SteamUtils->IsOverlayEnabled();
}




void cmd_SteamTriggerScreenshot(StaticFunctionTag* base)
{
	if (!iSteamLoaded || !SteamAPI_SteamScreenshots)
		return;

	SteamAPI_SteamScreenshots->TriggerScreenshot();
}