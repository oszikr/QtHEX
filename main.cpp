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

    std::srand(time(0));
    hexStateSpace h;
    hexStateSpace::color winner;

    int count = 0;
    do
    {
        count++;
        //std::cout << ">>> ROUND: " << count << std::endl;
        unsigned short int rand = std::rand() % (11*11);
        //std::cout << ">>> RANDOM NUMBER: " << rand << std::endl;
        if(count % 2 == 0)
            h.set(rand, hexStateSpace::RED);
        else
            h.set(rand, hexStateSpace::BLUE);
        //std::cout << ">>> CURRENT STATESPECE: " << std::endl << h;
        winner = h.winner();
        //std::cout << ">>> THE WINNER IS: "  << winner << std::endl;
        //std::cout << "-----------------------------------" << std::endl;
    }
    while(winner == hexStateSpace::EMPTY);

    std::cout << ">>> ROUND: " << count << std::endl;
    std::cout << ">>> CURRENT STATESPECE: " << std::endl << h;
    winner = h.winner();
    std::cout << ">>> THE WINNER IS: "  << winner << std::endl;

    HexCanvas hc;
    hc.setStateSpace(h.space, h.SIZE, 21);
    hc.show();

    return a.exec();
}
