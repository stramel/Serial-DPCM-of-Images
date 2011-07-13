#include <cstdlib>
#include "compression.h"
#include <iostream>

int main (int argc, char *argv[]) {
  int size = atoi(argv[2]);
  char *in_filename = argv[1];
  int *out_buffer = new int [size*size];
  ComputeResidual (in_filename, size, out_buffer);
  ComputeEntropy (out_buffer, size);
  delete [] out_buffer;
  return 0;
}
