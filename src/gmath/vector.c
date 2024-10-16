#include "vector.h"

#include <math.h>
#include <stdio.h>

inline V3u v3u(unsigned int x, unsigned int y, unsigned int z)
{
    V3u a = {0};
    a.c[0] = x;
    a.c[1] = y;
    a.c[2] = z;
    return a;
}

inline V2f v2f(float x, float y)
{
    V2f a = {0};
    a.c[0] = x;
    a.c[1] = y;
    return a;
}

V2f v2f_add(V2f a, V2f b)
{
    V2f v = {0};
    for(size_t i = 0; i < 2; ++i) {
        v.c[i] = a.c[i] + b.c[i];
    }
    return v;
}

V2f v2f_subtract(V2f a, V2f b)
{
    V2f v = {0};
    for(size_t i = 0; i < 2; ++i) {
        v.c[i] = a.c[i] - b.c[i];
    }
    return v;
}

V2f v2f_multiply(V2f a, V2f b)
{
    V2f v = {0};
    for(size_t i = 0; i < 2; ++i) {
        v.c[i] = a.c[i] * b.c[i];
    }
    return v;
}

V2f v2f_divide(V2f a, V2f b)
{
    V2f v = {0};
    for(size_t i = 0; i < 2; ++i) {
        v.c[i] = a.c[i] / b.c[i];
    }
    return v;
}

V2f v2f_normalize(V2f v)
{
    float magnitude = sqrtf(v2f_dot_product(v, v));
    V2f normalized = {0};
    for(size_t i = 0; i < 2; ++i) {
        normalized.c[i] = v.c[i]/magnitude;
    }
    return normalized;
}

float v2f_dot_product(V2f a, V2f b)
{
    float total = 0.0f;
    for(size_t i = 0; i < 2; ++i) {
        total += a.c[i] * b.c[i];
    }
    return total;
}

void v2f_print(V2f a)
{
    for(size_t i = 0; i < 2; ++i) {
        printf("%f ", a.c[i]);
    }
    printf("\n");
}

inline V3f v3f(float x, float y, float z)
{
    V3f a = {0};
    a.c[0] = x;
    a.c[1] = y;
    a.c[2] = z;
    return a;
}

inline V3f v3ff(float x)
{
    V3f a = {0};
    a.c[0] = x;
    a.c[1] = x;
    a.c[2] = x;
    return a;
}

V3f v3f_add(V3f a, V3f b)
{
    V3f v = {0};
    for(size_t i = 0; i < 3; ++i) {
        v.c[i] = a.c[i] + b.c[i];
    }
    return v;
}

V3f v3f_subtract(V3f a, V3f b)
{
    V3f v = {0};
    for(size_t i = 0; i < 3; ++i) {
        v.c[i] = a.c[i] - b.c[i];
    }
    return v;
}

V3f v3f_multiply(V3f a, V3f b)
{
    V3f v = {0};
    for(size_t i = 0; i < 3; ++i) {
        v.c[i] = a.c[i] * b.c[i];
    }
    return v;
}

V3f v3f_divide(V3f a, V3f b)
{
    V3f v = {0};
    for(size_t i = 0; i < 3; ++i) {
        v.c[i] = a.c[i] / b.c[i];
    }
    return v;
}

V3f v3f_normalize(V3f v)
{
    float magnitude = sqrtf(v3f_dot_product(v, v));
    V3f normalized = {0};
    for(size_t i = 0; i < 3; ++i) {
        normalized.c[i] = v.c[i]/magnitude;
    }
    return normalized;
}

V3f v3f_cross_product(V3f a, V3f b)
{
    return v3f(a.c[1] * b.c[2] - a.c[2] * b.c[1],
               a.c[2] * b.c[0] - a.c[0] * b.c[2],
               a.c[0] * b.c[1] - a.c[1] * b.c[0]);
}

float v3f_dot_product(V3f a, V3f b)
{
    float total = 0.0f;
    for(size_t i = 0; i < 3; ++i) {
        total += a.c[i] * b.c[i];
    }
    return total;
}

void v3f_print(V3f a)
{
    for(size_t i = 0; i < 3; ++i) {
        printf("%f ", a.c[i]);
    }
    printf("\n");
}

inline V4f v4f(float x, float y, float z, float w)
{
    V4f a = {0};
    a.c[0] = x;
    a.c[1] = y;
    a.c[2] = z;
    a.c[3] = w;
    return a;
}

V4f v4f_add(V4f a, V4f b)
{
    V4f v = {0};
    for(size_t i = 0; i < 4; ++i) {
        v.c[i] = a.c[i] + b.c[i];
    }
    return v;
}

V4f v4f_subtract(V4f a, V4f b)
{
    V4f v = {0};
    for(size_t i = 0; i < 4; ++i) {
        v.c[i] = a.c[i] - b.c[i];
    }
    return v;
}

V4f v4f_multiply(V4f a, V4f b)
{
    V4f v = {0};
    for(size_t i = 0; i < 4; ++i) {
        v.c[i] = a.c[i] * b.c[i];
    }
    return v;
}

V4f v4f_divide(V4f a, V4f b)
{
    V4f v = {0};
    for(size_t i = 0; i < 4; ++i) {
        v.c[i] = a.c[i] / b.c[i];
    }
    return v;
}

V4f v4f_normalize(V4f v)
{
    float magnitude = sqrtf(v4f_dot_product(v, v));
    V4f normalized = {0};
    for(size_t i = 0; i < 4; ++i) {
        normalized.c[i] = v.c[i]/magnitude;
    }
    return normalized;
}

float v4f_dot_product(V4f a, V4f b)
{
    float total = 0.0f;
    for(size_t i = 0; i < 4; ++i) {
        total += a.c[i] * b.c[i];
    }
    return total;
}

void v4f_print(V4f a)
{
    for(size_t i = 0; i < 4; ++i) {
        printf("%f ", a.c[i]);
    }
    printf("\n");
}
