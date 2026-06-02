/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "dof.hpp"
#include "../render_context.hpp"
#include <Helpers.h>

dof_debug& dof_debug_data = *(dof_debug*)0x0000000141753410;
dof_pv& dof_pv_data = *(dof_pv*)0x000000014CC2CFE8;

FUNCTION_PTR(float_t, FASTCALL, dof_quality_get, 0x00000001402C2240);

FUNCTION_PTR(void, FASTCALL, renderer__DOF3__write_data,
    0x00000001404992C0, renderer::DOF3* This, render_data_context& rend_data_ctx,
    float_t min_distance, float_t max_distance, float_t fov, float_t focus, float_t focal_length,
    float_t f_number, float_t focus_range, float_t fuzzing_range, float_t ratio, int32_t width, int32_t height);
FUNCTION_PTR(void, FASTCALL, renderer__DOF3__render_tiles,
    0x00000001404997E0, renderer::DOF3* This, render_data_context& rend_data_ctx,
    p_dx_texture& depth_texture, bool f2);
FUNCTION_PTR(void, FASTCALL, renderer__DOF3__downsample,
    0x0000000140499930, renderer::DOF3* This, render_data_context& rend_data_ctx,
    p_dx_texture& color_texture, p_dx_texture& depth_texture, bool f2, int32_t width, int32_t height);
FUNCTION_PTR(void, FASTCALL, renderer__DOF3__apply_main_filter,
    0x0000000140499AC0, renderer::DOF3* This, render_data_context& rend_data_ctx,
    bool f2, int32_t width, int32_t height);
FUNCTION_PTR(void, FASTCALL, renderer__DOF3__upsample,
    0x0000000140499CB0, renderer::DOF3* This, render_data_context& rend_data_ctx,
    RenderTexture& rt, p_dx_texture& color_texture, p_dx_texture& depth_texture, bool f2, int32_t width, int32_t height);

namespace renderer {
    // Inlined
    void DOF3::apply(render_data_context& rend_data_ctx,
        RenderTexture& rt, p_dx_texture& color_texture, p_dx_texture& depth_texture,
        float_t min_distance, float_t max_distance, float_t fov, float_t focus,
        float_t focal_length, float_t f_number, int32_t width, int32_t height) {
        m_scale = dof_quality_get();
        rend_data_ctx.state.begin_event("renderer::DOF3::apply");
        rend_data_ctx.state.set_blend_state(&m_blend_state);
        rend_data_ctx.state.set_depth_stencil_state(&m_depth_stencil_state);
        rend_data_ctx.state.set_rasterizer_state(&m_rasterizer_state);
        renderer__DOF3__write_data(this, rend_data_ctx, min_distance, max_distance, fov, focus,
            focal_length, f_number, 0.0f, 0.1f, 0.0f, width, height);
        renderer__DOF3__render_tiles(this, rend_data_ctx, depth_texture, false);
        renderer__DOF3__downsample(this, rend_data_ctx, color_texture, depth_texture, false, width, height);
        renderer__DOF3__apply_main_filter(this, rend_data_ctx, false, width, height);
        renderer__DOF3__upsample(this, rend_data_ctx, rt, color_texture, depth_texture, false, width, height);
        rend_data_ctx.state.end_event();
    }

    // 0x140499120
    void DOF3::apply_f2(render_data_context& rend_data_ctx,
        RenderTexture& rt, p_dx_texture& color_texture, p_dx_texture& depth_texture,
        float_t min_distance, float_t max_distance, float_t fov, float_t focus,
        float_t focus_range, float_t fuzzing_range, float_t ratio, int32_t width, int32_t height) {
        m_scale = dof_quality_get();
        rend_data_ctx.state.begin_event("renderer::DOF3::apply_f2");
        rend_data_ctx.state.set_blend_state(&m_blend_state);
        rend_data_ctx.state.set_depth_stencil_state(&m_depth_stencil_state);
        rend_data_ctx.state.set_rasterizer_state(&m_rasterizer_state);
        renderer__DOF3__write_data(this, rend_data_ctx, min_distance, max_distance, fov, focus,
            0.0f, 1.0f, focus_range, fuzzing_range, ratio, width, height);
        renderer__DOF3__render_tiles(this, rend_data_ctx, depth_texture, true);
        renderer__DOF3__downsample(this, rend_data_ctx, color_texture, depth_texture, true, width, height);
        renderer__DOF3__apply_main_filter(this, rend_data_ctx, true, width, height);
        renderer__DOF3__upsample(this, rend_data_ctx, rt, color_texture, depth_texture, true, width, height);
        rend_data_ctx.state.end_event();
    }
}
