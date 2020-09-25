#ifndef HEXCANVAS_H
#define HEXCANVAS_H

#include <map>
#include <cmath>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include <algorithm>
#include "hexstatespace.h"
#include "hexagon.hpp"
#include "hexabcontrol.h"
#include "hexnnetcontrol.h"
#include "hexminmaxcontrol.h"

# define M_PI           3.14159265358979323846

class HexCanvas : public QWidget
{
    Q_OBJECT
public:

    HexNnetControl* nnetctrl;

    unsigned short int HEXAGONSIZE; // 27: hexagon's radius
    unsigned short int PADDING; // 20: padding from widget's edges
    short int pointed; // last pointed hexangon's index
    QVector<Hexagon> hexagons; // hexagons to draw
    double HEXAGONWIDTH, HEXAGONHEIGHT; // hexagons' dimensions
    HexStateSpace* stateSpace; // state spece

    // points to draw red and blue zig zag edges
    QVector<QPoint> upBorderPoints;
    QVector<QPoint> dnBorderPoints;
    QVector<QPoint> ltBorderPoints;
    QVector<QPoint> rtBorderPoints;

    QTimer* setMouseTrackingEnabledTimer; // timer to get cusor coordinates and highlight pointed hexagon
    HexStateSpace::color player; // next player
    Hexagon nextInfoBtn; // green button: cout next player's color (green)
    Hexagon hintBtnAB; // build alfa/beta game three (yellow)
    Hexagon hintBtnTF; // predicting with nnet (orange)
    Hexagon hintBtnHeur; // get heuristic value (purple)
    Hexagon prevBtn; // roll back last move (blue)
    Hexagon clearBtn; // clear table (red)

    explicit HexCanvas(QWidget *parent = 0);
    ~HexCanvas() Q_DECL_OVERRIDE;

    void setStateSpace(HexStateSpace* stateSpace); // simple setter rof game state
    short int getHexagonIndex(QPoint hit); // get hexagon index by cursor coordinates
    bool isHex(const QPoint &hit, const Hexagon &hex); // return true if the cursor in the hexagon
    void paintTriangle(const QPoint &a, const QPoint &b, const QPoint &c, const QColor& qcolor, QPainter& painter); // print board and buttons to the canvas
    void paintHex(const Hexagon &hexagon, const QColor &qcolor, QPainter& painter); // paints a single hexagon
    HexStateSpace::color getPlayerNextPlayer(); // set swap and return with player variable

    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE; // pain the board and the buttons
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE; // when user clicked recognizes the clicked hexagon
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE; // highlight pointed hexagon

signals:

public slots:
    void setMouseTrackingEnabled(); // qt tracking mouse coords
    void nextInfo(); // green button: cout next player's color (green)
    void hintAB(); // build alfa/beta game three (yellow)
    void hintTF(); // predicting with nnet (orange)
    void hintHeur(); // get heuristic value (purple)
    void prev(); // roll back last move (blue)
    void clear(); // clear table (red)

private:
    double getWallTime() const; // for measure the elapsed time
    //double getLimit(std::vector<double> v, int l) const;
    std::string toString(std::vector<double> &v) const;
    std::string toString(std::vector<short int> &v) const;
};

#endif // HEXCANVAS_H
