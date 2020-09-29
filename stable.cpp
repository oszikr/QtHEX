#include "stable.h"

STable::STable()
{
    {
        std::string fileName = "../QtHEX/tablesize.size";
        std::cout << "The table size will be loaded: " << fileName << std::endl;
        std::fstream myfile(fileName, std::ios_base::in);
        unsigned short int size;
        while (myfile >> size)
        {
            _HEXTABLESIZE_ = size;
            std::cout << "tablesize.txt: " << size << std::endl;
            return;
        }
    }
    {
        std::string fileName = "tablesize.size";
        std::cout << "The table size will be loaded: " << fileName << std::endl;
        std::fstream myfile(fileName, std::ios_base::in);
        unsigned short int size;
        while (myfile >> size)
        {
            _HEXTABLESIZE_ = size;
            std::cout << "tablesize.txt: " << size << std::endl;
            return;
        }
    }
    std::cout << "tablesize.txt: nor found." << std::endl;
}

STable* STable::setInstance()
{
   if (!instance)
     instance = new STable();
   return instance;
}

unsigned short int STable::getSize()
{
   setInstance();
   return instance->_HEXTABLESIZE_;
}

STable* STable::instance = 0;
