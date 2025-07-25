/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../../KKdLib/default.hpp"
#include "../render_texture.hpp"
#include "../wrap.hpp"

enum dof_debug_flags {
    DOF_DEBUG_USE_UI_PARAMS   = 0x01,
    DOF_DEBUG_ENABLE_DOF      = 0x02,
    DOF_DEBUG_ENABLE_PHYS_DOF = 0x04,
    DOF_DEBUG_AUTO_FOCUS      = 0x08,
};

struct dof_f2 {
    float_t focus;
    float_t focus_range;
    float_t fuzzing_range;
    float_t ratio;
};

static_assert(sizeof(dof_f2) == 0x10, "\"dof_f2\" struct should have a size of 0x10");

struct dof_debug {
    dof_debug_flags flags;
    float_t focus;
    float_t focal_length;
    float_t f_number;
    dof_f2 f2;
};

static_assert(sizeof(dof_debug) == 0x20, "\"dof_debug\" struct should have a size of 0x14");

struct dof_pv {
    bool enable;
    dof_f2 f2;
};

static_assert(sizeof(dof_pv) == 0x14, "\"dof_pv\" struct should have a size of 0x14");

struct render_data_context;

namespace renderer {
    struct DOF3 {
        int32_t width;
        int32_t height;
        float_t scale;
        int8_t field_C;
        p_dx_texture textures[6];
        p_dx_render_target render_targets[4];
        p_dx_sampler_state samplers[2];
        dx_vertex_pixel_shader shaders[15];
        p_dx_buffer buffers[8];
        p_dx_buffer field_1A0;
        int32_t field_1A8;
        int32_t field_1AC;
        p_dx_rasterizer_state rasterizer_state;
        p_dx_depth_stencil_state depth_stencil_state;
        p_dx_blend_state blend_state;

        void apply(render_data_context& rend_data_ctx,
            RenderTexture& rt, p_dx_texture& color_texture, p_dx_texture& depth_texture,
            float_t min_distance, float_t max_distance, float_t fov, float_t focus,
            float_t focal_length, float_t f_number, int32_t width, int32_t height);
        void apply_f2(render_data_context& rend_data_ctx,
            RenderTexture& rt, p_dx_texture& color_texture, p_dx_texture& depth_texture,
            float_t min_distance, float_t max_distance, float_t fov, float_t focus,
            float_t focus_range, float_t fuzzing_range, float_t ratio, int32_t width, int32_t height);
    };
}

static_assert(sizeof(renderer::DOF3) == 0x1C8, "\"renderer::DOF3\" struct should have a size of 0x1C8");

extern dof_debug& dof_debug_data;
extern dof_pv& dof_pv_data;
