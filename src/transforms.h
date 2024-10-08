#ifndef TRANSFORMS_H_
#define TRANSFORMS_H_

#include "vector.h"
#include "matrix.h"

float degrees_to_radians(float degrees);

V4f v4f_m4x4f_product(M4x4f mat, V4f vec);

M4x4f m4x4f_translate_xyz(float x, float y, float z);
M4x4f m4x4f_translate_v3f(V3f v);

M4x4f m4x4f_scale_xyz(float x, float y, float z);
M4x4f m4x4f_scale_v3f(V3f v);

M4x4f m4x4f_rotate_x(float rads);
M4x4f m4x4f_rotate_y(float rads);
M4x4f m4x4f_rotate_z(float rads);

#endif // TRANSFORMS_H_
