void kernel_example(float (&B)[32][16], float (&C)[32][16], float (&A)[32][16]) {
  float temp0[32][16];
  for (int i = 0; i < 32; ++i) {
    for (int j = 0; j < 16; ++j) {
      temp0[i][j] = C[i][j] * B[i][j];
    }
  }
  for (int i = 0; i < 32; ++i) {
    for (int j = 0; j < 16; ++j) {
      A[i][j] = temp0[i][j];
    }
  }
}
