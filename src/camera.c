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
#if 0
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
#else
    (void) eye;
    (void) target;
    (void) up;
    fprintf(stderr, "WARN: m4x4f_look_at not yet implemented\n");
    return m4x4f_diagonal(1.0f);
#endif // 0
}

M4x4f m4x4f_ortho(float left, float right,
                  float bottom, float top,
                  float near, float far)
{
    M4x4f trans = m4x4f_translation_xyz(-(right + left) / 2,
                                        -(bottom + top) / 2,
                                        -(near));

    M4x4f scale = m4x4f_scale_xyz(2 / (right-left),
                                  2 / (bottom - top),
                                  2 / (far - near));

    return m4x4f_multiply(scale, trans);
}

M4x4f m4x4f_projection(float aspect_ratio, float fov,
                       float near_plane, float far_plane)
{
    float fov_rad = 1.0f / tanf(degrees_to_radians(fov * 0.5f));
    return (M4x4f) {
        .c = {
            aspect_ratio * fov_rad,0.0f,0.0f,0.0f,
            0.0f,fov_rad,0.0f,0.0f,
            0.0f,0.0f,far_plane / (far_plane - near_plane),1.0f,
            0.0f,0.0f,(-far_plane * near_plane) / (far_plane - near_plane),0.0f,
        }
    };
}
