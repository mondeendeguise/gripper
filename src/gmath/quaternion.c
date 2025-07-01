#include "quaternion.h"
#include "vector.h"
#include "matrix.h"

#include <math.h>

Quaternion quaternion_identity(void)
{
    return (Quaternion) {
        .x = 0.0f,
        .y = 0.0f,
        .z = 0.0f,
        .w = 1.0f,
    };
}

Quaternion quaternion_from_angle_axis(float angle, V3f axis)
{
    float s = sinf(angle / 2.0f);
    return (Quaternion) {
        .x = s * axis.c[0],
        .y = s * axis.c[1],
        .z = s * axis.c[2],
        .w = cosf(angle / 2.0f),
    };
}

Quaternion quaternion_square_magnitude(Quaternion q)
{
    return (Quaternion) {
        .x = q.x * q.x,
        .y = q.y * q.y,
        .z = q.z * q.z,
        .w = q.w * q.w,
    };
}

Quaternion quaternion_pow(Quaternion q, float p)
{
    return q;
}

Quaternion quaternion_scalar_divide(Quaternion q, float s)
{
    return (Quaternion) {
        .x = q.x / s,
        .y = q.y / s,
        .z = q.z / s,
        .w = q.w / s,
    };
}

Quaternion quaternion_conjugate(Quaternion q)
{
    return (Quaternion) {
        .x = -q.x,
        .y = -q.y,
        .z = -q.z,
        .w = q.w
    };
}

Quaternion quaternion_inverse(Quaternion q)
{
    Quaternion c = quaternion_conjugate(q);
    Quaternion s = quaternion_square_magnitude(q);
    return (Quaternion) {
        .x = c.x / s.x,
        .y = c.y / s.y,
        .z = c.z / s.z,
        .w = c.w / s.w,
    };
}

Quaternion quaternion_combine(Quaternion a, Quaternion b)
{
    return (Quaternion) {
        /*
            w_a w_b - x_a x_b - y_a y_b - z_a z_b
        + ( w_a x_b + x_a w_b + y_a z_b - z_a y_b ) i
        + ( w_a y_b - x_a z_b + y_a w_b + z_a x_b ) j
        + ( w_a z_b + x_a y_b - y_a x_b + z_a w_b ) k
        */

        .w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
        .x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
        .y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
        .z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,

        // .x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
        // .y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
        // .z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.x,
        // .w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
    };
}

Quaternion quaternion_transform_vector(Quaternion q, V3f v)
{
    Quaternion k = (Quaternion) { v.c[0], v.c[1], v.c[2], 0.0f };
    Quaternion qn = quaternion_conjugate(q);
    return quaternion_combine(quaternion_combine(q, k), qn);
}

Quaternion quaternion_extrinsic_a_to_b(Quaternion a, Quaternion b)
{
    return quaternion_combine(b, quaternion_inverse(a));
}

Quaternion quaternion_intrinsic_a_to_b(Quaternion a, Quaternion b)
{
    return quaternion_combine(quaternion_inverse(a), b);
}

Quaternion quaternion_slerp(Quaternion a, Quaternion b, float t)
{
    return quaternion_combine(a, quaternion_pow(quaternion_intrinsic_a_to_b(a, b), t));
}

M4x4f quaternion_to_rotation_matrix(Quaternion q)
{
    return (M4x4f) {
        .c = {
            // 1 - 2 * (q.y * q.y + q.z * q.z), 2 * (q.x * q.y + q.z * q.w), 2 * (q.x * q.z + q.y * q.w), 0.0f,
            // 2 * (q.x * q.y + q.z * q.w), 1 - 2 * (q.x * q.x + q.z * q.z), 2 * (q.y * q.z - q.x * q.w), 0.0f,
            // 2 * (q.x * q.z - q.y * q.w), 2 * (q.y * q.z + q.x * q.w), 1 - 2 * (q.x * q.x + q.y * q.y), 0.0f,
            // 0.0f, 0.0f, 0.0f, 1.0f,

            q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z, 2 * ((q.x * q.y) - (q.w * q.z)), 2 * ((q.x * q.z + q.w * q.y)), 0.0f,
            2 * ((q.x * q.y) + (q.w * q.z)), q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z, 2 * ((q.y * q.z) - (q.w * q.x)), 0.0f,
            2 * ((q.x * q.z) - (q.w * q.y)), 2 * ((q.y * q.z) + (q.w * q.x)), q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
        },
    };
}
