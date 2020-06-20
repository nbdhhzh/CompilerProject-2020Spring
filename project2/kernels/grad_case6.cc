void grad_case6(float (&C)[8][16][3][3], float (&dA)[2][8][5][5], float (&dB)[2][16][7][7]) {
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 16; ++j) {
      for (int k = 0; k < 7; ++k) {
        for (int l = 0; l < 7; ++l) {
          dB[i][j][k][l] = 0;
        }
      }
    }
  }
  for (int n = 0; n < 2; ++n) {
    for (int k = 0; k < 8; ++k) {
      for (int p = 0; p < 5; ++p) {
        for (int q = 0; q < 5; ++q) {
          for (int c = 0; c < 16; ++c) {
            for (int r = 0; r < 3; ++r) {
              for (int s = 0; s < 3; ++s) {
                for (int b = n; b < n + 1; ++b) {
                  for (int d = c; d < c + 1; ++d) {
                    for (int e = p + r; e < p + r + 1; ++e) {
                      for (int f = q + s; f < q + s + 1; ++f) {
                        dB[b][d][e][f] = dB[b][d][e][f] + dA[n][k][p][q] * 1 * C[k][c][r][s];
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}
