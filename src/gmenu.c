#include <gmenu.h>
#include <stdio.h>

GtkWindow *parentWindow;
/*----------------------------------------*/
/*             File Menu                  */
/*----------------------------------------*/
static GtkWidget *quitMi;
static GtkWidget *openMi;
/*----------------------------------------*/
/*             Dialogs                    */
/*----------------------------------------*/
static GtkWidget *fileDialog;

static void dialog_open_file(){
    fileDialog = gtk_file_chooser_dialog_new ("Select STL Mesh",
                                      parentWindow,
                                      GTK_FILE_CHOOSER_ACTION_OPEN,
                                      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                      GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                      NULL
    );

    GtkFileFilter *filter;
    filter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter, "*.stl");
    gtk_file_filter_add_pattern(filter, "*.ast");
    gtk_file_filter_set_name(filter, "Stereolitography Meshes (.stl, .ast)");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(fileDialog), filter);

    if (gtk_dialog_run (GTK_DIALOG (fileDialog)) == GTK_RESPONSE_ACCEPT){
        char *filename;

        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (fileDialog));
        //printf("Selected file: %s\n", filename);
        glwrap_load_mesh(filename);
        g_free (filename);
    }
    gtk_widget_destroy (fileDialog);
}

static void connect_callbacks(){
    g_signal_connect(
    G_OBJECT(openMi),
    "activate",
    G_CALLBACK(dialog_open_file),
    NULL
    );
}

void gmenu_close_operation(){
    glwrap_cleanup();
    printf("[kSTL] Goodbye <3");
    gtk_main_quit();
}

void gmenu_init(GtkWindow *parent_window, GtkWidget *parentLayout){
    parentWindow = parent_window;
    GtkWidget *menubar;
    /*----------------------------------------*/
    /*             File Menu                  */
    /*----------------------------------------*/
    GtkWidget *fileMenu;
    GtkWidget *fileMi;
    /*----------------------------------------*/
    /*            Settings                    */
    /*----------------------------------------*/
    GtkWidget *settingsMenu;
    GtkWidget *settingsMi;
    GtkWidget *colourPickerMi;
    GtkWidget *separator1Mi;
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
    colourPickerMi = gtk_image_menu_item_new_from_stock(GTK_STOCK_SELECT_COLOR, NULL);
    separator1Mi = gtk_separator_menu_item_new();
    settingsMi = gtk_image_menu_item_new_from_stock(GTK_STOCK_PROPERTIES, NULL);
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
    aboutMi = gtk_image_menu_item_new_from_stock(GTK_STOCK_INFO, NULL);

    //Add Sub Menus
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi), fileMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), openMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), quitMi);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(settingsMi), settingsMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(settingsMenu), colourPickerMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(settingsMenu), separator1Mi);
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

    g_signal_connect(
        G_OBJECT(colourPickerMi),
        "activate",
        G_CALLBACK(dialog_colour_show),
        NULL
    );

    connect_callbacks();
}

GtkWidget * gmenu_get_quit_button(){
    return quitMi;
}