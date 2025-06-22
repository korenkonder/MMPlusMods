/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "stage.hpp"
#include "../KKdLib/hash.hpp"
#include "../KKdLib/mat.hpp"
#include "pv_game/firstread.hpp"
#include "object.hpp"
#include "render_manager.hpp"
#include <Helpers.h>

enum stage_data_flags {
    STAGE_DATA_REFLECT_TEXTURE_MASK = 0x01,
    STAGE_DATA_LIGHT_CHARA_AMBIENT  = 0x02,
    STAGE_DATA_REFLECT_TONE_CURVE   = 0x04,
    STAGE_DATA_LIGHT_STAGE_AMBIENT  = 0x08,
};

struct stage_data_refract {
    reflect_refract_resolution_mode mode;
};

static_assert(sizeof(stage_data_refract) == 0x04, "\"stage_data_refract\" struct should have a size of 0x04");

struct stage_data {
    const char* name;
    const char* auth_3d_name;
    int32_t object_set_id;
    object_info object_ground;
    object_info object_ring;
    object_info object_sky;
    object_info object_shadow;
    object_info object_reflect;
    object_info object_refract;
    uint32_t lens_flare_texture;
    uint32_t lens_shaft_texture;
    uint32_t lens_ghost_texture;
    float_t lens_shaft_inv_scale;
    uint32_t field_3C;
    uint32_t render_texture;
    uint32_t movie_texture;
    const char* collision_file_path;
    stage_data_reflect_type reflect_type;
    bool refract;
    stage_data_reflect* reflect_data;
    stage_data_refract* refract_data;
    stage_data_flags flags;
    float_t ring_rectangle_x;
    float_t ring_rectangle_y;
    float_t ring_rectangle_width;
    float_t ring_rectangle_height;
    float_t ring_height;
    float_t ring_out_height;
    int32_t field_84;
};

static_assert(sizeof(stage_data) == 0x88, "\"stage_data\" struct should have a size of 0x88");

struct stage {
    int32_t index;
    uint16_t counter;
    int32_t state;
    stage_data* stage_data;
    bool stage_display;
    bool lens_flare;
    bool ground;
    bool ring;
    bool sky;
    bool auth_3d_loaded;
    mat4 mat;
    float_t rot_y;
    int32_t obj_set;
};

static_assert(sizeof(stage) == 0x68, "\"stage\" struct should have a size of 0x68");

static bool& chara_reflect = *(bool*)0x0000000141148679;

HOOK(void, FASTCALL, stage__set, 0x000000015A47C0A0, stage* prev, stage* curr) {
    originalstage__set(prev, curr);

    extern bool reflect_full;
    extern const firstread* firstread_ptr;
    if (reflect_full && firstread_ptr && firstread_ptr->stage_data_array && (!curr || prev != curr) && curr) {
        const firstread_stage_data_array* stage_data_array = firstread_ptr->stage_data_array;

        ::stage_data* stage_data = curr->stage_data;
        const uint64_t name_hash = hash_utf8_xxh3_64bits(stage_data->name);
        const uint32_t num_stage_data = stage_data_array->num_stage_data;
        for (uint32_t i = 0; i < num_stage_data; i++) {
            if (hash_utf8_xxh3_64bits(stage_data_array->stage_data_array[i].name) != name_hash)
                continue;

            const firstread_stage_data* frg_stage_data = &stage_data_array->stage_data_array[i];

            chara_reflect = false;
            render_manager.reflect_type = (stage_data_reflect_type)frg_stage_data->reflect_type;

            if (frg_stage_data->reflect) {
                const stage_data_reflect* reflect = frg_stage_data->reflect;
                render_manager.pass_sw[rndr::RND_PASSID_REFLECT] = true;
                render_manager.reflect_blur_num = reflect->blur_num;
                render_manager.reflect_blur_filter = (blur_filter_mode)reflect->blur_filter;
                render_manager.reflect = true;
                reflect_refract_resolution_mode  mode = REFLECT_REFRACT_RESOLUTION_512x256;
                if (frg_stage_data->reflect_type != STAGE_DATA_REFLECT_REFLECT_MAP)
                    mode = reflect->mode;
                render_manager.tex_index[0] = mode;
            }
            break;
        }
    }
}

void stage_patch() {
    INSTALL_HOOK(stage__set);
}
