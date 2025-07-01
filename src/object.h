#ifndef OBJECT_H_
#define OBJECT_H_

#include "gmath/vector.h"
#include "gmath/quaternion.h"

typedef struct {
    V3f position;
    Quaternion orientation;
} Object3D;

#endif // OBJECT_H_
