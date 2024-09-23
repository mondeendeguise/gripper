#ifndef MESH_H_
#define MESH_H_

#include "vector.h"

#include <stddef.h>

typedef enum {
    VA_POS = 0,
    /* VA_UV, */
    VA_COLOR,
    VA_COUNT,
} Vertex_Attrib;

typedef struct {
    V3f pos;
    /* V2f uv; */
    V4f color;
} Vertex;

typedef struct {
    Vertex *vertices;
    size_t *indices;
} Mesh;

Vertex vertex(V3f pos, V4f color);

#endif // MESH_H_
