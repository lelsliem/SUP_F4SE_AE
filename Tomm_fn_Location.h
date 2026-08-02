
#include "F4SE_Compat.h"

bool cmd_IsLocationEverCleared(StaticFunctionTag* base, BGSLocation* Location)
{
	if (!Location)
		return false;

	return GetLocationEverCleared(Location);
}