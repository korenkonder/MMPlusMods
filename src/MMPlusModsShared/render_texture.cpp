/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "render_texture.hpp"
#include "texture.hpp"
#include "types.hpp"
#include <Helpers.h>

int32_t& RenderTexture::m_tex_count = *(int32_t*)0x000000014114867C;

// 0x1404DAE00
int32_t RenderTexture::create_fbo(int32_t level) {
    const uint32_t count = m_level + 1;
    static const size_t count_size = align_val(sizeof(uint32_t), sizeof(intptr_t));
    size_t d = (size_t)_operator_new(count_size + sizeof(p_dx_render_target) * count);
    *(uint32_t*)d = count;
    m_fb = new ((p_dx_render_target*)(d + count_size)) p_dx_render_target[count];
    return m_fb ? 0 : -1;
}

// 0x1404DAE90
int32_t RenderTexture::set_render_target(const p_dx_texture& color, int32_t level, const p_dx_texture& depth) {
    if (level < 0 || level > m_level)
        return -1;

    std::pair<p_dx_texture, int32_t> color_tex(color, level);
    std::pair<p_dx_texture, int32_t> depth_tex(depth, level);
    m_fb[level].create(&color_tex, 1, &depth_tex);
    return 0;
}

// 0x1404DAF50
int32_t RenderTexture::set_render_target(p_dx_texture** color,
    int32_t count, int32_t level, const p_dx_texture& depth) {
    if (level < 0 || level > m_level)
        return -1;

    count = min_def(count, 4);

    std::pair<p_dx_texture, int32_t> depth_tex(depth, level);
    std::pair<p_dx_texture, int32_t> color_tex[4];
    for (int32_t i = 0; i < count; i++)
        color_tex[i] = { *color[i], level };
    m_fb[level].create(color_tex, count, &depth_tex);
    return 0;
}

// 0x1404DAB20
void RenderTexture::init() {
    m_tex_count = 0;
}

// 0x1404DAB30
RenderTexture::RenderTexture() : m_txhd_color(), m_txhd_depth(), m_tex_unit(), m_level(), m_fb() {

}

// 0x1404DABC0
RenderTexture::~RenderTexture() {
    destroy();
}

// 0x1404DABD0
int32_t RenderTexture::create_texture(int32_t width, int32_t height,
    int32_t level, int32_t pixel_format, int32_t depth_format, bool generate_mips) {
    if (level < 0)
        return -1;

    destroy();

    if (pixel_format) {
        m_txhd_color[0] = texture_load_tex_2d(0x23000000 | m_tex_count,
            pixel_format, width, height, level, 0, 0, generate_mips);
        if (!m_txhd_color[0])
            return -1;

        m_tex_count++;
    }
    else
        m_txhd_color[0] = 0;

    if (depth_format) {
        m_txhd_depth = texture_load_tex_2d(0x23000000 | m_tex_count,
            depth_format, width, height, 0, 0, 0, generate_mips);
        if (!m_txhd_depth)
            return -1;

        m_tex_count++;
    }
    else
        m_txhd_depth = 0;
    m_level = level;

    if (create_fbo(level) < 0)
        return -1;

    p_dx_texture empty_tex;
    for (int32_t i = 0; i <= level; i++)
        if (set_render_target(
            m_txhd_color[0] ? m_txhd_color[0]->tex : empty_tex, i,
            m_txhd_depth ? m_txhd_depth->tex : empty_tex) < 0)
            return -1;
    return 0;
}

// Missing
int32_t RenderTexture::attach_texture(texture* color_tex, int32_t level, texture* depth_tex) {
    m_level = level;

    int32_t error = 0;
    if (!m_fb)
        error = create_fbo(level);

    m_txhd_color[0] = color_tex;
    m_txhd_depth = depth_tex;

    p_dx_texture empty_tex;
    set_render_target(color_tex ? color_tex->tex : empty_tex,
        level, depth_tex ? depth_tex->tex : empty_tex);
    return error;
}

// 0x1404DAD50, 0x157B71220
int32_t RenderTexture::attach_texture(const p_dx_texture& color_tex, int32_t level, const p_dx_texture& depth_tex) {
    m_level = level;

    int32_t error = 0;
    if (!m_fb)
        error = create_fbo(level);

    set_render_target(color_tex, level, depth_tex);
    return error;
}

// 0x1404DADB0, 0x157B804D0
int32_t RenderTexture::attach_texture(p_dx_texture** color_tex,
    int32_t count, int32_t level, const p_dx_texture& depth_tex) {
    m_level = level;

    int32_t error = 0;
    if (!m_fb)
        error = create_fbo(level);

    set_render_target(color_tex, count, level, depth_tex);
    return error;
}

// 0x1404DB080
void RenderTexture::destroy() {
    if (m_txhd_depth) {
        texture_release(m_txhd_depth);
        m_txhd_depth = 0;
    }

    for (texture*& i : m_txhd_color)
        if (i) {
            texture_release(i);
            i = 0;
        }

    if (m_fb) {
        static const size_t count_size = align_val(sizeof(uint32_t), sizeof(intptr_t));
        size_t d = (size_t)m_fb - count_size;
        const uint32_t count = *(uint32_t*)d;
        for (uint32_t i = 0; i < count; i++)
            m_fb[i].~p_dx_render_target();
        _operator_delete((void*)d);
        m_fb = 0;
    }
    m_level = 0;
}

// 0x1404DB140
int32_t RenderTexture::begin_render(p_dx_state& p_dx_st, int32_t level, bool clear) {
    if (level < 0 || level > m_level)
        return -1;

    p_dx_st.set_render_target(&m_fb[level]);
    if (clear)
        p_dx_st.clear_render_target_view(0.0f, 0.0f, 0.0f, 0.0f);
    return 0;
}

// 0x1404DB170
void RenderTexture::end_render(p_dx_state& p_dx_st) {
    p_dx_render_target empty_rt;
    p_dx_st.set_render_target(&empty_rt);
}

// 0x1404DB1D0
void RenderTexture::bind_texture(p_dx_state& p_dx_st, uint32_t tex_unit) {
    m_tex_unit = tex_unit;
    p_dx_st.set_vs_textures(tex_unit, 1, &get_texture_glid());
    p_dx_st.set_ps_textures(tex_unit, 1, &get_texture_glid());
}

// Added
void RenderTexture::set_viewport(p_dx_state& p_dx_st) {
    p_dx_st.set_viewport(0, 0, get_width(), get_height());
}
