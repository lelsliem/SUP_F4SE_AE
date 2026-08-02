#pragma once

// Stub implementation for steam_api.h
// Full Steam SDK would be needed for Steam integration features

#define S_API
#define STEAM_CALL

typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef int int32;
typedef short int16;
typedef char int8;

typedef uint64 SteamAPICall_t;
typedef uint32 HAuthTicket;
typedef uint64 CSteamID;

#define k_HAuthTicketInvalid 0

class ISteamUser
{
public:
    virtual CSteamID GetSteamID() { return 0; }
    virtual HAuthTicket GetAuthSessionTicket(void* pTicket, int cbMaxTicket, uint32* pcbTicket) { return k_HAuthTicketInvalid; }
};

class ISteamFriends
{
public:
    virtual const char* GetPersonaName() { return "Player"; }
};

class ISteamUtils
{
public:
    virtual uint32 GetSecondsSinceAppActive() { return 0; }
    virtual uint32 GetSecondsSinceComputerActive() { return 0; }
    virtual int GetConnectedUniverse() { return 0; }
    virtual uint32 GetServerRealTime() { return 0; }
    virtual const char* GetIPCountry() { return "US"; }
    virtual bool GetImageSize(int iImage, uint32* pnWidth, uint32* pnHeight) { return false; }
    virtual bool GetImageRGBA(int iImage, uint8* pubDest, int nDestBufferSize) { return false; }
    virtual bool GetCSERIPPort(uint32* unIP, uint16* usPort) { return false; }
    virtual uint8 GetCurrentBatteryPower() { return 255; }
    virtual uint32 GetAppID() { return 377160; } // Fallout 4 App ID
    virtual bool IsOverlayEnabled() { return false; }
    virtual bool BOverlayNeedsPresent() { return false; }
    virtual const char* GetSteamUILanguage() { return "english"; }
    virtual bool IsSteamRunningOnSteamDeck() { return false; }
};

class ISteamApps
{
public:
    virtual bool BIsSubscribed() { return true; }
    virtual bool BIsLowViolence() { return false; }
    virtual bool BIsCybercafe() { return false; }
    virtual bool BIsVACBanned() { return false; }
    virtual const char* GetCurrentGameLanguage() { return "english"; }
    virtual const char* GetAvailableGameLanguages() { return "english"; }
    virtual bool BIsSubscribedApp(uint32 appID) { return true; }
    virtual bool BIsDlcInstalled(uint32 appID) { return false; }
    virtual uint32 GetEarliestPurchaseUnixTime(uint32 appID) { return 0; }
    virtual bool BIsSubscribedFromFreeWeekend() { return false; }
    virtual int GetDLCCount() { return 0; }
    virtual bool BGetDLCDataByIndex(int iDLC, uint32* pAppID, bool* pbAvailable, char* pchName, int cchNameBufferSize) { return false; }
    virtual void InstallDLC(uint32 appID) {}
    virtual void UninstallDLC(uint32 appID) {}
};

class ISteamScreenshots
{
public:
    virtual uint32 WriteScreenshot(void* pubRGB, uint32 cubRGB, int nWidth, int nHeight) { return 0; }
    virtual uint32 AddScreenshotToLibrary(const char* pchFilename, const char* pchThumbnailFilename, int nWidth, int nHeight) { return 0; }
    virtual void TriggerScreenshot() {}
    virtual void HookScreenshots(bool bHook) {}
    virtual bool SetLocation(uint32 hScreenshot, const char* pchLocation) { return false; }
    virtual bool TagUser(uint32 hScreenshot, CSteamID steamID) { return false; }
    virtual bool TagPublishedFile(uint32 hScreenshot, uint64 unPublishedFileID) { return false; }
};

// Stub functions
inline bool SteamAPI_Init() { return false; }
inline void SteamAPI_Shutdown() {}
inline void SteamAPI_RunCallbacks() {}
inline ISteamUser* SteamUser() { return nullptr; }
inline ISteamFriends* SteamFriends() { return nullptr; }
