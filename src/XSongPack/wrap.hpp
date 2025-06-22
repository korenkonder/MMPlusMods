/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../MMPlusModsShared/wrap.hpp"

extern p_dx_rasterizer_state* dx_default_states_get_rasterizer_state_depth_clip(dx_cull_mode cull_mode, bool depth_clip = true);

extern void dx_default_states_patch();
