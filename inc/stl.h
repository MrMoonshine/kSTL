#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Include GLEW
#include <GL/glew.h>

#include <math.h>
#include <cglm/cglm.h>
#include <errno.h>

extern int errno;

#define STL_HEADER_SIZE 80
#define STL_VERTEX_SIZE 50
#define STL_VERTEX_FLOAT_COUNT 9

#define STL_GL_10_BITS 10
#define STL_GL_10_MAX ((2 << (STL_GL_10_BITS - 1))-1)

void stl_recon();

void stl_model_init(GLuint* vbo, GLuint *normals, GLuint *vertexCount, const char *filename);
void stl_model_draw(GLuint vbo, GLuint normals, GLuint vertexCount);