void kernel_case7(float (&A)[32][16], float (&B)[16][32]) {
  float temp0[16][32];
  for (int i = 0; i < 16; ++i) {
    for (int j = 0; j < 32; ++j) {
      temp0[i][j] = A[j][i];
    }
  }
  for (int i = 0; i < 16; ++i) {
    for (int j = 0; j < 32; ++j) {
      B[i][j] = temp0[i][j];
    }
  }
}
