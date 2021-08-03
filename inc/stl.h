#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Include GLEW
#include <GL/glew.h>

void stl_recon();

int stl_load_ascii(const char* filename, size_t* buffsize, GLfloat* data);