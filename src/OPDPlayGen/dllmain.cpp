/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "rob/rob.hpp"
#include "file_handler.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tomlc99/toml.h>

extern bool opd_play_gen_use_cache;

bool cpu_caps_aes_ni;

void FASTCALL opd_play_gen_main() {
    int32_t cpuid_data[4] = {};
    __cpuid(cpuid_data, 1);
    cpu_caps_aes_ni = (cpuid_data[2] & (1 << 25)) ? true : false;

    printf("[OPD Play Gen] Patching memory\n");
    rob_patch();
    file_handler_patch();
}

extern "C" __declspec(dllexport) void PreInit() {
    FILE* file = fopen("config.toml", "r");
    toml_table_t* config = toml_parse_file(file, 0, 0);
    fclose(file);

    opd_play_gen_use_cache = false;
    if (config) {
        toml_raw_t use_cache = toml_raw_in(config, "use_cache");
        int32_t ret = 0;
        if (!toml_rtob(use_cache, &ret))
            opd_play_gen_use_cache = !!ret;
    }
    toml_free(config);

    opd_play_gen_main();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    return TRUE;
}
