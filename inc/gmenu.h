#pragma once
#include <gtk/gtk.h>
#include <dialogs.h>
/*
* @brief Init the maenu bar
* @param Pointer to the parent widget
*/
void gmenu_init(GtkWindow *parentWindow, GtkWidget *parentLayout);
/*
* @brief close operation to clear all memory garbage
*/
void gmenu_close_operation();