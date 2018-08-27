#include "hexstatespace.h"

#ifndef _HEXTABLESIZE_
#define _HEXTABLESIZE_ 4
#endif

// ctor
HexStateSpace::HexStateSpace(): SIZE(_HEXTABLESIZE_), LENGTH(SIZE*SIZE), lastField(-1)
{
    stateSpace = new color[SIZE * SIZE];
    for (unsigned short int i = 0; i < SIZE * SIZE; i++)
    {
        stateSpace[i] = EMPTY;
    }
}

// copy ctor
HexStateSpace::HexStateSpace(const HexStateSpace& other): SIZE(_HEXTABLESIZE_), LENGTH(SIZE*SIZE), lastField(-1)
{
    stateSpace = new color[SIZE * SIZE];
    for (unsigned short int i = 0; i < SIZE * SIZE; i++)
    {
        stateSpace[i] = other.stateSpace[i];
    }
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
    os << "Hex State Space at " << &obj << std::endl;
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
        os << std::endl;
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

HexStateSpace::color* HexStateSpace::getSpace() const
{
    return stateSpace;
}

HexStateSpace::color HexStateSpace::get(unsigned short int i, unsigned short int j) const
{
    return stateSpace[SIZE * i + j];
}

HexStateSpace::color HexStateSpace::get(unsigned short int i) const
{
    return stateSpace[i];
}

void HexStateSpace::set(unsigned short int i, unsigned short int j, color value)
{
    lastField = (SIZE * i + j);
    stateSpace[lastField] = value;
}

void HexStateSpace::set(unsigned short int i, color value)
{
    lastField = i;
    stateSpace[lastField] = value;
}

void HexStateSpace::clear()
{
    for (unsigned short int i = 0; i < SIZE * SIZE; i++)
    {
        stateSpace[i] = EMPTY;
    }
}

void HexStateSpace::undo()
{
    stateSpace[lastField] = EMPTY;
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

    delete[] edges;
    delete[] routeColoring;

    return winner;
}

HexStateSpace::color HexStateSpace::depthFirst(color* routeColoring, bool* edges, short int cursor) const
{
    if(!(cursor >= 0 && cursor < SIZE * SIZE))  return EMPTY; // out of range
    if(routeColoring[cursor] != EMPTY)          return EMPTY; // already visited
    if(stateSpace[cursor] != stateSpace[lastField])       return EMPTY; // is not owned by the current player

    routeColoring[cursor] = stateSpace[lastField]; // coloring curent field

    if(routeColoring[cursor] == BLUE) // current field is blue
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
    else // (routeColoring[lastField] == RED)
    {
        if(cursor % SIZE == 0) // first column
        {
            edges[0] = true; // reached left edge
        }
        else if(cursor % SIZE == 10) // last column
        {
            edges[1] = true; // reached right edge
        }
    }
    if(edges[0] == true && edges[1] == true)
    {
        return stateSpace[lastField]; // reached two edges too!!
    }

    // upper neighbour is always exists if is is in rage
    if(depthFirst(routeColoring, edges, cursor - _HEXTABLESIZE_) == stateSpace[lastField])
        return stateSpace[lastField];

    // downer neighbour is always exists if is is in rage
    if(depthFirst(routeColoring, edges, cursor + _HEXTABLESIZE_) == stateSpace[lastField])
        return stateSpace[lastField];

    // if left neighbour is exists
    if( cursor % _HEXTABLESIZE_ > 0 && depthFirst(routeColoring, edges, cursor - 1) == stateSpace[lastField])
        return stateSpace[lastField];

    // if right neighbour is exists
    if( cursor % _HEXTABLESIZE_ < 10 && depthFirst(routeColoring, edges, cursor + 1) == stateSpace[lastField])
        return stateSpace[lastField];

    // if upper neighbour is exists
    if( cursor % _HEXTABLESIZE_ > 0 && depthFirst(routeColoring, edges, cursor - 12) == stateSpace[lastField])
        return stateSpace[lastField];

    // if downer neighbour is exists
    if( cursor % _HEXTABLESIZE_ < 10 && depthFirst(routeColoring, edges, cursor + 12) == stateSpace[lastField])
        return stateSpace[lastField];

    return EMPTY;
}
