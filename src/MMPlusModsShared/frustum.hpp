/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/vec.hpp"

struct frustum_corners {
    vec3 data[8];

    void get(struct cam_data& cam, float_t min_distance, float_t max_distance);
};

static_assert(sizeof(frustum_corners) == 0x60, "\"frustum_corners\" struct should have a size of 0x60");

struct frustum_planes {
    vec4 data[6];

    void get(const frustum_corners& corners);
};

static_assert(sizeof(frustum_planes) == 0x60, "\"frustum_planes\" struct should have a size of 0x60");

extern frustum_planes& frustum_data;
