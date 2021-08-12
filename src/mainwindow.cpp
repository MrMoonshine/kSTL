#include <mainwindow.hpp>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    menu(),
    meta(),
    renderSpace("Here comes the Vulkan doo doo doo do")
{
    //Use the Menu from the other class
    setMenuBar(&menu);
    //Add toolbar widgets
    toollay.addWidget(&meta, Qt::AlignTop);
    //add toolbar to main layout
    hlay.addLayout(&toollay);
    //apply colour to the test widget
    renderSpace.setStyleSheet("background-color: orange");
    //add tentative "render" space
    hlay.addWidget(&renderSpace);
    //Set the horizontal layout for container widget
    contents.setLayout(&hlay);
    //Use content widget to display on mainwindow
    setCentralWidget(&contents);
    //Set App dimensions
    resize(DEFAULT_APP_WIDTH, DEFAULT_APP_HEIGHT);
}

MainWindow::~MainWindow(){

};
