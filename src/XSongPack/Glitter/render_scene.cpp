/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "glitter.hpp"
#include "../../MMPlusModsShared/camera.hpp"
#include "../../MMPlusModsShared/matrix_stack.hpp"
#include "../../MMPlusModsShared/wrap.hpp"
#include "../mdl/disp_manager.hpp"

namespace Glitter {
    RenderSceneX::RenderSceneX() : ctrl_quad(), ctrl_line(), ctrl_locus(),
        ctrl_mesh(), disp_quad(), disp_line(), disp_locus(), disp_mesh() {

    }

    RenderSceneX::~RenderSceneX() {
        for (RenderGroupX*& i : groups)
            delete i;
    }

    void RenderSceneX::Append(RenderGroupX* rend_group) {
        groups.push_back(rend_group);
    }

    void RenderSceneX::CalcDisp(render_data_context& rend_data_ctx) {
        disp_quad = 0;
        disp_line = 0;
        disp_locus = 0;

        for (RenderGroupX*& i : groups) {
            if (!i)
                continue;

            RenderGroupX* rend_group = i;
            if (rend_group->CannotDisp())
                continue;

            CalcDisp(rend_data_ctx, rend_group);
        }
    }

    void RenderSceneX::CalcDisp(render_data_context& rend_data_ctx, RenderGroupX* rend_group) {
        switch (rend_group->type) {
        case PARTICLE_QUAD:
            rend_group->disp = 0;
            CalcDispQuad(rend_data_ctx, rend_group);
            disp_quad += rend_group->disp;
            break;
        case PARTICLE_LINE:
            rend_group->disp = 0;
            CalcDispLine(rend_data_ctx, rend_group);
            disp_line += rend_group->disp;
            break;
        case PARTICLE_LOCUS:
            rend_group->disp = 0;
            CalcDispLocus(rend_data_ctx, rend_group);
            disp_locus += rend_group->disp;
            break;
        case PARTICLE_MESH:
            break;
        default:
            rend_group->disp = 0;
            break;
        }
    }

    void RenderSceneX::CalcDispLine(render_data_context& rend_data_ctx, RenderGroupX* rend_group) {
#if SHARED_GLITTER_BUFFER
        if (!rend_group->elements || rend_group->ctrl < 1)
            return;
#else
        if (!rend_group->elements || rend_group->GetDxBuffer().is_null() || rend_group->ctrl < 1)
            return;
#endif

        size_t count = 0;
        RenderElementX* elem = rend_group->elements;
        for (size_t i = rend_group->ctrl; i > 0; i--, elem++) {
            if (!elem->alive)
                continue;

            if (elem->locus_history) {
                size_t length = elem->locus_history->data.size();
                if (length > 1)
                    count += length;
            }
        }

        if (!count || count > rend_group->max_count)
            return;

        bool has_scale = false;
        vec3 scale = 0.0f;
        if (rend_group->flags & PARTICLE_EMITTER_LOCAL) {
            rend_group->GetEmitterScale(scale);
            if (!(has_scale |= fabsf(scale.x) > 0.000001f ? true : false))
                scale.x = 0.0f;
            if (!(has_scale |= fabsf(scale.y) > 0.000001f ? true : false))
                scale.y = 0.0f;
            if (!(has_scale |= fabsf(scale.z) > 0.000001f ? true : false))
                scale.z = 0.0f;
            mat4_normalize_rotation(&rend_group->mat, &rend_group->mat_draw);
        }
        else
            rend_group->mat_draw = mat4_identity;

        Buffer* buf = rend_group->buffer;
        if (!buf)
            return;

        elem = rend_group->elements;
        size_t disp = 0;
        rend_group->draw_list.clear();
        for (size_t i = rend_group->ctrl, index = 0; i > 0; elem++) {
            if (!elem->alive)
                continue;

            i--;
            LocusHistoryX* hist = elem->locus_history;
            if (!elem->disp || !hist || hist->data.size() < 2)
                continue;

            size_t j = 0;
            if (has_scale)
                for (LocusHistoryX::Data& hist_data : hist->data) {
                    vec3& pos = hist_data.translation;
                    buf->position = pos + (pos - elem->base_translation) * scale;
                    buf->uv[0] = 0.0f;
                    buf->uv[1] = 0.0f;
                    buf->color = hist_data.color;
                    j++;
                    buf++;
                }
            else
                for (LocusHistoryX::Data& hist_data : hist->data) {
                    buf->position = hist_data.translation;
                    buf->uv[0] = 0.0f;
                    buf->uv[1] = 0.0f;
                    buf->color = hist_data.color;
                    j++;
                    buf++;
                }

            if (j > 0) {
                disp += j;
                rend_group->draw_list.push_back({ (int32_t)index, (uint32_t)j });
                index += j;
            }
        }
        rend_group->disp = disp;

#if !SHARED_GLITTER_BUFFER
        rend_data_ctx.state.write_buffer(rend_group->GetDxBuffer(), rend_group->buffer,
            rend_group->max_count * sizeof(Buffer));
#endif
    }

    void RenderSceneX::CalcDispLocus(render_data_context& rend_data_ctx, RenderGroupX* rend_group) {
#if SHARED_GLITTER_BUFFER
        if (!rend_group->elements || rend_group->ctrl < 1)
            return;
#else
        if (!rend_group->elements || rend_group->GetDxBuffer().is_null() || rend_group->ctrl < 1)
            return;
#endif

        size_t count = 0;
        RenderElementX* elem = rend_group->elements;
        for (size_t i = rend_group->ctrl; i > 0; i--, elem++) {
            if (!elem->alive)
                continue;

            if (elem->locus_history) {
                size_t length = elem->locus_history->data.size();
                if (length > 1)
                    count += 2 * length;
            }
        }

        if (!count || count > rend_group->max_count)
            return;

        mat4 cam_view;
        mat4 cam_inv_view;
        mat4_transpose(&camera_data.view, &cam_view);
        mat4_transpose(&camera_data.inv_view, &cam_inv_view);

        vec3 x_vec = { 1.0f, 0.0f, 0.0f };
        if (rend_group->flags & PARTICLE_SCREEN) {
            mat4 mat;
            mat4_mul(&cam_inv_view, &rend_group->mat, &mat);
            mat4_mul(&cam_view, &mat, &mat);
            mat4_mul(&mat, &cam_inv_view, &rend_group->mat_draw);
        }
        else
            rend_group->mat_draw = mat4_identity;

        mat3 model_mat;
        bool has_scale = false;
        vec3 scale = 0.0f;
        if (rend_group->flags & PARTICLE_EMITTER_LOCAL) {
            mat4_get_scale(&rend_group->mat, &scale);
            if (rend_group->flags & PARTICLE_SCALE)
                x_vec.x = scale.x;
            scale -= 1.0f;
            if (!(has_scale |= fabsf(scale.x) > 0.000001f ? true : false))
                scale.x = 0.0f;
            if (!(has_scale |= fabsf(scale.y) > 0.000001f ? true : false))
                scale.y = 0.0f;
            if (!(has_scale |= fabsf(scale.z) > 0.000001f ? true : false))
                scale.z = 0.0f;

            mat4_to_mat3(&rend_group->mat, &model_mat);
            mat3_normalize_rotation(&model_mat, &model_mat);
        }
        else
            model_mat = mat3_identity;

        mat3 cam_inv_view_mat3;
        mat4_to_mat3(&cam_inv_view, &cam_inv_view_mat3);
        mat3_invert(&cam_inv_view_mat3, &cam_inv_view_mat3);
        mat3_transform_vector(&cam_inv_view_mat3, &x_vec, &x_vec);

        Buffer* buf = rend_group->buffer;
        if (!buf)
            return;

        elem = rend_group->elements;
        vec2 split_uv = rend_group->split_uv;
        Pivot pivot = rend_group->pivot;
        size_t disp = 0;
        rend_group->draw_list.clear();
        for (size_t i = rend_group->ctrl, index = 0; i > 0; elem++) {
            if (!elem->alive)
                continue;

            i--;
            LocusHistoryX* hist = elem->locus_history;
            if (!elem->disp || !hist || hist->data.size() < 2)
                continue;

            float_t uv_u = elem->uv.x + elem->uv_scroll.x;
            float_t uv_u_2nd = elem->uv.x + elem->uv_scroll.x + split_uv.x;
            float_t uv_v_2nd = elem->uv.y + elem->uv_scroll.y + split_uv.y;
            float_t uv_v_scale = split_uv.y / (float_t)(hist->data.size() - 1);

            uv_v_2nd = 1.0f - uv_v_2nd;

            size_t j = 0;
            if (has_scale)
                for (LocusHistoryX::Data& hist_data : hist->data) {
                    vec3 pos = hist_data.translation;
                    vec3 pos_diff = (pos - elem->base_translation) * scale;
                    mat3_transform_vector(&model_mat, &pos_diff, &pos_diff);
                    pos += pos_diff;

                    float_t v00;
                    float_t v01;
                    CalcDispLocusSetPivot(pivot,
                        hist_data.scale * elem->scale.x * elem->scale_all,
                        v00, v01);

                    buf[0].position = pos + x_vec * v00;
                    buf[0].uv[0].x = uv_u;
                    buf[0].uv[0].y = uv_v_2nd + (float_t)j * uv_v_scale;
                    buf[0].uv[1] = buf[0].uv[0];
                    buf[0].color = hist_data.color;

                    buf[1].position = pos + x_vec * v01;
                    buf[1].uv[0].x = uv_u_2nd;
                    buf[1].uv[0].y = uv_v_2nd + (float_t)j * uv_v_scale;
                    buf[1].uv[1] = buf[1].uv[0];
                    buf[1].color = hist_data.color;
                    j++;
                    buf += 2;
                }
            else
                for (LocusHistoryX::Data& hist_data : hist->data) {
                    vec3 pos = hist_data.translation;

                    float_t v00;
                    float_t v01;
                    CalcDispLocusSetPivot(pivot,
                        hist_data.scale * elem->scale.x * elem->scale_all,
                        v00, v01);

                    buf[0].position = pos + x_vec * v00;
                    buf[0].uv[0].x = uv_u;
                    buf[0].uv[0].y = uv_v_2nd + (float_t)j * uv_v_scale;
                    buf[0].uv[1] = buf[0].uv[0];
                    buf[0].color = hist_data.color;

                    buf[1].position = pos + x_vec * v01;
                    buf[1].uv[0].x = uv_u_2nd;
                    buf[1].uv[0].y = uv_v_2nd + (float_t)j * uv_v_scale;
                    buf[1].uv[1] = buf[1].uv[0];
                    buf[1].color = hist_data.color;
                    j++;
                    buf += 2;
                }

            if (j > 0) {
                disp += j;
                rend_group->draw_list.push_back({ (int32_t)index, (uint32_t)(j * 2) });
                index += j * 2;
            }
        }
        rend_group->disp = disp;

#if !SHARED_GLITTER_BUFFER
        rend_data_ctx.state.write_buffer(rend_group->GetDxBuffer(), rend_group->buffer,
            rend_group->max_count * sizeof(Buffer));
#endif
    }

    void RenderSceneX::CalcDispQuad(render_data_context& rend_data_ctx, RenderGroupX* rend_group) {
#if SHARED_GLITTER_BUFFER
        if (!rend_group->elements || rend_group->ctrl < 1)
            return;
#else
        if (!rend_group->elements || rend_group->GetDxBuffer().is_null() || rend_group->ctrl < 1)
            return;
#endif

        mat4 cam_view;
        mat4 cam_inv_view;
        mat4_transpose(&camera_data.view, &cam_view);
        mat4_transpose(&camera_data.inv_view, &cam_inv_view);

        mat4 model_mat;
        mat4 view_mat;
        mat4 inv_view_mat;

        if (rend_group->flags & PARTICLE_EMITTER_LOCAL) {
            if (rend_group->draw_type == DIRECTION_EMITTER_ROTATION
                || rend_group->draw_type == DIRECTION_PARTICLE_ROTATION)
                mat4_clear_rot(&rend_group->mat, &model_mat);
            else
                mat4_normalize_rotation(&rend_group->mat, &model_mat);
            mat4_mul(&model_mat, &cam_view, &view_mat);
            mat4_invert(&view_mat, &inv_view_mat);
        }
        else {
            model_mat = mat4_identity;
            view_mat = cam_view;
            inv_view_mat = cam_inv_view;
        }

        if (rend_group->flags & PARTICLE_SCREEN) {
            if (rend_group->flags & PARTICLE_EMITTER_LOCAL)
                mat4_mul(&inv_view_mat, &rend_group->mat, &inv_view_mat);
            mat4_mul(&view_mat, &inv_view_mat, &view_mat);
            mat4_invert(&view_mat, &inv_view_mat);
        }
        mat4_mul(&view_mat, &cam_inv_view, &rend_group->mat_draw);

        mat4 dir_mat;
        switch (rend_group->draw_type) {
        case DIRECTION_BILLBOARD:
            mat4_clear_trans(&model_mat, &dir_mat);
            mat4_mul(&dir_mat, &inv_view_mat, &dir_mat);
            mat4_clear_trans(&dir_mat, &dir_mat);
            break;
        case DIRECTION_EMITTER_DIRECTION:
        case DIRECTION_EMITTER_ROTATION:
        case DIRECTION_PARTICLE_ROTATION:
            dir_mat = rend_group->mat_rot;
            break;
        case DIRECTION_Y_AXIS:
            mat4_rotate_y((float_t)M_PI_2, &dir_mat);
            break;
        case DIRECTION_X_AXIS:
            mat4_rotate_x((float_t)-M_PI_2, &dir_mat);
            break;
        case DIRECTION_BILLBOARD_Y_AXIS:
            mat4_rotate_y(camera_data.rotation.y, &dir_mat);
            break;
        default:
            dir_mat = mat4_identity;
            break;
        }

        switch (rend_group->draw_type) {
        case DIRECTION_PREV_POSITION:
        case DIRECTION_EMIT_POSITION:
        case DIRECTION_PREV_POSITION_DUP:
            CalcDispQuadDirectionRotation(rend_data_ctx, rend_group, &model_mat);
            break;
        default:
            CalcDispQuadNormal(rend_data_ctx, rend_group, &model_mat, &dir_mat);
            break;
        }
    }

    void RenderSceneX::CalcDispQuadDirectionRotation(render_data_context& rend_data_ctx,
        RenderGroupX* rend_group, mat4* model_mat) {
        vec3 up_vec;
        mat4(*rotate_func)(RenderGroupX*, RenderElementX*, vec3*);
        if (rend_group->draw_type == DIRECTION_EMIT_POSITION) {
            up_vec = { 0.0f, 0.0f, 1.0f };
            rotate_func = RenderGroupX::RotateToEmitPosition;
        }
        else {
            up_vec = { 0.0f, 1.0f, 0.0f };
            rotate_func = RenderGroupX::RotateToPrevPosition;
        }

        bool use_scale = false;
        vec3 scale = 1.0f;
        if (rend_group->flags & PARTICLE_EMITTER_LOCAL
            && rend_group->GetEmitterScale(scale))
            use_scale = rend_group->flags & PARTICLE_SCALE ? true : false;

        vec3 x_vec_base = { 1.0f, 0.0f, 0.0f };
        vec3 y_vec_base = { 0.0f, 1.0f, 0.0f };

        if (use_scale) {
            x_vec_base.x *= scale.x;
            y_vec_base.y *= scale.y;
        }

        Buffer* buf = rend_group->buffer;
        if (!buf)
            return;

        RenderElementX* elem = rend_group->elements;
        vec2 split_uv = rend_group->split_uv;
        Pivot pivot = rend_group->pivot;
        size_t disp = 0;
        for (size_t i = rend_group->ctrl, j_max = 1024; i > 0; i -= j_max) {
            j_max = min_def(i, j_max);
            for (size_t j = j_max; j > 0; elem++) {
                if (!elem->alive)
                    continue;
                j--;

                if (!elem->disp)
                    continue;

                vec2 scale_particle = *(vec2*)&elem->scale * elem->scale_particle * elem->scale_all;
                if (fabsf(scale_particle.x) < 0.000001f || fabsf(scale_particle.y) < 0.000001f)
                    continue;

                vec3 pos = elem->translation;
                if (use_scale)
                    pos *= scale;

                float_t v00;
                float_t v01;
                float_t v10;
                float_t v11;
                CalcDispQuadSetPivot(pivot,
                    scale_particle.x, scale_particle.y,
                    v00, v01, v10, v11);

                vec2 pos_add[4];
                vec2 uv_add[4];
                pos_add[0].x = v00;
                pos_add[0].y = v11;
                uv_add[0].x = 0.0f;
                uv_add[0].y = 0.0f;
                pos_add[1].x = v00;
                pos_add[1].y = v10;
                uv_add[1].x = 0.0f;
                uv_add[1].y = split_uv.y;
                pos_add[2].x = v01;
                pos_add[2].y = v10;
                uv_add[2].x = split_uv.x;
                uv_add[2].y = split_uv.y;
                pos_add[3].x = v01;
                pos_add[3].y = v11;
                uv_add[3].x = split_uv.x;
                uv_add[3].y = 0.0f;

                vec3 x_vec = x_vec_base;
                vec3 y_vec = y_vec_base;

                if (fabsf(elem->rotation.z) > 0.000001f) {
                    mat3 ptc_rot;
                    mat3_rotate_z(elem->rot_z_sin, elem->rot_z_cos, &ptc_rot);
                    mat3_transform_vector(&ptc_rot, &x_vec, &x_vec);
                    mat3_transform_vector(&ptc_rot, &y_vec, &y_vec);
                }

                vec2 uv = elem->uv + elem->uv_scroll;

                mat4 mat = rotate_func(rend_group, elem, &up_vec);
                mat4_transform_vector(&mat, &x_vec, &x_vec);
                mat4_transform_vector(&mat, &y_vec, &y_vec);

                for (int32_t k = 0; k < 4; k++, buf++) {
                    vec3 x_vec_rot = pos_add[k].x * x_vec;
                    vec3 y_vec_rot = pos_add[k].y * y_vec;
                    buf->position = pos + (x_vec_rot + y_vec_rot);
                    buf->uv[0] = uv + uv_add[k];
                    buf->uv[1] = buf->uv[0];
                    buf->color = elem->color;
                }
                disp++;
            }
        }
        rend_group->disp = disp;

#if !SHARED_GLITTER_BUFFER
        rend_data_ctx.state.write_buffer(rend_group->GetDxBuffer(), rend_group->buffer,
            rend_group->max_count * sizeof(Buffer));
#endif
    }

    void RenderSceneX::CalcDispQuadNormal(render_data_context& rend_data_ctx,
        RenderGroupX* rend_group, mat4* model_mat, mat4* dir_mat) {
        mat4 inv_model_mat;
        mat4_invert(model_mat, &inv_model_mat);
        mat4_clear_trans(&inv_model_mat, &inv_model_mat);

        vec3 x_vec = { 1.0f, 0.0f, 0.0f };
        vec3 y_vec = { 0.0f, 1.0f, 0.0f };
        bool use_z_offset = false;
        vec3 dist_to_cam = 0.0f;
        mat4 z_offset_inv_mat = mat4_identity;
        if (fabsf(rend_group->z_offset) > 0.000001f) {
            use_z_offset = true;
            mat4_get_translation(model_mat, &dist_to_cam);
            dist_to_cam = camera_data.view_point - dist_to_cam;
            if (rend_group->flags & PARTICLE_EMITTER_LOCAL) {
                mat4_normalize_rotation(model_mat, &z_offset_inv_mat);
                mat4_invert(&z_offset_inv_mat, &z_offset_inv_mat);
            }
        }

        bool has_scale = false;
        bool emitter_local = false;
        vec3 scale = 0.0f;
        if (rend_group->flags & PARTICLE_EMITTER_LOCAL) {
            rend_group->GetEmitterScale(scale);
            if (rend_group->flags & PARTICLE_SCALE) {
                x_vec.x = scale.x;
                y_vec.y = scale.y;
            }
            if (!(has_scale |= fabsf(scale.x) > 0.000001f ? true : false))
                scale.x = 0.0f;
            if (!(has_scale |= fabsf(scale.y) > 0.000001f ? true : false))
                scale.y = 0.0f;
            if (!(has_scale |= fabsf(scale.z) > 0.000001f ? true : false))
                scale.z = 0.0f;
            emitter_local = true;
        }

        vec3 ext_anim_scale;
        float_t ext_scale = 0.0f;
        if (rend_group->GetExtAnimScale(&ext_anim_scale, &ext_scale)) {
            if (ext_scale <= 0.0f)
                ext_scale = 1.0f;

            ext_anim_scale += ext_scale;
            x_vec.x *= ext_anim_scale.x;
            y_vec.y *= ext_anim_scale.y;
        }

        if (rend_group->draw_type != DIRECTION_BILLBOARD) {
            mat4_transform_vector(dir_mat, &x_vec, &x_vec);
            mat4_transform_vector(dir_mat, &y_vec, &y_vec);
        }
        else
            mat4_mul(&inv_model_mat, dir_mat, &inv_model_mat);

        mat4_transform_vector(&inv_model_mat, &x_vec, &x_vec);
        mat4_transform_vector(&inv_model_mat, &y_vec, &y_vec);

        Buffer* buf = rend_group->buffer;
        if (!buf)
            return;

        RenderElementX* elem = rend_group->elements;
        vec2 split_uv = rend_group->split_uv;
        Pivot pivot = rend_group->pivot;
        float_t z_offset = rend_group->z_offset;
        size_t disp = 0;
        if (rend_group->draw_type == DIRECTION_PARTICLE_ROTATION)
            for (size_t i = rend_group->ctrl, j_max = 1024; i > 0; i -= j_max) {
                j_max = min_def(i, j_max);
                for (size_t j = j_max; j > 0; elem++) {
                    if (!elem->alive)
                        continue;
                    j--;

                    if (!elem->disp)
                        continue;

                    vec2 scale_particle = *(vec2*)&elem->scale * elem->scale_particle * elem->scale_all;
                    if (fabsf(scale_particle.x) < 0.000001f || fabsf(scale_particle.y) < 0.000001f)
                        continue;

                    vec3 pos = elem->translation;
                    if (has_scale)
                        pos *= scale;

                    float_t v00;
                    float_t v01;
                    float_t v11;
                    float_t v10;
                    CalcDispQuadSetPivot(pivot,
                        scale_particle.x, scale_particle.y,
                        v00, v01, v10, v11);

                    vec2 pos_add[4];
                    vec2 uv_add[4];
                    pos_add[0].x = v00;
                    pos_add[0].y = v11;
                    uv_add[0].x = 0.0f;
                    uv_add[0].y = 0.0f;
                    pos_add[1].x = v00;
                    pos_add[1].y = v10;
                    uv_add[1].x = 0.0f;
                    uv_add[1].y = split_uv.y;
                    pos_add[2].x = v01;
                    pos_add[2].y = v10;
                    uv_add[2].x = split_uv.x;
                    uv_add[2].y = split_uv.y;
                    pos_add[3].x = v01;
                    pos_add[3].y = v11;
                    uv_add[3].x = split_uv.x;
                    uv_add[3].y = 0.0f;

                    vec2 uv = elem->uv + elem->uv_scroll;

                    if (use_z_offset) {
                        vec3 z_offset_dir = vec3::normalize(dist_to_cam - pos);
                        if (emitter_local)
                            mat4_transform_vector(&z_offset_inv_mat, &z_offset_dir, &z_offset_dir);
                        pos += z_offset_dir * z_offset;
                    }

                    mat3 ptc_rot;
                    mat3_rotate_zyx(elem->rotation.x, elem->rotation.y, elem->rotation.z, &ptc_rot);
                    for (int32_t k = 0; k < 4; k++, buf++) {
                        vec3 xy_vec_rot;
                        xy_vec_rot.x = x_vec.x * pos_add[k].x;
                        xy_vec_rot.y = y_vec.y * pos_add[k].y;
                        xy_vec_rot.z = 0.0f;
                        mat3_transform_vector(&ptc_rot, &xy_vec_rot, &xy_vec_rot);
                        buf->position = pos + xy_vec_rot;
                        buf->uv[0] = uv + uv_add[k];
                        buf->uv[1] = buf->uv[0];
                        buf->color = elem->color;
                    }
                    disp++;
                }
            }
        else
            for (size_t i = rend_group->ctrl, j_max = 1024; i > 0; i -= j_max) {
                j_max = min_def(i, j_max);
                for (size_t j = j_max; j > 0; elem++) {
                    if (!elem->alive)
                        continue;
                    j--;

                    if (!elem->disp)
                        continue;

                    vec2 scale_particle = *(vec2*)&elem->scale * elem->scale_particle * elem->scale_all;
                    if (fabsf(scale_particle.x) < 0.000001f || fabsf(scale_particle.y) < 0.000001f)
                        continue;

                    vec3 pos = elem->translation;
                    if (has_scale)
                        pos *= scale;

                    float_t v00;
                    float_t v01;
                    float_t v11;
                    float_t v10;
                    CalcDispQuadSetPivot(pivot,
                        scale_particle.x, scale_particle.y,
                        v00, v01, v10, v11);

                    vec2 pos_add[4];
                    vec2 uv_add[4];
                    pos_add[0].x = v00;
                    pos_add[0].y = v11;
                    uv_add[0].x = 0.0f;
                    uv_add[0].y = 0.0f;
                    pos_add[1].x = v00;
                    pos_add[1].y = v10;
                    uv_add[1].x = 0.0f;
                    uv_add[1].y = split_uv.y;
                    pos_add[2].x = v01;
                    pos_add[2].y = v10;
                    uv_add[2].x = split_uv.x;
                    uv_add[2].y = split_uv.y;
                    pos_add[3].x = v01;
                    pos_add[3].y = v11;
                    uv_add[3].x = split_uv.x;
                    uv_add[3].y = 0.0f;

                    vec2 uv = elem->uv + elem->uv_scroll;

                    if (use_z_offset) {
                        vec3 z_offset_dir = vec3::normalize(dist_to_cam - pos);
                        if (emitter_local)
                            mat4_transform_vector(&z_offset_inv_mat, &z_offset_dir, &z_offset_dir);
                        pos += z_offset_dir * z_offset;
                    }

                    float_t rot_z_cos = elem->rot_z_cos;
                    float_t rot_z_sin = elem->rot_z_sin;
                    for (int32_t k = 0; k < 4; k++, buf++) {
                        vec3 x_vec_rot = x_vec * (rot_z_cos * pos_add[k].x - rot_z_sin * pos_add[k].y);
                        vec3 y_vec_rot = y_vec * (rot_z_sin * pos_add[k].x + rot_z_cos * pos_add[k].y);
                        buf->position = pos + (x_vec_rot + y_vec_rot);
                        buf->uv[0] = uv + uv_add[k];
                        buf->uv[1] = buf->uv[0];
                        buf->color = elem->color;
                    }
                    disp++;
                }
            }
        rend_group->disp = disp;

#if !SHARED_GLITTER_BUFFER
        rend_data_ctx.state.write_buffer(rend_group->GetDxBuffer(), rend_group->buffer,
            rend_group->max_count * sizeof(Buffer));
#endif
    }

    bool RenderSceneX::CanDisp(DispType disp_type, bool a3) {
        for (RenderGroupX*& i : groups)
            if (!i->CannotDisp() && i->disp_type == disp_type && (!a3 || !i->HasEnded()))
                return true;
        return false;
    }

    void RenderSceneX::CheckUseCamera() {
        for (RenderGroupX*& i : groups)
            if (i)
                i->CheckUseCamera();
    }

    void RenderSceneX::Ctrl(float_t delta_frame, bool copy_mats) {
        ctrl_quad = 0;
        ctrl_line = 0;
        ctrl_locus = 0;
        ctrl_mesh = 0;

        for (RenderGroupX*& i : groups) {
            if (!i)
                continue;

            switch (i->type) {
            case PARTICLE_QUAD:
                ctrl_quad += i->ctrl;
                break;
            case PARTICLE_LINE:
                ctrl_line += i->ctrl;
                break;
            case PARTICLE_LOCUS:
                ctrl_locus += i->ctrl;
                break;
            case PARTICLE_MESH:
                ctrl_mesh += i->ctrl;
                break;
            }

            i->Ctrl(delta_frame, copy_mats);
        }
    }

    void RenderSceneX::Disp(render_data_context& rend_data_ctx, cam_data& cam, DispType disp_type, bool reflect) {
        for (RenderGroupX* i : groups) {
            if (!i)
                continue;

            RenderGroupX* rend_group = i;
            if ((rend_group)->disp_type != disp_type || rend_group->CannotDisp())
                continue;

            Disp(rend_data_ctx, cam, rend_group, reflect);
        }
    }

    void RenderSceneX::Disp(render_data_context& rend_data_ctx, cam_data& cam, RenderGroupX* rend_group, bool reflect) {
        if (rend_group->disp < 1)
            return;

        switch (rend_group->type) {
        case PARTICLE_QUAD:
        case PARTICLE_LINE:
        case PARTICLE_LOCUS:
            break;
        case PARTICLE_MESH:
        default:
            return;
        }

        if (rend_group->GetDxBuffer().is_null())
            return;

        bool alpha_test = false;
        dx_blend src_blend;
        dx_blend dst_blend;
        switch (rend_group->blend_mode) {
        case PARTICLE_BLEND_ADD:
            src_blend = DX_BLEND_SRC_ALPHA;
            dst_blend = DX_BLEND_ONE;
            break;
        case PARTICLE_BLEND_SUBTRACT:
            src_blend = DX_BLEND_SRC_ALPHA;
            dst_blend = DX_BLEND_INV_SRC_ALPHA;
            break;
        case PARTICLE_BLEND_MULTIPLY:
            src_blend = DX_BLEND_ZERO;
            dst_blend = DX_BLEND_SRC_COLOR;
            break;
        default:
            src_blend = DX_BLEND_SRC_ALPHA;
            dst_blend = DX_BLEND_INV_SRC_ALPHA;
            alpha_test = rend_group->blend_mode == PARTICLE_BLEND_PUNCH_THROUGH;
            break;
        }

        if (rend_group->type != PARTICLE_LINE && rend_group->texture.not_null()) {
            rend_data_ctx.state.set_ps_textures(0, 1, &rend_group->texture);
            if (rend_group->mask_texture.not_null()) {
                rend_data_ctx.state.set_ps_textures(1, 1, &rend_group->mask_texture);

                rend_data_ctx.uniform->arr[U_TEXTURE_COUNT] = 2;
                switch (rend_group->mask_blend_mode) {
                default:
                    rend_data_ctx.uniform->arr[U_TEXTURE_BLEND] = 0;
                    break;
                case PARTICLE_BLEND_MULTIPLY:
                    rend_data_ctx.uniform->arr[U_TEXTURE_BLEND] = 1;
                    break;
                case PARTICLE_BLEND_ADD:
                    rend_data_ctx.uniform->arr[U_TEXTURE_BLEND] = 2;
                    break;
                }
            }
            else {
                rend_data_ctx.uniform->arr[U_TEXTURE_COUNT] = 1;
                rend_data_ctx.uniform->arr[U_TEXTURE_BLEND] = 0;
            }
        }
        else {
            rend_data_ctx.uniform->arr[U_TEXTURE_COUNT] = 0;
            rend_data_ctx.uniform->arr[U_TEXTURE_BLEND] = 0;
        }

        switch (rend_group->fog_type) {
        default:
            rend_data_ctx.uniform->arr[U_FOG_STAGE] = 0;
            break;
        case Glitter::FOG_DEPTH:
            rend_data_ctx.uniform->arr[U_FOG_STAGE] = 1;
            break;
        case Glitter::FOG_HEIGHT:
            rend_data_ctx.uniform->arr[U_FOG_STAGE] = 2;
            break;
        }

        dx_depth_test depth_test;
        bool blend_enable;
        dx_blend_write_mask write_mask;
        if (alpha_test) {
            rend_data_ctx.uniform->arr[U_ALPHA_TEST] = 1;
            rend_data_ctx.set_batch_alpha_threshold(0.5f);
            rend_data_ctx.uniform->arr[U_ALPHA_BLEND] = rend_group->disp_type ? 3 : 1;
            depth_test = DX_DEPTH_WRITE;
            blend_enable = false;
            write_mask = DX_BLEND_WRITE_MASK_RGBA;
        }
        else if (rend_group->disp_type) {
            rend_data_ctx.uniform->arr[U_ALPHA_TEST] = 0;
            rend_data_ctx.uniform->arr[U_ALPHA_BLEND] = 2;
            depth_test = DX_DEPTH_TEST;
            blend_enable = true;
            write_mask = DX_BLEND_WRITE_MASK_RGB;
        }
        else {
            rend_data_ctx.uniform->arr[U_ALPHA_TEST] = 0;
            rend_data_ctx.uniform->arr[U_ALPHA_BLEND] = 0;
            depth_test = DX_DEPTH_WRITE;
            blend_enable = false;
            write_mask = DX_BLEND_WRITE_MASK_RGBA;
        }

        if (rend_group->flags & PARTICLE_DEPTH_TEST)
            depth_test = DX_DEPTH_NONE;

        rend_data_ctx.state.set_depth_stencil_state(dx_default_states_get_depth_stencil_state(depth_test, DX_DEPTH_FUNC_GREATER_EQUAL));

        rend_data_ctx.uniform->arr[U_NORMAL] = 1;
        rend_data_ctx.uniform->arr[U_SPECULAR] = 1;
        rend_data_ctx.uniform->arr[U_ENV_MAP] = 1;
        rend_data_ctx.uniform->arr[U_SPECULAR_IBL] = 1;

        rend_data_ctx.state.set_blend_state(dx_default_states_get_blend_state(blend_enable,
            src_blend, dst_blend, src_blend, dst_blend, write_mask));

        if (dx_glitter_struct_data) {
            float_t emission = 1.0f;
            if (rend_group->flags & PARTICLE_EMISSION || rend_group->blend_mode == PARTICLE_BLEND_TYPICAL)
                emission = rend_group->emission;

            mat4 mat;
            mat4_transpose(&rend_group->mat_draw, &mat);
            dx_glitter_struct_data->set_state(rend_data_ctx, cam, mat, { emission, emission, emission, 1.0f },
                alpha_test, rend_group->draw_type == DIRECTION_BILLBOARD && !rend_group->use_culling,
                reflect, rend_group->disp_type == DISP_SCREEN);
        }

        p_dx_buffer* buffers[] = { &rend_group->GetDxBuffer() };
        int32_t strides[] = { sizeof(Buffer) };
#if SHARED_GLITTER_BUFFER
        int32_t offsets[] = { rend_group->buffer_offset };
#else
        int32_t offsets[] = { 0 };
#endif
        rend_data_ctx.state.set_vertex_buffer(0, 1, buffers, strides, offsets);

        rend_data_ctx.set_glitter_render_data_state();

        switch (rend_group->type) {
        case PARTICLE_QUAD:
            rend_data_ctx.state.set_primitive_topology(DX_PRIMITIVE_TRIANGLE_STRIP);
            rend_data_ctx.state.draw_indexed((uint32_t)(5 * rend_group->disp));
            break;
        case PARTICLE_LINE:
            rend_data_ctx.state.set_primitive_topology(DX_PRIMITIVE_LINE_STRIP);
            for (std::pair<int32_t, uint32_t>& i : rend_group->draw_list)
                rend_data_ctx.state.draw(i.second, i.first);
            break;
        case PARTICLE_LOCUS:
            rend_data_ctx.state.set_primitive_topology(DX_PRIMITIVE_TRIANGLE_STRIP);
            for (std::pair<int32_t, uint32_t>& i : rend_group->draw_list)
                rend_data_ctx.state.draw(i.second, i.first);
            break;
        }
    }

    void RenderSceneX::DispMesh() {
        disp_mesh = 0;

        for (RenderGroupX*& i : groups) {
            if (!i)
                continue;

            RenderGroupX* rend_group = i;
            if (rend_group->CannotDisp())
                continue;

            switch (rend_group->type) {
            case PARTICLE_MESH:
                rend_group->disp = 0;
                DispMesh(rend_group);
                disp_mesh += rend_group->disp;
                break;
            }
        }
    }

    void RenderSceneX::DispMesh(RenderGroupX* rend_group) {
        if (!rend_group->elements || rend_group->object.is_null() || rend_group->ctrl < 1)
            return;

        mat4 cam_view;
        mat4 cam_inv_view;
        mat4_transpose(&camera_data.view, &cam_view);
        mat4_transpose(&camera_data.inv_view, &cam_inv_view);

        bool has_scale = false;
        bool emitter_local = false;
        vec3 emit_scale = 0.0f;
        mat4 model_mat;
        mat4 view_mat;
        mat4 inv_view_mat;
        if (rend_group->flags & PARTICLE_EMITTER_LOCAL) {
            model_mat = rend_group->mat;
            mat4_normalize_rotation(&model_mat, &view_mat);
            mat4_mul(&view_mat, &cam_view, &view_mat);
            mat4_invert(&view_mat, &inv_view_mat);

            emitter_local = true;
            if (rend_group->flags & PARTICLE_SCALE) {
                if (rend_group->GetEmitterScale(emit_scale))
                    has_scale = true;
            }
        }
        else {
            model_mat = mat4_identity;
            view_mat = cam_view;
            inv_view_mat = cam_inv_view;
        }

        bool screen = false;
        if (rend_group->flags & PARTICLE_SCREEN) {
            mat4_mul(&inv_view_mat, &rend_group->mat, &inv_view_mat);
            mat4_mul(&view_mat, &inv_view_mat, &view_mat);
            mat4_invert(&view_mat, &inv_view_mat);
            screen = true;
        }
        mat4_mul(&view_mat, &cam_inv_view, &rend_group->mat_draw);

        mat4 dir_mat = mat4_identity;
        vec3 up_vec = { 0.0f, 0.0f, 1.0f };
        bool billboard = false;
        bool emitter_rotation = false;
        mat4(*rotate_func)(RenderGroupX*, RenderElementX*, vec3*, vec3*) = 0;
        switch (rend_group->draw_type) {
        case DIRECTION_BILLBOARD:
            mat4_clear_trans(&model_mat, &dir_mat);
            mat4_mul(&dir_mat, &inv_view_mat, &dir_mat);
            mat4_clear_trans(&dir_mat, &dir_mat);
            billboard = true;
            break;
        case DIRECTION_EMITTER_DIRECTION:
            dir_mat = rend_group->mat_rot;
            break;
        case DIRECTION_PREV_POSITION:
        case DIRECTION_PREV_POSITION_DUP:
            rotate_func = RenderGroupX::RotateMeshToPrevPosition;
            break;
        case DIRECTION_EMIT_POSITION:
            rotate_func = RenderGroupX::RotateMeshToEmitPosition;
            break;
        case DIRECTION_Y_AXIS:
            mat4_rotate_y((float_t)M_PI_2, &dir_mat);
            break;
        case DIRECTION_X_AXIS:
            mat4_rotate_x((float_t)-M_PI_2, &dir_mat);
            break;
        case DIRECTION_BILLBOARD_Y_AXIS:
            mat4_rotate_y(camera_data.rotation.y, &dir_mat);
            break;
        case DIRECTION_EMITTER_ROTATION:
            emitter_rotation = true;
            break;
        }

        ParticleX* particle = rend_group->particle->data.particle;

        obj* obj = objset_info_storage_get_obj(rend_group->object);
        if (!obj)
            return;

        texture_transform_struct tex_trans[2];
        int32_t tex_trans_count = 0;

        for (texture_transform_struct& i : tex_trans) {
            tex_trans->id = -1;
            tex_trans->mat = mat4_identity;
        }

        obj_material& material = obj->material_array[0].material;

        for (obj_material_texture_data& i : material.texdata) {
            if (i.tex_index == -1)
                continue;

            tex_trans[tex_trans_count].id = i.tex_index & 0xFFFFF;
            tex_trans[tex_trans_count].mat = mat4_identity;

            if (++tex_trans_count == 2)
                break;
        }

        vec3 ext_anim_scale;
        float_t ext_scale = 0.0f;
        if (rend_group->GetExtAnimScale(&ext_anim_scale, &ext_scale)) {
            if (!has_scale) {
                emit_scale = 1.0f;
                has_scale = true;
            }

            if (ext_scale >= 0.0f)
                emit_scale *= ext_anim_scale + ext_scale;
            else
                emit_scale += ext_anim_scale;
        }

        mdl::DispManager& disp_manager = *::disp_manager;
        extern bool reflect_full;
        mdl::obj_reflect_enable = reflect_full && !screen;
        disp_manager.set_texture_pattern(0, 0);
        disp_manager.set_obj_flags((mdl::ObjFlags)0);

        RenderElementX* elem = rend_group->elements;
        size_t disp = 0;
        for (size_t i = rend_group->ctrl, j_max = 1024; i > 0; i -= j_max) {
            j_max = min_def(i, j_max);
            for (size_t j = j_max; j > 0; elem++) {
                if (!elem->alive)
                    continue;
                j--;

                if (!elem->disp)
                    continue;

                vec3 trans = elem->translation;
                vec3 rot = elem->rotation;
                vec3 scale = elem->scale * elem->scale_all;
                if (has_scale)
                    scale *= emit_scale;

                if (emitter_local)
                    mat4_transform_point(&model_mat, &trans, &trans);

                mat4 mat;
                if (billboard) {
                    if (screen)
                        mat = mat4_identity;
                    else
                        mat = dir_mat;
                }
                else if (rotate_func) {
                    mat = rotate_func(rend_group, elem, &up_vec, &trans);
                }
                else if (emitter_rotation)
                    mat = elem->mat;
                else
                    mat = dir_mat;

                mat4_set_translation(&mat, &trans);
                mat4_mul_rotate_zyx(&mat, &rot, &mat);
                mat4_scale_rot(&mat, &scale, &mat);

                vec3 uv_scroll;
                vec3 uv_scroll_2nd;
                uv_scroll.x = elem->uv_scroll.x;
                uv_scroll.y = -elem->uv_scroll.y;
                uv_scroll.z = 0.0f;
                uv_scroll_2nd.x = elem->uv_scroll_2nd.x;
                uv_scroll_2nd.y = -elem->uv_scroll_2nd.y;
                uv_scroll_2nd.z = 0.0f;

                mat4_set_translation(&tex_trans[0].mat, &uv_scroll);
                mat4_set_translation(&tex_trans[1].mat, &uv_scroll_2nd);

                mat4_transpose(&tex_trans[0].mat, &tex_trans[0].mat);
                mat4_transpose(&tex_trans[1].mat, &tex_trans[1].mat);
                disp_manager.set_texture_transform(tex_trans_count, tex_trans);

                if (screen)
                    mat4_mul(&mat, &cam_inv_view, &mat);
                elem->mat_draw = mat;

                mat4_transpose(&mat, &mat);
                matrix_stack_data_push_matrix();
                matrix_stack_data_load_matrix(mat);
                if (screen)
                    disp_manager.entry_obj_by_object_info_screen(rend_group->object, &elem->color);
                else
                    disp_manager.entry_obj_by_object_info(rend_group->object, &elem->color);
                disp++;
                matrix_stack_data_pop_matrix();
            }
        }
        disp_manager.set_texture_transform(0, 0);
        rend_group->disp = disp;
        mdl::obj_reflect_enable = false;
    }

    size_t RenderSceneX::GetCtrlCount(ParticleType type) {
        switch (type) {
        case PARTICLE_QUAD:
            return ctrl_quad;
        case PARTICLE_LINE:
            return ctrl_line;
        case PARTICLE_LOCUS:
            return ctrl_locus;
        case PARTICLE_MESH:
            return ctrl_mesh;
        default:
            return 0;
        }
    }

    size_t RenderSceneX::GetDispCount(ParticleType type) {
        switch (type) {
        case PARTICLE_QUAD:
            return disp_quad;
        case PARTICLE_LINE:
            return disp_line;
        case PARTICLE_LOCUS:
            return disp_locus;
        case PARTICLE_MESH:
            return disp_mesh;
        default:
            return 0;
        }
    }

    void RenderSceneX::CalcDispLocusSetPivot(Pivot pivot,
        float_t w, float_t& v00, float_t& v01) {
        switch (pivot) {
        case PIVOT_TOP_LEFT:
        case PIVOT_MIDDLE_LEFT:
        case PIVOT_BOTTOM_LEFT:
            v00 = 0.0f;
            v01 = w;
            break;
        case PIVOT_TOP_CENTER:
        case PIVOT_MIDDLE_CENTER:
        case PIVOT_BOTTOM_CENTER:
        default:
            v00 = w * -0.5f;
            v01 = w * 0.5f;
            break;
        case PIVOT_TOP_RIGHT:
        case PIVOT_MIDDLE_RIGHT:
        case PIVOT_BOTTOM_RIGHT:
            v00 = -w;
            v01 = 0.0f;
            break;
        }
    }

    void RenderSceneX::CalcDispQuadSetPivot(Pivot pivot,
        float_t w, float_t h, float_t& v00, float_t& v01, float_t& v10, float_t& v11) {
        switch (pivot) {
        case PIVOT_TOP_LEFT:
            v00 = 0.0f;
            v01 = w;
            v10 = -h;
            v11 = 0.0f;
            break;
        case PIVOT_TOP_CENTER:
            v00 = w * -0.5f;
            v01 = w * 0.5f;
            v10 = -h;
            v11 = 0.0f;
            break;
        case PIVOT_TOP_RIGHT:
            v00 = -w;
            v01 = 0.0f;
            v10 = -h;
            v11 = 0.0f;
            break;
        case PIVOT_MIDDLE_LEFT:
            v00 = 0.0f;
            v01 = w;
            v10 = h * -0.5f;
            v11 = h * 0.5f;
            break;
        case PIVOT_MIDDLE_CENTER:
        default:
            v00 = w * -0.5f;
            v01 = w * 0.5f;
            v10 = h * -0.5f;
            v11 = h * 0.5f;
            break;
        case PIVOT_MIDDLE_RIGHT:
            v00 = -w;
            v01 = 0.0f;
            v10 = h * -0.5f;
            v11 = h * 0.5f;
            break;
        case PIVOT_BOTTOM_LEFT:
            v00 = 0.0f;
            v01 = w;
            v10 = 0.0f;
            v11 = h;
            break;
        case PIVOT_BOTTOM_CENTER:
            v00 = w * -0.5f;
            v01 = w * 0.5f;
            v10 = 0.0f;
            v11 = h;
            break;
        case PIVOT_BOTTOM_RIGHT:
            v00 = -w;
            v01 = 0.0f;
            v10 = 0.0f;
            v11 = h;
            break;
        }
    }
}
