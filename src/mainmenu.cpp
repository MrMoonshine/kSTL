#include <mainmenu.hpp>

MainMenu::MainMenu() : QMenuBar(){
      /*------------------------------------*/
     /*            File Menu               */
    /*------------------------------------*/
    fileMenu = addMenu(tr("&File"));
    fileOpen = fileMenu->addAction(QIcon::fromTheme("folder-open"),tr("&Open"));

      /*------------------------------------*/
     /*           Settings Menu            */
    /*------------------------------------*/
    settingsMenu = addMenu(tr("&Settings"));
    settingsColour = settingsMenu->addAction(QIcon::fromTheme("preferences-desktop-theme"),tr("&Color"));
      /*------------------------------------*/
     /*           Settings Menu            */
    /*------------------------------------*/
    aboutMenu = addMenu(tr("&About"));
    aboutAbout = aboutMenu->addAction(QIcon::fromTheme("help-about"),tr("&About"));
    aboutHelp = aboutMenu->addAction(QIcon::fromTheme("help-faq"),tr("&Help"));
}

MainMenu::~MainMenu(){
    delete fileOpen;
    delete settingsColour;
    delete aboutAbout;
    delete aboutHelp;
    //Delete Menus
    delete fileMenu;
    delete settingsMenu;
    delete aboutMenu;
}
