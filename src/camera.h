#ifndef CAMERA_H_
#define CAMERA_H_

#include "vector.h"
#include "matrix.h"

M4x4f m4x4f_look_at(V3f eye, V3f target, V3f up);

M4x4f m4x4f_ortho(float left, float right,
                  float bottom, float top,
                  float near, float far);

M4x4f m4x4f_projection(float aspect_ratio, float fov,
                       float near_plane, float far_plane);

#endif // CAMERA_H_
