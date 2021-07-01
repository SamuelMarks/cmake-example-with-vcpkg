// A simple program that computes the square root of a number
#include <iostream>
#include <string>

#include "versions.h"
#include "TutorialConfig.h"

int main(int argc, char* argv[])
{
  if (argc < 2) {
    // report version
    std::cout << argv[0] << " Version " << Tutorial_VERSION_MAJOR << "."
              << Tutorial_VERSION_MINOR << std::endl;
    std::cout << "Usage: " << argv[0] << " number" << std::endl;
    return 1;
  }

    for(unsigned i=0; i<VERSIONS_N; i++)
        std::cout << VERSIONS[i][0] << '\t' << VERSIONS[i][1] << std::endl;

  return EXIT_SUCCESS;
}
