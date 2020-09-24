#include "hexminmaxcontrol.h"

HexMinMaxControl::HexMinMaxControl(const HexStateSpace& hex, const HexStateSpace::color &player, const unsigned short int limit)
    :hex(hex), player(player), limit(limit)
{}

// Start game tree generator for all EMPTY fields. If a winning strategy found, return with the field index.
// Ff the return value of the recursions equals with the current player, this a winning way
short int HexMinMaxControl::getWinningStep() const
{
    std::cout << (player == HexStateSpace::BLUE ? "\e[0;34mBlue" : "\e[0;31mRed") << "\e[m" << " palyer's h score is: " << hex.heuristicScore() << std::endl;
    std::cout << ">>> Searching winning strategy for all empty field: " << std::endl;

    HexStateSpace::color curPlayer = player;
    unsigned int curLevel = 0; // A player's level

    unsigned int nextLevel = curLevel + 1;
    HexStateSpace::color nextPlayer = curPlayer == HexStateSpace::BLUE ? HexStateSpace::RED : HexStateSpace::BLUE;

    short int min = 1000; // lost score
    short int min_i = -1;
    for (unsigned short int i = 0; i < hex.getLength(); i++)
    {
        std::cout << i+1 << "/" << hex.getLength();
        if (hex.get(i) == HexStateSpace::EMPTY) { // Empty Field
            HexStateSpace nextHex(hex); // Copy the state of the table
            nextHex.set(i, nextPlayer); // Mark the field for the current player
            short int score = minMaxRecursion(nextHex, nextPlayer, nextLevel);
            //short int score = hex.heuristicScore();
            std::cout << ": " << score << std::endl;
            if(min < score) {
                min = score;
                min_i = i;
            }
            if(score == 0) // max score - winning state
            {
                return min_i;
            }
        }
        std::cout << std::endl;
    }
    return min_i;
}

// DFS in the game tree. Alpha/beta cutting is applyed
short int HexMinMaxControl::minMaxRecursion(const HexStateSpace& curHex, const HexStateSpace::color &curPlayer, const unsigned int &curLevel) const
{
    unsigned int nextLevel = curLevel + 1;
    HexStateSpace::color nextPlayer = curPlayer == HexStateSpace::BLUE ? HexStateSpace::RED : HexStateSpace::BLUE;

    // CURPLAYER
    if(curPlayer == player) {
        short int curScore = curHex.heuristicScore();
        if(curScore == 0 || curLevel == limit)
        {
            return curScore;
        }

        short int min = 1000;
        for (unsigned short int i = 0; i < hex.getLength(); i++)
        {

            if (hex.get(i) == HexStateSpace::EMPTY) { // Empty Field
                HexStateSpace nextHex(hex); // Copy the state of the table
                nextHex.set(i, nextPlayer); // Mark the field for the current player
                short int score = minMaxRecursion(nextHex, nextPlayer, nextLevel);

                if(min > score) {
                    min = score;
                }
                if(score == 0)
                {
                    return score;
                }
            }
        }
        return min;
    }
    //OPPPLAYER
    else {
        short int curScore = curHex.heuristicScore();
        if(curScore == 0 || curLevel == limit)
        {
            return -curScore;
        }

        short int max = 0;
        for (unsigned short int i = 0; i < hex.getLength(); i++)
        {

            if (hex.get(i) == HexStateSpace::EMPTY) { // Empty Field
                HexStateSpace nextHex(hex); // Copy the state of the table
                nextHex.set(i, nextPlayer); // Mark the field for the current player
                short int score = minMaxRecursion(nextHex, nextPlayer, nextLevel);

                if(max < score) {
                    max = score;
                }
                if(score == 0)
                {
                    return 1000;
                }
            }
        }
        return max;
    }

}
