#include <cstdlib>
#include "compression.h"
#include <iostream>

int main(int argc, char *argv[]) {
  int size = atoi(argv[2]);
  char *in_filename = argv[1];
  int *residual = new int [size*size];
  ComputeResidual(in_filename, size, residual);
  ComputeEntropy(residual, size);
  delete [] residual;
  return 0;
}
