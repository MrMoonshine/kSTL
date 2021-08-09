#pragma once
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Include GLEW
#include <GL/glew.h>
#include <gtk/gtk.h>

#include <math.h>
#include <cglm/cglm.h>

#include <stl.h>
#include <glshader.h>
#include <gmouse.h>

#define GL_WRAP_SCROLL_FACTOR_GROW (6.0/5.0)
#define GL_WRAP_SCROLL_FACTOR_SHRINK (5.0/6.0)

/*typedef struct MetaGL{
    GLuint shader;

} MetaGL;*/

int glwrap_init_gl(const char* filename, GtkWidget *window, GtkWidget *parentLayout);
int glwrap_load_mesh(const char* filename);
int glwrap_cleanup();