/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "wrap.hpp"
#include <Helpers.h>

struct dx_default_states_ext_struct {
    p_dx_rasterizer_state rasterizer_states[3];
    bool init;

    dx_default_states_ext_struct();
    ~dx_default_states_ext_struct();

    void create();
    void release();
};

dx_default_states_ext_struct dx_default_states_ext;

p_dx_rasterizer_state* dx_default_states_get_rasterizer_state_depth_clip(dx_cull_mode cull_mode, bool depth_clip) {
    if (depth_clip)
        return dx_default_states_get_rasterizer_state(cull_mode);
    return &dx_default_states_ext.rasterizer_states[cull_mode];
}

HOOK(void, FASTCALL, dx_default_states_struct__create, 0x00000001404CE610, __int64 This) {
    originaldx_default_states_struct__create(This);
    dx_default_states_ext.create();
}

HOOK(void, FASTCALL, dx_default_states_struct__release, 0x00000001579A7AB0, __int64 This) {
    dx_default_states_ext.release();
    originaldx_default_states_struct__release(This);
}

void dx_default_states_patch() {
    INSTALL_HOOK(dx_default_states_struct__create);
    INSTALL_HOOK(dx_default_states_struct__release);
}

dx_default_states_ext_struct::dx_default_states_ext_struct() : init() {

}

dx_default_states_ext_struct::~dx_default_states_ext_struct() {

}

void dx_default_states_ext_struct::create() {
    rasterizer_states[0].create(DX_CULL_NONE, false, false);
    rasterizer_states[1].create(DX_CULL_FRONT, false, false);
    rasterizer_states[2].create(DX_CULL_BACK, false, false);
    init = true;
}

void dx_default_states_ext_struct::release() {
    for (p_dx_rasterizer_state& i : rasterizer_states)
        i.release();
    init = false;
}
