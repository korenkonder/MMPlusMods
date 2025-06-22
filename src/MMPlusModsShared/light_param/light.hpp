/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../../KKdLib/default.hpp"
#include "../../KKdLib/mat.hpp"
#include "../../KKdLib/vec.hpp"

enum light_id {
    LIGHT_CHARA       = 0x00,
    LIGHT_STAGE       = 0x01,
    LIGHT_SUN         = 0x02,
    LIGHT_REFLECT     = 0x03,
    LIGHT_SHADOW      = 0x04,
    LIGHT_CHARA_COLOR = 0x05,
    LIGHT_TONE_CURVE  = 0x06,
    LIGHT_PROJECTION  = 0x07,
    LIGHT_MAX         = 0x08,
};

enum light_set_id {
    LIGHT_SET_MAIN = 0x00,
    LIGHT_SET_MAX  = 0x01,
};

enum light_type {
    LIGHT_OFF      = 0x00,
    LIGHT_PARALLEL = 0x01,
    LIGHT_POINT    = 0x02,
    LIGHT_SPOT     = 0x03,
};

struct light_attenuation {
    float_t constant;
    float_t linear;
    float_t quadratic;
};

struct light_clip_plane {
    bool data[4];
};

struct light_tone_curve {
    float_t start_point;
    float_t end_point;
    float_t coefficient;
};

struct light_data {
    light_type type;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 position;
    vec3 spot_direction;
    float_t spot_exponent;
    float_t spot_cutoff;
    light_attenuation attenuation;
    vec4 ibl_color0;
    vec4 ibl_color1;
    vec4 ibl_direction;
    light_tone_curve tone_curve;
    light_clip_plane clip_plane;
};

static_assert(sizeof(light_data) == 0xA4, "\"light\" struct should have a size of 0xA4");

struct light_set {
    light_data lights[LIGHT_MAX];
    vec4 ambient_intensity;
    mat4 irradiance_r;
    mat4 irradiance_g;
    mat4 irradiance_b;
};

static_assert(sizeof(light_set) == 0x5F0, "\"light_set\" struct should have a size of 0x5F0");

extern light_set* light_set_data;
