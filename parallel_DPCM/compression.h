// Copyright 2011 Michael Stramel
#ifndef _HOME_MSTRAMEL_DPCM_P_COMPRESSION_H_
#define _HOME_MSTRAMEL_DPCM_P_COMPRESSION_H_

void SetPartitions(int num_processors, int rank, int *partition_size, int size,
                   int *end_location);
void ReadFile(char *input_filename, int rank, int size, int partition_size,
              int end_location, int *image, int num_processors, int way);
void Predictor(int *image, int *predicted_values, int end_location, int size);
void ComputeResidual(char *input_filename, int *residual, int rank, int size,
                     int end_location, int num_processors, int partition_size,
                     int way);
void Probability(float *probabilities, int *residual, int num_processors,
                 int size, int end_location, int rank);
void ComputeEntropy(int *residual, int num_processors, int size,
                    int end_location, int rank);
#endif  // _HOME_MSTRAMEL_DPCM_P_COMPRESSION_H_
