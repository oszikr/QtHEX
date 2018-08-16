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
    QPoint flatHexCorner(QPoint &center, unsigned short size, unsigned short i) const;

    std::map<unsigned short int, QPoint> hexagonCenterMap;

signals:

public slots:

};

#endif // HEXCANVAS_H
