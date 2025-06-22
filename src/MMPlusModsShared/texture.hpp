/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/image.hpp"
#include "wrap.hpp"

enum texture_flags {
    TEXTURE_BLOCK_COMPRESSION = 0x01,
};

union texture_id {
    struct {
        uint32_t index : 28;
        uint32_t id : 4;
    };
    uint32_t data;

    inline texture_id() {
        data = 0;
    }

    inline texture_id(uint32_t data) {
        this->data = data;
    }

    inline operator uint32_t() {
        return *(uint32_t*)this;
    }
};

static_assert(sizeof(texture_id) == 0x04, "\"texture_id\" struct should have a size of 0x04");

constexpr bool operator==(const texture_id& left, const texture_id& right) {
    return left.data == right.data;
}

constexpr bool operator!=(const texture_id& left, const texture_id& right) {
    return !(left == right);
}

constexpr bool operator<(const texture_id& left, const texture_id& right) {
    return left.data < right.data;
}

constexpr bool operator>(const texture_id& left, const texture_id& right) {
    return right < left;
}

constexpr bool operator<=(const texture_id& left, const texture_id& right) {
    return !(right < left);
}

constexpr bool operator>=(const texture_id& left, const texture_id& right) {
    return !(left < right);
}

struct texture {
    int32_t ref_count;
    texture_id id;
    texture_flags flags;
    int16_t width;
    int16_t height;
    GLenum target;
    GLenum internal_format;
    int32_t max_mipmap_level;
    int32_t size_texmem;
    int field_20;
    p_dx_texture tex;

    texture();

    int32_t get_height_align_mip_level(uint8_t mip_level = 0) const;
    int32_t get_width_align_mip_level(uint8_t mip_level = 0) const;

    inline int32_t get_height_mip_level(uint8_t mip_level) const {
        return max_def(height >> mip_level, 1);
    }
    inline int32_t get_width_mip_level(uint8_t mip_level) const {
        return max_def(width >> mip_level, 1);
    }
};

static_assert(sizeof(texture) == 0x30, "\"texture\" struct should have a size of 0x30");

extern texture* (FASTCALL* texture_load_tex_2d)(uint32_t id, int32_t internal_format, uint32_t width,
    uint32_t height, int32_t mip_levels, const void** data, int32_t a7, bool generate_mips);
extern void (FASTCALL* texture_release)(texture* tex);
extern void (FASTCALL* txp_set_load)(const void* data, size_t size, texture*** textures, uint32_t* texture_ids);
extern void (FASTCALL* texture_array_free)(texture** tex_arr);
extern texture* (FASTCALL* texture_manager_get_texture)(uint32_t id);
extern int32_t(FASTCALL* texture_info_get_id)(const char* name);
