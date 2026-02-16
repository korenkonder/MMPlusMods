/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include <stdint.h>
#include "../MMPlusModsShared/types.hpp"
#include "Helpers.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tomlc99/toml.h>

const uint32_t* opd_motion_ids = (uint32_t*)0x0000000140DB9170;
const uint32_t* opd_motion_set_ids = (uint32_t*)0x0000000140DB9160;

static void add_ids(prj::vector<uint32_t>& ids, const uint32_t* ptr);
static const uint32_t* copy_ids(const prj::vector<uint32_t>& ids, uint32_t* ptr);
static void parse_toml(prj::vector<uint32_t>& mot_ids, prj::vector<uint32_t>& mot_set_ids, const char* path);

HOOK(const uint32_t*, FASTCALL, get_opd_motion_ids, 0x000000014047BF20) {
    return opd_motion_ids;
}

HOOK(const uint32_t*, FASTCALL, get_opd_motion_set_ids, 0x000000014047BF30) {
    return opd_motion_set_ids;
}

// Convenient location after DivaModLoader added all "mod_" prefixes
HOOK(void, FASTCALL, pv_weight_db_init, 0x000000014061D5A0) {
    static size_t(FASTCALL * mdata_manager_get)() = (size_t(FASTCALL*)())0x000000014043E040;
    static const prj::list<prj::string>& (*MdataMgr__GetPrefixes)(size_t This)
        = (const prj::list<prj::string>&(*)(size_t This))0x000000014043F8A0;

    prj::vector<uint32_t> mot_ids;
    prj::vector<uint32_t> mot_set_ids;
    for (const prj::string& i : MdataMgr__GetPrefixes(mdata_manager_get())) {
        prj::string path("rom/rob/");
        path.append(i);
        path.append("OPDPlus.toml");

        parse_toml(mot_ids, mot_set_ids, path.c_str());
    }

    if (mot_ids.size()) {
        add_ids(mot_ids, opd_motion_ids);

        uint32_t* _opd_motion_ids = (uint32_t*)malloc(sizeof(uint32_t) * (mot_ids.size() + 1));
        if (_opd_motion_ids) {
            opd_motion_ids = copy_ids(mot_ids, _opd_motion_ids);

            INSTALL_HOOK(get_opd_motion_ids);
        }
    }

    if (mot_set_ids.size()) {
        add_ids(mot_set_ids, opd_motion_set_ids);

        uint32_t* _opd_motion_set_ids = (uint32_t*)malloc(sizeof(uint32_t) * (mot_set_ids.size() + 1));
        if (_opd_motion_set_ids) {
            opd_motion_set_ids = copy_ids(mot_set_ids, _opd_motion_set_ids);

            INSTALL_HOOK(get_opd_motion_set_ids);
        }
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        INSTALL_HOOK(pv_weight_db_init);
        break;
    }
    return TRUE;
}

static void add_ids(prj::vector<uint32_t>& ids, const uint32_t* ptr) {
    size_t count = 0;
    while (ptr[count] != -1)
        count++;

    ids.insert(ids.end(), ptr, ptr + count);
    std::stable_sort(ids.begin(), ids.end());
    ids.erase(std::unique(ids.begin(), ids.end()), ids.end());
}

static const uint32_t* copy_ids(const prj::vector<uint32_t>& ids, uint32_t* ptr) {
    memmove(ptr, ids.data(), sizeof(uint32_t) * ids.size());
    ptr[ids.size()] = -1;
    return ptr;
}

static void parse_toml(prj::vector<uint32_t>& mot_ids, prj::vector<uint32_t>& mot_set_ids, const char* path) {
    prj::string _path(path);
    static bool(FASTCALL * path_get_file_first_mount_path)(prj::string* src, prj::string* dst)
        = (bool(FASTCALL*)(prj::string * src, prj::string * dst))0x00000001402A5330;
    if (!path_get_file_first_mount_path(&_path, &_path))
        return;

    FILE* file = fopen(_path.c_str(), "r");
    if (!file)
        return;

    toml_table_t* config = toml_parse_file(file, 0, 0);
    fclose(file);

    if (!config)
        return;

    toml_array_t* _mot_ids = toml_array_in(config, "mot_ids");
    if (_mot_ids)
        for (int32_t i = 0; ; i++) {
            toml_datum_t mot_id = toml_int_at(_mot_ids, i);
            if (mot_id.ok)
                mot_ids.push_back((uint32_t)mot_id.u.i);
            else
                break;
        }

    toml_array_t* _mot_set_ids = toml_array_in(config, "mot_set_ids");
    if (_mot_set_ids)
        for (int32_t i = 0; ; i++) {
            toml_datum_t mot_set_id = toml_int_at(_mot_set_ids, i);
            if (mot_set_id.ok)
                mot_set_ids.push_back((uint32_t)mot_set_id.u.i);
            else
                break;
        }
    toml_free(config);
}
