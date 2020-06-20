void grad_case2(float (&A)[4][16], float (&dB)[4][16], float (&dA)[4][16]) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 16; ++j) {
      dA[i][j] = 0;
    }
  }
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 16; ++j) {
      for (int b = i; b < i + 1; ++b) {
        for (int c = j; c < j + 1; ++c) {
          dA[b][c] = dA[b][c] + dB[i][j] * 1 * A[i][j];
        }
      }
      for (int b = i; b < i + 1; ++b) {
        for (int c = j; c < j + 1; ++c) {
          dA[b][c] = dA[b][c] + dB[i][j] * A[i][j] * 1;
        }
      }
    }
  }
}
