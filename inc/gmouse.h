#pragma once
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <gtk/gtk.h>
#include <cglm/cglm.h>

void gmouse_init_mouse_events(GtkWidget *window, vec2 *buffer);
void gmouse_get_movement_offset(float *x, float *y);