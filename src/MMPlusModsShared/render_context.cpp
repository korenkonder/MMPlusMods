/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "render_context.hpp"
#include "render_manager.hpp"
#include "types.hpp"

draw_state_struct& draw_state = *(draw_state_struct*)0x000000014174D9E0;

sss_data*& _sss_data = *(sss_data**)0x00000001411497D8;

void dx_sss_struct::calc_coef(render_data_context& rend_data_ctx, __int64 a3, __int64 a4, double_t step,
    __int64 a6, const double_t* weights, const double_t* r_radius, const double_t* g_radius, const double_t* b_radius) {

    void (*dx_sss_struct__calc_coef)(dx_sss_struct * This, render_data_context & rend_data_ctx, __int64 a3, __int64 a4, double_t step,
        __int64 a6, const double_t * weights, const double_t * r_radius, const double_t * g_radius, const double_t * b_radius)
        = (void (*)(dx_sss_struct * This, render_data_context & rend_data_ctx, __int64 a3, __int64 a4, double_t step,
            __int64 a6, const double_t * weights, const double_t * r_radius, const double_t * g_radius, const double_t * b_radius))0x00000001405BD2A0;
    dx_sss_struct__calc_coef(this, rend_data_ctx, a3, a4, step, a6, weights, r_radius, g_radius, b_radius);
}

void dx_sss_struct::draw_quad(render_data_context& rend_data_ctx, int32_t width, int32_t height,
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

vec4& sss_data::get_sss_param() {
    return dx->sss_param;
}

void render_data::obj_shader_data::reset() {
    g_shader_flags = {};
}

void render_data::obj_shader_data::set_shader_flags(int32_t shader_flags[U_MAX]) {
    g_shader_flags.x.m.alpha_mask    = shader_flags[U_ALPHA_MASK];
    g_shader_flags.x.m.alpha_test    = shader_flags[U_ALPHA_TEST];
    g_shader_flags.x.m.aniso         = shader_flags[U_ANISO];
    g_shader_flags.x.m.aet_back      = shader_flags[U_AET_BACK];
    g_shader_flags.x.m.texture_blend = shader_flags[U_TEXTURE_BLEND];
    g_shader_flags.x.m.unk           = shader_flags[U_UNK_NEW_IN_MM_IDK_WHAT_TO_DO_WITH_IT];
    g_shader_flags.x.m.chara_color   = shader_flags[U_CHARA_COLOR];
    g_shader_flags.x.m.clip_plane    = shader_flags[U_CLIP_PLANE];
    g_shader_flags.x.m.u08           = shader_flags[U08];
    g_shader_flags.x.m.depth_peel    = shader_flags[U_DEPTH_PEEL];
    g_shader_flags.x.m.depth         = shader_flags[U_DEPTH];
    g_shader_flags.x.m.u0b           = shader_flags[U0B];
    g_shader_flags.x.m.alpha_blend   = shader_flags[U_ALPHA_BLEND];
    g_shader_flags.x.m.ripple_emit   = shader_flags[U_RIPPLE_EMIT];
    g_shader_flags.x.m.esm_filter    = shader_flags[U_ESM_FILTER];
    g_shader_flags.x.m.exposure      = shader_flags[U_EXPOSURE];
    g_shader_flags.x.m.scene_fade    = shader_flags[U_SCENE_FADE];
    g_shader_flags.x.m.fade          = shader_flags[U_FADE];
    g_shader_flags.x.m.stage_ambient = shader_flags[U_STAGE_AMBIENT];
    g_shader_flags.x.m.flare         = shader_flags[U_FLARE];
    g_shader_flags.y.m.fog_stage     = shader_flags[U_FOG_STAGE];
    g_shader_flags.y.m.fog_chara     = shader_flags[U_FOG_CHARA];
    g_shader_flags.y.m.u16           = shader_flags[U16];
    g_shader_flags.y.m.gauss         = shader_flags[U_GAUSS];
    g_shader_flags.y.m.eye_lens      = shader_flags[U_EYE_LENS];
    g_shader_flags.y.m.image_filter  = shader_flags[U_IMAGE_FILTER];
    g_shader_flags.y.m.instance      = shader_flags[U_INSTANCE];
    g_shader_flags.y.m.tone_curve    = shader_flags[U_TONE_CURVE];
    g_shader_flags.y.m.light_proj    = shader_flags[U_LIGHT_PROJ];
    g_shader_flags.y.m.magnify       = shader_flags[U_MAGNIFY];
    g_shader_flags.y.m.membrane      = shader_flags[U_MEMBRANE];
    g_shader_flags.y.m.mlaa          = shader_flags[U_MLAA];
    g_shader_flags.y.m.mlaa_search   = shader_flags[U_MLAA_SEARCH];
    g_shader_flags.y.m.morph_color   = shader_flags[U_MORPH_COLOR];
    g_shader_flags.y.m.morph         = shader_flags[U_MORPH];
    g_shader_flags.y.m.movie         = shader_flags[U_MOVIE];
    g_shader_flags.y.m.u24           = shader_flags[U24];
    g_shader_flags.y.m.u25           = shader_flags[U25];
    g_shader_flags.y.m.npr_normal    = shader_flags[U_NPR_NORMAL];
    g_shader_flags.z.m.npr           = shader_flags[U_NPR];
    g_shader_flags.z.m.stage_shadow2 = shader_flags[U_STAGE_SHADOW2];
    g_shader_flags.z.m.reflect       = shader_flags[U_REFLECT];
    g_shader_flags.z.m.reduce        = shader_flags[U_REDUCE];
    g_shader_flags.z.m.chara_shadow  = shader_flags[U_CHARA_SHADOW];
    g_shader_flags.z.m.chara_shadow2 = shader_flags[U_CHARA_SHADOW2];
    g_shader_flags.z.m.u2d           = shader_flags[U2D];
    g_shader_flags.z.m.u2e           = shader_flags[U2E];
    g_shader_flags.z.m.show_vector   = shader_flags[U_SHOW_VECTOR];
    g_shader_flags.z.m.skinning      = shader_flags[U_SKINNING];
    g_shader_flags.z.m.snow_particle = shader_flags[U_SNOW_PARTICLE];
    g_shader_flags.z.m.specular_ibl  = shader_flags[U_SPECULAR_IBL];
    g_shader_flags.z.m.combiner      = shader_flags[U_COMBINER];
    g_shader_flags.z.m.tex_0_type    = shader_flags[U_TEX_0_TYPE];
    g_shader_flags.z.m.tex_1_type    = shader_flags[U_TEX_1_TYPE];
    g_shader_flags.z.m.sss_filter    = shader_flags[U_SSS_FILTER];
    g_shader_flags.w.m.sss_chara     = shader_flags[U_SSS_CHARA];
    g_shader_flags.w.m.star          = shader_flags[U_STAR];
    g_shader_flags.w.m.texture_count = shader_flags[U_TEXTURE_COUNT];
    g_shader_flags.w.m.env_map       = shader_flags[U_ENV_MAP];
    g_shader_flags.w.m.ripple        = shader_flags[U_RIPPLE];
    g_shader_flags.w.m.translucency  = shader_flags[U_TRANSLUCENCY];
    g_shader_flags.w.m.normal        = shader_flags[U_NORMAL];
    g_shader_flags.w.m.transparency  = shader_flags[U_TRANSPARENCY];
    g_shader_flags.w.m.water_reflect = shader_flags[U_WATER_REFLECT];
    g_shader_flags.w.m.u40           = shader_flags[U40];
    g_shader_flags.w.m.u41           = shader_flags[U41];
    g_shader_flags.w.m.stage_shadow  = shader_flags[U_STAGE_SHADOW];
    g_shader_flags.w.m.specular      = shader_flags[U_SPECULAR];
    g_shader_flags.w.m.tone_map      = shader_flags[U_TONE_MAP];
    g_shader_flags.w.m.u45           = shader_flags[U45];
}

int32_t render_data_context::get_uniform_value(uniform_name name) {
    return uniform->arr[name];
}

void render_data_context::reset_render_target(p_dx_state& p_dx_st) {
    p_dx_render_target rt;
    p_dx_st.set_render_target(&rt);
}

void sss_data::set_texture(render_data_context& rend_data_ctx, int32_t index) {
    rend_data_ctx.state.set_ps_textures(16, 1, &textures[index].GetColorTex());
    rend_data_ctx.state.set_ps_sampler_state(12, 1, &dx->sampler_state);
}

void draw_state_struct::set_blend(render_data_context& rend_data_ctx, bool value) {
    rend_data[rend_data_ctx.index].blend = value;
}

void draw_state_struct::set_cull_front(render_data_context& rend_data_ctx, bool value) {
    rend_data[rend_data_ctx.index].cull_front = value;
}

void render_data_context::set_batch_alpha_threshold(float_t value) {
    render_data* data = this->data;
    data->buffer_batch_data.g_max_alpha.z = value;
    enum_or(data->flags, RENDER_DATA_BATCH_UPDATE);
}

void render_data_context::set_batch_min_alpha(const float_t value) {
    render_data* data = this->data;
    data->buffer_batch_data.g_max_alpha.w = value;
    enum_or(data->flags, RENDER_DATA_BATCH_UPDATE);
}

void render_data_context::set_batch_sss_param(const vec4& value) {
    render_data* data = this->data;
    data->buffer_batch_data.g_sss_param = value;
    enum_or(data->flags, RENDER_DATA_BATCH_UPDATE);
}

void render_data_context::set_glitter_render_data_state() {
    render_data* data = this->data;
    data->buffer_shader_data.set_shader_flags(data->shader_flags.arr);

    state.write_buffer(data->buffer_shader, data->buffer_shader_data);
    state.set_vs_constant_buffer(0, 1, &data->buffer_shader);
    state.set_ps_constant_buffer(0, 1, &data->buffer_shader);

    if (data->flags & RENDER_DATA_SCENE_UPDATE) {
        state.write_buffer(data->buffer_scene, data->buffer_scene_data);
        enum_and(data->flags, ~RENDER_DATA_BATCH_UPDATE);
    }

    state.set_vs_constant_buffer(1, 1, &data->buffer_scene);
    state.set_ps_constant_buffer(1, 1, &data->buffer_scene);
    return;

    static void (FASTCALL * render_data_context__set_glitter_render_data_state)(render_data_context * This)
        = (void (FASTCALL*)(render_data_context * This))0x00000001404D4F20;
    render_data_context__set_glitter_render_data_state(this);
}

void render_data_context::set_npr(void* render_manager) {
    uniform->arr[U_NPR] = 0;
    if (((rndr::RenderManager*)render_manager)->get_npr_param() == 1)
        uniform->arr[U_NPR] = 1;
}

void render_data_context::uniform_value_reset() {
    uniform->arr[U_TEXTURE_COUNT] = 0;
    uniform->arr[U_TEXTURE_BLEND] = 0;
    uniform->arr[U_NORMAL] = 0;
    uniform->arr[U_SPECULAR] = 0;
    uniform->arr[U_TRANSPARENCY] = 0;
    uniform->arr[U_TRANSLUCENCY] = 0;
    uniform->arr[U_ENV_MAP] = 0;
    uniform->arr[U0B] = 0;
    uniform->arr[U_SPECULAR_IBL] = 0;
    uniform->arr[U_FOG_STAGE] = 0;
    uniform->arr[U_FOG_CHARA] = 0;
    uniform->arr[U_ANISO] = 0;
    uniform->arr[U_STAGE_SHADOW] = 0;
    uniform->arr[U45] = 0;
    uniform->arr[U_TEX_0_TYPE] = 0;
    uniform->arr[U_TEX_1_TYPE] = 0;
}

sss_data* sss_data_get() {
    return _sss_data;
}
