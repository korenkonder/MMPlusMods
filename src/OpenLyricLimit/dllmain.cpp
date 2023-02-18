/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include <Windows.h>
#include <stdint.h>
#include "Helpers.h"
#include "SigScan.h"

SIG_SCAN(
    sigFindLyricCount,
    0x1404B1CF9,
    "\x48\xFF\xC7\x48\x81\xFF\x96\x00\x00\x00\x0F\x82\xD7\xFE\xFF\xFF",
    "xxxxxxxxxxxx????"
);

extern "C" __declspec(dllexport) void PreInit() {
    WRITE_MEMORY((size_t)sigFindLyricCount() + 0x06, uint32_t, 1000);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    return TRUE;
}
