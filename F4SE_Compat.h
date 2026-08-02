#pragma once

// F4SE 0.7.7 compatibility layer
// Stubs for methods that don't exist in the new F4SE version

#include "f4se/GameWorkshop.h"
#include "f4se/GameReferences.h"
#include "f4se/GameForms.h"

// TESFaction is only forward-declared in F4SE 0.7.7
// We need full definition for Papyrus VMArray support (needs kTypeID member)
// Form type 14 (FACT) from GameForms.h
class TESFaction : public TESForm {
public:
    enum { kTypeID = kFormType_FACT };
};

// Workshop::ExtraData compatibility stubs
namespace WorkshopCompat {

inline void CalculateCapacityAndLoad(Workshop::ExtraData* data) {
    // Stub - functionality not available in F4SE 0.7.7
    _DMESSAGE("WorkshopCompat::CalculateCapacityAndLoad - stub called");
}

inline bool IsPowered(Workshop::ExtraData* data, TESObjectREFR* ref) {
    // Stub - functionality not available in F4SE 0.7.7
    return false;
}

inline void RemoveConnection(Workshop::ExtraData* data, TESObjectREFR* ref1, TESObjectREFR* ref2, TESObjectREFR* wireRef = nullptr, bool cleanUp = false) {
    // Stub - functionality not available in F4SE 0.7.7
    _DMESSAGE("WorkshopCompat::RemoveConnection - stub called");
}

inline void CleanUpEmptyPowerGrids(Workshop::ExtraData* data) {
    // Stub - functionality not available in F4SE 0.7.7
    _DMESSAGE("WorkshopCompat::CleanUpEmptyPowerGrids - stub called");
}

inline bool AreDirectlyConnected(Workshop::ExtraData* data, TESObjectREFR* ref1, TESObjectREFR* ref2) {
    // Stub - functionality not available in F4SE 0.7.7
    return false;
}

inline void RemoveItem(Workshop::ExtraData* data, TESObjectREFR* ref) {
    // Stub - functionality not available in F4SE 0.7.7
    _DMESSAGE("WorkshopCompat::RemoveItem - stub called");
}

} // namespace WorkshopCompat

// BGSLocation compatibility
inline bool GetLocationEverCleared(BGSLocation* loc) {
    // Stub - bEverCleared not available in F4SE 0.7.7
    return false;
}

// PlayerCharacter map marker compatibility
inline void SetPlayerMapMarkerCompat(PlayerCharacter* player, NiPoint3* coords, TESWorldSpace* worldSpace) {
    // Stub - SetPlayerMapMarker not available in F4SE 0.7.7
    _DMESSAGE("SetPlayerMapMarkerCompat - stub called");
}

inline void RemovePlayerMapMarkerCompat(PlayerCharacter* player) {
    // Stub - RemovePlayerMapMarker not available in F4SE 0.7.7
    _DMESSAGE("RemovePlayerMapMarkerCompat - stub called");
}

// GetActorByHandle compatibility
inline void GetActorByHandle_Execute(UInt32* handle, Actor** outActor) {
    // Stub - GetActorByHandle not available in F4SE 0.7.7
    if (outActor) *outActor = nullptr;
}

// Actor method stubs
inline bool ActorIsInAir(Actor* actor) {
    // Stub - IsInAir not available in F4SE 0.7.7
    return false;
}

inline bool ActorCanBeKnockedDown(Actor* actor) {
    // Stub - CanBeKnockedDown not available in F4SE 0.7.7
    return false;
}

// Workshop functions
inline void WorkshopDeleteWorkshopItem(TESObjectREFR* ref, bool unk = false) {
    // Use Address Library resolved address
    if (fn_WorkShop_DeleteWorkshopItem) {
        typedef void(*_WorkshopDeleteWorkshopItem)(TESObjectREFR* akRef);
        reinterpret_cast<_WorkshopDeleteWorkshopItem>(fn_WorkShop_DeleteWorkshopItem)(ref);
    } else {
        _DMESSAGE("WorkshopDeleteWorkshopItem - address not resolved");
    }
}

// TESObjectREFR method stubs
inline NiPoint3 GetStartingLocation(TESObjectREFR* ref) {
    // Stub - GetStartingLocation not available in F4SE 0.7.7
    // Return the current position as a fallback
    if (ref) {
        return ref->pos;
    }
    return NiPoint3();
}

inline NiPoint3 GetStartingAngle(TESObjectREFR* ref) {
    // Stub - GetStartingAngle not available in F4SE 0.7.7
    // Return the current rotation as a fallback
    if (ref) {
        return ref->rot;
    }
    return NiPoint3();
}

// Actor method stubs
inline bool ActorIsInFaction(Actor* actor, TESFaction* faction) {
    // Stub - IsInFaction not available in F4SE 0.7.7
    return false;
}

inline UInt32 ActorJump(Actor* actor, float arg = -3.4028235e38f) {
    // Stub - Jump not available in F4SE 0.7.7
    return 0;
}

inline bool ActorIsSwimming(Actor* actor) {
    // Stub - charController access not available in F4SE 0.7.7
    // The internal structures have changed
    return false;
}
