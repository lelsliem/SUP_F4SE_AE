



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
	// GetSteamUILanguage not available in F4SE's Steam SDK version
	// Fall back to game language
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


bool cmd_IsSteamRunningOnSteamDeck(StaticFunctionTag* base)
{
	// IsSteamRunningOnSteamDeck not available in F4SE's Steam SDK version
	// Return false as default
	return false;
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