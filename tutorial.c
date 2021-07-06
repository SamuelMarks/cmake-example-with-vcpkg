#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include <versions.h>
#include "TutorialConfig.h"

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
/* https://docs.microsoft.com/en-us/cpp/c-runtime-library/path-field-limits */
#define PATH_SEP "\\"
#define MAX_FNAME _MAX_FNAME
#else
#define PATH_SEP "/"
#define MAX_FNAME PATH_MAX
#endif

int main(int argc, char *argv[]) {
    {
        char basename[MAX_FNAME] = {0};
        {
/* TODO: #define macros for STRTOK and STRCPY */
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
            char *token = strtok_s(argv[0], PATH_SEP, NULL);
#else
            char *token = strtok(argv[0], PATH_SEP);
#endif
            for (; token;
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
            token = strtok_s(NULL, PATH_SEP, NULL)
#else
                token = strtok(NULL, PATH_SEP)
#endif
                    )
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
                strcpy_s(basename, strlen(token), token);
#else
            strcpy(basename, token);
#endif
        }

        printf("%s version: %4d.%d\n"
               "\nget_hash(): %20lu\n", basename,
               Tutorial_VERSION_MAJOR, Tutorial_VERSION_MINOR,
               get_hash());
    }

    {
        size_t i;
        for (i = 0; i < sizeof VERSIONS / sizeof VERSIONS[0]; i++)
            wprintf(L"%ls: %14ls\n", VERSIONS[i][0], VERSIONS[i][1]);
    }

    return EXIT_SUCCESS;
}
