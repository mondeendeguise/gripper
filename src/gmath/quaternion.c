#include "vector.h"

#include <math.h>

typedef struct {
    float x, y, z, w;
} Quaternion;

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

Quaternion quatern_compose(Quaternion a, Quaternion b)
{
    return (Quaternion) {
        .x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
        .y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
        .z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.x,
        .w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
    };
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
