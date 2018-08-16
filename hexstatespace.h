#ifndef HEXSTATESPACE_H
#define HEXSTATESPACE_H
#include <iostream>

class hexStateSpace
{
public:
    enum color {EMPTY, BLUE, RED};
    unsigned short int SIZE;
    color* space; // state spece
    // ctor
    hexStateSpace();
    hexStateSpace(const unsigned short int size);
    // copy ctor
    hexStateSpace(const hexStateSpace& other);
    // =operator
    hexStateSpace& operator=(const hexStateSpace& other);
    // dtor
    ~hexStateSpace();

    friend std::ostream& operator<<(std::ostream& os, const hexStateSpace& obj);
    color get(unsigned short int i, unsigned short int j) const;
    color get(unsigned short int i) const;
    void set(unsigned short i, unsigned short j, color value);
    void set(unsigned short i, color value);

    color winner() const;
    color depthFirst(color* routeColoring, bool *edges, short int cursor) const;

private:
    short int lastField;
};

#endif // HEXSTATESPACE_H
