// A simple program that builds a sqrt table
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  // make sure we have enough arguments
  if (argc < 2) return EXIT_FAILURE;

    FILE * fout;

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
    fopen_s(&fout, argv[1], "wt");
#else
    fout = fopen(argv[1], "w");
#endif
  if ((int)fout != EOF) {
      fprintf(fout, "double sqrtTable[] = {\n");
      for (unsigned short i = 0; i < 10; ++i)
          fprintf(fout, "%f,\n", sqrt(i));

      // close the table with a zero
      fputs("0};\n", fout);
      return fclose(fout) == EOF ? EXIT_FAILURE : EXIT_SUCCESS;
  }
}
