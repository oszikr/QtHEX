#include "hexstatespace.h"

#ifndef _HEXTABLESIZE_
#define _HEXTABLESIZE_ 13
#endif

// ctor
HexStateSpace::HexStateSpace(): SIZE(_HEXTABLESIZE_), LENGTH(SIZE*SIZE), lastField(-1), count(0)
{
    stateSpace = new color[SIZE * SIZE];
    for (unsigned short int i = 0; i < SIZE * SIZE; i++)
    {
        stateSpace[i] = EMPTY;
    }
}

// copy ctor
HexStateSpace::HexStateSpace(const HexStateSpace& other): SIZE(_HEXTABLESIZE_), LENGTH(SIZE*SIZE), lastField(-1), count(0)
{
    stateSpace = new color[SIZE * SIZE];
    for (unsigned short int i = 0; i < SIZE * SIZE; i++)
    {
        stateSpace[i] = other.stateSpace[i];
    }
    count = other.count;
}

// =operator
HexStateSpace& HexStateSpace::operator=(const HexStateSpace& other)
{
    if(this != &other)
    {
        for (unsigned short int i = 0; i < SIZE * SIZE; i++)
        {
            stateSpace[i] = other.stateSpace[i];
        }
        count = other.count;
    }
    return *this;
}

// dtor
HexStateSpace::~HexStateSpace()
{
    delete[] stateSpace;
}

std::ostream& operator<<(std::ostream& os, const HexStateSpace& obj)
{
    os << "Hex State Space at " << &obj << endl;
    for (unsigned short int i = 0; i < obj.SIZE; i++)
    {
        for (unsigned short int j = 0; j < obj.SIZE; j++)
        {
            if(obj.get(i, j) == HexStateSpace::RED)
                os << " \e[0;31m" << obj.get(i, j) << "\e[m";
            else if(obj.get(i, j) == HexStateSpace::BLUE)
                os << " \e[0;34m" << obj.get(i, j) << "\e[m";
            else
                os << " \e[0;37m" << obj.get(i, j) << "\e[m";
        }
        os << endl;
    }
    return os;
}

short int HexStateSpace::getLast() const
{
    return lastField;
}

unsigned short int HexStateSpace::getSize() const
{
    return SIZE;
}

unsigned short int HexStateSpace::getLength() const
{
    return LENGTH;
}

unsigned short int HexStateSpace::getCount() const
{
    return count;
}

HexStateSpace::color* HexStateSpace::getSpace() const
{
    return stateSpace;
}

HexStateSpace::color HexStateSpace::get(unsigned short int i, unsigned short int j) const
{
    return get(SIZE * i + j);
}

HexStateSpace::color HexStateSpace::get(unsigned short int i) const
{
    return stateSpace[i];
}

void HexStateSpace::set(unsigned short int i, unsigned short int j, color value)
{
    set(SIZE * i + j, value);
}

void HexStateSpace::set(unsigned short int i, color value)
{
    lastField = i;
    stateSpace[lastField] = value;
    count++;
}

void HexStateSpace::clear()
{
    for (unsigned short int i = 0; i < SIZE * SIZE; i++)
    {
        stateSpace[i] = EMPTY;
    }
    count = 0;
}

void HexStateSpace::undo()
{
    stateSpace[lastField] = EMPTY;
    count--;
}

// Starts depth-first search in the table to recognize the winner state
HexStateSpace::color HexStateSpace::isWinner() const
{
    // prepare coloring table
    color* routeColoring = new color[SIZE * SIZE];
    for (unsigned short int i = 0; i < SIZE * SIZE; i++)
    {
        routeColoring[i] = EMPTY;
    }

    // prepare two flag to note that the left-right or top-down edge is reached
    bool* edges = new bool[2];
    for (unsigned short int i = 0; i < 2; i++)
    {
        edges[i] = false;
    }

    // start the DFS recursion
    color winner = depthFirst(routeColoring, edges, lastField);

    // debug - do not remove
    /*cout << ">>> CURRENT ROUTING: " << endl;
    for (unsigned short int i = 0; i < SIZE; i++)
    {
        for (unsigned short int j = 0; j < SIZE; j++)
        {
            if(routeColoring[SIZE * i + j] == HexStateSpace::RED)
                cout << " \e[0;31m" << routeColoring[SIZE * i + j] << "\e[m";
            else if(routeColoring[SIZE * i + j] == HexStateSpace::BLUE)
                cout << " \e[0;34m" << routeColoring[SIZE * i + j] << "\e[m";
            else
                cout << " \e[0;37m" << routeColoring[SIZE * i + j] << "\e[m";
        }
        cout << endl;
    }*/
    // /debug

    delete[] edges;
    delete[] routeColoring;

    return winner;
}

HexStateSpace::color HexStateSpace::depthFirst(color* routeColoring, bool* edges, const short int cursor) const
{
    if(!(cursor >= 0 && cursor < SIZE * SIZE))  return EMPTY; // out of range
    if(routeColoring[cursor] != EMPTY)          return EMPTY; // already visited
    if(stateSpace[cursor] != stateSpace[lastField])       return EMPTY; // is not owned by the current player

    routeColoring[cursor] = stateSpace[lastField]; // coloring curent field

    if(routeColoring[cursor] == BLUE) // current field is blue
    {
        if(cursor % SIZE == 0) // first column
        {
            edges[0] = true; // reached left edge
        }
        else if(cursor % SIZE == SIZE - 1) // last column
        {
            edges[1] = true; // reached right edge
        }
    }
    else // (routeColoring[lastField] == RED)
    {
        if(cursor >= 0 && cursor < SIZE) // first row
        {
            edges[0] = true; // reached top edge
        }
        else if(cursor >= SIZE * (SIZE-1) && cursor < SIZE * SIZE) // last row
        {
            edges[1] = true; // reached down edge
        }
    }
    if(edges[0] == true && edges[1] == true)
    {
        return stateSpace[lastField]; // reached two edges too!!
    }

    // up neighbour is always exists if is is in rage
    if(depthFirst(routeColoring, edges, cursor - SIZE) == stateSpace[lastField])
    {
        //cout << "up neighbour is always exists if is is in rage" << endl;
        return stateSpace[lastField];
    }

    // down neighbour is always exists if is is in rage
    if(depthFirst(routeColoring, edges, cursor + SIZE) == stateSpace[lastField])
    {
        //cout << "down neighbour is always exists if is is in rage" << endl;
        return stateSpace[lastField];
    }

    // if left neighbour is exists
    if( cursor % SIZE > 0 && depthFirst(routeColoring, edges, cursor - 1) == stateSpace[lastField])
    {
        //cout << "if left neighbour is exists" << endl;
        return stateSpace[lastField];
    }

    // if right neighbour is exists
    if( cursor % SIZE < SIZE - 1 && depthFirst(routeColoring, edges, cursor + 1) == stateSpace[lastField])
    {
        //cout << "if right neighbour is exists" << endl;
        return stateSpace[lastField];
    }

    // if top neighbour is exists
    if( cursor % SIZE < SIZE - 1 && depthFirst(routeColoring, edges, cursor - SIZE + 1) == stateSpace[lastField])
    {
        //cout << "if top neighbour is exists" << endl;
        return stateSpace[lastField];
    }

    // if buttom neighbour is exists
    if( cursor % SIZE != 0 && depthFirst(routeColoring, edges, cursor + SIZE - 1) == stateSpace[lastField])
    {
        //cout << "if buttom neighbour is exists" << endl;
        return stateSpace[lastField];
    }

    return EMPTY;
}

short int HexStateSpace::heuristicScore() const
{
    color player = stateSpace[lastField];
    return heuristicScore(player);
}

short int HexStateSpace::heuristicScore(color player) const
{
    HexStateSpace::color oppPlayer = player == HexStateSpace::BLUE ? HexStateSpace::RED : HexStateSpace::BLUE;
    std::vector<E> edges;
    std::vector<unsigned short int> weight;

    // computing edges and weight
    for(unsigned short int cursor = 0; cursor < LENGTH; cursor++)
    {
        if(stateSpace[cursor] == oppPlayer)  continue; // is not owned by the current player

        std::vector<unsigned short int> neighbours;

        // up neighbour
        if(cursor - SIZE >= 0)
        {
            unsigned short int neighbour = cursor - SIZE;
            neighbours.push_back(neighbour);
        }

        // down neighbour
        if(cursor + SIZE < SIZE * SIZE)
        {
            unsigned short int neighbour = cursor + SIZE;
            neighbours.push_back(neighbour);
        }

        // left neighbour
        if( cursor % SIZE > 0)
        {
            unsigned short int neighbour = cursor - 1;
            neighbours.push_back(neighbour);
        }

        // right neighbour
        if(cursor % SIZE < SIZE - 1)
        {
            unsigned short int neighbour = cursor + 1;
            neighbours.push_back(neighbour);
        }

        // top neighbour
        if(cursor % SIZE < SIZE - 1 && cursor > SIZE - 1)
        {
            unsigned short int neighbour = cursor - SIZE + 1;
            neighbours.push_back(neighbour);
        }

        // buttom neighbour
        if( cursor % SIZE != 0 && cursor < SIZE * (SIZE-1))
        {
            unsigned short int neighbour = cursor + SIZE - 1;
            neighbours.push_back(neighbour);
        }

        for (std::vector<unsigned short int>::iterator it = neighbours.begin() ; it != neighbours.end(); ++it)
        {
            unsigned short int neighbour = *it;
            if(stateSpace[neighbour] == player)
            {
                // add edge to cursor-neighbour
                edges.push_back(std::pair<int, int>(cursor, neighbour));
                // weight 0
                weight.push_back(0);
            }
            else if(stateSpace[neighbour] == EMPTY)
            {
                // add edge to cursor-neighbour
                edges.push_back(std::pair<int, int>(cursor, neighbour));
                // weight 1
                weight.push_back(1);
            }
        }
    }

    // computing 2 soures and 2 targets

    // blue left
    for(unsigned short int cursor = 0; cursor < LENGTH; cursor += SIZE)
    {
        // add edge to blue_s-cursor
        edges.push_back(std::pair<int, int>(BLUE_S, cursor));
        // weight 0
        weight.push_back(0);
    }

    // blue right
    for(unsigned short int cursor = SIZE-1; cursor < LENGTH; cursor += SIZE)
    {
        // add edge to cursor-blue_t
        edges.push_back(std::pair<int, int>(cursor, BLUE_T));
        // weight 0
        weight.push_back(0);
    }

    // red top
    for(unsigned short int cursor = 0; cursor < SIZE; cursor++)
    {
        // add edge to red_s-cursor
        edges.push_back(std::pair<int, int>(RED_S, cursor));
        // weight 0
        weight.push_back(0);
    }

    // red bottom
    for(unsigned short int cursor = SIZE * (SIZE-1); cursor < LENGTH; cursor++)
    {
        // add edge to cursor-red_t
        edges.push_back(std::pair<int, int>(cursor, RED_T));
        // weight 0
        weight.push_back(0);
    }

    int score = 0;
    if(player == BLUE)
    {
        cout << "heuristicScore>" << player << ": " << score << endl;
        return score;
    }
    else //(player == RED)
    {
        cout << "heuristicScore>" << player << ": " << score<< endl;
        return score;
    }

}












