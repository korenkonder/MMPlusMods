/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "../KKdLib/default.hpp"
#include "hook.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <intrin.h>
#include <tomlc99/toml.h>

bool cpu_caps_aes_ni;
bool cpu_caps_f16c;
bool config_reflect_full;

extern "C" __declspec(dllexport) void PreInit() {
    FILE* file = fopen("config.toml", "r");
    toml_table_t* config = toml_parse_file(file, 0, 0);
    fclose(file);

    config_reflect_full = false;
    if (config) {
        toml_raw_t reflect_full = toml_raw_in(config, "reflect_full");
        int32_t ret = 0;
        if (!toml_rtob(reflect_full, &ret))
            config_reflect_full = !!ret;
    }
    toml_free(config);
}

void x_song_pack_main() {
    int32_t cpuid_data[4] = {};
    __cpuid(cpuid_data, 1);
    cpu_caps_aes_ni = (cpuid_data[2] & (1 << 25)) ? true : false;
    cpu_caps_f16c = (cpuid_data[2] & (1 << 29)) ? true : false;

    hook_funcs();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        x_song_pack_main();
        break;
    }
    return TRUE;
}
