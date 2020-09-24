#include "hexnnetcontrol.h"

HexNnetControl::HexNnetControl(QObject *parent) : QProcess(parent)
{
    #ifdef __linux__
        python = "/usr/bin/python3";
        workingDir = "/home/oszikr/PycharmProjects/hex13/";
    #elif _WIN32
        python = "C:\\Users\\oszikr\\AppData\\Local\\Programs\\Python\\Python36\\python.exe";
        workingDir = "C:/Users/oszikr/Documents/MSCAPPS/hex13/";
    #else
        # not supported
    #endif

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

void HexNnetControl::Start(const HexStateSpace* hex)
{
    if(!QUICKSTARTPYTHON) {
        return;
    }
    this->hex = hex;
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
    //state = "WORKING";
    QByteArray qba = readAllStandardError();
    std::string read = qba.toStdString();
    std::cout << "HexNnetControl::pyreadyReadStandardError()" << std::endl;
    std::cout << "PYTHON>ERR> " << read << std::endl;
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
        std::cout << "PYTHON>OUT> " << line << std::endl;
        if(line.compare("Enter input\r") == 0 || line.compare("Enter input") == 0) // \r for windows
        {
            std::cout << "emit readyForInputSignal()" << std::endl;
            emit readyForInputSignal();
        }
        if(line.find("Result") != std::string::npos)
        {
            std::cout << "emit resultReadySignal()" << std::endl;
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
    std::cout << "HexNnetControl::result()" << std::endl;
    std::cout << result << std::endl;

    std::vector<double> resoultVect = from_JSON(result);
    unsigned short int r = 0;
    short int maxr = -1;
    short int maxi = -1;
    for (unsigned short int i = 0; i < hex->getLength(); i++)
    {
        if (hex->get(i) == HexStateSpace::EMPTY)
        { // Empty Field
            if(maxr == -1) {
                maxr = r;
                maxi = i;
            }
            else if(resoultVect[maxr] < resoultVect[r])
            {
                maxr = r;
                maxi = i;
            }
            r++;
        }
    }
    std::cout << "The Predicted field's array index is: " << maxi << std::endl;
    std::cout << "The Predicted field's matrix index is: [" <<
                 maxi/hex->getSize()+1 << ". row, " << maxi%hex->getSize()+1 << ". col]" << std::endl;
    std::cout << "The Predicted field's p = " << resoultVect[maxr] << std::endl;
}

void HexNnetControl::hintTF(HexStateSpace* curStateSpace, HexStateSpace::color player) {
    HexStateSpace* hex = curStateSpace;
    std::vector<std::vector<HexStateSpace::color>> statespaces;
    for (unsigned short int i = 0; i < hex->getLength(); i++)
    {
        if (hex->get(i) == HexStateSpace::EMPTY) { // Empty Field
            HexStateSpace nextHex(*hex); // Copy the state of the table
            nextHex.set(i, player); // Mark the field for the current player

            HexStateSpace::color* sp = nextHex.getSpace();
            HexStateSpace::color count =
                    (nextHex.getCount() < 16 ? HexStateSpace::EMPTY : (nextHex.getCount() < 32 ? HexStateSpace::BLUE : HexStateSpace::RED));
            unsigned int n = nextHex.getLength();
            std::vector<HexStateSpace::color> stateSpaceVector(sp, sp+n); // copy ctor for vector.
            stateSpaceVector.push_back(count);
            statespaces.push_back(stateSpaceVector);
        }
    }

    std::string statespaces_json = to_JSON(statespaces);
    std::cout << "HexNnetControl::hintTF>statespaces_json: " << statespaces_json << std::endl;
    write(statespaces_json.c_str());
    write("\n");
}

std::string HexNnetControl::to_JSON(std::vector<std::vector<HexStateSpace::color>> &statespaces) const
{
    // sample statespaces:
    // [[0,0,0,0,0,0,0,1,1,0,1,2,0, ... ,0,0,0,0,0,0,0], ... , [0,0,0,0,0,0,0,0,0,1,2,0, ... ,0,0,0,0,0,0,1,2,0,1,0,0,0,0,0,0,0,0,0]]
    std::stringstream ss;
    ss << "[";
    for (std::vector<std::vector<HexStateSpace::color>>::iterator it = statespaces.begin() ; it != statespaces.end(); ++it)
    {
        if(it != statespaces.begin()) ss << ", ";
        ss << "[";
        std::vector<HexStateSpace::color> state = *it;
        for (std::vector<HexStateSpace::color>::iterator it2 = state.begin() ; it2 != state.end(); ++it2)
        {
            if(it2 != state.begin()) ss << ",";
            HexStateSpace::color color = *it2;
            ss << color;
        }
        ss << "]";
    }
    ss << "]";
    return ss.str();
}

std::vector<double> HexNnetControl::from_JSON(std::string result) const
{
    // sample result:
    // {"Result" : [0.0,0.0]}
    std::size_t pos1 = result.find("[") + 1;
    std::size_t pos2 = result.find("]");
    result = result.substr(pos1, pos2 - pos1);
    std::replace( result.begin(), result.end(), ',', ' ');

    std::vector<double> v;
    std::string s = result;
    std::istringstream is( s );
    double d;
    while( is >> d )
    {
        v.push_back(d);
    }
    return v;
}
