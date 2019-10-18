#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <map>
#include <sstream>
#include <vector>

class Montador
{
private:
    const std::string fileName;
    std::map <std::string, std::uint16_t> codes = {
        {"ADD", 1},
        {"SUB", 2},
        {"MULT", 3},
        {"DIV", 4},
        {"JMP", 5},
        {"JMPN", 6},
        {"JMPP", 7},
        {"JMPZ", 8},
        {"COPY", 9},
        {"LOAD", 10},
        {"STORE", 11},
        {"INPUT", 12},
        {"OUTPUT", 13},
        {"STOP", 14}
    };
    std::map <std::string, int> labels;
    std::map < std::string, std::vector<std::uint16_t> > deps;
    std::vector<std::uint16_t> endCode;

    void mountCode(const std::string &code);
    void mountData(const std::string &data);
    void dealInstruction (std::stringstream &instructionLine, std::string instruction, std::size_t &currentPosition);
    bool checkVar(std::string &var);
    std::size_t checkIfThereIsSum( std::string &variable, std::stringstream &instructionLine );
public:
    Montador();
    ~Montador();

    std::string mount ( std::string fileName);
};

