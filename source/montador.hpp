#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <map>
#include <sstream>
#include <vector>

/* Erros a serem detectados
– declarações e rótulos ausentes; Ok
– declarações e rótulos repetidos; Ok
– pulo para rótulos inválidos; ~
– pulo para seção errada; Ok
– diretivas inválidas; ~
– instruções inválidas; ~
– diretivas ou instruções na seção errada;
– divisão por zero (para constante); Ok
– instruções com a quantidade de operando inválida; Ok
– instruções com o tipo de operando inválido; ~
– tokens inválidos;
– dois rótulos na mesma linha; OK
– seção TEXT faltante; OK
– seção inválida;
– tipo de argumento inválido; ~
– modificação de um valor constante; Ok
– acessar posição não reservada pelo SPACE (exemplo accesar SPACE+4,
sendo que somente foi reservada uma posição) Ok
*/

class Montador
{
private:
    const std::string fileName;
    bool codeIsFineToGo = true;
    bool isSecondArg = false; // Para uso esclusivo NAO modifique esse argumento
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
    std::map < std::string, int > labels;
    std::map < std::string, std::vector< std::uint16_t > > deps;
    std::vector < std::int16_t > endCode;
    std::map < std::uint16_t, std::string> instructionLines;
    std::map < std::string, std::vector< int > > useTable;
    std::map < std::string, std::vector< int > > defTable;
    std::vector < uint8_t > relocable;
    std::string programName;
    std::string mountedCode;

    void mountCode(const std::string &code);
    void mountData(const std::string &data);
    void dealInstruction (std::stringstream &instructionLine, std::string instruction, std::size_t &currentPosition);
    bool checkVar(std::string &var);
    bool checkInst( std::uint16_t i, std::vector<std::string> &instructions);
    std::size_t checkIfThereIsSum( std::string &variable, std::stringstream &instructionLine );
    void showInstructions( const std::vector<std::uint16_t> &pos );
    std::uint8_t countOcurrences ( std::string &s, const char &character );
public:
    Montador();
    ~Montador();

    std::string mount ( std::string fileName);
};

