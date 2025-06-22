/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "render_texture.hpp"
#include "texture.hpp"
#include "types.hpp"
#include <Helpers.h>

static uint32_t& render_texture_counter = *(uint32_t*)0x000000014114867C;

static int32_t render_texture_init_framebuffer(RenderTexture* rt, int32_t max_level);
static int32_t render_texture_set_framebuffer_texture(RenderTexture* rt,
    p_dx_texture& color_tex, int32_t level, p_dx_texture& depth_tex);
static int32_t render_texture_set_framebuffer_textures(RenderTexture* rt,
    p_dx_texture** color_texs, int32_t count, int32_t level, p_dx_texture& depth_tex);

RenderTexture::RenderTexture() : color_texture(), depth_texture(),
binding(), max_level(), render_targets() {

}

RenderTexture::~RenderTexture() {
    Free();
}

int32_t RenderTexture::Bind(p_dx_state& p_dx_st, int32_t level, bool clear) {
    if (level < 0 || level > max_level)
        return -1;
    p_dx_st.set_render_target(&render_targets[level]);
    if (clear)
        p_dx_st.clear_render_target_view(0.0f, 0.0f, 0.0f, 0.0f);
    return 0;
}

void RenderTexture::Free() {
    if (depth_texture) {
        texture_release(depth_texture);
        depth_texture = 0;
    }

    for (texture*& i : color_texture)
        if (i) {
            texture_release(i);
            i = 0;
        }

    if (render_targets) {
        static const size_t count_size = align_val(sizeof(uint32_t), sizeof(intptr_t));
        size_t d = (size_t)render_targets - count_size;
        const uint32_t count = *(uint32_t*)d;
        for (uint32_t i = 0; i < count; i++)
            render_targets[i].~p_dx_render_target();
        _operator_delete((void*)d);
        render_targets = 0;
    }
    max_level = 0;
}

int32_t RenderTexture::Init(uint32_t width, uint32_t height, int32_t max_level,
    int32_t color_texture_format, int32_t depth_texture_format, bool generate_mips) {
    if (max_level < 0)
        return -1;

    Free();

    if (color_texture_format > 0) {
        texture* color_texture = texture_load_tex_2d(0x23000000 | render_texture_counter,
            color_texture_format, width, height, max_level, 0, 0, generate_mips);
        this->color_texture[0] = color_texture;
        if (!color_texture)
            return -1;

        render_texture_counter++;
    }
    else
        color_texture[0] = 0;

    if (depth_texture_format > 0) {
        texture* depth_texture = texture_load_tex_2d(0x23000000 | render_texture_counter,
            depth_texture_format, width, height, 0, 0, 0, false);
        this->depth_texture = depth_texture;
        if (!depth_texture)
            return -1;

        render_texture_counter++;
    }
    else
        depth_texture = 0;
    this->max_level = max_level;

    if (render_texture_init_framebuffer(this, max_level) < 0)
        return -1;

    p_dx_texture empty_tex;
    for (int32_t i = 0; i <= max_level; i++)
        if (render_texture_set_framebuffer_texture(this,
            color_texture[0] ? color_texture[0]->tex : empty_tex,
            i, depth_texture ? depth_texture->tex : empty_tex) < 0)
            return -1;
    return 0;
}

int32_t RenderTexture::SetColorDepthTexture(p_dx_texture& color_texture,
    int32_t max_level, p_dx_texture& depth_texture) {
    int32_t error = 0;
    if (!render_targets)
        error = render_texture_init_framebuffer(this, 0);
    render_texture_set_framebuffer_texture(this, color_texture, max_level, depth_texture);
    return error;
}

int32_t RenderTexture::SetColorDepthTextures(p_dx_texture** color_textures, int32_t count,
    int32_t max_level, p_dx_texture& depth_texture) {
    int32_t error = 0;
    max_level = 0;
    if (!render_targets)
        error = render_texture_init_framebuffer(this, 0);
    render_texture_set_framebuffer_textures(this, color_textures, count, max_level, depth_texture);
    return error;
}

void render_texture_counter_reset() {
    render_texture_counter = 0;
}

static int32_t render_texture_init_framebuffer(RenderTexture* rt, int32_t max_level) {
    const uint32_t count = max_level + 1;
    static const size_t count_size = align_val(sizeof(uint32_t), sizeof(intptr_t));
    size_t d = (size_t)_operator_new(count_size + sizeof(p_dx_render_target) * count);
    *(uint32_t*)d = count;
    rt->render_targets = new ((p_dx_render_target*)(d + count_size)) p_dx_render_target[count];
    return rt->render_targets ? 0 : -1;
}

static int32_t render_texture_set_framebuffer_texture(RenderTexture* rt,
    p_dx_texture& color_tex, int32_t level, p_dx_texture& depth_tex) {
    if (level < 0 || rt->max_level < level)
        return -1;

    std::pair<p_dx_texture, int32_t> color{ color_tex, level };
    std::pair<p_dx_texture, int32_t> depth{ depth_tex, level };
    rt->render_targets[level].create(&color, 1, &depth);
    return 0;
}

static int32_t render_texture_set_framebuffer_textures(RenderTexture* rt,
    p_dx_texture** color_texs, int32_t count, int32_t level, p_dx_texture& depth_tex) {
    if (level < 0 || rt->max_level < level)
        return -1;

    std::pair<p_dx_texture, int32_t> color[4];
    count = min_def(count, 4);
    for (int32_t i = 0; i < count; i++)
        color[i] = { *color_texs[i], level };
    std::pair<p_dx_texture, int32_t> depth{ depth_tex, level };
    rt->render_targets[level].create(color, count, &depth);
    return 0;
}
