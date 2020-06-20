void grad_case3(float (&B)[16][16], float (&dC)[4][16], float (&dA)[4][16]) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 16; ++j) {
      dA[i][j] = 0;
    }
  }
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 16; ++j) {
      for (int k = 0; k < 16; ++k) {
        for (int b = i; b < i + 1; ++b) {
          for (int c = k; c < k + 1; ++c) {
            dA[b][c] = dA[b][c] + dC[i][j] * 1 * B[k][j];
          }
        }
      }
    }
  }
}
