#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <cstdlib>

#include <QApplication>
#include "mainwindow.h"
#include "hexstatespace.h"
#include "hexcanvas.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    HexStateSpace stateSpace; // Create hex game statespace.
    HexCanvas hc; // create canvas. board and buttons.
    hc.setStateSpace(&stateSpace); // bind statespace to canvas.
    hc.show();

    return a.exec();
}
