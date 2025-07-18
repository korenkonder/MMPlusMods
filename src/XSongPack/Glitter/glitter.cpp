/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "glitter.hpp"
#include "../wrap.hpp"

extern bool reflect_draw[4];
extern mat4 reflect_mat;

namespace Glitter {
    const float_t min_emission = 0.01f;

    void axis_angle_from_vectors(vec3& axis, float_t& angle, const vec3& vec1, const vec3& vec2) {
        axis = vec3::cross(vec1, vec2);
        angle = vec3::length(axis);

        if (angle >= 0.000001f)
            angle = asinf(clamp_def(angle, -1.0f, 1.0f));
        else {
            angle = 0.0f;
            axis = vec3::cross(vec3(0.0f, 1.0f, 0.0f), vec1);
            if (vec3::length(axis) < 0.000001f)
                axis = vec3::cross(vec3(0.0f, 0.0f, 1.0f), vec1);
        }

        if (vec3::dot(vec1, vec2) < 0.0f)
            angle = (float_t)M_PI - angle;
    }

    void Init() {
        if (!glt_particle_manager_x)
            glt_particle_manager_x = new (_operator_new(sizeof(GltParticleManagerX))) GltParticleManagerX;
    }

    void Free() {
        if (glt_particle_manager_x) {
            glt_particle_manager_x->~GltParticleManagerX();
            _operator_delete(glt_particle_manager_x);
            glt_particle_manager_x = 0;
        }
    }
}

void dx_glitter_struct::set_state(render_data_context& rend_data_ctx, cam_data& cam, const mat4& draw_mat,
    const vec4& emission, bool alpha_test, bool cull, bool reflect, bool local) {
    rend_data_ctx.state.set_vs_shader(shaders[alpha_test].vertex);
    rend_data_ctx.state.set_ps_shader(shaders[alpha_test].pixel);

    dx_glitter_struct::batch_data* batch_data = (dx_glitter_struct::batch_data*)rend_data_ctx.state.map(buffer);

    mat4 view_mat;
    view_mat.row0 = cam.view_mat[0];
    view_mat.row1 = cam.view_mat[1];
    view_mat.row2 = cam.view_mat[2];
    view_mat.row3 = { 0.0f, 0.0f, 0.0f, 1.0f };

    if (reflect_draw[rend_data_ctx.index]) {
        mat4_transpose(&view_mat, &view_mat);
        mat4_mul(&reflect_mat, &view_mat, &view_mat);
        mat4_transpose(&view_mat, &view_mat);
    }

    mat4 vp_mat;
    mat4_mul(&cam.proj_mat, &view_mat, &vp_mat);

    mat4 mat;
    mat4_mul(&vp_mat, &draw_mat, &mat);
    batch_data->g_mvp[0] = mat.row0;
    batch_data->g_mvp[1] = mat.row1;
    batch_data->g_mvp[2] = mat.row2;
    batch_data->g_mvp[3] = mat.row3;

    mat4_mul(&view_mat, &draw_mat, &mat);
    batch_data->g_wv[0] = mat.row0;
    batch_data->g_wv[1] = mat.row1;
    batch_data->g_wv[2] = mat.row2;

    const float_t min_distance = cam.min_distance;
    const float_t max_distance = cam.max_distance;
    batch_data->g_fz_proj_row2.x = 0.0f;
    batch_data->g_fz_proj_row2.y = 0.0f;
    batch_data->g_fz_proj_row2.z = max_distance / (min_distance - max_distance);
    batch_data->g_fz_proj_row2.w = min_distance / (min_distance - max_distance);
    batch_data->g_glitter_blend_color = 1.0f;
    batch_data->g_state_material_diffuse = 1.0f;
    batch_data->g_state_material_emission = emission;

    rend_data_ctx.state.unmap(buffer);

    rend_data_ctx.state.set_vs_constant_buffer(3, 1, &buffer);
    rend_data_ctx.state.set_ps_constant_buffer(3, 1, &buffer);
    rend_data_ctx.state.set_rasterizer_state(dx_default_states_get_rasterizer_state_depth_clip(
        cull ? (reflect ? DX_CULL_FRONT : DX_CULL_BACK) : DX_CULL_NONE, !local));
    rend_data_ctx.state.set_index_buffer(&index_buffer, 0, 0);
    rend_data_ctx.state.set_ps_sampler_state(0, 1, &sampler_state);
    rend_data_ctx.state.set_ps_sampler_state(1, 1, &sampler_state);
}
