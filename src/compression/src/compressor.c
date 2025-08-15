#include "../include/compressor.h"
#include <stdio.h>
#include <stdlib.h>

long compress_message(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen) {
    int res = compress(dest, destLen, source, sourceLen);
    if (res != Z_OK) {
        fprintf(stderr, "Compression failed with error: %d\n", res);
        return -1;
    }
    return *destLen;
}

long decompress_message(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen) {
    int res = uncompress(dest, destLen, source, sourceLen);
    if (res != Z_OK) {
        fprintf(stderr, "Decompression failed with error: %d\n", res);
        return -1;
    }
    return *destLen;
}