







VMArray<BGSConstructibleObject*> cmd_GetAllConstructibleObjectsFromForm(StaticFunctionTag* base, TESForm* ChosenForm) {

	_DMESSAGE("GetAllConstructibleObjectsFromForm started");
	VMArray<BGSConstructibleObject*> result;

	if (!ChosenForm)
	{
		_DMESSAGE("GetAllConstructibleObjectsFromForm:: form not valid");
		return result;
	}



	auto& AllCOBJs = (*g_dataHandler)->GetFormArray<BGSConstructibleObject>();

	for (UInt32 i = 0; i < AllCOBJs.size(); i++)
	{
		//_DMESSAGE("New constr object");
		BGSConstructibleObject* CurrentObject = AllCOBJs[i];



		if (CurrentObject) {


			if (CurrentObject->GetCreatedItem())
			{
				if (CurrentObject->GetCreatedItem()->formType == kFormType_FLST)
				{
					BGSListForm* formList = (BGSListForm*)CurrentObject->GetCreatedItem();


					for (UInt32 i = 0; i < formList->arrayOfForms.size(); i++)
					{
						TESForm* form = nullptr;
						form = formList->arrayOfForms[i];
						if (form && form == ChosenForm)
						{
							//_DMESSAGE("EQUAL");
							result.Push(&CurrentObject);
							break;
						}

					}

				}
				else {
					if (CurrentObject->GetCreatedItem() == ChosenForm)
					{
						//_DMESSAGE("EQUAL");
						result.Push(&CurrentObject);
					}
				}


			}


		}
			
			


	}

	_DMESSAGE("GetAllConstructibleObjectsFromForm ended");
	return result;

}