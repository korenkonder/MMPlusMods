/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "object.hpp"
#include "../KKdLib/hash.hpp"
#include "pv_game/firstread.hpp"
#include <Helpers.h>

static int32_t load_set_id;

HOOK(bool, FASTCALL, _objset_info_storage_load_obj_set_check_not_read, 0x0000000140467110, int32_t set) {
    load_set_id = set;
    bool ret = original_objset_info_storage_load_obj_set_check_not_read(set);
    load_set_id = -1;
    return ret;
}

HOOK(obj_set*, FASTCALL, obj_set_parse_file, 0x000000014046A360,
    prj::shared_ptr<prj::stack_allocator>& alloc, __int64 set_file, int32_t& result) {
    obj_set* set = originalobj_set_parse_file(alloc, set_file, result);

    extern bool reflect_full;
    extern const firstread* firstread_ptr;
    if (reflect_full && firstread_ptr && firstread_ptr->objset_array && load_set_id != -1) {
        const firstread_objset_array* objset_array = firstread_ptr->objset_array;
        if (objset_array->objset_id_array && objset_array->objset_array) {
            const uint32_t num_objset = objset_array->num_objset;
            for (uint32_t i = 0; i < num_objset; i++) {
                if (objset_array->objset_id_array[i] != load_set_id || !objset_array->objset_array[i].data)
                    continue;

                const firstread_objset* frb_set = firstread_objset::read(alloc,
                    objset_array->objset_array[i].data, objset_array->objset_array[i].size);
                if (!frb_set || !set->obj_data || !frb_set->obj_data)
                    break;

                const int32_t frb_obj_num = frb_set->obj_num;
                for (int32_t j = 0; j < frb_obj_num; j++) {
                    const int32_t obj_num = set->obj_num;
                    for (int32_t k = 0; k < obj_num; k++) {
                        if (frb_set->obj_data[j].id != set->obj_id_data[k])
                            continue;

                        obj* obj = set->obj_data[k];
                        const firstread_obj* frb_obj = &frb_set->obj_data[j];
                        if (obj->mesh_array && frb_obj->mesh_array) {
                            const int32_t frb_num_mesh = frb_obj->num_mesh;
                            int32_t index_match = -1;
                            for (int32_t l = 0; l < frb_num_mesh; l++) {
                                const uint64_t name_hash = hash_utf8_xxh3_64bits(frb_obj->mesh_array[l].name);
                                const int32_t num_mesh = obj->num_mesh;
                                for (int32_t m = 0; m < num_mesh; m++) {
                                    if (name_hash != hash_utf8_xxh3_64bits(obj->mesh_array[m].name))
                                        continue;
                                    else if (frb_obj->mesh_array[l].index != (uint8_t)-1) {
                                        if (++index_match != frb_obj->mesh_array[l].index)
                                            continue;
                                    }

                                    obj_mesh* mesh = &obj->mesh_array[m];
                                    const firstread_obj_mesh* frb_mesh = &frb_obj->mesh_array[l];
                                    if (frb_mesh->remove) {
                                        mesh->flags = 0;
                                        mesh->bounding_sphere = {};
                                        mesh->num_submesh = 0;
                                        mesh->submesh_array = 0;
                                        mesh->vertex_format = (obj_vertex_format)0;
                                        mesh->size_vertex = 0;
                                        mesh->num_vertex = 0;
                                        for (int32_t n = 0; n < 20; n++)
                                            mesh->vertex_array_ptr[n] = 0;
                                        mesh->attrib = {};
                                        mesh->vertex_flags = 0;
                                        break;
                                    }

                                    if (mesh->submesh_array && mesh->num_submesh) {
                                        const int32_t num_submesh = mesh->num_submesh;
                                        for (int32_t n = 0; n < num_submesh; n++) {
                                            obj_sub_mesh* submesh = &mesh->submesh_array[n];
                                            submesh->attrib.m.no_reflect = frb_mesh->attrib.m.no_reflect;
                                            submesh->attrib.m.reflect = frb_mesh->attrib.m.reflect;
                                            submesh->attrib.m.reflect_cam_back = frb_mesh->attrib.m.reflect_cam_back;
                                        }
                                    }

                                    if (mesh->submesh_array && frb_mesh->submesh_array
                                        && mesh->num_submesh && mesh->num_submesh == frb_mesh->num_submesh) {
                                        const int32_t num_submesh = mesh->num_submesh;
                                        int32_t num_submesh_split = num_submesh;
                                        for (int32_t n = 0; n < num_submesh; n++) {
                                            const firstread_obj_sub_mesh* frb_submesh = &frb_mesh->submesh_array[n];
                                            if (frb_submesh->split)
                                                num_submesh_split++;
                                        }

                                        if (num_submesh < num_submesh_split) {
                                            obj_sub_mesh* submesh_array = mesh->submesh_array;
                                            mesh->num_submesh = num_submesh_split;
                                            mesh->submesh_array = alloc->allocate<obj_sub_mesh>(num_submesh_split);
                                            for (int32_t n = 0, o = 0; n < num_submesh; n++, o++) {
                                                const firstread_obj_sub_mesh* frb_submesh = &frb_mesh->submesh_array[n];
                                                mesh->submesh_array[o] = submesh_array[n];
                                                if (frb_submesh->split)
                                                    mesh->submesh_array[++o] = submesh_array[n];
                                            }
                                        }

                                        for (int32_t n = 0, o = 0; n < num_submesh; n++, o++) {
                                            obj_sub_mesh* submesh = &mesh->submesh_array[o];
                                            const firstread_obj_sub_mesh* frb_submesh = &frb_mesh->submesh_array[n];

                                            if (frb_submesh->split) {
                                                firstread_obj_sub_mesh_split* split = frb_submesh->split;
                                                if (split->material_index[0] != -1)
                                                    submesh->material_index = split->material_index[0];
                                                if (split->num_index[0] && split->index_array[0]) {
                                                    submesh->bounding_sphere = split->bounding_sphere[0];
                                                    submesh->index_format = split->index_format;
                                                    submesh->num_index = split->num_index[0];
                                                    submesh->index_array = (void*)split->index_array[0];
                                                }
                                                submesh->attrib = split->attrib[0];

                                                obj_sub_mesh* submesh_before = &mesh->submesh_array[++o];
                                                if (split->material_index[1] != -1)
                                                    submesh_before->material_index = split->material_index[1];
                                                if (split->num_index[1] && split->index_array[1]) {
                                                    submesh_before->bounding_sphere = split->bounding_sphere[1];
                                                    submesh_before->index_format = split->index_format;
                                                    submesh_before->num_index = split->num_index[1];
                                                    submesh_before->index_array = (void*)split->index_array[1];
                                                }
                                                submesh_before->attrib = split->attrib[1];
                                                continue;
                                            }
                                            else if (frb_submesh->num_index && frb_submesh->index_array) {
                                                submesh->bounding_sphere = frb_submesh->bounding_sphere;
                                                submesh->index_format = frb_submesh->index_format;
                                                submesh->num_index = frb_submesh->num_index;
                                                submesh->index_array = (void*)frb_submesh->index_array;
                                            }

                                            submesh->attrib = frb_submesh->attrib;
                                        }
                                    }

                                    if (frb_mesh->vertex_format && frb_mesh->num_vertex) {
                                        mesh->vertex_format = frb_mesh->vertex_format;
                                        mesh->size_vertex = frb_mesh->size_vertex;
                                        mesh->num_vertex = frb_mesh->num_vertex;
                                        for (int32_t n = 0; n < 20; n++)
                                            mesh->vertex_array_ptr[n] = frb_mesh->vertex_array_ptr[n];
                                    }

                                    mesh->attrib = frb_mesh->attrib;
                                    break;
                                }
                            }
                        }

                        if (frb_obj->num_material && frb_obj->material_array) {
                            obj->num_material = frb_obj->num_material;
                            obj->material_array = (obj_material_data*)frb_obj->material_array;
                        }
                        break;
                    }
                }
                break;
            }
        }
    }

    return set;
}

void object_patch() {
    INSTALL_HOOK(_objset_info_storage_load_obj_set_check_not_read);
    INSTALL_HOOK(obj_set_parse_file);
}
