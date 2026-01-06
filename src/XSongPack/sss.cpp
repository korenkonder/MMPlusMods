/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "sss.hpp"
#include "../MMPlusModsShared/rob/rob.hpp"
#include "../MMPlusModsShared/render_context.hpp"
#include "../MMPlusModsShared/render_manager.hpp"
#include "reflect_full.hpp"
#include <Helpers.h>

sss_data*& _sss_data = *(sss_data**)0x00000001411497D8;

float_t sss_inverse_scale_reflect = 0.0f;
vec4 sss_param_reflect = 0.0f;

void dx_sss_struct::calc_coef(struct render_data_context& rend_data_ctx, __int64 a3, __int64 a4, double_t step,
    __int64 a6, const double_t* weights, const double_t* r_radius, const double_t* g_radius, const double_t* b_radius) {
    void (*dx_sss_struct__calc_coef)(dx_sss_struct * This, render_data_context & rend_data_ctx, __int64 a3, __int64 a4, double_t step,
        __int64 a6, const double_t * weights, const double_t * r_radius, const double_t * g_radius, const double_t * b_radius)
        = (void (*)(dx_sss_struct * This, render_data_context & rend_data_ctx, __int64 a3, __int64 a4, double_t step,
            __int64 a6, const double_t * weights, const double_t * r_radius, const double_t * g_radius, const double_t * b_radius))0x00000001405BD2A0;
    dx_sss_struct__calc_coef(this, rend_data_ctx, a3, a4, step, a6, weights, r_radius, g_radius, b_radius);
}

void dx_sss_struct::draw_quad(struct render_data_context& rend_data_ctx, int32_t width, int32_t height,
    float_t s0, float_t t0, float_t s1, float_t t1, const vec4& param) {
    s0 -= s1;
    t0 -= t1;

    const float_t w = (float_t)max_def(width, 1);
    const float_t h = (float_t)max_def(height, 1);

    render_data::quad_shader_data* quad = (render_data::quad_shader_data*)rend_data_ctx.state.map(buffer_quad);
    quad->g_texcoord_modifier = { 0.5f * s0, -0.5f * t0, 0.5f * s0, -0.5f * t0 + t0 };
    quad->g_texel_size = { 1.0f / w, 1.0f / h, w, h };
    quad->g_color = param;
    rend_data_ctx.state.unmap(buffer_quad);

    rend_data_ctx.state.set_vs_constant_buffer(0, 1, &buffer_quad);
    rend_data_ctx.state.set_ps_constant_buffer(0, 1, &buffer_quad);

    p_dx_buffer nul_buf;
    p_dx_buffer* buffers[] = { &nul_buf };
    int32_t strides[] = { 0 };
    int32_t offsets[] = { 0 };
    rend_data_ctx.state.set_vertex_buffer(0, 1, buffers, strides, offsets);
    rend_data_ctx.state.set_index_buffer(&nul_buf, 0, 0);
    rend_data_ctx.state.set_rasterizer_state(&rasterizer_state);
    rend_data_ctx.state.set_depth_stencil_state(dx_default_states_get_depth_stencil_state(
        DX_DEPTH_NONE, DX_DEPTH_FUNC_GREATER_EQUAL));
    rend_data_ctx.state.set_blend_state(dx_default_states_get_blend_state(false,
        DX_BLEND_SRC_ALPHA, DX_BLEND_INV_SRC_ALPHA,
        DX_BLEND_SRC_ALPHA, DX_BLEND_INV_SRC_ALPHA, DX_BLEND_WRITE_MASK_RGBA));
    rend_data_ctx.state.set_primitive_topology(DX_PRIMITIVE_TRIANGLE_STRIP);
    rend_data_ctx.state.draw(4, 0);
}

// 0x1405BE2B0
void sss_data::apply_filter(struct render_data_context& rend_data_ctx) {
    const int32_t sss_count = 6;

    rend_data_ctx.set_batch_sss_param(reflect_draw[rend_data_ctx.index] ? sss_param_reflect : dx->sss_param);
    if (downsample) {
        textures[0].Bind(rend_data_ctx.state);

        float_t render_width = 0.0f;
        float_t render_height = 0.0f;
        float_t render_post_width = 0.0f;
        float_t render_post_height = 0.0f;
        render_get()->get_render_resolution(&render_width, &render_height, &render_post_width, &render_post_height);

        RenderTexture& rt = reflect_draw[rend_data_ctx.index] && reflect_full_ptr
            ? reflect_full_ptr->reflect_texture
            : render_get()->rend_texture[0];

        rend_data_ctx.state.set_viewport(0, 0, 640, 360);
        rend_data_ctx.state.set_ps_textures(0, 1, &rt.GetColorTex());
        rend_data_ctx.state.set_ps_sampler_state(0, 1, &dx->sampler_state);
        rend_data_ctx.state.set_vs_shader(dx->shaders[0].vertex);
        rend_data_ctx.state.set_ps_shader(dx->shaders[0].pixel);
        dx->draw_quad(rend_data_ctx, rt.GetWidth(), rt.GetHeight(),
            render_width / render_post_width,
            render_height / render_post_height, 0.0f, 0.0f, 1.0f);
    }

    textures[2].Bind(rend_data_ctx.state);
    rend_data_ctx.state.set_viewport(0, 0, 320, 180);
    rend_data_ctx.state.set_ps_textures(0, 1, &textures[0].GetColorTex());
    rend_data_ctx.state.set_ps_sampler_state(0, 1, &dx->sampler_state);
    rend_data_ctx.state.set_vs_shader(&dx->shaders[rend_data_ctx.get_uniform_value(U_NPR) == 1 ? 2 : 1].vertex);
    rend_data_ctx.state.set_ps_shader(&dx->shaders[rend_data_ctx.get_uniform_value(U_NPR) == 1 ? 2 : 1].pixel);
    dx->draw_quad(rend_data_ctx, textures[0].GetWidth(), textures[0].GetHeight(), 1.0, 1.0, 0.0f, 0.0f, 1.0f);

    const double_t weights[] = { 0.4, 0.3, 0.3 };
    const double_t r_radius[] = { 1.0, 2.0, 5.0 };
    const double_t g_radius[] = { 0.2, 0.4, 1.2 };
    const double_t b_radius[] = { 0.3, 0.7, 2.0 };
    dx->calc_coef(rend_data_ctx, 0, 0, reflect_draw[rend_data_ctx.index]
        ? sss_inverse_scale_reflect : dx->inverse_scale, 0, weights, r_radius, g_radius, b_radius);

    textures[reflect_draw[rend_data_ctx.index] ? 3 : 1].Bind(rend_data_ctx.state);
    rend_data_ctx.state.set_viewport(0, 0, 320, 180);
    rend_data_ctx.state.set_ps_textures(0, 1, &textures[2].GetColorTex());
    rend_data_ctx.state.set_ps_sampler_state(0, 1, &dx->sampler_state);
    rend_data_ctx.state.set_vs_shader(&dx->shaders[3].vertex);
    rend_data_ctx.state.set_ps_shader(&dx->shaders[3].pixel);
    rend_data_ctx.state.set_ps_constant_buffer(1, 1, &dx->buffer_gaussian_coef);
    dx->draw_quad(rend_data_ctx, textures[2].GetWidth(), textures[2].GetHeight(), 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
}

// 0x1405BE2A0
vec4& sss_data::get_sss_param() {
    return dx->sss_param;
}

// 0x1405BDE90
bool sss_data::set(struct render_data_context& rend_data_ctx) {
    if (!init_data)
        return false;
    else if (!enable) {
        rend_data_ctx.set_batch_sss_param(0.0f);
        return false;
    }

    rndr::Render* rend = render_get();
    float_t render_width;
    float_t render_height;
    rend->get_render_resolution(&render_width, &render_height, 0, 0);
    downsample = true;
    rend->bind_render_texture(rend_data_ctx.state);

    rend_data_ctx.state.clear_render_target_view(param.x, param.y, param.z, 0.0f);
    rend_data_ctx.state.clear_depth_stencil_view(0x00, 0.0f, true);
    return true;
}

// Added
bool sss_data::set(struct render_data_context& rend_data_ctx, RenderTexture& rt, float_t quality) {
    if (!init_data)
        return false;
    else if (!enable) {
        rend_data_ctx.set_batch_sss_param(0.0f);
        return false;
    }

    downsample = true;
    rt.Bind(rend_data_ctx.state);
    rend_data_ctx.state.set_viewport(0, 0,
        (int32_t)((float_t)rt.GetWidth() * quality),
        (int32_t)((float_t)rt.GetHeight() * quality));

    rend_data_ctx.state.clear_render_target_view(param.x, param.y, param.z, 0.0f);
    rend_data_ctx.state.clear_depth_stencil_view(0x00, 0.0f, true);
    return true;
}

// 0x1405BE630
void sss_data::set_texture(struct render_data_context& rend_data_ctx, int32_t index) {
    rend_data_ctx.state.set_ps_textures(16, 1, &textures[index].GetColorTex());
    rend_data_ctx.state.set_ps_sampler_state(12, 1, &dx->sampler_state);
}

// 0x1405BDA20
sss_data* sss_data_get() {
    return _sss_data;
}

HOOK(void, FASTCALL, sss_data__pre_proc, 0x00000001405BDF60, sss_data* This, const cam_data& cam) {
    originalsss_data__pre_proc(This, cam);

    const int32_t sss_count = 6;
    vec3 interest = camera_data.interest;
    vec3 view_point = camera_data.view_point;

    vec3 chara_position[2];
    chara_position[0] = 0.0f;
    chara_position[1] = 0.0f;
    float_t chara_distance[2];
    size_t rob_chara_smth = get_rob_chara_smth();
    for (int32_t i = 0; i < 2; i++) {
        chara_position[i] = interest;
        chara_distance[i] = 999999.0f;
        rob_chara_bone_data* rob_bone_data = rob_chara_array_get_bone_data(rob_chara_smth, i);
        if (rob_chara_pv_data_array_check_chara_id(rob_chara_smth, i)
            && rob_chara_array_check_visibility(rob_chara_smth, i)) {
            const mat4* mat = rob_chara_bone_data_get_mats_mat(rob_bone_data, 0);
            if (mat) {
                mat4 temp;
                mat4_transpose(mat, &temp);
                mat4_get_translation(&temp, &chara_position[i]);
                if (reflect_draw)
                    mat4_transform_point(&reflect_mat, &chara_position[i], &chara_position[i]);
                chara_distance[i] = vec3::distance(view_point, chara_position[i]);
            }
        }
    }

    vec3 closest_chara_position = chara_distance[0] <= chara_distance[1]
        ? chara_position[0] : chara_position[1];

    float_t length = vec3::distance(interest, closest_chara_position);
    if (length > 1.25f)
        interest = chara_position[0];

    float_t distance_to_interest = max_def(vec3::distance(view_point, interest), 0.25f);
    float_t fov_scale = max_def(tanf(camera_data.fov * 0.5f * DEG_TO_RAD_FLOAT) * 5.0f, 0.25f);
    float_t sss_strength = 0.6f;
    float_t inverse_scale = (float_t)(1.0 / clamp_def(fov_scale * distance_to_interest, 0.25f, 100.0f));
    if (inverse_scale < 0.145f)
        sss_strength = max_def(inverse_scale - 0.02f, 0.0f) * 8.0f * 0.6f;
    sss_inverse_scale_reflect = inverse_scale;
    sss_param_reflect = { sss_strength, 0.0f, 0.0f, 0.0f };
}

void sss_patch() {
    INSTALL_HOOK(sss_data__pre_proc);
}
