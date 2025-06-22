/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "texture.hpp"
#include "types.hpp"
#include <Helpers.h>

texture* (FASTCALL* texture_load_tex_2d)(uint32_t id, int32_t internal_format, uint32_t width,
    uint32_t height, int32_t mip_levels, const void** data, int32_t a7, bool generate_mips)
    = (texture * (FASTCALL * )(uint32_t id, int32_t internal_format, uint32_t width,
        uint32_t height, int32_t mip_levels, const void** data, int32_t a7, bool generate_mips))0x00000001405F0720;
void (FASTCALL* texture_release)(texture* tex)
    = (void (FASTCALL*)(texture * tex)) 0x00000001405F0950;
void (FASTCALL* txp_set_load)(const void* data, size_t size, texture*** textures, uint32_t* texture_ids)
    = (void (FASTCALL*)(const void* data, size_t size, texture * **textures, uint32_t * texture_ids))0x000000015A889230; // 0x00000001405F0B60
void (FASTCALL* texture_array_free)(texture** tex_arr)
    = (void (FASTCALL*)(texture * *tex_arr))0x000000015A895BA0; // 0x00000001405F0C40
texture* (FASTCALL* texture_manager_get_texture)(uint32_t id)
    = (texture * (FASTCALL*)(uint32_t id))0x00000001405F0CB0;
int32_t(FASTCALL* texture_info_get_id)(const char* name)
    = (int32_t(FASTCALL*)(const char* name))0x00000001405F1450;

texture::texture() : ref_count(), id(), flags(), width(), height(),
target(), internal_format(), max_mipmap_level(), size_texmem(), field_20() {

}

int32_t texture::get_height_align_mip_level(uint8_t mip_level) const {
    if (flags & TEXTURE_BLOCK_COMPRESSION)
        return max_def((uint32_t)height >> mip_level, 4u);
    else
        return max_def((uint32_t)height >> mip_level, 1u);
}

int32_t texture::get_width_align_mip_level(uint8_t mip_level) const {
    if (flags & TEXTURE_BLOCK_COMPRESSION)
        return max_def((uint32_t)width >> mip_level, 4u);
    else
        return max_def((uint32_t)width >> mip_level, 1u);
}
