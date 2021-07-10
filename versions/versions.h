#ifndef PP_VERSIONS_H
#define PP_VERSIONS_H

#include <stddef.h>
#include <zlib.h>
#include <openssl/crypto.h>
#include "versions_export.h"

extern const wchar_t VERSIONS_EXPORT *VERSIONS[][2];
extern const size_t VERSIONS_EXPORT VERSIONS_N;
uLong VERSIONS_EXPORT get_hash();

#endif /* PP_VERSIONS_H */
