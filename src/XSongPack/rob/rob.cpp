/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "rob.hpp"
#include "../../KKdLib/str_utils.hpp"
#include "../mdl/disp_manager.hpp"
#include <Helpers.h>
#include <algorithm>

struct mothead_data {
    mothead_data_type type;
    int32_t frame;
    void* data;
};

static_assert(sizeof(mothead_data) == 0x10, "\"mothead_data\" struct should have a size of 0x10");

struct mothead_func_data {
    rob_chara* rob_chr;
    rob_chara_data* rob_chr_data;
    rob_chara_data* field_10;
    rob_chara* field_18;
    rob_chara_data* field_20;
    struc_223* field_28;
};

static_assert(sizeof(mothead_func_data) == 0x30, "\"mothead_func_data\" struct should have a size of 0x30");

struct rob_chara_item_adjust_x {
    mat4 mat;
    vec3 pos;
    float_t scale;

    rob_chara_item_adjust_x();

    void reset();
};

struct rob_chara_arm_adjust_x {
    float_t next_value;
    float_t prev_value;
    int32_t start_frame;
    float_t duration;
    float_t scale;

    rob_chara_arm_adjust_x();

    void reset();
};

rob_chara_item_adjust_x rob_chara_item_adjust_x_array[ROB_CHARA_COUNT];
rob_chara_arm_adjust_x rob_chara_arm_adjust_x_array[ROB_CHARA_COUNT];

const mat4* rob_chara_get_item_adjust_data_mat(rob_chara* rob_chr) {
    return &rob_chara_item_adjust_x_array[rob_chr - rob_chara_array].mat;
}

HOOK(void, FASTCALL, sub_1404DD2C0, 0x00000001404DD2C0, rob_chara* rob_chr) {
    rob_chara_item_adjust_x& item_adjust = rob_chara_item_adjust_x_array[rob_chr - rob_chara_array];
    rob_chara_arm_adjust_x& arm_adjust = rob_chara_arm_adjust_x_array[rob_chr - rob_chara_array];
    if (arm_adjust.duration > 0) {
        float_t blend = 1.0f;
        if (fabsf(arm_adjust.duration) > 0.000001f)
            blend = (rob_chr->data.motion.frame_data.frame
                - (float_t)arm_adjust.start_frame) / arm_adjust.duration;

        blend = clamp_def(blend, 0.0f, 1.0f);
        if (fabsf(blend - 1.0f) <= 0.000001f)
            arm_adjust.duration = -1.0f;
        arm_adjust.scale = lerp_def(arm_adjust.prev_value, arm_adjust.next_value, blend);

        static float_t(FASTCALL * chara_size_table_get_value)(uint32_t id)
            = (float_t(FASTCALL*)(uint32_t id))0x00000001404E0750;

        float_t default_scale = chara_size_table_get_value(1);
        item_adjust.scale = default_scale + (rob_chr->data.adjust_data.scale
            - default_scale) * arm_adjust.scale;
    }

    originalsub_1404DD2C0(rob_chr);
}

static void rob_chara_data_adjuct_set_pos(rob_chara_adjust_data* rob_chr_adj,
    rob_chara_item_adjust_x* rob_chr_itm_adj_x, const vec3& pos, bool pos_adjust, const vec3* global_position) {
    float_t scale = rob_chr_adj->scale;
    float_t item_scale = rob_chr_itm_adj_x->scale;
    vec3 _offset = rob_chr_adj->offset;
    if (global_position)
        _offset.y += global_position->y;

    vec3 _pos = pos;
    vec3 _item_pos = pos;
    if (rob_chr_adj->height_adjust) {
        _pos.y += rob_chr_adj->pos_adjust_y;
        _item_pos.y += rob_chr_adj->pos_adjust_y;
    }
    else {
        vec3 temp = (_pos - _offset) * scale + _offset;
        vec3 arm_temp = (_item_pos - _offset) * item_scale + _offset;

        if (!rob_chr_adj->offset_x) {
            _pos.x = temp.x;
            _item_pos.x = arm_temp.x;
        }

        if (!rob_chr_adj->offset_y) {
            _pos.y = temp.y;
            _item_pos.y = arm_temp.y;
        }

        if (!rob_chr_adj->offset_z) {
            _pos.z = temp.z;
            _item_pos.z = arm_temp.z;
        }
    }

    if (pos_adjust) {
        _pos = rob_chr_adj->pos_adjust + _pos;
        _item_pos = rob_chr_adj->pos_adjust + _item_pos;
    }

    rob_chr_adj->pos = _pos - pos * scale;
    rob_chr_itm_adj_x->pos = _item_pos - pos * item_scale;
}

HOOK(void, FASTCALL, rob_chara_set_data_adjust_mat, 0x00000001404DDC20,
    rob_chara* rob_chr, rob_chara_adjust_data* rob_chr_adj, bool pos_adjust) {
    mat4 mat = *rob_chara_bone_data_get_mats_mat(rob_chr->bone_data, ROB_BONE_N_HARA_CP);

    vec3 pos;
    mat4_transpose(&mat, &mat);
    mat4_get_translation(&mat, &pos);

    rob_chara_item_adjust_x* item_adjust = &rob_chara_item_adjust_x_array[rob_chr - rob_chara_array];

    vec3* global_position = 0;
    if (rob_chr_adj->get_global_position) {
        static vec3* (FASTCALL * rob_chara_bone_data_get_global_position)(size_t rob_bone_data)
            = (vec3 * (FASTCALL*)(size_t rob_bone_data))0x0000000140449EB0;

        global_position = rob_chara_bone_data_get_global_position((size_t)rob_chr->bone_data);
    }
    rob_chara_data_adjuct_set_pos(rob_chr_adj, item_adjust, pos, pos_adjust, global_position);

    float_t scale = rob_chr_adj->scale;
    mat4_scale(scale, scale, scale, &rob_chr_adj->mat);
    mat4_set_translation(&rob_chr_adj->mat, &rob_chr_adj->pos);
    mat4_transpose(&rob_chr_adj->mat, &rob_chr_adj->mat);

    float_t item_scale = item_adjust->scale;
    mat4_scale(item_scale, item_scale, item_scale, &item_adjust->mat);
    mat4_set_translation(&item_adjust->mat, &item_adjust->pos);
    mat4_transpose(&item_adjust->mat, &item_adjust->mat);
}

HOOK(void, FASTCALL, rob_chara_item_equip_disp, 0x00000001404E0D40, rob_chara_item_equip* rob_item_equip, int32_t chara_id) {
    extern bool reflect_full;
    mdl::obj_reflect_enable = reflect_full;
    originalrob_chara_item_equip_disp(rob_item_equip, chara_id);
    mdl::obj_reflect_enable = false;
}

HOOK(void, FASTCALL, rob_chara_reset_data, 0x0000000157C0C2A0, rob_chara* rob_chr, rob_chara_pv_data* pv_data) {
    rob_chara_item_adjust_x& item_adjust = rob_chara_item_adjust_x_array[rob_chr - rob_chara_array];
    rob_chara_arm_adjust_x& arm_adjust = rob_chara_arm_adjust_x_array[rob_chr - rob_chara_array];

    item_adjust.reset();
    arm_adjust.reset();
    originalrob_chara_reset_data(rob_chr, pv_data);
    item_adjust.scale = rob_chr->data.adjust_data.scale;
}

HOOK(void, FASTCALL, rob_chara_set_chara_size, 0x00000001404E2460, rob_chara* rob_chr, float_t value) {
    originalrob_chara_set_chara_size(rob_chr, value);
    rob_chara_item_adjust_x& item_adjust = rob_chara_item_adjust_x_array[rob_chr - rob_chara_array];
    item_adjust.scale = value;
}

HOOK(void, FASTCALL, sub_1588E2160, 0x0000001588E2160, struc_223* a1) {
    originalsub_1588E2160(a1);
    rob_chara* rob_chr = (rob_chara*)((size_t)a1 - 0x19C8);
    rob_chara_arm_adjust_x_array[rob_chr - rob_chara_array].reset();
}

HOOK(void, FASTCALL, mothead_func_32, 0x00000001404F07C0, mothead_func_data* func_data,
    void* data, const mothead_data* mhd_data, int64_t frame) {
    float_t v8 = (float_t)((int16_t*)data)[0];
    int32_t v5 = ((int32_t*)data)[1];
    float_t v9 = ((float_t*)data)[2];
    int32_t v10 = ((int32_t*)data)[3];

    if (v8 != (int16_t)0xFA0C || v10 != 0xD62721C5) { // X
        originalmothead_func_32(func_data, data, mhd_data, frame);
        return;
    }

    rob_chara* rob_chr = func_data->rob_chr;

    float_t value = v9;
    float_t duration = (float_t)v5;
    rob_chara_arm_adjust_x& arm_adjust = rob_chara_arm_adjust_x_array[rob_chr - rob_chara_array];
    arm_adjust.next_value = value;
    arm_adjust.prev_value = arm_adjust.scale;
    arm_adjust.start_frame = (int32_t)frame;
    arm_adjust.duration = max_def(duration, 0.0f);
    return;
}

HOOK(bool, FASTCALL, sub_1404F46A0, 0x00000001404F46A0, rob_chara* rob_chr, int32_t hand) {
    if (hand >= 2 || !rob_chr->data.motion.hand_adjust[hand].enable)
        return false;

    rob_chara_item_adjust_x& item_adjust = rob_chara_item_adjust_x_array[rob_chr - rob_chara_array];
    rob_chara_item_adjust_x item_adjust_temp = item_adjust;
    float_t chara_scale = rob_chr->data.adjust_data.scale;
    float_t adjust_scale = rob_chr->data.motion.hand_adjust[hand].current_scale;
    item_adjust.scale = adjust_scale / chara_scale;
    originalsub_1404F46A0(rob_chr, hand);
    item_adjust = item_adjust_temp;
    return true;
}

HOOK(void, FASTCALL, rob_chara_set_hand_adjust, 0x00000001404F3D60, rob_chara* rob_chr,
    rob_chara_data_hand_adjust* adjust, rob_chara_data_hand_adjust* prev_adjust) {
    if (!adjust->enable)
        return;

    static float_t(FASTCALL * chara_size_table_get_value)(uint32_t id)
        = (float_t(FASTCALL*)(uint32_t id))0x00000001404E0750;
    static float_t(FASTCALL * rob_chara_array_get_data_adjust_scale)(size_t rob_chr_smth, int32_t chara_id)
        = (float_t(FASTCALL*)(size_t rob_chr_smth, int32_t chara_id))0x00000001404EF600;

    float_t chara_scale = rob_chr->data.adjust_data.scale;
    float_t opposite_chara_scale = rob_chara_array_get_data_adjust_scale(
        get_rob_chara_smth(), rob_chr - rob_chara_array ? 0 : 1);
    bool chara_opposite_chara_same = fabsf(chara_scale - opposite_chara_scale) <= 0.000001f;

    switch (adjust->type) {
    case ROB_CHARA_DATA_HAND_ADJUST_NORMAL:
        adjust->scale = chara_size_table_get_value(1);
        break;
    case ROB_CHARA_DATA_HAND_ADJUST_SHORT:
        adjust->scale = chara_size_table_get_value(2);
        break;
    case ROB_CHARA_DATA_HAND_ADJUST_TALL:
        adjust->scale = chara_size_table_get_value(0);
        break;
    case ROB_CHARA_DATA_HAND_ADJUST_MIN:
        adjust->scale = min_def(opposite_chara_scale, chara_scale);
        break;
    case ROB_CHARA_DATA_HAND_ADJUST_MAX:
        adjust->scale = max_def(opposite_chara_scale, chara_scale);
        break;
    case ROB_CHARA_DATA_HAND_ADJUST_OPPOSITE_CHARA:
        adjust->scale = opposite_chara_scale;
        break;
    case ROB_CHARA_DATA_HAND_ADJUST_1P:
        adjust->scale = rob_chara_array_get_data_adjust_scale(get_rob_chara_smth(), 0);
        break;
    case ROB_CHARA_DATA_HAND_ADJUST_2P:
        adjust->scale = rob_chara_array_get_data_adjust_scale(get_rob_chara_smth(), 1);
        break;
    case ROB_CHARA_DATA_HAND_ADJUST_3P:
        adjust->scale = rob_chara_array_get_data_adjust_scale(get_rob_chara_smth(), 2);
        break;
    case ROB_CHARA_DATA_HAND_ADJUST_4P:
        adjust->scale = rob_chara_array_get_data_adjust_scale(get_rob_chara_smth(), 3);
        break;
    case ROB_CHARA_DATA_HAND_ADJUST_ITEM: // X
        adjust->scale = rob_chara_item_adjust_x_array[rob_chr - rob_chara_array].scale;
        break;
    }

    if (adjust->scale_select == 1 && adjust->enable_scale && chara_opposite_chara_same) {
        adjust->scale_select = 0;
        adjust->rotation_blend = prev_adjust->rotation_blend;
        adjust->disable_x = prev_adjust->disable_x;
        adjust->disable_y = prev_adjust->disable_y;
        adjust->disable_z = prev_adjust->disable_z;
        adjust->scale_blend = 1.0f;
    }

    float_t prev_scale;
    if (prev_adjust->enable || prev_adjust->scale_select == 1)
        prev_scale = prev_adjust->current_scale;
    else
        prev_scale = chara_scale;

    float_t scale;
    if (adjust->scale_select == 1)
        scale = adjust->scale;
    else
        scale = chara_scale;

    if (fabsf(prev_scale - scale * adjust->scale_blend) <= 0.000001f || adjust->duration <= adjust->current_time
        || fabsf(adjust->duration - adjust->current_time) <= 0.000001f) {
        adjust->current_scale = lerp_def(prev_scale, scale, adjust->scale_blend);
        if (fabsf(adjust->current_scale - chara_scale) <= 0.000001f
            && adjust->type != ROB_CHARA_DATA_HAND_ADJUST_ITEM) {
            adjust->current_scale = chara_scale;
            adjust->enable = false;
        }
    }
    else {
        float_t t = (adjust->current_time + 1.0f) / (adjust->duration + 1.0f);
        adjust->current_scale = lerp_def(prev_scale, scale, t * adjust->scale_blend);
        adjust->current_time += rob_chr->data.motion.step_data.frame;
    }
}

void rob_patch() {
    INSTALL_HOOK(rob_chara_set_data_adjust_mat);
    INSTALL_HOOK(rob_chara_item_equip_disp);
    INSTALL_HOOK(rob_chara_reset_data);
    INSTALL_HOOK(sub_1404DD2C0);
    INSTALL_HOOK(rob_chara_set_chara_size);
    INSTALL_HOOK(sub_1588E2160);
    INSTALL_HOOK(mothead_func_32);
    INSTALL_HOOK(sub_1404F46A0);
    INSTALL_HOOK(rob_chara_set_hand_adjust);
}

rob_chara_item_adjust_x::rob_chara_item_adjust_x() : scale() {
    reset();
}

void rob_chara_item_adjust_x::reset() {
    mat4_translate(&pos, &mat);
    mat4_transpose(&mat, &mat);
    scale = 1.0f;
}

rob_chara_arm_adjust_x::rob_chara_arm_adjust_x() : next_value(),
prev_value(), start_frame(), duration(), scale() {
    reset();
}

void rob_chara_arm_adjust_x::reset() {
    next_value = 0.0f;
    prev_value = 0.0f;
    start_frame = -1;
    duration = -1.0f;
    scale = 1.0f;
}
