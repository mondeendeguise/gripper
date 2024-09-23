#include "matrix.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

void m2x2f_print(M2x2f a)
{
    for(size_t i = 0; i < 2; ++i) {
        for(size_t j = 0; j < 2; ++j) {
            printf("%f ", a.c[(i*2)+j]);
        }
        printf("\n");
    }
}

M2x2f m2x2f_diagonal(float x)
{
    M2x2f mat = {0};
    for(size_t i = 0; i < 2; ++i) {
        mat.c[(i*2)+i] = x;
    }
    return mat;
}

M2x2f m2x2f_add(M2x2f a, M2x2f b)
{
    for(size_t i = 0; i < 2*2; ++i) {
        a.c[i] += b.c[i];
    }
    return a;
}

M2x2f m2x2f_subtract(M2x2f a, M2x2f b)
{
    for(size_t i = 0; i < 2*2; ++i) {
        a.c[i] -= b.c[i];
    }
    return a;
}

M2x2f m2x2f_scale(M2x2f m, float s)
{
    for(size_t i = 0; i < 2*2; ++i) {
        m.c[i] *= s;
    }
    return m;
}

M2x2f m2x2f_multiply(M2x2f a, M2x2f b)
{
    M2x2f mat = {0};
    for(size_t i = 0; i < 2*2; ++i) {
        float total = 0.0f;
        for(size_t j = 0; j < 2; ++j) {
            total += a.c[2*(i/2)+j] * b.c[(i%2)+2*j];
        }
        mat.c[i] = total;
    }
    return mat;
}

void m3x3f_print(M3x3f a)
{
    for(size_t i = 0; i < 3; ++i) {
        for(size_t j = 0; j < 3; ++j) {
            printf("%f ", a.c[(i*3)+j]);
        }
        printf("\n");
    }
}

M3x3f m3x3f_diagonal(float x)
{
    M3x3f mat = {0};
    for(size_t i = 0; i < 3; ++i) {
        mat.c[(i*3)+i] = x;
    }
    return mat;
}

M3x3f m3x3f_add(M3x3f a, M3x3f b)
{
    for(size_t i = 0; i < 3*3; ++i) {
        a.c[i] += b.c[i];
    }
    return a;
}

M3x3f m3x3f_subtract(M3x3f a, M3x3f b)
{
    for(size_t i = 0; i < 3*3; ++i) {
        a.c[i] -= b.c[i];
    }
    return a;
}

M3x3f m3x3f_scale(M3x3f m, float s)
{
    for(size_t i = 0; i < 3*3; ++i) {
        m.c[i] *= s;
    }
    return m;
}

M3x3f m3x3f_multiply(M3x3f a, M3x3f b)
{
    M3x3f mat = {0};
    for(size_t i = 0; i < 3*3; ++i) {
        float total = 0.0f;
        for(size_t j = 0; j < 3; ++j) {
            total += a.c[3*(i/3)+j] * b.c[(i%3)+3*j];
        }
        mat.c[i] = total;
    }
    return mat;
}

void m4x4f_print(M4x4f a)
{
    for(size_t i = 0; i < 4; ++i) {
        for(size_t j = 0; j < 4; ++j) {
            printf("%f ", a.c[(i*4)+j]);
        }
        printf("\n");
    }
}

M4x4f m4x4f_diagonal(float x)
{
    M4x4f mat = {0};
    for(size_t i = 0; i < 4; ++i) {
        mat.c[(i*4)+i] = x;
    }
    return mat;
}

M4x4f m4x4f_add(M4x4f a, M4x4f b)
{
    for(size_t i = 0; i < 4*4; ++i) {
        a.c[i] += b.c[i];
    }
    return a;
}

M4x4f m4x4f_subtract(M4x4f a, M4x4f b)
{
    for(size_t i = 0; i < 4*4; ++i) {
        a.c[i] -= b.c[i];
    }
    return a;
}

M4x4f m4x4f_scale(M4x4f m, float s)
{
    for(size_t i = 0; i < 4*4; ++i) {
        m.c[i] *= s;
    }
    return m;
}

M4x4f m4x4f_multiply(M4x4f a, M4x4f b)
{
    M4x4f mat = {0};
    for(size_t i = 0; i < 4*4; ++i) {
        float total = 0.0f;
        for(size_t j = 0; j < 4; ++j) {
            total += a.c[4*(i/4)+j] * b.c[(i%4)+4*j];
        }
        mat.c[i] = total;
    }
    return mat;
}
