#include <gmenu.h>

void gmenu_init(GtkWidget *parentLayout){
    GtkWidget *menubar;
    /*----------------------------------------*/
    /*             File Menu                  */
    /*----------------------------------------*/
    GtkWidget *fileMenu;
    GtkWidget *fileMi;
    GtkWidget *openMi;
    GtkWidget *quitMi;
    /*----------------------------------------*/
    /*            About Menu                  */
    /*----------------------------------------*/
    GtkWidget *helpMenu;
    GtkWidget *helpMi;
    GtkWidget *guideMi;
    GtkWidget *aboutMi;

    menubar = gtk_menu_bar_new();

    fileMenu = gtk_menu_new();
    helpMenu = gtk_menu_new();
    
    //File
    fileMi = gtk_image_menu_item_new_from_stock(GTK_STOCK_FILE, NULL);
    openMi = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN, NULL);
    quitMi = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, NULL);
    //Help
    helpMi = gtk_image_menu_item_new_from_stock(GTK_STOCK_HELP, NULL);
    guideMi = gtk_image_menu_item_new_from_stock(GTK_STOCK_HELP, NULL);
    aboutMi = gtk_image_menu_item_new_from_stock(GTK_STOCK_ABOUT, NULL);

    //Add Sub Menus
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi), fileMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), openMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), quitMi);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(helpMi), helpMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(helpMenu), aboutMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(helpMenu), guideMi);
    
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), helpMi);
    gtk_box_pack_start(GTK_BOX(parentLayout), menubar, FALSE, FALSE, 0);

    //Connect Quit-Button with window-close 
    g_signal_connect(G_OBJECT(quitMi), "activate",
        G_CALLBACK(gtk_main_quit), NULL);
}