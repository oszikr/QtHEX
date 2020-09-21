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
    /*std::cout << ">>> CURRENT ROUTING: " << std::endl;
    for (unsigned short int i = 0; i < SIZE; i++)
    {
        for (unsigned short int j = 0; j < SIZE; j++)
        {
            if(routeColoring[SIZE * i + j] == HexStateSpace::RED)
                std::cout << " \e[0;31m" << routeColoring[SIZE * i + j] << "\e[m";
            else if(routeColoring[SIZE * i + j] == HexStateSpace::BLUE)
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
        //std::cout << "up neighbour is always exists if is is in rage" << std::endl;
        return stateSpace[lastField];
    }

    // down neighbour is always exists if is is in rage
    if(depthFirst(routeColoring, edges, cursor + SIZE) == stateSpace[lastField])
    {
        //std::cout << "down neighbour is always exists if is is in rage" << std::endl;
        return stateSpace[lastField];
    }

    // if left neighbour is exists
    if( cursor % SIZE > 0 && depthFirst(routeColoring, edges, cursor - 1) == stateSpace[lastField])
    {
        //std::cout << "if left neighbour is exists" << std::endl;
        return stateSpace[lastField];
    }

    // if right neighbour is exists
    if( cursor % SIZE < SIZE - 1 && depthFirst(routeColoring, edges, cursor + 1) == stateSpace[lastField])
    {
        //std::cout << "if right neighbour is exists" << std::endl;
        return stateSpace[lastField];
    }

    // if top neighbour is exists
    if( cursor % SIZE < SIZE - 1 && depthFirst(routeColoring, edges, cursor - SIZE + 1) == stateSpace[lastField])
    {
        //std::cout << "if top neighbour is exists" << std::endl;
        return stateSpace[lastField];
    }

    // if buttom neighbour is exists
    if( cursor % SIZE != 0 && depthFirst(routeColoring, edges, cursor + SIZE - 1) == stateSpace[lastField])
    {
        //std::cout << "if buttom neighbour is exists" << std::endl;
        return stateSpace[lastField];
    }

    return EMPTY;
}

short int HexStateSpace::heuristicScore() const
{



    return 0;
}


ListGraph& HexStateSpace::toGraph() const
{
    ListGraph g;
    ListGraph::Node** nodes = new ListGraph::Node*[SIZE*SIZE];
    ListGraph::EdgeMap<int> length(g);

    for(short int cursor = 0; cursor < this->LENGTH; cursor++)
    {
        ListGraph::Node node = g.addNode();
        nodes[0] = &node;
    }

    for(short int cursor = 0; cursor < this->LENGTH; cursor++)
    {
        color curPlayer = stateSpace[lastField];
        if(stateSpace[cursor] != curPlayer)  continue; // is not owned by the current player

        ListGraph::Node* node1 = nodes[cursor];
        std::vector<short int> neighbours;

        // up neighbour is always exists if is is in rage
        if(cursor - SIZE >= 0)
        {
            //std::cout << "up neighbour is always exists if is is in rage" << std::endl;
            short int neighbour = cursor - SIZE;
            neighbours.push_back(neighbour);
        }

        // down neighbour is always exists if is is in rage
        if(cursor + SIZE < SIZE * SIZE)
        {
            //std::cout << "down neighbour is always exists if is is in rage" << std::endl;
            short int neighbour = cursor + SIZE;
            neighbours.push_back(neighbour);
        }

        // if left neighbour is exists
        if( cursor % SIZE > 0)
        {
            //std::cout << "if left neighbour is exists" << std::endl;
            short int neighbour = cursor - 1;
            neighbours.push_back(neighbour);
        }

        // if right neighbour is exists
        if(cursor % SIZE < SIZE - 1)
        {
            //std::cout << "if right neighbour is exists" << std::endl;
            short int neighbour = cursor + 1;
            neighbours.push_back(neighbour);
        }

        // if top neighbour is exists
        if(cursor % SIZE < SIZE - 1 && cursor > SIZE - 1)
        {
            //std::cout << "if top neighbour is exists" << std::endl;
            short int neighbour = cursor - SIZE + 1;
            neighbours.push_back(neighbour);
        }

        // if buttom neighbour is exists
        if( cursor % SIZE != 0 && cursor < SIZE * (SIZE-1))
        {
            //std::cout << "if buttom neighbour is exists" << std::endl;
            short int neighbour = cursor + SIZE - 1;
            neighbours.push_back(neighbour);
        }

        for (std::vector<short int>::iterator it = neighbours.begin() ; it != neighbours.end(); ++it)
        {
            short int neighbour = *it;
            ListGraph::Node* node2 = nodes[neighbour];
            if(stateSpace[neighbour] == curPlayer)
            {
                ListGraph::Edge edge = g.addEdge(*node1, *node2);
                length[edge] = 0;
            }
            else if(stateSpace[neighbour] != EMPTY)
            {
                ListGraph::Edge edge = g.addEdge(*node1, *node2);
                length[edge] = 1;
            }
        }

        //TODO add 4 soure and target

        delete[] nodes;
        return g;
    }
}












