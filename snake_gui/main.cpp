/* snake_gui
 * Description main.cpp:
 *  This file only calls MainWindow, where the project is implemented in.
 *
 * * Program author:
 * Name: Timo Virtanen
 * */
#include "mainwindow.hh"
#include <iostream>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
