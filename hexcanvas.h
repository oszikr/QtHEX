#ifndef HEXCANVAS_H
#define HEXCANVAS_H

#include <map>
#include <cmath>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include "hexstatespace.h"
#include "hexagon.hpp"

# define M_PI           3.14159265358979323846

class HexCanvas : public QWidget
{
    Q_OBJECT
public:
    unsigned short int TABLESIZE;
    hexStateSpace::color* SPACE; // state spece
    unsigned short int HEXAGONSIZE;
    unsigned short int PADDING;
    short int pointed;
    QVector<QPoint> upBorderPoints;
    QVector<QPoint> dnBorderPoints;
    QVector<QPoint> ltBorderPoints;
    QVector<QPoint> rtBorderPoints;
    QTimer* setMouseTrackingEnabledTimer;

    explicit HexCanvas(QWidget *parent = 0);
    ~HexCanvas() Q_DECL_OVERRIDE;
    void setStateSpace(hexStateSpace::color* space, unsigned short int tablesize, int hexagonSize);
    unsigned short int getHexagonIndex(QPoint hit);
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    Hexagon* hexagons;

signals:

public slots:
    void setMouseTrackingEnabled();
};

#endif // HEXCANVAS_H
