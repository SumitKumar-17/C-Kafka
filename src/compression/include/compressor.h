#pragma once
#include <zlib.h>

long compress_message(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);
long decompress_message(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);