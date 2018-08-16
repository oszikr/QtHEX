#include "hexcanvas.h"

HexCanvas::HexCanvas(QWidget *parent) : QWidget(parent)
{
}

void HexCanvas::setStateSpace(hexStateSpace::color* space, unsigned short int tablesize, int hexagonSize)
{
    this->SPACE = space;
    this->TABLESIZE = tablesize;
    this->HEXAGONSIZE = hexagonSize;
}

void HexCanvas::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.fillRect(event->rect(), Qt::white);

    double w = 2 * HEXAGONSIZE;
    double h = std::sqrt(3) * HEXAGONSIZE;

    for(unsigned short int i = 0; i < TABLESIZE; i++)
    {
        for(unsigned short int j = 0; j < TABLESIZE; j++)
        {
            QPoint center(w/2 + j*(w/4*3),
                         (h/2 * TABLESIZE - j*h/2) + (i*h));
            for(int i = 0; i < 6; i++)
            {
                painter.drawLine(flatHexCorner(center, HEXAGONSIZE, i),
                                 flatHexCorner(center, HEXAGONSIZE, i+1));
            }
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

QPoint HexCanvas::flatHexCorner(QPoint& center, unsigned short int size, unsigned short int i) const
{
    int angle_deg = 60 * i;
    double angle_rad = M_PI / 180 * angle_deg;
    return QPoint(center.x() + size * std::cos(angle_rad),
                  center.y() + size * std::sin(angle_rad));
}
