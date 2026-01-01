/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/vec.hpp"
#include "../MMPlusModsShared/render_texture.hpp"
#include "wrap.hpp"

struct dx_sss_struct {
    struct gaussian_coef_data {
        vec4 g_param;
        vec4 g_coef[64];
    };

    static_assert(sizeof(dx_sss_struct::gaussian_coef_data) == 0x410, "\"dx_sss_struct::gaussian_coef_data\" struct should have a size of 0x410");

    p_dx_sampler_state sampler_state;
    p_dx_rasterizer_state rasterizer_state;
    p_dx_buffer buffer_quad;
    p_dx_buffer buffer_gaussian_coef;
    dx_vertex_pixel_shader shaders[4];
    float_t inverse_scale;
    vec4 sss_param;

    void calc_coef(struct render_data_context& rend_data_ctx, __int64 a3, __int64 a4, double_t step,
        __int64 a6, const double_t* weights, const double_t* r_radius, const double_t* g_radius, const double_t* b_radius);
    void draw_quad(struct render_data_context& rend_data_ctx, int32_t width, int32_t height,
        float_t s0, float_t t0, float_t s1, float_t t1, const vec4& param);
};

static_assert(sizeof(dx_sss_struct) == 0x78, "\"dx_sss_struct\" struct should have a size of 0x78");

struct sss_data {
    bool init_data;
    bool enable;
    bool downsample;
    RenderTexture textures[4];
    vec4 param;
    dx_sss_struct* dx;

    void apply_filter(struct render_data_context& rend_data_ctx);
    vec4& get_sss_param();
    bool set(struct render_data_context& rend_data_ctx);
    bool set(struct render_data_context& rend_data_ctx, RenderTexture& rt, float_t quality = 1.0f); // Added
    void set_texture(struct render_data_context& rend_data_ctx, int32_t index);
};

static_assert(sizeof(sss_data) == 0x120, "\"sss_data\" struct should have a size of 0x120");

extern vec4 sss_param_reflect;

extern sss_data* sss_data_get();

extern void sss_patch();
