#include "hexabcontrol.h"

HexStrategyControl::HexStrategyControl(const HexStateSpace& hex, const HexStateSpace::color &A, const HexStateSpace::color &B)
    :hex(hex), A(A), B(B)
{}

// Start game tree generator for all EMPTY fields. If a winning strategy found, return with the field index.
// if the return value of the recursions equals with the current player, this a winning way
short int HexStrategyControl::getWinningStep() const
{
    std::cout << ">>> Searching winning strategy for all empty field: " << std::endl;
    // level := 0 -- A player's level
    for (unsigned short int i = 0; i < hex.getLength(); i++)
    {
        std::cout << i+1 << "/" << hex.getLength() << std::endl;
        if (hex.get(i) == HexStateSpace::EMPTY) { // Empty Field
            HexStateSpace nextHex(hex); // Copy the state of the table
            nextHex.set(i, A); // Mark the field for the current player
            HexStateSpace::color winner = alphaBetaRecursion(nextHex, 1);
            if (winner == A)
            {
                return i;
            }
        }
    }
    std::cout << std::endl;
    return -1;
}

// DFS in the game tree. Alpha/beta cutting is applyed
HexStateSpace::color HexStrategyControl::alphaBetaRecursion(HexStateSpace& curHex, const unsigned int &level) const
{
    // In the winner state
    HexStateSpace::color winner = curHex.isWinner();
    if (winner != HexStateSpace::EMPTY) return winner;

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
