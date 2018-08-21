#ifndef HEXAGON_CPP
#define HEXAGON_CPP

#include <QWidget>

struct Hexagon
{
    inline Hexagon(){}

    inline Hexagon(QPoint center, unsigned short int HEXAGONSIZE)
    {
        this->center = center;
        d = flatHexCorner(center, HEXAGONSIZE, 0);
        e = flatHexCorner(center, HEXAGONSIZE, 1);
        f = flatHexCorner(center, HEXAGONSIZE, 2);
        a = flatHexCorner(center, HEXAGONSIZE, 3);
        b = flatHexCorner(center, HEXAGONSIZE, 4);
        c = flatHexCorner(center, HEXAGONSIZE, 5);
    }

    inline QPoint flatHexCorner(QPoint& center, unsigned short int size, unsigned short int i) const
    {
        int angle_deg = 60 * i;
        double angle_rad = M_PI / 180 * angle_deg;
        return QPoint(center.x() + size * std::cos(angle_rad),
                      center.y() + size * std::sin(angle_rad));
    }
    QPoint center;
    QPoint a, b, c, d, e, f;

    inline void mergeWithUp(unsigned short int tablesize, short int cursor, Hexagon* hexagons)
    {
        cursor -= tablesize;
        if(!(cursor >= 0 && cursor < tablesize * tablesize)) return;
        b = hexagons[cursor].f;
        c = hexagons[cursor].e;
    }
    inline void mergeWithLt(unsigned short int tablesize, short int cursor, Hexagon* hexagons)
    {
        if(cursor % tablesize == 0) return;
        cursor -= 1;
        if(!(cursor >= 0 && cursor < tablesize * tablesize)) return;
        a = hexagons[cursor].c;
        f = hexagons[cursor].d;
    }
};

#endif // HEXAGON_CPP
