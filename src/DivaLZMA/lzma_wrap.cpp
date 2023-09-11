/*
    by korenkonder
    GitHub/GitLab: korenkonder

    NOT ACTUAL .lzma FORMAT!
*/

#include "lzma_wrap.hpp"
#include <stdlib.h>
#include <string.h>
#include "LZMA/7zCrc.h"
#include "LZMA/LzmaLib.h"

#pragma pack(push, 1)
struct lzma_header {
    uint16_t signature;
    uint8_t props[LZMA_PROPS_SIZE];
    uint32_t size;
    uint32_t crc;
    uint8_t name[1];
};
#pragma pack(pop)

void LzmaInit() {
    CrcGenerateTable();
}

int32_t LzmaCompressLzma(void* dest, size_t* dest_len, const void* source, size_t source_len, const char* name) {
    if (*dest_len <= sizeof(lzma_header))
        return -SZ_ERROR_OUTPUT_EOF;

    size_t name_len = 0;
    if (name)
        while (name[name_len])
            name_len++;

    size_t header_size = sizeof(lzma_header) + name_len;

    if (*dest_len <= header_size)
        return -SZ_ERROR_OUTPUT_EOF;

    int32_t level = 8;

    uint32_t calc_crc = CrcCalc(source, source_len);

    lzma_header header;
    header.signature = LZMA_SIGNATURE;
    memset(header.props, 0, LZMA_PROPS_SIZE);
    header.size = (uint32_t)source_len;
    header.crc = calc_crc;
    header.name[0] = 0;

    *dest_len -= header_size;
    size_t _props_size = LZMA_PROPS_SIZE;
    int32_t code = LzmaCompress(
        (unsigned char*)((size_t)dest + header_size),
        dest_len,
        (const unsigned char*)source,
        source_len,
        (unsigned char*)header.props,
        &_props_size,
        level, 1 << 24, 3, 0, 2, 32, 1);
    *dest_len += header_size;

    switch (code) {
    default:
    case SZ_OK:
        *(lzma_header*)dest = header;
        if (name) {
            memcpy(((lzma_header*)dest)->name, name, name_len);
            ((lzma_header*)dest)->name[name_len] = 0;
        }
        return 0;
    case SZ_ERROR_MEM:
        return -SZ_ERROR_MEM;
    case SZ_ERROR_PARAM:
        return -SZ_ERROR_PARAM;
    case SZ_ERROR_OUTPUT_EOF:
        return -SZ_ERROR_OUTPUT_EOF;
    case SZ_ERROR_THREAD:
        return -SZ_ERROR_THREAD;
    }
}

int32_t LzmaUncompressLzma(void* dest, size_t* dest_len, const void* source, size_t source_len) {
    lzma_header* header = (lzma_header*)source;
    size_t name_len = 0;
    while (header->name[name_len])
        name_len++;

    size_t header_size = sizeof(lzma_header) + name_len;

    int32_t code = LzmaUncompress(
        (unsigned char*)dest,
        dest_len,
        (const unsigned char*)((size_t)source + header_size),
        &source_len,
        header->props,
        LZMA_PROPS_SIZE);

    switch (code) {
    default:
    case SZ_OK: {
        uint32_t calc_crc = CrcCalc(dest, *dest_len);
        if (calc_crc != header->crc)
            return -SZ_ERROR_DATA;
    } return 0;
    case SZ_ERROR_DATA:
        return -SZ_ERROR_DATA;
    case SZ_ERROR_MEM:
        return -SZ_ERROR_MEM;
    case SZ_ERROR_UNSUPPORTED:
        return -SZ_ERROR_UNSUPPORTED;
    case SZ_ERROR_INPUT_EOF:
        return -SZ_ERROR_INPUT_EOF;
    }
}
