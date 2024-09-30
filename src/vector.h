#ifndef VECTOR_H_
#define VECTOR_H_

typedef struct {
    unsigned int c[3];
} V3u;

typedef struct {
    float c[2];
} V2f;

typedef struct {
    float c[3];
} V3f;

typedef struct {
    float c[4];
} V4f;

V3u v3u(unsigned int x, unsigned int y, unsigned int z);

V2f v2f(float x, float y);
V2f v2f_add(V2f a, V2f b);
V2f v2f_subtract(V2f a, V2f b);
V2f v2f_multiply(V2f a, V2f b);
V2f v2f_divide(V2f a, V2f b);
V2f v2f_normalize(V2f v);
float v2f_dot_product(V2f a, V2f b);
void v2f_print(V2f a);

V3f v3f(float x, float y, float z);
V3f v3ff(float x);
V3f v3f_add(V3f a, V3f b);
V3f v3f_subtract(V3f a, V3f b);
V3f v3f_normalize(V3f v);
V3f v3f_cross_product(V3f a, V3f b);
float v3f_dot_product(V3f a, V3f b);
void v3f_print(V3f a);

V4f v4f(float x, float y, float z, float w);
V4f v4f_add(V4f a, V4f b);
V4f v4f_subtract(V4f a, V4f b);
V4f v4f_normalize(V4f v);
float v4f_dot_product(V4f a, V4f b);
void v4f_print(V4f a);

#endif // VECTOR_H_
