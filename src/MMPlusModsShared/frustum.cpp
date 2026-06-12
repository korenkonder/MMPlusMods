/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "frustum.hpp"

frustum_planes& frustum_data = *(frustum_planes*)0x000000014CC2B530;

void frustum_corners::get(cam_data& cam, float_t min_distance, float_t max_distance) {
    static void (FASTCALL * frustum_corners__get)(frustum_corners * This, cam_data & cam, float_t min_distance, float_t max_distance)
        = (void (FASTCALL*)(frustum_corners * This, cam_data & cam, float_t min_distance, float_t max_distance))0x00000001404CD2B0;
    frustum_corners__get(this, cam, min_distance, max_distance);
}

void frustum_planes::get(const frustum_corners& corners) {
    static void (FASTCALL * frustum_planes__get)(frustum_planes * This, const frustum_corners & corners)
        = (void (FASTCALL*)(frustum_planes * This, const frustum_corners & corners))0x00000001404CD760;
    frustum_planes__get(this, corners);
}
