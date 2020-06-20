void kernel_case2(float (&A)[16][8]) {
  float temp0[16][8];
  for (int i = 0; i < 16; ++i) {
    for (int j = 0; j < 8; ++j) {
      temp0[i][j] = A[i][j];
      temp0[i][j] = temp0[i][j] + 2;
    }
  }
  for (int i = 0; i < 16; ++i) {
    for (int j = 0; j < 8; ++j) {
      A[i][j] = temp0[i][j];
    }
  }
}
