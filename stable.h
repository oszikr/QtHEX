#ifndef STABLE_H
#define STABLE_H

#include <iostream>
#include <fstream>

class STable
{
    static STable *instance;
    unsigned short int _HEXTABLESIZE_ = 0;
    STable();
    static STable* setInstance();
public:
    static unsigned short int getSize();
};


#endif // STABLE_H

