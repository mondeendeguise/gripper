#include "camera.h"
#include "vector.h"
#include "matrix.h"
#include "transforms.h"

#include <math.h>
#include <stdio.h>

/*
M4x4f m4x4f_look_at(V3f eye, V3f target, V3f up)
{
    V3f f = v3f_normalize(v3f_subtract(eye, target)); // Forward
    V3f r = v3f_normalize(v3f_cross_product(up, f));  // Right
    V3f u = v3f_cross_product(f, r);                  // Up

    return (M4x4f) {
        .c = {
            r.c[0],u.c[0],f.c[0],0.0f,
            r.c[1],u.c[1],f.c[1],0.0f,
            r.c[2],u.c[2],f.c[2],0.0f,
            -v3f_dot_product(eye,r), -v3f_dot_product(eye,u), -v3f_dot_product(eye,f), 1.0f,
        }
    };
}
*/

M4x4f m4x4f_look_at(V3f eye, V3f target, V3f up)
{
    fprintf(stderr, "WARNING: m4x4f_look_at might be broken\n");
    V3f f = v3f_normalize(v3f_subtract(target, eye));
    V3f s = v3f_normalize(v3f_cross_product(f, up));
    V3f t = v3f_cross_product(s, f);

    return (M4x4f) {
        .c = {
            s.c[0],    s.c[1],    s.c[2],   0.0f,
            t.c[0],    t.c[1],    t.c[2],   0.0f,
           -f.c[0],   -f.c[1],   -f.c[2],   0.0f,
           -eye.c[0], -eye.c[1], -eye.c[2], 1.0f,
        }
    };
}

M4x4f m4x4f_ortho(float left, float right,
                  float bottom, float top,
                  float near, float far)
{
    /* M4x4f trans = m4x4f_translate_xyz(-(right + left) / 2, */
    /*                                     -(bottom + top) / 2, */
    /*                                     -(near)); */

    /* M4x4f scale = m4x4f_scale_xyz(2 / (right-left), */
    /*                               2 / (bottom - top), */
    /*                               2 / (far - near)); */

    /* return m4x4f_multiply(scale, trans); */

    return (M4x4f) {
        .c = {
            2/(right-left), 0.0f,           0.0f,         -(right+left)/(right-left),
            0.0f,           2/(bottom-top), 0.0f,         -(bottom+top)/(bottom-top),
            0.0f,           0.0f,           2/(far-near), -near/(far-near),
            0.0f,           0.0f,           0.0f,          1.0f,
        }
    };
}

M4x4f m4x4f_perspective(float near, float far)
{
    return (M4x4f) {
        .c = {
            near, 0.0f, 0.0f,      0.0f,
            0.0f, near, 0.0f,      0.0f,
            0.0f, 0.0f, far+near, -far*near,
            0.0f, 0.0f, 1.0f,      0.0f,
        }
    };
}

M4x4f m4x4f_projection(float aspect_ratio, float fov,
                       float znear, float zfar)
{
    float fov_rad = tanf(degrees_to_radians(fov) / 2.0f);
    return (M4x4f) {
        .c = {
            aspect_ratio * (1.0f / (fov_rad)),  0.0f,            0.0f,                   0.0f,
            0.0f,                               1.0f / fov_rad,  0.0f,                   0.0f,
            0.0f,                               0.0f,            zfar / (zfar - znear), -(zfar * znear) / (zfar - znear),
            0.0f,                               0.0f,            1.0f,                   0.0f,
        }
    };
}

V4f m4x4f_v4f_project(M4x4f m, V4f v)
{
    V4f result = v4f_m4x4f_product(m, v);
    if(result.c[3]) {
        result.c[0] /= result.c[3];
        result.c[1] /= result.c[3];
        result.c[2] /= result.c[3];
    }
    return result;
}
