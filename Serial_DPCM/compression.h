#ifndef COMPRESSION_H_
#define COMPRESSION_H_

void ReadInputFile(int *image, int size, char *input);
void Predictor(char *image, char *residual, int size);
void ComputeResidual (char *input, int size, int *residual);
void ProbabilityOccurence(int size, float probability[], int *residual);
void ComputeEntropy(int *residual, int size);

#endif
