#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <cstdlib>
#include <climits>
#include <cmath>

#include "hexstatespace.h"

/*int main(int, char**)
{
    unsigned long long l = 0; //ULLONG_MAX;
    for(int i = 0; i < (11*11); ++i)
    {
        unsigned long long prev = l;
        l += 2 * std::pow(3, i);
        std::cout << i << "\t" << l << std::endl;
        if(prev > l)
        {
            std::cout << "max\t" << ULLONG_MAX << std::endl;
            return -1;
        }
    }
    return 0;
}

/*int main(int, char**)
{
    std::srand(time(NULL));
    hexStateSpace h;
    hexStateSpace::color winner;

    do
    {
        static int count = 0;
        count++;
        std::cout << ">>> ROUND: " << count << std::endl;
        unsigned short int rand = std::rand() % (11*11);
        std::cout << ">>> RANDOM NUMBER: " << rand << std::endl;
        if(count % 2 == 0)
            h.set(rand, hexStateSpace::RED);
        else
            h.set(rand, hexStateSpace::BLUE);
        std::cout << ">>> CURRENT STATESPECE: " << std::endl << h;
        winner = h.winner();
        std::cout << ">>> THE WINNER IS: "  << winner << std::endl;
        std::cout << "-----------------------------------" << std::endl;
    }
    while(winner == hexStateSpace::EMPTY);

    return 0;
}*/

/*
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
    hc.setStateSpace(h.space, h.SIZE, 27);
    hc.show();

    return a.exec();
}
*/
