/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "file_handler.hpp"
#include "../MMPlusModsShared/file_handler.hpp"
#include <Helpers.h>

HOOK(bool, FASTCALL, file_handler__read_farc_file, 0x00000001402A38D0, file_handler* This) {
    bool ret = originalfile_handler__read_farc_file(This);
    if (ret)
        return ret;

    if (This->file_path.size() >= 4 && !This->file_path.compare(This->file_path.size() - 4, 4, ".opd")) {
        This->size = 0x14;
        This->read_data = prj::HeapCMallocAllocateByType(This->heap_malloc_type, This->size, This->file_path.c_str());;
        if (This->read_data) {
            memset(This->read_data, 0, 0x14);
            *(uint32_t*)This->read_data = 'OPDP';
            return true;
        }
    }
    return ret;
}

void file_handler_patch() {
    INSTALL_HOOK(file_handler__read_farc_file);
}
