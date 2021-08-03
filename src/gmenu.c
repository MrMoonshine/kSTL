#include <gmenu.h>

static GtkWidget *quitMi;

void gmenu_close_operation(){
    glwrap_cleanup();
    printf("[kSTL] Goodbye <3");
    gtk_main_quit();
}

void gmenu_init(GtkWidget *parentLayout){
    GtkWidget *menubar;
    /*----------------------------------------*/
    /*             File Menu                  */
    /*----------------------------------------*/
    GtkWidget *fileMenu;
    GtkWidget *fileMi;
    GtkWidget *openMi;
    /*----------------------------------------*/
    /*            Settings                    */
    /*----------------------------------------*/
    GtkWidget *settingsMenu;
    GtkWidget *settingsMi;
    GSList *measurement_units = NULL;
    GtkWidget *unitsMicroMi;
    GtkWidget *unitsMilliMi;
    GtkWidget *unitsDefaultMi;
    /*----------------------------------------*/
    /*            About Menu                  */
    /*----------------------------------------*/
    GtkWidget *helpMenu;
    GtkWidget *helpMi;
    GtkWidget *guideMi;
    GtkWidget *aboutMi;

    menubar = gtk_menu_bar_new();

    fileMenu = gtk_menu_new();
    settingsMenu = gtk_menu_new();
    helpMenu = gtk_menu_new();
    
    //File
    fileMi = gtk_image_menu_item_new_from_stock(GTK_STOCK_FILE, NULL);
    openMi = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN, NULL);
    quitMi = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, NULL);
    //Settings
    settingsMi = gtk_image_menu_item_new_from_stock(GTK_STOCK_EDIT, NULL);
    unitsMicroMi = gtk_radio_menu_item_new_with_mnemonic(measurement_units, "_µm");
    measurement_units = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (unitsMicroMi));
    unitsMilliMi = gtk_radio_menu_item_new_with_mnemonic(measurement_units, "_mm");
    measurement_units = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (unitsMilliMi));
    unitsDefaultMi = gtk_radio_menu_item_new_with_mnemonic(measurement_units, "_m");
    measurement_units = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (unitsDefaultMi));
    gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (unitsMilliMi), TRUE);
    //Help
    helpMi = gtk_image_menu_item_new_from_stock(GTK_STOCK_HELP, NULL);
    guideMi = gtk_image_menu_item_new_from_stock(GTK_STOCK_HELP, NULL);
    aboutMi = gtk_image_menu_item_new_from_stock(GTK_STOCK_ABOUT, NULL);

    //Add Sub Menus
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi), fileMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), openMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), quitMi);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(settingsMi), settingsMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(settingsMenu), unitsMicroMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(settingsMenu), unitsMilliMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(settingsMenu), unitsDefaultMi);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(helpMi), helpMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(helpMenu), aboutMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(helpMenu), guideMi);
    
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), settingsMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), helpMi);
    gtk_box_pack_start(GTK_BOX(parentLayout), menubar, FALSE, FALSE, 0);

    g_signal_connect(
        G_OBJECT(quitMi),
        "activate",
        G_CALLBACK(gmenu_close_operation),
        NULL
    );
}

GtkWidget * gmenu_get_quit_button(){
    return quitMi;
}