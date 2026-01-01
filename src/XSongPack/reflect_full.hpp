/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/mat.hpp"
#include "../KKdLib/vec.hpp"
#include "../MMPlusModsShared/render_texture.hpp"

struct reflect_full_struct {
    RenderTexture reflect_texture;
    RenderTexture reflect_buffer_texture;
    RenderTexture reflect_nonsss_texture;
    RenderTexture reflect_contour_texture;
    texture* reflect_nonsss_tex;

    reflect_full_struct();
    ~reflect_full_struct();

    void free();
    void init();
};

extern vec4 reflect_clip_plane;
extern bool reflect_draw[4];
extern mat4 reflect_mat;
extern reflect_full_struct* reflect_full_ptr;
