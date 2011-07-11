
#include <cmath>
#include <fstream>
#include <iostream>
#include "compression.h"

using std::ifstream;
using std::ofstream;
using std::ios;
using namespace std;


void ReadInputFile (int *buffer, unsigned int &length, char *input) {
  ifstream in (input, ios::binary);
  in.seekg(0, ios::end);
  length = in.tellg();
  in.seekg(0, ios::beg);
  in.read((char *)buffer, length);
  in.close();
  return;
}


void WriteOutputFile (int *buffer, char *output, unsigned int size) {
  ofstream out (output, ios::binary);
  out.write((char *)buffer, size);
  out.close();
  return;
}

void Predictor (int *input_buffer, int *output_buffer, unsigned int size) {
  float a = 1.0/3.0;
  for ( unsigned int i = 0; i < size; i++) {
    for (unsigned int j = 0; j < size; j++) {
      if ( i - 1 < 0 || j - 1 < 0) {
	output_buffer[i*size+j] = ceil(a*input_buffer[(i-1)*size+j] + a*input_buffer[(i-1)*size+(j-1)] + a*input_buffer[i*size+(j-1)]);
      } else { 
	output_buffer[i*size+j] = input_buffer[i*size+j];
      }
    }
  }
  return;
}
 
void ComputeResidual  (char *input, char *output, unsigned int size) {
  int *input_buffer = new int [size*size];
  int *output_buffer = new int [size*size];
  unsigned int length = 0;
  ReadInputFile (input_buffer, length, input);
  Predictor (input_buffer, output_buffer, size);
  for (unsigned int i = 0; i < size; i++) {
    for (unsigned int j = 0; j < size; j++) {
      output_buffer[i*size+j] -= input_buffer[i*size+j];
    }
  }
  WriteOutputFile (output_buffer, output, length);
  return;
}
