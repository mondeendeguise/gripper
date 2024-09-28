#include "transforms.h"
#include "vector.h"
#include "matrix.h"

#include <math.h>
#include <stddef.h>

float degrees_to_radians(float degrees)
{
    return degrees * (M_PI/180);
}

V4f v4f_m4x4f_product(M4x4f mat, V4f vec)
{
    V4f a = {0};
    for(size_t i = 0; i < 4; ++i) {
        float total = 0.0f;
        for(size_t j = 0; j < 4; ++j) {
            total += mat.c[(i*4)+j] * vec.c[i];
        }
        a.c[i] = total;
    }
    return a;
}

M4x4f m4x4f_translate_xyz(float x, float y, float z)
{
    return (M4x4f) {
        .c = {
            1.0f, 0.0f, 0.0f, x,
            0.0f, 1.0f, 0.0f, y,
            0.0f, 0.0f, 1.0f, z,
            0.0f, 0.0f, 0.0f, 1.0f,
        }
    };
}

M4x4f m4x4f_translate_v3f(V3f v)
{
    return m4x4f_translate_xyz(v.c[0], v.c[1], v.c[2]);
}

M4x4f m4x4f_scale_xyz(float x, float y, float z)
{
    return (M4x4f) {
        .c = {
            x, 0.,0.,0.,
            0.,y, 0.,0.,
            0.,0.,z, 0.,
            0.,0.,0.,1.,
        }
    };
}

M4x4f m4x4f_scale_v3f(V3f v)
{
    return m4x4f_scale_xyz(v.c[0], v.c[1], v.c[2]);
}

M4x4f m4x4f_rotation_x(float rads)
{
    return (M4x4f) {
        .c = {
            1.0f,          0.0f,          0.0f,          0.0f,
            0.0f,          cosf(rads),   -sinf(rads),    0.0f,
            0.0f,          sinf(rads),    cosf(rads),    0.0f,
            0.0f,          0.0f,          0.0f,          1.0f,
        },
    };
}

M4x4f m4x4f_rotation_y(float rads)
{
    return (M4x4f) {
        .c = {
            cosf(rads),    0.0f,          sinf(rads),    0.0f,
            0.0f,          1.0f,          0.0f,          0.0f,
           -sinf(rads),    0.0f,          cosf(rads),    0.0f,
            0.0f,          0.0f,          0.0f,          1.0f,
        },
    };
}

M4x4f m4x4f_rotation_z(float rads)
{
    return (M4x4f) {
        .c = {
            cosf(rads),   -sinf(rads),    0.0f,          0.0f,
            sinf(rads),    cosf(rads),    0.0f,          0.0f,
            0.0f,          0.0f,          1.0f,          0.0f,
            0.0f,          0.0f,          0.0f,          1.0f,
        },
    };
}
