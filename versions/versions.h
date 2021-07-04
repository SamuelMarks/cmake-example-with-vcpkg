#ifndef PP_VERSIONS_H
#define PP_VERSIONS_H

#if defined(_WIN32)
#    define DECLSPEC __declspec(dllimport)
#else // non windows
#  define DECLSPEC
#endif

#include <stddef.h>
#include <zlib.h>
#include <openssl/crypto.h>

#ifndef ZLIB_VERSION
#define ZLIB_VERSION "0.0.0"
#endif
#ifndef OPENSSL_VERSION_TEXT
#define OPENSSL_VERSION_TEXT "0.0.0"
#endif
typedef unsigned long uLong;

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)

static const wchar_t *VERSIONS[][2] = {
        {L"ZLIB_VERSION",         WIDEN(ZLIB_VERSION)},
        {L"OPENSSL_VERSION_TEXT", WIDEN(OPENSSL_VERSION_TEXT)}
};

uLong get_hash();

#endif /* PP_VERSIONS_H */
