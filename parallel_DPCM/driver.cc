// Copyright 2011 Michael Stramel

#include "./compression.h"

#include <mpi.h>

#include <cstdlib>

const int kMasterNode = 0;

int main(int argc, char **argv) {
  if (argc != 4) {
    printf("Usage: %s input_file size way\n", argv[0]);
    return 1;
  }
  double initial_time = MPI_Wtime();
  int way = atoi(argv[3]);
  int size = atoi(argv[2]);
  int rank, partition_size, num_processors, end_location, error_code;
  error_code = MPI_Init(&argc, &argv);
  if (error_code != 0) {
    printf("ERROR: MPI_ Init unsuccessful");
    MPI_Abort(MPI_COMM_WORLD, error_code);
  }
  MPI_Comm_size(MPI_COMM_WORLD, &num_processors);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  SetPartitions(num_processors, rank, &partition_size, size, &end_location);
  int *residual = new int[end_location];
  for (int i = 0; i < end_location; i++) {
    residual[i] = 0;
  }
  ComputeResidual(argv[1], residual, rank, size, end_location, num_processors,
                  partition_size, way);
  ComputeEntropy(residual, num_processors, size, end_location, rank);
  if (rank == kMasterNode) {
    printf("Total Time Taken: %f\n", MPI_Wtime() - initial_time);
  }
  MPI_Finalize();
  delete [] residual;
  return 0;
}