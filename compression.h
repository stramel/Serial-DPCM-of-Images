void ReadInputFile (int *buffer, int size, char *input);
void Predictor (char *inp_buffer, char *out_buffer, int size);
void ComputeResidual  (char *input, int size, int *out_buffer);
void ProbabilityOccurence (int size, float probability[], int *buffer,
			   float &temp);
void ComputeEntropy (int *buffer, int size);
