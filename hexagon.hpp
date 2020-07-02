#ifndef HEXAGON_CPP
#define HEXAGON_CPP

#include <QWidget>
#include <iostream>

// This class represents a hexagon object int the front end widget
struct Hexagon
{
    QPoint center; // center is the hexagon's outer circle's center.
    QPoint a, b, c, d, e, f; // the hexagon's vertices

    inline Hexagon(){}

    // The size is the radius of the circle
    inline Hexagon(QPoint center, unsigned short int HEXAGONSIZE)
    {
        this->center = center;
        a = flatHexCorner(center, HEXAGONSIZE, 0); // right
        b = flatHexCorner(center, HEXAGONSIZE, 1); // this point is the bottom vertice
        c = flatHexCorner(center, HEXAGONSIZE, 2); // left
        d = flatHexCorner(center, HEXAGONSIZE, 3); // left
        e = flatHexCorner(center, HEXAGONSIZE, 4); // this point is the top vertice
        f = flatHexCorner(center, HEXAGONSIZE, 5); // right
    }

    // compute the vertices
    inline QPoint flatHexCorner(QPoint& center, unsigned short int size, unsigned short int i) const
    {
        int angle_deg = 30 + 60 * i;
        double angle_rad = M_PI / 180 * angle_deg;
        QPoint qp = QPoint(center.x() + size * std::cos(angle_rad),
                           center.y() + size * std::sin(angle_rad));
        //std::cout  << i << " " << qp.x() << " " << qp.y() << std::endl;
        return qp;
    }

    // let the hexagon stole the nighbour's vertice points
    inline void mergeWithTop(unsigned short int tablesize, short int cursor, QVector<Hexagon> hexagons)
    {
        if(cursor % tablesize != tablesize - 1)  // not right edge
        {
            cursor -= (tablesize - 1); // top neighbour
            if(cursor >= 0 && cursor < tablesize * tablesize) // in range
            {
                e = hexagons[cursor].c;
                f = hexagons[cursor].b;
            }
        }
    }

    // let the hexagon stole the nighbour's vertice points
    inline void mergeWithUp(unsigned short int tablesize, short int cursor, QVector<Hexagon> hexagons)
    {
        cursor -= tablesize; // up neighbour
        if(cursor >= 0 && cursor < tablesize * tablesize) // in range
        {
            e = hexagons[cursor].a;
            d = hexagons[cursor].b;
        }
    }

    // let the hexagon stole the nighbour's vertice points
    inline void mergeWithLt(unsigned short int tablesize, short int cursor, QVector<Hexagon> hexagons)
    {
        if(cursor % tablesize != 0)  // not left edge
        {
            cursor -= 1; // left neighbour
            if(cursor >= 0 && cursor < tablesize * tablesize) // in range
            {
                c = hexagons[cursor].a;
                d = hexagons[cursor].f;
            }
        }
    }
};

#endif // HEXAGON_CPP
