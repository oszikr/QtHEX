#include "hexminmaxcontrol.h"

HexMinMaxControl::HexMinMaxControl(const HexStateSpace& hex, const HexStateSpace::color& A, const HexStateSpace::color& B, const unsigned short limit)
    :hex(hex), A(A), B(B), limit(limit)
{}

std::string HexMinMaxControl::toString(std::vector<short int> &v) const
{
    std::stringstream ss;
    for(size_t i = 0; i < v.size(); i++) {
        ss << v[i] << " ";
    }
    return ss.str();
}

// Start game tree generator for all EMPTY fields. If a winning strategy found, return with the field index.
// if the return value of the recursions equals with the current player, this a winning way
std::vector<short int> HexMinMaxControl::getWinningStep(/*std::set<unsigned short int> fields*/) const
{
    std::cout << ">>> Searching winning strategy for all empty field: " << std::endl;
    // level := 0 -- A player's level
    int min_i = -1;
    int min = 1000;
    std::vector<short int> path;
    std::vector<short int> result;
    for (unsigned short int i = 0; i < hex.getLength(); i++)
    {
        if (hex.get(i) == HexStateSpace::EMPTY) { // Empty Field
            HexStateSpace nextHex(hex); // Copy the state of the table
            nextHex.set(i, A); // Mark the field for the current player !!!!KÉK!!!!
            path.push_back(i);
            short int h_score = minMaxRecursion(nextHex, 1, path);
            std::cout << toString(path) << ": " << h_score << std::endl;
            path.pop_back();
            if(h_score < min)
            {
                min = h_score;
                min_i = i;
            }
            result.push_back(h_score);
        }
    }
    return result;
}

short int HexMinMaxControl::minMaxRecursion(HexStateSpace& curHex, const unsigned int &level, std::vector<short int> &path) const
{
    HexStateSpace:: color curPlayer;
    HexStateSpace:: color nextPlayer;
    if(level % 2 == 1) // kék játékos szintje. a kéknek lépett az imént. a piros következik
    {
        curPlayer = HexStateSpace::BLUE;
        nextPlayer = HexStateSpace::RED;
    }
    else // piros játékos szintje. a piros lépett az imént. értékeljük ki. a kék következik
    {
        curPlayer = HexStateSpace::RED;
        nextPlayer = HexStateSpace::BLUE;
    }

    short int curPlayer_h_score = curHex.heuristicScore(curPlayer);
    if(curPlayer_h_score == -1000)
    {
        return curPlayer_h_score;
    }
    if(level == 2)
    {
        short int nextPlayer_h_score = curHex.heuristicScore(nextPlayer);
        short int h_score = curPlayer_h_score - nextPlayer_h_score;
        return h_score;
    }

    if(level % 2 == 1) // kék játékos szintje. a kéknek lépett az imént. a piros következik
    {
        //HexStateSpace:: color curPlayer = HexStateSpace::BLUE;
        HexStateSpace:: color nextPlayer = HexStateSpace::RED;
        short int m = 1000; // min
        for (unsigned short int i = 0; i < curHex.getLength(); i++)
        {
            if (curHex.get(i) == HexStateSpace::EMPTY)
            {
                HexStateSpace nextHex(curHex);
                nextHex.set(i, nextPlayer);
                path.push_back(i);
                short int h_score = minMaxRecursion(nextHex, level + 1, path);
                //std::cout << toString(path) << ": " << h_score << std::endl;
                path.pop_back();
                if(m > h_score) {
                    m = h_score;
                }
            }
        }
        return -m;
    }
    else //if(level % 2 == 0) // piros játékos szintje. a piros lépett az imént. értékeljük ki. a kék következik
    {
        //HexStateSpace:: color curPlayer = HexStateSpace::RED;
        HexStateSpace:: color nextPlayer = HexStateSpace::BLUE;
        short int m = 0; // max
        for (unsigned short int i = 0; i < curHex.getLength(); i++)
        {
            if (curHex.get(i) == HexStateSpace::EMPTY)
            {
                HexStateSpace nextHex(curHex);
                nextHex.set(i, nextPlayer);
                path.push_back(i);
                short int h_score = minMaxRecursion(nextHex, level + 1, path);
                //std::cout << toString(path) << ": " << h_score << std::endl;
                path.pop_back();
                if(m < h_score) {
                    m = h_score;
                }
            }
        }
        return m;
    }
}




/*short int HexMinMaxControl::minMaxRecursion(HexStateSpace& curHex, const unsigned int &level, std::vector<short int> &path) const
{
    short int h_score = curHex.heuristicScore();
    if(h_score == 0 || level == 3)
    {
        return h_score;
    }

    if(level % 2 == 1) // kék játékos szintje. a kéknek lépett az imént. a piros következik
    {
        //HexStateSpace:: color curPlayer = HexStateSpace::BLUE;
        HexStateSpace:: color nextPlayer = HexStateSpace::RED;
        short int m = 1000; // min
        for (unsigned short int i = 0; i < curHex.getLength(); i++)
        {
            if (curHex.get(i) == HexStateSpace::EMPTY)
            {
                HexStateSpace nextHex(curHex);
                nextHex.set(i, nextPlayer);
                path.push_back(i);
                short int h_score = minMaxRecursion(nextHex, level + 1, path);
                std::cout << toString(path) << ": " << h_score << std::endl;
                path.pop_back();
                if(m > h_score) {
                    m = h_score;
                }
            }
        }
        return m;
    }
    else //if(level % 2 == 0) // piros játékos szintje. a piros lépett az imént. értékeljük ki. a kék következik
    {
        //HexStateSpace:: color curPlayer = HexStateSpace::RED;
        HexStateSpace:: color nextPlayer = HexStateSpace::BLUE;
        short int m = 0; // max
        for (unsigned short int i = 0; i < curHex.getLength(); i++)
        {
            if (curHex.get(i) == HexStateSpace::EMPTY)
            {
                HexStateSpace nextHex(curHex);
                nextHex.set(i, nextPlayer);
                path.push_back(i);
                short int h_score = minMaxRecursion(nextHex, level + 1, path);
                std::cout << toString(path) << ": " << h_score << std::endl;
                path.pop_back();
                if(m < h_score) {
                    m = h_score;
                }
            }
        }
        return m;
    }
}*/
