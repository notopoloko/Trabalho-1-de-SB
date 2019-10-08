#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <sstream>

class Montador
{
private:
    const std::string fileName;
    const std::map <std::string, uint8_t> codes = {
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
    std::map <std::string, int> dependencias;

    void mountCode(const std::string &code);
    void mountData(const std::string &data);
public:
    Montador();
    ~Montador();

    void mount (const std::string fileName);
};

