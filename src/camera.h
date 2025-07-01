#ifndef CAMERA_H_
#define CAMERA_H_

#include "gmath/vector.h"
#include "gmath/matrix.h"
#include "gmath/quaternion.h"

typedef struct {
    V3f position;
    Quaternion orientation;
} Camera;

#endif // CAMERA_H_
