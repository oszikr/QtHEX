#include "hexstrategycontrol.h"

HexStrategyControl::HexStrategyControl(HexStateSpace hex, HexStateSpace::color A, HexStateSpace::color B)
    :hex(hex), A(A), B(B)
{}

short int HexStrategyControl::getWinningStep() const
{
    std::cout << ">>> Searching winning strategy for all empty field." << std::endl;
    // level := 0 -- A player's level
    for (unsigned short int i = 0; i < hex.getLength(); i++)
    {
        if (hex.get(i) == HexStateSpace::EMPTY) {
            std::cout << ">>> Searching winning strategy for field " << i << ": " << std::endl;
            HexStateSpace nextHex(hex);
            nextHex.set(i, A);
            HexStateSpace::color winner = alphaBetaRecursion(nextHex, 1);
            if (winner == A)
            {
                std::cout << "Found!" << std::endl;
                return i;
            }
        }
        std::cout << "Not found." << std::endl;
    }
    //std::cout << std::endl;
    return -1;
}


HexStateSpace::color HexStrategyControl::alphaBetaRecursion(HexStateSpace& curHex, const unsigned int level) const
{
    std::cout << "Level: " << level << std::endl;

    // In the winner state
    HexStateSpace::color winner = curHex.isWinner();
    if (winner == A) return A;
    if (winner == B) return B;

    // recursion
    if (level % 2 != 0) { // B player's level
        unsigned short int count = 0;
        for (unsigned short int i = 0; i < hex.getLength(); i++)
        {
            if (hex.get(i) == HexStateSpace::EMPTY) {
                count++;
                HexStateSpace nextHex(hex);
                nextHex.set(i, B);
                std::cout << "Field: " << i << std::endl;
                HexStateSpace::color winner = alphaBetaRecursion(nextHex, level + 1);
                if (winner == B)
                {
                    // if there is at least one field which B can win => return B
                    return B;
                }
            }
        }
        if(count == 0) return HexStateSpace::EMPTY; // no more field to play
        return A;
    }
    else // A player's level
    {
        unsigned short int count = 0;
        for (unsigned short int i = 0; i < hex.getLength(); i++)
        {
            if (hex.get(i) == HexStateSpace::EMPTY) {
                count++;
                HexStateSpace nextHex(hex);
                nextHex.set(i, A);
                std::cout << "Field: " << i << std::endl;
                HexStateSpace::color winner = alphaBetaRecursion(nextHex, level + 1);
                if (winner == A)
                {
                    // if there is at least one field which A can win => return A
                    return A;
                }
            }
        }
        if(count == 0) return HexStateSpace::EMPTY; // no more field to play
        return B;
    }

    //return HexStateSpace::EMPTY;
}
