#pragma once
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Include GLEW
#include <GL/glew.h>
#include <glshader.h>

#include <math.h>
#include <cglm/cglm.h>

#define GIZMO_VERTEX_COUNT 12
/* 
 * TODO: Generate a three planes in avertexbuffer.
 * Generate a Circle in the shader by using gl_tex_coord[0].
 * The Radius must alter accordingly to the model's dimensions
*/
void gizmo_init();
void gizmo_delete();
void gizmo_draw(GLuint radius);