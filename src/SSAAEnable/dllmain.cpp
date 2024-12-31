/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Helpers.h>

#define FASTCALL __fastcall

HOOK(void, FASTCALL, render_manager_init_data, 0x00000001404D77F0,
    int32_t ssaa, int32_t hd_res, int32_t ss_alpha_mask, bool npr, bool a5) {
    originalrender_manager_init_data(1, 0, ss_alpha_mask, npr, a5);
}

extern "C" __declspec(dllexport) void PreInit() {
    INSTALL_HOOK(render_manager_init_data);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    return TRUE;
}
