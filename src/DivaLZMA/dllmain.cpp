/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include <Windows.h>
#include <stdint.h>
#include <time.h> 
#include "Helpers.h"
#include "SigScan.h"
#include "lzma_wrap.hpp"
#include <unordered_map>
#include <mutex>

SIG_SCAN(
    sigSub1402A01B0,
    0x1402A01B0,
    "\x40\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C"
    "\x24\xE1\x48\x81\xEC\x98\x00\x00\x00\x48\x8B\x05\x98\xC1\xAF\x00",
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxx????"
);

SIG_SCAN(
    sigZlibCompressGzip,
    0x140173430,
    "\x40\x55\x53\x56\x57\x48\x8D\x6C\x24\xF8\x48\x81\xEC\x08\x01\x00\x00\x48\x8B\x05\x20\x8F\xC2\x00",
    "xxxxxxxxxxxxxxxxxxxx????"
);

SIG_SCAN(
    sigZlibUncompressGzip,
    0x14D8FDEB0,
    "\x40\x57\x48\x81\xEC\xA0\x00\x00\x00\x48\x8B\x05\xA8\xE4\x49\xF3",
    "xxxxxxxxxxxx????"
);

bool lzma_compress_enable = true;

std::mutex lzma_mtx;
std::unordered_map<DWORD, bool> lzma_compress_enable_map;

HOOK(bool, __fastcall, Sub1402A01B0, sigSub1402A01B0(), __int64 a1, __int64 a2, __int64 a3, __int64 a4) {
    DWORD thread_id = GetCurrentThreadId();
    {
        std::unique_lock<std::mutex> u_lock(lzma_mtx);
        lzma_compress_enable_map.insert({ thread_id, lzma_compress_enable });
    }

    bool ret = originalSub1402A01B0(a1, a2, a3, a4);

    {
        std::unique_lock<std::mutex> u_lock(lzma_mtx);
        lzma_compress_enable_map.erase(thread_id);
    }
    return ret;
}

HOOK(int32_t, __fastcall, ZlibCompressGzip, sigZlibCompressGzip(),
    void* dest, size_t* dest_len, const void* source, size_t source_len, const char* name) {
    DWORD thread_id = GetCurrentThreadId();

    bool lzma_compress_enable = false;
    {
        std::unique_lock<std::mutex> u_lock(lzma_mtx);
        auto elem = lzma_compress_enable_map.find(thread_id);
        if (elem != lzma_compress_enable_map.end())
            lzma_compress_enable = elem->second;
    }

    if (lzma_compress_enable)
        return LzmaCompressLzma(dest, dest_len, source, source_len, name);

    return originalZlibCompressGzip(dest, dest_len, source, source_len, name);
}

HOOK(int32_t, __fastcall, ZlibUncompressGzip, sigZlibUncompressGzip(),
    void* dest, size_t* dest_len, const void* source, size_t source_len) {
    if (source && source_len >= 2 && *(uint16_t*)source == LZMA_SIGNATURE)
        return LzmaUncompressLzma(dest, dest_len, source, source_len);
    return originalZlibUncompressGzip(dest, dest_len, source, source_len);
}

extern "C" __declspec(dllexport) void PreInit() {
    if (sigSub1402A01B0Valid && sigZlibCompressGzipValid && sigZlibUncompressGzipValid) {
        LzmaInit();
        INSTALL_HOOK(Sub1402A01B0);
        INSTALL_HOOK(ZlibCompressGzip);
        INSTALL_HOOK(ZlibUncompressGzip);
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    return TRUE;
}
