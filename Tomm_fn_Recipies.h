







VMArray<BGSConstructibleObject*> cmd_GetAllConstructibleObjectsFromForm(StaticFunctionTag* base, TESForm* ChosenForm) {

	_DMESSAGE("GetAllConstructibleObjectsFromForm started");
	VMArray<BGSConstructibleObject*> result;

	if (!ChosenForm)
	{
		_DMESSAGE("GetAllConstructibleObjectsFromForm:: form not valid");
		return result;
	}



	for (int i = 0; i < (*g_dataHandler)->arrCOBJ.count; i++)
	{
		//_DMESSAGE("New constr object");
		BGSConstructibleObject* CurrentObject = NULL;
		(*g_dataHandler)->arrCOBJ.GetNthItem(i, CurrentObject);



		if (CurrentObject) {


			if (CurrentObject->createdObject)
			{
				if (CurrentObject->createdObject->formType == kFormType_FLST)
				{
					BGSListForm* formList = (BGSListForm*)CurrentObject->createdObject;


					for (UInt32 i = 0; i < formList->forms.count; i++)
					{
						TESForm* form = nullptr;
						formList->forms.GetNthItem(i, form);
						if (form && form == ChosenForm)
						{
							//_DMESSAGE("EQUAL");
							result.Push(&CurrentObject);
							break;
						}

					}

				}
				else {
					if (CurrentObject->createdObject == ChosenForm)
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