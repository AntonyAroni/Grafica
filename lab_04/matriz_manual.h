#ifndef MATRIZ_MANUAL_H
#define MATRIZ_MANUAL_H

#include <math.h>
#include <string.h>

inline void multiply4x4(float A[16], float B[16], float C[16]) {
  for (int col = 0; col < 4; col++) {
    for (int row = 0; row < 4; row++) {
      C[col * 4 + row] = 0;
      for (int k = 0; k < 4; k++) {
        C[col * 4 + row] += A[k * 4 + row] * B[col * 4 + k];
      }
    }
  }
}

inline void setIdentity(float M[16]) {
  memset(M, 0, 16 * sizeof(float));
  M[0] = M[5] = M[10] = M[15] = 1.0f;
}

inline void getTranslation(float M[16], float tx, float ty, float tz) {
  setIdentity(M);
  M[12] = tx;
  M[13] = ty;
  M[14] = tz;
}

inline void getRotationZ(float M[16], float angle) {
  setIdentity(M);
  float c = cosf(angle);
  float s = sinf(angle);
  M[0] = c;
  M[4] = -s;
  M[1] = s;
  M[5] = c;
}

inline void getRotationX(float M[16], float angle) {
  setIdentity(M);
  float c = cosf(angle);
  float s = sinf(angle);
  M[5] = c;
  M[9] = -s;
  M[6] = s;
  M[10] = c;
}

#endif
