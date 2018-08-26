#include "hexcanvas.h"

HexCanvas::HexCanvas(QWidget *parent) : QWidget(parent), HEXAGONSIZE(27), PADDING(20), pointed(-1),
    HEXAGONWIDTH(2 * HEXAGONSIZE), HEXAGONHEIGHT(std::sqrt(3) * HEXAGONSIZE), player(hexStateSpace::BLUE)
{
    setMouseTrackingEnabledTimer = new QTimer(this);
    connect(setMouseTrackingEnabledTimer, SIGNAL(timeout()), this, SLOT(setMouseTrackingEnabled()));
}

void HexCanvas::setStateSpace(hexStateSpace* stateSpace)
{
    this->stateSpace = stateSpace;
    this->hexagons.clear();

    setFixedSize(QSize(
                (HEXAGONWIDTH/4*3) * stateSpace->getSize() + (HEXAGONWIDTH/4*1) + 2*PADDING,
                (HEXAGONHEIGHT*stateSpace->getSize()) + (HEXAGONHEIGHT/2*(stateSpace->getSize()-1)) + 2*PADDING));

    for(unsigned short int i = 0; i < stateSpace->getSize(); i++)
    {
        for(unsigned short int j = 0; j < stateSpace->getSize(); j++)
        {
            QPoint center(HEXAGONWIDTH/2 + j*(HEXAGONWIDTH/4*3) + PADDING,
                         (HEXAGONHEIGHT/2 * stateSpace->getSize() - j*HEXAGONHEIGHT/2) + (i*HEXAGONHEIGHT) + PADDING);
            hexagons.push_back(Hexagon(center, HEXAGONSIZE)); //hexagons[stateSpace->getSize() * i + j]

            hexagons[stateSpace->getSize() * i + j].mergeWithUp(stateSpace->getSize(), stateSpace->getSize() * i + j, hexagons);
            hexagons[stateSpace->getSize() * i + j].mergeWithLt(stateSpace->getSize(), stateSpace->getSize() * i + j, hexagons);
        }
    }

    // create borders
    upBorderPoints.clear();
    dnBorderPoints.clear();
    ltBorderPoints.clear();
    rtBorderPoints.clear();
    unsigned short int i = 0;
    for(unsigned short int j = 0; j < stateSpace->getSize(); j++)
    {
        QPoint center(HEXAGONWIDTH/2 + j*(HEXAGONWIDTH/4*3) + PADDING - 1,
                     (HEXAGONHEIGHT/2 * stateSpace->getSize() - j*HEXAGONHEIGHT/2) + (i*HEXAGONHEIGHT) + PADDING - 3);
        Hexagon hexagon(center, HEXAGONSIZE);
        upBorderPoints.push_back(hexagon.b);
        upBorderPoints.push_back(hexagon.c);
    }
    i = stateSpace->getSize() - 1;
    for(unsigned short int j = 0; j < stateSpace->getSize(); j++)
    {
        QPoint center(HEXAGONWIDTH/2 + j*(HEXAGONWIDTH/4*3) + PADDING + 1,
                     (HEXAGONHEIGHT/2 * stateSpace->getSize() - j*HEXAGONHEIGHT/2) + (i*HEXAGONHEIGHT) + PADDING + 3);
        Hexagon hexagon(center, HEXAGONSIZE);
        dnBorderPoints.push_back(hexagon.f);
        dnBorderPoints.push_back(hexagon.e);
    }
    unsigned short int j = 0;
    for(unsigned short int i = 0; i < stateSpace->getSize(); i++)
    {
        QPoint center(HEXAGONWIDTH/2 + j*(HEXAGONWIDTH/4*3) + PADDING - 3,
                     (HEXAGONHEIGHT/2 * stateSpace->getSize() - j*HEXAGONHEIGHT/2) + (i*HEXAGONHEIGHT) + PADDING);
        Hexagon hexagon(center, HEXAGONSIZE);
        ltBorderPoints.push_back(hexagon.a);
        ltBorderPoints.push_back(hexagon.f);
    }
    j = stateSpace->getSize() - 1;
    for(unsigned short int i = 0; i < stateSpace->getSize(); i++)
    {
        QPoint center(HEXAGONWIDTH/2 + j*(HEXAGONWIDTH/4*3) + PADDING + 3,
                     (HEXAGONHEIGHT/2 * stateSpace->getSize() - j*HEXAGONHEIGHT/2) + (i*HEXAGONHEIGHT) + PADDING);
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

    // func buttons
    QPoint tmp(hexagons[0].center.x(), hexagons[stateSpace->getSize()-1].center.y());
    nextInfoBtn = Hexagon(tmp, HEXAGONSIZE);
    tmp.setX(tmp.x() + 3 * HEXAGONSIZE);
    hintBtn = Hexagon(tmp, HEXAGONSIZE);
    tmp.setX(tmp.x() + 3 * HEXAGONSIZE);
    prevBtn = Hexagon(tmp, HEXAGONSIZE);
    tmp.setX(tmp.x() + 3 * HEXAGONSIZE);
    clearBtn = Hexagon(tmp, HEXAGONSIZE);

    setMouseTrackingEnabled();
}

HexCanvas::~HexCanvas()
{}

hexStateSpace::color HexCanvas::getPlayerNextPlayer()
{
    hexStateSpace::color prevPlayer = player;
    if(player == hexStateSpace::BLUE)
        player = hexStateSpace::RED;
    else
        player = hexStateSpace::BLUE;
    return prevPlayer;
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
    for(unsigned short int i = 0; i < stateSpace->getSize(); i++)
    {
        for(unsigned short int j = 0; j < stateSpace->getSize(); j++)
        {
            hexStateSpace::color color = stateSpace->getSpace()[stateSpace->getSize() * i + j];
            Hexagon hexagon = hexagons[stateSpace->getSize() * i + j];
            QColor qcolor;
            if(color == hexStateSpace::BLUE)
                qcolor = QColor(5,73,188);
            else if(color == hexStateSpace::RED)
                qcolor = QColor(184,20,9);
            else if(stateSpace->getSize() * i + j == pointed)
            {
                if(player == hexStateSpace::BLUE)
                    qcolor = QColor(5,73,188);
                else
                    qcolor = QColor(184,20,9);
            }
            else if(color == hexStateSpace::EMPTY)
                qcolor = QColor(226,226,226);
            paintHex(hexagon, qcolor, painter);

        }
        paintHex(hintBtn,       QColor(245,245,0), painter);
        paintHex(nextInfoBtn,   QColor(4,188,44),  painter);
        paintHex(prevBtn,       QColor(5,73,188),  painter);
        paintHex(clearBtn,      QColor(184,20,9),  painter);
    }

    QWidget::paintEvent(event);
    painter.end();
}

void HexCanvas::paintHex(const Hexagon& hexagon, const QColor& qcolor, QPainter& painter)
{
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
    brush.setColor(qcolor);

    painter.setBrush(brush);
    painter.drawPolygon(QPolygon(hexagonPoints));
    // drawing grid
    painter.setPen(Qt::white);
    painter.drawPolygon(QPolygon(hexagonPoints));

}

short int HexCanvas::getHexagonIndex(QPoint hit)
{
    for(unsigned short int i = 0; i < stateSpace->getSize() * stateSpace->getSize(); i++)
    {
        Hexagon hex = hexagons[i];
        if(isHex(hit, hex))
        {
            return i;
        }
    }
    return -1;
}

bool HexCanvas::isHex(const QPoint& hit, const Hexagon& hex)
{
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
        return true;
    else
        return false;
}

void HexCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint hit = QPoint(event->x(), event->y());
    pointed = getHexagonIndex(hit);
    if(pointed >= 0 && stateSpace->get(pointed) == stateSpace->EMPTY)
    {
        std::cout << ">>> Clicked hexagon is: " << pointed << "." << std::endl;
        stateSpace->set(pointed, player);
        getPlayerNextPlayer();
        update();
    }
    else
    {
        if(isHex(hit, nextInfoBtn))     nextInfo();
        else if(isHex(hit, hintBtn))    hint();
        else if(isHex(hit, prevBtn))    prev();
        else if(isHex(hit, clearBtn))   clear();
    }

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

void HexCanvas::hint()
{
    std::cout << "Hit is not implemented yet." << std::endl;
}

void HexCanvas::nextInfo()
{
    std::cout << "The next player is the " << (player == hexStateSpace::BLUE ? "\e[0;34mBlue" : "\e[0;31mRed") << "\e[m." << std::endl;
}

void HexCanvas::prev()
{
    if(stateSpace->getLast() == -1) return;
    if(stateSpace->get(stateSpace->getLast()) == hexStateSpace::EMPTY) return;
    stateSpace->set(stateSpace->getLast(), hexStateSpace::EMPTY);
    getPlayerNextPlayer();
    update();
}

void HexCanvas::clear()
{
    stateSpace->clear();
    player = hexStateSpace::BLUE;
    update();
}

