/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "sprite.hpp"
#include <Helpers.h>

void (FASTCALL* sprite_manager_unload_set)(int32_t set_id)
    = (void (FASTCALL*)(int32_t set_id))0x00000001405B48B0;
void (FASTCALL* sprite_manager_read_file)(int32_t set_id, const prj::string_range& mdata_dir)
    = (void (FASTCALL*)(int32_t set_id, const prj::string_range & mdata_dir))0x00000001405B4770;
bool (FASTCALL* sprite_manager_load_file)(int32_t set_id)
    = (bool (FASTCALL*)(int32_t set_id))0x00000001405B4810;

namespace spr {
    spr::SprArgs* (FASTCALL* put_sprite)(const spr::SprArgs& args)
        = (spr::SprArgs * (FASTCALL*)(const spr::SprArgs & args))0x00000001405B49C0;

    SprArgs::SprArgs() : kind(), attr(), blend(), index(), layer(),
        prio(), resolution_mode_screen(), resolution_mode_sprite(), texture(),
        shader(), field_AC(), field_F0(), vertex_array(), num_vertex(), field_108(),
        field_110(), set_viewport(), flags(), field_138(), next() {
        Reset();
    }

    void SprArgs::Reset() {
        kind = SPR_KIND_NORMAL;
        id = {};
        color = 0xFFFFFFFF;
        attr = (SprAttr)0;
        blend = 0;
        index = -1;
        layer = 0;
        prio = SPR_PRIO_DEFAULT;
        resolution_mode_screen = RESOLUTION_MODE_HD;
        resolution_mode_sprite = RESOLUTION_MODE_HD;
        center = 0.0f;
        trans = 0.0f;
        scale = 1.0f;
        rot = 0.0f;
        skew_angle = 0.0f;
        mat = mat4_identity;
        texture = 0;
        shader = 0;
        field_AC = -1;
        transform = mat4_null;
        field_F0 = false;
        vertex_array = 0;
        num_vertex = 0;
        field_108 = -1;
        field_110 = 0;
        set_viewport = false;
        viewport = { 0.0f, 0.0f };
        flags = (SprArgs::Flags)0;
        sprite_size = 0.0f;
        field_138 = 0;
        texture_pos = 0.0f;
        texture_size = 1.0f;
        next = 0;
    }

    void SprArgs::SetSpriteSize(vec2 size) {
        if (flags & SPRITE_SIZE)
            return;

        enum_or(flags, SPRITE_SIZE);
        sprite_size = size;
        field_138 = 0;
    }

    void SprArgs::SetTexturePosSize(float_t x, float_t y, float_t width, float_t height) {
        if (flags & TEXTURE_POS_SIZE)
            return;

        enum_or(flags, TEXTURE_POS_SIZE);
        texture_pos.x = x;
        texture_pos.y = y;
        texture_size.x = width;
        texture_size.y = height;
    }

    void SprArgs::SetVertexArray(SpriteVertex* vertex_array, size_t num_vertex) {
        void (FASTCALL * spr__SprArgs__SetVertexArray)(spr::SprArgs * This,
            spr::SpriteVertex * vertex_array, size_t num_vertex)
            = (void (FASTCALL*)(spr::SprArgs * This,
                spr::SpriteVertex * vertex_array, size_t num_vertex))0x000000014063F040;
        spr__SprArgs__SetVertexArray(this, vertex_array, num_vertex);
    }

    void SprArgs::SetNext(SprArgs* args, SprArgs* next) {
        while (args->next)
            args = args->next;
        args->next = next;
        next->kind = SPR_KIND_LINE;
    }

    void put_sprite_line(vec2 p1, vec2 p2, resolution_mode mode, spr::SprPrio prio, color4u8 color, int32_t layer) {
        spr::SprArgs args;
        args.trans.x = p1.x;
        args.trans.y = p1.y;
        args.trans.z = 0.0f;
        args.layer = layer;
        args.kind = SPR_KIND_LINES;
        args.resolution_mode_screen = mode;
        args.resolution_mode_sprite = mode;
        args.prio = prio;
        args.color = color;
        args.SetSpriteSize({ p2.x - p1.x, p2.y - p1.y });
        enum_or(args.attr, SPR_ATTR_MATRIX);
        spr::put_sprite(args);
    }

    void put_sprite_line_list(vec2* points, size_t count, resolution_mode mode,
        spr::SprPrio prio, color4u8 color, int32_t layer) {
        spr::SprArgs args;
        args.kind = SPR_KIND_ARROW_AB;
        args.layer = layer;
        args.resolution_mode_screen = mode;
        args.resolution_mode_sprite = mode;
        args.prio = prio;
        args.color = color;

        std::vector<spr::SpriteVertex> vertex_array;
        vertex_array.reserve(count);

        for (size_t i = 0; i < count; i++, points++) {
            spr::SpriteVertex vert = {};
            *(vec2*)&vert.pos = *points;
            vertex_array.push_back(vert);
        }

        args.SetVertexArray(vertex_array.data(), vertex_array.size());
        spr::put_sprite(args);
    }

    void put_sprite_multi(rectangle rect, resolution_mode mode, spr::SprPrio prio, color4u8 color, int32_t layer) {
        spr::SprArgs args;
        args.trans.x = rect.pos.x;
        args.trans.y = rect.pos.y;
        args.trans.z = 0.0f;
        args.layer = layer;
        args.kind = SPR_KIND_MULTI;
        args.resolution_mode_screen = mode;
        args.resolution_mode_sprite = mode;
        args.prio = prio;
        args.color = color;
        args.SetSpriteSize(rect.size);
        enum_or(args.attr, SPR_ATTR_MATRIX);
        spr::put_sprite(args);
    }

    void put_sprite_rect(rectangle rect, resolution_mode mode, spr::SprPrio prio, color4u8 color, int32_t layer) {
        spr::SprArgs args;
        args.trans.x = rect.pos.x;
        args.trans.y = rect.pos.y;
        args.trans.z = 0.0f;
        args.layer = layer;
        args.kind = SPR_KIND_RECT;
        args.resolution_mode_screen = mode;
        args.resolution_mode_sprite = mode;
        args.prio = prio;
        args.color = color;
        args.SetSpriteSize(rect.size);
        enum_or(args.attr, SPR_ATTR_MATRIX);
        spr::put_sprite(args);
    }
}

int32_t sprite_database_get_spr_set_id_by_name(const prj::string& name) {
    struct spr_db_spr_set {
        int32_t id;
        prj::string_range name;
        prj::string_range file_name;
        int32_t index;
    };

    static_assert(sizeof(spr_db_spr_set) == 0x30, "\"spr_db_spr_set\" struct should have a size of 0x30");

    const spr_db_spr_set* (FASTCALL* sprite_database_struct__get_spr_set_id_by_name)
        (size_t This, const prj::string_range& name)
        = (const spr_db_spr_set * (FASTCALL*)(size_t This, const prj::string_range & name))0x00000001405BC770;
    return sprite_database_struct__get_spr_set_id_by_name(0, prj::string_range(*(prj::string*)&name))->id;
}
