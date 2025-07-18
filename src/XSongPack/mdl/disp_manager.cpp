/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "disp_manager.hpp"
#include "../../KKdLib/hash.hpp"
#include "../../MMPlusModsShared/camera.hpp"
#include "../../MMPlusModsShared/matrix_stack.hpp"
#include "../../MMPlusModsShared/render_context.hpp"
#include "../../MMPlusModsShared/render_manager.hpp"
#include "../wrap.hpp"
#include <Helpers.h>

mdl::DispManager*& disp_manager = *(mdl::DispManager**)0x0000000141148650;
mdl::ObjDataListSort* obj_data_list_sort = (mdl::ObjDataListSort*)0x00000001416ED140;

extern bool screen_draw[4];
extern bool reflect_draw[4];
extern mat4 reflect_mat;

material_list_struct::material_list_struct() : blend_color(), has_blend_color(), emission(), has_emission() {
    hash = (uint64_t)-1;
}

material_list_struct::material_list_struct(uint64_t hash, vec4& blend_color,
    vec4u8& has_blend_color, vec4& emission, vec4u8& has_emission) : hash(hash), blend_color(blend_color),
    has_blend_color(has_blend_color), emission(emission), has_emission(has_emission) {

}

namespace mdl {
    int32_t material_list_count;
    material_list_struct material_list_array[MATERIAL_LIST_COUNT];

    ObjList obj_screen[OBJ_TYPE_SCREEN_MAX];
    ObjListSort obj_list_sort_screen[4][OBJ_TYPE_SCREEN_MAX];
    ObjList obj_reflect[OBJ_TYPE_REFLECT_MAX];
    ObjListSort obj_list_sort_reflect[4][OBJ_TYPE_REFLECT_MAX];

    bool obj_reflect_enable;

    ObjSubMeshArgs::ObjSubMeshArgs() : sub_mesh(), mesh(), material(), textures(), mat_count(), mats(),
        vertex_buffer(), vertex_buffer_offset(), index_buffer(), set_blend_color(), chara_color(), self_shadow(),
        shadow(), morph_vertex_buffer(), morph_vertex_buffer_offset(), morph_weight(), texture_pattern_count(),
        texture_pattern_array(), texture_transform_count(), texture_transform_array(), field_D8(),
        field_E0(), field_E8(), instances_count(), instances_mat(), func(), func_data() {

    }

    ObjSubMeshArgsData::ObjSubMeshArgsData() : ptr(), field_8() {

    }

    EtcObjTeapot::EtcObjTeapot() {
        size = 1.0f;
    }

    EtcObjGrid::EtcObjGrid() {
        w = 10;
        h = 10;
        ws = 20;
        hs = 20;
    }

    EtcObjCube::EtcObjCube() : wire() {

    }

    EtcObjSphere::EtcObjSphere() : wire() {
        radius = 1.0f;
        slices = 8;
        stacks = 8;
    }

    EtcObjPlane::EtcObjPlane() {
        w = 10;
        h = 10;
    }

    EtcObjCone::EtcObjCone() : wire() {
        base = 1.0f;
        height = 1.0f;
        slices = 8;
        stacks = 8;
    }

    EtcObjLine::EtcObjLine() {
        pos[0] = { 0.0f, 0.0f, 0.0f };
        pos[1] = { 0.0f, 0.0f, 1.0f };
    }

    EtcObjCross::EtcObjCross() {
        size = 0.1f;
    }

    EtcObj::Data::Data() : line() {

    }

    EtcObj::EtcObj(EtcObjType type) {
        this->type = type;
        color = 0xFFFFFFFF;
        fog = false;
        switch (type) {
        case ETC_OBJ_TEAPOT:
            data.teapot = {};
            break;
        case ETC_OBJ_GRID:
            data.grid = {};
            break;
        case ETC_OBJ_CUBE:
            data.cube = {};
            break;
        case ETC_OBJ_SPHERE:
            data.sphere = {};
            break;
        case ETC_OBJ_PLANE:
            data.plane = {};
            break;
        case ETC_OBJ_CONE:
            data.cone = {};
            break;
        case ETC_OBJ_LINE:
            data.line = {};
            break;
        case ETC_OBJ_CROSS:
            data.cross = {};
            break;
        }
    }

    UserArgs::UserArgs() : func(), data() {

    }

    ObjTranslucentArgs::ObjTranslucentArgs() : count(), sub_mesh() {

    }

    ObjTranslucentArgsData::ObjTranslucentArgsData() : ptr(), field_8() {

    }

    ObjData::Args::Args() : sub_mesh() {

    }

    ObjData::ObjData() : kind(), radius() {

    }

    void ObjData::init_sub_mesh(const mat4& mat, float_t radius, const obj_sub_mesh* sub_mesh,
        const obj_mesh* mesh, const obj_material_data* material, const texture** textures,
        int32_t mat_count, const mat4* mats, p_dx_buffer* vertex_buffer, int32_t vertex_buffer_offset,
        p_dx_buffer* index_buffer, const vec4& blend_color, const vec4& emission, p_dx_buffer* morph_vertex_buffer,
        int32_t morph_vertex_buffer_offset, float_t morph_weight,
        int32_t texture_pattern_count, const texture_pattern_struct* texture_pattern_array,
        int32_t texture_transform_count, const texture_transform_struct* texture_transform_array,
        int64_t field_D8, int32_t field_E0, int64_t field_E8,
        int32_t instances_count, const mat4* instances_mat, draw_func func, const ObjSubMeshArgs* func_data) {
        kind = OBJ_KIND_NORMAL;
        this->mat = mat;
        this->radius = radius;

        ObjSubMeshArgs* args = this->args.sub_mesh.ptr;
        args->sub_mesh = sub_mesh;
        args->mesh = mesh;
        args->material = material;
        args->textures = textures;
        args->mat_count = mat_count;
        args->mats = mats;
        args->vertex_buffer = vertex_buffer;
        args->vertex_buffer_offset = vertex_buffer_offset;
        args->index_buffer = index_buffer;
        args->morph_vertex_buffer = morph_vertex_buffer;
        args->morph_vertex_buffer_offset = morph_vertex_buffer_offset;
        args->morph_weight = morph_weight;

        args->texture_pattern_count = texture_pattern_count;
        args->texture_pattern_array = texture_pattern_array;
        args->texture_transform_array = texture_transform_array;
        args->texture_transform_count = texture_transform_count;

        if (blend_color != 1.0f) {
            args->set_blend_color = true;
            args->blend_color = blend_color;
        }
        else {
            args->set_blend_color = false;
            args->blend_color = 1.0f;
        }

        args->emission = emission;

        args->chara_color = disp_manager->chara_color;
        if (disp_manager->obj_flags & mdl::OBJ_4)
            args->self_shadow = 1;
        else if (disp_manager->obj_flags & mdl::OBJ_8)
            args->self_shadow = 2;
        else
            args->self_shadow = 0;
        args->shadow = disp_manager->shadow_type;
        args->texture_color_coeff = disp_manager->texture_color_coeff;
        args->texture_color_coeff.w = disp_manager->wet_param;
        args->texture_color_offset = disp_manager->texture_color_offset;
        args->texture_specular_coeff = disp_manager->texture_specular_coeff;
        args->texture_specular_offset = disp_manager->texture_specular_offset;
        args->field_D8 = field_D8;
        args->field_E0 = field_E0;
        args->field_E8 = field_E8;
        args->instances_count = instances_count;
        args->instances_mat = instances_mat;
        args->func = func;
        args->func_data = func_data;
    }

    void ObjData::init_translucent(const mat4& mat, const ObjTranslucentArgs& translucent) {
        kind = OBJ_KIND_TRANSLUCENT;
        this->mat = mat;
        *args.translucent.ptr = translucent;
    }

    int32_t DispManager::get_obj_count(ObjType type) {
        return get_obj_list(type).count;
    }

    int32_t DispManager::get_obj_count(ObjTypeScreen type) {
        return get_obj_list(type).count;
    }

    int32_t DispManager::get_obj_count(ObjTypeReflect type) {
        return get_obj_list(type).count;
    }

    void DispManager::set_obj_flags(ObjFlags flags) {
        obj_flags = flags;
    }

    ObjList& DispManager::get_obj_list(ObjType type) {
        return obj[type];
    }

    ObjList& DispManager::get_obj_list(ObjTypeScreen type) {
        return obj_screen[type];
    }

    ObjList& DispManager::get_obj_list(ObjTypeReflect type) {
        return obj_reflect[type];
    }

    void DispManager::set_material_list(int32_t count, const material_list_struct* value) {
        if (count > MATERIAL_LIST_COUNT)
            return;

        material_list_count = count;

        if (count)
            for (int32_t i = 0; i < count; i++)
                material_list_array[i] = value[i];
        else
            for (int32_t i = 0; i < MATERIAL_LIST_COUNT; i++)
                material_list_array[i] = {};
    }

    void DispManager::set_texture_pattern(int32_t count, const texture_pattern_struct* value) {
        if (count > TEXTURE_PATTERN_COUNT)
            return;

        texture_pattern_count = count;

        if (count)
            for (int32_t i = 0; i < count; i++)
                texture_pattern_array[i] = value[i];
        else
            for (int32_t i = 0; i < TEXTURE_PATTERN_COUNT; i++)
                texture_pattern_array[i] = {};
    }

    void DispManager::set_texture_transform(int32_t count, const texture_transform_struct* value) {
        if (count > TEXTURE_TRANSFORM_COUNT)
            return;

        texture_transform_count = count;

        if (count)
            for (int32_t i = 0; i < count; i++)
                texture_transform_array[i] = value[i];
        else
            for (int32_t i = count; i < TEXTURE_TRANSFORM_COUNT; i++)
                texture_transform_array[i] = {};
    }

    void* DispManager::alloc_data(int32_t size) {
        if (!disp_manager->buff || disp_manager->buff_offset + size > disp_manager->buff_size)
            return 0;

        void* data = (void*)((size_t)disp_manager->buff + disp_manager->buff_offset);
        disp_manager->buff_offset += size;
        disp_manager->buff_max = max_def(disp_manager->buff_max, disp_manager->buff_offset);
        return data;
    }

    ObjData* DispManager::alloc_obj_data(ObjKind kind) {
        int32_t size = (int32_t)align_val(sizeof(ObjKind) + sizeof(mat4) + sizeof(float_t) * 2, 0x08);
        switch (kind) {
        case OBJ_KIND_NORMAL: {
            ObjData* data = (ObjData*)alloc_data(size + sizeof(ObjSubMeshArgsData));
            if (data)
                data->args.sub_mesh.ptr = &data->args.sub_mesh.data;
            return data;
        }
        case OBJ_KIND_ETC:
            return (ObjData*)alloc_data(size + sizeof(EtcObjData));
        case OBJ_KIND_USER:
            return (ObjData*)alloc_data(size + sizeof(UserArgs));
        case OBJ_KIND_TRANSLUCENT: {
            ObjData* data = (ObjData*)alloc_data(size + sizeof(ObjTranslucentArgsData));
            if (data)
                data->args.translucent.ptr = &data->args.translucent.data;
            return data;
        }
        default:
            return 0;
        }
    }

    mat4* DispManager::alloc_mat4_array(int32_t count) {
        return (mat4*)alloc_data(sizeof(mat4) * count);
    }

    void DispManager::draw(render_data_context& rend_data_ctx, ObjType type, const cam_data& cam,
        int32_t first, int32_t count, int32_t a6, int32_t a7, bool reflect_texture_mask, int32_t alpha, bool a10) {
        if (!disp_manager->get_obj_count(type))
            return;

        typedef void (*draw_func)(render_data_context& rend_data_ctx, ObjSubMeshArgs* args, const cam_data& cam, mat4* mat);

        static draw_func draw_sub_mesh_shadow = (draw_func)0x000000014045B480;
        static draw_func draw_sub_mesh_sss = (draw_func)0x000000014045B530;
        static draw_func draw_sub_mesh_default = (draw_func)0x00000001404629C0;
        static draw_func draw_sub_mesh_translucent = (draw_func)0x0000000140462AF0;
        static draw_func draw_sub_mesh_reflect_reflect_map = (draw_func)0x0000000140462E00;
        static draw_func draw_sub_mesh_reflect = (draw_func)0x0000000140462E80;

        static void (*draw_etc_obj)(render_data_context & rend_data_ctx, EtcObjData * etc, mat4 * mat)
            = (void (*)(render_data_context & rend_data_ctx, EtcObjData * etc, mat4 * mat))0x0000000140462F30;
        static void (*draw_sub_mesh)(render_data_context & rend_data_ctx, ObjSubMeshArgs * args, mat4 * mat, draw_func draw_object_func, __int64 a5, const cam_data & cam)
            = (void (*)(render_data_context & rend_data_ctx, ObjSubMeshArgs * args, mat4 * mat, draw_func draw_object_func, __int64 a5, const cam_data & cam))0x000000014045BDE0;

        static void (*shader_get_id_by_name)(render_data_context & rend_data_ctx, const char* str)
            = (void (*)(render_data_context & rend_data_ctx, const char* str))0x0000000140460AD0;

        int32_t alpha_test = 0;
        float_t min_alpha = 1.0f;
        float_t alpha_threshold = 0.0f;

        draw_func func = draw_sub_mesh_default;
        bool depth_write = true;

        rend_data_ctx.uniform_value_reset();
        switch (type) {
        case OBJ_TYPE_TRANSLUCENT:
        case OBJ_TYPE_TRANSLUCENT_SORT_BY_RADIUS:
            alpha_test = 1;
            min_alpha = 0.0f;
            depth_write = false;
            break;
        case OBJ_TYPE_TRANSPARENT:
            alpha_test = 1;
            min_alpha = 0.1f;
            alpha_threshold = 0.5f;
            break;
        case OBJ_TYPE_SHADOW_CHARA:
        case OBJ_TYPE_SHADOW_STAGE:
        case OBJ_TYPE_SHADOW_OBJECT_CHARA:
        case OBJ_TYPE_SHADOW_OBJECT_STAGE:
            func = draw_sub_mesh_shadow;
            break;
        case OBJ_TYPE_TYPE_6:
        case OBJ_TYPE_TYPE_30:
        case OBJ_TYPE_TYPE_31:
        case OBJ_TYPE_TYPE_32:
            func = draw_sub_mesh_translucent;
            shader_get_id_by_name(rend_data_ctx, "PREPASS");
            break;
        case OBJ_TYPE_TYPE_7:
            func = draw_sub_mesh_translucent;
            shader_get_id_by_name(rend_data_ctx, "SIL");

            alpha_test = 1;
            min_alpha = 0.0f;
            alpha_threshold = 1.0f;
            break;
        case OBJ_TYPE_REFLECT_CHARA_OPAQUE:
            draw_state.set_cull_front(rend_data_ctx, true);
            rend_data_ctx.state.set_rasterizer_state(dx_default_states_get_rasterizer_state(DX_CULL_FRONT));

            if (!reflect_draw[rend_data_ctx.index]) {
                if (rend_data_ctx.get_uniform_value(U_REFLECT) == 1)
                    func = draw_sub_mesh_reflect;
                else if (render_manager.reflect_type == STAGE_DATA_REFLECT_REFLECT_MAP)
                    func = draw_sub_mesh_reflect_reflect_map;
            }
            break;
        case OBJ_TYPE_REFLECT_CHARA_TRANSLUCENT:
            draw_state.set_cull_front(rend_data_ctx, true);
            rend_data_ctx.state.set_rasterizer_state(dx_default_states_get_rasterizer_state(DX_CULL_FRONT));

            min_alpha = 0.0f;

            if (!reflect_draw[rend_data_ctx.index]) {
                if (rend_data_ctx.get_uniform_value(U_REFLECT) == 1)
                    func = draw_sub_mesh_reflect;
                else if (render_manager.reflect_type == STAGE_DATA_REFLECT_REFLECT_MAP)
                    func = draw_sub_mesh_reflect_reflect_map;
            }
            break;
        case OBJ_TYPE_REFLECT_CHARA_TRANSPARENT:
            draw_state.set_cull_front(rend_data_ctx, true);
            rend_data_ctx.state.set_rasterizer_state(dx_default_states_get_rasterizer_state(DX_CULL_FRONT));

            alpha_test = 1;
            min_alpha = 0.1f;
            alpha_threshold = 0.5f;

            if (!reflect_draw[rend_data_ctx.index]) {
                if (rend_data_ctx.get_uniform_value(U_REFLECT) == 1)
                    func = draw_sub_mesh_reflect;
                else if (render_manager.reflect_type == STAGE_DATA_REFLECT_REFLECT_MAP)
                    func = draw_sub_mesh_reflect_reflect_map;
            }
            break;
        case OBJ_TYPE_REFLECT_OPAQUE:
            alpha_test = 1;
            alpha_threshold = 0.5f;

            if (reflect_draw[rend_data_ctx.index]) {
                draw_state.set_cull_front(rend_data_ctx, true);
                rend_data_ctx.state.set_rasterizer_state(dx_default_states_get_rasterizer_state(DX_CULL_FRONT));
            }
            else if (!reflect_texture_mask)
                func = draw_sub_mesh_reflect_reflect_map;
            break;
        case OBJ_TYPE_REFLECT_TRANSLUCENT:
            alpha_test = 1;
            min_alpha = 0.0f;
            depth_write = false;

            if (reflect_draw[rend_data_ctx.index]) {
                draw_state.set_cull_front(rend_data_ctx, true);
                rend_data_ctx.state.set_rasterizer_state(dx_default_states_get_rasterizer_state(DX_CULL_FRONT));
            }
            break;
        case OBJ_TYPE_REFLECT_TRANSPARENT:
            alpha_test = 1;
            min_alpha = 0.1f;
            alpha_threshold = 0.5f;

            if (reflect_draw[rend_data_ctx.index]) {
                draw_state.set_cull_front(rend_data_ctx, true);
                rend_data_ctx.state.set_rasterizer_state(dx_default_states_get_rasterizer_state(DX_CULL_FRONT));
            }
            break;
        case OBJ_TYPE_REFRACT_TRANSLUCENT:
            alpha_test = 1;
            min_alpha = 0.0f;
            depth_write = false;
            break;
        case OBJ_TYPE_REFRACT_TRANSPARENT:
            alpha_test = 1;
            min_alpha = 0.1f;
            alpha_threshold = 0.5f;
            break;
        case OBJ_TYPE_SSS:
            func = draw_sub_mesh_sss;

            if (reflect_draw[rend_data_ctx.index]) {
                draw_state.set_cull_front(rend_data_ctx, true);
                rend_data_ctx.state.set_rasterizer_state(dx_default_states_get_rasterizer_state(DX_CULL_FRONT));
            }
            break;
        case OBJ_TYPE_OPAQUE_ALPHA_ORDER_POST_GLITTER:
        case OBJ_TYPE_OPAQUE_ALPHA_ORDER_POST_TRANSLUCENT:
        case OBJ_TYPE_OPAQUE_ALPHA_ORDER_POST_OPAQUE:
            if (reflect_draw[rend_data_ctx.index]) {
                draw_state.set_cull_front(rend_data_ctx, true);
                rend_data_ctx.state.set_rasterizer_state(dx_default_states_get_rasterizer_state(DX_CULL_FRONT));
            }
            break;
        case OBJ_TYPE_TRANSPARENT_ALPHA_ORDER_POST_GLITTER:
        case OBJ_TYPE_TRANSPARENT_ALPHA_ORDER_POST_TRANSLUCENT:
        case OBJ_TYPE_TRANSPARENT_ALPHA_ORDER_POST_OPAQUE:
            alpha_test = 1;
            min_alpha = 0.1f;
            alpha_threshold = 0.5f;

            if (reflect_draw[rend_data_ctx.index]) {
                draw_state.set_cull_front(rend_data_ctx, true);
                rend_data_ctx.state.set_rasterizer_state(dx_default_states_get_rasterizer_state(DX_CULL_FRONT));
            }
            break;
        case OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_POST_GLITTER:
        case OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_POST_TRANSLUCENT:
        case OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_POST_OPAQUE:
            alpha_test = 1;
            min_alpha = 0.0f;
            depth_write = false;

            if (reflect_draw[rend_data_ctx.index]) {
                draw_state.set_cull_front(rend_data_ctx, true);
                rend_data_ctx.state.set_rasterizer_state(dx_default_states_get_rasterizer_state(DX_CULL_FRONT));
            }
            break;
        case OBJ_TYPE_BEFORE_PUNCH:
            alpha_test = 1;
            min_alpha = 0.0f;
            depth_write = false;
            break;
        case OBJ_TYPE_USER:
            func = draw_sub_mesh_translucent;
            break;
        case OBJ_TYPE_REFLECT_TRANSLUCENT_SORT_BY_RADIUS:
            alpha_test = 1;
            min_alpha = 0.0f;
            depth_write = false;

            draw_state.set_cull_front(rend_data_ctx, true);
            rend_data_ctx.state.set_rasterizer_state(dx_default_states_get_rasterizer_state(DX_CULL_FRONT));
            break;
        }

        rend_data_ctx.state.set_depth_stencil_state(dx_default_states_get_depth_stencil_state(
            depth_write ? DX_DEPTH_WRITE : DX_DEPTH_TEST, a10 ? DX_DEPTH_FUNC_GREATER_EQUAL : DX_DEPTH_FUNC_LESS_EQUAL));

        rend_data_ctx.set_batch_alpha_threshold(alpha_threshold);
        rend_data_ctx.set_batch_min_alpha(min_alpha);
        rend_data_ctx.uniform->arr[U_ALPHA_TEST] = alpha_test;

        ObjListSort& list = get_obj_data_list_sort(rend_data_ctx.index, type);
        if (count < 0)
            count = list.count - first;

        if (count > 0) {
            ObjDataSort* data_sort = list.data;
            data_sort += first;

            if (alpha < 0)
                for (; count > 0; count--, data_sort++) {
                    ObjData* data = data_sort->data;
                    switch (data_sort->data->kind) {
                    case OBJ_KIND_NORMAL: {
                        draw_sub_mesh(rend_data_ctx, data->args.sub_mesh.ptr, &data->mat, func, 0, cam);
                    } break;
                    case OBJ_KIND_ETC: {
                        draw_etc_obj(rend_data_ctx, (mdl::EtcObjData*)&data->args, &data->mat);
                    } break;
                    case OBJ_KIND_USER: {
                        data->args.user.func(rend_data_ctx, data->args.user.data, cam, &data->mat);
                    } break;
                    case OBJ_KIND_TRANSLUCENT: {
                        for (int32_t j = 0; j < data->args.translucent.ptr->count; j++)
                            draw_sub_mesh(rend_data_ctx, data->args.translucent.ptr->sub_mesh[j], &data->mat, func, 0, cam);
                    } break;
                    }
                }
            else
                for (; count > 0; count--, data_sort++) {
                    ObjData* data = data_sort->data;
                    switch (data_sort->data->kind) {
                    case OBJ_KIND_NORMAL: {
                        int32_t a = (int32_t)(data->args.sub_mesh.ptr->blend_color.w * 255.0f);
                        a = clamp_def(a, 0, 255);
                        if (a == alpha)
                            draw_sub_mesh(rend_data_ctx, data->args.sub_mesh.ptr, &data->mat, func, 0, cam);
                    } break;
                    case OBJ_KIND_TRANSLUCENT: {
                        for (int32_t j = 0; j < data->args.translucent.ptr->count; j++) {
                            ObjSubMeshArgs* args = data->args.translucent.ptr->sub_mesh[j];
                            int32_t a = (int32_t)(args->blend_color.w * 255.0f);
                            a = clamp_def(a, 0, 255);
                            if (a == alpha)
                                draw_sub_mesh(rend_data_ctx, args, &data->mat, func, 0, cam);
                        }
                    } break;
                    }
                }
        }

        rend_data_ctx.set_batch_alpha_threshold(0.0f);
        rend_data_ctx.uniform->arr[U_ALPHA_TEST] = 0;
        draw_state.set_cull_front(rend_data_ctx, false);
        rend_data_ctx.state.set_rasterizer_state(dx_default_states_get_rasterizer_state(DX_CULL_BACK));
        rend_data_ctx.uniform_value_reset();
    }

    void DispManager::draw(render_data_context& rend_data_ctx, ObjTypeScreen type,
        const cam_data& cam, int32_t first, int32_t count, int32_t alpha, bool a10) {
        if (!disp_manager->get_obj_count(type))
            return;

        typedef void (*draw_func)(render_data_context& rend_data_ctx, ObjSubMeshArgs* args, const cam_data& cam, mat4* mat);

        static draw_func draw_sub_mesh_default = (draw_func)0x00000001404629C0;

        static void (*draw_etc_obj)(render_data_context & rend_data_ctx, EtcObjData * etc, mat4 * mat)
            = (void (*)(render_data_context & rend_data_ctx, EtcObjData * etc, mat4 * mat))0x0000000140462F30;
        static void (*draw_sub_mesh)(render_data_context & rend_data_ctx, ObjSubMeshArgs * args, mat4 * mat, draw_func draw_object_func, __int64 a5, const cam_data & cam)
            = (void (*)(render_data_context & rend_data_ctx, ObjSubMeshArgs * args, mat4 * mat, draw_func draw_object_func, __int64 a5, const cam_data & cam))0x000000014045BDE0;

        static void (*shader_get_id_by_name)(render_data_context & rend_data_ctx, const char* str)
            = (void (*)(render_data_context & rend_data_ctx, const char* str))0x0000000140460AD0;

        int32_t alpha_test = 0;
        float_t min_alpha = 1.0f;
        float_t alpha_threshold = 0.0f;

        draw_func func = draw_sub_mesh_default;
        bool depth_write = true;

        rend_data_ctx.uniform_value_reset();
        switch (type) {
        case OBJ_TYPE_SCREEN_TRANSLUCENT:
            alpha_test = 1;
            min_alpha = 0.0f;
            depth_write = false;
            break;
        case OBJ_TYPE_SCREEN_TRANSPARENT:
            alpha_test = 1;
            min_alpha = 0.1f;
            alpha_threshold = 0.5f;
            break;
        case OBJ_TYPE_SCREEN_TRANSPARENT_ALPHA_ORDER_POST_TRANSLUCENT:
            alpha_test = 1;
            min_alpha = 0.1f;
            alpha_threshold = 0.5f;

            if (reflect_draw[rend_data_ctx.index]) {
                draw_state.set_cull_front(rend_data_ctx, true);
                rend_data_ctx.state.set_rasterizer_state(dx_default_states_get_rasterizer_state(DX_CULL_FRONT));
            }
            break;
        case OBJ_TYPE_SCREEN_TRANSLUCENT_ALPHA_ORDER_POST_TRANSLUCENT:
            alpha_test = 1;
            min_alpha = 0.0f;
            depth_write = false;

            if (reflect_draw[rend_data_ctx.index]) {
                draw_state.set_cull_front(rend_data_ctx, true);
                rend_data_ctx.state.set_rasterizer_state(dx_default_states_get_rasterizer_state(DX_CULL_FRONT));
            }
            break;
        }

        rend_data_ctx.state.set_depth_stencil_state(dx_default_states_get_depth_stencil_state(
            depth_write ? DX_DEPTH_WRITE : DX_DEPTH_TEST, a10 ? DX_DEPTH_FUNC_GREATER_EQUAL : DX_DEPTH_FUNC_LESS_EQUAL));

        rend_data_ctx.set_batch_alpha_threshold(alpha_threshold);
        rend_data_ctx.set_batch_min_alpha(min_alpha);
        rend_data_ctx.uniform->arr[U_ALPHA_TEST] = alpha_test;

        ObjListSort& list = get_obj_data_list_sort(rend_data_ctx.index, type);
        if (count < 0)
            count = list.count - first;

        if (count > 0) {
            ObjDataSort* data_sort = list.data;
            data_sort += first;

            if (alpha < 0)
                for (; count > 0; count--, data_sort++) {
                    ObjData* data = data_sort->data;
                    switch (data_sort->data->kind) {
                    case OBJ_KIND_NORMAL: {
                        draw_sub_mesh(rend_data_ctx, data->args.sub_mesh.ptr, &data->mat, func, 0, cam);
                    } break;
                    case OBJ_KIND_ETC: {
                        draw_etc_obj(rend_data_ctx, (mdl::EtcObjData*)&data->args, &data->mat);
                    } break;
                    case OBJ_KIND_USER: {
                        data->args.user.func(rend_data_ctx, data->args.user.data, cam, &data->mat);
                    } break;
                    case OBJ_KIND_TRANSLUCENT: {
                        for (int32_t j = 0; j < data->args.translucent.ptr->count; j++)
                            draw_sub_mesh(rend_data_ctx, data->args.translucent.ptr->sub_mesh[j], &data->mat, func, 0, cam);
                    } break;
                    }
                }
            else
                for (; count > 0; count--, data_sort++) {
                    ObjData* data = data_sort->data;
                    switch (data_sort->data->kind) {
                    case OBJ_KIND_NORMAL: {
                        int32_t a = (int32_t)(data->args.sub_mesh.ptr->blend_color.w * 255.0f);
                        a = clamp_def(a, 0, 255);
                        if (a == alpha)
                            draw_sub_mesh(rend_data_ctx, data->args.sub_mesh.ptr, &data->mat, func, 0, cam);
                    } break;
                    case OBJ_KIND_TRANSLUCENT: {
                        for (int32_t j = 0; j < data->args.translucent.ptr->count; j++) {
                            ObjSubMeshArgs* args = data->args.translucent.ptr->sub_mesh[j];
                            int32_t a = (int32_t)(args->blend_color.w * 255.0f);
                            a = clamp_def(a, 0, 255);
                            if (a == alpha)
                                draw_sub_mesh(rend_data_ctx, args, &data->mat, func, 0, cam);
                        }
                    } break;
                    }
                }
        }

        rend_data_ctx.set_batch_alpha_threshold(0.0f);
        rend_data_ctx.uniform->arr[U_ALPHA_TEST] = 0;
        draw_state.set_cull_front(rend_data_ctx, false);
        rend_data_ctx.state.set_rasterizer_state(dx_default_states_get_rasterizer_state(DX_CULL_BACK));
        rend_data_ctx.uniform_value_reset();
    }

    void DispManager::draw(render_data_context& rend_data_ctx, ObjTypeReflect type, const cam_data& cam, int32_t first, int32_t count, bool a10) {
        if (!disp_manager->get_obj_count(type))
            return;

        typedef void (*draw_func)(render_data_context& rend_data_ctx, ObjSubMeshArgs* args, const cam_data& cam, mat4* mat);

        static draw_func draw_sub_mesh_default = (draw_func)0x00000001404629C0;

        static void (*draw_etc_obj)(render_data_context & rend_data_ctx, EtcObjData * etc, mat4 * mat)
            = (void (*)(render_data_context & rend_data_ctx, EtcObjData * etc, mat4 * mat))0x0000000140462F30;
        static void (*draw_sub_mesh)(render_data_context & rend_data_ctx, ObjSubMeshArgs * args, mat4 * mat, draw_func draw_object_func, __int64 a5, const cam_data & cam)
            = (void (*)(render_data_context & rend_data_ctx, ObjSubMeshArgs * args, mat4 * mat, draw_func draw_object_func, __int64 a5, const cam_data & cam))0x000000014045BDE0;

        static void (*shader_get_id_by_name)(render_data_context & rend_data_ctx, const char* str)
            = (void (*)(render_data_context & rend_data_ctx, const char* str))0x0000000140460AD0;

        int32_t alpha_test = 0;
        float_t min_alpha = 1.0f;
        float_t alpha_threshold = 0.0f;

        draw_func func = draw_sub_mesh_default;
        bool depth_write = true;

        rend_data_ctx.uniform_value_reset();
        switch (type) {
        case OBJ_TYPE_REFLECT_TRANSLUCENT_SORT_BY_RADIUS:
            alpha_test = 1;
            min_alpha = 0.0f;
            depth_write = false;

            draw_state.set_cull_front(rend_data_ctx, true);
            rend_data_ctx.state.set_rasterizer_state(dx_default_states_get_rasterizer_state(DX_CULL_FRONT));
            break;
        }

        rend_data_ctx.state.set_depth_stencil_state(dx_default_states_get_depth_stencil_state(
            depth_write ? DX_DEPTH_WRITE : DX_DEPTH_TEST, a10 ? DX_DEPTH_FUNC_GREATER_EQUAL : DX_DEPTH_FUNC_LESS_EQUAL));

        rend_data_ctx.set_batch_alpha_threshold(alpha_threshold);
        rend_data_ctx.set_batch_min_alpha(min_alpha);
        rend_data_ctx.uniform->arr[U_ALPHA_TEST] = alpha_test;

        ObjListSort& list = get_obj_data_list_sort(rend_data_ctx.index, type);
        if (count < 0)
            count = list.count - first;

        if (count > 0) {
            ObjDataSort* data_sort = list.data;
            data_sort += first;

            for (; count > 0; count--, data_sort++) {
                ObjData* data = data_sort->data;
                switch (data_sort->data->kind) {
                case OBJ_KIND_NORMAL: {
                    draw_sub_mesh(rend_data_ctx, data->args.sub_mesh.ptr, &data->mat, func, 0, cam);
                } break;
                case OBJ_KIND_ETC: {
                    draw_etc_obj(rend_data_ctx, (mdl::EtcObjData*)&data->args, &data->mat);
                } break;
                case OBJ_KIND_USER: {
                    data->args.user.func(rend_data_ctx, data->args.user.data, cam, &data->mat);
                } break;
                case OBJ_KIND_TRANSLUCENT: {
                    for (int32_t j = 0; j < data->args.translucent.ptr->count; j++)
                        draw_sub_mesh(rend_data_ctx, data->args.translucent.ptr->sub_mesh[j], &data->mat, func, 0, cam);
                } break;
                }
            }
        }

        rend_data_ctx.set_batch_alpha_threshold(0.0f);
        rend_data_ctx.uniform->arr[U_ALPHA_TEST] = 0;
        draw_state.set_cull_front(rend_data_ctx, false);
        rend_data_ctx.state.set_rasterizer_state(dx_default_states_get_rasterizer_state(DX_CULL_BACK));
        rend_data_ctx.uniform_value_reset();
    }

    void DispManager::entry_obj(const ::obj* obj, const mat4& mat, obj_mesh_vertex_buffer* obj_vert_buf,
        obj_mesh_index_buffer* obj_index_buf, const texture** textures, const vec4* blend_color,
        const mat4* bone_mat, const ::obj* obj_morph, obj_mesh_vertex_buffer* obj_morph_vert_buf,
        float_t morph_value, int32_t instances_count, const mat4* instances_mat, int64_t a13,
        int64_t a14, draw_func func, const ObjSubMeshArgs* func_data, bool enable_bone_mat) {
        disp_manager->culling.passed.objects++;

        texture_pattern_struct* texture_pattern_array = 0;
        texture_transform_struct* texture_transform_array = 0;

        static int32_t(FASTCALL* obj_bounding_sphere_check_visibility)(
            const obj_bounding_sphere& sphere, const mat4 mat) = (int32_t(FASTCALL*)(
                const obj_bounding_sphere & sphere, const mat4 mat))0x000000014045DFC0;

        static int32_t(FASTCALL* obj_axis_aligned_bounding_box_check_visibility)(
            const obj_axis_aligned_bounding_box * aabb, const mat4 mat) = (int32_t(FASTCALL*)(
                const obj_axis_aligned_bounding_box * aabb, const mat4 mat))0x000000014045E0B0;

        for (int32_t i = 0; i < obj->num_mesh; i++) {
            const obj_mesh* mesh = &obj->mesh_array[i];
            const obj_mesh* mesh_morph = 0;
            if (obj_vert_buf && obj_morph_vert_buf) {
                if (obj_vert_buf[i].get_size() != obj_morph_vert_buf[i].get_size())
                    continue;

                if (obj_morph && i < obj_morph->num_mesh)
                    mesh_morph = &obj_morph->mesh_array[i];
            }

            if (disp_manager->object_culling && !instances_count && !bone_mat
                && !obj_bounding_sphere_check_visibility(mesh->bounding_sphere, mat)
                && (!mesh_morph || !obj_bounding_sphere_check_visibility(
                    mesh_morph->bounding_sphere, mat))) {
                disp_manager->culling.culled.meshes++;
                continue;
            }
            disp_manager->culling.passed.meshes++;

            int32_t translucent_priority_count = 0;

            for (int32_t j = 0; j < mesh->num_submesh; j++) {
                const obj_sub_mesh* sub_mesh = &mesh->submesh_array[j];
                const obj_sub_mesh* sub_mesh_morph = 0;
                if (sub_mesh->attrib.m.cloth)
                    continue;

                if (disp_manager->object_culling && !instances_count && !bone_mat) {
                    int32_t v47 = obj_bounding_sphere_check_visibility(
                        sub_mesh->bounding_sphere, mat);
                    if (v47 != 2 || (!mesh->attrib.m.billboard && !mesh->attrib.m.billboard_y_axis
                        && !mesh->attrib.m.disable_aabb_culling)) {
                        if (v47 == 2)
                            v47 = obj_axis_aligned_bounding_box_check_visibility(
                                sub_mesh->axis_aligned_bounding_box, mat);

                        if (!v47) {
                            if (!mesh_morph || j >= mesh_morph->num_submesh) {
                                disp_manager->culling.culled.sub_meshes++;
                                continue;
                            }

                            sub_mesh_morph = &mesh_morph->submesh_array[j];
                            if (!sub_mesh_morph) {
                                disp_manager->culling.culled.sub_meshes++;
                                continue;
                            }

                            int32_t v56 = obj_bounding_sphere_check_visibility(
                                sub_mesh_morph->bounding_sphere, mat);
                            if (v56 == 2)
                                v56 = obj_axis_aligned_bounding_box_check_visibility(
                                    sub_mesh_morph->axis_aligned_bounding_box, mat);

                            if (!v56) {
                                disp_manager->culling.culled.sub_meshes++;
                                continue;
                            }
                        }
                    }
                }
                disp_manager->culling.passed.sub_meshes++;

                int32_t num_bone_index = sub_mesh->num_bone_index;
                const uint16_t* bone_index = sub_mesh->bone_index_array;

                mat4* mats;
                if (num_bone_index && enable_bone_mat) {
                    mats = alloc_mat4_array(num_bone_index);
                    if (bone_mat)
                        for (int32_t k = 0; k < num_bone_index; k++, bone_index++)
                            mats[k] = bone_mat[*bone_index];
                    else
                        for (int32_t k = 0; k < num_bone_index; k++)
                            mats[k] = mat4_identity;
                }
                else {
                    mats = 0;
                    num_bone_index = 0;
                }

                static bool (*sub_1402C1D50)() = (bool (*)())0x00000001402C1D50;
                static bool (*sub_1402C1D60)() = (bool (*)())0x00000001402C1D60;
                static bool (*sub_1402C1CF0)() = (bool (*)())0x00000001402C1CF0;
                static bool (*sub_1402C1D90)() = (bool (*)())0x00000001402C1D90;
                obj_material_data* material = &obj->material_array[sub_mesh->material_index];
                if (sub_1402C1CF0() && (material->material.adjust_param.m.disp
                    || (material->material.adjust_param.m.permit_disp && !sub_1402C1D90())))
                    continue;

                ObjData* data = alloc_obj_data(mdl::OBJ_KIND_NORMAL);
                if (!data)
                    continue;

                p_dx_buffer* morph_vertex_buffer = 0;
                uint32_t morph_vertex_buffer_offset = 0;
                if (obj_morph_vert_buf)
                    obj_morph_vert_buf[i].get_buffer(morph_vertex_buffer, morph_vertex_buffer_offset);

                p_dx_buffer* index_buffer = 0;
                if (obj_index_buf)
                    index_buffer = &obj_index_buf[i].buffer;;

                p_dx_buffer* vertex_buffer = 0;
                uint32_t vertex_buffer_offset = 0;
                if (obj_vert_buf)
                    obj_vert_buf[i].get_buffer(vertex_buffer, vertex_buffer_offset);

                if (disp_manager->texture_pattern_count > 0 && !texture_pattern_array) {
                    texture_pattern_array = (texture_pattern_struct*)mdl::DispManager::alloc_data(
                        sizeof(texture_pattern_struct) * disp_manager->texture_pattern_count);
                    if (texture_pattern_array)
                        for (int32_t i = 0; i < disp_manager->texture_pattern_count; i++)
                            texture_pattern_array[i] = disp_manager->texture_pattern_array[i];
                }

                if (disp_manager->texture_transform_count > 0 && !texture_transform_array) {
                    texture_transform_array = (texture_transform_struct*)mdl::DispManager::alloc_data(
                        sizeof(texture_transform_struct) * disp_manager->texture_transform_count);
                    if (texture_transform_array)
                        for (int32_t i = 0; i < disp_manager->texture_transform_count; i++)
                            texture_transform_array[i] = disp_manager->texture_transform_array[i];
                }

                const uint64_t material_hash = hash_utf8_fnv1a64m(material->material.name);

                const material_list_struct* mat_list = 0;
                for (int32_t k = 0; k < material_list_count; k++)
                    if (material_list_array[k].hash == material_hash) {
                        mat_list = &material_list_array[k];
                        break;
                    }

                vec4 _blend_color = 1.0f;
                vec4 _emission = { 0.0f, 0.0f, 0.0f, 1.0f };

                if (mat_list) {
                    bool has_blend_color = false;
                    if (mat_list->has_blend_color.x) {
                        _blend_color.x = mat_list->blend_color.x;
                        has_blend_color = true;
                    }

                    if (mat_list->has_blend_color.y) {
                        _blend_color.y = mat_list->blend_color.y;
                        has_blend_color = true;
                    }

                    if (mat_list->has_blend_color.z) {
                        _blend_color.z = mat_list->blend_color.z;
                        has_blend_color = true;
                    }

                    if (mat_list->has_blend_color.w) {
                        _blend_color.w = mat_list->blend_color.w;
                        has_blend_color = true;
                    }

                    if (blend_color) {
                        if (!has_blend_color)
                            _blend_color = *blend_color;
                        else
                            _blend_color *= *blend_color;
                    }

                    bool has_emission = false;
                    if (mat_list->has_emission.x) {
                        _emission.x = mat_list->emission.x;
                        has_emission = true;
                    }

                    if (mat_list->has_emission.y) {
                        _emission.y = mat_list->emission.y;
                        has_emission = true;
                    }

                    if (mat_list->has_emission.z) {
                        _emission.z = mat_list->emission.z;
                        has_emission = true;
                    }

                    if (mat_list->has_emission.w) {
                        _emission.w = mat_list->emission.w;
                        has_emission = true;
                    }

                    if (!has_emission)
                        _emission = material->material.color.emission;
                }
                else {
                    if (blend_color)
                        _blend_color = *blend_color;
                    _emission = material->material.color.emission;
                }

                data->init_sub_mesh(mat, obj->bounding_sphere.radius, sub_mesh, mesh, material,
                    textures, num_bone_index, mats, vertex_buffer, vertex_buffer_offset, index_buffer,
                    _blend_color, _emission, morph_vertex_buffer, morph_vertex_buffer_offset,
                    morph_value, disp_manager->texture_pattern_count, texture_pattern_array,
                    disp_manager->texture_transform_count, texture_transform_array, 0, 0, 0,
                    instances_count, instances_mat, func, func_data);

                const ObjFlags obj_flags = disp_manager->obj_flags;
                if (obj_flags & mdl::OBJ_SHADOW_OBJECT) {
                    entry_list((ObjType)(OBJ_TYPE_SHADOW_OBJECT_CHARA
                        + disp_manager->shadow_type), data);

                    if (obj_flags & mdl::OBJ_USER)
                        entry_list(OBJ_TYPE_USER, data);
                    continue;
                }

                const obj_material_attrib_member attrib = material->material.attrib.m;
                if ((obj_flags & mdl::OBJ_ALPHA_ORDER) && data->args.sub_mesh.ptr->blend_color.w < 1.0f) {
                    if (!(obj_flags & mdl::OBJ_NO_TRANSLUCENCY)) {
                        bool translucent = false;
                        if (!attrib.flag_28 && (!attrib.punch_through
                            && (attrib.alpha_texture || attrib.alpha_material)
                            || sub_mesh->attrib.m.translucent)) {
                            if (!sub_1402C1CF0() || !material->material.adjust_param.m.force_opaque)
                                translucent = true;
                        }

                        if (translucent) {
                            if (!attrib.translucent_priority) {
                                if (obj_flags & mdl::OBJ_ALPHA_ORDER_POST_GLITTER)
                                    entry_list(OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_POST_GLITTER, data);
                                else if (obj_flags & mdl::OBJ_ALPHA_ORDER_POST_TRANSLUCENT)
                                    entry_list(OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_POST_TRANSLUCENT, data);
                                else
                                    entry_list(OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_POST_OPAQUE, data);
                            }
                            else if (translucent_priority_count < 40)
                                disp_manager->translucent_objects[translucent_priority_count++] = data->args.sub_mesh.ptr;
                        }
                        else {
                            if (attrib.punch_through) {
                                if (obj_flags & mdl::OBJ_ALPHA_ORDER_POST_GLITTER)
                                    entry_list(OBJ_TYPE_TRANSPARENT_ALPHA_ORDER_POST_GLITTER, data);
                                else if (obj_flags & mdl::OBJ_ALPHA_ORDER_POST_TRANSLUCENT)
                                    entry_list(OBJ_TYPE_TRANSPARENT_ALPHA_ORDER_POST_TRANSLUCENT, data);
                                else
                                    entry_list(OBJ_TYPE_TRANSPARENT_ALPHA_ORDER_POST_OPAQUE, data);
                            }
                            else {
                                if (obj_flags & mdl::OBJ_ALPHA_ORDER_POST_GLITTER)
                                    entry_list(OBJ_TYPE_OPAQUE_ALPHA_ORDER_POST_GLITTER, data);
                                else if (obj_flags & mdl::OBJ_ALPHA_ORDER_POST_TRANSLUCENT)
                                    entry_list(OBJ_TYPE_OPAQUE_ALPHA_ORDER_POST_TRANSLUCENT, data);
                                else
                                    entry_list(OBJ_TYPE_OPAQUE_ALPHA_ORDER_POST_OPAQUE, data);
                            }

                            if (obj_flags & mdl::OBJ_SSS)
                                entry_list(OBJ_TYPE_SSS, data);
                        }
                    }
                }
                else if (!attrib.flag_28 && (data->args.sub_mesh.ptr->blend_color.w < 1.0f
                    || !attrib.punch_through && attrib.alpha_texture | attrib.alpha_material
                    || sub_mesh->attrib.m.translucent)) {
                    if (!(obj_flags & mdl::OBJ_NO_TRANSLUCENCY)) {
                        if (!sub_1402C1CF0() || !material->material.adjust_param.m.force_opaque) {
                            if (sub_1402C1CF0() && material->material.adjust_param.m.before_punch
                                && (sub_1402C1D50() || sub_1402C1D60())) {
                                entry_list(OBJ_TYPE_BEFORE_PUNCH, data);
                                continue;
                            }

                            if (!attrib.translucent_priority)
                                if (mesh->attrib.m.translucent_sort_by_radius
                                    || obj_flags & mdl::OBJ_TRANSLUCENT_SORT_BY_RADIUS) {
                                    entry_list(OBJ_TYPE_TRANSLUCENT_SORT_BY_RADIUS, data);
                                }
                                else
                                    entry_list(OBJ_TYPE_TRANSLUCENT, data);
                            else if (translucent_priority_count < 40)
                                disp_manager->translucent_objects[translucent_priority_count++] = data->args.sub_mesh.ptr;
                        }
                    }
                }
                else {
                    if (obj_flags & mdl::OBJ_SHADOW)
                        entry_list((ObjType)(OBJ_TYPE_SHADOW_CHARA
                            + disp_manager->shadow_type), data);

                    if (obj_flags & mdl::OBJ_SSS)
                        entry_list(OBJ_TYPE_SSS, data);

                    if (attrib.punch_through) {
                        if (!(obj_flags & mdl::OBJ_NO_TRANSLUCENCY))
                            entry_list(OBJ_TYPE_TRANSPARENT, data);

                        if (obj_flags & mdl::OBJ_CHARA_REFLECT)
                            entry_list(OBJ_TYPE_REFLECT_CHARA_OPAQUE, data);

                        if (obj_flags & mdl::OBJ_REFLECT)
                            entry_list(OBJ_TYPE_REFLECT_OPAQUE, data);

                        if (obj_flags & mdl::OBJ_REFRACT)
                            entry_list(OBJ_TYPE_REFRACT_TRANSPARENT, data);
                    }
                    else {
                        if (!(obj_flags & mdl::OBJ_NO_TRANSLUCENCY))
                            entry_list(OBJ_TYPE_OPAQUE, data);

                        if (obj_flags & mdl::OBJ_CHARA_REFLECT)
                            entry_list(OBJ_TYPE_REFLECT_CHARA_OPAQUE, data);

                        if (obj_flags & mdl::OBJ_REFLECT)
                            entry_list(OBJ_TYPE_REFLECT_OPAQUE, data);

                        if (obj_flags & mdl::OBJ_REFRACT)
                            entry_list(OBJ_TYPE_REFRACT_OPAQUE, data);
                    }

                    if (obj_flags & mdl::OBJ_USER)
                        entry_list(OBJ_TYPE_USER, data);
                    continue;
                }

                if (obj_flags & mdl::OBJ_SHADOW)
                    entry_list((ObjType)(OBJ_TYPE_SHADOW_CHARA
                        + disp_manager->shadow_type), data);
                if (obj_flags & mdl::OBJ_40)
                    entry_list(OBJ_TYPE_TYPE_7, data);
                if (obj_flags & mdl::OBJ_CHARA_REFLECT)
                    entry_list(OBJ_TYPE_REFLECT_CHARA_OPAQUE, data);
                if (obj_flags & mdl::OBJ_REFLECT) {
                    if (render_manager.reflect_type == STAGE_DATA_REFLECT_REFLECT_MAP)
                        entry_list(OBJ_TYPE_REFLECT_OPAQUE, data);
                    else
                        entry_list(OBJ_TYPE_REFLECT_TRANSLUCENT, data);
                }
                if (obj_flags & mdl::OBJ_REFRACT)
                    entry_list(OBJ_TYPE_REFRACT_TRANSLUCENT, data);
                if (obj_flags & mdl::OBJ_USER)
                    entry_list(OBJ_TYPE_USER, data);
            }

            if (translucent_priority_count <= 0)
                continue;

            ObjTranslucentArgs translucent_args;
            translucent_args.count = 0;
            for (int32_t j = 62; j; j--)
                for (int32_t k = 0; k < translucent_priority_count; k++) {
                    ObjSubMeshArgs* sub_mesh = disp_manager->translucent_objects[k];
                    if (sub_mesh->material->material.attrib.m.translucent_priority != j)
                        continue;

                    translucent_args.sub_mesh[translucent_args.count] = sub_mesh;
                    translucent_args.count++;
                }

            ObjData* data = alloc_obj_data(mdl::OBJ_KIND_TRANSLUCENT);
            if (data) {
                data->init_translucent(mat, translucent_args);
                if (disp_manager->obj_flags & mdl::OBJ_ALPHA_ORDER_POST_GLITTER)
                    entry_list(OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_POST_GLITTER, data);
                else if (disp_manager->obj_flags & mdl::OBJ_ALPHA_ORDER_POST_TRANSLUCENT)
                    entry_list(OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_POST_TRANSLUCENT, data);
                else if (disp_manager->obj_flags & mdl::OBJ_ALPHA_ORDER_POST_OPAQUE)
                    entry_list(OBJ_TYPE_TRANSLUCENT_ALPHA_ORDER_POST_OPAQUE, data);
                else
                    entry_list(OBJ_TYPE_TRANSLUCENT, data);
            }
        }

        if ((disp_manager->obj_flags & mdl::OBJ_SHADOW_OBJECT)
            || ((disp_manager->obj_flags & mdl::OBJ_ALPHA_ORDER) && blend_color && blend_color->w < 1.0f))
            return;

        mat4 _mat;
        mat4_transpose(&mat, &_mat);
        mat4_mul(&_mat, &reflect_mat, &_mat);
        mat4_transpose(&_mat, &_mat);

        bool camera_front = vec3::normalize(camera_data.view_point - camera_data.interest).z >= 0.0f;

        for (int32_t i = 0; i < obj->num_mesh; i++) {
            const obj_mesh* mesh = &obj->mesh_array[i];
            const obj_mesh* mesh_morph = 0;
            if (mesh->attrib.m.no_reflect
                || !mdl::obj_reflect_enable && !mesh->attrib.m.reflect
                || camera_front && mesh->attrib.m.reflect_cam_back)
                continue;

            if (obj_vert_buf && obj_morph_vert_buf) {
                if (obj_vert_buf[i].get_size() != obj_morph_vert_buf[i].get_size())
                    continue;

                if (obj_morph && i < obj_morph->num_mesh)
                    mesh_morph = &obj_morph->mesh_array[i];
            }

            if (disp_manager->object_culling && !instances_count && !bone_mat
                && !obj_bounding_sphere_check_visibility(mesh->bounding_sphere, _mat)) {
                if (mesh_morph) {
                    if (!obj_bounding_sphere_check_visibility(mesh_morph->bounding_sphere, _mat)) {
                        disp_manager->culling.culled.meshes++;
                        continue;
                    }
                }
                else {
                    disp_manager->culling.culled.meshes++;
                    continue;
                }
            }
            disp_manager->culling.passed.meshes++;

            int32_t translucent_priority_count = 0;

            for (int32_t j = 0; j < mesh->num_submesh; j++) {
                const obj_sub_mesh* sub_mesh = &mesh->submesh_array[j];
                const obj_sub_mesh* sub_mesh_morph = 0;
                if (sub_mesh->attrib.m.cloth || sub_mesh->attrib.m.no_reflect
                    || !mdl::obj_reflect_enable && !sub_mesh->attrib.m.reflect
                    || camera_front && sub_mesh->attrib.m.reflect_cam_back)
                    continue;

                if (disp_manager->object_culling && !instances_count && !bone_mat) {
                    int32_t v47 = obj_bounding_sphere_check_visibility(
                        sub_mesh->bounding_sphere, _mat);
                    if (v47 != 2 || (!mesh->attrib.m.billboard && !mesh->attrib.m.billboard_y_axis
                        && !mesh->attrib.m.disable_aabb_culling)) {
                        if (v47 == 2)
                            v47 = obj_axis_aligned_bounding_box_check_visibility(
                                sub_mesh->axis_aligned_bounding_box, _mat);

                        if (!v47) {
                            if (!mesh_morph || j >= mesh_morph->num_submesh) {
                                disp_manager->culling.culled.sub_meshes++;
                                continue;
                            }

                            sub_mesh_morph = &mesh_morph->submesh_array[j];
                            if (!sub_mesh_morph) {
                                disp_manager->culling.culled.sub_meshes++;
                                continue;
                            }

                            int32_t v56 = obj_bounding_sphere_check_visibility(
                                sub_mesh_morph->bounding_sphere, _mat);
                            if (v56 == 2)
                                v56 = obj_axis_aligned_bounding_box_check_visibility(
                                    sub_mesh_morph->axis_aligned_bounding_box, _mat);

                            if (!v56) {
                                disp_manager->culling.culled.sub_meshes++;
                                continue;
                            }
                        }
                    }
                }
                disp_manager->culling.passed.sub_meshes++;

                int32_t num_bone_index = sub_mesh->num_bone_index;
                const uint16_t* bone_index = sub_mesh->bone_index_array;

                mat4* mats;
                if (num_bone_index && enable_bone_mat) {
                    mats = alloc_mat4_array(num_bone_index);
                    if (bone_mat)
                        for (int32_t k = 0; k < num_bone_index; k++, bone_index++)
                            mats[k] = bone_mat[*bone_index];
                    else
                        for (int32_t k = 0; k < num_bone_index; k++)
                            mats[k] = mat4_identity;
                }
                else {
                    mats = 0;
                    num_bone_index = 0;
                }

                static bool (*sub_1402C1D50)() = (bool (*)())0x00000001402C1D50;
                static bool (*sub_1402C1D60)() = (bool (*)())0x00000001402C1D60;
                static bool (*sub_1402C1CF0)() = (bool (*)())0x00000001402C1CF0;
                static bool (*sub_1402C1D90)() = (bool (*)())0x00000001402C1D90;
                obj_material_data* material = &obj->material_array[sub_mesh->material_index];
                if (sub_1402C1CF0() && (material->material.adjust_param.m.disp
                    || (material->material.adjust_param.m.permit_disp && !sub_1402C1D90())))
                    continue;

                ObjData* data = alloc_obj_data(mdl::OBJ_KIND_NORMAL);
                if (!data)
                    continue;

                p_dx_buffer* morph_vertex_buffer = 0;
                uint32_t morph_vertex_buffer_offset = 0;
                if (obj_morph_vert_buf)
                    obj_morph_vert_buf[i].get_buffer(morph_vertex_buffer, morph_vertex_buffer_offset);

                p_dx_buffer* index_buffer = 0;
                if (obj_index_buf)
                    index_buffer = &obj_index_buf[i].buffer;;

                p_dx_buffer* vertex_buffer = 0;
                uint32_t vertex_buffer_offset = 0;
                if (obj_vert_buf)
                    obj_vert_buf[i].get_buffer(vertex_buffer, vertex_buffer_offset);

                if (disp_manager->texture_pattern_count > 0 && !texture_pattern_array) {
                    texture_pattern_array = (texture_pattern_struct*)mdl::DispManager::alloc_data(
                        sizeof(texture_pattern_struct) * disp_manager->texture_pattern_count);
                    if (texture_pattern_array)
                        for (int32_t i = 0; i < disp_manager->texture_pattern_count; i++)
                            texture_pattern_array[i] = disp_manager->texture_pattern_array[i];
                }

                if (disp_manager->texture_transform_count > 0 && !texture_transform_array) {
                    texture_transform_array = (texture_transform_struct*)mdl::DispManager::alloc_data(
                        sizeof(texture_transform_struct) * disp_manager->texture_transform_count);
                    if (texture_transform_array)
                        for (int32_t i = 0; i < disp_manager->texture_transform_count; i++)
                            texture_transform_array[i] = disp_manager->texture_transform_array[i];
                }

                const uint64_t material_hash = hash_utf8_fnv1a64m(material->material.name);

                const material_list_struct* mat_list = 0;
                for (int32_t k = 0; k < material_list_count; k++)
                    if (material_list_array[k].hash == material_hash) {
                        mat_list = &material_list_array[k];
                        break;
                    }

                vec4 _blend_color = 1.0f;
                vec4 _emission = { 0.0f, 0.0f, 0.0f, 1.0f };

                if (mat_list) {
                    bool has_blend_color = false;
                    if (mat_list->has_blend_color.x) {
                        _blend_color.x = mat_list->blend_color.x;
                        has_blend_color = true;
                    }

                    if (mat_list->has_blend_color.y) {
                        _blend_color.y = mat_list->blend_color.y;
                        has_blend_color = true;
                    }

                    if (mat_list->has_blend_color.z) {
                        _blend_color.z = mat_list->blend_color.z;
                        has_blend_color = true;
                    }

                    if (mat_list->has_blend_color.w) {
                        _blend_color.w = mat_list->blend_color.w;
                        has_blend_color = true;
                    }

                    if (blend_color) {
                        if (!has_blend_color)
                            _blend_color = *blend_color;
                        else
                            _blend_color *= *blend_color;
                    }

                    bool has_emission = false;
                    if (mat_list->has_emission.x) {
                        _emission.x = mat_list->emission.x;
                        has_emission = true;
                    }

                    if (mat_list->has_emission.y) {
                        _emission.y = mat_list->emission.y;
                        has_emission = true;
                    }

                    if (mat_list->has_emission.z) {
                        _emission.z = mat_list->emission.z;
                        has_emission = true;
                    }

                    if (mat_list->has_emission.w) {
                        _emission.w = mat_list->emission.w;
                        has_emission = true;
                    }

                    if (!has_emission)
                        _emission = material->material.color.emission;
                }
                else {
                    if (blend_color)
                        _blend_color = *blend_color;
                    _emission = material->material.color.emission;
                }

                data->init_sub_mesh(mat, obj->bounding_sphere.radius, sub_mesh, mesh, material,
                    textures, num_bone_index, mats, vertex_buffer, vertex_buffer_offset, index_buffer,
                    _blend_color, _emission, morph_vertex_buffer, morph_vertex_buffer_offset,
                    morph_value, disp_manager->texture_pattern_count, texture_pattern_array,
                    disp_manager->texture_transform_count, texture_transform_array, 0, 0, 0,
                    instances_count, instances_mat, func, 0);

                const ObjFlags obj_flags = disp_manager->obj_flags;
                const obj_material_attrib_member attrib = material->material.attrib.m;
                if (!attrib.flag_28 && (data->args.sub_mesh.ptr->blend_color.w < 1.0f
                    || !attrib.punch_through && attrib.alpha_texture | attrib.alpha_material
                    || sub_mesh->attrib.m.translucent)) {
                    if (!(obj_flags & mdl::OBJ_NO_TRANSLUCENCY)) {
                        if (!sub_1402C1CF0() || !material->material.adjust_param.m.force_opaque) {
                            if (sub_1402C1CF0() && material->material.adjust_param.m.before_punch
                                && (sub_1402C1D50() || sub_1402C1D60()))
                                continue;

                            if (!attrib.translucent_priority)
                                if (mesh->attrib.m.translucent_sort_by_radius
                                    || obj_flags & mdl::OBJ_TRANSLUCENT_SORT_BY_RADIUS) {
                                    entry_list(OBJ_TYPE_REFLECT_TRANSLUCENT_SORT_BY_RADIUS, data);
                                }
                                else
                                    entry_list(OBJ_TYPE_REFLECT_TRANSLUCENT, data);
                            else if (translucent_priority_count < 40)
                                disp_manager->translucent_objects[translucent_priority_count++] = data->args.sub_mesh.ptr;
                        }
                    }
                }
                else {
                    if (!(obj_flags & mdl::OBJ_NO_TRANSLUCENCY))
                        if (attrib.punch_through)
                            entry_list(OBJ_TYPE_REFLECT_TRANSPARENT, data);
                        else
                            entry_list(OBJ_TYPE_REFLECT_OPAQUE, data);
                    continue;
                }
            }

            if (translucent_priority_count <= 0)
                continue;

            ObjTranslucentArgs translucent_args;
            translucent_args.count = 0;
            for (int32_t j = 62; j; j--)
                for (int32_t k = 0; k < translucent_priority_count; k++) {
                    ObjSubMeshArgs* sub_mesh = disp_manager->translucent_objects[k];
                    if (sub_mesh->material->material.attrib.m.translucent_priority != j)
                        continue;

                    translucent_args.sub_mesh[translucent_args.count] = sub_mesh;
                    translucent_args.count++;
                }

            ObjData* data = alloc_obj_data(mdl::OBJ_KIND_TRANSLUCENT);
            if (data) {
                data->init_translucent(mat, translucent_args);
                entry_list(OBJ_TYPE_TRANSLUCENT, data);
            }
        }
    }

    void DispManager::entry_obj_screen(const ::obj* obj, const mat4& mat, obj_mesh_vertex_buffer* obj_vert_buf,
        obj_mesh_index_buffer* obj_index_buf, const texture** textures, const vec4* blend_color) {
        disp_manager->culling.passed.objects++;

        texture_pattern_struct* texture_pattern_array = 0;
        texture_transform_struct* texture_transform_array = 0;

        static int32_t(FASTCALL * obj_bounding_sphere_check_visibility)(
            const obj_bounding_sphere & sphere, const mat4 mat) = (int32_t(FASTCALL*)(
                const obj_bounding_sphere & sphere, const mat4 mat))0x000000014045DFC0;

        static int32_t(FASTCALL * obj_axis_aligned_bounding_box_check_visibility)(
            const obj_axis_aligned_bounding_box * aabb, const mat4 mat) = (int32_t(FASTCALL*)(
                const obj_axis_aligned_bounding_box * aabb, const mat4 mat))0x000000014045E0B0;

        for (int32_t i = 0; i < obj->num_mesh; i++) {
            const obj_mesh* mesh = &obj->mesh_array[i];

            disp_manager->culling.passed.meshes++;

            int32_t translucent_priority_count = 0;

            for (int32_t j = 0; j < mesh->num_submesh; j++) {
                const obj_sub_mesh* sub_mesh = &mesh->submesh_array[j];
                if (sub_mesh->attrib.m.cloth)
                    continue;

                disp_manager->culling.passed.sub_meshes++;

                static bool (*sub_1402C1D50)() = (bool (*)())0x00000001402C1D50;
                static bool (*sub_1402C1D60)() = (bool (*)())0x00000001402C1D60;
                static bool (*sub_1402C1CF0)() = (bool (*)())0x00000001402C1CF0;
                static bool (*sub_1402C1D90)() = (bool (*)())0x00000001402C1D90;
                obj_material_data* material = &obj->material_array[sub_mesh->material_index];
                if (sub_1402C1CF0() && (material->material.adjust_param.m.disp
                    || (material->material.adjust_param.m.permit_disp && !sub_1402C1D90())))
                    continue;

                ObjData* data = alloc_obj_data(mdl::OBJ_KIND_NORMAL);
                if (!data)
                    continue;

                p_dx_buffer* index_buffer = 0;
                if (obj_index_buf)
                    index_buffer = &obj_index_buf[i].buffer;;

                p_dx_buffer* vertex_buffer = 0;
                uint32_t vertex_buffer_offset = 0;
                if (obj_vert_buf)
                    obj_vert_buf[i].get_buffer(vertex_buffer, vertex_buffer_offset);

                if (disp_manager->texture_pattern_count > 0 && !texture_pattern_array) {
                    texture_pattern_array = (texture_pattern_struct*)mdl::DispManager::alloc_data(
                        sizeof(texture_pattern_struct) * disp_manager->texture_pattern_count);
                    if (texture_pattern_array)
                        for (int32_t i = 0; i < disp_manager->texture_pattern_count; i++)
                            texture_pattern_array[i] = disp_manager->texture_pattern_array[i];
                }

                if (disp_manager->texture_transform_count > 0 && !texture_transform_array) {
                    texture_transform_array = (texture_transform_struct*)mdl::DispManager::alloc_data(
                        sizeof(texture_transform_struct) * disp_manager->texture_transform_count);
                    if (texture_transform_array)
                        for (int32_t i = 0; i < disp_manager->texture_transform_count; i++)
                            texture_transform_array[i] = disp_manager->texture_transform_array[i];
                }

                vec4 _blend_color = 1.0f;
                if (blend_color)
                    _blend_color = *blend_color;
                vec4 _emission = material->material.color.emission;

                data->init_sub_mesh(mat, obj->bounding_sphere.radius, sub_mesh, mesh, material,
                    textures, 0, 0, vertex_buffer, vertex_buffer_offset, index_buffer,
                    _blend_color, _emission, 0, 0, 0.0f, disp_manager->texture_pattern_count, texture_pattern_array,
                    disp_manager->texture_transform_count, texture_transform_array, 0, 0, 0, 0, 0, 0, 0);

                const ObjFlags obj_flags = disp_manager->obj_flags;
                if (obj_flags & mdl::OBJ_SHADOW_OBJECT) {
                    entry_list((ObjType)(OBJ_TYPE_SHADOW_OBJECT_CHARA
                        + disp_manager->shadow_type), data);

                    if (obj_flags & mdl::OBJ_USER)
                        entry_list(OBJ_TYPE_USER, data);
                    continue;
                }

                const obj_material_attrib_member attrib = material->material.attrib.m;
                if ((obj_flags & mdl::OBJ_ALPHA_ORDER) && data->args.sub_mesh.ptr->blend_color.w < 1.0f) {
                    if (!(obj_flags & mdl::OBJ_NO_TRANSLUCENCY)) {
                        bool translucent = false;
                        if (!attrib.flag_28 && (!attrib.punch_through
                            && (attrib.alpha_texture || attrib.alpha_material)
                            || sub_mesh->attrib.m.translucent)) {
                            if (!sub_1402C1CF0() || !material->material.adjust_param.m.force_opaque)
                                translucent = true;
                        }

                        if (translucent) {
                            if (!attrib.translucent_priority) {
                                if (obj_flags & mdl::OBJ_ALPHA_ORDER_POST_GLITTER);
                                else if (obj_flags & mdl::OBJ_ALPHA_ORDER_POST_TRANSLUCENT)
                                    entry_list(OBJ_TYPE_SCREEN_TRANSLUCENT_ALPHA_ORDER_POST_TRANSLUCENT, data);
                                else;
                            }
                            else if (translucent_priority_count < 40)
                                disp_manager->translucent_objects[translucent_priority_count++] = data->args.sub_mesh.ptr;
                        }
                        else {
                            if (attrib.punch_through) {
                                if (obj_flags & mdl::OBJ_ALPHA_ORDER_POST_GLITTER);
                                else if (obj_flags & mdl::OBJ_ALPHA_ORDER_POST_TRANSLUCENT)
                                    entry_list(OBJ_TYPE_SCREEN_TRANSPARENT_ALPHA_ORDER_POST_TRANSLUCENT, data);
                                else;
                            }
                            else {
                                if (obj_flags & mdl::OBJ_ALPHA_ORDER_POST_GLITTER);
                                else if (obj_flags & mdl::OBJ_ALPHA_ORDER_POST_TRANSLUCENT)
                                    entry_list(OBJ_TYPE_SCREEN_OPAQUE_ALPHA_ORDER_POST_TRANSLUCENT, data);
                                else;
                            }
                        }
                    }
                }
                else if (!attrib.flag_28 && (data->args.sub_mesh.ptr->blend_color.w < 1.0f
                    || !attrib.punch_through && attrib.alpha_texture | attrib.alpha_material
                    || sub_mesh->attrib.m.translucent)) {
                    if (!(obj_flags & mdl::OBJ_NO_TRANSLUCENCY)) {
                        if (!sub_1402C1CF0() || !material->material.adjust_param.m.force_opaque) {
                            if (sub_1402C1CF0() && material->material.adjust_param.m.before_punch
                                && (sub_1402C1D50() || sub_1402C1D60()))
                                continue;

                            if (!attrib.translucent_priority)
                                if (mesh->attrib.m.translucent_sort_by_radius
                                    || obj_flags & mdl::OBJ_TRANSLUCENT_SORT_BY_RADIUS);
                                else
                                    entry_list(OBJ_TYPE_SCREEN_TRANSLUCENT, data);
                            else if (translucent_priority_count < 40)
                                disp_manager->translucent_objects[translucent_priority_count++] = data->args.sub_mesh.ptr;
                        }
                    }
                }
                else {
                    if (attrib.punch_through) {
                        if (!(obj_flags & mdl::OBJ_NO_TRANSLUCENCY))
                            entry_list(OBJ_TYPE_SCREEN_TRANSPARENT, data);
                    }
                    else {
                        if (!(obj_flags & mdl::OBJ_NO_TRANSLUCENCY))
                            entry_list(OBJ_TYPE_SCREEN_OPAQUE, data);
                    }
                    continue;
                }
            }

            if (translucent_priority_count <= 0)
                continue;

            ObjTranslucentArgs translucent_args;
            translucent_args.count = 0;
            for (int32_t j = 62; j; j--)
                for (int32_t k = 0; k < translucent_priority_count; k++) {
                    ObjSubMeshArgs* sub_mesh = disp_manager->translucent_objects[k];
                    if (sub_mesh->material->material.attrib.m.translucent_priority != j)
                        continue;

                    translucent_args.sub_mesh[translucent_args.count] = sub_mesh;
                    translucent_args.count++;
                }

            ObjData* data = alloc_obj_data(mdl::OBJ_KIND_TRANSLUCENT);
            if (data) {
                data->init_translucent(mat, translucent_args);
                if (disp_manager->obj_flags & mdl::OBJ_ALPHA_ORDER_POST_GLITTER);
                else if (disp_manager->obj_flags & mdl::OBJ_ALPHA_ORDER_POST_TRANSLUCENT)
                    entry_list(OBJ_TYPE_SCREEN_TRANSLUCENT_ALPHA_ORDER_POST_TRANSLUCENT, data);
                else if (disp_manager->obj_flags & mdl::OBJ_ALPHA_ORDER_POST_OPAQUE);
                else
                    entry_list(OBJ_TYPE_SCREEN_TRANSLUCENT, data);
            }
        }
    }

    static int32_t(FASTCALL* obj_check_bounding_sphere)(const ::obj* obj, const mat4* bone_mat, const mat4* mat)
        = (int32_t(FASTCALL*)(const ::obj * obj, const mat4 * bone_mat, const mat4 * mat))0x000000014045DFA0;

    void DispManager::entry_obj_by_obj(const ::obj* obj, const texture** textures,
        obj_mesh_vertex_buffer* obj_vert_buf, obj_mesh_index_buffer* obj_index_buf,
        const mat4* bone_mat, float_t alpha) {
        if (!obj)
            return;

        const vec4 blend_color(1.0f, 1.0f, 1.0f, alpha);
        mat4 mat;
        matrix_stack_data_get_matrix(0, mat);

        if (!disp_manager->object_culling || (mdl::obj_reflect_enable
            || obj_check_bounding_sphere(obj, 0, &mat)))
            entry_obj(obj, mat, obj_vert_buf, obj_index_buf, textures,
                alpha < 1.0f ? &blend_color : 0, bone_mat, 0, 0, 0, 0, 0, 0, !!bone_mat);
        else
            disp_manager->culling.culled.objects++;
    }

    void DispManager::entry_obj_by_object_info(object_info obj_info, const vec4* blend_color,
        const mat4* bone_mat, int32_t instances_count, const mat4* instances_mat,
        draw_func func, const ObjSubMeshArgs* func_data, bool enable_bone_mat) {
        mat4 mat;
        matrix_stack_data_get_matrix(0, mat);

        object_info_cache obj_info_cache;
        if (!obj_info_cache.get(obj_info))
            return;

        ::obj* obj = obj_info_cache.get_obj();
        if (!obj)
            return;

        if (!disp_manager->object_culling || (mdl::obj_reflect_enable
            || obj_check_bounding_sphere(obj, bone_mat, &mat)))
            entry_obj_by_object_info_cache(obj_info_cache, mat, blend_color,
                bone_mat, instances_count, instances_mat, func, func_data, enable_bone_mat);
        else
            disp_manager->culling.culled.objects++;
    }

    void DispManager::entry_obj_by_object_info_screen(object_info obj_info, const vec4* blend_color,
        const mat4* bone_mat, int32_t instances_count, const mat4* instances_mat,
        draw_func func, const ObjSubMeshArgs* func_data, bool enable_bone_mat) {
        mat4 mat;
        matrix_stack_data_get_matrix(0, mat);

        object_info_cache obj_info_cache;
        if (!obj_info_cache.get(obj_info))
            return;

        ::obj* obj = obj_info_cache.get_obj();
        if (!obj)
            return;

        entry_obj_by_object_info_cache_screen(obj_info_cache, mat, blend_color);
    }

    void DispManager::entry_obj_by_object_info_cache(object_info_cache& obj_info_cache, const mat4& mat,
        const vec4* blend_color, const mat4* bone_mat, int32_t instances_count, const mat4* instances_mat,
        draw_func func, const ObjSubMeshArgs* func_data, bool enable_bone_mat) {
        ::obj* obj = obj_info_cache.get_obj();
        if (!obj)
            return;

        ::obj* obj_morph = 0;
        obj_mesh_vertex_buffer* obj_morph_vert_buf = 0;
        if (disp_manager->morph.object.not_null()) {
            object_info_cache morph_obj_info_cache;
            if (morph_obj_info_cache.get(disp_manager->morph.object)) {
                obj_morph = morph_obj_info_cache.get_obj();
                if (obj_morph)
                    obj_morph_vert_buf = morph_obj_info_cache.get_obj_mesh_vertex_buffers();
            }
        }

        const texture** textures = obj_info_cache.get_textures(disp_manager->field_C78);
        obj_mesh_index_buffer* obj_mesh_index_buffers = obj_info_cache.get_obj_mesh_index_buffers();
        obj_mesh_vertex_buffer* obj_mesh_vertex_buffers = obj_info_cache.get_obj_mesh_vertex_buffers();
        entry_obj(obj, mat, obj_mesh_vertex_buffers, obj_mesh_index_buffers, textures,
            blend_color, bone_mat, obj_morph, obj_morph_vert_buf, disp_manager->morph.weight,
            instances_count, instances_mat, 0, 0, func, func_data, enable_bone_mat);
    }

    void DispManager::entry_obj_by_object_info_cache_screen(object_info_cache& obj_info_cache,
        const mat4& mat, const vec4* blend_color) {
        ::obj* obj = obj_info_cache.get_obj();
        if (!obj)
            return;

        const texture** textures = obj_info_cache.get_textures(disp_manager->field_C78);
        obj_mesh_index_buffer* obj_mesh_index_buffers = obj_info_cache.get_obj_mesh_index_buffers();
        obj_mesh_vertex_buffer* obj_mesh_vertex_buffers = obj_info_cache.get_obj_mesh_vertex_buffers();
        entry_obj_screen(obj, mat, obj_mesh_vertex_buffers, obj_mesh_index_buffers, textures, blend_color);
    }

    void DispManager::entry_list(ObjType type, ObjData* data) {
        ObjList& list = disp_manager->get_obj_list(type);

        ObjData** list_data = 0;
        if (list.first) {
            int32_t count = list.last->count;
            if (count < 16) {
                list.last->count = count + 1;
                ObjListData* next = list.last;
                list.count++;
                list_data = &next->data[count];
            }
            else if (disp_manager->data_buff_count < disp_manager->data_buff_max_count) {
                ObjListData* next = &disp_manager->data_buff[disp_manager->data_buff_count++];
                if (next) {
                    next->count = 1;
                    next->next = 0;
                    list.last->next = next;
                    list.last = next;
                    list.count++;
                    list_data = &next->data[0];
                }
            }
        }
        else if (disp_manager->data_buff_count < disp_manager->data_buff_max_count) {
            ObjListData* next = &disp_manager->data_buff[disp_manager->data_buff_count++];
            if (next) {
                next->count = 1;
                next->next = 0;
                list.first = next;
                list.last = next;
                list.count++;
                list_data = &next->data[0];
            }
        }

        if (list_data)
            *list_data = data;
    }

    void DispManager::entry_list(ObjTypeScreen type, ObjData* data) {
        ObjList& list = disp_manager->get_obj_list(type);

        ObjData** list_data = 0;
        if (list.first) {
            int32_t count = list.last->count;
            if (count < 16) {
                list.last->count = count + 1;
                ObjListData* next = list.last;
                list.count++;
                list_data = &next->data[count];
            }
            else if (disp_manager->data_buff_count < disp_manager->data_buff_max_count) {
                ObjListData* next = &disp_manager->data_buff[disp_manager->data_buff_count++];
                if (next) {
                    next->count = 1;
                    next->next = 0;
                    list.last->next = next;
                    list.last = next;
                    list.count++;
                    list_data = &next->data[0];
                }
            }
        }
        else if (disp_manager->data_buff_count < disp_manager->data_buff_max_count) {
            ObjListData* next = &disp_manager->data_buff[disp_manager->data_buff_count++];
            if (next) {
                next->count = 1;
                next->next = 0;
                list.first = next;
                list.last = next;
                list.count++;
                list_data = &next->data[0];
            }
        }

        if (list_data)
            *list_data = data;
    }

    void DispManager::entry_list(ObjTypeReflect type, ObjData* data) {
        ObjList& list = disp_manager->get_obj_list(type);

        ObjData** list_data = 0;
        if (list.first) {
            int32_t count = list.last->count;
            if (count < 16) {
                list.last->count = count + 1;
                ObjListData* next = list.last;
                list.count++;
                list_data = &next->data[count];
            }
            else if (disp_manager->data_buff_count < disp_manager->data_buff_max_count) {
                ObjListData* next = &disp_manager->data_buff[disp_manager->data_buff_count++];
                if (next) {
                    next->count = 1;
                    next->next = 0;
                    list.last->next = next;
                    list.last = next;
                    list.count++;
                    list_data = &next->data[0];
                }
            }
        }
        else if (disp_manager->data_buff_count < disp_manager->data_buff_max_count) {
            ObjListData* next = &disp_manager->data_buff[disp_manager->data_buff_count++];
            if (next) {
                next->count = 1;
                next->next = 0;
                list.first = next;
                list.last = next;
                list.count++;
                list_data = &next->data[0];
            }
        }

        if (list_data)
            *list_data = data;
    }

    ObjListSort& DispManager::get_obj_data_list_sort(int32_t index, ObjType type) {
        return obj_data_list_sort[index].list[type];
    }
    
    ObjListSort& DispManager::get_obj_data_list_sort(int32_t index, ObjTypeScreen type) {
        return obj_list_sort_screen[index][type];
    }
    
    ObjListSort& DispManager::get_obj_data_list_sort(int32_t index, ObjTypeReflect type) {
        return obj_list_sort_reflect[index][type];
    }

    void DispManager::obj_sort(render_data_context& rend_data_ctx, ObjType type, int32_t compare_func, const cam_data& cam, bool a5) {
        static void (*draw_task_add_alpha_center_mat_center)(mdl::ObjListSort & obj_list_sort, const cam_data & cam)
            = (void (*)(mdl::ObjListSort & obj_list_sort, const cam_data & cam))0x000000014045D4B0;

        ObjListSort& list = get_obj_data_list_sort(rend_data_ctx.index, type);
        draw_task_add_alpha_center_mat_center(list, cam);

        if (a5 && type == OBJ_TYPE_TRANSLUCENT) {
            static void (*sub_14045DB30)(mdl::ObjListSort&, const vec4*)
                = (void (*)(mdl::ObjListSort&, const vec4*))0x000000014045DB30;
            sub_14045DB30(list, cam.view_mat);
        }

        switch (compare_func) {
        case 0: {
            static void (*sub_14045E7F0)(mdl::ObjDataSort*, ObjDataSort*, size_t)
                = (void (*)(mdl::ObjDataSort*, ObjDataSort*, size_t))0x000000014045E7F0;
            sub_14045E7F0(list.data, &list.data[list.count], list.count);
        } break;
        case 1: {
            static void (*sub_14045EB50)(mdl::ObjDataSort*, ObjDataSort*, size_t)
                = (void (*)(mdl::ObjDataSort*, ObjDataSort*, size_t))0x000000014045EB50;
            sub_14045EB50(list.data, &list.data[list.count], list.count);
        } break;
        case 2: {
            static void (*sub_14045EEC0)(mdl::ObjDataSort*, ObjDataSort*, size_t)
                = (void (*)(mdl::ObjDataSort*, ObjDataSort*, size_t))0x000000014045EEC0;
            sub_14045EEC0(list.data, &list.data[list.count], list.count);
        } break;
        }
    }

    void DispManager::obj_sort(render_data_context& rend_data_ctx, ObjTypeScreen type, int32_t compare_func, const cam_data& cam) {
        static void (*draw_task_add_alpha_center_mat_center)(mdl::ObjListSort & obj_list_sort, const cam_data & cam)
            = (void (*)(mdl::ObjListSort & obj_list_sort, const cam_data & cam))0x000000014045D4B0;

        ObjListSort& list = get_obj_data_list_sort(rend_data_ctx.index, type);
        draw_task_add_alpha_center_mat_center(list, cam);

        switch (compare_func) {
        case 0: {
            static void (*sub_14045E7F0)(mdl::ObjDataSort*, ObjDataSort*, size_t)
                = (void (*)(mdl::ObjDataSort*, ObjDataSort*, size_t))0x000000014045E7F0;
            sub_14045E7F0(list.data, &list.data[list.count], list.count);
        } break;
        case 1: {
            static void (*sub_14045EB50)(mdl::ObjDataSort*, ObjDataSort*, size_t)
                = (void (*)(mdl::ObjDataSort*, ObjDataSort*, size_t))0x000000014045EB50;
            sub_14045EB50(list.data, &list.data[list.count], list.count);
        } break;
        case 2: {
            static void (*sub_14045EEC0)(mdl::ObjDataSort*, ObjDataSort*, size_t)
                = (void (*)(mdl::ObjDataSort*, ObjDataSort*, size_t))0x000000014045EEC0;
            sub_14045EEC0(list.data, &list.data[list.count], list.count);
        } break;
        }
    }

    void DispManager::obj_sort(render_data_context& rend_data_ctx, ObjTypeReflect type, int32_t compare_func, const cam_data& cam) {
        static void (*draw_task_add_alpha_center_mat_center)(mdl::ObjListSort & obj_list_sort, const cam_data & cam)
            = (void (*)(mdl::ObjListSort & obj_list_sort, const cam_data & cam))0x000000014045D4B0;

        ObjListSort& list = get_obj_data_list_sort(rend_data_ctx.index, type);
        draw_task_add_alpha_center_mat_center(list, cam);

        switch (compare_func) {
        case 0: {
            static void (*sub_14045E7F0)(mdl::ObjDataSort*, ObjDataSort*, size_t)
                = (void (*)(mdl::ObjDataSort*, ObjDataSort*, size_t))0x000000014045E7F0;
            sub_14045E7F0(list.data, &list.data[list.count], list.count);
        } break;
        case 1: {
            static void (*sub_14045EB50)(mdl::ObjDataSort*, ObjDataSort*, size_t)
                = (void (*)(mdl::ObjDataSort*, ObjDataSort*, size_t))0x000000014045EB50;
            sub_14045EB50(list.data, &list.data[list.count], list.count);
        } break;
        case 2: {
            static void (*sub_14045EEC0)(mdl::ObjDataSort*, ObjDataSort*, size_t)
                = (void (*)(mdl::ObjDataSort*, ObjDataSort*, size_t))0x000000014045EEC0;
            sub_14045EEC0(list.data, &list.data[list.count], list.count);
        } break;
        }
    }

    p_dx_rasterizer_state* dx_default_states_get_rasterizer_state_swap(render_data_context& rend_data_ctx, dx_cull_mode cull_mode) {
        if (reflect_draw[rend_data_ctx.index])
            switch (cull_mode) {
            case DX_CULL_BACK:
                cull_mode = DX_CULL_FRONT;
                break;
            case DX_CULL_FRONT:
                cull_mode = DX_CULL_BACK;
                break;
            }
        return dx_default_states_get_rasterizer_state_depth_clip(cull_mode, !screen_draw[rend_data_ctx.index]);
    }

    HOOK(void, FASTCALL, ObjDataListSort__add_objects, 0x000000014045A170, ObjDataListSort* This, ObjList* obj_list) {
        int32_t total_count = 0;
        ObjListSort* list = This->list;
        for (int32_t i = 0; i < OBJ_TYPE_MAX; i++) {
            int32_t count = obj_list->count;
            list->data = 0;
            list->count = count;
            if (count > 0 && count + total_count <= 0x1000) {
                ObjDataSort* data_sort = &This->data[total_count];
                list->data = data_sort;
                for (mdl::ObjListData* j = obj_list->first; j; j = j->next)
                    for (int32_t k = 0; k < j->count; data_sort++) {
                        data_sort->data = j->data[k++];
                        data_sort->view_z = 0.0f;
                        data_sort->radius = 0.0f;
                        data_sort->abs_view_z = 0.0f;
                    }
                total_count += count;
            }
            list++;
            obj_list++;
        }

        const int32_t index = (int32_t)(This - obj_data_list_sort);
        obj_list = obj_screen;
        list = obj_list_sort_screen[index];
        for (int32_t i = 0; i < OBJ_TYPE_SCREEN_MAX; i++) {
            int32_t count = obj_list->count;
            list->data = 0;
            list->count = count;
            if (count > 0 && count + total_count <= 0x1000) {
                ObjDataSort* data_sort = &This->data[total_count];
                list->data = data_sort;
                for (mdl::ObjListData* j = obj_list->first; j; j = j->next)
                    for (int32_t k = 0; k < j->count; data_sort++) {
                        data_sort->data = j->data[k++];
                        data_sort->view_z = 0.0f;
                        data_sort->radius = 0.0f;
                        data_sort->abs_view_z = 0.0f;
                    }
                total_count += count;
            }
            list++;
            obj_list++;
        }

        obj_list = obj_reflect;
        list = obj_list_sort_reflect[index];
        for (int32_t i = 0; i < OBJ_TYPE_REFLECT_MAX; i++) {
            int32_t count = obj_list->count;
            list->data = 0;
            list->count = count;
            if (count > 0 && count + total_count <= 0x1000) {
                ObjDataSort* data_sort = &This->data[total_count];
                list->data = data_sort;
                for (mdl::ObjListData* j = obj_list->first; j; j = j->next)
                    for (int32_t k = 0; k < j->count; data_sort++) {
                        data_sort->data = j->data[k++];
                        data_sort->view_z = 0.0f;
                        data_sort->radius = 0.0f;
                        data_sort->abs_view_z = 0.0f;
                    }
                total_count += count;
            }
            list++;
            obj_list++;
        }
    }

    HOOK(void, FASTCALL, disp_manager_refresh, 0x000000014045A930) {
        originaldisp_manager_refresh();

        for (ObjList& i : obj_screen) {
            i.first = 0;
            i.last = 0;
            i.count = 0;
        }

        for (ObjList& i : obj_reflect) {
            i.first = 0;
            i.last = 0;
            i.count = 0;
        }
    }

    HOOK(bool, FASTCALL, object_info_cache__check_obj_bounding_sphere, 0x000000014045AEC0, object_info_cache* This, const mat4& mat) {
        ::obj* obj = This->get_obj();
        return !disp_manager->object_culling || obj && (mdl::obj_reflect_enable
            || obj_check_bounding_sphere(obj, 0, &mat));
    }

    HOOK(void, FASTCALL, DispManager__entry_obj_by_object_info_alpha, 0x000000014045AF10,
        object_info_cache& obj_info_cache, const mat4& mat, bool enable_alpha, float_t alpha) {
        const vec4 blend_color(1.0f, 1.0f, 1.0f, alpha);
        DispManager::entry_obj_by_object_info_cache(obj_info_cache, mat, enable_alpha ? &blend_color : 0);
    }

    HOOK(void, FASTCALL, DispManager__entry_obj_by_obj, 0x000000014045B230, ::obj* obj, const texture** textures,
        obj_mesh_vertex_buffer* obj_vert_buf, obj_mesh_index_buffer* obj_index_buf, mat4* bone_mat, float_t alpha) {
        DispManager::entry_obj_by_obj(obj, textures, obj_vert_buf, obj_index_buf, bone_mat, alpha);
    }

    HOOK(void, FASTCALL, DispManager__entry_obj_by_object_info, 0x000000014045C390, uint32_t obj_info,
        const vec4* blend_color, const mat4* mat, int32_t instances_count, const mat4* instances_mat,
        draw_func func, const ObjSubMeshArgs* func_data, bool enable_bone_mat) {
        DispManager::entry_obj_by_object_info(obj_info, blend_color, mat,
            instances_count, instances_mat, func, func_data, enable_bone_mat);
    }

    HOOK(void, FASTCALL, DispManager__entry_obj, 0x000000014045C580, ::obj* obj, mat4& mat,
        obj_mesh_vertex_buffer* obj_vert_buf, obj_mesh_index_buffer* obj_index_buf, const texture** textures,
        const vec4* blend_color, const mat4* bone_mat, ::obj* obj_morph,
        obj_mesh_vertex_buffer* obj_morph_vert_buf, float_t morph_value,
        int32_t instances_count, const mat4* instances_mat, __int64 a13, __int64 a14,
        draw_func func, const ObjSubMeshArgs* func_data, bool enable_bone_mat) {
        DispManager::entry_obj(obj, mat, obj_vert_buf, obj_index_buf, textures,
            blend_color, bone_mat, obj_morph, obj_morph_vert_buf, morph_value,
            instances_count, instances_mat, a13, a14, func, func_data, enable_bone_mat);
    }

    HOOK(void, FASTCALL, draw_object_material_set_default_mid, 0x0000000140461271);

    void disp_manager_patch() {
        // Use our returned Rasterizer State
        WRITE_NOP_5(0x0000000140461271);

        // Write own emission
        WRITE_MEMORY_STRING(0x0000000140461790, "\x90\x0F\x10\x83\x10\x01\x00\x00", 0x08);
        WRITE_MEMORY_STRING(0x0000000140462876, "\x41\x0F\x10\x87\x10\x01\x00\x00", 0x08);

        INSTALL_HOOK(ObjDataListSort__add_objects);
        INSTALL_HOOK(disp_manager_refresh);
        INSTALL_HOOK(object_info_cache__check_obj_bounding_sphere);
        INSTALL_HOOK(DispManager__entry_obj_by_object_info_alpha);
        INSTALL_HOOK(DispManager__entry_obj_by_obj);
        INSTALL_HOOK(DispManager__entry_obj_by_object_info);
        INSTALL_HOOK(DispManager__entry_obj);
        INSTALL_HOOK(draw_object_material_set_default_mid);
    }
}
