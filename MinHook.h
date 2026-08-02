#pragma once

// Stub implementation for MinHook
// Full MinHook library would be needed for hooking functionality

#define MH_OK 0
#define MH_ERROR_NOT_INITIALIZED 1

typedef int MH_STATUS;

inline MH_STATUS MH_Initialize() { return MH_OK; }
inline MH_STATUS MH_Uninitialize() { return MH_OK; }
inline MH_STATUS MH_CreateHook(void* pTarget, void* pDetour, void** ppOriginal) {
    if (ppOriginal) *ppOriginal = pTarget;
    return MH_OK;
}
inline MH_STATUS MH_EnableHook(void* pTarget) { return MH_OK; }
inline MH_STATUS MH_DisableHook(void* pTarget) { return MH_OK; }
inline MH_STATUS MH_RemoveHook(void* pTarget) { return MH_OK; }

#define MH_ALL_HOOKS nullptr
