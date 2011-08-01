// Copyright 2011 Michael Stramel

#include "./compression.h"

#include <mpi.h>

#include <cstdlib>
#include <cmath>

const int kMasterNode = 0;

void SetPartitions(int num_processors, int rank, int *partition_size, int size,
                   int *end_location) {
  *partition_size = size / num_processors;
  if (rank == kMasterNode) {
    *end_location = static_cast<int>(num_processors*pow(*partition_size, 2) +
                                    pow(size, 2) - (pow(*partition_size, 2)*
                                    pow(num_processors, 2)));
  } else {
    *end_location = static_cast<int>(num_processors*pow(*partition_size, 2) +
                                    size);
  }
  return;
}

void ReadFile(char *input_filename, int rank, int size, int partition_size,
              int end_location, int *image, int num_processors, int way) {
  FILE *pFile;
  pFile = fopen(input_filename, "r");
  char temp[5];
  if (pFile != NULL && !feof(pFile)) {
    if (way == 2) {
      if (rank != kMasterNode) {
        for (int i = 0; i < num_processors*rank*pow(partition_size, 2)-size;
             i++) {
          fgets(temp, 5, pFile);
        }
      }
      for (int i = 0; i < end_location; i++) {
        fgets(temp, 5, pFile);
        image[i] = atoi(temp);
      }
    } else if (way == 1) {
      for (int i = 0; i < size*size; i++) {
        fgets(temp, 5, pFile);
        image[i] = atoi(temp);
      }
    } else {
      printf("Error: Wrong input for way\n");
    }
  } else {
    printf("Error: Problem opening file\n");
    exit(1);
  }
  return;
}

void Predictor(int *image, int *predicted_values, int end_location, int size) {
  float a = 1.0/3.0;
  for (int i = size; i < end_location; i++) {
    if (i%size != 0) {
      predicted_values[i] =static_cast<int>(a*image[i-1]+a*image[i-size]+
                                            a*image[(i-size)-1]+.5);
    }
  }
  return;
}

void ComputeResidual(char *input_filename, int *residual, int rank, int size,
                     int end_location, int num_processors, int partition_size,
                     int way) {
  int *image;
  if (way == 1) {
    int *full_image = new int[size*size];
    image = new int[end_location];
    int *counts = new int[num_processors];
    int *displs = new int[num_processors];
    if (rank == kMasterNode) {
      ReadFile(input_filename, rank, size, partition_size, end_location,
               full_image, num_processors, way);
      counts[0] = end_location;
      displs[0] = 0;
      for (int i = 1; i < num_processors; i++) {
        counts[i] = static_cast<int>(num_processors*pow(partition_size, 2)
                                     +size);
        displs[i] = static_cast<int>(num_processors*i*pow(partition_size, 2)-
                                     size);
      }
    }
    MPI_Scatterv(full_image, counts, displs, MPI_INT, image, end_location,
                 MPI_INT, kMasterNode, MPI_COMM_WORLD);
    if (rank == kMasterNode) {
      delete [] full_image;
    }
  } else if (way == 2) {
    image = new int[end_location];
    ReadFile(input_filename, rank, size, partition_size, end_location, image,
             num_processors, way);
  } else {
    printf("Error: Wrong input for way\n");
  }
  Predictor(image, residual, end_location, size);
  if (rank != kMasterNode) {
    for (int i = size; i < end_location; i++) {
      residual[i-size] = image[i] - residual[i];
      residual[i] = 999999;
      if (residual[i-size] > 255 || residual[i-size] < -255) {
        printf("ERROR: Out of Bounds\n");
      }
    }
  } else {
    for (int i = 0; i < end_location; i++) {
      residual[i] = image[i] - residual[i];
      if (residual[i] > 255 || residual[i] < -255) {
        printf("ERROR: Out of Bounds\n");
      }
    }
  }
  if (rank == kMasterNode) {
    delete [] image;
  }
  return;
}

void Probability(float *probabilities, int *residual, int num_processors,
                 int size, int end_location, int rank) {
  float *temp = new float[num_processors*511];
  for (int i = 0; i < 511; i++) {
    probabilities[i] = 0.0;
  }
  if (rank == kMasterNode) {
    for (int i = 0; i < end_location; i++) {
      probabilities[residual[i]+255]++;
    }
  } else {
    for (int i = 0; i < end_location-size; i++) {
      probabilities[residual[i]+255]++;
    }
  }
  MPI_Gather(probabilities, 511, MPI_FLOAT, temp, 511, MPI_FLOAT, kMasterNode,
             MPI_COMM_WORLD);
  for (int i = 0; i < 511; i++) {
    probabilities[i] = 0.0;
  }
  for (int i = 0; i < num_processors; i++) {
    for (int j = 0; j < 511; j++) {
      probabilities[j] += temp[i*511+j];
      if (probabilities[j] < 0) {
        printf("ERROR: Negative Total\n");
      }
    }
  }
  for (int i = 0; i < 511; i++) {
    probabilities[i] /= size*size;
  }
  return;
}

void ComputeEntropy(int *residual, int num_processors, int size,
                    int end_location, int rank) {
  float *probabilities = new float[511];
  float temp = 0.0;
  Probability(probabilities, residual, num_processors, size, end_location,
              rank);
  for (int i = 0; i < 511; i++) {
    if (probabilities[i] > 0) {
      temp += probabilities[i] * log2(probabilities[i]);
    } else if (probabilities[i] < 0) {
      printf("ERROR: Negative Probability\n");
    }
  }
  temp *= -1;
  if (rank == kMasterNode) {
    printf("Entropy: %.3f\n", temp);
  }
  return;
}