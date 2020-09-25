#ifndef HEXMINMAXCONTROL_H
#define HEXMINMAXCONTROL_H
#include "hexstatespace.h"
#include <iostream>
#include <sstream>
#include <set>

class HexMinMaxControl
{
public:
    HexMinMaxControl(const HexStateSpace& hex, const HexStateSpace::color& A, const HexStateSpace::color& B, const unsigned short limit);
    std::vector<short int> getWinningStep() const;
    short int minMaxRecursion(HexStateSpace &curHex, const unsigned int& level, std::vector<short> &path) const;

private:
    const HexStateSpace hex;
    const HexStateSpace::color A; // the player who search the winning strategy
    const HexStateSpace::color B; // the other player
    const unsigned short int limit;

    std::string toString(std::vector<short int> &v) const;
};

#endif // HEXMINMAXCONTROL_H
