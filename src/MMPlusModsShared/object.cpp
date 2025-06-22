/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "object.hpp"

uint32_t(FASTCALL* object_database_get_object_info)(const char* name)
    = (uint32_t(FASTCALL*)(const char* name))0x0000000140466A00;
const char* (FASTCALL* object_database_get_obj_name)(uint32_t obj_info)
    = (const char* (FASTCALL*)(uint32_t obj_info))0x0000000140466B20;
int32_t(FASTCALL* objset_info_storage_load_set)(int32_t set, int32_t a2)
    = (int32_t(FASTCALL*)(int32_t set, int32_t a2))0x0000000156B11FE0; // 0x0000000140466E40
bool (FASTCALL* objset_info_storage_load_obj_set_check_not_read)(int32_t set)
    = (bool(FASTCALL*)(int32_t set))0x0000000140467110;
void (FASTCALL* objset_info_storage_unload_set)(int32_t set, int32_t a2)
    = (void (FASTCALL*)(int32_t set, int32_t a2))0x0000000156B2DBE0; // 0x0000000140468970
obj* (FASTCALL* objset_info_storage_get_obj)(uint32_t obj_info)
    = (obj * (FASTCALL*)(uint32_t obj_info))0x0000000140469880;
const char* (FASTCALL* object_database_get_set_name)(int32_t set_id, int32_t a2)
    = (const char* (FASTCALL*)(int32_t set_id, int32_t a2))0x00000001404699F0;
int32_t(FASTCALL* object_database_get_object_set_id)(const prj::string_range& name)
    = (int32_t(FASTCALL*)(const prj::string_range & name))0x0000000140469AC0;
obj_mesh* (FASTCALL* objset_info_storage_get_obj_mesh_by_index)(uint32_t obj_info, int32_t mesh_index)
    = (obj_mesh * (FASTCALL*)(uint32_t obj_info, int32_t mesh_index))0x0000000140469E90;
int32_t(FASTCALL* objset_info_storage_get_obj_mesh_index)(uint32_t obj_info, const char* mesh_name)
    = (int32_t(FASTCALL*)(uint32_t obj_info, const char* mesh_name))0x0000000140469EE0;

bool object_info_cache::get(object_info obj_info) {
    static bool (FASTCALL * object_info_cache__get)(object_info_cache * This, uint32_t obj_info)
        = (bool(FASTCALL*)(object_info_cache* This, uint32_t obj_info))0x0000000140469D60;
    return object_info_cache__get(this, obj_info);
}

obj* object_info_cache::get_obj() {
    static obj* (FASTCALL * object_info_cache__get_obj)(object_info_cache * This)
        = (obj * (FASTCALL*)(object_info_cache* This))0x0000000140469CA0;
    return object_info_cache__get_obj(this);
}

const texture** object_info_cache::get_textures(int32_t index) {
    static const texture** (FASTCALL * object_info_cache__get_textures)(object_info_cache * This, int32_t index)
        = (const texture * *(FASTCALL*)(object_info_cache * This, int32_t index))0x0000000140469D00;
    return object_info_cache__get_textures(this, index);
}

obj_mesh_index_buffer* object_info_cache::get_obj_mesh_index_buffers() {
    static obj_mesh_index_buffer* (FASTCALL * object_info_cache__get_obj_mesh_index_buffers)(object_info_cache * This)
        = (obj_mesh_index_buffer * (FASTCALL*)(object_info_cache * This))0x0000000140469CC0;
    return object_info_cache__get_obj_mesh_index_buffers(this);
}

obj_mesh_vertex_buffer* object_info_cache::get_obj_mesh_vertex_buffers() {
    static obj_mesh_vertex_buffer* (FASTCALL * object_info_cache__get_obj_mesh_vertex_buffers)(object_info_cache * This)
        = (obj_mesh_vertex_buffer * (FASTCALL*)(object_info_cache * This))0x0000000140469CE0;
    return object_info_cache__get_obj_mesh_vertex_buffers(this);
}

void obj_mesh_vertex_buffer::cycle_index() {
    if (++index >= count)
        index = 0;
}

void obj_mesh_vertex_buffer::get_buffer(p_dx_buffer*& buffer, uint32_t& offset) {
    if (index < count) {
        buffer = &buffers[index];
        offset = this->offset;
    }
    else {
        buffer = 0;
        offset = 0;
    }
}

int32_t obj_mesh_vertex_buffer::get_size() {
    if (this)
        return size;
    return 0;
}
