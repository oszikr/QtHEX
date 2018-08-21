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

    hexStateSpace h;
    hexStateSpace::color winner;

    HexCanvas hc;
    hc.setStateSpace(h.space, h.SIZE, 27);
    hc.show();

    return a.exec();
}
