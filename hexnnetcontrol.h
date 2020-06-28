#ifndef HEXNNETCONTROL_H
#define HEXNNETCONTROL_H
#include <iostream>
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
    ~HexNnetControl(){};
    void Start();
public slots:
    void pyreadyReadStandardError();
    void pyreadyReadStandardOutput();
    void pystarted();
private:
    QString python;
    QString workingDir;
    QString pyFilePath;
};

#endif // HEXNNETCONTROL_H
