/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../../KKdLib/default.hpp"
#include "../../KKdLib/mat.hpp"
#include "../../KKdLib/vec.hpp"
#include "../../MMPlusModsShared/mdl/disp_manager.hpp"
#include "../../MMPlusModsShared/color.hpp"
#include "../../MMPlusModsShared/shadow.hpp"
#include "../object.hpp"

#define MATERIAL_LIST_COUNT 24
#define TEXTURE_PATTERN_COUNT 24
#define TEXTURE_TRANSFORM_COUNT 24

namespace mdl {
    enum EtcObjType {
        ETC_OBJ_TEAPOT = 0,
        ETC_OBJ_GRID,
        ETC_OBJ_CUBE,
        ETC_OBJ_SPHERE,
        ETC_OBJ_PLANE,
        ETC_OBJ_CONE,
        ETC_OBJ_LINE,
        ETC_OBJ_CROSS,
        ETC_OBJ_MAX,
    };

    enum ObjKind {
        OBJ_KIND_NORMAL = 0,
        OBJ_KIND_ETC,
        OBJ_KIND_USER,
        OBJ_KIND_TRANSLUCENT,
        OBJ_KIND_MAX,
    };

    enum ObjType {
        OBJ_TYPE_OPAQUE = 0,
        OBJ_TYPE_TRANSLUCENT,
        OBJ_TYPE_TRANSLUCENT_SORT_BY_RADIUS,
        OBJ_TYPE_TRANSPARENT,
        OBJ_TYPE_SHADOW_CHARA,
        OBJ_TYPE_SHADOW_STAGE,
        OBJ_TYPE_TYPE_6,
        OBJ_TYPE_TYPE_7,
        OBJ_TYPE_SHADOW_OBJECT_CHARA,
        OBJ_TYPE_SHADOW_OBJECT_STAGE,
        OBJ_TYPE_REFLECT_CHARA_OPAQUE,
        OBJ_TYPE_REFLECT_CHARA_TRANSLUCENT,
        OBJ_TYPE_REFLECT_CHARA_TRANSPARENT,
        OBJ_TYPE_REFLECT_OPAQUE,
        OBJ_TYPE_REFLECT_TRANSLUCENT,
        OBJ_TYPE_REFLECT_TRANSPARENT,
        OBJ_TYPE_REFRACT_OPAQUE,
        OBJ_TYPE_REFRACT_TRANSLUCENT,
        OBJ_TYPE_REFRACT_TRANSPARENT,
        OBJ_TYPE_SSS,
        OBJ_TYPE_OPAQUE_ALPHA_ORDER_POST_GLITTER,
        OBJ_TYPE_TRANSPARENT_ALPHA_ORDER_POST_GLITTER,
        OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_POST_GLITTER,
        OBJ_TYPE_OPAQUE_ALPHA_ORDER_POST_TRANSLUCENT,
        OBJ_TYPE_TRANSPARENT_ALPHA_ORDER_POST_TRANSLUCENT,
        OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_POST_TRANSLUCENT,
        OBJ_TYPE_OPAQUE_ALPHA_ORDER_POST_OPAQUE,
        OBJ_TYPE_TRANSPARENT_ALPHA_ORDER_POST_OPAQUE,
        OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_POST_OPAQUE,
        OBJ_TYPE_BEFORE_PUNCH,
        OBJ_TYPE_TYPE_30,
        OBJ_TYPE_TYPE_31,
        OBJ_TYPE_TYPE_32,
        OBJ_TYPE_USER,
        OBJ_TYPE_MAX,
    };

    enum ObjTypeScreen {
        OBJ_TYPE_SCREEN_OPAQUE = 0,
        OBJ_TYPE_SCREEN_TRANSLUCENT,
        OBJ_TYPE_SCREEN_TRANSPARENT,
        OBJ_TYPE_SCREEN_OPAQUE_ALPHA_ORDER_POST_TRANSLUCENT,
        OBJ_TYPE_SCREEN_TRANSPARENT_ALPHA_ORDER_POST_TRANSLUCENT,
        OBJ_TYPE_SCREEN_TRANSLUCENT_ALPHA_ORDER_POST_TRANSLUCENT,
        OBJ_TYPE_SCREEN_MAX
    };

    enum ObjTypeReflect {
        OBJ_TYPE_REFLECT_TRANSLUCENT_SORT_BY_RADIUS = 0,
        OBJ_TYPE_REFLECT_MAX,
    };

}

struct material_list_struct {
    uint64_t hash;
    vec4 blend_color;
    vec4u8 has_blend_color;
    vec4 emission;
    vec4u8 has_emission;

    material_list_struct();
    material_list_struct(uint64_t hash, vec4& blend_color,
        vec4u8& has_blend_color, vec4& emission, vec4u8& has_emission);
};

struct cam_data;
struct render_data_context;

namespace mdl {
    struct DispManager;
    struct ObjSubMeshArgs;

    typedef void(FASTCALL* draw_func)(render_data_context& rend_data_ctx,
        const ObjSubMeshArgs* args, const cam_data& cam, mat4* mat);

    struct ObjSubMeshArgs {
        const obj_sub_mesh* sub_mesh;
        const obj_mesh* mesh;
        const obj_material_data* material;
        const texture** textures;
        int32_t mat_count;
        const mat4* mats;
        p_dx_buffer* vertex_buffer;
        int32_t vertex_buffer_offset;
        p_dx_buffer* index_buffer;
        bool set_blend_color;
        bool chara_color;
        vec4 blend_color;
        int32_t self_shadow;
        shadow_type_enum shadow;
        p_dx_buffer* morph_vertex_buffer;
        int32_t morph_vertex_buffer_offset;
        float_t morph_weight;
        int32_t texture_pattern_count;
        const texture_pattern_struct* texture_pattern_array;
        vec4 texture_color_coeff;
        vec4 texture_color_offset;
        vec4 texture_specular_coeff;
        vec4 texture_specular_offset;
        int32_t texture_transform_count;
        const texture_transform_struct* texture_transform_array;
        int64_t field_D8;
        int32_t field_E0;
        int64_t field_E8;
        int32_t instances_count;
        const mat4* instances_mat;
        draw_func func;
        const ObjSubMeshArgs* func_data;
        vec4 emission;

        ObjSubMeshArgs();
    };

    static_assert(sizeof(mdl::ObjSubMeshArgs) == 0x120, "\" mdl::ObjSubMeshArgs\" struct should have a size of 0x120");

    struct ObjSubMeshArgsData {
        ObjSubMeshArgs* ptr;
        int32_t field_8;
        ObjSubMeshArgs data;

        ObjSubMeshArgsData();
    };

    static_assert(sizeof(mdl::ObjSubMeshArgsData) == 0x130, "\" mdl::ObjSubMeshArgsData\" struct should have a size of 0x130");

    struct EtcObjTeapot {
        float_t size;

        EtcObjTeapot();
    };

    static_assert(sizeof(mdl::EtcObjTeapot) == 0x04, "\" mdl::EtcObjTeapot\" struct should have a size of 0x04");

    struct EtcObjGrid {
        int32_t w;
        int32_t h;
        int32_t ws;
        int32_t hs;

        EtcObjGrid();
    };

    static_assert(sizeof(mdl::EtcObjGrid) == 0x10, "\" mdl::EtcObjGrid\" struct should have a size of 0x10");

    struct EtcObjCube {
        vec3 size;
        bool wire;

        EtcObjCube();
    };

    static_assert(sizeof(mdl::EtcObjCube) == 0x10, "\" mdl::EtcObjCube\" struct should have a size of 0x10");

    struct EtcObjSphere {
        float_t radius;
        int32_t slices;
        int32_t stacks;
        bool wire;

        EtcObjSphere();
    };

    static_assert(sizeof(mdl::EtcObjSphere) == 0x10, "\" mdl::EtcObjSphere\" struct should have a size of 0x10");

    struct EtcObjPlane {
        int32_t w;
        int32_t h;

        EtcObjPlane();
    };

    static_assert(sizeof(mdl::EtcObjPlane) == 0x08, "\" mdl::EtcObjPlane\" struct should have a size of 0x08");

    struct EtcObjCone {
        float_t base;
        float_t height;
        int32_t slices;
        int32_t stacks;
        bool wire;

        EtcObjCone();
    };

    static_assert(sizeof(mdl::EtcObjCone) == 0x14, "\" mdl::EtcObjCone\" struct should have a size of 0x14");

    struct EtcObjLine {
        vec3 pos[2];

        EtcObjLine();
    };

    static_assert(sizeof(mdl::EtcObjLine) == 0x18, "\" mdl::EtcObjLine\" struct should have a size of 0x18");

    struct EtcObjCross {
        float_t size;

        EtcObjCross();
    };

    struct EtcObj {
        union Data {
            EtcObjTeapot teapot;
            EtcObjGrid grid;
            EtcObjCube cube;
            EtcObjSphere sphere;
            EtcObjPlane plane;
            EtcObjCone cone;
            EtcObjLine line;
            EtcObjCross cross;

            Data();
        };

        static_assert(sizeof(mdl::EtcObj::Data) == 0x18, "\" mdl::EtcObj::Data\" struct should have a size of 0x18");

        EtcObjType type;
        color4u8_bgra color;
        bool fog;
        Data data;

        EtcObj(EtcObjType type);
    };

    static_assert(sizeof(mdl::EtcObj) == 0x24, "\" mdl::EtcObj\" struct should have a size of 0x24");

    struct EtcObjData {
        color4u8_bgra color;
        bool fog;
        int32_t first;
        int32_t count;
    };

    static_assert(sizeof(mdl::EtcObjData) == 0x10, "\" mdl::EtcObjData\" struct should have a size of 0x10");

    typedef void(*UserArgsFunc)(render_data_context& rend_data_ctx, void* data, const cam_data& cam, mat4* mat);

    struct UserArgs {
        UserArgsFunc func;
        void* data;

        UserArgs();
    };

    static_assert(sizeof(mdl::UserArgs) == 0x10, "\" mdl::UserArgs\" struct should have a size of 0x10");

    struct ObjTranslucentArgs {
        int32_t count;
        ObjSubMeshArgs* sub_mesh[40];

        ObjTranslucentArgs();
    };

    static_assert(sizeof(mdl::ObjTranslucentArgs) == 0x148, "\" mdl::ObjTranslucentArgs\" struct should have a size of 0x148");

    struct ObjTranslucentArgsData {
        ObjTranslucentArgs* ptr;
        int32_t field_8;
        ObjTranslucentArgs data;

        ObjTranslucentArgsData();
    };

    static_assert(sizeof(mdl::ObjTranslucentArgsData) == 0x158, "\" mdl::ObjTranslucentArgs\" struct should have a size of 0x158");

    struct ObjData {
        union Args {
            ObjSubMeshArgsData sub_mesh;
            EtcObjData etc;
            UserArgs user;
            ObjTranslucentArgsData translucent;

            Args();
        };

        static_assert(sizeof(mdl::ObjData::Args) == 0x158, "\" mdl::ObjData::Args\" struct should have a size of 0x158");

        ObjKind kind;
        mat4 mat;
        union {
            float_t radius;
            float_t view_z;
        };
        Args args;

        ObjData();

        void init_sub_mesh(const mat4& mat, float_t radius, const obj_sub_mesh* sub_mesh,
            const obj_mesh* mesh, const obj_material_data* material, const texture** textures,
            int32_t mat_count, const mat4* mats, p_dx_buffer* vertex_buffer, int32_t vertex_buffer_offset,
            p_dx_buffer* index_buffer, const vec4& blend_color, const vec4& emission, p_dx_buffer* morph_vertex_buffer,
            int32_t morph_vertex_buffer_offset, float_t morph_weight,
            int32_t texture_pattern_count, const texture_pattern_struct* texture_pattern_array,
            int32_t texture_transform_count, const texture_transform_struct* texture_transform_array,
            int64_t field_D8, int32_t field_E0, int64_t field_E8,
            int32_t instances_count, const mat4* instances_mat, draw_func func, const ObjSubMeshArgs* func_data);
        void init_translucent(const mat4& mat, const ObjTranslucentArgs& translucent);
    };

    static_assert(sizeof(mdl::ObjData) == 0x1A0, "\" mdl::ObjData\" struct should have a size of 0x1A0");

    struct ObjListData {
        int32_t count;
        ObjListData* next;
        ObjData* data[16];
    };

    static_assert(sizeof(mdl::ObjListData) == 0x90, "\"mdl::ObjListData\" struct should have a size of 0x90");

    struct ObjList {
        ObjListData* first;
        ObjListData* last;
        int32_t count;
    };

    static_assert(sizeof(mdl::ObjList) == 0x18, "\"mdl::ObjList\" struct should have a size of 0x18");

    struct CullingCheck {
        struct Info {
            int32_t objects;
            int32_t meshes;
            int32_t sub_meshes;
            int pad;
        };

        Info passed;
        Info culled;
        Info passed_prev;
        Info culled_prev;
    };

    static_assert(sizeof(mdl::CullingCheck) == 0x40, "\"mdl::CullingCheck\" struct should have a size of 0x40");

    struct ObjDataSort {
        ObjData* data;
        float_t view_z;
        float_t radius;
        float_t abs_view_z;
    };

    static_assert(sizeof(mdl::ObjDataSort) == 0x18, "\"mdl::ObjDataSort\" struct should have a size of 0x18");

    struct ObjListSort {
        ObjDataSort* data;
        int32_t count;
    };

    static_assert(sizeof(mdl::ObjListSort) == 0x10, "\"mdl::ObjListSort\" struct should have a size of 0x10");

    struct ObjDataListSort {
        ObjDataSort data[4096];
        ObjListSort list[mdl::OBJ_TYPE_MAX];
        int32_t flags;
    };

    static_assert(sizeof(mdl::ObjDataListSort) == 0x18228, "\"mdl::ObjDataListSort\" struct should have a size of 0x18228");

    struct DispManager {
        ObjFlags obj_flags;
        shadow_type_enum shadow_type;
        int64_t field_8;
        ObjListData* data_buff;
        int32_t data_buff_max_count;
        int32_t data_buff_count;
        ObjList obj[OBJ_TYPE_MAX];
        bool field_350;
        CullingCheck culling;
        int32_t field_394;
        bool show_alpha_center;
        bool show_mat_center;
        bool object_culling;
        bool object_sort;
        bool chara_color;
        int32_t buff_offset;
        int32_t buff_max;
        int32_t buff_size;
        size_t buff;
        morph_struct morph;
        int32_t texture_pattern_count;
        texture_pattern_struct texture_pattern_array[24];
        vec4 texture_color_coeff;
        vec4 texture_color_offset;
        vec4 texture_specular_coeff;
        vec4 texture_specular_offset;
        float_t wet_param;
        int32_t texture_transform_count;
        texture_transform_struct texture_transform_array[24];
        bool(FASTCALL* culling_func)(const obj_bounding_sphere*, const mat4*);
        ObjSubMeshArgs* translucent_objects[40];
        int32_t field_C78;

        int32_t get_obj_count(ObjType type);
        int32_t get_obj_count(ObjTypeScreen type);
        int32_t get_obj_count(ObjTypeReflect type);
        void set_obj_flags(ObjFlags flags);
        ObjList& get_obj_list(ObjType type);
        ObjList& get_obj_list(ObjTypeScreen type);
        ObjList& get_obj_list(ObjTypeReflect type);
        void set_material_list(int32_t count = 0, const material_list_struct* value = 0);
        void set_texture_pattern(int32_t count = 0, const texture_pattern_struct* value = 0);
        void set_texture_transform(int32_t count = 0, const texture_transform_struct* value = 0);

        static void* alloc_data(int32_t size);
        static ObjData* alloc_obj_data(ObjKind kind);
        static mat4* alloc_mat4_array(int32_t count);
        static void draw(render_data_context& rend_data_ctx, ObjType type, const cam_data& cam,
            int32_t first = 0, int32_t count = -1, int32_t a6 = 0, int32_t a7 = 0,
            bool reflect_texture_mask = true, int32_t alpha = -1, bool a10 = true);
        static void draw(render_data_context& rend_data_ctx, ObjTypeScreen type, const cam_data& cam,
            int32_t first = 0, int32_t count = -1, int32_t alpha = -1, bool a10 = true);
        static void draw(render_data_context& rend_data_ctx, ObjTypeReflect type, const cam_data& cam,
            int32_t first = 0, int32_t count = -1, bool a10 = true);
        static void entry_list(ObjType type, ObjData* data);
        static void entry_list(ObjTypeScreen type, ObjData* data);
        static void entry_list(ObjTypeReflect type, ObjData* data);
        static void entry_obj(const ::obj* obj, const mat4& mat, obj_mesh_vertex_buffer* obj_vert_buf,
            obj_mesh_index_buffer* obj_index_buf, const texture** textures, const vec4* blend_color = 0,
            const mat4* bone_mat = 0, const ::obj* obj_morph = 0, obj_mesh_vertex_buffer* obj_morph_vert_buf = 0,
            float_t morph_value = 0.0f, int32_t instances_count = 0, const mat4* instances_mat = 0, int64_t a13 = 0,
            int64_t a14 = 0, draw_func func = 0, const ObjSubMeshArgs* func_data = 0, bool enable_bone_mat = false);
        static void entry_obj_screen(const ::obj* obj, const mat4& mat, obj_mesh_vertex_buffer* obj_vert_buf,
            obj_mesh_index_buffer* obj_index_buf, const texture** textures, const vec4* blend_color = 0);
        static void entry_obj_reflect(const ::obj* obj, const mat4& mat, obj_mesh_vertex_buffer* obj_vert_buf,
            obj_mesh_index_buffer* obj_index_buf, const texture** textures, const vec4* blend_color = 0,
            const mat4* bone_mat = 0, const ::obj* obj_morph = 0, obj_mesh_vertex_buffer* obj_morph_vert_buf = 0,
            float_t morph_value = 0.0f, int32_t instances_count = 0, const mat4* instances_mat = 0, int64_t a13 = 0,
            int64_t a14 = 0, draw_func func = 0, const ObjSubMeshArgs* func_data = 0, bool enable_bone_mat = false);
        static void entry_obj_by_obj(const ::obj* obj, const texture** textures,
            obj_mesh_vertex_buffer* obj_vert_buf, obj_mesh_index_buffer* obj_index_buf,
            const mat4* bone_mat, float_t alpha);
        static void entry_obj_by_object_info(object_info obj_info, const vec4* blend_color = 0,
            const mat4* bone_mat = 0, int32_t instances_count = 0, const mat4* instances_mat = 0,
            draw_func func = 0, const ObjSubMeshArgs* func_data = 0, bool enable_bone_mat = false);
        static void entry_obj_by_object_info_screen(object_info obj_info, const vec4* blend_color = 0,
            const mat4* bone_mat = 0, int32_t instances_count = 0, const mat4* instances_mat = 0,
            draw_func func = 0, const ObjSubMeshArgs* func_data = 0, bool enable_bone_mat = false);
        static void entry_obj_by_object_info_cache(object_info_cache& obj_info_cache,
            const mat4& mat, const vec4* blend_color = 0, const mat4* bone_mat = 0,
            int32_t instances_count = 0, const mat4* instances_mat = 0, draw_func func = 0,
            const ObjSubMeshArgs* func_data = 0, bool enable_bone_mat = false);
        static void entry_obj_by_object_info_cache_screen(object_info_cache& obj_info_cache,
            const mat4& mat, const vec4* blend_color = 0);
        static ObjListSort& get_obj_data_list_sort(int32_t index, ObjType type);
        static ObjListSort& get_obj_data_list_sort(int32_t index, ObjTypeScreen type);
        static ObjListSort& get_obj_data_list_sort(int32_t index, ObjTypeReflect type);
        static void obj_sort(render_data_context& rend_data_ctx,
            ObjType type, int32_t compare_func, const cam_data& cam, bool a5 = false);
        static void obj_sort(render_data_context& rend_data_ctx,
            ObjTypeScreen type, int32_t compare_func, const cam_data& cam);
        static void obj_sort(render_data_context& rend_data_ctx,
            ObjTypeReflect type, int32_t compare_func, const cam_data& cam);
    };

    static_assert(sizeof(mdl::DispManager) == 0xC80, "\"mdl::DispManager\" struct should have a size of 0xC80");

    extern bool obj_reflect_enable;

    extern void disp_manager_patch();
}

extern mdl::DispManager*& disp_manager;
extern mdl::ObjDataListSort* obj_data_list_sort;
