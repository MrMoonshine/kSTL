#include <dialogs/colour.h>

void dialog_colour_init(GtkWidget *widget){
    widget = gtk_color_selection_new();
    gtk_color_selection_set_has_opacity_control(widget, false);
}

void dialog_colour_show(GtkWidget *widget){
    GtkColorSelectionDialog colorsel;
    GtkWidget *dialog;
    dialog = gtk_color_selection_dialog_new("Oida");
    //gtk_window_set_title(GTK_WINDOW(dialog), "Question");
    gtk_dialog_run(GTK_DIALOG(dialog));
    
    gtk_widget_destroy(dialog);
}