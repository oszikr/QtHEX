#include "hexstrategycontrol.h"

HexStrategyControl::HexStrategyControl()
{

}

short int HexStrategyControl::getWinningStep() const
{
    //std::cout << ">>> Searching winning strategy: ";
    // level 0
    for (unsigned short int i = 0; i < hex.getLength(); i++) {
            if (hex.get(i) == HexStateSpace::EMPTY) {
                HexStateSpace nextHex(hex);
                //nextHex.set(BLUE/RED);
                //short int result = recursiveAnalisys(nextHex, 1);
                //if (result == 1) {
                    // if I find a winning strategy, return it
                    //return i;
                //}
            }
            std::cout << "|";
    }
    std::cout << std::endl;
    return -1;
}

/*
public int analisys(Chomp curChomp, int level) {
    // System.out.println("> " + getNSpaces(level) + info);

    // The tixic fiels is the last
    if (curChomp.getActiveFields() == 1) {
        if (level % 2 == 0) {// A player's level
            return B;
        } else {// B player's level
            return A;
        }
    }

    // no more fields
    if (curChomp.getActiveFields() == 0) {
        if (level % 2 == 0) {// A player's level
            return A;
        } else {// B player's level
            return B;
        }
    }

    // find special states
    int heur = getAdvancedStates(curChomp);
    if (heur > 0) {
        if (level % 2 == 0) {// A player's level
            return A;
        } else {// B player's level
            return B;
        }
    }

    // recursion
    if (level % 2 == 0) {// A player's level
        for (int i = 0; i < curChomp.getWidth(); i++) {
            for (int j = 0; j < curChomp.getHeight(); j++) {
                if (curChomp.getStateMap()[i][j]) {
                    Chomp nextChomp = new Chomp(curChomp);
                    nextChomp.action(i, j);
                    int res = analisys(nextChomp, level + 1);
                    if (res == A) {
                        return A;
                    }
                }
            }
        }
        return B;
    } else {// B player's level
        for (int i = 0; i < curChomp.getWidth(); i++) {
            for (int j = 0; j < curChomp.getHeight(); j++) {
                if (curChomp.getStateMap()[i][j]) {
                    Chomp nextChomp = new Chomp(curChomp);
                    nextChomp.action(i, j);
                    int res = analisys(nextChomp, level + 1);
                    if (res == B) {
                        return B;
                    }
                }
            }
        }
        return A;
    }
}

*/
