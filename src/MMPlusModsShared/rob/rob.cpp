/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "rob.hpp"
#include "../../KKdLib/str_utils.hpp"
#include <Helpers.h>
#include <algorithm>

const mat4* (FASTCALL* rob_chara_bone_data_get_mats_mat)(rob_chara_bone_data* rob_bone_data, size_t index)
    = (const mat4 * (FASTCALL*)(rob_chara_bone_data* rob_bone_data, size_t index))0x000000014044A3E0;
const char* (FASTCALL* chara_index_get_auth_3d_name)(chara_index chara_index)
    = (const char* (FASTCALL*)(chara_index chara_index))0x00000001404DE4D0;
const mat4* (FASTCALL* sub_1404E22C0)(rob_chara* rob_chr)
    = (const mat4 * (FASTCALL*)(rob_chara * rob_chr))0x00000001404E22C0;
const mat4* (FASTCALL* rob_chara_get_bone_data_mat)(rob_chara* rob_chr, mot_bone_index index)
    = (const mat4 * (FASTCALL*)(rob_chara* rob_chr, mot_bone_index index))0x00000001404E2330;
size_t(FASTCALL* get_rob_chara_smth)() = (size_t(FASTCALL*)())0x00000001404EF0F0;
rob_chara* (FASTCALL* rob_chara_array_get)(size_t rob_chr_smth, int32_t chara_id)
    = (rob_chara * (FASTCALL*)(size_t rob_chr_smth, int32_t chara_id))0x00000001404EF3C0;
bool(FASTCALL* rob_chara_array_check_visibility)(size_t rob_chr_smth, int32_t chara_id)
    = (bool(FASTCALL*)(size_t rob_chr_smth, int32_t chara_id))0x00000001404EF4F0;
bool(FASTCALL* rob_chara_pv_data_array_check_chara_id)(size_t rob_chr_smth, int32_t chara_id)
    = (bool(FASTCALL*)(size_t rob_chr_smth, int32_t chara_id))0x00000001404EF4B0;
rob_chara_bone_data* (FASTCALL* rob_chara_array_get_bone_data)(size_t rob_chr_smth, int32_t chara_id)
    = (rob_chara_bone_data * (FASTCALL*)(size_t rob_chr_smth, int32_t chara_id))0x00000001404EF6A0;

rob_chara* rob_chara_array = (rob_chara*)0x000000014175D750;

const mat4* rob_chara_get_adjust_data_mat(rob_chara* rob_chr) {
    return &rob_chr->data.adjust_data.mat;
}
