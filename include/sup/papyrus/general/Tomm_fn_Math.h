
UInt32 cmd_Round(StaticFunctionTag* base, float fValue)
{
	return round(fValue);
}




GetCalculatedPosRes cmd_GetCalculatedPos(StaticFunctionTag* base, TESObjectREFR* ObjectRef, float fAngle, float fDistance)
{
	GetCalculatedPosRes result;

	if (!ObjectRef)
		return result;

	result.Set<float>("OutX", ((ObjectRef->GetPosition().x) + fDistance * (sin(fAngle * 0.017453292519943295))));
	result.Set<float>("OutY", ((ObjectRef->GetPosition().y) + fDistance * (cos(fAngle * 0.017453292519943295))));
	return result;
}


GetCalculatedPosRes cmd_GetCalculatedPosAlt(StaticFunctionTag* base, float fXIn, float fYIn, float fAngle, float fDistance)
{
	GetCalculatedPosRes result;
	result.Set<float>("OutX", (fXIn + fDistance * (sin(fAngle * 0.017453292519943295))));
	result.Set<float>("OutY", (fYIn + fDistance * (cos(fAngle * 0.017453292519943295))));
	return result;
}

float cmd_GetDistanceBetweenPoints(StaticFunctionTag* base, float x1, float x2, float y1, float y2, float z1, float z2)
{
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2) * 1.0);
}



float cmd_GetHeadingAngleAlt(StaticFunctionTag* base, TESObjectREFR* ObjectRef, float CallX, float CallY, float dir)
{
	if (!ObjectRef)
		return -1;

	float TargetX, TargetY;

	TargetX = ObjectRef->GetPosition().x;
	TargetY = ObjectRef->GetPosition().y;

	double heading = std::atan2(TargetX - CallX, TargetY - CallY) * 180 / 3.1415926535897 - dir;
	if (heading < -180) heading += 360;
	if (heading > 180) heading -= 360;

	return heading;
}


float cmd_GetHeadingAngleBetweenPoints(StaticFunctionTag* base, float CallX, float CallY, float dir, float TargetX, float TargetY)
{
	double heading = std::atan2(TargetX - CallX, TargetY - CallY) * 180 / 3.1415926535897 - dir;
	if (heading < -180) heading += 360;
	if (heading > 180) heading -= 360;

	return heading;
}


#define maxALT(a,b)            (((a) > (b)) ? (a) : (b))


bool f_RelativelyClose(double fOnlineVersion, double fLocalVersion)
{
	_DMESSAGE("f_RelativelyClose comparing %f with %f", fOnlineVersion, fLocalVersion);
	const auto relative_difference_factor = 0.0001;
	const auto greater_magnitude = maxALT(std::abs(fOnlineVersion), std::abs(fLocalVersion));

	if (std::abs(fOnlineVersion - fLocalVersion) < relative_difference_factor * greater_magnitude)
	{
		_DMESSAGE("Relatively close");
		return true;
	}
	else
	{
		_DMESSAGE("NOT relatively close");
		return false;
	}

}