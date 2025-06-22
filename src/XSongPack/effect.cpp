/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "effect.hpp"
#include "auth_3d.hpp"
#include <Helpers.h>

static void TaskEffectAuth3D__SetVisibility(size_t This, bool value);

HOOK(void, FASTCALL, TaskEffectAuth3D__SetEnable, 0x00000001403DE5F0, size_t This, bool value) {
    *(bool*)(This + 0x118) = value;
    TaskEffectAuth3D__SetVisibility(This, value);
}

HOOK(void, FASTCALL, TaskEffectAuth3D__SetCurrentStageIndex, 0x00000001403DE920, size_t This, int32_t stage_index) {
    if (*(int32_t*)(This + 0x138) != stage_index) {
        TaskEffectAuth3D__SetVisibility(This, 0);
        *(int32_t*)(This + 0x138) = stage_index;
        TaskEffectAuth3D__SetVisibility(This, *(bool*)(This + 0x118));
    }
}

void effect_patch() {
    INSTALL_HOOK(TaskEffectAuth3D__SetEnable);
    INSTALL_HOOK(TaskEffectAuth3D__SetCurrentStageIndex);
}

static void TaskEffectAuth3D__SetVisibility(size_t This, bool value) {
    struct struc_621 {
        int32_t stage_index;
        prj::vector<auth_3d_id> auth_3d_ids;
    };

    int32_t current_stage_index = *(int32_t*)(This + 0x138);
    if (current_stage_index == -1)
        return;

    for (struc_621& i : *(prj::vector<struc_621>*)(This + 0x120)) {
        if (i.stage_index != current_stage_index)
            continue;

        extern bool reflect_full;
        for (auth_3d_id& j : i.auth_3d_ids) {
            j.set_visibility(value);
            j.set_reflect(reflect_full);
        }
        break;
    }
}
