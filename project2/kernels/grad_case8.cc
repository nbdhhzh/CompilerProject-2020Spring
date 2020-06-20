void grad_case8(float (&dB)[32], float (&dA)[2][16]) {
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 16; ++j) {
      dA[i][j] = 0;
    }
  }
  for (int i = 0; i < 32; ++i) {
    for (int b = i / 16; b < i / 16 + 1; ++b) {
      for (int c = i % 16; c < i % 16 + 1; ++c) {
        dA[b][c] = dA[b][c] + dB[i] * 1;
      }
    }
  }
}
