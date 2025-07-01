#ifndef GMATH_QUATERNION_H_
#define GMATH_QUATERNION_H_

#include "vector.h"
#include "matrix.h"

typedef struct {
    float x, y, z, w;
} Quaternion;

Quaternion quaternion_identity(void);
Quaternion quaternion_from_angle_axis(float angle, V3f axis);

Quaternion quaternion_square_magnitude(Quaternion q);
Quaternion quaternion_pow(Quaternion q, float p);
Quaternion quaternion_scalar_divide(Quaternion q, float s);

Quaternion quaternion_conjugate(Quaternion q);  // q^*
Quaternion quaternion_inverse(Quaternion q);  // q^-1

Quaternion quaternion_combine(Quaternion a, Quaternion b);    // a b
Quaternion quaternion_transform_vector(Quaternion q, V3f v);  // q v q^-1

Quaternion quaternion_extrinsic_a_to_b(Quaternion a, Quaternion b); // b a^-1
Quaternion quaternion_intrinsic_a_to_b(Quaternion a, Quaternion b); // a^-1 b

Quaternion quaternion_slerp(Quaternion a, Quaternion b, float t);   // a (a^-1 b)^t

M4x4f quaternion_to_rotation_matrix(Quaternion q);

#endif // GMATH_QUATERNION_H_
