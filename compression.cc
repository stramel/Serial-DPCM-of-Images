#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "compression.h"

using std::ifstream;
using std::ofstream;
using std::ios;
using namespace std;

void ReadInputFile(int *image, int size, char *input) {
  ifstream in (input);
  char temp_buffer[4];
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      in.getline(temp_buffer, 4, '\n');
      image[i*size+j] = atoi(temp_buffer);
    }
  }
  return;
}

void Predictor(int *image, int *residual, int size) {
  float a = 1.0/3.0;
  for (int i = 1; i < size; i++) {
    for (int j = 1; j < size; j++) {
      residual[i*size+j] = ceil(a*image[(i-1)*size+j] + 
				a*image[(i-1)*size+(j-1)] +
				a*image[i*size+(j-1)]- .5);
    }
  }
  return;
}
 
void ComputeResidual(char *input, int size, int *residual) {
  int *image = new int [size*size];
  ReadInputFile(image, size, input);
  Predictor(image, residual, size);
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      residual[i*size+j] = image[i*size+j] - residual[i*size+j];
    }
  }
  delete [] image;
  return;
}

void ProbabilityOccurence(int size, float probability[], int *residual) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      probability[residual[i*size+j]++;
    }    
  }
  for (int i = 0; i < 511; i++) {
    probability[i] /= size*size; 
  }
  return;
}

void ComputeEntropy(int *residual, int size) {
  float probability[511]= {0.0};
  float temp = 0.0;
  ProbabilityOccurence(size, probability, residual);
  for (int i = 0; i < 511; i++) {
    if (probability[i] != 0) {
      temp += probability[i] * log2(probability[i];)
    }
  }
  temp *= -1;
  printf("Entropy: %f\n", temp);
  return;
}

