/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "auth_3d.hpp"
#include "../KKdLib/hash.hpp"
#include "../MMPlusModsShared/canonical_properties.hpp"
#include "mdl/disp_manager.hpp"
#include "pv_game/firstread.hpp"
#include "rob/rob.hpp"
#include "object.hpp"
#include <Helpers.h>

enum auth_3d_material_list_flags {
    AUTH_3D_MATERIAL_LIST_BLEND_COLOR = 0x01,
    AUTH_3D_MATERIAL_LIST_EMISSION    = 0x02,
};

struct auth_3d_material_list {
    auth_3d_material_list_flags flags;
    auth_3d_rgba blend_color;
    auth_3d_rgba emission;
    uint64_t hash;
};

struct struc_8 {
    float_t max_frame;
    void* data;
    size_t data_size;
    bool field_18;
    CanonicalProperties CanProp;
    int32_t field_60;
    int32_t field_64;
    int64_t field_68;
    bool field_70;
    void* binary_data;
    size_t binary_size;
    bool field_88;
};

static_assert(sizeof(struc_8) == 0x90, "\"struc_8\" struct should have a size of 0x90");

static void (FASTCALL* auth_3d_rgba__ctrl)(auth_3d_rgba* rgba, float_t frame)
    = (void (FASTCALL*)(auth_3d_rgba * rgba, float_t frame))0x00000001402D2930;
static bool (FASTCALL* auth_3d_key__parse)(auth_3d_key* k, struc_8* a2, prj::string_range_capacity str_rng_cap)
    = (bool (FASTCALL*)(auth_3d_key * k, struc_8 * a2, prj::string_range_capacity str_rng_cap))0x00000001402D7F10;
static bool (FASTCALL* auth_3d_rgba__parse)(auth_3d_rgba* rgba, struc_8* a2, prj::string_range_capacity str_rng_cap)
    = (bool (FASTCALL*)(auth_3d_rgba * rgba, struc_8 * a2, prj::string_range_capacity str_rng_cap))0x00000001402CDE30;

static void auth_3d_material_list_ctrl(auth_3d_material_list* ml, float_t frame);
static bool auth_3d_material_list_parse(auth_3d_material_list* ml, struc_8* a2, prj::string_range_capacity str_rng_cap);
static void auth_3d_set_material_list(auth_3d* auth);

void auth_3d_object__pre_disp_mid_impl(mat4& mat, auth_3d* auth, rob_chara* rob_chr) {
    mat4 m;
    mat4_mul(auth->chara_item
        ? rob_chara_get_item_adjust_data_mat(rob_chr)
        : rob_chara_get_adjust_data_mat(rob_chr),
        sub_1404E22C0(rob_chr), &m);
    mat4_mul(&m, &mat, &mat);
}

void auth_3d_object_hrc__pre_disp_mid_impl(auth_3d_object_hrc* oh, auth_3d* auth, rob_chara* rob_chr) {
    mat4_mul(auth->chara_item
        ? rob_chara_get_item_adjust_data_mat(rob_chr)
        : rob_chara_get_adjust_data_mat(rob_chr),
        sub_1404E22C0(rob_chr), &oh->disp_data.mat);
}

void auth_3d_curve_free_mid_impl(size_t c) {
    auth_3d_curve* _c = (auth_3d_curve*)c;
    static void(FASTCALL * auth_3d_key___auth_3d_key)(void*) = (void(FASTCALL*)(void*))0x0000000140258160;
    if (_c->type == 1) {
        auth_3d_material_list* ml = (auth_3d_material_list*)_c->curve.keys;
        auth_3d_key___auth_3d_key(&ml->emission.a);
        auth_3d_key___auth_3d_key(&ml->emission.g);
        auth_3d_key___auth_3d_key(&ml->emission.b);
        auth_3d_key___auth_3d_key(&ml->emission.r);
        auth_3d_key___auth_3d_key(&ml->blend_color.a);
        auth_3d_key___auth_3d_key(&ml->blend_color.g);
        auth_3d_key___auth_3d_key(&ml->blend_color.b);
        auth_3d_key___auth_3d_key(&ml->blend_color.r);
        _operator_delete(ml);
    }
    else
        auth_3d_key___auth_3d_key(&_c->curve.keys_vec);
}

HOOK(void, FASTCALL, auth_3d_curve__ctrl, 0x00000001402CFE50, auth_3d_curve* c, float_t frame) {
    if (c->type == 1)
        auth_3d_material_list_ctrl((auth_3d_material_list*)c->curve.keys, frame);
    else
        originalauth_3d_curve__ctrl(c, frame);
}

HOOK(bool, FASTCALL, auth_3d_curve_parse, 0x00000001402D83D0,
    auth_3d_curve* c, struc_8* a2, prj::string_range_capacity str_rng_cap) {
    prj::string_range_capacity name = prj::string_range_capacity(str_rng_cap, ".name");
    c->name.assign(a2->CanProp.GetValueString(name.range));

    prj::string_range_capacity ml_str = prj::string_range_capacity(str_rng_cap, ".ml");
    if (a2->CanProp.GetValue(ml_str.range)) {
        auth_3d_material_list* ml = (auth_3d_material_list*)_operator_new(sizeof(auth_3d_material_list));
        memset(ml, 0, sizeof(auth_3d_material_list));
        auth_3d_material_list_parse(ml, a2, ml_str);
        ml->hash = hash_fnv1a64m(c->name.data(), c->name.size());
        c->curve.keys = (kft3*)ml;
        c->type = 1;
    }
    else {
        prj::string_range_capacity cv = prj::string_range_capacity(str_rng_cap, ".cv");
        auth_3d_key__parse(&c->curve, a2, cv);
        c->type = 0;
    }
    return true;
}

HOOK(void, FASTCALL, auth_3d_parse, 0x00000001402D7200, auth_3d* auth) {
    const int32_t state_before = auth->state;
    originalauth_3d_parse(auth);
    const int32_t state_after = auth->state;
    extern bool reflect_full;
    if (!reflect_full || state_before != 1 || state_after != 2)
        return;

    extern const firstread* firstread_ptr;
    if (!firstread_ptr || !firstread_ptr->auth_3d_array)
        return;

    const firstread_auth_3d_array* auth_3d_array = firstread_ptr->auth_3d_array;

    for (uint32_t i = 0; i < auth_3d_array->num_auth_3d; i++) {
        if (auth_3d_array->auth_3d_array[i].uid != auth->uid)
            continue;

        const firstread_auth_3d* frg_auth_3d = &auth_3d_array->auth_3d_array[i];

        auto obj_begin = auth->object.begin();
        auto obj_end = auth->object.end();
        const uint32_t num_object = frg_auth_3d->num_object;
        for (uint32_t j = 0; j < num_object; j++) {
            const uint64_t name_hash = hash_utf8_xxh3_64bits(frg_auth_3d->object_array[j].name);
            for (auth_3d_object& k : auth->object) {
                if (name_hash != hash_utf8_xxh3_64bits(k.name.c_str()))
                    continue;

                const firstread_auth_3d_object* frg_a3d_obj = &frg_auth_3d->object_array[j];
                k.uid_name.assign(frg_a3d_obj->uid_name);
                k.object_info = object_database_get_object_info(frg_a3d_obj->uid_name);

                k.reflect = k.uid_name.find("_REFLECT") != -1;
                k.refract = k.uid_name.find("_REFRACT") != -1;
                break;
            }
        }

        auto obj_list_begin = auth->object_list.begin();
        auto obj_list_end = auth->object_list.end();
        const uint32_t num_object_list = frg_auth_3d->num_object_list;
        for (uint32_t j = 0; j < num_object_list; j++) {
            const uint64_t name_hash = hash_utf8_xxh3_64bits(frg_auth_3d->object_list_array[j].name);
            for (auto obj_list = obj_list_begin; obj_list != obj_list_end; )
                if (name_hash == hash_utf8_xxh3_64bits((*obj_list)->name.c_str())) {
                    auth->object_list.erase(obj_list);
                    obj_list_end = auth->object_list.end();
                    break;
                }
                else
                    obj_list++;
        }
        break;
    }
}

HOOK(void, FASTCALL, auth_3d_object__pre_disp_mid, 0x00000001402DEFFE);
HOOK(void, FASTCALL, auth_3d_object_hrc__pre_disp_mid, 0x00000001402DFB20);

HOOK(void, FASTCALL, auth_3d__disp, 0x0000001402E0AE0, auth_3d* auth) {
    if (auth->state != 2 || !auth->enable || auth->bone_mats)
        return;

    auth_3d_set_material_list(auth);
    extern bool reflect_full;
    mdl::obj_reflect_enable = reflect_full && auth->reflect;
    originalauth_3d__disp(auth);
    mdl::obj_reflect_enable = false;
    disp_manager->set_material_list();
}

HOOK(void, FASTCALL, auth_3d_curve_free_mid, 0x00000001402E4130);

void auth_3d_patch() {
    // Move both auth_3d_curve's value and type
    WRITE_MEMORY_STRING(0x00000001402E9766, "\x4B\x8B\x44\x1A\x38\x49\x89\x42\x38", 0x09);
    // Null both auth_3d_curve's value and type
    WRITE_MEMORY_STRING(0x00000001402ED974, "\x74\x5E", 0x02); // Fix jump
    WRITE_MEMORY_STRING(0x00000001402ED9BD, "\x48\x89\x48\x38\x48\x83\xC2\x70"
        "\x48\x89\x54\x24\x30\x48\x8D\x40\x70\x49\x83\xE8\x01\x75\xAC\x4C\x8B"
        "\xCF\x4D\x8B\x46\x08\x49\x8B\x16\xE8\xAD\xBC\xFF\xFF", 0x26);
    // Move both auth_3d_curve's value and type
    WRITE_MEMORY_STRING(0x00000001402EC415, "\x48\x89\x41\x38"
        "\x48\x8D\x49\x70\x49\x83\xE8\x01\x75\xBD\x48\x8B\xC2\xC3", 0x12);
    // Null auth_3d's shadow, chara_item and reflect
    WRITE_MEMORY_STRING(0x00000001402D6B46, "\x44\x89\xB3\x98\x00\x00\x00", 0x07);

    // NOP anything that will call rob_chara_get_adjust_data_mat
    WRITE_NOP_5(0x00000001402DEFFE);
    WRITE_MEMORY_STRING(0x00000001402DF003, "\xEB\x35", 0x02);
    WRITE_NOP(0x00000001402DF005, 0x35);

    WRITE_NOP_5(0x00000001402DFB20);
    WRITE_MEMORY_STRING(0x00000001402DFB25, "\xEB\x15", 0x02);
    WRITE_NOP(0x00000001402DFB27, 0x15);

    INSTALL_HOOK(auth_3d_curve__ctrl);
    INSTALL_HOOK(auth_3d_curve_parse);
    INSTALL_HOOK(auth_3d_parse);
    INSTALL_HOOK(auth_3d__disp);
    INSTALL_HOOK(auth_3d_object__pre_disp_mid);
    INSTALL_HOOK(auth_3d_object_hrc__pre_disp_mid);
    INSTALL_HOOK(auth_3d_curve_free_mid);
}

static void auth_3d_material_list_ctrl(auth_3d_material_list* ml, float_t frame) {
    auth_3d_rgba__ctrl(&ml->blend_color, frame);
    auth_3d_rgba__ctrl(&ml->emission, frame);
}

static bool auth_3d_material_list_parse(auth_3d_material_list* ml, struc_8* a2, prj::string_range_capacity str_rng_cap) {
    ml->flags = (auth_3d_material_list_flags)0;
    prj::string_range_capacity blend_color = prj::string_range_capacity(str_rng_cap, ".blend_color");
    if (a2->CanProp.GetValue(blend_color.range)) {
        auth_3d_rgba__parse(&ml->blend_color, a2, blend_color);
        enum_or(ml->flags, AUTH_3D_MATERIAL_LIST_BLEND_COLOR);
    }

    prj::string_range_capacity emission = prj::string_range_capacity(str_rng_cap, ".emission");
    if (a2->CanProp.GetValue(emission.range)) {
        auth_3d_rgba__parse(&ml->emission, a2, emission);
        enum_or(ml->flags, AUTH_3D_MATERIAL_LIST_EMISSION);
    }
    return true;
}

static void auth_3d_set_material_list(auth_3d* auth) {
    int32_t mat_list_count = 0;
    material_list_struct mat_list[MATERIAL_LIST_COUNT];
    for (auth_3d_curve& i : auth->curve) {
        if (i.type != 1)
            continue;

        auth_3d_material_list* ml = (auth_3d_material_list*)i.curve.keys;
        if (!ml->flags)
            continue;

        if (ml->flags & AUTH_3D_MATERIAL_LIST_BLEND_COLOR) {
            vec4& blend_color = mat_list[mat_list_count].blend_color;
            vec4u8& has_blend_color = mat_list[mat_list_count].has_blend_color;

            blend_color = ml->blend_color.value;
            has_blend_color.x = ml->blend_color.has_got[0];
            has_blend_color.y = ml->blend_color.has_got[1];
            has_blend_color.z = ml->blend_color.has_got[2];
            has_blend_color.w = ml->blend_color.has_got[3];
        }
        else {
            mat_list[mat_list_count].blend_color = {};
            mat_list[mat_list_count].has_blend_color = {};
        }

        if (ml->flags & AUTH_3D_MATERIAL_LIST_EMISSION) {
            vec4& emission = mat_list[mat_list_count].emission;
            vec4u8& has_emission = mat_list[mat_list_count].has_emission;

            emission = ml->emission.value;
            has_emission.x = ml->emission.has_got[0];
            has_emission.y = ml->emission.has_got[1];
            has_emission.z = ml->emission.has_got[2];
            has_emission.w = ml->emission.has_got[3];
        }
        else {
            mat_list[mat_list_count].emission = {};
            mat_list[mat_list_count].has_emission = {};
        }

        mat_list[mat_list_count].hash = ml->hash;
        mat_list_count++;
    }

    disp_manager->set_material_list(mat_list_count, mat_list);
}
