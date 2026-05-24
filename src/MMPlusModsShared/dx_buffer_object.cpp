/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "dx_buffer_object.hpp"
#include "object.hpp"

BufObjMgr& bufobj_mgr = *(BufObjMgr*)0x000000014CC2D078;

void VertexBuffer::cycle_index() {
    if (++index >= count)
        index = 0;
}

void VertexBuffer::get_buffer(p_dx_buffer*& buffer, uint32_t& offset) {
    if (index < count) {
        buffer = &buffers[index];
        offset = this->offset;
    }
    else {
        buffer = 0;
        offset = 0;
    }
}

int32_t VertexBuffer::get_size() {
    if (this)
        return size;
    return 0;
}
