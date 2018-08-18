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
    void setStateSpace(hexStateSpace::color* space, unsigned short int tablesize, int hexagonSize);
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

    //std::map<unsigned short int, Hexagon> hexagonCenterMap;

    struct Hexagon
    {
        Hexagon(QPoint center, unsigned short int HEXAGONSIZE)
        {
            this->center = center;
            a = flatHexCorner(center, HEXAGONSIZE, 0);
            b = flatHexCorner(center, HEXAGONSIZE, 1);
            c = flatHexCorner(center, HEXAGONSIZE, 2);
            d = flatHexCorner(center, HEXAGONSIZE, 3);
            e = flatHexCorner(center, HEXAGONSIZE, 4);
            f = flatHexCorner(center, HEXAGONSIZE, 5);
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
    };

signals:

public slots:

};

#endif // HEXCANVAS_H
