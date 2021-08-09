#include <dialogs.h>

void dialog_colour_init(GtkWidget *widget){

}

void dialog_colour_show(GtkWidget *widget){
    GtkColorSelectionDialog colorsel;
    GtkWidget *dialog;
    dialog = gtk_color_selection_dialog_new("Oida");
    //gtk_window_set_title(GTK_WINDOW(dialog), "Question");
    gtk_dialog_run(GTK_DIALOG(dialog));
    
    gtk_widget_destroy(dialog);
}

void dialog_file_show(GtkWindow *parent){

}