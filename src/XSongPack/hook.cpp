/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "hook.hpp"
#include "Glitter/glitter.hpp"
#include "mdl/disp_manager.hpp"
#include "pv_game/pv_game.hpp"
#include "rob/rob.hpp"
#include "auth_3d.hpp"
#include "camera.hpp"
#include "effect.hpp"
#include "object.hpp"
#include "render_manager.hpp"
#include "sss.hpp"
#include "stage.hpp"
#include "wrap.hpp"
#include <Helpers.h>

HOOK(int32_t, FASTCALL, data_init, 0x00000001402B6300) {
    auth_3d_patch();
    camera_patch();
    dx_default_states_patch();
    effect_patch();
    mdl::disp_manager_patch();
    object_patch();
    pv_game_patch();
    render_manager_patch();
    rob_patch();
    sss_patch();
    stage_patch();

    reflect_full_init();
    int32_t res = originaldata_init();
    Glitter::Init();
    pv_game_init();
    return res;
}

HOOK(int32_t, FASTCALL, data_free, 0x00000001402B6AC0) {
    pv_game_free();
    Glitter::Free();
    int32_t res = originaldata_free();
    reflect_full_free();
    return res;
}

void hook_funcs() {
    INSTALL_HOOK(data_init);
    INSTALL_HOOK(data_free);
}
