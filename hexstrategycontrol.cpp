#include "hexstrategycontrol.h"

HexStrategyControl::HexStrategyControl(const HexStateSpace& hex, const HexStateSpace::color &A, const HexStateSpace::color &B)
    :hex(hex), A(A), B(B)
{}

short int HexStrategyControl::getWinningStep() const
{
    std::cout << ">>> Searching winning strategy for all empty field: " << std::endl;
    // level := 0 -- A player's level
    for (unsigned short int i = 0; i < hex.getLength(); i++)
    {
        std::cout << "|";
        if (hex.get(i) == HexStateSpace::EMPTY) {
            HexStateSpace nextHex(hex);
            nextHex.set(i, A);
            HexStateSpace::color winner = alphaBetaRecursion(nextHex, 1);
            if (winner == A)
            {
                std::cout << std::endl;
                return i;
            }
        }
    }
    std::cout << std::endl;
    return -1;
}


HexStateSpace::color HexStrategyControl::alphaBetaRecursion(HexStateSpace& curHex, const unsigned int &level) const
{
    // In the winner state
    HexStateSpace::color winner = curHex.isWinner();
    if (winner == A) return A;
    if (winner == B) return B;

    // recursion
    if (level % 2 != 0) { // B player's level
        for (unsigned short int i = 0; i < curHex.getLength(); i++)
        {
            if (curHex.get(i) == HexStateSpace::EMPTY)
            {
                HexStateSpace nextHex(curHex);
                nextHex.set(i, B);
                HexStateSpace::color winner = alphaBetaRecursion(nextHex, level + 1);
                if (winner == B)
                {
                    return B; // if there is at least one field which B can win => return B
                }
            }
        }
        return A;
    }
    else // A player's level
    {
        for (unsigned short int i = 0; i < curHex.getLength(); i++)
        {
            if (curHex.get(i) == HexStateSpace::EMPTY)
            {
                HexStateSpace nextHex(curHex);
                nextHex.set(i, A);
                HexStateSpace::color winner = alphaBetaRecursion(nextHex, level + 1);
                if (winner == A)
                {
                    return A; // if there is at least one field which A can win => return A
                }
            }
        }
        return B;
    }
}
