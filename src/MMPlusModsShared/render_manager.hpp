/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "renderer/dof.hpp"
#include "renderer/transparency.hpp"
#include "camera.hpp"
#include "render_context.hpp"
#include "render_texture.hpp"
#include "shadow.hpp"
#include "stage.hpp"
#include "types.hpp"

enum draw_pass_3d_type {
    DRAW_PASS_3D_OPAQUE = 0,
    DRAW_PASS_3D_TRANSLUCENT,
    DRAW_PASS_3D_TRANSPARENT,
    DRAW_PASS_3D_MAX,
};

struct draw_pre_process {
    int32_t type;
    void(FASTCALL* func)(void*);
    void* data;
};

namespace rndr {
    enum RenderPassID {
        RND_PASSID_SHADOW = 0,
        RND_PASSID_1,
        RND_PASSID_SS_SSS,
        RND_PASSID_3,
        RND_PASSID_REFLECT,
        RND_PASSID_REFRACT,
        RND_PASSID_PRE_PROCESS,
        RND_PASSID_CLEAR,
        RND_PASSID_PRE_SPRITE,
        RND_PASSID_3D_0,
        RND_PASSID_3D_1,
        RND_PASSID_3D_2,
        RND_PASSID_SHOW_VECTOR,
        RND_PASSID_POST_PROCESS,
        RND_PASSID_SPRITE,
        RND_PASSID_ADJUST_SCREEN,
        RND_PASSID_16,
        RND_PASSID_NUM,
    };

    struct Render {
        enum FrameTextureType {
            RND_RENDER_FRAME_TEXTURE_PRE_PP  = 0x00,
            RND_RENDER_FRAME_TEXTURE_POST_PP = 0x01,
            RND_RENDER_FRAME_TEXTURE_FB      = 0x02,
            RND_RENDER_FRAME_TEXTURE_MAX     = 0x03,
        };

        struct ExposureCharaData {
            vec4 field_0[8];
            float_t field_80;
            p_dx_query_occlusion query[3];
            int64_t query_data[3];
        };

        static_assert(sizeof(rndr::Render::ExposureCharaData) == 0xB8, "\"rndr::Render::ExposureCharaData\" struct should have a size of 0xB8");

        struct Data {
            p_dx_sampler_state sampler_states[6];
            p_dx_rasterizer_state rasterizer_state;
            p_dx_depth_stencil_state depth_stencil_states[3];
            p_dx_blend_state blend_states[4];
            p_dx_buffer buffers[8];
            p_dx_buffer field_B0[4][4];
            dx_vertex_pixel_shader shaders[54];
            p_dx_texture exposure_history;
            p_dx_texture exposure;
            p_dx_texture field_4A0;
            p_dx_texture field_4A8;
            p_dx_texture tonemap_lut_texture[2];
            int32_t field_4C0;
            p_dx_render_target render_targets[3];
            float_t image_quality;
            int32_t field_4E4;
            int32_t flags;
            p_dx_query_occlusion lens_shaft_query[3];
            p_dx_query_occlusion lens_flare_query[3];
            int64_t lens_shaft_query_data[3];
            int64_t lens_flare_query_data[3];
            int32_t lens_flare_query_index;
            vec3 sun_position;
            vec3 field_560;
            vec4 field_56C;
            float_t field_57C;
            rndr::Render::ExposureCharaData exposure_chara_data[6];
            int32_t exposure_history_counter;
            int32_t exposure_query_index;
            bool field_9D8;
            p_dx_texture lens_texture[3];
            float_t field_9F8[2];

            void draw_npr_frame(render_data_context& rend_data_ctx, float_t width_scale, float_t height_scale,
                p_dx_texture* contour_color_texture, p_dx_texture* contour_depth_texture,
                p_dx_texture* scene_depth_texture, float_t quality);
            void draw_npr_frame_quad(render_data_context& rend_data_ctx,
                float_t width_scale, float_t height_scale, int32_t width, int32_t height, float_t quality);
            void draw_sss_contour(render_data_context& rend_data_ctx,
                p_dx_texture* dst_color_tex, p_dx_texture* dst_depth_tex, int32_t dst_width, int32_t dst_height,
                p_dx_texture* src_color_tex, p_dx_texture* src_depth_tex, int32_t src_width, int32_t src_height,
                float_t src_width_scale, float_t src_height_scale, bool non_sss, float_t quality);
            void draw_quad(render_data_context& rend_data_ctx, float_t lod, int32_t width, int32_t height,
                float_t s0, float_t t0, float_t s1, float_t t1, float_t scale, const vec4& params);
            void draw_quad_copy(render_data_context& rend_data_ctx, int32_t x, int32_t y, int32_t width, int32_t height,
                p_dx_texture& color_texture, float_t s0, float_t t0, float_t lod, bool linear);
        };

        static_assert(sizeof(rndr::Render::Data) == 0xA00, "\"rndr::Render::Data\" struct should have a size of 0xA00");

        struct FrameTextureData {
            texture* texture;
            RenderTexture render_texture;
            rndr::Render::FrameTextureType type;
        };

        static_assert(sizeof(rndr::Render::FrameTextureData) == 0x50, "\"rndr::Render::FrameTextureData\" struct should have a size of 0x50");

        struct FrameTexture {
            rndr::Render::FrameTextureData data[4];
            bool capture;
        };

        static_assert(sizeof(rndr::Render::FrameTexture) == 0x148, "\"rndr::Render::FrameTexture\" struct should have a size of 0x148");

        int32_t downsample_count;
        int32_t downsample_max_count;
        int32_t ssaa;
        int32_t taa;
        int32_t mlaa;
        int32_t fxaa;
        int32_t cam_blur;
        int32_t hd_res;
        int32_t base_downsample;
        int32_t ss_alpha_mask;
        RenderTexture rend_texture[5];
        RenderTexture rend_texture_mm[5];
        RenderTexture taa_buffer[4];
        texture* taa_tex[4];
        RenderTexture aet_back_texture;
        texture* aet_back_tex;
        RenderTexture field_410;
        RenderTexture reduce_texture[5];
        texture* reduce_tex[5];
        RenderTexture reduce_texture_dup0[5];
        RenderTexture reduce_texture_dup1[5];
        texture* reduce_tex_dup0[5];
        texture* reduce_tex_dup1[5];
        RenderTexture downsample_texture;
        texture* downsample_tex;
        RenderTexture reduce_texture_mm[5];
        RenderTexture reduce_texture_mm_dup0[5];
        RenderTexture reduce_texture_mm_dup1[5];
        texture* reduce_tex_mm[5];
        texture* reduce_tex_mm_dup0[5];
        texture* reduce_tex_mm_dup1[5];
        RenderTexture downsample_texture_mm;
        texture* downsample_tex_mm;
        texture* history_tex[2];
        RenderTexture dof_texture;
        p_dx_texture render_buffer;
        RenderTexture contour_texture;
        RenderTexture rend_nonsss_texture;
        texture* rend_nonsss_tex;
        RenderTexture field_E30;
        RenderTexture field_E70;
        RenderTexture field_EB0;
        RenderTexture mlaa_buffer;
        RenderTexture temp_buffer;
        RenderTexture* sss_contour_texture;
        RenderTexture field_F78;
        int32_t texture_counter;
        int32_t lens_ghost_count;
        int32_t width;
        int32_t height;
        int32_t inner_width;
        int32_t inner_height;
        int32_t render_width[5];
        int32_t render_height[5];
        int32_t render_post_width[5];
        int32_t render_post_height[5];
        int32_t render_post_width_min[5];
        int32_t render_post_height_min[5];
        float_t render_post_width_scale;
        float_t render_post_height_scale;
        int32_t reduce_width[5];
        int32_t reduce_height[5];
        int32_t reduce_width_mm[5];
        int32_t reduce_height_mm[5];
        int32_t taa_texture_selector;
        int32_t taa_texture;
        float_t taa_blend;
        int32_t field_10AC;
        int32_t field_10B0;
        bool field_10B4;
        bool field_10B5;
        bool field_10B6;
        float_t field_10B8;
        vec3 view_point;
        vec3 interest;
        vec3 view_point_prev;
        vec3 interest_prev;
        mat4 cam_view_projection;
        mat4 cam_view_projection_prev;
        int32_t stage_index;
        int32_t stage_index_prev;
        bool reset_exposure;
        int32_t screen_x_offset;
        int32_t screen_y_offset;
        int32_t screen_width;
        int32_t screen_height;
        int32_t update_lut;
        int32_t saturate_lock;
        vec3 lens_flare_pos;
        float_t lens_shaft_scale;
        float_t lens_shaft_inv_scale;
        float_t lens_flare_power;
        float_t field_11A8;
        float_t lens_flare_appear_power;
        texture* render_textures_data[16];
        RenderTexture render_textures[16];
        texture* movie_textures_data[1];
        RenderTexture movie_textures[1];
        p_dx_texture* field_1678;
        p_dx_texture* field_1680;
        RenderTexture field_1688;
        bool aet_back;
        renderer::DOF3* dof;
        texture* transparency_tex[1];
        renderer::Transparency* transparency;
        int32_t saturate_select;
        int32_t scene_fade_select;
        int32_t tone_trans_select;
        float_t saturate_coeff[2];
        float_t scene_fade_color[6];
        float_t scene_fade_alpha[2];
        int32_t scene_fade_blend_func[2];
        float_t tone_trans_scale[6];
        float_t tone_trans_offset[6];
        float_t tone_trans_start[6];
        float_t tone_trans_end[6];
        int32_t field_1784;
        int64_t field_1788;
        int64_t field_1790;
        int64_t field_1798;
        int64_t field_17A0;
        int64_t field_17A8;
        int64_t field_17B0;
        int64_t field_17B8;
        int64_t field_17C0;
        int64_t field_17C8;
        int64_t field_17D0;
        int64_t field_17D8;
        int64_t field_17E0;
        int64_t field_17E8;
        int64_t field_17F0;
        int64_t field_17F8;
        int64_t field_1800;
        int64_t field_1808;
        int64_t field_1810;
        int64_t field_1818;
        int64_t field_1820;
        int64_t field_1828;
        int64_t field_1830;
        int64_t field_1838;
        int64_t field_1840;
        int64_t field_1848;
        int64_t field_1850;
        int64_t field_1858;
        int64_t field_1860;
        int64_t field_1868;
        int64_t field_1870;
        int64_t field_1878;
        int64_t field_1880;
        int64_t field_1888;
        int64_t field_1890;
        int64_t field_1898;
        int64_t field_18A0;
        int64_t field_18A8;
        int64_t field_18B0;
        int64_t field_18B8;
        int32_t field_18C0;
        int32_t tone_map;
        float_t exposure;
        float_t exposure_rate;
        int32_t auto_exposure;
        float_t gamma;
        float_t gamma_rate;
        int32_t saturate_power;
        int32_t mag_filter;
        float_t fade_alpha;
        int32_t fade;
        float_t lens_flare;
        float_t lens_shaft;
        float_t lens_ghost;
        float_t field_18F8;
        vec3 radius;
        vec3 intensity;
        float_t field_1914;
        float_t field_1918;
        float_t field_191C;
        float_t field_1920;
        float_t field_1924;
        float_t field_1928;
        float_t field_192C;
        int32_t field_1930;
        float_t field_1934;
        int32_t field_1938;
        int32_t update;
        rndr::Render::Data* render_ptr;
        rndr::Render::FrameTexture frame_texture[6];

        void apply_dof(render_data_context& rend_data_ctx, const cam_data& cam, RenderTexture& rt, const float_t image_quality);
        void bind_render_texture(p_dx_state& state, bool aet_back = false);
        void draw_npr_frame(render_data_context& rend_data_ctx);
        void get_render_resolution(float_t* render_width, float_t* render_height,
            float_t* render_post_width, float_t* render_post_height);
    };

    static_assert(sizeof(rndr::Render) == 0x20F8, "\"rndr::Render\" struct should have a size of 0x20F8");

    struct RenderManager {
        bool pass_sw[17];
        Shadow* shadow_ptr;
        bool reflect;
        bool refract;
        int32_t reflect_blur_num;
        blur_filter_mode reflect_blur_filter;
        rndr::Render* render;
        bool sync_gpu;
        int64_t cpu_time[17];
        int64_t gpu_time[17];
        int64_t field_150[4][3];
        int64_t field_1B0;
        int64_t field_1B8[8][52];
        int32_t field_EB8;
        int32_t field_EBC;
        int64_t field_EC0[17];
        void* field_F48;
        bool shadow;
        bool opaque_z_sort;
        bool alpha_z_sort;
        bool glitter;
        bool draw_pass_3d[4];
        int8_t field_11F;
        int8_t field_120;
        int8_t field_121;
        stage_data_reflect_type reflect_type;
        bool clear;
        int32_t tex_index[12];
        RenderTexture render_textures[9];
        int32_t width;
        int32_t height;
        bool multisample;
        int8_t field_2E9;
        int32_t show_vector_flags;
        float_t show_vector_length;
        float_t show_vector_z_offset;
        int8_t stage_shadow;
        prj::list<draw_pre_process> pre_process;
        texture* effect_texture;
        int32_t field_1210;
        int32_t npr_param;
        bool field_31C;
        bool reflect_texture_mask;
        bool reflect_tone_curve;
        bool field_31F;
        bool stage_ambient;
        bool npr;
        cam_data cam[2];
        int32_t field_1588;
        float_t resolution_scale;
        int32_t field_1590;

        int32_t get_npr_param();
        RenderTexture& get_render_texture(int32_t index);
    };

    static_assert(sizeof(rndr::RenderManager) == 0x1598, "\"rndr::RenderManager\" struct should have a size of 0x1598");
}

extern rndr::RenderManager& render_manager;

extern float_t(*reflection_quality_get)();
extern bool (*sub_1402C1D20)();
extern void (*fog_set)(render_data_context& rend_data_ctx, int32_t id);
extern void (*image_filter_scale)(render_data_context& rend_data_ctx, texture* dst, int64_t a3,
    texture* src, int64_t a5, const vec4& scale, float_t quality);
extern void (*lighting_set)(render_data_context& rend_data_ctx);
extern void (*shader_get_id_by_name)(render_data_context& rend_data_ctx, const char* str);
extern rndr::Render* (*render_get)();
extern bool (*sub_1404D9FD0)();
extern void (*draw_pass_3d_shadow_set)(Shadow* shad, render_data_context& rend_data_ctx);
extern void (*draw_pass_3d_shadow_reset)(Shadow* shad, render_data_context& rend_data_ctx);
