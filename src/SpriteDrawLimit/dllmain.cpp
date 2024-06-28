/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "sprite.hpp"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern "C" __declspec(dllexport) void PreInit() {
    sprite_draw_limit_patch();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    return TRUE;
}
