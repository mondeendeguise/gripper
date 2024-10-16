#ifndef MATRIX_H_
#define MATRIX_H_

#include "vector.h"

typedef struct {
    float c[2*2];
} M2x2f;

typedef struct {
    float c[3*3];
} M3x3f;

typedef struct {
    float c[4*4];
} M4x4f;

void m2x2f_print(M2x2f a);
M2x2f m2x2f_diagonal(float x);
M2x2f m2x2f_add(M2x2f a, M2x2f b);
M2x2f m2x2f_subtract(M2x2f a, M2x2f b);
M2x2f m2x2f_scale(M2x2f m, float s);
M2x2f m2x2f_multiply(M2x2f a, M2x2f b);

void m3x3f_print(M3x3f a);
M3x3f m3x3f_diagonal(float x);
M3x3f m3x3f_add(M3x3f a, M3x3f b);
M3x3f m3x3f_subtract(M3x3f a, M3x3f b);
M3x3f m3x3f_scale(M3x3f m, float s);
M3x3f m3x3f_multiply(M3x3f a, M3x3f b);

void m4x4f_print(M4x4f a);
M4x4f m4x4f_diagonal(float x);
M4x4f m4x4f_add(M4x4f a, M4x4f b);
M4x4f m4x4f_subtract(M4x4f a, M4x4f b);
M4x4f m4x4f_scale(M4x4f m, float s);
M4x4f m4x4f_multiply(M4x4f a, M4x4f b);

#endif // MATRIX_H_
