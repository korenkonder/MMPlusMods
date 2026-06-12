/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "camera.hpp"
#include "resolution_mode.hpp"
#include <Helpers.h>

CameraInfo& camera_info = *(CameraInfo*)0x000000014CC2B590;

FUNCTION_PTR(void, FASTCALL, calc_camera_matrix, 0x00000001402FC3A0);
FUNCTION_PTR(void, FASTCALL, calc_frustum_plane_normal, 0x00000001402FC280, CameraData* data);
FUNCTION_PTR(vec3, FASTCALL, calc_screen_pos3d, 0x00000001402FBB90, const vec3* pos);
FUNCTION_PTR(float_t, FASTCALL, calc_screen_pos_r, 0x00000001402FBA00, vec3* sc_pos, const vec3* pos, float_t r, bool with_render_offset);
FUNCTION_PTR(bool, FASTCALL, check_camera_discontinuity_prev, 0x00000001402FC8A0);
FUNCTION_PTR(int32_t, FASTCALL, check_screen_pos_r, 0x00000001402FBCB0, const vec3* wpos, float_t wr);
FUNCTION_PTR(void, FASTCALL, ctrl_camera, 0x00000001402FB0F0);
FUNCTION_PTR(void, FASTCALL, dest_camera, 0x00000001402FB210);
FUNCTION_PTR(float_t, FASTCALL, get_camera_far_clip, 0x00000001402FB9F0);
FUNCTION_PTR(void, FASTCALL, get_camera_frustum_offset, 0x00000001402FB860, float_t* left_offset, float_t* right_offset, float_t* bottom_offset, float_t* top_offset);
FUNCTION_PTR(const vec3&, FASTCALL, get_camera_intr, 0x00000001402FB980);
FUNCTION_PTR(void, FASTCALL, get_camera_matrix, 0x00000001402FB940, mat4* cmat, mat4* pmat, mat4* vpmat);
FUNCTION_PTR(float_t, FASTCALL, get_camera_near_clip, 0x00000001402FB9E0);
FUNCTION_PTR(float_t, FASTCALL, get_camera_pers, 0x00000001402FB9D0);
FUNCTION_PTR(const vec3&, FASTCALL, get_camera_pos, 0x00000001402FB960);
FUNCTION_PTR(float_t, FASTCALL, get_camera_rot_y_deg, 0x00000001402FB9B0);
FUNCTION_PTR(float_t, FASTCALL, get_camera_rot_z, 0x00000001402FB9A0);
FUNCTION_PTR(void, FASTCALL, init_camera, 0x00000001402FAF60);
FUNCTION_PTR(void, FASTCALL, init_projection_matrix, 0x00000001402FB410);
FUNCTION_PTR(void, FASTCALL, project_screen_r, 0x00000001402FBD70, vec2* pos2d, const vec3* pos3d, float_t r, bool with_render_offset);
FUNCTION_PTR(void, FASTCALL, project_screen, 0x00000001402FBDD0, vec2* pos2d, const vec3* pos3d, bool with_render_offset);
FUNCTION_PTR(void, FASTCALL, set_camera_discontinuity2, 0x00000001402FC890);
FUNCTION_PTR(void, FASTCALL, set_camera_discontinuity, 0x00000001402FC880);
FUNCTION_PTR(void, FASTCALL, set_camera_frustum_offset, 0x00000001402FB830, float_t left_offset, float_t right_offset, float_t bottom_offset, float_t top_offset);
FUNCTION_PTR(void, FASTCALL, set_camera_intr, 0x00000001402FB780, const vec3* in_intr);
FUNCTION_PTR(void, FASTCALL, set_camera_near_clip, 0x00000001402FB7F0, float_t clip_near);
FUNCTION_PTR(void, FASTCALL, set_camera_pers, 0x00000001402FB7D0, float_t pers);
FUNCTION_PTR(void, FASTCALL, set_camera_pos, 0x00000001402FB760, const vec3* in_pos);
FUNCTION_PTR(void, FASTCALL, set_camera_rot_z, 0x00000001402FB7A0, float_t rot_z);
FUNCTION_PTR(void, FASTCALL, set_camera_up, 0x00000001402FB7B0, bool use_up, const vec3* in_up);
FUNCTION_PTR(void, FASTCALL, set_debug_camera_distance, 0x00000001402FB930, float_t distance);
FUNCTION_PTR(void, FASTCALL, set_debug_camera_intr, 0x00000001402FB8C0, const vec3* in_intr);
FUNCTION_PTR(void, FASTCALL, set_debug_camera_near, 0x00000001402FB900, float_t in_clip_near);
FUNCTION_PTR(void, FASTCALL, set_debug_camera_pers, 0x00000001402FB8F0, float_t pers);
FUNCTION_PTR(void, FASTCALL, set_debug_camera_pos, 0x00000001402FB8A0, const vec3* in_pos);
FUNCTION_PTR(void, FASTCALL, set_debug_camera_rot, 0x00000001402FB910, const vec3* in_rot);
FUNCTION_PTR(void, FASTCALL, set_debug_camera_rot_z, 0x00000001402FB8E0, float_t rot);
FUNCTION_PTR(void, FASTCALL, set_ignore_near_clip_camera, 0x00000001402FB820, bool ignore_clip_near);
FUNCTION_PTR(void, FASTCALL, set_ignore_pers_camera, 0x00000001402FB810, bool ignore_pers);
FUNCTION_PTR(const mat4&, FASTCALL, set_projection_matrix_2d, 0x00000001402FB740, bool is_pre2d);

// Missing
vec2 calc_screen_pos2d(const vec3* pos, bool with_render_offset) {
    vec2 sc_pos;
    mat4 cmat;
    vec3 cpos;
    mat4_transpose(&camera_info.data.cmat, &cmat);
    mat4_transform_point(&cmat, pos, &cpos);
    if (fabsf(cpos.z) >= 1.0e-10f) {
        float_t pers_div = 1.0f / cpos.z;
        sc_pos.x = cpos.x * camera_info.data.fv * pers_div;
        sc_pos.y = cpos.y * camera_info.data.fv * pers_div;
        const resolution_struct* res_wind_int = res_window_internal_get();
        sc_pos.x = (float_t)res_wind_int->width * 0.5f - sc_pos.x;
        sc_pos.y = (float_t)res_wind_int->height * 0.5f + sc_pos.y;
        if (with_render_offset) {
            const resolution_struct* res_wind = res_window_get();
            sc_pos.x += (float_t)res_wind_int->x_offset;
            sc_pos.y += (float_t)(res_wind->height - res_wind_int->y_offset - res_wind_int->height);
        }
    }
    else
        sc_pos = 0.0f;
    return sc_pos;
}

// Missing
bool check_camera_discontinuity() {
    return camera_info.data.discontinuity || camera_info.data.discontinuity2;
}

// Missing
int32_t check_screen_pos(const vec3* wpos) {
    return check_screen_pos_r(wpos, 0.0f);
}

// Missing
double_t get_camera_aspect() {
    return camera_info.data.aspect;
}

// Missing
float_t get_camera_fv_2d() {
    return camera_info.data.fv_2d;
}

// Missing
void get_camera_imatrix(mat4* imat) {
    if (imat)
        *imat = camera_info.data.imat;
}

// Missing
const vec3& get_camera_up() {
    return camera_info.data.up;
}

// Missing
bool get_ignore_near_clip() {
    return camera_info.data.ignore_near_clip;
}

// Missing
bool get_ignore_pers() {
    return camera_info.data.ignore_pers;
}

// Missing
void set_camera_aspect(double_t aspect) {
    camera_info.data.aspect = aspect;
}

// Missing
void set_camera_portrait(bool portrait) {
    camera_info.data.portrait = portrait;
}
