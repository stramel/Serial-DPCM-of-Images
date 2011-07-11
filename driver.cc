#include <cstdlib>
#include "compression.h"

int main (int argc, char *argv[]) {
  unsigned int size = atoi(argv[3]);
  char *input_filename = argv[1];
  char *output_filename = argv[2];

  ComputeResidual (input_filename, output_filename, size);

  return 0;
}
