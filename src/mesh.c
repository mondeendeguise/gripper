#include "mesh.h"
#include "vector.h"

Vertex vertex(V3f pos, V4f color)
{
    Vertex a = {0};
    a.pos = pos;
    a.color = color;
    return a;
}
