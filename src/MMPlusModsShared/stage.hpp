/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/image.hpp"

enum reflect_refract_resolution_mode {
    REFLECT_REFRACT_RESOLUTION_256x256 = 0,
    REFLECT_REFRACT_RESOLUTION_512x256,
    REFLECT_REFRACT_RESOLUTION_512x512,
    REFLECT_REFRACT_RESOLUTION_MAX,
};

enum stage_data_reflect_type {
    STAGE_DATA_REFLECT_DISABLE     = 0x00,
    STAGE_DATA_REFLECT_NORMAL      = 0x01,
    STAGE_DATA_REFLECT_REFLECT_MAP = 0x02,
};

struct stage_data_reflect {
    reflect_refract_resolution_mode mode;
    int32_t blur_num;
    ImgfBoxSampl blur_filter;
};

static_assert(sizeof(stage_data_reflect) == 0x0C, "\"stage_data_reflect\" struct should have a size of 0x0C");

