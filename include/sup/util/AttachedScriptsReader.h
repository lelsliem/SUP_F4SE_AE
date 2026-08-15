#include "sup/util/string.hpp"

// Phase 3: rewritten on CommonLibF4 APIs (FindBoundObject / GetObjectHandlePolicy).
// The F4SE deep internals (scriptsLock, m_attachedScripts, IdentifierItem) are not modeled
// by CommonLibF4; the two IsScriptLoaded* helpers now use the supported IVirtualMachine API.

namespace AttachedScriptReader {



	struct HandlerResult
	{
		bool bSuccess = false;
		UInt64 handle = 0;
	};

	bool IsScriptAttachedToHandle(UInt64 Handle, string sScriptName)
	{
		if (!g_MyVirtMachine || !g_MyVirtMachine->raw())
		{
			return false;
		}

		BSTSmartPointer<BSScript::Object> obj;
		return g_MyVirtMachine->raw()->FindBoundObject(Handle, sScriptName.c_str(), false, obj, false);
	}

	HandlerResult GetHandlerForForm(TESForm* CurrentObject)
	{
		HandlerResult result;

		if (!CurrentObject || !g_MyVirtMachine || !g_MyVirtMachine->raw())
		{
			return result;
		}

		auto& policy = g_MyVirtMachine->raw()->GetObjectHandlePolicy();

		result.handle = policy.GetHandleForObject(static_cast<std::uint32_t>(CurrentObject->GetFormType()), CurrentObject);

		if (result.handle == policy.EmptyHandle())
		{
			_DMESSAGE("invalid handle???");
			return result;
		}

		result.bSuccess = true;
		return result;
	}



	bool IsScriptLoaded(string Scriptname)
	{
		if (!g_MyVirtMachine || !g_MyVirtMachine->raw())
		{
			return false;
		}

		sup::string::to_lower(Scriptname);

		// F4SE checked the VM's loaded-scripts registry by name; the supported equivalent is
		// GetScriptObjectTypeNoLoad, which reports whether the script type is registered without
		// triggering a load (matching F4SE's "is it already loaded" semantics).
		BSTSmartPointer<BSScript::ObjectTypeInfo> objType;
		return g_MyVirtMachine->raw()->GetScriptObjectTypeNoLoad(BSFixedString(Scriptname.c_str()), objType);
	}


	HandlerResult IsScriptLoadedForForm(string ScriptName, TESForm* CurrentObject)
	{
		auto result = GetHandlerForForm(CurrentObject);

		if (!result.bSuccess)
		{
			return result;
		}

		result.bSuccess = IsScriptAttachedToHandle(result.handle, ScriptName);

		return result;
	}



	struct VMIdentifierResult
	{
		bool bSuccess = false;
		UInt64 handle = 0;
		VMIdentifier* VMIdent = NULL;
	};


	VMIdentifier* GetVMIdentifierForForm(string ScriptName, TESForm* CurrentObject)
	{
		auto result = GetHandlerForForm(CurrentObject);

		if (!result.bSuccess)
		{
			return NULL;
		}

		VMIdentifier* identifier = NULL;
		g_MyVirtMachine->GetObjectIdentifier(result.handle, ScriptName.c_str(), 0, &identifier, 0);

		return identifier;
	}


	// Legacy debug dumps of the F4SE attached-scripts map — no longer available; kept as no-ops.
	void GetHandleAndCheckForScript(UInt32 Handle, string Scriptname)
	{
		_DMESSAGE("GetHandleAndCheckForScript - transition no-op (handle=%x)", Handle);
	}

	void DumpALLScripts()
	{
		_DMESSAGE("DumpALLScripts - transition no-op");
	}


}




template <class T>
AttachedScriptReader::HandlerResult GetObjectHandleForEvent(T CurrentEvent) {

	AttachedScriptReader::HandlerResult result;

	TESForm* CurrentObject = BuildRefFormFromSTR(CurrentEvent->sFormID, CurrentEvent->sPluginName, 0);

	if (!CurrentObject)
	{
		_DMESSAGE("Cannot build form from event.");
		return result;
	}

	return AttachedScriptReader::IsScriptLoadedForForm(CurrentEvent->s_ScriptName, CurrentObject);
}