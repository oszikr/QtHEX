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

class HexNnetControl : public QProcess
{
    Q_OBJECT
public:
    HexNnetControl(QObject *parent = 0);
    ~HexNnetControl();
    void Start();
    std::string getState();
public slots:
    void pyreadyReadStandardError();
    void pyreadyReadStandardOutput();
    void pystarted();
    void input();
    void result(std::string result);
signals:
    void readyForInput();
    void resultReady(std::string result);
private:
    QString python;
    QString workingDir;
    QString pyFilePath;
    std::string state;
};

#endif // HEXNNETCONTROL_H
