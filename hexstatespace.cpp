#include "hexstatespace.h"

// ctor
hexStateSpace::hexStateSpace(): SIZE(11), lastField(-1)
{
    space = new color[SIZE * SIZE];
    for (unsigned short int i = 0; i < SIZE * SIZE; i++)
    {
        space[i] = EMPTY;
    }
}

hexStateSpace::hexStateSpace(const unsigned short int size): SIZE(size), lastField(-1)
{
    space = new color[SIZE * SIZE];
    for (unsigned short int i = 0; i < SIZE * SIZE; i++)
    {
        space[i] = EMPTY;
    }
}

// copy ctor
hexStateSpace::hexStateSpace(const hexStateSpace& other)
{
    SIZE = other.SIZE;
    space = new color[SIZE * SIZE];
    for (unsigned short int i = 0; i < SIZE * SIZE; i++)
    {
        space[i] = other.space[i];
    }
}

// =operator
hexStateSpace& hexStateSpace::operator=(const hexStateSpace& other)
{
    if(this != &other)
    {
        if(SIZE != other.SIZE)
        {
            delete[] space;
            SIZE = other.SIZE;
            space = new color[SIZE * SIZE];
        }
        for (unsigned short int i = 0; i < SIZE * SIZE; i++)
        {
            space[i] = other.space[i];
        }
    }
    return *this;
}

// dtor
hexStateSpace::~hexStateSpace()
{
    delete[] space;
}

std::ostream& operator<<(std::ostream& os, const hexStateSpace& obj)
{
    os << "Hex State Space at " << &obj << std::endl;
    for (unsigned short int i = 0; i < obj.SIZE; i++)
    {
        for (unsigned short int j = 0; j < obj.SIZE; j++)
        {
            if(obj.get(i, j) == hexStateSpace::RED)
                os << " \e[0;31m" << obj.get(i, j) << "\e[m";
            else if(obj.get(i, j) == hexStateSpace::BLUE)
                os << " \e[0;34m" << obj.get(i, j) << "\e[m";
            else
                os << " \e[0;37m" << obj.get(i, j) << "\e[m";
        }
        os << std::endl;
    }
    return os;
}

short int hexStateSpace::getLast() const
{
    return lastField;
}

unsigned short int hexStateSpace::getSize() const
{
    return SIZE;
}

unsigned short int hexStateSpace::getLength() const
{
    return LENGTH;
}

hexStateSpace::color* hexStateSpace::getSpace() const
{
    return space;
}

hexStateSpace::color hexStateSpace::get(unsigned short int i, unsigned short int j) const
{
    return space[SIZE * i + j];
}

hexStateSpace::color hexStateSpace::get(unsigned short int i) const
{
    return space[i];
}

void hexStateSpace::set(unsigned short int i, unsigned short int j, color value)
{
    lastField = (SIZE * i + j);
    space[lastField] = value;
}

void hexStateSpace::set(unsigned short int i, color value)
{
    lastField = i;
    space[lastField] = value;
}

void hexStateSpace::clear()
{
    for (unsigned short int i = 0; i < SIZE * SIZE; i++)
    {
        space[i] = EMPTY;
    }
}

void hexStateSpace::undo()
{
    space[lastField] = EMPTY;
}

hexStateSpace::color hexStateSpace::winner() const
{
    color* routeColoring = new color[SIZE * SIZE];
    for (unsigned short int i = 0; i < SIZE * SIZE; i++)
    {
        routeColoring[i] = EMPTY;
    }

    bool* edges = new bool[2];
    for (unsigned short int i = 0; i < 2; i++)
    {
        edges[i] = false;
    }

    color winner = depthFirst(routeColoring, edges, lastField);

    // debug
    /*std::cout << ">>> CURRENT ROUTING: " << std::endl;
    for (unsigned short int i = 0; i < SIZE; i++)
    {
        for (unsigned short int j = 0; j < SIZE; j++)
        {
            if(routeColoring[SIZE * i + j] == hexStateSpace::RED)
                std::cout << " \e[0;31m" << routeColoring[SIZE * i + j] << "\e[m";
            else if(routeColoring[SIZE * i + j] == hexStateSpace::BLUE)
                std::cout << " \e[0;34m" << routeColoring[SIZE * i + j] << "\e[m";
            else
                std::cout << " \e[0;37m" << routeColoring[SIZE * i + j] << "\e[m";
        }
        std::cout << std::endl;
    }*/
    // /debug

    delete[] edges;
    delete[] routeColoring;

    return winner;
}

hexStateSpace::color hexStateSpace::depthFirst(color* routeColoring, bool* edges, short int cursor) const
{
    if(!(cursor >= 0 && cursor < SIZE * SIZE))  return EMPTY; // out of range
    if(routeColoring[cursor] != EMPTY)          return EMPTY; // already visited
    if(space[cursor] != space[lastField])       return EMPTY; // is not owned by the current player

    routeColoring[cursor] = space[lastField]; // coloring curent field

    if(routeColoring[cursor] == BLUE)
    {
        if(cursor >= 0 && cursor < SIZE) // first row
        {
            edges[0] = true;
        }
        else if(cursor >= SIZE * (SIZE-1) && cursor < SIZE * SIZE)
        {
            edges[1] = true; // last row
        }
    }
    else // (routeColoring[lastField] == RED)
    {
        if(cursor % SIZE == 0) // first column
        {
            edges[0] = true;
        }
        else if(cursor % SIZE == 10) // last column
        {
            edges[1] = true;
        }
    }
    if(edges[0] == true && edges[1] == true)
    {
        return space[lastField];
    }

    if( cursor % 11 > 0 && depthFirst(routeColoring, edges, cursor - 1) == space[lastField])
        return space[lastField];

    if( cursor % 11 < 10 && depthFirst(routeColoring, edges, cursor + 1) == space[lastField])
        return space[lastField];

    if( cursor % 11 > 0 && depthFirst(routeColoring, edges, cursor - 12) == space[lastField])
        return space[lastField];

    if(depthFirst(routeColoring, edges, cursor - 11) == space[lastField])
        return space[lastField];

    if(depthFirst(routeColoring, edges, cursor + 11) == space[lastField])
        return space[lastField];

    if( cursor % 11 < 10 && depthFirst(routeColoring, edges, cursor + 12) == space[lastField])
        return space[lastField];

    return EMPTY;
}
