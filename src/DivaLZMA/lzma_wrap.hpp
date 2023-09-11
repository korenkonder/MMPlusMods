/*
    by korenkonder
    GitHub/GitLab: korenkonder

    NOT ACTUAL .lzma FORMAT!
*/

#pragma once

#include <stdint.h>

#define LZMA_SIGNATURE ((uint16_t)(0xFB1F))

extern void LzmaInit();
extern int32_t LzmaCompressLzma(void* dest, size_t* dest_len, const void* source, size_t source_len, const char* name);
extern int32_t LzmaUncompressLzma(void* dest, size_t* dest_len, const void* source, size_t source_len);
