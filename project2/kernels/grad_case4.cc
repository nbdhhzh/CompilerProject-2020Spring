void grad_case4(float (&B)[16][32], float (&C)[32][32], float (&dA)[16][32], float (&dB)[16][32], float (&dC)[32][32]) {
  for (int i = 0; i < 32; ++i) {
    for (int j = 0; j < 32; ++j) {
      dC[i][j] = 0;
    }
  }
  for (int i = 0; i < 16; ++i) {
    for (int j = 0; j < 32; ++j) {
      dB[i][j] = 0;
    }
  }
  for (int i = 0; i < 16; ++i) {
    for (int j = 0; j < 32; ++j) {
      for (int k = 0; k < 32; ++k) {
        for (int b = i; b < i + 1; ++b) {
          for (int c = k; c < k + 1; ++c) {
            dB[b][c] = dB[b][c] + dA[i][j] * 1 * C[k][j];
          }
        }
      }
      for (int k = 0; k < 32; ++k) {
        for (int b = k; b < k + 1; ++b) {
          for (int c = j; c < j + 1; ++c) {
            dC[b][c] = dC[b][c] + dA[i][j] * B[i][k] * 1;
          }
        }
      }
    }
  }
}
