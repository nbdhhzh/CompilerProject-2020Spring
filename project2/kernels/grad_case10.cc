void grad_case10(float (&dA)[8][8], float (&dB)[10][8]) {
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 8; ++j) {
      dB[i][j] = 0;
    }
  }
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      for (int b = i; b < i + 1; ++b) {
        for (int c = j; c < j + 1; ++c) {
          dB[b][c] = dB[b][c] + dA[i][j] * (1) / 3;
        }
      }
      for (int b = i + 1; b < i + 1 + 1; ++b) {
        for (int c = j; c < j + 1; ++c) {
          dB[b][c] = dB[b][c] + dA[i][j] * (1) / 3;
        }
      }
      for (int b = i + 2; b < i + 2 + 1; ++b) {
        for (int c = j; c < j + 1; ++c) {
          dB[b][c] = dB[b][c] + dA[i][j] * (1) / 3;
        }
      }
    }
  }
}
