#include <mainwindow.hpp>

#include <QApplication>
#include <iostream>

#include <KLocalizedContext>
#include <KLocalizedString>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow* window = new MainWindow(); 
    window->show();
    return a.exec();
}
