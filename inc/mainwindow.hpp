#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP
#include <QObject>
#include <QWidget>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#include <mainmenu.hpp>
#include <metawidget.hpp>

class MainWindow : public QMainWindow{
    Q_OBJECT
    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow() override;
    private:

    public:
    private:
        QHBoxLayout hlay;
        QVBoxLayout toollay;
        QWidget contents;
        MainMenu menu;
        MetaWidget meta;
        QLabel renderSpace;
};
#endif
