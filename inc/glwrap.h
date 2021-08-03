#pragma once
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Include GLEW
#include <GL/glew.h>
#include <gtk/gtk.h>

#include <glshader.h>

int glwrap_init_gl(GtkWidget *parentLayout);
int glwrap_cleanup();