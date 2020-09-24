#ifndef HEXMINMAXCONTROL_H
#define HEXMINMAXCONTROL_H
#include "hexstatespace.h"

class HexMinMaxControl
{
public:
    HexMinMaxControl(const HexStateSpace& hex, const HexStateSpace::color& player, const unsigned short limit);
    short int getWinningStep() const;
    short int minMaxRecursion(const HexStateSpace &curHex, const HexStateSpace::color& curPlayer, const unsigned int& curLevel) const;

private:
    const HexStateSpace hex;
    const HexStateSpace::color player; // the player who search the winning strategy
    const unsigned short int limit;
};

#endif // HEXMINMAXCONTROL_H
