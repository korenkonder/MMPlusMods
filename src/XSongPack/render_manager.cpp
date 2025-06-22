/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "render_manager.hpp"
#include "../KKdLib/default.hpp"
#include "../MMPlusModsShared/light_param/light.hpp"
#include "../MMPlusModsShared/app.hpp"
#include "../MMPlusModsShared/render_context.hpp"
#include "Glitter/glitter.hpp"
#include "mdl/disp_manager.hpp"
#include "rob/rob.hpp"
#include <Helpers.h>

struct draw_pass_3d_translucent_alpha_data {
    int32_t alpha;
    float_t depth;
    int32_t count;
};

struct reflect_full_struct {
    RenderTexture reflect_texture;
    RenderTexture reflect_buffer_texture;
    RenderTexture reflect_nonsss_texture;
    RenderTexture reflect_contour_texture;
    texture* reflect_nonsss_tex;

    reflect_full_struct();
    ~reflect_full_struct();

    void free();
    void init();
};

bool local_draw[4] = { false };
vec4 reflect_clip_plane = 0.0f;
bool reflect_draw[4] = { false };
mat4 reflect_mat = mat4_identity;
reflect_full_struct* reflect_full_ptr = 0;

float_t sss_inverse_scale = 0.0f;
vec4 sss_param_reflect = 0.0f;

static void draw_npr_frame(render_data_context& rend_data_ctx);

static void draw_pass_sss_contour(rndr::Render* rend, render_data_context& rend_data_ctx);
static void draw_pass_sss_filter(sss_data* sss, render_data_context& rend_data_ctx);
static void draw_pass_3d_translucent(render_data_context& rend_data_ctx, mdl::ObjType opaque,
    mdl::ObjType transparent, mdl::ObjType translucent, cam_data& cam);
static void draw_pass_3d_translucent(render_data_context& rend_data_ctx, mdl::ObjTypeLocal opaque,
    mdl::ObjTypeLocal transparent, mdl::ObjTypeLocal translucent, cam_data& cam);
static int32_t draw_pass_3d_translucent_count_layers(render_data_context& rend_data_ctx, int32_t alpha_array[0x100],
    mdl::ObjType opaque, mdl::ObjType transparent, mdl::ObjType translucent, cam_data& cam);
static int32_t draw_pass_3d_translucent_count_layers(render_data_context& rend_data_ctx, int32_t alpha_array[0x100],
    mdl::ObjTypeLocal opaque, mdl::ObjTypeLocal transparent, mdl::ObjTypeLocal translucent, cam_data& cam);
static void draw_pass_3d_translucent_has_objects(bool* arr, mdl::ObjListSort& obj_list_sort, cam_data& cam);

static void draw_pass_reflect_full(render_data_context& rend_data_ctx, rndr::RenderManager& render_manager);

static bool draw_pass_reflect_get_obj_reflect_surface();
static bool draw_pass_reflect_get_obj_reflect_surface(mdl::ObjType type);

static float_t reflection_quality_full_get();

void rndr__RenderManager__rndpass_pre_proc_mid_impl(render_data_context& rend_data_ctx, rndr::RenderManager& render_manager) {
    if (render_manager.glitter)
        Glitter::glt_particle_manager_x->CalcDisp(rend_data_ctx);
}

void rndr__RenderManager__pass_ss_sss_mid_impl(render_data_context& rend_data_ctx, rndr::RenderManager& render_manager) {
    extern bool reflect_full;
    if (!reflect_full || !render_manager.pass_sw[rndr::RND_PASSID_REFLECT]
        || !draw_pass_reflect_get_obj_reflect_surface()
        || !disp_manager->get_obj_count(mdl::OBJ_TYPE_SSS) || !reflect_full_ptr)
        return;

    rend_data_ctx.state.begin_event("reflect");

    if (sub_1402C1D20()) {
        ::sss_data* sss = sss_data_get();
        if (sss->init_data && sss->enable) {
            rndr::Render* rend = render_manager.render;
            RenderTexture& refl_tex = reflect_full_ptr->reflect_texture;
            reflect_draw[rend_data_ctx.index] = true;

            refl_tex.Bind(rend_data_ctx.state);
            const float_t reflection_quality = reflection_quality_full_get();
            rend_data_ctx.state.set_viewport(0, 0,
                (int32_t)((float_t)refl_tex.GetWidth() * reflection_quality),
                (int32_t)((float_t)refl_tex.GetHeight() * reflection_quality));

            ::sss_data* sss = sss_data_get();
            rend_data_ctx.state.clear_render_target_view(sss->param.x, sss->param.y, sss->param.z, 0.0f);
            rend_data_ctx.state.clear_depth_stencil_view(0x00, 0.0f, true);

            lighting_set(rend_data_ctx);

            if (render_manager.shadow)
                draw_pass_3d_shadow_set(render_manager.shadow_ptr, rend_data_ctx);
            else
                draw_pass_3d_shadow_reset(render_manager.shadow_ptr, rend_data_ctx);

            shader_get_id_by_name(rend_data_ctx, "SSS_SKIN");
            cam_data cam = render_manager.cam[0];
            rend_data_ctx.set_batch_scene_camera(cam);
            mdl::DispManager::draw(rend_data_ctx, mdl::OBJ_TYPE_SSS, cam);
            shader_get_id_by_name(rend_data_ctx, 0);

            draw_pass_3d_shadow_reset(render_manager.shadow_ptr, rend_data_ctx);

            if (render_manager.get_npr_param() == 1 && sss->enable && sss->npr_contour) {
                rend_data_ctx.uniform->arr[U_NPR] = 1;
                draw_pass_sss_contour(rend, rend_data_ctx);
            }

            draw_pass_sss_filter(sss, rend_data_ctx);

            reflect_draw[rend_data_ctx.index] = false;
            rend->bind_render_texture(rend_data_ctx.state);
            float_t render_width = 0.0f;
            float_t render_height = 0.0f;
            rend->get_render_resolution(&render_width, &render_height, 0, 0);
        }
        rend_data_ctx.set_npr(&render_manager);
    }

    rend_data_ctx.state.end_event();
}

void rndr__RenderManager__pass_3d__DispGlitterXScenesLocal_impl(
    render_data_context& rend_data_ctx, rndr::RenderManager& render_manager) {
    if (Glitter::glt_particle_manager_x->CheckHasLocalEffect()) { // X
        rend_data_ctx.state.begin_event("local");

        cam_data local_cam;
        local_cam.get(32.2673416137695f);
        rend_data_ctx.set_batch_scene_camera(local_cam);

        local_draw[rend_data_ctx.index] = true;
        if (render_manager.alpha_z_sort) {
            mdl::DispManager::obj_sort(rend_data_ctx,
                mdl::OBJ_TYPE_LOCAL_TRANSLUCENT, 1, local_cam);
            mdl::DispManager::obj_sort(rend_data_ctx,
                mdl::OBJ_TYPE_LOCAL_TRANSLUCENT_ALPHA_ORDER_POST_TRANSLUCENT, 1, local_cam);
        }

        if (render_manager.opaque_z_sort)
            mdl::DispManager::obj_sort(rend_data_ctx, mdl::OBJ_TYPE_LOCAL_OPAQUE, 0, local_cam);

        mdl::DispManager::draw(rend_data_ctx, mdl::OBJ_TYPE_LOCAL_OPAQUE, local_cam, 0, -1, true);
        mdl::DispManager::draw(rend_data_ctx, mdl::OBJ_TYPE_LOCAL_TRANSPARENT, local_cam, 0, -1, true);
        draw_state.set_blend(rend_data_ctx, true);
        mdl::DispManager::draw(rend_data_ctx, mdl::OBJ_TYPE_LOCAL_TRANSLUCENT, local_cam, 0, -1, true);
        draw_state.set_blend(rend_data_ctx, false);

        draw_pass_3d_translucent(rend_data_ctx,
            mdl::OBJ_TYPE_LOCAL_OPAQUE_ALPHA_ORDER_POST_TRANSLUCENT,
            mdl::OBJ_TYPE_LOCAL_TRANSPARENT_ALPHA_ORDER_POST_TRANSLUCENT,
            mdl::OBJ_TYPE_LOCAL_TRANSLUCENT_ALPHA_ORDER_POST_TRANSLUCENT, local_cam);
        local_draw[rend_data_ctx.index] = false;

        if (render_manager.glitter)
            Glitter::glt_particle_manager_x->DispScenes(rend_data_ctx, local_cam, Glitter::DISP_LOCAL);

        rend_data_ctx.state.end_event();

        rend_data_ctx.set_batch_scene_camera(render_manager.cam[0]);
    }
}

HOOK(void, FASTCALL, sub_1402B9460, 0x00000001402B9460, __int64 a1, render_data::obj_npr_param_data* data) {
    originalsub_1402B9460(a1, data);

    extern bool reflect_full;
    if (reflect_full && reflect_full_ptr)
        data->g_npr_param[7].x = reflection_quality_full_get();
}

HOOK(void, FASTCALL, rndr__Render__draw_npr_frame, 0x00000001404A52B0, rndr::Render* This, render_data_context& rend_data_ctx) {
    if (reflect_draw[rend_data_ctx.index] && reflect_full_ptr) {
        RenderTexture& refl_tex = reflect_full_ptr->reflect_texture;
        RenderTexture& refl_buf_tex = reflect_full_ptr->reflect_buffer_texture;
        if (!sub_1402C1D20()) {
            refl_buf_tex.Bind(rend_data_ctx.state);
            rend_data_ctx.state.clear_render_target_view(0.0f, 0.0f, 0.0f, 0.0f);
            rend_data_ctx.state.clear_depth_stencil_view(0, 0.0f, false);
            This->render_ptr->draw_sss_contour(rend_data_ctx,
                &refl_buf_tex.GetColorTex(), &refl_buf_tex.GetDepthTex(),
                refl_tex.GetWidth(), refl_tex.GetHeight(),
                &reflect_full_ptr->reflect_nonsss_tex->tex, &refl_tex.GetDepthTex(),
                refl_tex.GetWidth(), refl_tex.GetHeight(), 1.0f, 1.0f, true, reflection_quality_full_get());
        }

        reflect_full_ptr->reflect_contour_texture.Bind(rend_data_ctx.state);
        This->render_ptr->draw_npr_frame(rend_data_ctx,
            This->render_post_width_scale, This->render_post_height_scale,
            &refl_buf_tex.GetColorTex(), &refl_buf_tex.GetDepthTex(),
            &refl_tex.GetDepthTex(), reflection_quality_full_get());
        refl_tex.Bind(rend_data_ctx.state);
    }
    else {
        if (!sub_1402C1D20()) {
            This->sss_contour_texture->Bind(rend_data_ctx.state);
            rend_data_ctx.state.clear_render_target_view(0.0f, 0.0f, 0.0f, 0.0f);
            rend_data_ctx.state.clear_depth_stencil_view(0, 0.0f, false);
            This->render_ptr->draw_sss_contour(rend_data_ctx,
                &This->sss_contour_texture->GetColorTex(), &This->sss_contour_texture->GetDepthTex(),
                This->render_width[0], This->render_height[0],
                &This->rend_nonsss_tex->tex, &This->rend_texture[0].GetDepthTex(),
                This->render_post_width[0], This->render_post_height[0],
                This->render_post_width_scale,
                This->render_post_height_scale, true, This->render_ptr->image_quality);
        }

        This->contour_texture.Bind(rend_data_ctx.state);
        This->render_ptr->draw_npr_frame(rend_data_ctx,
            This->render_post_width_scale, This->render_post_height_scale,
            &This->sss_contour_texture->GetColorTex(), &This->sss_contour_texture->GetDepthTex(),
            &This->rend_texture[0].GetDepthTex(), This->render_ptr->image_quality);
        This->bind_render_texture(rend_data_ctx.state);
    }
}

HOOK(void, FASTCALL, rndr__Render__transparency_copy, 0x00000001404A6EA0,
    rndr::Render * This, render_data_context& rend_data_ctx) {
    if (reflect_draw[rend_data_ctx.index] && reflect_full_ptr) {
        RenderTexture& refl_tex = reflect_full_ptr->reflect_texture;
        const float_t reflection_quality = reflection_quality_full_get();
        int32_t width = (int32_t)((float_t)refl_tex.GetWidth() * reflection_quality);
        int32_t height = (int32_t)((float_t)refl_tex.GetHeight() * reflection_quality);
        rend_data_ctx.state.copy_texture_region(&This->transparency->texture,
            0, 0, &refl_tex.GetColorTex(), 0, 0, width, height);
        rend_data_ctx.state.set_render_target(&This->transparency->render_target);
        rend_data_ctx.state.set_viewport(0, 0, width, height);
    }
    else {
        float_t image_quality = This->render_ptr->image_quality;
        int32_t width = app::get_value_scaled(This->transparency->texture.get_width(), image_quality);
        int32_t height = app::get_value_scaled(This->transparency->texture.get_height(), image_quality);
        rend_data_ctx.state.copy_texture_region(&This->transparency->texture,
            0, 0, &This->rend_texture[0].GetColorTex(), 0, 0, width, height);
        rend_data_ctx.state.set_render_target(&This->transparency->render_target);
        rend_data_ctx.state.set_viewport(0, 0, width, height);
    }
}

HOOK(void, FASTCALL, rndr__Render__transparency_combine, 0x00000001404A6F60,
    rndr::Render* This, render_data_context& rend_data_ctx, float_t alpha) {
    static void (*renderer__Transparency__combine)(renderer::Transparency * This, render_data_context & rend_data_ctx,
        RenderTexture * dst, p_dx_texture * buf, float_t alpha, float_t quality)
        = (void (*)(renderer::Transparency * This, render_data_context & rend_data_ctx,
            RenderTexture * dst, p_dx_texture * buf, float_t alpha, float_t quality))0x00000014049A540;
    if (reflect_draw[rend_data_ctx.index] && reflect_full_ptr) {
        RenderTexture& refl_tex = reflect_full_ptr->reflect_texture;
        renderer__Transparency__combine(This->transparency, rend_data_ctx,
            &refl_tex, &This->render_buffer, alpha, 1.0f);
    }
    else
        renderer__Transparency__combine(This->transparency, rend_data_ctx,
            &This->rend_texture[0], &This->render_buffer, alpha, This->render_ptr->image_quality);
}

HOOK(void, FASTCALL, render_data_context__set_batch_scene_camera,
    0x00000001404D5090, render_data_context& This, cam_data& cam) {
    render_data* data = This.data;
    data->cam = cam;

    mat4 view_mat;
    view_mat.row0 = data->cam.view_mat[0];
    view_mat.row1 = data->cam.view_mat[1];
    view_mat.row2 = data->cam.view_mat[2];
    view_mat.row3 = { 0.0f, 0.0f, 0.0f, 1.0f };

    extern bool reflect_draw[4];
    extern mat4 reflect_mat;
    if (reflect_draw[This.index]) {
        mat4_transpose(&view_mat, &view_mat);
        mat4_mul(&reflect_mat, &view_mat, &view_mat);
        mat4_transpose(&view_mat, &view_mat);

        data->cam.view_mat[0] = view_mat.row0;
        data->cam.view_mat[1] = view_mat.row1;
        data->cam.view_mat[2] = view_mat.row2;

        mat4_mul(&cam.proj_mat, &view_mat, &data->cam.view_proj_mat);
    }

    data->buffer_scene_data.g_view[0] = view_mat.row0;
    data->buffer_scene_data.g_view[1] = view_mat.row1;
    data->buffer_scene_data.g_view[2] = view_mat.row2;

    mat4 temp;
    mat4_invert(&view_mat, &temp);
    data->buffer_scene_data.g_view_inverse[0] = temp.row0;
    data->buffer_scene_data.g_view_inverse[1] = temp.row1;
    data->buffer_scene_data.g_view_inverse[2] = temp.row2;

    mat4_mul(&cam.proj_mat, &view_mat, &temp);
    data->buffer_scene_data.g_projection_view[0] = temp.row0;
    data->buffer_scene_data.g_projection_view[1] = temp.row1;
    data->buffer_scene_data.g_projection_view[2] = temp.row2;
    data->buffer_scene_data.g_projection_view[3] = temp.row3;

    vec3 view_position = data->cam.view_point;
    if (reflect_draw[This.index]) {
        mat4_transform_point(&reflect_mat, &view_position, &view_position);
        data->cam.view_point = view_position;
    }

    data->buffer_scene_data.g_view_position.x = view_position.x;
    data->buffer_scene_data.g_view_position.y = view_position.y;
    data->buffer_scene_data.g_view_position.z = view_position.z;
    data->buffer_scene_data.g_view_position.w = 0.0f;

    const float_t min_distance = data->cam.min_distance;
    const float_t max_distance = data->cam.max_distance;
    data->buffer_scene_data.g_forward_z_projection_row2.x = 0.0f;
    data->buffer_scene_data.g_forward_z_projection_row2.y = 0.0f;
    data->buffer_scene_data.g_forward_z_projection_row2.z = (max_distance + min_distance)
        * (1.0f / (min_distance - max_distance));
    data->buffer_scene_data.g_forward_z_projection_row2.w = (min_distance + min_distance) * max_distance
        * (1.0f / (min_distance - max_distance));
    enum_or(data->flags, RENDER_DATA_SCENE_UPDATE | RENDER_DATA_BATCH_UPDATE);
}

HOOK(void, FASTCALL, render_manager_free_render_data, 0x00000001404D7920) {
    if (reflect_full_ptr)
        reflect_full_ptr->free();
    originalrender_manager_free_render_data();
}

HOOK(void, FASTCALL, render_manager_init_render_data, 0x00000001404D7730,
    int32_t ssaa, int32_t hd_res, int32_t ss_alpha_mask, bool resize) {
    originalrender_manager_init_render_data(ssaa, hd_res, ss_alpha_mask, resize);
    extern bool config_reflect_full;
    if (reflect_full_ptr && config_reflect_full)
        reflect_full_ptr->init();
}

HOOK(void, FASTCALL, rndr__RenderManager__pass_reflect, 0x00000001404D9590, render_data_context& rend_data_ctx, rndr::RenderManager& render_manager) {
    extern bool reflect_full;
    if (reflect_full && reflect_full_ptr) {
        draw_pass_reflect_full(rend_data_ctx, render_manager);
        return;
    }

    originalrndr__RenderManager__pass_reflect(rend_data_ctx, render_manager);
}

HOOK(void, FASTCALL, RenderTexture__SetColorTextureAtBinding, 0x00000001404DB1D0, RenderTexture* This, render_data_context& rend_data_ctx) {
    extern bool reflect_full;
    if (reflect_full && reflect_full_ptr)
        This = &reflect_full_ptr->reflect_texture;
    originalRenderTexture__SetColorTextureAtBinding(This, rend_data_ctx);
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
    sss_inverse_scale = inverse_scale;
    sss_param_reflect = { sss_strength, 0.0f, 0.0f, 0.0f };
}

HOOK(void, FASTCALL, rndr__RenderManager__rndpass_pre_proc_mid, 0x00000001404D8024);
HOOK(void, FASTCALL, rndr__RenderManager__pass_ss_sss_mid, 0x00000001404D9439);
HOOK(void, FASTCALL, rndr__RenderManager__pass_3d__DispGlitterXScenesOpaque, 0x00000001404DA5F7);
HOOK(void, FASTCALL, rndr__RenderManager__pass_3d__DispGlitterXScenesAlpha, 0x00000001404DA7C7);
HOOK(void, FASTCALL, rndr__RenderManager__pass_3d__DispGlitterXScenesType2, 0x00000001404DA84A);
HOOK(void, FASTCALL, rndr__RenderManager__pass_3d__DispGlitterXScenesNormal, 0x00000001404DA87A);
HOOK(void, FASTCALL, rndr__RenderManager__pass_3d__DispGlitterXScenesLocal, 0x00000001404DA894);

void reflect_full_init() {
    if (!reflect_full_ptr)
        reflect_full_ptr = new reflect_full_struct;
}

void reflect_full_free() {
    if (reflect_full_ptr) {
        delete reflect_full_ptr;
        reflect_full_ptr = 0;
    }
}

void render_manager_patch() {
    // Free up space for rndr__RenderManager__pass_3d__DispGlitterXScenesLocal func call
    WRITE_MEMORY_STRING(0x00000001404DA5CC, "\x0F\x84\xD5\x02\x00\x00", 0x06);
    WRITE_NOP_5(0x00000001404DA894);
    WRITE_MEMORY_STRING(0x00000001404DA899, "\xEB\x11"
        "\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC", 0x0E);

    // Reflect Full resolution scale fix on 3D pass
    INSTALL_HOOK(sub_1402B9460);

    INSTALL_HOOK(rndr__Render__draw_npr_frame);
    INSTALL_HOOK(rndr__Render__transparency_copy);
    INSTALL_HOOK(rndr__Render__transparency_combine);
    INSTALL_HOOK(render_data_context__set_batch_scene_camera);
    INSTALL_HOOK(render_manager_free_render_data);
    INSTALL_HOOK(render_manager_init_render_data);
    INSTALL_HOOK(rndr__RenderManager__pass_reflect);
    INSTALL_HOOK(RenderTexture__SetColorTextureAtBinding);
    INSTALL_HOOK(sss_data__pre_proc);

    INSTALL_HOOK(rndr__RenderManager__rndpass_pre_proc_mid);
    INSTALL_HOOK(rndr__RenderManager__pass_ss_sss_mid);
    INSTALL_HOOK(rndr__RenderManager__pass_3d__DispGlitterXScenesOpaque);
    INSTALL_HOOK(rndr__RenderManager__pass_3d__DispGlitterXScenesAlpha);
    INSTALL_HOOK(rndr__RenderManager__pass_3d__DispGlitterXScenesType2);
    INSTALL_HOOK(rndr__RenderManager__pass_3d__DispGlitterXScenesNormal);
    INSTALL_HOOK(rndr__RenderManager__pass_3d__DispGlitterXScenesLocal);
}

reflect_full_struct::reflect_full_struct() : reflect_nonsss_tex() {

}

reflect_full_struct::~reflect_full_struct() {
    free();
}

void reflect_full_struct::free() {
    reflect_contour_texture.Free();
    reflect_nonsss_texture.Free();

    if (reflect_nonsss_tex) {
        texture_release(reflect_nonsss_tex);
        reflect_nonsss_tex = 0;
    }

    reflect_buffer_texture.Free();
    reflect_texture.Free();
}

void reflect_full_struct::init() {
    rndr::Render* render = render_manager.render;

    reflect_texture.Init(render->render_width[0], render->render_height[0], 0, 23, 27);
    reflect_buffer_texture.Init(render->render_width[0], render->render_height[0], 0, 23, 27);

    reflect_nonsss_tex = texture_load_tex_2d(0x25000000 | render_get()->texture_counter++,
        23, render->render_width[0], render->render_height[0], 0, 0, 0, false);

    p_dx_texture* color_texs[2];
    color_texs[0] = &reflect_texture.GetColorTex();
    color_texs[1] = &reflect_nonsss_tex->tex;
    reflect_nonsss_texture.SetColorDepthTextures(color_texs, 2, 0, reflect_texture.GetDepthTex());

    p_dx_texture empty_tex;
    reflect_contour_texture.SetColorDepthTexture(reflect_texture.GetColorTex(), 0, empty_tex);
}

void get_reflect_mat() {
    vec4 reflect_clip_plane;
    const light_set* set = &light_set_data[LIGHT_SET_MAIN];
    const light_data* light_reflect = &set->lights[LIGHT_REFLECT];
    if (light_reflect->type == LIGHT_SPOT) {
        const vec3 spot_direction = -light_reflect->spot_direction;
        const vec3 position = *(vec3*)&light_reflect->position;

        float_t length = vec3::length(spot_direction);
        if (length > 0.000001f)
            *(vec3*)&reflect_clip_plane = spot_direction * (1.0f / length);
        else
            *(vec3*)&reflect_clip_plane = { 0.0f, 1.0f, 0.0f };

        reflect_clip_plane.w = -vec3::dot(position, *(vec3*)&reflect_clip_plane);
    }
    else if (light_reflect->type == LIGHT_POINT)
        reflect_clip_plane = { 0.0f, -1.0f, 0.0f, 9999.0f };
    else
        reflect_clip_plane = { 0.0f, -1.0f, 0.0f, 0.0f };

    const vec4 temp = 2.0f * reflect_clip_plane;

    reflect_mat = mat4_identity;
    *(vec3*)&reflect_mat.row0 -= temp.x * *(vec3*)&reflect_clip_plane;
    *(vec3*)&reflect_mat.row1 -= temp.y * *(vec3*)&reflect_clip_plane;
    *(vec3*)&reflect_mat.row2 -= temp.z * *(vec3*)&reflect_clip_plane;
    *(vec3*)&reflect_mat.row3 -= temp.w * *(vec3*)&reflect_clip_plane;
    reflect_mat.row0.w = 0.0f;
    reflect_mat.row1.w = 0.0f;
    reflect_mat.row2.w = 0.0f;
    reflect_mat.row3.w = 1.0f;
}

static void draw_npr_frame(render_data_context& rend_data_ctx) {
    rend_data_ctx.state.begin_event("`anonymous-namespace'::draw_npr_frame");
    render_manager.render->draw_npr_frame(rend_data_ctx);
    rend_data_ctx.state.end_event();
}

static void draw_pass_sss_contour(rndr::Render* rend, render_data_context& rend_data_ctx) {
    if (reflect_draw[rend_data_ctx.index] && reflect_full_ptr) {
        RenderTexture& refl_tex = reflect_full_ptr->reflect_texture;
        RenderTexture& refl_buf_tex = reflect_full_ptr->reflect_buffer_texture;
        refl_buf_tex.Bind(rend_data_ctx.state);
        rend->render_ptr->draw_sss_contour(rend_data_ctx,
            &refl_tex.GetColorTex(), &refl_tex.GetDepthTex(),
            refl_tex.GetWidth(), refl_tex.GetHeight(),
            &refl_tex.GetColorTex(), &refl_tex.GetDepthTex(),
            refl_tex.GetWidth(), refl_tex.GetHeight(), 1.0f, 1.0f, false, reflection_quality_full_get());
    }
    else {
        rend->sss_contour_texture->Bind(rend_data_ctx.state);
        rend->render_ptr->draw_sss_contour(rend_data_ctx,
            &rend->rend_texture[0].GetColorTex(), &rend->rend_texture[0].GetDepthTex(),
            rend->render_width[0], rend->render_height[0],
            &rend->rend_texture[0].GetColorTex(), &rend->rend_texture[0].GetDepthTex(),
            rend->render_post_width[0], rend->render_post_height[0],
            rend->render_post_width_scale,
            rend->render_post_height_scale, false, rend->render_ptr->image_quality);
    }
}

static void draw_pass_sss_filter(sss_data* sss, render_data_context& rend_data_ctx) {
    const int32_t sss_count = 6;

    dx_sss_struct* dx = sss->dx;

    rend_data_ctx.set_batch_sss_param(reflect_draw[rend_data_ctx.index] ? sss_param_reflect : dx->sss_param);
    if (sss->npr_contour) {
        sss->textures[0].Bind(rend_data_ctx.state);

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

    sss->textures[2].Bind(rend_data_ctx.state);
    RenderTexture& rt0 = sss->textures[0];
    rend_data_ctx.state.set_viewport(0, 0, 320, 180);
    rend_data_ctx.state.set_ps_textures(0, 1, &rt0.GetColorTex());
    rend_data_ctx.state.set_ps_sampler_state(0, 1, &dx->sampler_state);
    rend_data_ctx.state.set_vs_shader(&dx->shaders[rend_data_ctx.get_uniform_value(U_NPR) == 1 ? 2 : 1].vertex);
    rend_data_ctx.state.set_ps_shader(&dx->shaders[rend_data_ctx.get_uniform_value(U_NPR) == 1 ? 2 : 1].pixel);
    dx->draw_quad(rend_data_ctx, rt0.GetWidth(), rt0.GetHeight(), 1.0, 1.0, 0.0f, 0.0f, 1.0f);

    const double_t weights[] = { 0.4, 0.3, 0.3 };
    const double_t r_radius[] = { 1.0, 2.0, 5.0 };
    const double_t g_radius[] = { 0.2, 0.4, 1.2 };
    const double_t b_radius[] = { 0.3, 0.7, 2.0 };
    dx->calc_coef(rend_data_ctx, 0, 0, sss_inverse_scale, 0, weights, r_radius, g_radius, b_radius);

    sss->textures[reflect_draw[rend_data_ctx.index] ? 3 : 1].Bind(rend_data_ctx.state);
    RenderTexture& rt1 = sss->textures[2];
    rend_data_ctx.state.set_viewport(0, 0, 320, 180);
    rend_data_ctx.state.set_ps_textures(0, 1, &rt1.GetColorTex());
    rend_data_ctx.state.set_ps_sampler_state(0, 1, &dx->sampler_state);
    rend_data_ctx.state.set_vs_shader(&dx->shaders[3].vertex);
    rend_data_ctx.state.set_ps_shader(&dx->shaders[3].pixel);
    rend_data_ctx.state.set_ps_constant_buffer(1, 1, &dx->buffer_gaussian_coef);
    dx->draw_quad(rend_data_ctx, rt1.GetWidth(), rt1.GetHeight(), 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
}

static void draw_pass_3d_translucent(render_data_context& rend_data_ctx, mdl::ObjType opaque,
    mdl::ObjType transparent, mdl::ObjType translucent, cam_data& cam) {
    if (disp_manager->get_obj_count(opaque) < 1
        && disp_manager->get_obj_count(transparent) < 1
        && disp_manager->get_obj_count(translucent) < 1)
        return;

    static void (*rndr__Render__transparency_copy)(rndr::Render * This, render_data_context & rend_data_ctx)
        = (void (*)(rndr::Render * This, render_data_context & rend_data_ctx))0x00000001404A6EA0;
    static void (*rndr__Render__transparency_combine)(rndr::Render * This, render_data_context & rend_data_ctx, float_t alpha)
        = (void (*)(rndr::Render * This, render_data_context & rend_data_ctx, float_t alpha))0x00000001404A6F60;

    int32_t alpha_array[0x100];
    int32_t count = draw_pass_3d_translucent_count_layers(rend_data_ctx, alpha_array, opaque, transparent, translucent, cam);
    for (int32_t i = 0; i < count; i++) {
        int32_t alpha = alpha_array[i];
        rndr__Render__transparency_copy(render_manager.render, rend_data_ctx);
        if (render_manager.draw_pass_3d[DRAW_PASS_3D_TRANSLUCENT] && disp_manager->get_obj_count(opaque))
            mdl::DispManager::draw(rend_data_ctx, opaque, cam, 0, -1, 0, 0, true, alpha);
        if (render_manager.draw_pass_3d[DRAW_PASS_3D_TRANSLUCENT] && disp_manager->get_obj_count(transparent))
            mdl::DispManager::draw(rend_data_ctx, transparent, cam, 0, -1, 0, 0, true, alpha);
        if (render_manager.draw_pass_3d[DRAW_PASS_3D_TRANSLUCENT] && disp_manager->get_obj_count(translucent)) {
            draw_state.set_blend(rend_data_ctx, true);
            mdl::DispManager::draw(rend_data_ctx, translucent, cam, 0, -1, 0, 0, true, alpha);
            draw_state.set_blend(rend_data_ctx, false);
            rend_data_ctx.state.set_blend_state(dx_default_states_get_blend_state(false, DX_BLEND_SRC_ALPHA,
                DX_BLEND_INV_SRC_ALPHA, DX_BLEND_SRC_ALPHA,
                DX_BLEND_INV_SRC_ALPHA, DX_BLEND_WRITE_MASK_RGBA));
        }
        rndr__Render__transparency_combine(render_manager.render, rend_data_ctx, (float_t)alpha * (float_t)(1.0 / 255.0));
    }
}

static void draw_pass_3d_translucent(render_data_context& rend_data_ctx, mdl::ObjTypeLocal opaque,
    mdl::ObjTypeLocal transparent, mdl::ObjTypeLocal translucent, cam_data& cam) {
    if (disp_manager->get_obj_count(opaque) < 1
        && disp_manager->get_obj_count(transparent) < 1
        && disp_manager->get_obj_count(translucent) < 1)
        return;

    static void (*rndr__Render__transparency_copy)(rndr::Render * This, render_data_context & rend_data_ctx)
        = (void (*)(rndr::Render * This, render_data_context & rend_data_ctx))0x00000001404A6EA0;
    static void (*rndr__Render__transparency_combine)(rndr::Render * This, render_data_context & rend_data_ctx, float_t alpha)
        = (void (*)(rndr::Render * This, render_data_context & rend_data_ctx, float_t alpha))0x00000001404A6F60;

    int32_t alpha_array[0x100];
    int32_t count = draw_pass_3d_translucent_count_layers(rend_data_ctx, alpha_array, opaque, transparent, translucent, cam);
    for (int32_t i = 0; i < count; i++) {
        int32_t alpha = alpha_array[i];
        rndr__Render__transparency_copy(render_manager.render, rend_data_ctx);
        if (render_manager.draw_pass_3d[DRAW_PASS_3D_TRANSLUCENT] && disp_manager->get_obj_count(opaque))
            mdl::DispManager::draw(rend_data_ctx, opaque, cam, 0, -1, alpha);
        if (render_manager.draw_pass_3d[DRAW_PASS_3D_TRANSLUCENT] && disp_manager->get_obj_count(transparent))
            mdl::DispManager::draw(rend_data_ctx, transparent, cam, 0, -1, alpha);
        if (render_manager.draw_pass_3d[DRAW_PASS_3D_TRANSLUCENT] && disp_manager->get_obj_count(translucent)) {
            draw_state.set_blend(rend_data_ctx, true);
            mdl::DispManager::draw(rend_data_ctx, translucent, cam, 0, -1, alpha);
            draw_state.set_blend(rend_data_ctx, false);
            rend_data_ctx.state.set_blend_state(dx_default_states_get_blend_state(false, DX_BLEND_SRC_ALPHA,
                DX_BLEND_INV_SRC_ALPHA, DX_BLEND_SRC_ALPHA,
                DX_BLEND_INV_SRC_ALPHA, DX_BLEND_WRITE_MASK_RGBA));
        }
        rndr__Render__transparency_combine(render_manager.render, rend_data_ctx, (float_t)alpha * (float_t)(1.0 / 255.0));
    }
}

static int32_t draw_pass_3d_translucent_count_layers(render_data_context& rend_data_ctx, int32_t alpha_array[0x100],
    mdl::ObjType opaque, mdl::ObjType transparent, mdl::ObjType translucent, cam_data& cam) {
    bool arr[0x100] = { false };

    draw_pass_3d_translucent_has_objects(arr,
        mdl::DispManager::get_obj_data_list_sort(rend_data_ctx.index, opaque), cam);
    draw_pass_3d_translucent_has_objects(arr,
        mdl::DispManager::get_obj_data_list_sort(rend_data_ctx.index, transparent), cam);
    draw_pass_3d_translucent_has_objects(arr,
        mdl::DispManager::get_obj_data_list_sort(rend_data_ctx.index, translucent), cam);

    int32_t count = 0;
    for (int32_t i = 0xFF; i >= 1; i--)
        if (arr[i]) {
            count++;
            *alpha_array++ = i;
        }
    return count;
}

static int32_t draw_pass_3d_translucent_count_layers(render_data_context& rend_data_ctx, int32_t alpha_array[0x100],
    mdl::ObjTypeLocal opaque, mdl::ObjTypeLocal transparent, mdl::ObjTypeLocal translucent, cam_data& cam) {
    bool arr[0x100] = { false };

    draw_pass_3d_translucent_has_objects(arr,
        mdl::DispManager::get_obj_data_list_sort(rend_data_ctx.index, opaque), cam);
    draw_pass_3d_translucent_has_objects(arr,
        mdl::DispManager::get_obj_data_list_sort(rend_data_ctx.index, transparent), cam);
    draw_pass_3d_translucent_has_objects(arr,
        mdl::DispManager::get_obj_data_list_sort(rend_data_ctx.index, translucent), cam);

    int32_t count = 0;
    for (int32_t i = 0xFF; i >= 1; i--)
        if (arr[i]) {
            count++;
            *alpha_array++ = i;
        }
    return count;
}

static void draw_pass_3d_translucent_has_objects(bool* arr, mdl::ObjListSort& obj_list_sort, cam_data& cam) {
    static void (*draw_task_add_alpha_center_mat_center)(mdl::ObjListSort & obj_list_sort, cam_data & cam)
        = (void (*)(mdl::ObjListSort & obj_list_sort, cam_data & cam))0x000000014045D4B0;

    int32_t count = obj_list_sort.count;
    mdl::ObjDataSort* data_sort = obj_list_sort.data;

    for (; count; count--, data_sort++) {
        mdl::ObjData* data = data_sort->data;
        switch (data->kind) {
        case mdl::OBJ_KIND_NORMAL: {
            int32_t alpha = (int32_t)(data->args.sub_mesh.ptr->blend_color.w * 255.0f);
            alpha = clamp_def(alpha, 0, 255);
            arr[alpha] = true;
        } break;
        case mdl::OBJ_KIND_TRANSLUCENT: {
            for (int32_t j = 0; j < data->args.translucent.ptr->count; j++) {
                int32_t alpha = (int32_t)(data->args.translucent.ptr->sub_mesh[j]->blend_color.w * 255.0f);
                alpha = clamp_def(alpha, 0, 255);
                arr[alpha] = true;
            }
        } break;
        }
    }
}

static void draw_pass_reflect_full(render_data_context& rend_data_ctx, rndr::RenderManager& render_manager) {
    rend_data_ctx.state.begin_event("pass_reflect");
    RenderTexture& refl_tex = reflect_full_ptr->reflect_texture;
    RenderTexture& refl_buf_tex = reflect_full_ptr->reflect_buffer_texture;
    RenderTexture& refl_nonsss_tex = reflect_full_ptr->reflect_nonsss_texture;

    if (sub_1402C1D20())
        refl_tex.Bind(rend_data_ctx.state);
    else
        refl_nonsss_tex.Bind(rend_data_ctx.state);

    if (!sub_1402C1D20())
        rend_data_ctx.state.clear_render_target_view(0.0f, 0.0f, 0.0f, 0.0f);

    extern bool reflect_full;
    if (draw_pass_reflect_get_obj_reflect_surface()
        && (disp_manager->get_obj_count(mdl::OBJ_TYPE_REFLECT_OPAQUE)
            || disp_manager->get_obj_count(mdl::OBJ_TYPE_REFLECT_TRANSPARENT)
            || disp_manager->get_obj_count(mdl::OBJ_TYPE_REFLECT_TRANSLUCENT_SORT_BY_RADIUS)
            || disp_manager->get_obj_count(mdl::OBJ_TYPE_REFLECT_TRANSLUCENT))) {
        reflect_draw[rend_data_ctx.index] = true;
        draw_state.shader_index = -1;

        const float_t reflection_quality = reflection_quality_full_get();
        rend_data_ctx.state.set_viewport(0, 0,
            (int32_t)((float_t)refl_tex.GetWidth() * reflection_quality),
            (int32_t)((float_t)refl_tex.GetHeight() * reflection_quality));

        p_dx_texture* default_texture = dx_default_states_get_texture(1);
        rend_data_ctx.state.set_vs_textures(15, 1, default_texture);
        rend_data_ctx.state.set_ps_textures(15, 1, default_texture);
        rend_data_ctx.uniform->arr[U_WATER_REFLECT] = 0;

        p_dx_sampler_state* default_sampler_state = dx_default_states_get_sampler_state(
            DX_TEXTURE_ADDRESS_CLAMP, DX_TEXTURE_ADDRESS_CLAMP, false);
        rend_data_ctx.state.set_ps_sampler_state(15, 1, default_sampler_state);
        rend_data_ctx.uniform->arr[U_STAGE_AMBIENT] = render_manager.stage_ambient;

        p_dx_texture* effect_texture = render_manager.effect_texture
            ? &render_manager.effect_texture->tex : dx_default_states_get_texture(1);
        rend_data_ctx.state.set_vs_textures(14, 1, effect_texture);
        rend_data_ctx.state.set_vs_sampler_state(14, 1, default_sampler_state);
        rend_data_ctx.state.set_ps_textures(14, 1, effect_texture);
        rend_data_ctx.state.set_ps_sampler_state(14, 1, default_sampler_state);

        sss_data_get()->set_texture(rend_data_ctx, 3);

        rend_data_ctx.set_npr(&render_manager);
        rend_data_ctx.set_batch_sss_param(sss_param_reflect);

        if (!sss_data_get()->enable || !sss_data_get()->npr_contour || !sub_1402C1D20() || sub_1404D9FD0())
            rend_data_ctx.state.clear_depth_stencil_view(0x00, 0.0f, true);

        lighting_set(rend_data_ctx);
        fog_set(rend_data_ctx, 0); // FOG_DEPTH
        fog_set(rend_data_ctx, 1); // FOG_HEIGHT
        fog_set(rend_data_ctx, 2); // FOG_BUMP

        if (render_manager.shadow)
            draw_pass_3d_shadow_set(render_manager.shadow_ptr, rend_data_ctx);
        else
            draw_pass_3d_shadow_reset(render_manager.shadow_ptr, rend_data_ctx);

        cam_data cam = render_manager.cam[0];
        rend_data_ctx.set_batch_scene_camera(cam);

        if (render_manager.alpha_z_sort) {
            mdl::DispManager::obj_sort(rend_data_ctx,
                mdl::OBJ_TYPE_REFLECT_TRANSLUCENT, 1, cam, render_manager.field_31F);
            mdl::DispManager::obj_sort(rend_data_ctx,
                mdl::OBJ_TYPE_REFLECT_TRANSLUCENT_SORT_BY_RADIUS, 2, cam);
        }

        if (render_manager.opaque_z_sort)
            mdl::DispManager::obj_sort(rend_data_ctx, mdl::OBJ_TYPE_REFLECT_OPAQUE, 0, cam);

        if (render_manager.draw_pass_3d[DRAW_PASS_3D_OPAQUE])
            mdl::DispManager::draw(rend_data_ctx, mdl::OBJ_TYPE_REFLECT_OPAQUE, cam);

        if (render_manager.glitter)
            Glitter::glt_particle_manager_x->DispScenes(rend_data_ctx, cam, Glitter::DISP_OPAQUE, true);

        if (render_manager.draw_pass_3d[DRAW_PASS_3D_TRANSPARENT])
            mdl::DispManager::draw(rend_data_ctx, mdl::OBJ_TYPE_REFLECT_TRANSPARENT, cam);

        static bool (*sub_1402C1AD0)() = (bool (*)())0x00000001402C1AD0;
        if (sub_1402C1AD0() || render_manager.get_npr_param() == 1) {
            draw_npr_frame(rend_data_ctx);

            lighting_set(rend_data_ctx);
            fog_set(rend_data_ctx, 0); // FOG_DEPTH
            fog_set(rend_data_ctx, 1); // FOG_HEIGHT
            fog_set(rend_data_ctx, 2); // FOG_BUMP
        }

        draw_pass_3d_translucent(rend_data_ctx,
            mdl::OBJ_TYPE_OPAQUE_ALPHA_ORDER_POST_OPAQUE,
            mdl::OBJ_TYPE_TRANSPARENT_ALPHA_ORDER_POST_OPAQUE,
            mdl::OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_POST_OPAQUE, cam);

        if (render_manager.glitter)
            Glitter::glt_particle_manager_x->DispScenes(rend_data_ctx, cam, Glitter::DISP_PRE_TRANSLUCENT, true);

        if (render_manager.draw_pass_3d[DRAW_PASS_3D_TRANSLUCENT]) {
            draw_state.set_blend(rend_data_ctx, true);
            mdl::DispManager::draw(rend_data_ctx, mdl::OBJ_TYPE_REFLECT_TRANSLUCENT_SORT_BY_RADIUS, cam);
            mdl::DispManager::draw(rend_data_ctx, mdl::OBJ_TYPE_REFLECT_TRANSLUCENT, cam);
            draw_state.set_blend(rend_data_ctx, false);
        }

        if (render_manager.glitter)
            Glitter::glt_particle_manager_x->DispScenes(rend_data_ctx, cam, Glitter::DISP_POST_TRANSLUCENT, true);

        draw_pass_3d_translucent(rend_data_ctx,
            mdl::OBJ_TYPE_OPAQUE_ALPHA_ORDER_POST_TRANSLUCENT,
            mdl::OBJ_TYPE_TRANSPARENT_ALPHA_ORDER_POST_TRANSLUCENT,
            mdl::OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_POST_TRANSLUCENT, cam);

        if (render_manager.glitter)
            Glitter::glt_particle_manager_x->DispScenes(rend_data_ctx, cam, Glitter::DISP_NORMAL, true);

        draw_pass_3d_translucent(rend_data_ctx,
            mdl::OBJ_TYPE_OPAQUE_ALPHA_ORDER_POST_GLITTER,
            mdl::OBJ_TYPE_TRANSPARENT_ALPHA_ORDER_POST_GLITTER,
            mdl::OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_POST_GLITTER, cam);

        if (render_manager.shadow)
            draw_pass_3d_shadow_reset(render_manager.shadow_ptr, rend_data_ctx);

        reflect_draw[rend_data_ctx.index] = false;

        static bool (*sub_1402C2280)() = (bool (*)())0x00000001402C2280;
        if (sub_1402C2280())
            if (!(render_manager.render->field_1938 & 0x01))
                render_manager.render->apply_dof(rend_data_ctx, cam, refl_tex, reflection_quality);

        static void (*blur_filter_apply)(render_data_context & rend_data_ctx, texture * dst, __int64 a3, texture * src, int a5,
            blur_filter_mode filter, const vec2 & res_scale, const vec4 & scale, const vec4 & offset, float_t quality)
            = (void (*)(render_data_context & rend_data_ctx, texture * dst, __int64 a3, texture * src, int a5,
                blur_filter_mode filter, const vec2 & res_scale, const vec4 & scale, const vec4 & offset, float_t quality))0x0000000140422DB0;

        int32_t index = 0;
        for (int32_t i = render_manager.reflect_blur_num, j = 0; i > 0; i--, j++, index = 1 - index) {
            RenderTexture& src_tex = index ? refl_buf_tex : refl_tex;
            RenderTexture& dst_tex = index ? refl_tex : refl_buf_tex;
            dst_tex.Bind(rend_data_ctx.state);
            blur_filter_apply(rend_data_ctx, dst_tex.color_texture[0], 0, src_tex.color_texture[0], 0,
                render_manager.reflect_blur_filter, reflection_quality, 1.0f, 0.0f, reflection_quality);
        }

        if (index == 1) {
            refl_tex.Bind(rend_data_ctx.state);
            image_filter_scale(rend_data_ctx, refl_tex.color_texture[0], 0,
                refl_buf_tex.color_texture[0], 0, 1.0f, reflection_quality);
        }

        rend_data_ctx.reset_render_target(rend_data_ctx.state);
    }
    else
        rend_data_ctx.state.clear_render_target_view(0.0f, 0.0f, 0.0f, 0.0f);
    rend_data_ctx.state.end_event();
}

static bool draw_pass_reflect_get_obj_reflect_surface() {
    return draw_pass_reflect_get_obj_reflect_surface(mdl::OBJ_TYPE_OPAQUE)
        || draw_pass_reflect_get_obj_reflect_surface(mdl::OBJ_TYPE_TRANSLUCENT)
        || draw_pass_reflect_get_obj_reflect_surface(mdl::OBJ_TYPE_TRANSLUCENT_SORT_BY_RADIUS)
        || draw_pass_reflect_get_obj_reflect_surface(mdl::OBJ_TYPE_TRANSPARENT);
}

static bool draw_pass_reflect_get_obj_reflect_surface(mdl::ObjType type) {
    mdl::ObjList& list = disp_manager->get_obj_list(type);

    mdl::ObjListData* data = list.first;
    while (data) {
        for (int32_t i = 0; i < data->count; i++)
            switch (data->data[i]->kind) {
            case mdl::OBJ_KIND_NORMAL: {
                switch (data->data[i]->args.sub_mesh.ptr->material->material.shader.index) {
                case 0x22: // _WATER01
                case 0x2D: // _FLOOR
                case 0x2E: // _PUDDLE
                    return true;
                }
            } break;
            case mdl::OBJ_KIND_TRANSLUCENT: {
                for (int32_t j = 0; j < data->data[i]->args.translucent.ptr->count; j++)
                    switch (data->data[i]->args.translucent.ptr->sub_mesh[j]->material->material.shader.index) {
                    case 0x22: // _WATER01
                    case 0x2D: // _FLOOR
                    case 0x2E: // _PUDDLE
                        return true;
                    }
            } break;
            }
        data = data->next;
    }
    return false;
}

static float_t reflection_quality_full_get() {
    return (reflection_quality_get() - 0.5f) * 1.5f + 0.25f;
}
