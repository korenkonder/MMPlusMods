/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "sprite.hpp"
#include <stdint.h>
#include <stdlib.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Helpers.h>

#define FASTCALL __fastcall

namespace spr {
    struct SpriteVertex {
        float_t pos[3]; // vec3
        float_t uv[2]; // vec2
        uint32_t color; // color4u8
    };

    static_assert(sizeof(spr::SpriteVertex) == 0x18, "\"spr::SpriteVertex\" struct should have a size of 0x18");

    struct SpriteVertexData {
        size_t max_count;
        spr::SpriteVertex* array;
    };
}

static void* (*_operator_new)(size_t) = (void* (*)(size_t))0x00000001409777D0;
static void(*_operator_delete)(void*) = (void(*)(void*))0x0000000140167020;

spr::SpriteVertexData& sprite_vertex_data = *(spr::SpriteVertexData*)0x000000014CC2D250;

const size_t sprite_vertex_array_max_count = 0x2000;
spr::SpriteVertex* sprite_vertex_array = (spr::SpriteVertex*)0x000000014CC2D250;

size_t& sprite_vertex_array_count = *(size_t*)0x000000014CC2D248;

HOOK(void, FASTCALL, spr__SprArgs__put_sprite_line_list_mid, 0x00000001405B4BC0);

HOOK(void, FASTCALL, spr__SprArgs__set_vertex_array, 0x00000001405B7B40,
    __int64 This, spr::SpriteVertex* vertex_array, size_t num_vertex) {
    if (!sprite_vertex_data.array)
        sprite_vertex_data.array = (spr::SpriteVertex*)_operator_new(
            sizeof(spr::SpriteVertex) * sprite_vertex_data.max_count);

    if (sprite_vertex_array_count + num_vertex >= sprite_vertex_data.max_count) {
        while (sprite_vertex_array_count + num_vertex >= sprite_vertex_data.max_count)
            sprite_vertex_data.max_count *= 2;

        spr::SpriteVertex* _sprite_vertex_array = (spr::SpriteVertex*)_operator_new(
            sizeof(spr::SpriteVertex) * sprite_vertex_data.max_count);
        memmove(_sprite_vertex_array, sprite_vertex_data.array,
            sizeof(spr::SpriteVertex) * sprite_vertex_array_count);
        _operator_delete(sprite_vertex_data.array);
        sprite_vertex_data.array = _sprite_vertex_array;
    }

    *(size_t*)(This + 0x100) = num_vertex;

    *(size_t*)(This + 0xF8) = sprite_vertex_array_count;
    memmove(sprite_vertex_data.array + *(size_t*)(This + 0xF8),
        vertex_array, sizeof(spr::SpriteVertex) * num_vertex);

    sprite_vertex_array_count += num_vertex;
}

HOOK(void, FASTCALL, spr__SprArgs__reserve_vertex_array, 0x00000001405B7BA0, __int64 This, int32_t num_vertex) {
    if (!sprite_vertex_data.array)
        sprite_vertex_data.array = (spr::SpriteVertex*)_operator_new(
            sizeof(spr::SpriteVertex) * sprite_vertex_data.max_count);

    if (sprite_vertex_array_count + num_vertex >= sprite_vertex_data.max_count) {
        while (sprite_vertex_array_count + num_vertex >= sprite_vertex_data.max_count)
            sprite_vertex_data.max_count *= 2;

        spr::SpriteVertex* _sprite_vertex_array = (spr::SpriteVertex*)_operator_new(
            sizeof(spr::SpriteVertex) * sprite_vertex_data.max_count);
        memmove(_sprite_vertex_array, sprite_vertex_data.array,
            sizeof(spr::SpriteVertex) * sprite_vertex_array_count);
        _operator_delete(sprite_vertex_data.array);
        sprite_vertex_data.array = _sprite_vertex_array;
    }

    *(size_t*)(This + 0xF8) = sprite_vertex_array_count;

    sprite_vertex_array_count += num_vertex;
}

HOOK(void, FASTCALL, spr__draw_sprite_scale_mid, 0x00000001405B7CC0);
HOOK(void, FASTCALL, spr__calc_sprite_texture_param_mid1, 0x00000001405B7EA0);
HOOK(void, FASTCALL, spr__calc_sprite_texture_param_mid2, 0x00000001405B7EC2);
HOOK(void, FASTCALL, spr__sub_1405B9550_mid1, 0x00000001405B9CDB);
HOOK(void, FASTCALL, spr__sub_1405B9550_mid2, 0x00000001405BA422);
HOOK(void, FASTCALL, spr__draw_sprite_fill_vertex_array_mid, 0x00000001405BA9BA);

HOOK(void, FASTCALL, sprite_manager_clear, 0x00000001405B8AA0) {
    if (sprite_vertex_data.array)
        _operator_delete(sprite_vertex_data.array);
    originalsprite_manager_clear();
}

void sprite_draw_limit_patch() {
    sprite_vertex_data.array = 0;
    sprite_vertex_data.max_count = sprite_vertex_array_max_count;

    WRITE_NOP_8(0x00000001405B4BC0);
    WRITE_NOP_7(0x00000001405B7CC0);
    WRITE_NOP_7(0x00000001405B7EA0);
    WRITE_NOP_7(0x00000001405B7EC2);
    WRITE_NOP_7(0x00000001405B9CDB);
    WRITE_NOP_7(0x00000001405BA422);
    WRITE_NOP_7(0x00000001405BA9BA);

    INSTALL_HOOK(spr__SprArgs__put_sprite_line_list_mid);
    INSTALL_HOOK(spr__SprArgs__set_vertex_array);
    INSTALL_HOOK(spr__SprArgs__reserve_vertex_array);
    INSTALL_HOOK(spr__draw_sprite_scale_mid);
    INSTALL_HOOK(spr__calc_sprite_texture_param_mid1);
    INSTALL_HOOK(spr__calc_sprite_texture_param_mid2);
    INSTALL_HOOK(spr__sub_1405B9550_mid1);
    INSTALL_HOOK(spr__sub_1405B9550_mid2);
    INSTALL_HOOK(spr__draw_sprite_fill_vertex_array_mid);
    INSTALL_HOOK(sprite_manager_clear);
}
