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
const char* (FASTCALL* chara_index_get_chara_name)(chara_index chara_index)
    = (const char* (FASTCALL*)(chara_index chara_index))0x00000001404DE490;
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

SkinParam::CollisionParam::CollisionParam() : type(), node_idx(), pos() {
    radius = 0.2f;
}

void RobOsage::SetAirRes(float_t air_res) {
    skin_param_ptr->air_res = air_res;
}

void RobOsage::SetColiR(float_t coli_r) {
    RobOsageNode* i_begin = nodes.data() + 1;
    RobOsageNode* i_end = nodes.data() + nodes.size();
    for (RobOsageNode* i = i_begin; i != i_end; i++)
        i->data_ptr->skp_osg_node.coli_r = coli_r;
}

void RobOsage::SetForce(float_t force, float_t force_gain) {
    skin_param_ptr->force = force;
    skin_param_ptr->force_gain = force_gain;
    RobOsageNode* i_begin = nodes.data() + 1;
    RobOsageNode* i_end = nodes.data() + nodes.size();
    for (RobOsageNode* i = i_begin; i != i_end; i++) {
        i->data_ptr->force = force;
        force = force * force_gain;
    }
}

void RobOsage::SetHinge(float_t hinge_y, float_t hinge_z) {
    hinge_y = min_def(hinge_y, 179.0f);
    hinge_z = min_def(hinge_z, 179.0f);
    hinge_y = hinge_y * DEG_TO_RAD_FLOAT;
    hinge_z = hinge_z * DEG_TO_RAD_FLOAT;
    RobOsageNode* i_begin = nodes.data() + 1;
    RobOsageNode* i_end = nodes.data() + nodes.size();
    for (RobOsageNode* i = i_begin; i != i_end; i++) {
        RobOsageNodeData* data = i->data_ptr;
        data->skp_osg_node.hinge.ymin = -hinge_y;
        data->skp_osg_node.hinge.ymax = hinge_y;
        data->skp_osg_node.hinge.zmin = -hinge_z;
        data->skp_osg_node.hinge.zmax = hinge_z;
    }
}

void RobOsage::SetInitRot(float_t init_rot_y, float_t init_rot_z) {
    skin_param_ptr->init_rot.y = init_rot_y * DEG_TO_RAD_FLOAT;
    skin_param_ptr->init_rot.z = init_rot_z * DEG_TO_RAD_FLOAT;
}

void RobOsage::SetMotionResetData(uint32_t motion_id, float_t frame) {
    osage_reset = true;
    auto elem = motion_reset_data.find({ motion_id, (int32_t)prj::roundf(frame * 1000.0f) });
    if (elem != motion_reset_data.end() && elem->second.size() + 1 == nodes.size())
        reset_data_list = &elem->second;
}

// 0x1404815F0
void RobOsage::SetNodesExternalForce(const vec3* external_force, const float_t& strength) {
    if (!external_force) {
        RobOsageNode* i_begin = nodes.data() + 1;
        RobOsageNode* i_end = nodes.data() + nodes.size();
        for (RobOsageNode* i = i_begin; i != i_end; i++)
            i->external_force = 0.0f;
        return;
    }

    vec3 _external_force = *external_force;
    for (size_t i = osage_setting.exf; i; i--)
        _external_force *= strength;

    RobOsageNode* i_begin = nodes.data() + 1;
    RobOsageNode* i_end = nodes.data() + nodes.size();
    for (RobOsageNode* i = i_begin; i != i_end; i++) {
        i->external_force = _external_force;
        _external_force *= strength;
    }
}

// 0x1404818D0
void RobOsage::SetNodesForce(const float_t& force) {
    RobOsageNode* i_begin = nodes.data() + 1;
    RobOsageNode* i_end = nodes.data() + nodes.size();
    for (RobOsageNode* i = i_begin; i != i_end; i++)
        i->force = force;
}

void RobOsage::SetRot(float_t rot_y, float_t rot_z) {
    skin_param_ptr->rot.y = rot_y * DEG_TO_RAD_FLOAT;
    skin_param_ptr->rot.z = rot_z * DEG_TO_RAD_FLOAT;
}

rob_chara_item_equip_object* rob_chara_item_equip::get_item_equip_object(item_id id) {
    if (id >= ITEM_BODY && id <= ITEM_ITEM16)
        return &item_equip_object[id];
    return 0;
}

const mat4* rob_chara_get_adjust_data_mat(rob_chara* rob_chr) {
    return &rob_chr->data.adjust_data.mat;
}
