/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "camera_render.hpp"
#include <Helpers.h>

void cam_data::get() {
    static void (FASTCALL * cam_data__get)(cam_data * This, __int64, __int64, const vec2 & persp_offset)
        = (void (FASTCALL*)(cam_data * This, __int64, __int64, const vec2 & persp_offset))0x000000014041F200;
    cam_data__get(this, 0, 0, 0.0f);
}

void cam_data::get(float_t fov) {
    mat4 view_mat;
    static FUNCTION_PTR(void, FASTCALL, camera_data_get_view_matrix, 0x00000001402FB940, mat4 * mat);
    static FUNCTION_PTR(const vec3&, FASTCALL, get_camera_pos, 0x00000001402FB960);
    static FUNCTION_PTR(float_t, FASTCALL, camera_data_get_min_distance, 0x00000001402FB9E0);
    static FUNCTION_PTR(float_t, FASTCALL, camera_data_get_max_distance, 0x00000001402FB9F0);
    camera_data_get_view_matrix(&view_mat);
    const vec3& view_point = get_camera_pos();
    const float_t min_distance = camera_data_get_min_distance();
    const float_t max_distance = camera_data_get_max_distance();

    static FUNCTION_PTR(void, FASTCALL, cam_data__set_view_point, 0x00000001404CCD10, cam_data * This, const vec3 & value);
    static FUNCTION_PTR(void, FASTCALL, cam_data__set_interest, 0x00000001404CCD30, cam_data * This, const vec3 & value);
    static FUNCTION_PTR(void, FASTCALL, cam_data__set_up, 0x00000001404CCD20, cam_data * This, const vec3 & value);
    static FUNCTION_PTR(void, FASTCALL, cam_data__calc_view_mat, 0x00000001404CCDC0, cam_data * This);
    static FUNCTION_PTR(void, FASTCALL, cam_data__set_fov, 0x00000001404CCD70, cam_data * This, float_t value);
    static FUNCTION_PTR(void, FASTCALL, cam_data__set_aspect, 0x00000001404CCD50, cam_data * This, float_t value);
    static FUNCTION_PTR(void, FASTCALL, cam_data__set_min_distance, 0x00000001404CCDA0, cam_data * This, float_t value);
    static FUNCTION_PTR(void, FASTCALL, cam_data__set_max_distance, 0x00000001404CCDB0, cam_data * This, float_t value);
    static FUNCTION_PTR(void, FASTCALL, cam_data__calc_proj_mat_offset, 0x00000001404CCE30, cam_data * This, const vec2 & persp_scale, const vec2 & persp_offset);
    static FUNCTION_PTR(void, FASTCALL, cam_data__calc_view_proj_mat, 0x00000001404CCFB0, cam_data * This);
    static FUNCTION_PTR(void, FASTCALL, cam_data__calc_frustum, 0x00000001404CD1A0, cam_data * This, float_t min_distance);
    cam_data__set_view_point(this, view_point);
    cam_data__set_interest(this, view_point - *(vec3*)&view_mat.row2);
    cam_data__set_up(this, *(vec3*)&view_mat.row1);
    cam_data__calc_view_mat(this);
    cam_data__set_fov(this, fov * DEG_TO_RAD_FLOAT);
    cam_data__set_aspect(this, (float_t)(16.0 / 9.0));
    cam_data__set_min_distance(this, min_distance);
    cam_data__set_max_distance(this, max_distance);
    cam_data__calc_proj_mat_offset(this, 1.0f, 0.0f);
    cam_data__calc_view_proj_mat(this);
    cam_data__calc_frustum(this, min_distance);
}
