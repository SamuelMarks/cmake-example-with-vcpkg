// A simple program that computes the square root of a number
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "versions.h"
#include "TutorialConfig.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        // report version
        fprintf(stderr, "%s Version %d.%d\n", argv[0],
                Tutorial_VERSION_MAJOR, Tutorial_VERSION_MINOR);
        printf("Usage: %s number\n", argv[0]);
        return EXIT_FAILURE;
    }
    size_t i;

    for (i = 0; i < sizeof VERSIONS / sizeof VERSIONS[0]; i++)
        wprintf(L"%ls\t%ls\n", VERSIONS[i][0], VERSIONS[i][1]);

    printf("\nget_hash(): %lu\n", get_hash());

    return EXIT_SUCCESS;
}
