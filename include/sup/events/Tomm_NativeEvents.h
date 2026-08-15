// code copied from LL Four play plugin, originally from HUD++ plugin by kassent (on GitHub)
// Phase 3: rewritten on CommonLibF4 event plumbing (RE::BSTEventSink / BSTEventSource).
// The F4SE-internal crosshair/placement event sources are not exposed by CommonLibF4, so
// those registrations are logged transition stubs; TESActivateEvent registers via
// RE::TESActivateEvent::GetEventSource() and stays functional.

struct ViewCasterUpdateEvent_SUP
{
	struct ViewCasterValueEvent_SUP
	{
		UInt32			crosshairHandler;	// 00	RefHandler
		UInt32			unk04;				// 04
		UInt32			unk08;				// 08
		UInt32			unk0C;				// 0C
		UInt64			unk10;				// 10
		UInt64			unk18;				// 18
		UInt32			unk20;				// 20
		UInt32			unk24;				// 24
	};
	ViewCasterValueEvent_SUP value;			// 00
	bool					unk28;			// 28
	UInt32					unk2C;			// 2C
	bool					isNPC;			// 30 ?
	float					unk34;			// 34
	bool					canInteractive;	// 38
};
STATIC_ASSERT(sizeof(ViewCasterUpdateEvent_SUP) == 0x40);

TESObjectREFR* g_LastCrosshair = nullptr;

// F4SE subscribed to the game-internal ViewCasterUpdateEvent source (a raw game global,
// g_Source at 0x05A5F758 in the 1.10.163-era binary). CommonLibF4 does not model that event
// source, so the handler compiles against RE::BSTEventSink but is not registered (the
// F4SE-internal source has no CommonLibF4 equivalent — see MODERNIZATION_PLAN.md §5.5).
class CrosshairChangeHandler_SUP : public BSTEventSink<ViewCasterUpdateEvent_SUP>
{
public:
	virtual ~CrosshairChangeHandler_SUP() = default;
	virtual BSEventNotifyControl ProcessEvent(const ViewCasterUpdateEvent_SUP& evn, BSTEventSource<ViewCasterUpdateEvent_SUP>*) override
	{
		if (!evn.value.crosshairHandler)
		{
			return BSEventNotifyControl::kContinue;
		}

		NiPointer<TESObjectREFR> CurrentCrosshair;
		LookupREFRByHandle(evn.value.crosshairHandler, CurrentCrosshair);

		TESObjectREFR* NewCrosshair = CurrentCrosshair.get();

		if (OnCrosshairRefChangeEventHandler.HasEvents())
		{
			if (g_LastCrosshair != NewCrosshair) //necessary
			{
				if (g_LastCrosshair)
				{
					OnCrosshairRefChangeEventHandler.ProcessEventOnCrosshairRefChange(false, g_LastCrosshair, (UInt32)g_LastCrosshair->formType.underlying());
				}
				if (NewCrosshair)
				{
					OnCrosshairRefChangeEventHandler.ProcessEventOnCrosshairRefChange(true, NewCrosshair, (UInt32)NewCrosshair->formType.underlying());
				}
			}
		}

		g_LastCrosshair = NewCrosshair;
		return BSEventNotifyControl::kContinue;
	}

	static void Register();
};

CrosshairChangeHandler_SUP g_CrosshairChangeHandler;


void CrosshairChangeHandler_SUP::Register()
{
	// The F4SE-internal ViewCasterUpdateEvent source is not available on CommonLibF4, so
	// the crosshair-ref-change events are deliberately disabled (documented in
	// MODERNIZATION_PLAN.md §5.5). Logged once at startup.
	_DMESSAGE("CrosshairChangeHandler: F4SE-internal crosshair event source not available on CommonLibF4 - disabled");
	g_LastCrosshair = NULL;
}





class SUP_ActivatetEventSink : public BSTEventSink<RE::TESActivateEvent>
{
public:
	virtual ~SUP_ActivatetEventSink() = default;
	virtual BSEventNotifyControl ProcessEvent(const RE::TESActivateEvent& evn, BSTEventSource<RE::TESActivateEvent>*) override
	{
		if (bTermLinkFix)
		{
			// F4SE member names: Activation (what was activated) / Activator (who activated)
			// -> RE: objectActivated / actionRef.
			if (evn.objectActivated && evn.objectActivated->formID == 0x14)
			{
				if (evn.actionRef && evn.actionRef->GetObjectReference() && evn.actionRef->GetObjectReference()->formType == kFormType_TERM)
				{
					ReestablishTerminalLinks(evn.actionRef.get());
				}
			}
		}

		return BSEventNotifyControl::kContinue;
	}
};
SUP_ActivatetEventSink g_SUPActivateEvent;
