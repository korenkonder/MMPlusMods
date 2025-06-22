/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/mat.hpp"
#include "../KKdLib/vec.hpp"
#include "camera.hpp"
#include "render_texture.hpp"
#include "wrap.hpp"

enum DxShaderName {
    DX_SHADER_LAMBERT                = 0x00,
    DX_SHADER_STAGE_BLINN            = 0x01,
    DX_SHADER_CONSTANT               = 0x02,
    DX_SHADER_SKY_DEFAULT            = 0x03,
    DX_SHADER_BLINN_PER_VERT         = 0x04,
    DX_SHADER_BLINN_PER_FRAG         = 0x05,
    DX_SHADER_HAIR_DEFAULT           = 0x06,
    DX_SHADER_HAIR_NPR1              = 0x07,
    DX_SHADER_CLOTH_DEFAULT          = 0x08,
    DX_SHADER_CLOTH_ANISO            = 0x09,
    DX_SHADER_CLOTH_NPR1             = 0x0A,
    DX_SHADER_SKIN_DEFAULT           = 0x0B,
    DX_SHADER_ITEM_BLINN             = 0x0C,
    DX_SHADER_GLASS_EYE              = 0x0D,
    DX_SHADER_TIGHTS                 = 0x0E,
    DX_SHADER_FLOOR                  = 0x0F,
    DX_SHADER_SILHOUETTE             = 0x10,
    DX_SHADER_LITPROJ                = 0x11,
    DX_SHADER_SIMPLE_REFLECT         = 0x12,
    DX_SHADER_SSS_SKIN               = 0x13,
    DX_SHADER_PUDDLE                 = 0x14,
    DX_SHADER_WATER01                = 0x15,
    DX_SHADER_SIMPLE                 = 0x16,
    DX_SHADER_PREPASS                = 0x17,
    DX_SHADER_SKIN_NPR1              = 0x18,
    DX_SHADER_SKIN_NPR1_NONSSS       = 0x19,
    DX_SHADER_GLASS_EYE_NONSSS       = 0x1A,
    DX_SHADER_CLOTH_NPR1_NONSSS      = 0x1B,
    DX_SHADER_HAIR_NPR1_NONSSS       = 0x1C,
    DX_SHADER_ITEM_BLINN_NONSSS      = 0x1D,
    DX_SHADER_TIGHTS_NPR1_NONSSS     = 0x1E,
    DX_SHADER_LAMBERTP               = 0x1F,
    DX_SHADER_LAMBERT_NONSSS         = 0x20,
    DX_SHADER_LAMBERT_NONSSSP        = 0x21,
    DX_SHADER_BLINN_PER_VERTP        = 0x22,
    DX_SHADER_BLINN_PER_VERT_NONSSS  = 0x23,
    DX_SHADER_BLINN_PER_VERT_NONSSSP = 0x24,
    DX_SHADER_BLINN_PER_FRAGP        = 0x25,
    DX_SHADER_BLINN_PER_FRAG_NONSSS  = 0x26,
    DX_SHADER_BLINN_PER_FRAG_NONSSSP = 0x27,
    DX_SHADER_STAGE_BLINNP           = 0x28,
    DX_SHADER_STAGE_BLINN_NONSSS     = 0x29,
    DX_SHADER_STAGE_BLINN_NONSSSP    = 0x2A,
    DX_SHADER_CONSTANTP              = 0x2B,
    DX_SHADER_CONSTANT_NONSSS        = 0x2C,
    DX_SHADER_CONSTANT_NONSSSP       = 0x2D,
    DX_SHADER_FLOOR_NONSSS           = 0x2E,
    DX_SHADER_MAX                    = 0x2F,
};


enum render_data_flags {
    RENDER_DATA_SHADER_UPDATE = 0x01,
    RENDER_DATA_SCENE_UPDATE  = 0x02,
    RENDER_DATA_BATCH_UPDATE  = 0x04,
};

enum uniform_name {
    U_NONE          = 0x00,
    U_ALPHA_MASK    = 0x01,
    U_ALPHA_TEST    = 0x02,
    U_ANISO         = 0x03,
    U_AET_BACK      = 0x04,
    U_TEXTURE_BLEND = 0x05,
    U_UNK_NEW_IN_MM_IDK_WHAT_TO_DO_WITH_IT = 0x06,
    U_CHARA_COLOR   = 0x07,
    U_CLIP_PLANE    = 0x08,
    U08             = 0x09,
    U_DEPTH_PEEL    = 0x0A,
    U_DEPTH         = 0x0B,
    U0B             = 0x0C,
    U_ALPHA_BLEND   = 0x0D,
    U_RIPPLE_EMIT   = 0x0E,
    U_ESM_FILTER    = 0x0F,
    U_EXPOSURE      = 0x10,
    U_SCENE_FADE    = 0x11,
    U_FADE          = 0x12,
    U_STAGE_AMBIENT = 0x13,
    U_FLARE         = 0x14,
    U_FOG_STAGE     = 0x15,
    U_FOG_CHARA     = 0x16,
    U16             = 0x17,
    U_GAUSS         = 0x18,
    U_EYE_LENS      = 0x19,
    U_IMAGE_FILTER  = 0x1A,
    U_INSTANCE      = 0x1B,
    U_TONE_CURVE    = 0x1C,
    U_LIGHT_PROJ    = 0x1D,
    U_MAGNIFY       = 0x1E,
    U_MEMBRANE      = 0x1F,
    U_MLAA          = 0x20,
    U_MLAA_SEARCH   = 0x21,
    U_MORPH_COLOR   = 0x22,
    U_MORPH         = 0x23,
    U_MOVIE         = 0x24,
    U24             = 0x25,
    U25             = 0x26,
    U_NPR_NORMAL    = 0x27,
    U_NPR           = 0x28,
    U_STAGE_SHADOW2 = 0x29,
    U_REFLECT       = 0x2A,
    U_REDUCE        = 0x2B,
    U_CHARA_SHADOW  = 0x2C,
    U_CHARA_SHADOW2 = 0x2D,
    U2D             = 0x2E,
    U2E             = 0x2F,
    U_SHOW_VECTOR   = 0x30,
    U_SKINNING      = 0x31,
    U_SNOW_PARTICLE = 0x32,
    U_SPECULAR_IBL  = 0x33,
    U_COMBINER      = 0x34,
    U_TEX_0_TYPE    = 0x35,
    U_TEX_1_TYPE    = 0x36,
    U_SSS_FILTER    = 0x37,
    U_SSS_CHARA     = 0x38,
    U_STAR          = 0x39,
    U_TEXTURE_COUNT = 0x3A,
    U_ENV_MAP       = 0x3B,
    U_RIPPLE        = 0x3C,
    U_TRANSLUCENCY  = 0x3D,
    U_NORMAL        = 0x3E,
    U_TRANSPARENCY  = 0x3F,
    U_WATER_REFLECT = 0x40,
    U40             = 0x41,
    U41             = 0x42,
    U_STAGE_SHADOW  = 0x43,
    U_SPECULAR      = 0x44,
    U_TONE_MAP      = 0x45,
    U45             = 0x46,
    U_MAX           = 0x47,
    U_INVALID       = 0xFFFFFFFF,
};

struct uniform_value {
    int32_t arr[U_MAX];
};

static_assert(sizeof(uniform_value) == 0x11C, "\"uniform_value\" struct should have a size of 0x11C");

struct matrix_stack {
    mat4 mats[64];
    int32_t depth;
};

static_assert(sizeof(matrix_stack) == 0x1004, "\"matrix_stack\" struct should have a size of 0x1004");

struct draw_state_stats {
    int32_t draw_triangle_count;
    int32_t draw_triangle_strip_count;
    int32_t object_draw_count;
    int32_t object_reflect_draw_count;
    int32_t object_translucent_draw_count;
};

static_assert(sizeof(draw_state_stats) == 0x14, "\"draw_state_stats\" struct should have a size of 0x14");

struct render_data_context;

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

    void calc_coef(render_data_context& rend_data_ctx, __int64 a3, __int64 a4, double_t step,
        __int64 a6, const double_t* weights, const double_t* r_radius, const double_t* g_radius, const double_t* b_radius);
    void draw_quad(render_data_context& rend_data_ctx, int32_t width, int32_t height,
        float_t s0, float_t t0, float_t s1, float_t t1, const vec4& param);
};

static_assert(sizeof(dx_sss_struct) == 0x78, "\"dx_sss_struct\" struct should have a size of 0x78");

struct sss_data {
    bool init_data;
    bool enable;
    bool npr_contour;
    RenderTexture textures[4];
    vec4 param;
    dx_sss_struct* dx;

    vec4& get_sss_param();
    void set_texture(render_data_context& rend_data_ctx, int32_t index);
};

static_assert(sizeof(sss_data) == 0x120, "\"sss_data\" struct should have a size of 0x120");

struct global_material_struct {
    float_t bump_depth;
    float_t intensity;
    float_t reflectivity;
    float_t reflect_uv_scale;
    float_t refract_uv_scale;
};

static_assert(sizeof(global_material_struct) == 0x14, "\"global_material_struct\" struct should have a size of 0x14");

struct draw_state_struct {
    struct render_data {
        int32_t shader;
        bool self_shadow;
        bool shadow;
        bool blend;
        bool cull_front;
        int32_t field_8;
        draw_state_stats stats;
    };

    draw_state_stats stats;
    bool vertex_buffer;
    bool wireframe;
    bool wireframe_overlay;
    bool shader_debug_flag;
    bool use_global_material;
    bool fog_height;
    bool back_display;
    int32_t shader_index;
    global_material_struct global_material;
    int32_t field_34;
    float_t field_38;
    draw_state_struct::render_data rend_data[4];
    bool field_BC;

    void set_blend(render_data_context& rend_data_ctx, bool value);
    void set_cull_front(render_data_context& rend_data_ctx, bool value);
};

static_assert(sizeof(draw_state_struct) == 0xC0, "\"draw_state_struct\" struct should have a size of 0xC0");

struct render_data {
    struct quad_shader_data {
        vec4 g_texcoord_modifier;
        vec4 g_texel_size;
        vec4 g_color;
        vec4 g_texture_lod;
    };

    static_assert(sizeof(render_data::quad_shader_data) == 0x40, "\"render_data::quad_shader_data\" struct should have a size of 0x40");

    struct obj_pre_pass_param_data {
        vec4i g_instance_id;
    };

    static_assert(sizeof(render_data::obj_pre_pass_param_data) == 0x10, "\"render_data::obj_pre_pass_param_data\" struct should have a size of 0x10");

    struct obj_scene_data {
        vec4 g_irradiance_r_transforms[4];
        vec4 g_irradiance_g_transforms[4];
        vec4 g_irradiance_b_transforms[4];
        vec4 g_light_env_stage_diffuse;
        vec4 g_light_env_stage_specular;
        vec4 g_light_env_chara_diffuse;
        vec4 g_light_env_chara_ambient;
        vec4 g_light_env_chara_specular;
        vec4 g_light_env_reflect_diffuse;
        vec4 g_light_env_reflect_ambient;
        vec4 g_light_env_reflect_specular;
        vec4 g_light_env_proj_diffuse;
        vec4 g_light_env_proj_specular;
        vec4 g_light_env_proj_position;
        vec4 g_light_stage_dir;
        vec4 g_light_stage_diff;
        vec4 g_light_stage_spec;
        vec4 g_light_chara_dir;
        vec4 g_light_chara_spec;
        vec4 g_light_chara_luce;
        vec4 g_light_chara_back;
        vec4 g_light_face_diff;
        vec4 g_chara_color0;
        vec4 g_chara_color1;
        vec4 g_chara_f_dir;
        vec4 g_chara_f_ambient;
        vec4 g_chara_f_diffuse;
        vec4 g_chara_tc_param;
        vec4 g_fog_depth_color;
        vec4 g_fog_height_params;
        vec4 g_fog_height_color;
        vec4 g_fog_bump_params;
        vec4 g_fog_state_params;
        vec4 g_normal_tangent_transforms[3];
        vec4 g_esm_param;
        vec4 g_self_shadow_receivers[6];
        vec4 g_shadow_ambient;
        vec4 g_shadow_ambient1;
        vec4 g_framebuffer_size;
        vec4 g_light_reflect_dir;
        vec4 g_clip_plane;
        vec4 g_npr_cloth_spec_color;
        vec4 g_view[3];
        vec4 g_view_inverse[3];
        vec4 g_projection_view[4];
        vec4 g_view_position;
        vec4 g_light_projection[4];
        vec4 g_light_projection_depth[4];
        vec4 g_forward_z_projection_row2;
    };

    static_assert(sizeof(render_data::obj_scene_data) == 0x4E0, "\"render_data::obj_scene_data\" struct should have a size of 0x4E0");

    struct obj_batch_data {
        vec4 g_transforms[4];
        vec4 g_worlds[3];
        vec4 g_worlds_invtrans[3];
        vec4 g_worldview[3];
        vec4 g_worldview_inverse[3];
        vec4 g_joint[3];
        vec4 g_joint_inverse[3];
        vec4 g_texcoord_transforms[4];
        vec4 g_blend_color;
        vec4 g_offset_color;
        vec4 g_material_state_diffuse;
        vec4 g_material_state_ambient;
        vec4 g_material_state_emission;
        vec4 g_material_state_shininess;
        vec4 g_material_state_specular;
        vec4 g_fresnel_coefficients;
        vec4 g_texture_color_coefficients;
        vec4 g_texture_color_offset;
        vec4 g_texture_specular_coefficients;
        vec4 g_texture_specular_offset;
        vec4 g_shininess;
        vec4 g_max_alpha;
        vec4 g_morph_weight;
        vec4 g_sss_param;
        vec4 g_bump_depth;
        vec4 g_intensity;
        vec4 g_reflect_uv_scale;
        vec4 g_shadow_position[2];
        vec4 g_shadow_direction[2];
        vec4 g_shadow_param;
        vec4i g_skip_flags;
    };

    static_assert(sizeof(render_data::obj_batch_data) == 0x330, "\"render_data::obj_batch_data\" struct should have a size of 0x330");

    struct obj_shader_data {
        struct {
            union {
                struct {
                    uint32_t alpha_mask    : 1; // bit 0
                    uint32_t alpha_test    : 1; // bit 1
                    uint32_t aniso         : 2; // bit 2:3
                    uint32_t aet_back      : 1; // bit 4
                    uint32_t texture_blend : 3; // bit 5:7
                    uint32_t unk           : 2; // bit 8:9
                    uint32_t chara_color   : 1; // bit 10
                    uint32_t clip_plane    : 1; // bit 11
                    uint32_t u08           : 1; // bit 12
                    uint32_t depth_peel    : 1; // bit 13
                    uint32_t depth         : 1; // bit 14
                    uint32_t u0b           : 1; // bit 15
                    uint32_t alpha_blend   : 3; // bit 16:18
                    uint32_t ripple_emit   : 1; // bit 19
                    uint32_t esm_filter    : 2; // bit 20:21
                    uint32_t exposure      : 2; // bit 22:23
                    uint32_t scene_fade    : 1; // bit 24
                    uint32_t fade          : 3; // bit 25:27
                    uint32_t stage_ambient : 1; // bit 28
                    uint32_t flare         : 2; // bit 29:30
                } m;
                uint32_t w;
            } x;
            union {
                struct {
                    uint32_t fog_stage     : 2; // bit 32+0:1
                    uint32_t fog_chara     : 2; // bit 32+2:3
                    uint32_t u16           : 1; // bit 32+4
                    uint32_t gauss         : 2; // bit 32+5:6
                    uint32_t eye_lens      : 1; // bit 32+7
                    uint32_t image_filter  : 3; // bit 32+8:10
                    uint32_t instance      : 1; // bit 32+11
                    uint32_t tone_curve    : 1; // bit 32+12
                    uint32_t light_proj    : 1; // bit 32+13
                    uint32_t magnify       : 4; // bit 32+14:17
                    uint32_t membrane      : 2; // bit 32+18:19
                    uint32_t mlaa          : 2; // bit 32+20:21
                    uint32_t mlaa_search   : 2; // bit 32+22:23
                    uint32_t morph_color   : 1; // bit 32+24
                    uint32_t morph         : 1; // bit 32+25
                    uint32_t movie         : 2; // bit 32+26:27
                    uint32_t u24           : 2; // bit 32+28:29
                    uint32_t u25           : 1; // bit 32+30
                    uint32_t npr_normal    : 1; // bit 32+31
                } m;
                uint32_t w;
            } y;
            union {
                struct {
                    uint32_t npr           : 1; // bit 64+0
                    uint32_t stage_shadow2 : 2; // bit 64+1:2
                    uint32_t reflect       : 2; // bit 64+3:4
                    uint32_t reduce        : 4; // bit 64+5:8
                    uint32_t chara_shadow  : 1; // bit 64+9
                    uint32_t chara_shadow2 : 2; // bit 64+10:11
                    uint32_t u2d           : 3; // bit 64+12:14
                    uint32_t u2e           : 2; // bit 64+15:16
                    uint32_t show_vector   : 2; // bit 64+17:18
                    uint32_t skinning      : 1; // bit 64+19
                    uint32_t snow_particle : 2; // bit 64+20:21
                    uint32_t specular_ibl  : 2; // bit 64+22:23
                    uint32_t combiner      : 1; // bit 64+24
                    uint32_t tex_0_type    : 2; // bit 64+25:26
                    uint32_t tex_1_type    : 2; // bit 64+27:28
                    uint32_t sss_filter    : 2; // bit 64+29:30
                } m;
                uint32_t w;
            } z;
            union {
                struct {
                    uint32_t sss_chara     : 1; // bit 96+0
                    uint32_t star          : 1; // bit 96+1
                    uint32_t texture_count : 2; // bit 96+2:3
                    uint32_t env_map       : 1; // bit 96+4
                    uint32_t ripple        : 2; // bit 96+5:6
                    uint32_t translucency  : 1; // bit 96+7
                    uint32_t normal        : 1; // bit 96+8
                    uint32_t transparency  : 1; // bit 96+9
                    uint32_t water_reflect : 1; // bit 96+10
                    uint32_t u40           : 1; // bit 96+11
                    uint32_t u41           : 1; // bit 96+12
                    uint32_t stage_shadow  : 1; // bit 96+13
                    uint32_t specular      : 1; // bit 96+14
                    uint32_t tone_map      : 2; // bit 96+15
                    uint32_t u45           : 1; // bit 96+16
                } m;
                uint32_t w;
            } w;
        } g_shader_flags;

        void reset();
        void set_shader_flags(int32_t shader_flags[U_MAX]);
    };

    static_assert(sizeof(render_data::obj_shader_data) == 0x10, "\"render_data::obj_shader_data\" struct should have a size of 0x10");

    struct obj_npr_param_data {
        vec4i g_npr_flags;
        vec4 g_npr_param[8];
        vec4i g_npr_dbg_flags;
        vec4 g_npr_dbg_param[4];
    };

    static_assert(sizeof(render_data::obj_npr_param_data) == 0xE0, "\"render_data::obj_npr_param_data\" struct should have a size of 0xE0");

    render_data_flags flags;
    cam_data cam;
    vec4 inv_view_mat[3];
    DxShaderName shader_index;
    int32_t shader_permutation;
    uniform_value shader_flags;
    obj_shader_data buffer_shader_data;
    obj_scene_data buffer_scene_data;
    obj_batch_data buffer_batch_data;
    obj_pre_pass_param_data buffer_pre_pass_param_data;
    int32_t field_B3C;
    int64_t field_B40;
    p_dx_buffer buffer_shader;
    p_dx_buffer buffer_scene;
    p_dx_buffer buffer_batch;
    p_dx_buffer buffer_pre_pass_param;
    p_dx_buffer buffer_skinning;
    int32_t curr_matrix_stack;
    matrix_stack mat_stack[3];
    bool field_3B80;
};

static_assert(sizeof(render_data) == 0x3B88, "\"render_data\" struct should have a size of 0x3B88");

struct render_data_context {
    p_dx_state state;
    int32_t index;
    render_data* data;
    uniform_value* uniform;

    int32_t get_uniform_value(uniform_name name);
    void reset_render_target(p_dx_state& p_dx_st);
    void set_batch_alpha_threshold(float_t value);
    void set_batch_min_alpha(float_t value);
    void set_batch_sss_param(const vec4& value);
    void set_glitter_render_data_state();
    void set_npr(void* render_manager);
    void uniform_value_reset();

    inline void set_batch_scene_camera(cam_data& cam) {
        static void (*render_data_context__set_batch_scene_camera)(render_data_context * This, cam_data & cam)
            = (void (*)(render_data_context * This, cam_data & cam))0x00000001404D5090;
        render_data_context__set_batch_scene_camera(this, cam);
    }
};

static_assert(sizeof(render_data_context) == 0x20, "\"render_data_context\" struct should have a size of 0x20");

extern draw_state_struct& draw_state;

extern sss_data* sss_data_get();
