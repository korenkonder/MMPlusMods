/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include <deque>
#include <list>
#include <mutex>
#include <string>
#include <thread>
#include "../KKdLib/default.hpp"
#include "../KKdLib/farc.hpp"
#include "types.hpp"

typedef void (PFNFILEHANDLERCALLBACK)(void*, const void*);

struct file_handler_callback_func {
    PFNFILEHANDLERCALLBACK* func;
    void* data;
    bool ready;
};

struct file_handler {
    int32_t count;
    __int64 mtx[10];
    bool not_ready;
    bool reading;
    prj::string farc_path;
    prj::string file_path;
    prj::HeapCMallocType heap_malloc_type;
    bool cache;
    file_handler_callback_func callback[2];
    ssize_t size;
    void* read_data;
    void* write_data;
};

static_assert(sizeof(file_handler) == 0xF0, "\"file_handler\" struct should have a size of 0xF0");

struct p_file_handler {
    file_handler* ptr;

    p_file_handler();
    ~p_file_handler();

    void call_free_callback();
    bool check_not_ready();
    const void* get_data();
    size_t get_size();
    bool read_file(const char* farc_path, const char* file_path, prj::HeapCMallocType heap_malloc_type, bool cache);
    bool read_file_path(const char* file_path, prj::HeapCMallocType heap_malloc_type);
    void read_now();
    void reset();
    void set_callback_data(int32_t index, PFNFILEHANDLERCALLBACK* func, void* data);
};

static_assert(sizeof(p_file_handler) == 0x08, "\"p_file_handler\" struct should have a size of 0x08");
