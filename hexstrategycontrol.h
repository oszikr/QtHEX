#ifndef HEXSTRATEGYCONTROL_H
#define HEXSTRATEGYCONTROL_H
#include "hexstatespace.h"

class HexStrategyControl
{
public:
    HexStrategyControl();
    short int getWinningStep() const;

    HexStateSpace hex;
};

#endif // HEXSTRATEGYCONTROL_H
