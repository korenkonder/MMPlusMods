/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "shared.hpp"
#include "texture.hpp"
#include "wrap.hpp"

struct RenderTexture {
    texture* color_texture[4];
    texture* depth_texture;
    int32_t binding;
    int32_t max_level;
    p_dx_render_target* render_targets;

    RenderTexture();
    virtual ~RenderTexture();

    int32_t Bind(p_dx_state& p_dx_st, int32_t level = 0, bool clear = false);
    void Free();
    int32_t Init(uint32_t width, uint32_t height, int32_t max_level,
        int32_t color_texture_format, int32_t depth_texture_format, bool generate_mips = false);
    int32_t SetColorDepthTexture(p_dx_texture& color_texture,
        int32_t max_level, p_dx_texture& depth_texture);
    int32_t SetColorDepthTextures(p_dx_texture** color_textures, int32_t count,
        int32_t max_level, p_dx_texture& depth_texture);

    inline p_dx_texture& GetColorTex() {
        return color_texture[0]->tex;
    };

    inline p_dx_texture& GetDepthTex() {
        return depth_texture->tex;
    };

    inline int32_t GetHeight() {
        return color_texture[0]->tex.get_height();
    };

    inline int32_t GetWidth() {
        return color_texture[0]->tex.get_width();
    };

    inline void SetViewport(p_dx_state& p_dx_st) {
        p_dx_st.set_viewport(0, 0, GetWidth(), GetHeight());
    };
};

static_assert(sizeof(RenderTexture) == 0x40, "\"RenderTexture\" struct should have a size of 0x40");

extern void render_texture_counter_reset();
