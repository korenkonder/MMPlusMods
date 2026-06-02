/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../../KKdLib/default.hpp"
#include "../wrap.hpp"

namespace renderer {
    struct Transparency {
        p_dx_render_target m_fbo;
        p_dx_texture m_tex;
        dx_vertex_pixel_shader m_program;
        p_dx_sampler_state m_sampler_state;
        p_dx_buffer m_buffer;
        p_dx_blend_state m_blend_state;
        p_dx_rasterizer_state m_rasterizer_state;
        p_dx_depth_stencil_state m_depth_stencil_state;
    };
}

static_assert(sizeof(renderer::Transparency) == 0x48, "\"renderer::Transparency\" struct should have a size of 0x48");
