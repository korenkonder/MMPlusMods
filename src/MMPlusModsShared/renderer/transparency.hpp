/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../../KKdLib/default.hpp"
#include "../wrap.hpp"

namespace renderer {
    struct Transparency {
        p_dx_render_target render_target;
        p_dx_texture texture;
        dx_vertex_pixel_shader shader;
        p_dx_sampler_state sampler_state;
        p_dx_buffer buffer;
        p_dx_blend_state blend_state;
        p_dx_rasterizer_state rasterizer_state;
        p_dx_depth_stencil_state depth_stencil_state;
    };
}

static_assert(sizeof(renderer::Transparency) == 0x48, "\"renderer::Transparency\" struct should have a size of 0x48");
