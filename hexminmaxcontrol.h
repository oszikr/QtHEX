#ifndef HEXMINMAXCONTROL_H
#define HEXMINMAXCONTROL_H
#include "hexstatespace.h"

class HexMinMaxControl
{
public:
    HexMinMaxControl(const HexStateSpace& hex, const HexStateSpace::color& A, const HexStateSpace::color& B, unsigned short limit);
    short int getWinningStep() const;
    HexStateSpace::color alphaBetaRecursion(HexStateSpace &curHex, const unsigned int& level) const;

private:
    const HexStateSpace hex;
    const HexStateSpace::color A; // the player who search the winning strategy
    const HexStateSpace::color B; // the other player
    const unsigned short int limit;
};

#endif // HEXMINMAXCONTROL_H
