#include "hexcanvas.h"

HexCanvas::HexCanvas(QWidget *parent) : QWidget(parent), HEXAGONSIZE(27), PADDING(20), pointed(-1),
    HEXAGONWIDTH(std::sqrt(3) * HEXAGONSIZE), HEXAGONHEIGHT(2 * HEXAGONSIZE), player(HexStateSpace::BLUE)
{
    setMouseTrackingEnabledTimer = new QTimer(this);
    connect(setMouseTrackingEnabledTimer, SIGNAL(timeout()), this, SLOT(setMouseTrackingEnabled()));
    nnetctrl = new HexNnetControl(this);
    setWindowTitle("HEX");
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

    setGeometry(0,0,
                HEXAGONWIDTH * stateSpace->getSize() + (HEXAGONWIDTH * (stateSpace->getSize()-1) / 2) + 2*PADDING,
                HEXAGONHEIGHT * stateSpace->getSize() - (HEXAGONHEIGHT * 1 / 4) * (stateSpace->getSize() - 2)  + 2*PADDING);

    /*if(stateSpace->getSize() < 9)
    {
        this->setFixedSize(QSize(this->width(), this->height()+200));
    }
    if(stateSpace->getSize() == 7)
    {
        this->setFixedSize(QSize(this->width(), 340));
    }*/

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
    if(PAINTBORDERS)
    {
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
    }

    // func buttons
    QPoint tmp(
            hexagons[0].center.x(),
            hexagons[0].center.y()
    );

    tmp.setY(tmp.y() + 2.5*HEXAGONHEIGHT);
    nextInfoBtn = Hexagon(tmp, HEXAGONSIZE);

    tmp.setY(tmp.y() + HEXAGONHEIGHT*3/4);
    tmp.setX(tmp.x() + HEXAGONWIDTH/2);
    hintBtnAB = Hexagon(tmp, HEXAGONSIZE);

    tmp.setY(tmp.y() + HEXAGONHEIGHT*3/4);
    tmp.setX(tmp.x() + HEXAGONWIDTH/2);
    hintBtnTF = Hexagon(tmp, HEXAGONSIZE);

    tmp.setY(tmp.y() + HEXAGONHEIGHT*3/4);
    tmp.setX(tmp.x() + HEXAGONWIDTH/2);
    hintBtnHeur = Hexagon(tmp, HEXAGONSIZE);

    tmp.setY(tmp.y() + HEXAGONHEIGHT*3/4);
    tmp.setX(tmp.x() + HEXAGONWIDTH/2);
    prevBtn = Hexagon(tmp, HEXAGONSIZE);

    tmp.setY(tmp.y() + HEXAGONHEIGHT*3/4);
    tmp.setX(tmp.x() + HEXAGONWIDTH/2);
    clearBtn = Hexagon(tmp, HEXAGONSIZE);

    tmp.setY(tmp.y() + HEXAGONHEIGHT*3/4);
    tmp.setX(tmp.x() + HEXAGONWIDTH/2);
    loadGameBtn = Hexagon(tmp, HEXAGONSIZE);
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
        if(PAINTFUNCBUTTONS) {
            paintHex(hintBtnAB,     QColor(245,245,0), painter);
            paintHex(hintBtnTF,     QColor(255,165,0), painter);
            paintHex(hintBtnHeur,   QColor(128,0,128), painter);
            paintHex(nextInfoBtn,   QColor(4,188,44),  painter);
            paintHex(prevBtn,       QColor(5,73,188),  painter);
            paintHex(clearBtn,      QColor(184,20,9),  painter);
            paintHex(loadGameBtn,   QColor(127,127,127),  painter);
        }
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
        getPlayerNextPlayer();
        if(stateSpace->isWinner())
            std::cout << "The " << (player != HexStateSpace::BLUE ? "\e[0;34mBlue" : "\e[0;31mRed") << "\e[m player won." << std::endl;
        update();
    }
    else
    {
        if(isHex(hit, nextInfoBtn))         nextInfo();
        else if(isHex(hit, hintBtnAB))      hintAB();
        else if(isHex(hit, hintBtnTF))      hintTF();
        else if(isHex(hit, hintBtnHeur))    hintHeur();
        else if(isHex(hit, prevBtn))        prev();
        else if(isHex(hit, clearBtn))       clear();
        else if(isHex(hit, loadGameBtn))    loadGame();
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

void HexCanvas::hintAB()
{  
    std::cout << "Using Game tree" << std::endl;
    HexABcontrol ctrl(*stateSpace, player, (player == HexStateSpace::BLUE ? HexStateSpace::RED : HexStateSpace::BLUE) );

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
        std::cout << "NNET is not loaded yet. (nnetctrl in stage "<< nnetctrl->getState() << ")" << std::endl;
    }
}

void HexCanvas::hintHeur()
{
    std::vector<double> nHints;
    std::cout << "Using NNET" << std::endl;
    if(nnetctrl->getState().compare("INPUT") == 0)
    {
        nnetctrl->hintTF(stateSpace, player);
    }
    else {
        std::cout << "NNET is not loaded yet. (nnetctrl in stage "<< nnetctrl->getState() << ")" << std::endl;
    }

    std::cout << "Using Heuristic. Current state info:" << std::endl;
    HexMinMaxControl ctrl(*stateSpace, player, (player == HexStateSpace::BLUE ? HexStateSpace::RED : HexStateSpace::BLUE), 2);
    double startTime = getWallTime();
    std::vector<short int> hHints = ctrl.getWinningStep();
    double meansurement = getWallTime() - startTime;
    std::cout << "Elapse time: " << meansurement << std::endl;

    nHints = nnetctrl->resoultVect;
    std::cout << "hHints.size(): " << hHints.size() << std::endl;
    std::cout << "nHints.size(): " << nHints.size() << std::endl;
    std::vector<double> cHints;

    if(nHints.size() != 0)
    {
        for(size_t i = 0; i < hHints.size(); i++)
        {
            double n = nHints[i];
            double h = hHints[i];
            double c;
            if(h == -1000) {
                c = 1000;
            }
            else if(h == -1000) {
                c = 0;
            }
            else {
                c = (100 / (100-h)) * n;
            }
            cHints.push_back(c);
        }
    }
    else
    {
        for(size_t i = 0; i < hHints.size(); i++)
        {
            double h = hHints[i];
            double c;
            if(h == -1000) {
                c = 1000;
            }
            else if(h == -1000) {
                c = 0;
            }
            else {
                c = (100 / (100-h));
            }
            cHints.push_back(c);
        }
    }

    std::cout << "\e[0;32m" << "cHints: " << toString(cHints) << "\e[m" << std::endl;

    unsigned short int r = 0;
    short int maxr = -1;
    short int maxi = -1;
    for (unsigned short int i = 0; i < stateSpace->getLength(); i++)
    {
        if (stateSpace->get(i) == HexStateSpace::EMPTY)
        { // Empty Field
            if(maxr == -1) {
                maxr = r;
                maxi = i;
            }
            else if(cHints[maxr] < cHints[r])
            {
                maxr = r;
                maxi = i;
            }
            r++;
        }
    }
    std::cout << "\e[0;32m";
    std::cout << "resoultVect.size(): " << cHints.size() << std::endl;
    std::cout << "The Predicted field's array index is: " << maxi << std::endl;
    std::cout << "The Predicted field's matrix index is: [" <<
                 maxi/stateSpace->getSize()+1 << ". row, " << maxi%stateSpace->getSize()+1 << ". col]" << std::endl;
    std::cout << "The Predicted field's p = " << cHints[maxr] << std::endl;
    std::cout << "\e[m" << std::endl;
}

std::string HexCanvas::toString(std::vector<short int> &v) const
{
    std::stringstream ss;
    for(size_t i = 0; i < v.size(); i++) {
        ss << v[i] << " ";
    }
    return ss.str();
}

std::string HexCanvas::toString(std::vector<double> &v) const
{
    std::stringstream ss;
    for(size_t i = 0; i < v.size(); i++) {
        ss << v[i] << " ";
    }
    return ss.str();
}


/*double HexCanvas::getLimit(std::vector<double> v, int l) const
{

}*/

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

void HexCanvas::loadGame()
{
    QString qFileName = QFileDialog::getOpenFileName(this, tr("Open HEX file"), "../QtHEX/", tr("HEX files (*.txt *.hex)"));
    std::string fileName = qFileName.toStdString();
    std::cout << "The Game will be loaded:" << fileName << std::endl;
    std::fstream myfile(fileName, std::ios_base::in);
    int pointed;
    while (myfile >> pointed)
    {
        std::cout << ">>> Pointed hexagon is: " << pointed << "." << std::endl;
        if(stateSpace->isWinner()) return;
        stateSpace->set(pointed, player);
        getPlayerNextPlayer();
        if(stateSpace->isWinner())
            std::cout << "The " << (player != HexStateSpace::BLUE ? "\e[0;34mBlue" : "\e[0;31mRed") << "\e[m player won." << std::endl;
    }
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
