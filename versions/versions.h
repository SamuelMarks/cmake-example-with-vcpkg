#ifndef PP_VERSIONS_H
#define PP_VERSIONS_H

#if defined(_WIN32)
#  if defined(EXPORTING_MYMATH)
#    define DECLSPEC __declspec(dllexport)
#  else
#    define DECLSPEC __declspec(dllimport)
#  endif
#else // non windows
#  define DECLSPEC
#endif

#include <stddef.h>
/*#include <zlib.h>
#include <openssl/crypto.h>
 */
#define ZLIB_VERSION "0.0.0"
#define OPENSSL_VERSION_TEXT "0.0.0"
typedef unsigned long uLong;

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)

static const wchar_t *VERSIONS[][2] = {
        {L"ZLIB_VERSION",         WIDEN(ZLIB_VERSION)},
        {L"OPENSSL_VERSION_TEXT", WIDEN(OPENSSL_VERSION_TEXT)}
};

uLong get_hash();

#endif /* PP_VERSIONS_H */
