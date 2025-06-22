/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/mat.hpp"
#include "../KKdLib/vec.hpp"

struct cam_data;

struct frustum_corners {
    vec3 data[8];

    void get(cam_data& cam, float_t min_distance, float_t max_distance);
};

static_assert(sizeof(frustum_corners) == 0x60, "\"frustum_corners\" struct should have a size of 0x60");

struct frustum_planes {
    vec4 data[6];

    void get(const frustum_corners& corners);
};

static_assert(sizeof(frustum_planes) == 0x60, "\"frustum_planes\" struct should have a size of 0x60");

struct cam_data {
    vec3 view_point;
    vec3 interest;
    vec3 up;
    float_t fov;
    float_t aspect;
    float_t min_distance;
    float_t max_distance;
    vec4 view_mat[3];
    mat4 proj_mat;
    mat4 view_proj_mat;
    vec2 persp_scale;
    vec2 persp_offset;
    frustum_planes frustum[2];

    inline cam_data() : fov(), aspect(), min_distance(), max_distance(), frustum() {
        static cam_data* (FASTCALL * cam_data__cam_data)(cam_data * This)
            = (cam_data * (FASTCALL*)(cam_data * This))0x00000001404CCBC0;
        cam_data__cam_data(this);
    }

    void get();
    void get(float_t fov);
};

static_assert(sizeof(cam_data) == 0x1B4, "\"cam_data\" struct should have a size of 0x1B4");

struct camera_struct {
    vec3 view_point;
    vec3 interest;
    float_t roll;
    float_t fov;
    float_t aet_fov;
    int32_t field_24;
    double_t aspect;
    float_t min_distance;
    float_t max_distance;
    vec4 projection_scale;
    bool use_up;
    char field_49;
    char field_4A;
    char field_4B;
    vec3 up;
    bool ignore_fov;
    bool ignore_min_dist;
    char field_5A;
    char field_5B;
    mat4 view;
    mat4 inv_view;
    mat4 projection;
    mat4 view_projection;
    mat4 view_projection_aet_2d;
    mat4 view_projection_aet_3d;
    float_t depth;
    float_t aet_depth;
    vec3 field_1E4;
    vec3 field_1F0;
    vec3 field_1FC;
    vec3 field_208;
    float_t distance;
    vec3 rotation;
    float_t fov_horizontal_rad;
    char field_228;
    bool fast_change;
    bool fast_change_hist0;
    bool fast_change_hist1;
    int32_t field_22C;
};

static_assert(sizeof(camera_struct) == 0x230, "\"camera_struct\" struct should have a size of 0x230");

extern frustum_planes& frustum_data;
extern camera_struct& camera_data;
