#include "montador.hpp"

Montador::Montador()
{
}

Montador::~Montador()
{
}

void Montador::mount (const std::string fileName) {
    std::ifstream arq;

    arq.open(fileName);
    if ( !arq.is_open() )
    {
        std::cerr << "Unable to open file to mount" << std::endl;
    }

    // Le todo o arquivo assumindo que
    // estamos lidando com arquivos pequenos
    // e que nao possuem caracteres especiais: '\0'
    std::stringstream buffer;
    buffer << arq.rdbuf();

    std::string content = buffer.str();
    std::size_t textBegin = content.find("SECTION TEXT");
    if (textBegin == std::string::npos)
    {
        std::cerr << "Sem parte de codigo" << std::endl;
        exit(0);
    }
    textBegin += 13;
    std::size_t dataBegin = content.find("SECTION DATA");

    if ( dataBegin != std::string::npos) {
        dataBegin -= textBegin;
    }
     
    std::string code = content.substr(textBegin, dataBegin);

    std::cout << code << std::endl;

    // std::string linha;

    // while ( !arq.eof() ) {
    //     std::getline(arq, linha);
    //     // Trata parte de codigo
    //     if (linha.find("SECTION TEXT") != std::string::npos)
    //     {
    //         arq.read()
    //     }

    // }
    arq.close();
}

// TODO: terminar de montar a parte de codigo
void Montador::mountCode (const std::string &code) {

}
// TODO: terminar de montar a parte de dados
void Montador::mountData (const std::string &data) {

}