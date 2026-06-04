/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "render_context.hpp"
#include "render_manager.hpp"
#include "types.hpp"

draw_state_struct& draw_state = *(draw_state_struct*)0x000000014174D9E0;

void render_data::obj_shader_data::reset() {
    g_shader_flags = {};
}

void render_data::obj_shader_data::set_shader_flags(int32_t shader_flags[U_MAX]) {
    g_shader_flags.x.m.alpha_mask               = shader_flags[U_ALPHA_MASK];
    g_shader_flags.x.m.alpha_test               = shader_flags[U_ALPHA_TEST];
    g_shader_flags.x.m.aniso_tangent            = shader_flags[U_ANISO_TANGENT];
    g_shader_flags.x.m.composite_back           = shader_flags[U_COMPOSITE_BACK];
    g_shader_flags.x.m.blend_func_01            = shader_flags[U_BLEND_FUNC_01];
    g_shader_flags.x.m.unk                      = shader_flags[U_UNK_NEW_IN_MM_IDK_WHAT_TO_DO_WITH_IT];
    g_shader_flags.x.m.chara_color              = shader_flags[U_CHARA_COLOR];
    g_shader_flags.x.m.clip_plane               = shader_flags[U_CLIP_PLANE];
    g_shader_flags.x.m.u08                      = shader_flags[U08];
    g_shader_flags.x.m.depth_peel               = shader_flags[U_DEPTH_PEEL];
    g_shader_flags.x.m.depth                    = shader_flags[U_DEPTH];
    g_shader_flags.x.m.double_side              = shader_flags[U_DOUBLE_SIDE];
    g_shader_flags.x.m.alpha_blend              = shader_flags[U_ALPHA_BLEND];
    g_shader_flags.x.m.ripple_emit              = shader_flags[U_RIPPLE_EMIT];
    g_shader_flags.x.m.esm_filter               = shader_flags[U_ESM_FILTER];
    g_shader_flags.x.m.exposure                 = shader_flags[U_EXPOSURE];
    g_shader_flags.x.m.fade_color               = shader_flags[U_FADE_COLOR];
    g_shader_flags.x.m.fade_type                = shader_flags[U_FADE_TYPE];
    g_shader_flags.x.m.stage_ambient            = shader_flags[U_STAGE_AMBIENT];
    g_shader_flags.x.m.flare                    = shader_flags[U_FLARE];
    g_shader_flags.y.m.fog                      = shader_flags[U_FOG];
    g_shader_flags.y.m.fogmap                   = shader_flags[U_FOGMAP];
    g_shader_flags.y.m.u16                      = shader_flags[U16];
    g_shader_flags.y.m.gauss                    = shader_flags[U_GAUSS];
    g_shader_flags.y.m.eye_lens                 = shader_flags[U_EYE_LENS];
    g_shader_flags.y.m.image_filter             = shader_flags[U_IMAGE_FILTER];
    g_shader_flags.y.m.instance                 = shader_flags[U_INSTANCE];
    g_shader_flags.y.m.tone_curve               = shader_flags[U_TONE_CURVE];
    g_shader_flags.y.m.light_proj               = shader_flags[U_LIGHT_PROJ];
    g_shader_flags.y.m.magnify                  = shader_flags[U_MAGNIFY];
    g_shader_flags.y.m.membrane                 = shader_flags[U_MEMBRANE];
    g_shader_flags.y.m.mlaa                     = shader_flags[U_MLAA];
    g_shader_flags.y.m.mlaa_search              = shader_flags[U_MLAA_SEARCH];
    g_shader_flags.y.m.morph_color              = shader_flags[U_MORPH_COLOR];
    g_shader_flags.y.m.morph_pos_normal_uv      = shader_flags[U_MORPH_POS_NORMAL_UV];
    g_shader_flags.y.m.movie                    = shader_flags[U_MOVIE];
    g_shader_flags.y.m.u24                      = shader_flags[U24];
    g_shader_flags.y.m.u25                      = shader_flags[U25];
    g_shader_flags.y.m.npr_normal               = shader_flags[U_NPR_NORMAL];
    g_shader_flags.z.m.npr                      = shader_flags[U_NPR];
    g_shader_flags.z.m.num_shadowmap            = shader_flags[U_NUM_SHADOWMAP];
    g_shader_flags.z.m.reflect                  = shader_flags[U_REFLECT];
    g_shader_flags.z.m.reduce_tex               = shader_flags[U_REDUCE_TEX];
    g_shader_flags.z.m.self_shadow              = shader_flags[U_SELF_SHADOW];
    g_shader_flags.z.m.shadow_sample            = shader_flags[U_SHADOW_SAMPLE];
    g_shader_flags.z.m.u2d                      = shader_flags[U2D];
    g_shader_flags.z.m.show_mipmap              = shader_flags[U_SHOW_MIPMAP];
    g_shader_flags.z.m.show_vector              = shader_flags[U_SHOW_VECTOR];
    g_shader_flags.z.m.skinning                 = shader_flags[U_SKINNING];
    g_shader_flags.z.m.snow_type                = shader_flags[U_SNOW_TYPE];
    g_shader_flags.z.m.specular                 = shader_flags[U_SPECULAR];
    g_shader_flags.z.m.combiner                 = shader_flags[U_COMBINER];
    g_shader_flags.z.m.tex_0_type               = shader_flags[U_TEX_0_TYPE];
    g_shader_flags.z.m.tex_1_type               = shader_flags[U_TEX_1_TYPE];
    g_shader_flags.z.m.sss_filter               = shader_flags[U_SSS_FILTER];
    g_shader_flags.w.m.sss_chara                = shader_flags[U_SSS_CHARA];
    g_shader_flags.w.m.star                     = shader_flags[U_STAR];
    g_shader_flags.w.m.tex_color                = shader_flags[U_TEX_COLOR];
    g_shader_flags.w.m.tex_envmap               = shader_flags[U_TEX_ENVMAP];
    g_shader_flags.w.m.tex_format               = shader_flags[U_TEX_FORMAT];
    g_shader_flags.w.m.tex_lucency              = shader_flags[U_TEX_LUCENCY];
    g_shader_flags.w.m.tex_normal               = shader_flags[U_TEX_NORMAL];
    g_shader_flags.w.m.tex_parency              = shader_flags[U_TEX_PARENCY];
    g_shader_flags.w.m.tex_reflectmap           = shader_flags[U_TEX_REFLECTMAP];
    g_shader_flags.w.m.tex_refractmap           = shader_flags[U_TEX_REFRACTMAP];
    g_shader_flags.w.m.tex_refractmap_waterring = shader_flags[U_TEX_REFRACTMAP_WATERRING];
    g_shader_flags.w.m.tex_shadow               = shader_flags[U_TEX_SHADOW];
    g_shader_flags.w.m.tex_specular             = shader_flags[U_TEX_SPECULAR];
    g_shader_flags.w.m.tone_map_method          = shader_flags[U_TONE_MAP_METHOD];
    g_shader_flags.w.m.uv_layer                 = shader_flags[U_UV_LAYER];
}

int32_t render_data_context::get_uniform_value(UniformName name) {
    return uniform->arr[name];
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
    uniform->arr[U_TEX_COLOR] = 0;
    uniform->arr[U_BLEND_FUNC_01] = 0;
    uniform->arr[U_TEX_NORMAL] = 0;
    uniform->arr[U_TEX_SPECULAR] = 0;
    uniform->arr[U_TEX_PARENCY] = 0;
    uniform->arr[U_TEX_LUCENCY] = 0;
    uniform->arr[U_TEX_ENVMAP] = 0;
    uniform->arr[U_DOUBLE_SIDE] = 0;
    uniform->arr[U_SPECULAR] = 0;
    uniform->arr[U_FOG] = 0;
    uniform->arr[U_FOGMAP] = 0;
    uniform->arr[U_ANISO_TANGENT] = 0;
    uniform->arr[U_TEX_SHADOW] = 0;
    uniform->arr[U_UV_LAYER] = 0;
    uniform->arr[U_TEX_0_TYPE] = 0;
    uniform->arr[U_TEX_1_TYPE] = 0;
}
