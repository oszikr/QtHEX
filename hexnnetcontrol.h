#ifndef HEXNNETCONTROL_H
#define HEXNNETCONTROL_H
#include <iostream>
#include <sstream>
#include <QCoreApplication>
#include <QProcessEnvironment>
#include <QWidget>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QDebug>
#include "hexstatespace.h"
#include "json.hpp"

class HexNnetControl : public QProcess
{
    Q_OBJECT
public:
    HexNnetControl(QObject *parent = 0);
    ~HexNnetControl();
    void Start(const HexStateSpace* hex);
    std::string getState();
    void hintTF(HexStateSpace* curStateSpace, HexStateSpace::color player);
public slots:
    void readyReadStandardErrorSlot();
    void readyReadStandardOutputSlot();
    void startedSlot();
    void inputSlot();
    void resultSlot(std::string result);
signals:
    void readyForInputSignal();
    void resultReadySignal(std::string result);
private:
    QString python;
    QString workingDir;
    QString pyFilePath;
    std::string state;
    const HexStateSpace* hex;
};

#endif // HEXNNETCONTROL_H
