#include "hexnnetcontrol.h"

HexNnetControl::HexNnetControl(QObject *parent)
{
    python = "C:\\Users\\Oszi Krisztian\\AppData\\Local\\Programs\\Python\\Python36\\python.exe";
    workingDir = "C:/Users/Oszi Krisztian/Documents/MSCAPPS/hex13/";
    pyFilePath = workingDir + "service.py";
    state = "NONE";

    connect(this, SIGNAL(readyReadStandardError()), this, SLOT(pyreadyReadStandardError()));
    connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(pyreadyReadStandardOutput()));
    connect(this, SIGNAL(started()), this, SLOT(pystarted()));
    connect(this, SIGNAL(readyForInput()), this, SLOT(input()));
    connect(this, SIGNAL(resultReady(std::string)), this, SLOT(result(std::string)));
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

void HexNnetControl::pyreadyReadStandardError()
{
    QByteArray qba = readAllStandardError();
    std::string read = qba.toStdString();
    std::cout << "HexNnetControl::pyreadyReadStandardError()" << std::endl;
    std::cout << "PYTHON> " << read << std::endl;
}

void HexNnetControl::pyreadyReadStandardOutput()
{
    std::cout << "HexNnetControl::pyreadyReadStandardOutput()" << std::endl;
    QByteArray qba = readAllStandardOutput();
    std::string read = qba.toStdString();

    std::istringstream stream(read);
    std::string line;
    while(std::getline(stream, line)) {
        std::cout << "PYTHON> " << line << std::endl;
        if(line.compare("Enter input\r") == 0)
        {
            state = "INPUT";
            emit readyForInput();
        }
        if(line.find("{Result:") != std::string::npos)
        {
            state = "RESULT";
            emit resultReady(line);
        }
    }
}

void HexNnetControl::pystarted()
{
    std::cout << "HexNnetControl::pystarted()" << std::endl;
}

void HexNnetControl::input()
{
    std::cout << "HexNnetControl::input()" << std::endl;
}

void HexNnetControl::result(std::string result)
{
    std::cout << "HexNnetControl::resoult()" << std::endl;
    std::cout << result << std::endl;
}

