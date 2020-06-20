void grad_case5(float (&C)[32][32], float (&D)[4][32], float (&dA)[16][32], float (&dB)[16][32][4]) {
  for (int i = 0; i < 16; ++i) {
    for (int j = 0; j < 32; ++j) {
      for (int k = 0; k < 4; ++k) {
        dB[i][j][k] = 0;
      }
    }
  }
  for (int i = 0; i < 16; ++i) {
    for (int j = 0; j < 32; ++j) {
      for (int k = 0; k < 32; ++k) {
        for (int l = 0; l < 4; ++l) {
          for (int b = i; b < i + 1; ++b) {
            for (int c = k; c < k + 1; ++c) {
              for (int d = l; d < l + 1; ++d) {
                dB[b][c][d] = dB[b][c][d] + dA[i][j] * 1 * C[k][j] * D[l][j];
              }
            }
          }
        }
      }
    }
  }
}
