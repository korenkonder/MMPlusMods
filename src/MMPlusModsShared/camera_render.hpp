/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/mat.hpp"
#include "../KKdLib/vec.hpp"
#include "frustum.hpp"

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
