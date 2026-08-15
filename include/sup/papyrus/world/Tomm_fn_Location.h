



bool cmd_IsLocationEverCleared(StaticFunctionTag* base, BGSLocation* Location)
{
	if (!Location)
		return false;

	return Location->everCleared;
}