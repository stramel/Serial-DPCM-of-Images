void ReadInputFile (int *buffer, unsigned int &length, char *input);
void WriteOutputFile (int *buffer, char *output, unsigned int size);
void Predictor (int *input_buffer, int *output_buffer, unsigned int size);
void ComputeResidual  (char *input, char *output, unsigned int size);
