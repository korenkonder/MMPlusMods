/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include <Windows.h>
#include <stdint.h>
#include "detours.h"
#include "Helpers.h"
#include "SigScan.h"

SIG_SCAN(
    sigDofPvSetEnable,
    0x14049F700,
    "\x88\x15\xE2\xD8\x78\x0C\xC3",
    "xxxxxxx",
    "\x88\x15\xA2\xD8\x78\x0C\xC3",
    "xxxxxxx",
);

HOOK(void, __fastcall, DofPvSetEnable, sigDofPvSetEnable(), __int64 a1, __int8 a2) {
    originalDofPvSetEnable(a1, false);
}

extern "C" __declspec(dllexport) void PreInit() {
    if (sigDofPvSetEnableValid)
        INSTALL_HOOK(DofPvSetEnable);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    return TRUE;
}
