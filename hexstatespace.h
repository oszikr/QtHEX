#ifndef HEXSTATESPACE_H
#define HEXSTATESPACE_H
#include <iostream>
#include <sstream>

#include <lemon/list_graph.h>
#include <lemon/maps.h>
#include <lemon/dijkstra.h>

using namespace lemon;
using namespace std;

template <typename T> using NodeMap = ListGraph::NodeMap<T>;
template <typename T> using EdgeMap = ListGraph::EdgeMap<T>;

class HexStateSpace
{
public:
    enum color {EMPTY, BLUE, RED}; // this enum represents the colors on teh table

    HexStateSpace();
    HexStateSpace(const HexStateSpace& other);
    HexStateSpace& operator=(const HexStateSpace& other);
    ~HexStateSpace();

    friend std::ostream& operator<<(std::ostream& os, const HexStateSpace& obj);

    unsigned short int getSize() const; // table height/width. It is 13.
    unsigned short int getLength() const; // count of fields. It is 13*13.
    unsigned short int getCount() const; // count of steps
    color* getSpace() const; // the 11*11 sized vector represents the fields and colors on the table

    short int getLast() const; // get last clicked field vector like index
    color get(unsigned short int i, unsigned short int j) const; // get field value by table like indexing
    color get(unsigned short int i) const; // get field value by vector like indexing
    void set(unsigned short i, unsigned short j, color value); // set field value by table like indexing
    void set(unsigned short i, color value); // set field value by vector like indexing
    void clear(); // fill the statespacee with EMPTY
    void undo(); // set lastField to EMPTY
    color isWinner() const; // if the last field change make a winning state then return the winner plyer's color

private:
    unsigned short int SIZE; // table height/width
    unsigned short int LENGTH; // cout of fields
    color* stateSpace; // state spece
    short int lastField; // last clicked field vector like index
    unsigned short int count; // count of steps

    // Depth-first search in the hex board. The search shearch edges connections with the current color.
    // Used by isWinner(). coloring and edges arrays have to init.
    color depthFirst(color* routeColoring, bool *edges, short int cursor) const;
    short int heuristicScore() const;
    ListGraph &toGraph() const;
};

#endif // HEXSTATESPACE_H
