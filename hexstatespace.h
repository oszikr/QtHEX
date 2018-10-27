#ifndef HEXSTATESPACE_H
#define HEXSTATESPACE_H
#include <iostream>

class HexStateSpace
{
public:
    enum color {EMPTY, BLUE, RED}; // this enum represents the colors on teh table

    HexStateSpace();
    HexStateSpace(const HexStateSpace& other);
    HexStateSpace& operator=(const HexStateSpace& other);
    ~HexStateSpace();

    friend std::ostream& operator<<(std::ostream& os, const HexStateSpace& obj);

    unsigned short int getSize() const; // table height/width. It is 11.
    unsigned short int getLength() const; // count of fields. It is 11*11.
    color* getSpace() const; // the 11*11 sized vector represents the fields and colors on the table

    short int getLast() const; // get last clicked field vector like index
    color get(unsigned short int i, unsigned short int j) const; // get field value by table like indexing
    color get(unsigned short int i) const; // get field value by vector like indexing
    void set(unsigned short i, unsigned short j, color value); // set field value by table like indexing
    void set(unsigned short i, color value); // set field value by vector like indexing
    void clear(); // fill the statespacee with EMPTY
    void undo(); // set lastField to EMPTY
    color isWinner() const; // if the last field change make a winning state then return the winner plyer's color
    //long getHashCode() const;

private:
    unsigned short int SIZE; // table height/width
    unsigned short int LENGTH; // cout of fields
    color* stateSpace; // state spece
    short int lastField; // last clicked field vector like index

    color depthFirst(color* routeColoring, bool *edges, short int cursor) const;
};

#endif // HEXSTATESPACE_H
