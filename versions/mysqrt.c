#include <iostream>

#include "versions.h"

// include the generated table
#include "Table.h"

// a hack square root calculation using simple operations
double mysqrt(double x)
{
  if (x <= 0) {
    return 0;
  }

  // use the table to help find an initial value
  double result = x;
  if (x >= 1 && x < 10) {
    puts("Use the table to help find an initial value ");
    result = sqrtTable[static_cast<int>(x)];
  }

  // do ten iterations
  for (unsigned short i = 0; i < 10; ++i) {
    if (result <= 0) {
      result = 0.1;
    }
    double delta = x - (result * result);
    result = result + 0.5 * delta / result;
    printf("Computing sqrt of %f to be %f\n", x, result);
  }

  return result;
}
