#include "hexcanvas.h"

HexCanvas::HexCanvas(QWidget *parent) : QWidget(parent), HEXAGONSIZE(27), PADDING(20), pointed(-1),
    HEXAGONWIDTH(2 * HEXAGONSIZE), HEXAGONHEIGHT(std::sqrt(3) * HEXAGONSIZE)
{
    setMouseTrackingEnabledTimer = new QTimer(this);
    connect(setMouseTrackingEnabledTimer, SIGNAL(timeout()), this, SLOT(setMouseTrackingEnabled()));
}

void HexCanvas::setStateSpace(hexStateSpace* stateSpace)
{
    this->stateSpace = stateSpace;
    this->hexagons.clear();

    setFixedSize(QSize(
                (HEXAGONWIDTH/4*3) * stateSpace->SIZE + (HEXAGONWIDTH/4*1) + 2*PADDING,
                (HEXAGONHEIGHT*stateSpace->SIZE) + (HEXAGONHEIGHT/2*(stateSpace->SIZE-1)) + 2*PADDING));

    for(unsigned short int i = 0; i < stateSpace->SIZE; i++)
    {
        for(unsigned short int j = 0; j < stateSpace->SIZE; j++)
        {
            QPoint center(HEXAGONWIDTH/2 + j*(HEXAGONWIDTH/4*3) + PADDING,
                         (HEXAGONHEIGHT/2 * stateSpace->SIZE - j*HEXAGONHEIGHT/2) + (i*HEXAGONHEIGHT) + PADDING);
            hexagons.push_back(Hexagon(center, HEXAGONSIZE)); //hexagons[stateSpace->SIZE * i + j]

            hexagons[stateSpace->SIZE * i + j].mergeWithUp(stateSpace->SIZE, stateSpace->SIZE * i + j, hexagons);
            hexagons[stateSpace->SIZE * i + j].mergeWithLt(stateSpace->SIZE, stateSpace->SIZE * i + j, hexagons);
        }
    }

    // create borders
    upBorderPoints.clear();
    dnBorderPoints.clear();
    ltBorderPoints.clear();
    rtBorderPoints.clear();
    unsigned short int i = 0;
    for(unsigned short int j = 0; j < stateSpace->SIZE; j++)
    {
        QPoint center(HEXAGONWIDTH/2 + j*(HEXAGONWIDTH/4*3) + PADDING - 1,
                     (HEXAGONHEIGHT/2 * stateSpace->SIZE - j*HEXAGONHEIGHT/2) + (i*HEXAGONHEIGHT) + PADDING - 3);
        Hexagon hexagon(center, HEXAGONSIZE);
        upBorderPoints.push_back(hexagon.b);
        upBorderPoints.push_back(hexagon.c);
    }
    i = stateSpace->SIZE - 1;
    for(unsigned short int j = 0; j < stateSpace->SIZE; j++)
    {
        QPoint center(HEXAGONWIDTH/2 + j*(HEXAGONWIDTH/4*3) + PADDING + 1,
                     (HEXAGONHEIGHT/2 * stateSpace->SIZE - j*HEXAGONHEIGHT/2) + (i*HEXAGONHEIGHT) + PADDING + 3);
        Hexagon hexagon(center, HEXAGONSIZE);
        dnBorderPoints.push_back(hexagon.f);
        dnBorderPoints.push_back(hexagon.e);
    }
    unsigned short int j = 0;
    for(unsigned short int i = 0; i < stateSpace->SIZE; i++)
    {
        QPoint center(HEXAGONWIDTH/2 + j*(HEXAGONWIDTH/4*3) + PADDING - 3,
                     (HEXAGONHEIGHT/2 * stateSpace->SIZE - j*HEXAGONHEIGHT/2) + (i*HEXAGONHEIGHT) + PADDING);
        Hexagon hexagon(center, HEXAGONSIZE);
        ltBorderPoints.push_back(hexagon.a);
        ltBorderPoints.push_back(hexagon.f);
    }
    j = stateSpace->SIZE - 1;
    for(unsigned short int i = 0; i < stateSpace->SIZE; i++)
    {
        QPoint center(HEXAGONWIDTH/2 + j*(HEXAGONWIDTH/4*3) + PADDING + 3,
                     (HEXAGONHEIGHT/2 * stateSpace->SIZE - j*HEXAGONHEIGHT/2) + (i*HEXAGONHEIGHT) + PADDING);
        Hexagon hexagon(center, HEXAGONSIZE);
        rtBorderPoints.push_back(hexagon.c);
        rtBorderPoints.push_back(hexagon.d);
    }
    upBorderPoints.push_front(ltBorderPoints.first());
    rtBorderPoints.push_back(dnBorderPoints.last());

    upBorderPoints.back().setX( upBorderPoints.back().x() + 2);
    rtBorderPoints.front().setY( rtBorderPoints.front().y() - 2);
    rtBorderPoints.front().setX( rtBorderPoints.front().x() - 1);

    dnBorderPoints.front().setX( dnBorderPoints.front().x() - 2);
    ltBorderPoints.back().setY( ltBorderPoints.back().y() + 2);
    ltBorderPoints.back().setX( ltBorderPoints.back().x() + 1);

    setMouseTrackingEnabled();
}

HexCanvas::~HexCanvas()
{}

void HexCanvas::paintEvent(QPaintEvent *event)
{
    //std::cout << ">>> PaintEvent" << std::endl;
    // paint white canvas
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), Qt::white);

    // paint borders
    painter.setPen(QColor(5,73,188));
    painter.drawPolyline(upBorderPoints);
    painter.drawPolyline(dnBorderPoints);

    painter.setPen(QColor(184,20,9));
    painter.drawPolyline(ltBorderPoints);
    painter.drawPolyline(rtBorderPoints);

    // paint hexagons
    for(unsigned short int i = 0; i < stateSpace->SIZE; i++)
    {
        for(unsigned short int j = 0; j < stateSpace->SIZE; j++)
        {
            hexStateSpace::color color = stateSpace->space[stateSpace->SIZE * i + j];
            Hexagon hexagon = hexagons[stateSpace->SIZE * i + j];
            QVector<QPoint> hexagonPoints;
            hexagonPoints.push_back(hexagon.a);
            hexagonPoints.push_back(hexagon.b);
            hexagonPoints.push_back(hexagon.c);
            hexagonPoints.push_back(hexagon.d);
            hexagonPoints.push_back(hexagon.e);
            hexagonPoints.push_back(hexagon.f);

            // drawing hexagon
            QBrush brush;
            brush.setStyle(Qt::SolidPattern);
            if(stateSpace->SIZE * i + j == pointed)        brush.setColor(QColor(4, 188, 44));
            else if(color == hexStateSpace::EMPTY)  brush.setColor(QColor(226,226,226));
            else if(color == hexStateSpace::BLUE)   brush.setColor(QColor(5,73,188));
            else if(color == hexStateSpace::RED)    brush.setColor(QColor(184,20,9));
            painter.setBrush(brush);
            painter.drawPolygon(QPolygon(hexagonPoints));

            // drawing grid
            painter.setPen(Qt::white);
            painter.drawPolygon(QPolygon(hexagonPoints));
        }
    }

    QWidget::paintEvent(event);
    painter.end();
}

short int HexCanvas::getHexagonIndex(QPoint hit)
{
    for(unsigned short int i = 0; i < stateSpace->SIZE * stateSpace->SIZE; i++)
    {
        Hexagon hex = hexagons[i];
        QVector2D v(
                    hit.x() - hex.center.x(),
                    hit.y() - hex.center.y()
                    );
        int v_snd = std::sqrt(v.x()*v.x()+v.y()*v.y());
        QVector2D n(
                    HEXAGONSIZE*v.x()/v_snd,
                    HEXAGONSIZE*v.y()/v_snd
                    );
        int n_hex = std::max(
                    abs(n.x()),
                    abs((n.x() + std::sqrt(3)*std::abs(n.y()))/2)
                    );
        if(n_hex > v_snd)
        {
            return i;
        }
    }
    return -1;
}

void HexCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    pointed = getHexagonIndex(QPoint(event->x(), event->y()));
    std::cout << ">>> Clicked hexagon is: " << pointed << "." << std::endl;
}

void HexCanvas::mouseMoveEvent(QMouseEvent *event)
{
    pointed = getHexagonIndex(QPoint(event->x(), event->y()));
    //std::cout << ">>> Pointed hexagon is: " << event->x() << "\t" << event->y() << "." << std::endl;
    setMouseTracking(false);
    setMouseTrackingEnabledTimer->start(50);
    update();
}

void HexCanvas::setMouseTrackingEnabled()
{
    setMouseTracking(true);
}
