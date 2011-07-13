#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iostream>
#include "compression.h"
#include <errno.h>

using std::ifstream;
using std::ofstream;
using std::ios;
using namespace std;

void ReadInputFile (int *buffer, int size, char *input) {  //PROBLEM
  ifstream in (input);
  char temp_buffer[5];
  for (int i = 0; i < size*size; i++) {
      in.getline(temp_buffer, 4, '\n');
      buffer[i] = atoi(temp_buffer);
  }

  return;
}

void Predictor (int *in_buffer, int *out_buffer, int size) {
  float a = 1.0/3.0;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if ( i - 1 > 0 && j - 1 > 0) {
       	out_buffer[i*size+j] = (int)(a*in_buffer[(i-1)*size+j] + 
				    a*in_buffer[(i-1)*size+(j-1)] +
				     a*in_buffer[i*size+(j-1)]);
      } else { 
	out_buffer[i*size+j] = in_buffer[i*size+j];
      }
    }
  }
  return;
}
 
void ComputeResidual  (char *input, int size, int *out_buffer) {
  int *in_buffer = new int [size*size];
  ReadInputFile (in_buffer, size, input);
  Predictor (in_buffer, out_buffer, size);
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      out_buffer[i*size+j] = in_buffer[i*size+j] - out_buffer[i*size+j];
    }
  }
  delete [] in_buffer;
  return;
}


void ProbabilityOccurence (int size, float probability[], int *buffer,
			   float &temp) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      int temp2 = buffer[i*size+j];
      probability[temp2+255]++;
    }    
  }
float total = 0.0;
  for (int i = 0; i < 512; i++) {
    probability[i] /= size*size; 
    if (probability[i] > 0 && probability[i] < 1) {
      temp += probability[i] * log2(probability[i]);
    }
  }
  temp *= -1;
  if (errno == ERANGE) {
    cout << "ERROR: errno" << endl;
  }
  return;
}

void ComputeEntropy (int *buffer, int size) {
  float probability[512]= {0.0};
  float temp = 0.0;
  ProbabilityOccurence (size, probability, buffer, temp);
  printf("Entropy: %f\n", temp);
  return;
}

