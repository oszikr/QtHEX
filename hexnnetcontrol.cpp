#include "hexnnetcontrol.h"

HexNnetControl::HexNnetControl(QObject *parent)
{
    python = "C:\\Users\\Oszi Krisztian\\AppData\\Local\\Programs\\Python\\Python36\\python.exe";
    workingDir = "C:/Users/Oszi Krisztian/Documents/MSCAPPS/hex13/";
    pyFilePath = workingDir + "predict.py";

    connect(this, SIGNAL(readyReadStandardError()), this, SLOT(pyreadyReadStandardError()));
    connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(pyreadyReadStandardOutput()));
    connect(this, SIGNAL(started()), this, SLOT(pystarted()));
}

void HexNnetControl::Start() {
    std::cout << "Python starting..." << std::endl;

    QStringList arguments;
    arguments << pyFilePath;
    setWorkingDirectory(workingDir);
    start(python, arguments);

    waitForFinished(-1);
    std::cout << "Python ended..." << std::endl;
}

void HexNnetControl::pyreadyReadStandardError()
{
    qDebug("HexNnetControl::pyreadyReadStandardError()");
    qDebug(readAllStandardError());
}

void HexNnetControl::pyreadyReadStandardOutput()
{
    qDebug("HexNnetControl::pyreadyReadStandardOutput()");
    qDebug(readAllStandardOutput());
}

void HexNnetControl::pystarted()
{
    qDebug("HexNnetControl::pystarted()");
}

/*void HexNnetControl::pyreadyRead() {
    std::cout << "SLOT myReadyRead" << std::endl;
    //qDebug() << Q_FUNC_INFO;
}

void HexNnetControl::pyreadyReadStandardOutput() {
    std::cout << "SLOT myReadyReadStandardOutput" << std::endl;

    qDebug() << Q_FUNC_INFO;
    // Note we need to add \n (it's like pressing enter key)
    this->write(QString("myname" + QString("\n")).toLatin1());
    // Next line no required
    // qDebug() << this->readAll();
    qDebug() << this->readAllStandardOutput();

}*/

/*HexNnetControl::~HexNnetControl()
{
    pyProcess->close();
}


void HexNnetControl::Exec()
{
    std::cout << "Python starting" << std::endl;

    QStringList arguments;
    arguments << pyFile;
    pyProcess = new QProcess(this);
    pyProcess->setWorkingDirectory(programDir);
    pyProcess->start(program, arguments);

    if (!pyProcess->waitForStarted())
    {
        std::cout << "A problem occurs" << std::endl;
        return;
    }
    std::cout << "Python opened" << std::endl;
}*/

