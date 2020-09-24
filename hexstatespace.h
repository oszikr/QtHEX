#ifndef HEXSTATESPACE_H
#define HEXSTATESPACE_H
#define BLUE_S 170
#define BLUE_T 171
#define RED_S 172
#define RED_T 173

#include <boost/config.hpp>
#include <iostream>
#include <fstream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

using namespace boost;

typedef adjacency_list < listS, vecS, directedS,
  no_property, property < edge_weight_t, int > > graph_t;
typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
typedef std::pair<int, int> Edge;

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
    short int heuristicScore() const;
    short int heuristicScore(color player) const;

private:
    unsigned short int SIZE; // table height/width
    unsigned short int LENGTH; // cout of fields
    color* stateSpace; // state spece
    short int lastField; // last clicked field vector like index
    unsigned short int count; // count of steps

    // Depth-first search in the hex board. The search shearch edges connections with the current color.
    // Used by isWinner(). coloring and edges arrays have to init.
    color depthFirst(color* routeColoring, bool *edges, short int cursor) const;
};

#endif // HEXSTATESPACE_H
