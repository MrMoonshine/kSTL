#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include <QMenuBar>
#include <QObject>
#include <QWidget>
#include <QIcon>

class MainMenu : public QMenuBar
{
    Q_OBJECT
public:
    MainMenu();
    ~MainMenu() override;
private:
    QMenu *fileMenu, *settingsMenu, *aboutMenu;
    QAction *fileOpen;
    QAction *settingsColour;
    QAction *aboutAbout, *aboutHelp;
};

#endif // MAINMENU_HPP
