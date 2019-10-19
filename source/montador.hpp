#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <map>
#include <sstream>
#include <vector>

/* Erros a serem detectados
– declarações e rótulos ausentes;
– declarações e rótulos repetidos;
– pulo para rótulos inválidos;
– pulo para seção errada;
– diretivas inválidas;
– instruções inválidas;
– diretivas ou instruções na seção errada;
– divisão por zero (para constante);
– instruções com a quantidade de operando inválida;
– instruções com o tipo de operando inválido;
– tokens inválidos;
– dois rótulos na mesma linha;
– seção TEXT faltante;
– seção inválida;
– tipo de argumento inválido;
– modificação de um valor constante;
– acessar posição não reservada pelo SPACE (exemplo accesar SPACE+4,
sendo que somente foi reservada uma posição)
*/

class Montador
{
private:
    const std::string fileName;
    bool codeIsFineToGo = true;
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

