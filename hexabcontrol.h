#ifndef HEXABCONTROL_H
#define HEXABCONTROL_H
#include "hexstatespace.h"

class HexABcontrol
{
public:
    HexABcontrol(const HexStateSpace& hex, const HexStateSpace::color& A, const HexStateSpace::color& B);
    short int getWinningStep() const;
    HexStateSpace::color alphaBetaRecursion(HexStateSpace &curHex, const unsigned int& level) const;

private:
    const HexStateSpace hex;
    const HexStateSpace::color A; // the player who search the winning strategy
    const HexStateSpace::color B; // the other player
};

#endif // HEXABCONTROL_H
