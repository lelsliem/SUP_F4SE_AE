

// Resolve node is taken from SKSE and modified for FO4
NiAVObject* ResolveNode(TESObjectREFR* refr, BSFixedString NodeName, int iPCRequest = 0) // 1 for First person, 2 for third person
{
	_DMESSAGE("ResolveNode started with request::%s,iPCRequest is %d", NodeName.c_str(), iPCRequest);
	
	if (!refr) return NULL;

	string sNodeName = NodeName;

	NiAVObject* result = NULL;
	NiAVObject* root = NULL;

	if (iPCRequest)
	{
		PlayerCharacter* player = DYNAMIC_CAST(refr, TESObjectREFR, PlayerCharacter);
		if (player && player->unkF0)
		{
			if (iPCRequest == 1)
			{
				root = player->firstPersonSkeleton;

			}
			else {
				root = player->unkF0->rootNode;
			}

			if (root && !sNodeName.empty())
			{
				result = root->GetObjectByName(&NodeName);
			}
			else {
				result = refr->GetObjectRootNode();
			}

		}


	}
	else {

		if (!sNodeName.empty())
		{
			root = (NiAVObject*)refr->GetObjectRootNode();

			if (root)
				result = root->GetObjectByName(&NodeName);


		}else {

			result =  (NiAVObject*)refr->GetObjectRootNode();
		}

	}


	_DMESSAGE("ResolveNode ended");
	return result;

}




bool cmd_SetNodeScale(StaticFunctionTag* base, TESObjectREFR* refr, BSFixedString NodeName, float fScale, UInt32 iPCRequest)
{

	_DMESSAGE("SetNodeScale started, node is %s", NodeName.c_str());

	NiAVObject* child = ResolveNode(refr, NodeName, iPCRequest);
	
	if (!child)
	{
		_DMESSAGE("node doesn't exists");
		return false;
	}

	child->m_localTransform.scale = fScale;

	return true;
	//child->m_worldTransform.scale = fScale;

	//NiAVObject::NiUpdateData ctx;
	//child->UpdateWorldData(&ctx);
	//child->UpdateTransforms();
	//child->UpdateTransformAndBounds();
	//child->UpdateWorldBound();
}





float cmd_GetNodeScale(StaticFunctionTag* base, TESObjectREFR* refr, BSFixedString NodeName, UInt32 iPCRequest)
{

	_DMESSAGE("GetNodeScale started, node is %s", NodeName.c_str());

	NiAVObject* child = ResolveNode(refr, NodeName, iPCRequest);
	if (!child)
	{
		_DMESSAGE("child node doesn't exists");
		return false;
	}

	return child->m_localTransform.scale;
}



VMArray<BSFixedString> cmd_GetNodeChildren(StaticFunctionTag* base, TESObjectREFR* refr, BSFixedString NodeName, UInt32 iPCRequest)
{

	_DMESSAGE("GetNodeChildren started, node is %s", NodeName.c_str());

	string s_NodeName = NodeName;

	VMArray<BSFixedString> result;

	NiAVObject* child = ResolveNode(refr, NodeName, iPCRequest);

	NiPointer<NiNode> node = child->GetAsNiNode();

	if (node) {

		if (node->m_children.m_size == 0)
		{
			_DMESSAGE("no children");
			return result;
		}

		_DMESSAGE("Childen count is %d", node->m_children.m_size);

		for (UInt32 i = 0; i < node->m_children.m_size; i++) {
			NiPointer<NiAVObject> object(node->m_children.m_data[i]);
			if (object)
			{
				result.Push(&BSFixedString(object->m_name.c_str()));
				_DMESSAGE("Child name is %s", object->m_name.c_str());
			}
		}
	}

	return result;
}


UInt32 cmd_GetNodeChildrenCount(StaticFunctionTag* base, TESObjectREFR* refr, BSFixedString NodeName, UInt32 iPCRequest)
{
	_DMESSAGE("GetNodeChildrenCount started, node is %s", NodeName.c_str());

	string s_NodeName = NodeName;

	VMArray<BSFixedString> result;

	NiAVObject* child = ResolveNode(refr, NodeName, iPCRequest);

	NiPointer<NiNode> node = child->GetAsNiNode();

	if (node) {
		_DMESSAGE("Childen count is %d", node->m_children.m_size);
		return node->m_children.m_size;
	}else {
		return -1;
	}


	return 0;
}


bool cmd_SetNodeFlags(StaticFunctionTag* base, TESObjectREFR* refr, BSFixedString NodeName, UInt32 iFlags, UInt32 iPCRequest)
{

	_DMESSAGE("SetNodeFlags started, node is %s", NodeName.c_str());

	NiAVObject* child = ResolveNode(refr, NodeName, iPCRequest);
	if (!child)
	{
		_DMESSAGE("child node doesn't exists");
		return false;
	}

	//_DMESSAGE("prev float>>%f", child->unk118);
	//child->unk118 = 0.1;

	//bool mybool = true;
	//Console_Print("flags before>>%d", child->flags);
	//child->flags = child->flags | NiAVObject::kFlagNotVisible;
	//Console_Print("flags after>>%d", child->flags);
	//child->SetSelectiveUpdateFlags(&mybool, true, &mybool);
	//NiAVObject::NiUpdateData ctx;
	//child->UpdateControllers(&ctx);
	//child->UpdateWorldData(&ctx);
	//child->UpdateTransforms();

	child->flags = iFlags;
	return true;
}


//bool HasVisFlag(int flags) {
//	return (flags & (NiAVObject::kFlagNotVisible)) != 0;
//}



UInt32 cmd_GetNodeFlags(StaticFunctionTag* base, TESObjectREFR* refr, BSFixedString NodeName, UInt32 iPCRequest)
{

	_DMESSAGE("GetNodeFlags started, node is %s", NodeName.c_str());

	NiAVObject* child = ResolveNode(refr, NodeName, iPCRequest);
	if (!child)
	{
		_DMESSAGE("child node doesn't exists");
		return false;
	}


	return child->flags;

}




NodeTranslation cmd_GetNodeTranslation(StaticFunctionTag* base, TESObjectREFR* refr, BSFixedString NodeName, UInt32 iWorldRequest, UInt32 iPCRequest)
{

	NodeTranslation result;

	result.Set<UInt32>("Success", 0);



	_DMESSAGE("GetNodeTranslation started, node is %s", NodeName.c_str());

	NiAVObject* child = ResolveNode(refr, NodeName, iPCRequest);
	if (!child)
	{
		_DMESSAGE("node doesn't exists");
		return result;
	}


	result.Set<UInt32>("Success", 1);





	if (iWorldRequest)
	{
		result.Set<float>("x", child->m_worldTransform.pos.x);
		result.Set<float>("y", child->m_worldTransform.pos.y);
		result.Set<float>("z", child->m_worldTransform.pos.z);
	}
	else {
		result.Set<float>("x", child->m_localTransform.pos.x);
		result.Set<float>("y", child->m_localTransform.pos.y);
		result.Set<float>("z", child->m_localTransform.pos.z);
	}

	return result;
}


bool cmd_SetNodeTranslation(StaticFunctionTag* base, TESObjectREFR* refr, BSFixedString NodeName, UInt32 iWorldRequest, NodeTranslation nTranslation, UInt32 iPCRequest)
{

	_DMESSAGE("SetNodeTranslation started, node is %s", NodeName.c_str());

	NiAVObject* child = ResolveNode(refr, NodeName, iPCRequest);
	if (!child)
	{
		_DMESSAGE("child node doesn't exists");
		return false;
	}



	float fX, fY, fZ;

	nTranslation.Get<float>("x", &fX);
	nTranslation.Get<float>("y", &fY);
	nTranslation.Get<float>("z", &fZ);

	_DMESSAGE("Received values are X>>%f, Y>>%f, Z>>%f", fX, fY, fZ);

	if (iWorldRequest)
	{
		child->m_worldTransform.pos.x = fX;
		child->m_worldTransform.pos.y = fY;
		child->m_worldTransform.pos.z= fZ;

	}
	else {
		child->m_localTransform.pos.x = fX;
		child->m_localTransform.pos.y = fY;
		child->m_localTransform.pos.z = fZ;
	}



	if (refr->formType != kFormType_ACHR)
	{
		child->UpdateTransforms();
	}


	return true;
}



bool cmd_SetNodeTranslationAxis(StaticFunctionTag* base, TESObjectREFR* refr, BSFixedString NodeName, UInt32 iWorldRequest, BSFixedString Axis, float Value, UInt32 iPCRequest)
{


	_DMESSAGE("SetNodeTranslation started, node is %s", NodeName.c_str());


	string sAxis = Axis;

	NiAVObject* child = ResolveNode(refr, NodeName, iPCRequest);
	if (!child)
	{
		_DMESSAGE("child node doesn't exists");
		return false;
	}


	if (iWorldRequest)
	{

		if (sAxis == "x")
		{
			child->m_worldTransform.pos.x = Value;
		}
		else if (sAxis == "y")
		{
			child->m_worldTransform.pos.y = Value;
		}else
		{
			child->m_worldTransform.pos.z = Value;
		}

	}
	else {
		if (sAxis == "x")
		{
			child->m_localTransform.pos.x = Value;
		}
		else if (sAxis == "y")
		{
			child->m_localTransform.pos.y = Value;
		}
		else
		{
			child->m_localTransform.pos.z = Value;
		}
	}

	child->UpdateTransforms();

	return true;
}

NodeRotation cmd_GetNodeRotation(StaticFunctionTag* base, TESObjectREFR* refr, BSFixedString NodeName, UInt32 iWorldRequest, UInt32 iPCRequest)
{
	_DMESSAGE("GetNodeRotation started, node is %s", NodeName.c_str());

	NodeRotation result;
	result.Set<UInt32>("Success", 0);


	NiAVObject* child = ResolveNode(refr, NodeName, iPCRequest);
	if (!child)
	{
		_DMESSAGE("child node doesn't exists");
		return result;
	}

	float fX, fY, fZ;

	if (iWorldRequest)
	{
		child->m_worldTransform.rot.GetEulerAngles(&fX, &fY, &fZ);
	}
	else {
		child->m_localTransform.rot.GetEulerAngles(&fX, &fY, &fZ);
	}

	fX *= 180.0 / MATH_PI;
	fY *= 180.0 / MATH_PI;
	fZ *= 180.0 / MATH_PI;

	_DMESSAGE("Received angles are X>>%f, Y>>%f, Z>>%f", fX, fY, fZ);


	result.Set<UInt32>("Success", 1);
	result.Set<float>("x", fX);
	result.Set<float>("y", fY);
	result.Set<float>("z", fZ);
}


bool cmd_SetNodeRotation(StaticFunctionTag* base, TESObjectREFR* refr, BSFixedString NodeName, UInt32 iWorldRequest, NodeRotation nRotation, UInt32 iPCRequest)
{
	_DMESSAGE("SetNodeRotation started, node is %s", NodeName.c_str());

	NiAVObject* child = ResolveNode(refr, NodeName, iPCRequest);
	if (!child)
	{
		_DMESSAGE("child node doesn't exists");
		return false;
	}

	float fX, fY, fZ;


	nRotation.Get<float>("x", &fX);
	nRotation.Get<float>("y", &fY);
	nRotation.Get<float>("z", &fZ);


	fX *= MATH_PI / 180;
	fY *= MATH_PI / 180;
	fZ *= MATH_PI / 180;

	if (iWorldRequest)
	{
		child->m_worldTransform.rot.SetEulerAngles(fX, fY, fZ);
	}
	else {
		child->m_localTransform.rot.SetEulerAngles(fX, fY, fZ);
	}

	if (refr->formType != kFormType_ACHR)
	{
		child->UpdateTransformAndBounds();
	}
	return true;
}

bool cmd_SetNodeRotationAxis(StaticFunctionTag* base, TESObjectREFR* refr, BSFixedString NodeName, UInt32 iWorldRequest, BSFixedString sAxis, float Value, UInt32 iPCRequest)
{
	_DMESSAGE("SetNodeRotationAxis started, node is %s", NodeName.c_str());

	NiAVObject* child = ResolveNode(refr, NodeName, iPCRequest);
	if (!child)
	{
		_DMESSAGE("child node doesn't exists");
		return false;
	}

	float fX, fY, fZ;


	if (iWorldRequest)
	{
		child->m_worldTransform.rot.GetEulerAngles(&fX, &fY, &fZ);
	}
	else {
		child->m_localTransform.rot.GetEulerAngles(&fX, &fY, &fZ);
	}

	fX *= 180.0 / MATH_PI;
	fY *= 180.0 / MATH_PI;
	fZ *= 180.0 / MATH_PI;

	_DMESSAGE("Received angles are X>>%f, Y>>%f, Z>>%f", fX, fY, fZ);

	if (sAxis == "x")
	{
		fX = Value;
	}
	else if (sAxis == "y")
	{
		fY = Value;
	}
	else
	{
		fZ = Value;
	}


	_DMESSAGE("Received angles After setting are X>>%f, Y>>%f, Z>>%f", fX, fY, fZ);


	fX *= MATH_PI / 180;
	fY *= MATH_PI / 180;
	fZ *= MATH_PI / 180;


	if (iWorldRequest)
	{
		child->m_worldTransform.rot.SetEulerAngles(fX, fY, fZ);
	}
	else {
		child->m_localTransform.rot.SetEulerAngles(fX, fY, fZ);
	}



	if (refr->formType != kFormType_ACHR)
	{
		child->UpdateTransformAndBounds();
	}





	return true;
}




bool cmd_NodeAttachChild(StaticFunctionTag* base, TESObjectREFR* refr, BSFixedString NodeNameToAttachTo, BSFixedString NodeNameToAttach, BSFixedString NewNodeName, UInt32 iPCRequest)
{
	_DMESSAGE("NodeAttachChild started, NodeNameToAttachTo is %s, NodeNameToAttach is %s, NewNodeName is %s", NodeNameToAttachTo.c_str(), NodeNameToAttach.c_str(), NewNodeName.c_str());

	NiAVObject* ToAttachTo = ResolveNode(refr, NodeNameToAttachTo, iPCRequest);
	if (!ToAttachTo)
	{
		_DMESSAGE("NodeNameToAttachTo doesn't exists");
		return false;
	}


	_DMESSAGE("Attaching 0.1");
	NiPointer<NiNode> node = ToAttachTo->GetAsNiNode();
	_DMESSAGE("Attaching 0.2");


	NiAVObject* ToAttach = ResolveNode(refr, NodeNameToAttach, iPCRequest);
	if (!ToAttach)
	{
		_DMESSAGE("NodeNameToAttach doesn't exists");
		return false;
	}

	_DMESSAGE("Attaching 1");

	NiPointer<NiNode> n_ToAttach = ToAttach->GetAsNiNode();


	string s_NodeName = NewNodeName.c_str();
	BSFixedString g_NodeName;
	CALL_MEMBER_FN(&g_NodeName, Set)(s_NodeName.c_str());

	n_ToAttach->m_name = g_NodeName;

	node->AttachChild(ToAttach,1);




	_DMESSAGE("Attaching 2");
	return true;
}






VMArray<ConnectPointSUP> cmd_SUPGetConnectPoints(StaticFunctionTag* base, TESObjectREFR* refr)
{
	_DMESSAGE("SUPGetConnectPoints starts");
	VMArray<ConnectPointSUP> results;
	if (!refr)
		return results;
	_DMESSAGE("target refr->%x", refr->formID);
	NiNode* root = refr->GetObjectRootNode();
	if (!root)
		return results;

	BSConnectPoint::Parents* extraData = (BSConnectPoint::Parents*)Runtime_DynamicCast(root->GetExtraData("CPA"), RTTI_NiExtraData, RTTI_BSConnectPoint__Parents);
	if (extraData)
	{
		for (UInt32 i = 0; i < extraData->points.count; i++)
		{
			BSConnectPoint::Parents::ConnectPoint* connectPoint = extraData->points[i];
			if (connectPoint)
			{
				ConnectPointSUP point;

				point.Set<BSFixedString>("parent", connectPoint->parent);
				point.Set<BSFixedString>("name", connectPoint->name);

				float yaw, pitch, roll;
				connectPoint->rot.GetEulerAngles(roll, pitch, yaw);
				yaw *= 180.0 / MATH_PI;
				pitch *= 180.0 / MATH_PI;
				roll *= 180.0 / MATH_PI;

				point.Set<float>("roll", roll);
				point.Set<float>("pitch", pitch);
				point.Set<float>("yaw", yaw);

				NiPoint3 localPos = connectPoint->pos;



				NiPoint3 worldPos = localPos;
				NiAVObject* parent = root;

				worldPos = parent->m_worldTransform.rot.Transpose() * localPos + parent->m_worldTransform.pos;
				point.Set<float>("x", worldPos.x);
				point.Set<float>("y", worldPos.y);
				point.Set<float>("z", worldPos.z);

				point.Set<float>("scale", connectPoint->scale);

				point.Set<TESObjectREFR*>("object", nullptr);

				if (refr->parentCell) {
					bhkWorld* world = CALL_MEMBER_FN(refr->parentCell, GetHavokWorld)();
					if (world) {
						TESObjectREFR* connected = GetObjectAtConnectPoint(*refr, worldPos, *world, 8.0f);
						if (connected) {
							point.Set<TESObjectREFR*>("object", connected);
							_DMESSAGE("SUPGetConnectPoints, setting>>%x", connected->formID);
						}
					}
				}

				results.Push(&point);
			}
		}
	}
	_DMESSAGE("SUPGetConnectPoints end");
	return results;
}


float cmd_GetDistanceBetweenNodes(StaticFunctionTag* base, TESObjectREFR* refr, BSFixedString NodeNameOne, BSFixedString NodeNameTwo, UInt32 iPCRequest)
{

	_DMESSAGE("GetDistanceBetweenNodes started, node is %s", NodeNameOne.c_str());


	if (refr == NULL)
	{
		_DMESSAGE("reference doesn't exists");
		return -2;
	}

	NiAVObject* childone = ResolveNode(refr, NodeNameOne, iPCRequest);

	if (!childone)
	{
		_DMESSAGE("node one doesn't exists");
		return -1;
	}


	NiAVObject* childtwo = ResolveNode(refr, NodeNameTwo, iPCRequest);

	if (!childtwo)
	{
		_DMESSAGE("node two doesn't exists");
		return -1;
	}

	
	float fPosXOne = childone->m_worldTransform.pos.x;
	float fPosYOne = childone->m_worldTransform.pos.y;
	float fPosZOne = childone->m_worldTransform.pos.z;



	float fPosXTwo = childtwo->m_worldTransform.pos.x;
	float fPosYTwo = childtwo->m_worldTransform.pos.y;
	float fPosZTwo = childtwo->m_worldTransform.pos.z;


	return sqrt(pow(fPosXTwo - fPosXOne, 2) + pow(fPosYTwo - fPosYOne, 2) + pow(fPosZTwo - fPosZOne, 2) * 1.0);
}








SUPXYZ cmd_GetDistanceBetweenNodesEx(StaticFunctionTag* base, TESObjectREFR* refr, BSFixedString NodeNameOne, BSFixedString NodeNameTwo, UInt32 iPCRequest)
{

	_DMESSAGE("GetDistanceBetweenNodesEx started, node is %s", NodeNameOne.c_str());



	SUPXYZ result;

	if (refr == NULL)
	{
		_DMESSAGE("reference doesn't exists");
		result.Set<UInt32>("Success", -2);
		return result;
	}

	NiAVObject* childone = ResolveNode(refr, NodeNameOne, iPCRequest);

	if (!childone)
	{
		_DMESSAGE("node one doesn't exists");
		result.Set<UInt32>("Success", -1);
		return result;
	}


	NiAVObject* childtwo = ResolveNode(refr, NodeNameTwo, iPCRequest);

	if (!childtwo)
	{
		_DMESSAGE("node two doesn't exists");
		result.Set<UInt32>("Success", -1);
		return result;
	}


	float fPosXOne = childone->m_worldTransform.pos.x;
	float fPosYOne = childone->m_worldTransform.pos.y;
	float fPosZOne = childone->m_worldTransform.pos.z;


	float fPosXTwo = childtwo->m_worldTransform.pos.x;
	float fPosYTwo = childtwo->m_worldTransform.pos.y;
	float fPosZTwo = childtwo->m_worldTransform.pos.z;

	result.Set<UInt32>("Success", 1);
	result.Set<float>("x", fPosXTwo - fPosXOne);
	result.Set<float>("y", fPosYTwo - fPosYOne);
	result.Set<float>("z", fPosZTwo - fPosZOne);

	return result;
}










float cmd_GetDistanceBetweenNodesTwoRefs(StaticFunctionTag* base, TESObjectREFR* refrOne, BSFixedString NodeNameOne, UInt32 iPCRequestOne, TESObjectREFR* refrTwo, BSFixedString NodeNameTwo, UInt32 iPCRequestTwo)
{

	_DMESSAGE("GetDistanceBetweenNodes started, node is %s", NodeNameOne.c_str());


	if (refrOne == NULL)
	{
		_DMESSAGE("reference one doesn't exists");
		return -2;
	}

	NiAVObject* childone = ResolveNode(refrOne, NodeNameOne, iPCRequestOne);

	if (!childone)
	{
		_DMESSAGE("node one doesn't exists");
		return -1;
	}


	if (refrTwo == NULL)
	{
		_DMESSAGE("reference two doesn't exists");
		return -2;
	}


	NiAVObject* childtwo = ResolveNode(refrTwo, NodeNameTwo, iPCRequestTwo);

	if (!childtwo)
	{
		_DMESSAGE("node two doesn't exists");
		return -1;
	}


	float fPosXOne = childone->m_worldTransform.pos.x;
	float fPosYOne = childone->m_worldTransform.pos.y;
	float fPosZOne = childone->m_worldTransform.pos.z;



	float fPosXTwo = childtwo->m_worldTransform.pos.x;
	float fPosYTwo = childtwo->m_worldTransform.pos.y;
	float fPosZTwo = childtwo->m_worldTransform.pos.z;


	return sqrt(pow(fPosXTwo - fPosXOne, 2) + pow(fPosYTwo - fPosYOne, 2) + pow(fPosZTwo - fPosZOne, 2) * 1.0);
}




float cmd_GetDistanceBetweenNodeAndRef(StaticFunctionTag* base, TESObjectREFR* refr, BSFixedString NodeNameOne, TESObjectREFR* refrSecond, UInt32 iPCRequest)
{

	_DMESSAGE("GetDistanceBetweenNodeAndRef started, node is %s", NodeNameOne.c_str());


	if (refr == NULL)
	{
		_DMESSAGE("reference doesn't exists");
		return -2;
	}

	if (refrSecond == NULL)
	{
		_DMESSAGE("second reference doesn't exists");
		return -2;
	}

	
	NiAVObject* childone = ResolveNode(refr, NodeNameOne, iPCRequest);

	if (!childone)
	{
		_DMESSAGE("node one doesn't exists");
		return -1;
	}


	float fPosXOne = childone->m_worldTransform.pos.x;
	float fPosYOne = childone->m_worldTransform.pos.y;
	float fPosZOne = childone->m_worldTransform.pos.z;

	float fPosXTwo = refrSecond->pos.x;
	float fPosYTwo = refrSecond->pos.y;
	float fPosZTwo = refrSecond->pos.z;


	return sqrt(pow(fPosXTwo - fPosXOne, 2) + pow(fPosYTwo - fPosYOne, 2) + pow(fPosZTwo - fPosZOne, 2) * 1.0);
}





