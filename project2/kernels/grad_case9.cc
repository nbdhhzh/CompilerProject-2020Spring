void grad_case9(float (&dB)[4][6], float (&dA)[4]) {
  for (int i = 0; i < 4; ++i) {
    dA[i] = 0;
  }
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 6; ++j) {
      for (int b = i; b < i + 1; ++b) {
        dA[b] = dA[b] + dB[i][j] * 1;
      }
    }
  }
}
