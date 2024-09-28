#ifndef CAMERA_H_
#define CAMERA_H_

#include "vector.h"
#include "matrix.h"

M4x4f m4x4f_look_at(V3f eye, V3f target, V3f up);

M4x4f m4x4f_ortho(float left, float right,
                  float bottom, float top,
                  float near, float far);

M4x4f m4x4f_perspective(float near, float far);

M4x4f m4x4f_projection(float aspect_ratio, float fov,
                       float znear, float zfar);

V4f m4x4f_v4f_project(M4x4f m, V4f v);

#endif // CAMERA_H_
