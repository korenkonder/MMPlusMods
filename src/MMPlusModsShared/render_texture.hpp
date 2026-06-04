/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "texture.hpp"
#include "wrap.hpp"

class RenderTexture {
private:
    static int32_t& m_tex_count;

public:
    texture* m_txhd_color[4];
    texture* m_txhd_depth;
    int32_t m_tex_unit;
    int32_t m_level;
    p_dx_render_target* m_fb;

private:
    int32_t create_fbo(int32_t level);
    int32_t set_render_target(const p_dx_texture& color, int32_t level, const p_dx_texture& depth);
    int32_t set_render_target(p_dx_texture** color,
        int32_t count, int32_t level, const p_dx_texture& depth);

public:
    static void init();

    RenderTexture();
    virtual ~RenderTexture();

    int32_t create_texture(int32_t width, int32_t height, int32_t level,
        int32_t pixel_format, int32_t depth_format, bool generate_mips = false);
    int32_t attach_texture(texture* color_tex, int32_t level = 0, texture* depth_tex = 0);
    int32_t attach_texture(const p_dx_texture& color_tex, int32_t level, const p_dx_texture& depth_tex);
    int32_t attach_texture(p_dx_texture** color_tex,
        int32_t count, int32_t level, const p_dx_texture& depth_tex);
    void destroy();
    int32_t begin_render(p_dx_state& p_dx_st, int32_t level = 0, bool clear = false);
    void end_render(p_dx_state& p_dx_st);
    texture* get_texture();
    void bind_texture(p_dx_state& p_dx_st, uint32_t tex_unit);
    texture* get_depth_texture();

    void set_viewport(p_dx_state& p_dx_st); // Added

    // Added
    inline p_dx_render_target* get_fb(int32_t level = 0) {
        return &m_fb[level];
    }

    // Added
    inline p_dx_texture& get_texture_glid() {
        return m_txhd_color[0]->tex;
    };

    // Added
    inline p_dx_texture& get_depth_texture_glid() {
        return m_txhd_depth->tex;
    };

    // Added
    inline int32_t get_width() {
        return m_txhd_color[0]->width;
    };

    // Added
    inline int32_t get_height() {
        return m_txhd_color[0]->height;
    };
};

static_assert(sizeof(RenderTexture) == 0x40, "\"RenderTexture\" struct should have a size of 0x40");

// Inlined
inline texture* RenderTexture::get_texture() {
    return m_txhd_color[0];
}

// Inlined
inline texture* RenderTexture::get_depth_texture() {
    return m_txhd_depth;
}
