#include "hexcanvas.h"

HexCanvas::HexCanvas(QWidget *parent) : QWidget(parent), HEXAGONSIZE(27), PADDING(20), pointed(-1),
    HEXAGONWIDTH(std::sqrt(3) * HEXAGONSIZE), HEXAGONHEIGHT(2 * HEXAGONSIZE), player(HexStateSpace::BLUE)
{
    setMouseTrackingEnabledTimer = new QTimer(this);
    connect(setMouseTrackingEnabledTimer, SIGNAL(timeout()), this, SLOT(setMouseTrackingEnabled()));
    nnetctrl = new HexNnetControl(this);
}

void HexCanvas::setStateSpace(HexStateSpace* stateSpace)
{
    this->stateSpace = stateSpace;

    if(stateSpace->getSize() == 13)
    {
        nnetctrl->Start(stateSpace);
    }
    else
    {
        std::cout << "Python NNET process did not started, because table size is not 13." << std::endl;
    }

    this->hexagons.clear();

    //if(stateSpace->getSize() < 9) PADDING += (9-stateSpace->getSize())*HEXAGONSIZE;

    setFixedSize(QSize(
                HEXAGONWIDTH * stateSpace->getSize() + (HEXAGONWIDTH * (stateSpace->getSize()-1) / 2) + 2*PADDING,
                HEXAGONHEIGHT * stateSpace->getSize() - (HEXAGONHEIGHT * 1 / 4) * (stateSpace->getSize() - 2)  + 2*PADDING));

    for(unsigned short int i = 0; i < stateSpace->getSize(); i++)
    {
        for(unsigned short int j = 0; j < stateSpace->getSize(); j++)
        {
            QPoint center(PADDING + HEXAGONWIDTH/2  + j*HEXAGONWIDTH + i*HEXAGONWIDTH/2,
                         (PADDING + HEXAGONHEIGHT/2 + i*HEXAGONHEIGHT*3/4));
            hexagons.push_back(Hexagon(center, HEXAGONSIZE));

            hexagons[stateSpace->getSize() * i + j].mergeWithLt(stateSpace->getSize(), stateSpace->getSize() * i + j, hexagons);
            hexagons[stateSpace->getSize() * i + j].mergeWithUp(stateSpace->getSize(), stateSpace->getSize() * i + j, hexagons);
            hexagons[stateSpace->getSize() * i + j].mergeWithTop(stateSpace->getSize(), stateSpace->getSize() * i + j, hexagons);
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
        QPoint center(PADDING + HEXAGONWIDTH/2  + j*HEXAGONWIDTH + i*HEXAGONWIDTH/2,
                     (PADDING-3 + HEXAGONHEIGHT/2 + i*HEXAGONHEIGHT*3/4));
        Hexagon hexagon(center, HEXAGONSIZE);
        upBorderPoints.push_back(hexagon.d);
        upBorderPoints.push_back(hexagon.e);
    }
    i = stateSpace->getSize() - 1;
    for(unsigned short int j = 0; j < stateSpace->getSize(); j++)
    {
        QPoint center(PADDING+1 + HEXAGONWIDTH/2  + j*HEXAGONWIDTH + i*HEXAGONWIDTH/2,
                     (PADDING+3 + HEXAGONHEIGHT/2 + i*HEXAGONHEIGHT*3/4));
        Hexagon hexagon(center, HEXAGONSIZE);
        dnBorderPoints.push_back(hexagon.b);
        dnBorderPoints.push_back(hexagon.a);
    }
    unsigned short int j = 0;
    for(unsigned short int i = 0; i < stateSpace->getSize(); i++)
    {
        QPoint center(PADDING-3 + HEXAGONWIDTH/2  + j*HEXAGONWIDTH + i*HEXAGONWIDTH/2,
                     (PADDING+1 + HEXAGONHEIGHT/2 + i*HEXAGONHEIGHT*3/4));
        Hexagon hexagon(center, HEXAGONSIZE);
        ltBorderPoints.push_back(hexagon.d);
        ltBorderPoints.push_back(hexagon.c);
    }
    j = stateSpace->getSize() - 1;
    for(unsigned short int i = 0; i < stateSpace->getSize(); i++)
    {
        QPoint center(PADDING+3 + HEXAGONWIDTH/2  + j*HEXAGONWIDTH + i*HEXAGONWIDTH/2,
                     (PADDING-1+ HEXAGONHEIGHT/2 + i*HEXAGONHEIGHT*3/4));
        Hexagon hexagon(center, HEXAGONSIZE);
        rtBorderPoints.push_back(hexagon.f);
        rtBorderPoints.push_back(hexagon.a);
    }
    upBorderPoints.push_front(ltBorderPoints.first());
    rtBorderPoints.push_back(dnBorderPoints.last());

    upBorderPoints.push_back(rtBorderPoints.first());
    ltBorderPoints.push_back(dnBorderPoints.first());

    // func buttons
    QPoint tmp(
            hexagons[0].center.x(),
            hexagons[0].center.y()
    );

    tmp.setY(tmp.y() + 2.5*HEXAGONHEIGHT);
    nextInfoBtn = Hexagon(tmp, HEXAGONSIZE);

    tmp.setY(tmp.y() + HEXAGONHEIGHT*3/4);
    tmp.setX(tmp.x() + HEXAGONWIDTH/2);
    hintBtn = Hexagon(tmp, HEXAGONSIZE);

    tmp.setY(tmp.y() + HEXAGONHEIGHT*3/4);
    tmp.setX(tmp.x() + HEXAGONWIDTH/2);
    hintBtnTF = Hexagon(tmp, HEXAGONSIZE);

    tmp.setY(tmp.y() + HEXAGONHEIGHT*3/4);
    tmp.setX(tmp.x() + HEXAGONWIDTH/2);
    prevBtn = Hexagon(tmp, HEXAGONSIZE);

    tmp.setY(tmp.y() + HEXAGONHEIGHT*3/4);
    tmp.setX(tmp.x() + HEXAGONWIDTH/2);
    clearBtn = Hexagon(tmp, HEXAGONSIZE);

    setMouseTrackingEnabled();
}

HexCanvas::~HexCanvas()
{}

HexStateSpace::color HexCanvas::getPlayerNextPlayer()
{
    HexStateSpace::color prevPlayer = player;
    if(player == HexStateSpace::BLUE)
        player = HexStateSpace::RED;
    else
        player = HexStateSpace::BLUE;
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
    /*paintTriangle(
                QPoint(hexagons[0].center.x() - HEXAGONSIZE - 5, hexagons[0].center.y() - HEXAGONSIZE - 5),
                QPoint(hexagons[stateSpace->getSize()-1].center.x() + HEXAGONSIZE + 5, hexagons[stateSpace->getSize()-1].center.y() - HEXAGONSIZE - 5),
                QPoint(hexagons[stateSpace->getLength()-1].center.x() + HEXAGONSIZE + 5, hexagons[stateSpace->getLength()-1].center.y() + HEXAGONSIZE + 5),
                QColor(184,20,9),
                painter);*/

    // paint hexagons
    for(unsigned short int i = 0; i < stateSpace->getSize(); i++)
    {
        for(unsigned short int j = 0; j < stateSpace->getSize(); j++)
        {
            HexStateSpace::color color = stateSpace->getSpace()[stateSpace->getSize() * i + j];
            Hexagon hexagon = hexagons[stateSpace->getSize() * i + j];
            QColor qcolor;
            if(color == HexStateSpace::BLUE)
                qcolor = QColor(5,73,188);
            else if(color == HexStateSpace::RED)
                qcolor = QColor(184,20,9);
            else if(stateSpace->getSize() * i + j == pointed)
            {
                if(player == HexStateSpace::BLUE)
                    qcolor = QColor(5,73,188);
                else
                    qcolor = QColor(184,20,9);
            }
            else if(color == HexStateSpace::EMPTY)
                qcolor = QColor(226,226,226);
            paintHex(hexagon, qcolor, painter);

        }
        paintHex(hintBtn,       QColor(245,245,0), painter);
        paintHex(hintBtnTF,     QColor(255,165,0),  painter);
        paintHex(nextInfoBtn,   QColor(4,188,44),  painter);
        paintHex(prevBtn,       QColor(5,73,188),  painter);
        paintHex(clearBtn,      QColor(184,20,9),  painter);
    }

    painter.setPen(QColor(184,20,9));
    painter.drawPolyline(upBorderPoints);
    painter.drawPolyline(dnBorderPoints);

    painter.setPen(QColor(5,73,188));
    painter.drawPolyline(ltBorderPoints);
    painter.drawPolyline(rtBorderPoints);
    QWidget::paintEvent(event);
    painter.end();
}

/*void HexCanvas::paintTriangle(const QPoint& a, const QPoint& b, const QPoint& c, const QColor& qcolor, QPainter& painter)
{
     QVector<QPoint> points;
     points.push_back(a);
     points.push_back(b);
     points.push_back(c);

     QBrush brush;
     brush.setStyle(Qt::SolidPattern);
     brush.setColor(qcolor);

     painter.setBrush(brush);
     painter.drawPolygon(QPolygon(points));
}*/

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
                abs((double)n.x()),
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
        if(stateSpace->isWinner()) return;
        std::cout << ">>> Clicked hexagon is: " << pointed << "." << std::endl;
        stateSpace->set(pointed, player);
        if(!stateSpace->isWinner())
            getPlayerNextPlayer();
        else
            std::cout << "The " << (player == HexStateSpace::BLUE ? "\e[0;34mBlue" : "\e[0;31mRed") << "\e[m player won." << std::endl;
        update();
    }
    else
    {
        if(isHex(hit, nextInfoBtn))     nextInfo();
        else if(isHex(hit, hintBtn))    hint();
        else if(isHex(hit, hintBtnTF))  hintTF();
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
    HexStrategyControl ctrl(*stateSpace, player, (player == HexStateSpace::BLUE ? HexStateSpace::RED : HexStateSpace::BLUE) );

    double startTime = getWallTime();
    short int hint = ctrl.getWinningStep();
    double meansurement = getWallTime() - startTime;
    std::cout << "Elapse time: " << meansurement << std::endl;

    if(hint >= 0)
    {
        std::cout << "The winning field's array index is: " << hint << std::endl;
        std::cout << "The winning field's matrix index is: [" <<
                     hint/stateSpace->getSize()+1 << ". row, " << hint%stateSpace->getSize()+1 << ". col]" << std::endl;
    }
    else
    {
        std::cout << "You can not win: " << hint << std::endl;
    }
}

void HexCanvas::hintTF()
{
    std::cout << "Using NNET" << std::endl;
    if(nnetctrl->getState().compare("INPUT") == 0)
    {
        nnetctrl->hintTF(stateSpace, player);
    }
    else {
        std::cout << "NNET is not loaded yet." << std::endl;
    }
}

void HexCanvas::nextInfo()
{
    std::cout << "The next player is the " << (player == HexStateSpace::BLUE ? "\e[0;34mBlue" : "\e[0;31mRed") << "\e[m." << std::endl;
}

void HexCanvas::prev()
{
    if(stateSpace->getLast() == -1) return;
    if(stateSpace->get(stateSpace->getLast()) == HexStateSpace::EMPTY) return;
    stateSpace->set(stateSpace->getLast(), HexStateSpace::EMPTY);
    getPlayerNextPlayer();
    update();
}

void HexCanvas::clear()
{
    stateSpace->clear();
    player = HexStateSpace::BLUE;
    update();
}

//Get elapsed time for windows/linux
#ifdef _WIN32
#include <windows.h>
double HexCanvas::getWallTime() const
{
    LARGE_INTEGER time,freq;
    if (!QueryPerformanceFrequency(&freq))
    {
        return 0; //Handle error
    }
    if (!QueryPerformanceCounter(&time))
    {
        return 0; //Handle error
    }
    return (double)time.QuadPart / freq.QuadPart;
}
#else
#include <sys/time.h>
double HexCanvas::getWallTime() const
{
    struct timeval time;
    if (gettimeofday(&time, 0))
    {
        return 0; //Handle error
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
#endif
