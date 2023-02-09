/* snake_gui
 * Description main.cpp:
 *  This file only calls MainWindow, where the project is implemented in.
 *
 * * Program author:
 * Name: Timo Virtanen
 * Student number: 150769440
 * UserID: kktivi
 * E-Mail: timo.virtanen@tuni.fi
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
