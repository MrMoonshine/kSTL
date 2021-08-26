#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <errno.h>

extern int errno;
#define STL_HEADER_SIZE 80
#define STL_VERTEX_SIZE 50
#define STL_VERTEX_FLOAT_COUNT 9

#define STL_GL_10_BITS 10
#define STL_GL_10_MAX ((2 << (STL_GL_10_BITS - 1))-1)

int stl_model_init(const char *filename, float* vertices, size_t* vertexSize, int32_t* normals, size_t *normalSize);

#ifdef __cplusplus
} // extern "C"
#endif
