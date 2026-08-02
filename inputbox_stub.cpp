
#include "inputbox.h"
#include <windows.h>
#include <string>
#include <vector>

// Stub implementation - returns empty string
// Full implementation requires ATL (atlbase.h) to be installed

static std::string g_stubResult;

char* InputBox(char* Prompt, char* Title, char* Default) {
    // Simple MessageBox-based fallback - just shows the prompt and returns default
    MessageBoxA(NULL, Prompt, Title, MB_OK);
    g_stubResult = Default ? Default : "";
    return (char*)g_stubResult.c_str();
}

char* PasswordBox(char* Prompt, char* Title, char* Default) {
    // Simple MessageBox-based fallback
    MessageBoxA(NULL, Prompt, Title, MB_OK);
    g_stubResult = Default ? Default : "";
    return (char*)g_stubResult.c_str();
}
