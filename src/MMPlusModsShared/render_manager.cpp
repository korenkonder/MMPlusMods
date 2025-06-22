/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "render_manager.hpp"
#include "../KKdLib/default.hpp"
#include "light_param/light.hpp"
#include "mdl/disp_manager.hpp"
#include "renderer/dof.hpp"
#include "rob/rob.hpp"
#include "app.hpp"
#include "render_context.hpp"

rndr::RenderManager& render_manager = *(rndr::RenderManager*)0x00000001417582D0;

float_t(* reflection_quality_get)() = (float_t(*)())0x00000001402C2260;

bool (*sub_1402C1D20)() = (bool (*)())0x00000001402C1D20;

void (*fog_set)(render_data_context& rend_data_ctx, int32_t id)
    = (void (*)(render_data_context & rend_data_ctx, int32_t id))0x00000001403F44B0;

void (*image_filter_scale)(render_data_context& rend_data_ctx, texture* dst, __int64 a3,
    texture* src, __int64 a5, const vec4& scale, float_t quality)
    = (void (*)(render_data_context & rend_data_ctx, texture * dst, __int64 a3,
        texture * src, __int64 a5, const vec4 & scale, float_t quality))0x0000000140422D70;

void (*lighting_set)(render_data_context& rend_data_ctx)
    = (void (*)(render_data_context & rend_data_ctx))0x00000001404318C0;

void (*shader_get_id_by_name)(render_data_context& rend_data_ctx, const char* str)
    = (void (*)(render_data_context & rend_data_ctx, const char* str))0x0000000140460AD0;

rndr::Render* (*render_get)() = (rndr::Render * (*)())0x000000014049F8E0;

bool (*sub_1404D9FD0)() = (bool (*)())0x00000001404D9FD0;

void (*draw_pass_3d_shadow_set)(Shadow* shad, render_data_context& rend_data_ctx)
    = (void (*)(Shadow * shad, render_data_context & rend_data_ctx))0x000000014058F0B0;
void (*draw_pass_3d_shadow_reset)(Shadow* shad, render_data_context& rend_data_ctx)
    = (void (*)(Shadow * shad, render_data_context & rend_data_ctx))0x000000014058F6A0;

namespace rndr {
    struct RenderTextureIndex {
        int32_t arr[3];
    };

    static const RenderTextureIndex* render_manager_render_texture_index_array
        = (const RenderTextureIndex*)0x0000000140C0CC80;

    void Render::Data::draw_npr_frame(render_data_context& rend_data_ctx, float_t width_scale, float_t height_scale,
        p_dx_texture* contour_color_texture, p_dx_texture* contour_depth_texture,
        p_dx_texture* scene_depth_texture, float_t quality) {
        rend_data_ctx.state.set_ps_textures(8, 1, contour_color_texture);
        rend_data_ctx.state.set_ps_sampler_state(8, 1, sampler_states);
        rend_data_ctx.state.set_ps_textures(9, 1, contour_depth_texture);
        rend_data_ctx.state.set_ps_sampler_state(9, 1, sampler_states);
        rend_data_ctx.state.set_ps_textures(10, 1, scene_depth_texture);
        rend_data_ctx.state.set_ps_sampler_state(10, 1, sampler_states);

        const double_t min_distance = camera_data.min_distance;
        const double_t max_distance = camera_data.max_distance;
        rend_data_ctx.state.write_buffer(buffers[7], vec4(
            (float_t)(min_distance * (1.0 / (min_distance - max_distance))),
            (float_t)(-(max_distance * min_distance) * (1.0 / (min_distance - max_distance))),
            (float_t)min_distance, (float_t)max_distance
        ));
        rend_data_ctx.state.set_ps_constant_buffer(1, 1, &buffers[7]);
        rend_data_ctx.state.set_vs_shader(&shaders[48].vertex);
        rend_data_ctx.state.set_ps_shader(&shaders[48].pixel);
        draw_npr_frame_quad(rend_data_ctx, width_scale, height_scale,
            scene_depth_texture->get_width(), scene_depth_texture->get_height(), quality);
    }

    void Render::Data::draw_npr_frame_quad(render_data_context& rend_data_ctx,
        float_t width_scale, float_t height_scale, int32_t width, int32_t height, float_t quality) {
        p_dx_buffer& v9 = field_B0[rend_data_ctx.index][0];
        render_data::quad_shader_data* quad = (render_data::quad_shader_data*)rend_data_ctx.state.map(v9);
        const float_t v12 = width_scale * quality * 0.5f;
        const float_t v11 = height_scale * quality * -0.5f;
        quad->g_texcoord_modifier.x = v12;
        quad->g_texcoord_modifier.y = v11;
        quad->g_texcoord_modifier.z = v12;
        quad->g_texcoord_modifier.w = v11 + height_scale * quality;

        quad->g_texel_size.x = quality / (float_t)width;
        quad->g_texel_size.y = quality / (float_t)height;
        quad->g_texel_size.z = (float_t)width;
        quad->g_texel_size.w = (float_t)height;
        quad->g_color = 1.0f;
        quad->g_texture_lod = 0.0f;
        rend_data_ctx.state.unmap(v9);

        rend_data_ctx.state.set_vs_constant_buffer(0, 1, &v9);
        rend_data_ctx.state.set_ps_constant_buffer(0, 1, &v9);

        p_dx_buffer null_buf;

        int32_t offsets[] = { 0 };
        int32_t strides[] = { 0 };
        p_dx_buffer* buffers[] = { &null_buf };
        rend_data_ctx.state.set_vertex_buffer(0, 1, buffers, strides, offsets);
        rend_data_ctx.state.set_index_buffer(&null_buf, 0, 0);
        rend_data_ctx.state.set_rasterizer_state(&rasterizer_state);
        rend_data_ctx.state.set_depth_stencil_state(&depth_stencil_states[0]);
        rend_data_ctx.state.set_blend_state(&blend_states[3]);
        rend_data_ctx.state.set_primitive_topology(DX_PRIMITIVE_TRIANGLE_STRIP);
        rend_data_ctx.state.draw(4, 0);
    }

    void Render::Data::draw_sss_contour(render_data_context& rend_data_ctx,
        p_dx_texture* dst_color_tex, p_dx_texture* dst_depth_tex, int32_t dst_width, int32_t dst_height,
        p_dx_texture* src_color_tex, p_dx_texture* src_depth_tex, int32_t src_width, int32_t src_height,
        float_t src_width_scale, float_t src_height_scale, bool non_sss, float_t quality) {
        rend_data_ctx.state.set_viewport(0, 0, (int32_t)(dst_width * quality),
            (int32_t)(dst_height * quality));
        rend_data_ctx.state.set_ps_textures(0, 1, src_color_tex);
        rend_data_ctx.state.set_ps_sampler_state(0, 1, sampler_states);
        rend_data_ctx.state.set_ps_textures(1, 1, src_depth_tex);
        rend_data_ctx.state.set_ps_sampler_state(1, 1, sampler_states);
        rend_data_ctx.state.set_vs_shader(shaders[non_sss ? 50 : 32].vertex);
        rend_data_ctx.state.set_ps_shader(shaders[non_sss ? 50 : 32].pixel);
        rend_data_ctx.state.set_depth_stencil_state(&depth_stencil_states[2]);
        rend_data_ctx.state.set_ps_constant_buffer(2, 1, &buffers[5]);

        p_dx_buffer& buffer = field_B0[rend_data_ctx.index][0];
        render_data::quad_shader_data* quad = (render_data::quad_shader_data*)rend_data_ctx.state.map(buffer);
        const float_t v21 = src_width_scale * quality * 0.5f;
        const float_t v20 = src_height_scale * quality * -0.5f;
        quad->g_texcoord_modifier.x = v21;
        quad->g_texcoord_modifier.y = v20;
        quad->g_texcoord_modifier.z = v21;
        quad->g_texcoord_modifier.w = v20 + src_height_scale * quality;

        const int32_t width = src_width >= 0 ? src_width : 1;
        const int32_t height = src_height >= 0 ? src_height : 1;
        quad->g_texel_size.x = quality / (float_t)width;
        quad->g_texel_size.y = quality / (float_t)height;
        quad->g_texel_size.z = (float_t)width;
        quad->g_texel_size.w = (float_t)height;
        quad->g_color = 1.0f;
        quad->g_texture_lod = 0.0f;
        rend_data_ctx.state.unmap(buffer);

        rend_data_ctx.state.set_vs_constant_buffer(0, 1, &buffer);
        rend_data_ctx.state.set_ps_constant_buffer(0, 1, &buffer);

        p_dx_buffer null_buf;

        int32_t offsets[] = { 0 };
        int32_t strides[] = { 0 };
        p_dx_buffer* buffers[] = { &null_buf };
        rend_data_ctx.state.set_vertex_buffer(0, 1, buffers, strides, offsets);
        rend_data_ctx.state.set_index_buffer(&null_buf, 0, 0);
        rend_data_ctx.state.set_rasterizer_state(&rasterizer_state);
        rend_data_ctx.state.set_blend_state(&blend_states[1]);
        rend_data_ctx.state.set_primitive_topology(DX_PRIMITIVE_TRIANGLE_STRIP);
        rend_data_ctx.state.draw(4, 0);
    }

    void Render::Data::draw_quad(render_data_context& rend_data_ctx, float_t lod, int32_t width, int32_t height,
        float_t s0, float_t t0, float_t s1, float_t t1, float_t scale, const vec4& params) {
        t1 -= t0;
        s0 -= s1;

        const float_t w = (float_t)max_def(width, 1);
        const float_t h = (float_t)max_def(height, 1);

        p_dx_buffer& buffer = field_B0[rend_data_ctx.index][0];
        render_data::quad_shader_data* quad = (render_data::quad_shader_data*)rend_data_ctx.state.map(buffer);
        quad->g_texcoord_modifier = { s0 * 0.5f, t1 * 0.5f, s0 * 0.5f + s1, t1 * 0.5f + t0 };
        quad->g_texel_size = { scale / w, scale / h, w, h };
        quad->g_color = params;
        quad->g_texture_lod = { lod, 0.0f, 0.0f, 0.0f };
        rend_data_ctx.state.unmap(buffer);

        rend_data_ctx.state.set_vs_constant_buffer(0, 1, &buffer);
        rend_data_ctx.state.set_ps_constant_buffer(0, 1, &buffer);

        p_dx_buffer null_buf;

        int32_t offsets[] = { 0 };
        int32_t strides[] = { 0 };
        p_dx_buffer* buffers[] = { &null_buf };
        rend_data_ctx.state.set_vertex_buffer(0, 1, buffers, strides, offsets);
        rend_data_ctx.state.set_index_buffer(&null_buf, 0, 0);
        rend_data_ctx.state.set_rasterizer_state(&rasterizer_state);
        rend_data_ctx.state.set_depth_stencil_state(depth_stencil_states);
        rend_data_ctx.state.set_blend_state(&blend_states[1]);
        rend_data_ctx.state.set_primitive_topology(DX_PRIMITIVE_TRIANGLE_STRIP);
        rend_data_ctx.state.draw(4);
    }

    void Render::Data::draw_quad_copy(render_data_context& rend_data_ctx, int32_t x, int32_t y, int32_t width, int32_t height,
        p_dx_texture& color_texture, float_t s0, float_t t0, float_t lod, bool linear) {
        rend_data_ctx.state.set_viewport(x, y, width, height);
        rend_data_ctx.state.set_ps_textures(0, 1, &color_texture);
        rend_data_ctx.state.set_ps_sampler_state(0, 1, &sampler_states[linear ? 2 : 5]);
        rend_data_ctx.state.set_vs_shader(&shaders[0].vertex);
        rend_data_ctx.state.set_ps_shader(&shaders[0].pixel);
        draw_quad(rend_data_ctx, lod, color_texture.get_width(), color_texture.get_height(), s0, t0, 0.0f, 0.0f, 1.0f, 1.0f);
    }

    void Render::apply_dof(render_data_context& rend_data_ctx, const cam_data& cam, RenderTexture& rt, const float_t image_quality) {
        const int32_t width = app::get_value_scaled(render_width[0], image_quality);
        const int32_t height = app::get_value_scaled(render_height[0], image_quality);
        const bool upsample = render_width[0] != width || render_height[0] != height;
        if (dof_debug_data.flags & DOF_DEBUG_USE_UI_PARAMS) {
            if (dof_debug_data.flags & DOF_DEBUG_ENABLE_DOF) {
                if (dof_debug_data.flags & DOF_DEBUG_ENABLE_PHYS_DOF) {
                    float_t focus = dof_debug_data.focus;
                    if (dof_debug_data.flags & DOF_DEBUG_AUTO_FOCUS) {
                        for (int32_t i = 0; i < ROB_CHARA_COUNT; i++) {
                            rob_chara* rob_chr = rob_chara_array_get(0, i);
                            if (!rob_chr || !rob_chr->is_visible())
                                continue;

                            void(FASTCALL * sub_1405163C0)(rob_chara * rob_chr, int32_t index, mat4 * mat)
                                = (void(FASTCALL*)(rob_chara * rob_chr, int32_t index, mat4 * mat))0x00000001404E2370;

                            mat4 mat;
                            sub_1405163C0(rob_chr, 4, &mat);
                            mat4_transpose(&mat, &mat);

                            vec3 chara_trans = 0.0f;
                            mat4_get_translation(&mat, &chara_trans);

                            mat4 view;
                            view.row0 = cam.view_mat[0];
                            view.row1 = cam.view_mat[1];
                            view.row2 = cam.view_mat[2];
                            view.row3 = { 0.0f, 0.0f, 0.0f, 1.0f };

                            mat4_transpose(&view, &view);
                            focus = -vec3::dot(*(vec3*)&view.row2, chara_trans) - view.row2.w - 0.1f;
                            break;
                        }
                    }

                    dof->apply(rend_data_ctx, dof_texture, rt.GetColorTex(), rt.GetDepthTex(),
                        cam.min_distance, cam.max_distance, cam.fov,
                        max_def(focus, cam.min_distance),
                        dof_debug_data.focal_length, dof_debug_data.f_number, width, height);
                }
                else
                    dof->apply_f2(rend_data_ctx, dof_texture, rt.GetColorTex(), rt.GetDepthTex(),
                        cam.min_distance, cam.max_distance, cam.fov,
                        dof_debug_data.f2.focus, dof_debug_data.f2.focus_range,
                        max_def(dof_debug_data.f2.fuzzing_range, 0.01f), dof_debug_data.f2.ratio, width, height);

                rt.Bind(rend_data_ctx.state);
                rend_data_ctx.state.set_blend_state(dx_default_states_get_blend_state(false,
                    DX_BLEND_SRC_ALPHA, DX_BLEND_INV_SRC_ALPHA,
                    DX_BLEND_SRC_ALPHA, DX_BLEND_INV_SRC_ALPHA, DX_BLEND_WRITE_MASK_RGBA));
                render_ptr->draw_quad_copy(rend_data_ctx, 0, 0,
                    (int32_t)((float_t)render_width[0] * image_quality),
                    (int32_t)((float_t)render_height[0] * image_quality),
                    dof_texture.GetColorTex(),
                    (float_t)width / (float_t)render_width[0],
                    (float_t)height / (float_t)render_height[0], 0.0f, true);
            }
        }
        else if (dof_pv_data.enable && dof_pv_data.f2.ratio > 0.0f) {
            dof->apply_f2(rend_data_ctx, dof_texture, rt.GetColorTex(), rt.GetDepthTex(),
                cam.min_distance, cam.max_distance, cam.fov,
                dof_pv_data.f2.focus, dof_pv_data.f2.focus_range,
                max_def(dof_pv_data.f2.fuzzing_range, 0.01f), dof_pv_data.f2.ratio, width, height);

            rt.Bind(rend_data_ctx.state);
            rend_data_ctx.state.set_blend_state(dx_default_states_get_blend_state(false,
                DX_BLEND_SRC_ALPHA, DX_BLEND_INV_SRC_ALPHA,
                DX_BLEND_SRC_ALPHA, DX_BLEND_INV_SRC_ALPHA, DX_BLEND_WRITE_MASK_RGBA));
            render_ptr->draw_quad_copy(rend_data_ctx, 0, 0,
                (int32_t)((float_t)render_width[0] * image_quality),
                (int32_t)((float_t)render_height[0] * image_quality),
                dof_texture.GetColorTex(),
                (float_t)width / (float_t)render_width[0],
                (float_t)height / (float_t)render_height[0], 0.0f, true);

            enum_or(dof_debug_data.flags, DOF_DEBUG_ENABLE_DOF);
            dof_debug_data.f2 = dof_pv_data.f2;
        }
        else
            enum_and(dof_debug_data.flags, ~DOF_DEBUG_ENABLE_DOF);
    }

    void Render::bind_render_texture(p_dx_state& state, bool aet_back) {
        if (aet_back) {
            aet_back_texture.Bind(state);
            state.set_viewport(0, 0, render_width[0], render_height[0]);
        }
        else {
            rend_texture->Bind(state);
            state.set_viewport(0, 0,
                app::get_value_scaled(render_width[0], render_ptr->image_quality),
                app::get_value_scaled(render_height[0], render_ptr->image_quality));
        }
    }

    void Render::draw_npr_frame(render_data_context& rend_data_ctx) {
        void (*rndr__Render__draw_npr_frame)(rndr::Render * This, render_data_context & rend_data_ctx)
            = (void (*)(rndr::Render * This, render_data_context & rend_data_ctx))0x00000001404A52B0;
        rndr__Render__draw_npr_frame(this, rend_data_ctx);
    }

    void Render::get_render_resolution(float_t* render_width, float_t* render_height,
        float_t* render_post_width, float_t* render_post_height) {
        if (render_width)
            *render_width = (float_t)this->render_width[0];
        if (render_height)
            *render_height = (float_t)this->render_height[0];
        if (render_post_width)
            *render_post_width = (float_t)this->render_post_width[0];
        if (render_post_height)
            *render_post_height = (float_t)this->render_post_height[0];
    }

    int32_t RenderManager::get_npr_param() {
        int32_t(*rndr__RenderManager__get_npr_param)(RenderManager * This)
            = (int32_t(*)(RenderManager * This))0x00000001404D8C50;
        return rndr__RenderManager__get_npr_param(this);
    }

    RenderTexture& RenderManager::get_render_texture(int32_t index) {
        return render_textures[render_manager_render_texture_index_array[index].arr[tex_index[index]]];
    }
}
