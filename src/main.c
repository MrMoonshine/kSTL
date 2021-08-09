#include <gtk/gtk.h>
#include <gmenu.h>
#include <glwrap.h>
#include <stl.h>

int main(int argc, char *argv[]) {
    //stl_recon();
    GtkWidget *window;
    GtkWidget *vbox;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), APP_WIDTH, APP_HEIGHT);
    gtk_window_set_title(GTK_WINDOW(window), APP_TITLE);

    g_signal_connect(G_OBJECT(window), "destroy",
        G_CALLBACK(gtk_main_quit), NULL);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    gtk_widget_add_events(window, 0);
    g_signal_connect(
        G_OBJECT(window),
        "destroy",
        G_CALLBACK(gmenu_close_operation),
        NULL
    );

    gmenu_init(vbox);
    glwrap_init_gl(
        argc > 1 ? argv[1] : "",
        window,
        vbox
    );

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}