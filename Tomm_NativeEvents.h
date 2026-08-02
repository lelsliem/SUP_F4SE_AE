
#include "F4SE_Compat.h"

// code copied from LL Four play plugin, originally from HUD++ plugin by kassent (on GitHub)

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

template <typename EventT>
class BSTEventDispatcher_SUP	// BSTGlobalEvent::EventSource<typename EventT> : public struct BSTSingletonSDM<EventT,BSTSingletonSDMOpStaticBuffer>, BSTEventSink<BSTGlobalEvent::KillSDMEvent>, BSTEventSource<ViewCasterUpdateEvent>
{
public:
	typedef BSTEventSink<EventT> SinkT;

	bool AddEventSink(SinkT* sink)
	{
		SimpleLocker locker(&lock);

		// Check for duplicate first
		for (int i = 0; i < eventSinks.count; i++)
		{
			if (eventSinks[i] == sink)
				return false;
		}

		if (!eventSinks.Insert(0, sink))
			eventSinks.Push(sink);
		return true;
	}

	void RemoveEventSink(SinkT* sink)
	{
		SimpleLocker locker(&lock);

		for (int i = 0; i < eventSinks.count; i++)
		{
			if (eventSinks[i] == sink) {
				eventSinks.Remove(i);
				break;
			}
		}
	}

	SimpleLock			lock;				// 000
	tArray<SinkT*>		eventSinks;			// 008
	tArray<SinkT*>		addBuffer;			// 020
	tArray<SinkT*>		removeBuffer;		// 038
	bool				stateFlag;			// 050
	char				pad[3];
};


typedef BSTEventDispatcher<ViewCasterUpdateEvent_SUP> EventDispatcher;
typedef BSTGlobalEvent::EventSource<ViewCasterUpdateEvent_SUP> EventSource;
// NOTE: g_Source address needs Address Library lookup for NG compatibility
// Crosshair handler is disabled until proper address is found
// RelocPtr<EventSource*> g_Source(0x05A5F758);
static EventSource* g_Source_Placeholder = nullptr;




TESObjectREFR* g_LastCrosshair = nullptr;

class CrosshairChangeHandler_SUP : public BSTEventSink<ViewCasterUpdateEvent_SUP>
{
public:
	virtual ~CrosshairChangeHandler_SUP() { };
	virtual	EventResult	ReceiveEvent(ViewCasterUpdateEvent_SUP* evn, void* dispatcher) override
	{
		//Console_Print("event received");

		if (!evn)
		{
			return kEvent_Continue;
		}

		NiPointer<TESObjectREFR> CurrentCrosshair;
		LookupREFRByHandle(evn->value.crosshairHandler, CurrentCrosshair);

		TESObjectREFR* NewCrosshair = CurrentCrosshair.get();

		if (OnCrosshairRefChangeEventHandler.HasEvents())
		{
			if (g_LastCrosshair != NewCrosshair) //necessary
			{
				if (g_LastCrosshair)
				{
					OnCrosshairRefChangeEventHandler.ProcessEventOnCrosshairRefChange(false, g_LastCrosshair, g_LastCrosshair->formType);
				}
				if (NewCrosshair)
				{
					OnCrosshairRefChangeEventHandler.ProcessEventOnCrosshairRefChange(true, NewCrosshair, NewCrosshair->formType);
				}
			}
		}

		g_LastCrosshair = NewCrosshair;
		return kEvent_Continue;
	}

	static void Register();
};

CrosshairChangeHandler_SUP g_CrosshairChangeHandler;


void CrosshairChangeHandler_SUP::Register()
{
	// NOTE: Crosshair handler disabled for NG - g_Source address not yet resolved via Address Library
	_MESSAGE("Crosshair handler disabled (NG compatibility - address not resolved)");
	g_LastCrosshair = NULL;
	return;

	// Original code - requires g_Source address resolution
	/*
	EventSource* e = (*g_Source);
	g_LastCrosshair = NULL;
	if (e != nullptr)
	{
		if (e->eventDispatcher.AddEventSink(&g_CrosshairChangeHandler))
			_DMESSAGE("Registered crosshair update handler.");
		g_LastCrosshair = 0;
	}
	else
		_MESSAGE("Can't register crosshair update handler...");
	*/
}


struct TESPlacementEvent {
	TESObjectREFR* WorkshopRef;
	TESObjectREFR* ItemRef;
};

DECLARE_EVENT_DISPATCHER(TESPlacementEvent, 0x22A020)
class SUP_PLacementEventSink : public BSTEventSink<TESPlacementEvent>
{
public:
	EventResult	ReceiveEvent(TESPlacementEvent* evn, void* dispatcher) {



		if (bWirelessPowerFix)
		{
			if (evn->ItemRef && evn->WorkshopRef)
			{
				Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(evn->WorkshopRef); 

				if (!WorkshopData)
				{
					_DMESSAGE("Placement event - no workshop data.");
					return kEvent_Continue;
				}

				bool bPowered = WorkshopCompat::IsPowered(WorkshopData, evn->ItemRef);
				_DMESSAGE("Placement event calls RadiatorFixProcessPowerOnOffEvent >>%d", bPowered);
				RadiatorFixProcessPowerOnOffEvent(bPowered, evn->ItemRef, evn->WorkshopRef, WorkshopData);
			}
		}


		return kEvent_Continue;
	}
};
SUP_PLacementEventSink g_SUPPLacementEvent;







//__int64 *`anonymous namespace'::QSource<TESActivateEvent>() 140441C90

struct TESActivateEvent {
	TESObjectREFR* Activator;
	TESObjectREFR* Activation;
};

DECLARE_EVENT_DISPATCHER(TESActivateEvent, 0x441DE0)  // Updated for NG (Address Library ID: 2197774)
class SUP_ActivatetEventSink : public BSTEventSink<TESActivateEvent>
{
public:
	EventResult	ReceiveEvent(TESActivateEvent* evn, void* dispatcher) {

		
		if (bTermLinkFix)
		{
			if (evn->Activation && evn->Activation->formID == 0x14)
			{
				if (evn->Activator && evn->Activator->baseForm && evn->Activator->baseForm->formType == kFormType_TERM)
				{
					//if ((*g_ui) != NULL)
					//{
					//	Console_Print("(*g_ui)->numPauseGame>>%d", (*g_ui)->numPauseGame);
					//}
					//Console_Print("PLAYER activated terminal.");
					ReestablishTerminalLinks(evn->Activator);
				}
			}
		}


		return kEvent_Continue;
	}
};
SUP_ActivatetEventSink g_SUPActivateEvent;







//struct TESPowerOnEvent {
//	TESObjectREFR* ItemRef;
//};
//
//DECLARE_EVENT_DISPATCHER(TESPowerOnEvent, 0x22A480) // __int64 *Workshop::QSource<Workshop::PowerOnEvent>()
//class SUP_PowerOnEventSink : public BSTEventSink<TESPowerOnEvent>
//{
//public:
//	EventResult	ReceiveEvent(TESPowerOnEvent* evn, void* dispatcher) {
//
//		Console_Print("POWER ON EVENT STARTS");
//
//		if (bWirelessPowerFix)
//		{
//			if (evn->ItemRef)
//			{
//
//					
//				TESObjectREFR* workshopRef = GetLinkedRef_Native(evn->ItemRef, g_SUPWorkshopKeyword);
//
//				if (!workshopRef)
//				{
//					return kEvent_Continue;
//				}
//
//				Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(workshopRef);
//
//				if (!WorkshopData)
//				{
//					return kEvent_Continue;
//				}
//			
//				Console_Print("POWER ON EVENT FOR>>> %x", evn->ItemRef->formID);
//				RadiatorFixProcessPowerOnOffEvent(true, evn->ItemRef, workshopRef, WorkshopData);
//			}
//		}
//
//
//		return kEvent_Continue;
//	}
//};
//
//SUP_PowerOnEventSink g_SUPPowerOnEvent;
//
//
//
//
//struct TESPowerOffEvent {
//	TESObjectREFR* ItemRef;
//};
//
//DECLARE_EVENT_DISPATCHER(TESPowerOffEvent, 0x22A3E0)
//class SUP_PowerOffEventSink : public BSTEventSink<TESPowerOffEvent>
//{
//public:
//	EventResult	ReceiveEvent(TESPowerOffEvent* evn, void* dispatcher) {
//
//		Console_Print("POWER OFF EVENT STARTS");
//
//		if (bWirelessPowerFix)
//		{
//			if (evn->ItemRef)
//			{
//
//
//				TESObjectREFR* workshopRef = GetLinkedRef_Native(evn->ItemRef, g_SUPWorkshopKeyword);
//
//				if (!workshopRef)
//				{
//					return kEvent_Continue;
//				}
//
//				Workshop::ExtraData* WorkshopData = f_GetPowerGridExtraData(workshopRef);
//
//				if (!WorkshopData)
//				{
//					return kEvent_Continue;
//				}
//
//				Console_Print("POWER OFF EVENT FOR>>> %x", evn->ItemRef->formID);
//				RadiatorFixProcessPowerOnOffEvent(false, evn->ItemRef, workshopRef, WorkshopData);
//			}
//		}
//
//
//		return kEvent_Continue;
//	}
//};
//
//SUP_PowerOffEventSink g_SUPPowerOffEvent;