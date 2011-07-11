
#include <cmath>
#include <fstream>
#include <iostream>
#include "compression.h"

using std::ifstream;
using std::ofstream;
using std::ios;


void ReadInputFile (char *buffer, int size, char *input) {
  ifstream in (input, ios::binary);
  in.read(input, size);
  in.close();
  return;
}

void WriteOutputFile (char *buffer, char *output, int size) {
  ofstream out (output, ios::binary);
  out.write(buffer, size);
  out.close();
  return;
}

void Predictor (char *input_buffer, char *output_buffer, int size) {
  float a = 1.0/3.0;
  for ( int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if ( i - 1 < 0 || j - 1 < 0) {
	output_buffer[i*size+j] = ceil(a*input_buffer[(i-1)*size+j] + a*input_buffer[(i-1)*size+(j-1)] + a*input_buffer[i*size+(j-1)]);
      } else { 
	output_buffer[i*size+j] = input_buffer[i*size+j];
      }
    }
  }
  return;
}
 
void ComputeResidual  (char *input, char *output, int size) {
  int size_of_file = size * size;
  char *input_buffer = new char [size_of_file];
  char *output_buffer = new char [size_of_file];
  ReadInputFile (input_buffer, size_of_file, input);
  Predictor (input_buffer, output_buffer, size);
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      output_buffer[i*size+j] -= input_buffer[i*size+j];
    }
  }
  WriteOutputFile (output_buffer, output, size_of_file);
  return;
}
