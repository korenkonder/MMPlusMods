/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../../KKdLib/default.hpp"
#include "../../KKdLib/vec.hpp"

enum fog_id {
    FOG_DEPTH  = 0x00,
    FOG_HEIGHT = 0x01,
    FOG_BUMP   = 0x02,
    FOG_MAX    = 0x03,
};

enum fog_type {
    FOG_NONE   = 0x00,
    FOG_LINEAR = 0x01,
    FOG_EXP    = 0x02,
    FOG_EXP2   = 0x03,
};
