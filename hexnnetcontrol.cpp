#include "hexnnetcontrol.h"

HexNnetControl::HexNnetControl(QObject *parent)
{
    python = "C:\\Users\\Oszi Krisztian\\AppData\\Local\\Programs\\Python\\Python36\\python.exe";
    workingDir = "C:/Users/Oszi Krisztian/Documents/MSCAPPS/hex13/";
    pyFilePath = workingDir + "service.py";
    state = "NONE";

    connect(this, SIGNAL(readyReadStandardError()), this, SLOT(readyReadStandardErrorSlot()));
    connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutputSlot()));
    connect(this, SIGNAL(started()), this, SLOT(startedSlot()));
    connect(this, SIGNAL(readyForInputSignal()), this, SLOT(inputSlot()));
    connect(this, SIGNAL(resultReadySignal(std::string)), this, SLOT(resultSlot(std::string)));
}

HexNnetControl::~HexNnetControl()
{
    close();
}

void HexNnetControl::Start() {
    std::cout << "Python starting..." << std::endl;

    QStringList arguments;
    arguments << pyFilePath;
    setWorkingDirectory(workingDir);
    start(python, arguments);

    std::cout << "Python started..." << std::endl;
}

std::string HexNnetControl::getState() {
    return state;
}

void HexNnetControl::readyReadStandardErrorSlot()
{
    state = "WORKING";
    QByteArray qba = readAllStandardError();
    std::string read = qba.toStdString();
    std::cout << "HexNnetControl::pyreadyReadStandardError()" << std::endl;
    std::cout << "PYTHON> " << read << std::endl;
}

void HexNnetControl::readyReadStandardOutputSlot()
{
    state = "WORKING";
    std::cout << "HexNnetControl::pyreadyReadStandardOutput()" << std::endl;
    QByteArray qba = readAllStandardOutput();
    std::string read = qba.toStdString();

    std::istringstream stream(read);
    std::string line;
    while(std::getline(stream, line)) {
        std::cout << "PYTHON> " << line << std::endl;
        if(line.compare("Enter input\r") == 0)
        {
            emit readyForInputSignal();
        }
        if(line.find("{\"Result\":") != std::string::npos)
        {
            emit resultReadySignal(line);
        }
    }
}

void HexNnetControl::startedSlot()
{
    state = "WORKING";
    std::cout << "HexNnetControl::pystarted()" << std::endl;
}

void HexNnetControl::inputSlot()
{
    state = "INPUT";
    std::cout << "HexNnetControl::input()" << std::endl;
}

void HexNnetControl::resultSlot(std::string result)
{
    state = "WORKING";
    std::cout << "HexNnetControl::resoult()" << std::endl;
    std::cout << result << std::endl;
}

