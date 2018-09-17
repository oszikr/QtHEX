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
#include "hexstrategycontrol.h"

# define M_PI           3.14159265358979323846

class HexCanvas : public QWidget
{
    Q_OBJECT
public:

    unsigned short int HEXAGONSIZE; // hexagon's radius
    unsigned short int PADDING;
    short int pointed;
    QVector<Hexagon> hexagons;
    double HEXAGONWIDTH, HEXAGONHEIGHT;
    HexStateSpace* stateSpace; // state spece
    QVector<QPoint> upBorderPoints;
    QVector<QPoint> dnBorderPoints;
    QVector<QPoint> ltBorderPoints;
    QVector<QPoint> rtBorderPoints;
    QTimer* setMouseTrackingEnabledTimer;
    HexStateSpace::color player;
    Hexagon hintBtn;
    Hexagon nextInfoBtn;
    Hexagon prevBtn;
    Hexagon clearBtn;

    explicit HexCanvas(QWidget *parent = 0);
    ~HexCanvas() Q_DECL_OVERRIDE;

    void setStateSpace(HexStateSpace* stateSpace);
    short int getHexagonIndex(QPoint hit);
    bool isHex(const QPoint &hit, const Hexagon &hex);
    void paintTriangle(const QPoint &a, const QPoint &b, const QPoint &c, const QColor& qcolor, QPainter& painter);
    void paintHex(const Hexagon &hexagon, const QColor &qcolor, QPainter& painter);
    HexStateSpace::color getPlayerNextPlayer();

    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

signals:

public slots:
    void setMouseTrackingEnabled();
    void hint();
    void nextInfo();
    void prev();
    void clear();

private:
    double getWallTime() const;
};

#endif // HEXCANVAS_H
