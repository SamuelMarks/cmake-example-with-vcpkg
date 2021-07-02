#include "versions.h"

uLong get_hash() {
#define BUF_LEN 5
    /*uLong crc = crc32(0L, Z_NULL, 0);
    static const Bytef buffer[BUF_LEN];
    // memset(buffer, 0, BUF_LEN);
    return crc32(crc, buffer, BUF_LEN);*/
    return BUF_LEN;
#undef BUF_LEN
}
