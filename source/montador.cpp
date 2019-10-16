#include "montador.hpp"

Montador::Montador()
{
}

Montador::~Montador()
{
}

void Montador::mount ( std::string fileName) {
    std::ifstream arq;

    arq.open(fileName);
    if ( !arq.is_open() )
    {
        std::cerr << "Unable to open file to mount" << std::endl;
        exit(0);
    }

    // Le todo o arquivo assumindo que
    // estamos lidando com arquivos pequenos
    // e que nao possuem caracteres especiais: '\0'
    std::stringstream buffer;
    buffer << arq.rdbuf();
    arq.close();

    std::string content = buffer.str();
    // Procurar pela parte de codigo e de dados
    std::size_t textBegin = content.find("SECTION TEXT");
    std::size_t dataBegin = content.find("SECTION DATA");

    std::string code, data;

    // Tratamento da falta de parte de codigo aqui
    if (textBegin == std::string::npos) {
        std::cerr << "Sem parte de codigo" << std::endl;
    } else {
        // Tratamento da falta da parte de dados aqui
        if (dataBegin == std::string::npos) {
            std::cerr << "Sem parte de dados" << std::endl;
            exit(1);
        }
        // std::string("SECTION TEXT").size() == std::string("SECTION TEXT").size() == 13
        if (textBegin > dataBegin) { // Parte de codigo vem depois da parte de dados
            code = content.substr(textBegin + 13);
            data = content.substr(dataBegin + 13, textBegin - dataBegin - 13);
        } else { // Parte de codigo vem antes da parte de dados
            code = content.substr(textBegin + 13, dataBegin - textBegin - 13);
            data = content.substr(dataBegin + 13);
        }
    }

    Montador::mountCode(code);
    Montador::mountData(data);

    // Dump file
    fileName.replace( fileName.find_last_of('.'), std::string::npos, ".obj" );
    std::ofstream outputFile(fileName);
    if (outputFile.is_open()) {
        std::ostream_iterator<std::uint16_t> output_iterator(outputFile, " ");
        std::copy(endCode.begin(), endCode.end(), output_iterator);
    } else {
        std::cerr << "Unable to open file to dump" << std::endl;
    }
    
}

void Montador::mountCode (const std::string &code) {
    std::string token, line;
    std::stringstream code_(code);
    std::size_t currentPosition = 0;

    while (std::getline(code_, line)) {
        std::stringstream temp (line);
        temp >> token;
        // Token pode ser uma label, um opcode ou variavel
        // TODO: Arrumar caso em que haja L1:ABCD:
        bool ehOpCode = codes.find(token) != codes.end();
        if (ehOpCode) {
            endCode.push_back(codes[token]);
            currentPosition++;
            Montador::dealInstruction(temp, token, currentPosition);
        } else { // É label
            if (token.find(':') == std::string::npos ) { // não identificado
                std::cout << "Something is wrong: " << token << " is not recognized as label." << std::endl;
            } else { // Achou uma label
                token.pop_back(); // Remove ':'
                labels[token] = currentPosition;
                temp >> token;
                if ( codes.find(token) != codes.end() ) {
                    endCode.push_back(codes[token]);
                    currentPosition++;
                    Montador::dealInstruction(temp, token, currentPosition);
                } else {
                    std::cout << "Something is wrong: " << token << " is not recognized as instruction after a label." << std::endl;
                }
            }
        }
    }
}
// TODO: terminar de montar a parte de dados
void Montador::mountData (const std::string &data) {
    std::string token, token2, line;
    std::stringstream data_(data);
    std::size_t currentLine = endCode.size();

    while (std::getline(data_, line))
    {
        std::stringstream tempSS(line);
        // Por enquanto assumindo que nao ha erros e que sempre comeca com label
        tempSS >> token;
        token.erase( token.find_last_of(':') );

        tempSS >> token2;
        if ( !token2.compare("SPACE") ) { // Pode ser SPACE
            if ( tempSS >> token2 ) { // Deve haver o numero de SPACEs
                try {
                    std::size_t numOfSpaces = std::stoi(token2);
                    while (numOfSpaces) {
                        endCode.push_back(0);
                        numOfSpaces--;
                    }
                } catch(const std::exception& e) {
                    std::cerr << "Erro na conversao de SPACEs:\n" << e.what() << '\n';
                    continue;
                }

            } else { // Do contrario so ha um SPACE
                endCode.push_back(0);
            }
        } else if ( !token2.compare("CONST") ) { // Pode ser CONST
            if ( tempSS >> token2 ) { // Deve haver uma declaracao de constante
                try {
                    std::size_t val = std::stoi(token2);
                    endCode.push_back(val);
                } catch(const std::exception& e) {
                    std::cerr << "Erro na conversao para inteiro:\n" << e.what() << '\n';
                    continue;
                }
            } else {
                std::cerr << "CONST sem numero" << std::endl;
                continue;
            }
        } else {
            std::cout << "Objeto Voador Nao Identificado: " << token2 << std::endl;
            continue;
        }
        // Resolver lista de pendencias
        if (deps.find(token) != deps.end() ) {
            std::uint16_t currentPosition = deps[token];
            std::uint16_t temp;
            while ( currentPosition != 65535 ) {
                temp = endCode[currentPosition];
                endCode[currentPosition] = currentLine;
                currentPosition = temp;
            }
        }
        
        currentLine++;
    }
    // Resolver todas a labels
    for (const auto& kv: labels) {
        std::uint16_t currentPosition = deps[kv.first];
        std::uint16_t temp;
        while ( currentPosition != 65535 ) {
            temp = endCode[currentPosition];
            endCode[currentPosition] = kv.second;
            currentPosition = temp;
        }
    }
}

void Montador::dealInstruction ( std::stringstream &instructionLine, std::string instruction, std::size_t &currentPosition ) {
    if ( !instruction.compare("COPY") ) { // need to split and push two vars
        instructionLine >> instruction;
        std::string var1, var2;
        std::stringstream temp(instruction);
        std::getline(temp, var1, ',');
        std::getline(temp, var2);

        // Tratar o caso de soma: COPY N1+3,N2+4

        endCode.push_back( deps.find(var1) != deps.end() ? deps[var1] : -1 );
        deps[var1] = currentPosition;
        currentPosition++;

        endCode.push_back( deps.find(var2) != deps.end() ? deps[var2] : -1 );
        deps[var2] = currentPosition;
        currentPosition++;
    // Instrução STOP não precisa de argumento. As outras precisam
    // Need to push one var
    } else if ( instruction.compare("STOP") ) {
        // Tratar o caso de soma: OPCODE N1+3
        instructionLine >> instruction;
        if (instruction.find('+') != std::string::npos) { // Possui uma soma
            /* code */
        } 
        endCode.push_back( deps.find(instruction) != deps.end() ? deps[instruction] : -1 );
        deps[instruction] = currentPosition;
        currentPosition++;
    }
}