#ifndef HEXAGON_CPP
#define HEXAGON_CPP

#include <QWidget>

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
        d = flatHexCorner(center, HEXAGONSIZE, 0);
        e = flatHexCorner(center, HEXAGONSIZE, 1);
        f = flatHexCorner(center, HEXAGONSIZE, 2);
        a = flatHexCorner(center, HEXAGONSIZE, 3); // point 'a' is the left vertice
        b = flatHexCorner(center, HEXAGONSIZE, 4);
        c = flatHexCorner(center, HEXAGONSIZE, 5);
    }

    // compute the vertices
    inline QPoint flatHexCorner(QPoint& center, unsigned short int size, unsigned short int i) const
    {
        int angle_deg = 60 * i;
        double angle_rad = M_PI / 180 * angle_deg;
        return QPoint(center.x() + size * std::cos(angle_rad),
                      center.y() + size * std::sin(angle_rad));
    }

    // let the hexagon stole the nighbour's vertice points
    inline void mergeWithUp(unsigned short int tablesize, short int cursor, QVector<Hexagon> hexagons)
    {
        cursor -= tablesize;
        if(!(cursor >= 0 && cursor < tablesize * tablesize)) return;
        b = hexagons[cursor].f;
        c = hexagons[cursor].e;
    }

    // let the hexagon stole the nighbour's vertice points
    inline void mergeWithLt(unsigned short int tablesize, short int cursor, QVector<Hexagon> hexagons)
    {
        if(cursor % tablesize == 0) return;
        cursor -= 1;
        if(!(cursor >= 0 && cursor < tablesize * tablesize)) return;
        a = hexagons[cursor].c;
        f = hexagons[cursor].d;
    }
};

#endif // HEXAGON_CPP
