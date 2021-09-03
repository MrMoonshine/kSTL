#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <errno.h>

extern int errno;
#define STL_HEADER_SIZE 80
#define STL_VERTEX_SIZE 50
#define STL_VERTEX_FLOAT_COUNT 9

#define STL_MOD_POS_X 0
#define STL_MOD_POS_Y 1
#define STL_MOD_POS_Z 2

struct MetaSTL{
    unsigned int vertexCount;
    float xmin, xmax;
    float ymin, ymax;
    float zmin, zmax;
};

int stl_model_init(const char *filename, float* vertices, size_t* vertexSize, float* normals, size_t *normalSize);
int stl_meta(struct MetaSTL *meta, float* vertices, size_t vertexSize);

#ifdef __cplusplus
} // extern "C"
#endif
