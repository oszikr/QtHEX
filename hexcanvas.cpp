#include "hexcanvas.h"

HexCanvas::HexCanvas(QWidget *parent) : QWidget(parent), hexagons(0)
{
}

void HexCanvas::setStateSpace(hexStateSpace::color* space, unsigned short int tablesize, int hexagonSize)
{
    this->SPACE = space;
    this->TABLESIZE = tablesize;
    this->HEXAGONSIZE = hexagonSize; //radius
    this->hexagons = new Hexagon[TABLESIZE*TABLESIZE];
    this->PADDING = 20;

    double w = 2 * HEXAGONSIZE;
    double h = std::sqrt(3) * HEXAGONSIZE;

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

    // TODO create borders
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

    i = 0;
    j = TABLESIZE - 1;
    Hexagon UpRtBigHexagon(
                QPoint(w/2 + j*(w/4*3) + PADDING,
                             (h/2 * TABLESIZE - j*h/2) + (i*h) + PADDING),
                HEXAGONSIZE + 4);
    upBorderPoints.push_back(UpRtBigHexagon.c);
    rtBorderPoints.push_front(UpRtBigHexagon.c);

    i = TABLESIZE - 1;
    j = 0;
    Hexagon DnLtBigHexagon(
                QPoint(w/2 + j*(w/4*3) + PADDING,
                             (h/2 * TABLESIZE - j*h/2) + (i*h) + PADDING),
                HEXAGONSIZE + 4);
    dnBorderPoints.push_front(DnLtBigHexagon.f);
    ltBorderPoints.push_back(DnLtBigHexagon.f);
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
            if(color == hexStateSpace::EMPTY) brush.setColor(QColor(226,226,226));
            else if(color == hexStateSpace::BLUE) brush.setColor(QColor(5,73,188));
            else if(color == hexStateSpace::RED) brush.setColor(QColor(184,20,9));
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

QSize HexCanvas::sizeHint() const
{
    double w = 2 * HEXAGONSIZE;
    double h = std::sqrt(3) * HEXAGONSIZE;
    return QSize(
                (w/4*3) * TABLESIZE + (w/4*1) + 2*PADDING,
                h * TABLESIZE + (TABLESIZE-1) * h/2 + 2*PADDING);
}

void HexCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint hit(event->x(), event->y());
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
        int n_snd = std::sqrt(n.x()*n.x()+n.y()*n.y());
        int n_hex = std::max(
                    abs(n.x()),
                    abs((n.x() + std::sqrt(3)*std::abs(n.y()))/2)
                    );
        if(n_hex > v_snd)
        {
            std::cout << ">>> Clicked hexagon is: " << i << "." << std::endl;
        }
    }
}
