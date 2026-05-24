/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "types.hpp"
#include "wrap.hpp"

// Added
struct IndexBuffer {
    p_dx_buffer buffer;
};

static_assert(sizeof(IndexBuffer) == 0x08, "\"IndexBuffer\" struct should have a size of 0x08");

struct VertexBuffer {
    static const uint32_t NUM_FLIP_MAX = 3;

    p_dx_buffer buffers[NUM_FLIP_MAX];
    int32_t size;
    uint32_t offset;
    void* data;
    uint32_t count;
    uint32_t index;

    void cycle_index();
    void get_buffer(p_dx_buffer*& buffer, uint32_t& offset);
    int32_t get_size();
};

static_assert(sizeof(VertexBuffer) == 0x30, "\"VertexBuffer\" struct should have a size of 0x30");

struct BufObjMgr {
    int32_t vb_peak_size;
    int32_t vb_all_size;
    int32_t ib_peak_size;
    int32_t ib_all_size;
};

static_assert(sizeof(BufObjMgr) == 0x10, "\"BufObjMgr\" struct should have a size of 0x10");

extern BufObjMgr& bufobj_mgr;
