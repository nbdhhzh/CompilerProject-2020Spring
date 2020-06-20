void grad_case7(float (&dB)[16][32], float (&dA)[32][16]) {
  for (int i = 0; i < 32; ++i) {
    for (int j = 0; j < 16; ++j) {
      dA[i][j] = 0;
    }
  }
  for (int i = 0; i < 16; ++i) {
    for (int j = 0; j < 32; ++j) {
      for (int b = j; b < j + 1; ++b) {
        for (int c = i; c < i + 1; ++c) {
          dA[b][c] = dA[b][c] + dB[i][j] * 1;
        }
      }
    }
  }
}
