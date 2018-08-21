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
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), Qt::white);

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
            if(color == hexStateSpace::EMPTY) brush.setColor(Qt::gray);
            else if(color == hexStateSpace::BLUE) brush.setColor(Qt::blue);
            else if(color == hexStateSpace::RED) brush.setColor(Qt::red);
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
                (w/4*3) * TABLESIZE + (w/4*1),
                h * TABLESIZE + (TABLESIZE-1) * h/2);
}

void HexCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint hit(event->x(), event->y());
    //std::cout << "Clicked: " << hit.x() << ", " << hit.y() << std::endl;
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
        int n_inf = std::max(
                    abs(v.x()),
                    abs(v.y())
                    );
        int n_hex = std::max(
                    abs(n.x()),
                    abs((n.x() + std::sqrt(3)*std::abs(n.y()))/2)
                    );
        /*
        std::cout << "\t" << i << ". hexagon: " << hex.center.x() << ", " << hex.center.y() << std::endl;
        std::cout << "\t" << "v      : " << v.x() << ", " << v.y() << std::endl;
        std::cout << "\t" << "||v||^2: " << v_snd << std::endl;
        std::cout << "\t" << "n      : " << n.x() << ", " << n.y() << std::endl;
        std::cout << "\t" << "||n||^2: " << n_snd << std::endl;
        std::cout << "\t" << "||n||^i: " << n_inf << std::endl;
        std::cout << "\t" << "||n||^h: " << n_hex << std::endl;
        */
        if(n_hex > v_snd)
        {
            std::cout << ">>> Clicked hexagon is: " << i << "." << std::endl;
        }
    }
}
