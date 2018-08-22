#ifndef HEXSTATESPACE_H
#define HEXSTATESPACE_H
#include <iostream>

class hexStateSpace
{
public:
    enum color {EMPTY, BLUE, RED};

    hexStateSpace();
    hexStateSpace(const unsigned short int size);
    hexStateSpace(const hexStateSpace& other);
    hexStateSpace& operator=(const hexStateSpace& other);
    ~hexStateSpace();

    friend std::ostream& operator<<(std::ostream& os, const hexStateSpace& obj);

    unsigned short int getSize() const; // table height/width
    unsigned short int getLength() const; // count of fields
    color* getSpace() const;

    color get(unsigned short int i, unsigned short int j) const;
    color get(unsigned short int i) const;
    void set(unsigned short i, unsigned short j, color value);
    void set(unsigned short i, color value);

    color winner() const;
    color depthFirst(color* routeColoring, bool *edges, short int cursor) const;

private:
    unsigned short int SIZE; // table height/width
    unsigned short int LENGTH; // cout of fields
    color* space; // state spece
    short int lastField;
};

#endif // HEXSTATESPACE_H
