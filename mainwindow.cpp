#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    hexTable = new HexTable(this);
    show();
}

MainWindow::~MainWindow()
{
}
