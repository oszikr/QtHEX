#include "hexcanvas.h"

HexCanvas::HexCanvas(QWidget *parent) : QWidget(parent), hexagons(0)
{
}

void HexCanvas::setStateSpace(hexStateSpace::color* space, unsigned short int tablesize, int hexagonSize)
{
    this->SPACE = space;
    this->TABLESIZE = tablesize;
    this->HEXAGONSIZE = hexagonSize;
    this->hexagons = new Hexagon[TABLESIZE*TABLESIZE];

    double w = 2 * HEXAGONSIZE;
    double h = std::sqrt(3) * HEXAGONSIZE;

    for(unsigned short int i = 0; i < TABLESIZE; i++)
    {
        for(unsigned short int j = 0; j < TABLESIZE; j++)
        {
            QPoint center(w/2 + j*(w/4*3),
                         (h/2 * TABLESIZE - j*h/2) + (i*h));
            hexagons[TABLESIZE * i + j] = Hexagon(center, HEXAGONSIZE);

            hexagons[TABLESIZE * i + j].mergeWithUp(TABLESIZE, TABLESIZE * i + j, hexagons);
            hexagons[TABLESIZE * i + j].mergeWithLt(TABLESIZE, TABLESIZE * i + j, hexagons);
        }
    }
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
    QPainter painter;
    painter.begin(this);
    painter.fillRect(event->rect(), Qt::white);

    for(unsigned short int i = 0; i < TABLESIZE; i++)
    {
        for(unsigned short int j = 0; j < TABLESIZE; j++)
        {
            Hexagon h = hexagons[TABLESIZE * i + j];
            painter.drawLine(h.a, h.b);
            painter.drawLine(h.b, h.c);
            painter.drawLine(h.c, h.d);
            painter.drawLine(h.d, h.e);
            painter.drawLine(h.e, h.f);
            painter.drawLine(h.f, h.a);
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
                (w/4*3) * TABLESIZE + (w/4*1),
                h * TABLESIZE + (TABLESIZE-1) * h/2);
}
