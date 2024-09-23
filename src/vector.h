#ifndef VECTOR_H_
#define VECTOR_H_

typedef struct {
    float c[2];
} V2f;

typedef struct {
    float c[3];
} V3f;

typedef struct {
    float c[4];
} V4f;

V2f v2f(float x, float y);
void v2f_print(V2f a);

V3f v3f(float x, float y, float z);
void v3f_print(V3f a);

V4f v4f(float x, float y, float z, float w);
void v4f_print(V4f a);

#endif // VECTOR_H_
