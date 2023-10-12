/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include <Windows.h>
#include <stdint.h>
#include "Helpers.h"
#include <tomlc99/toml.h>

int32_t* opd_motion_set_ids = (int32_t*)0x140DB9160;

const int32_t* get_opd_motion_set_ids() {
    return opd_motion_set_ids;
}

extern "C" __declspec(dllexport) void PreInit() {
    FILE* file = fopen("config.toml", "r");
    toml_table_t* config = toml_parse_file(file, 0, 0);
    fclose(file);

    if (config) {
        toml_array_t* mot_set_ids = toml_array_in(config, "mot_set_ids");
        size_t count = 0;
        if (mot_set_ids)
            for (int32_t i = 0; ; i++) {
                toml_datum_t mot_set_id = toml_int_at(mot_set_ids, i);
                if (mot_set_id.ok)
                    count++;
                else
                    break;
            }

        if (mot_set_ids && count) {
            size_t org_count = 0;
            while (opd_motion_set_ids[org_count] != -1)
                org_count++;

            int32_t* _opd_motion_set_ids = (int32_t*)malloc(sizeof(int32_t) * (org_count + count + 1));
            if (_opd_motion_set_ids) {
                memmove(_opd_motion_set_ids, opd_motion_set_ids, sizeof(int32_t) * org_count);
                opd_motion_set_ids = _opd_motion_set_ids;
                _opd_motion_set_ids += org_count;

                for (int32_t i = 0; i < count; i++) {
                    toml_datum_t mot_set_id = toml_int_at(mot_set_ids, i);
                    if (mot_set_id.ok)
                        *_opd_motion_set_ids++ = (int32_t)mot_set_id.u.i;
                }
                *_opd_motion_set_ids++ = -1;

                uint8_t buf[12];
                buf[0] = 0x48;
                buf[1] = 0xB8;
                buf[10] = 0xFF;
                buf[11] = 0xE0;

                uint64_t get_opd_motion_set_ids_func_addr = (uint64_t)&get_opd_motion_set_ids;
                memcpy(&buf[2], &get_opd_motion_set_ids_func_addr, sizeof(uint64_t));

                WRITE_MEMORY_STRING(0x14047BF30, buf, sizeof(buf));
            }
        }
    }
    toml_free(config);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    return TRUE;
}
