#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <cstdlib>

#include "hexstatespace.h"

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
