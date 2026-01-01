/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "reflect_full.hpp"
#include "../MMPlusModsShared/light_param/light.hpp"
#include "render_manager.hpp"

vec4 reflect_clip_plane = 0.0f;
bool reflect_draw[4] = { false };
mat4 reflect_mat = mat4_identity;
reflect_full_struct* reflect_full_ptr = 0;

reflect_full_struct::reflect_full_struct() : reflect_nonsss_tex() {

}

reflect_full_struct::~reflect_full_struct() {
    free();
}

void reflect_full_struct::free() {
    reflect_contour_texture.Free();
    reflect_nonsss_texture.Free();

    if (reflect_nonsss_tex) {
        texture_release(reflect_nonsss_tex);
        reflect_nonsss_tex = 0;
    }

    reflect_buffer_texture.Free();
    reflect_texture.Free();
}

void reflect_full_struct::init() {
    rndr::Render* render = render_manager.render;

    reflect_texture.Init(render->render_width[0], render->render_height[0], 0, 23, 27);
    reflect_buffer_texture.Init(render->render_width[0], render->render_height[0], 0, 23, 27);

    reflect_nonsss_tex = texture_load_tex_2d(0x25000000 | render_get()->texture_counter++,
        23, render->render_width[0], render->render_height[0], 0, 0, 0, false);

    p_dx_texture* color_texs[2];
    color_texs[0] = &reflect_texture.GetColorTex();
    color_texs[1] = &reflect_nonsss_tex->tex;
    reflect_nonsss_texture.SetColorDepthTextures(color_texs, 2, 0, reflect_texture.GetDepthTex());

    p_dx_texture empty_tex;
    reflect_contour_texture.SetColorDepthTexture(reflect_texture.GetColorTex(), 0, empty_tex);
}

void get_reflect_mat() {
    vec4 reflect_clip_plane;
    const light_set* set = &light_set_data[LIGHT_SET_MAIN];
    const light_data* light_reflect = &set->lights[LIGHT_REFLECT];
    if (light_reflect->type == LIGHT_SPOT) {
        const vec3 spot_direction = -light_reflect->spot_direction;
        const vec3 position = *(vec3*)&light_reflect->position;

        float_t length = vec3::length(spot_direction);
        if (length > 0.000001f)
            *(vec3*)&reflect_clip_plane = spot_direction * (1.0f / length);
        else
            *(vec3*)&reflect_clip_plane = { 0.0f, 1.0f, 0.0f };

        reflect_clip_plane.w = -vec3::dot(position, *(vec3*)&reflect_clip_plane);
    }
    else if (light_reflect->type == LIGHT_POINT)
        reflect_clip_plane = { 0.0f, -1.0f, 0.0f, 9999.0f };
    else
        reflect_clip_plane = { 0.0f, -1.0f, 0.0f, 0.0f };

    const vec4 temp = 2.0f * reflect_clip_plane;

    reflect_mat = mat4_identity;
    *(vec3*)&reflect_mat.row0 -= temp.x * *(vec3*)&reflect_clip_plane;
    *(vec3*)&reflect_mat.row1 -= temp.y * *(vec3*)&reflect_clip_plane;
    *(vec3*)&reflect_mat.row2 -= temp.z * *(vec3*)&reflect_clip_plane;
    *(vec3*)&reflect_mat.row3 -= temp.w * *(vec3*)&reflect_clip_plane;
    reflect_mat.row0.w = 0.0f;
    reflect_mat.row1.w = 0.0f;
    reflect_mat.row2.w = 0.0f;
    reflect_mat.row3.w = 1.0f;
}
