#pragma once
#include <gdk/gdk.h>
#include <GL/glew.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

#define GL_WRAPPER_GLSL_MAX_ROW_LENGTH 256

GLuint glshader_load(const char* vtxshdrf, const char* frgmntshdrf);