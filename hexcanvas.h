#ifndef HEXCANVAS_H
#define HEXCANVAS_H

#include <map>
#include <cmath>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include "hexstatespace.h"

# define M_PI           3.14159265358979323846

class HexCanvas : public QWidget
{
    Q_OBJECT
public:
    unsigned short int TABLESIZE;
    hexStateSpace::color* SPACE; // state spece
    unsigned short int HEXAGONSIZE;
    explicit HexCanvas(QWidget *parent = 0);
    ~HexCanvas();
    void setStateSpace(hexStateSpace::color* space, unsigned short int tablesize, int hexagonSize);
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;    

    struct Hexagon
    {
        Hexagon(){}
        Hexagon(QPoint center, unsigned short int HEXAGONSIZE)
        {
            this->center = center;
            d = flatHexCorner(center, HEXAGONSIZE, 0);
            e = flatHexCorner(center, HEXAGONSIZE, 1);
            f = flatHexCorner(center, HEXAGONSIZE, 2);
            a = flatHexCorner(center, HEXAGONSIZE, 3);
            b = flatHexCorner(center, HEXAGONSIZE, 4);
            c = flatHexCorner(center, HEXAGONSIZE, 5);
        }
        QPoint flatHexCorner(QPoint& center, unsigned short int size, unsigned short int i) const
        {
            int angle_deg = 60 * i;
            double angle_rad = M_PI / 180 * angle_deg;
            return QPoint(center.x() + size * std::cos(angle_rad),
                          center.y() + size * std::sin(angle_rad));
        }
        QPoint center;
        QPoint a, b, c, d, e, f;

        void mergeWithUp(unsigned short int tablesize, short int cursor, Hexagon* hexagons)
        {
            cursor -= tablesize;
            if(!(cursor >= 0 && cursor < tablesize * tablesize)) return;
            b = hexagons[cursor].f;
            c = hexagons[cursor].e;
        }
        void mergeWithLt(unsigned short int tablesize, short int cursor, Hexagon* hexagons)
        {
            if(cursor % tablesize == 0) return;
            cursor -= 1;
            if(!(cursor >= 0 && cursor < tablesize * tablesize)) return;
            a = hexagons[cursor].c;
            f = hexagons[cursor].d;
        }
    };

    Hexagon* hexagons;

signals:

public slots:

};

#endif // HEXCANVAS_H
