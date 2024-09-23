#include "vector.h"

#include <stdio.h>

inline V2f v2f(float x, float y)
{
    V2f a = {0};
    a.c[0] = x;
    a.c[1] = y;
    return a;
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

void v4f_print(V4f a)
{
    for(size_t i = 0; i < 4; ++i) {
        printf("%f ", a.c[i]);
    }
    printf("\n");
}
