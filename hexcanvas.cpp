#include "hexcanvas.h"

HexCanvas::HexCanvas(QWidget *parent) : QWidget(parent), HEXAGONSIZE(27), pointed(-1), hexagons(0)
{
    setMouseTrackingEnabledTimer = new QTimer(this);
    connect(setMouseTrackingEnabledTimer, SIGNAL(timeout()), this, SLOT(setMouseTrackingEnabled()));
}

void HexCanvas::setStateSpace(hexStateSpace& space)
{
    this->SPACE = space.space;
    this->TABLESIZE = space.SIZE;
    this->hexagons = new Hexagon[TABLESIZE*TABLESIZE];
    this->PADDING = 20;

    double w = 2 * HEXAGONSIZE;
    double h = std::sqrt(3) * HEXAGONSIZE;

    setFixedSize(QSize(
                (w/4*3) * TABLESIZE + (w/4*1) + 2*PADDING,
                (h*TABLESIZE) + (h/2*(TABLESIZE-1)) + 2*PADDING));

    for(unsigned short int i = 0; i < TABLESIZE; i++)
    {
        for(unsigned short int j = 0; j < TABLESIZE; j++)
        {
            QPoint center(w/2 + j*(w/4*3) + PADDING,
                         (h/2 * TABLESIZE - j*h/2) + (i*h) + PADDING);
            hexagons[TABLESIZE * i + j] = Hexagon(center, HEXAGONSIZE);

            hexagons[TABLESIZE * i + j].mergeWithUp(TABLESIZE, TABLESIZE * i + j, hexagons);
            hexagons[TABLESIZE * i + j].mergeWithLt(TABLESIZE, TABLESIZE * i + j, hexagons);
        }
    }

    // create borders
    upBorderPoints.clear();
    dnBorderPoints.clear();
    ltBorderPoints.clear();
    rtBorderPoints.clear();
    unsigned short int i = 0;
    for(unsigned short int j = 0; j < TABLESIZE; j++)
    {
        QPoint center(w/2 + j*(w/4*3) + PADDING - 1,
                     (h/2 * TABLESIZE - j*h/2) + (i*h) + PADDING - 3);
        Hexagon hexagon(center, HEXAGONSIZE);
        upBorderPoints.push_back(hexagon.b);
        upBorderPoints.push_back(hexagon.c);
    }
    i = TABLESIZE - 1;
    for(unsigned short int j = 0; j < TABLESIZE; j++)
    {
        QPoint center(w/2 + j*(w/4*3) + PADDING + 1,
                     (h/2 * TABLESIZE - j*h/2) + (i*h) + PADDING + 3);
        Hexagon hexagon(center, HEXAGONSIZE);
        dnBorderPoints.push_back(hexagon.f);
        dnBorderPoints.push_back(hexagon.e);
    }
    unsigned short int j = 0;
    for(unsigned short int i = 0; i < TABLESIZE; i++)
    {
        QPoint center(w/2 + j*(w/4*3) + PADDING - 3,
                     (h/2 * TABLESIZE - j*h/2) + (i*h) + PADDING);
        Hexagon hexagon(center, HEXAGONSIZE);
        ltBorderPoints.push_back(hexagon.a);
        ltBorderPoints.push_back(hexagon.f);
    }
    j = TABLESIZE - 1;
    for(unsigned short int i = 0; i < TABLESIZE; i++)
    {
        QPoint center(w/2 + j*(w/4*3) + PADDING + 3,
                     (h/2 * TABLESIZE - j*h/2) + (i*h) + PADDING);
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
{
    if(hexagons != 0)
    {
        delete[] hexagons;
    }
}

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
    for(unsigned short int i = 0; i < TABLESIZE; i++)
    {
        for(unsigned short int j = 0; j < TABLESIZE; j++)
        {
            hexStateSpace::color color = SPACE[TABLESIZE * i + j];
            Hexagon hexagon = hexagons[TABLESIZE * i + j];
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
            if(TABLESIZE * i + j == pointed)        brush.setColor(QColor(4, 188, 44));
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
    for(unsigned short int i = 0; i < TABLESIZE * TABLESIZE; i++)
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
