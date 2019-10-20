#include "montador.hpp"

Montador::Montador()
{
}

Montador::~Montador()
{
}

std::string Montador::mount ( std::string fileName) {
    std::ifstream arq;

    arq.open(fileName);
    if ( !arq.is_open() ) {
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
        exit(1);
    } else {
        // Tratamento da falta da parte de dados aqui
        if (dataBegin == std::string::npos) {
            std::cerr << "Sem parte de dados" << std::endl;
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
    if (codeIsFineToGo) {
        fileName.replace( fileName.find_last_of('.'), std::string::npos, ".obj" );
        std::ofstream outputFile(fileName);
        if (outputFile.is_open()) {
            std::ostream_iterator<std::uint16_t> output_iterator(outputFile, " ");
            std::copy(endCode.begin(), endCode.end(), output_iterator);
        } else {
            std::cerr << "Unable to open file to dump" << std::endl;
            return "";
        }
        return fileName;
    }
    return "";
}

void Montador::mountCode (const std::string &code) {
    std::string token, line;
    std::stringstream code_(code);
    std::size_t currentPosition = 0;

    while (std::getline(code_, line)) {
        // Pular linhas vazias
        if (line.size() == 0) {
            continue;
        }
        // Guardar instrução
        instructionLines[ currentPosition ] = line;
        std::stringstream temp (line);
        temp >> token;
        // Token pode ser uma label, um opcode ou variavel
        // TODO: Arrumar caso em que haja L1:ABCD:
        bool ehOpCode = codes.find(token) != codes.end();
        if (ehOpCode) {
            endCode.push_back(codes[token]);
            currentPosition++;
            Montador::dealInstruction(temp, token, currentPosition);
        } else { // Pode ser uma label
            if (token.find(':') == std::string::npos ) { // não identificado
                std::cout << token << " in line " << currentPosition << " " + line << " is not a valid instruction."<< std::endl;
                currentPosition++;
            } else { // Achou uma label
                token.pop_back(); // Remove ':'
                // Checar se o rótulo está escrito corretamente
                if ( !checkVar( token ) ) {
                    std::cout << "Lexicon error: label is not spelled correctly in line: " + line << std::endl;
                    codeIsFineToGo = false;
                } else if ( labels.find(token) != labels.end() ) { // Checar se uma label já foi declarada
                    std::cout << "Semantic error: redefinition of label " + token + " in " + line << std::endl;
                    codeIsFineToGo = false;
                }
                
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
    std::size_t holdNumOfSpaces = 0;
    std::size_t currentTokenLine = currentLine;
    bool ehConst = false;
    std::map< std::string, std::vector<std::uint16_t> >::iterator itForDeps;

    while (std::getline(data_, line)) {
        if (line.size() == 0) { // Nada para fazer, eh so um \n
            continue;
        }
        
        std::stringstream tempSS(line);
        // Por enquanto assumindo que nao ha erros e que sempre comeca com label
        tempSS >> token;
        token.erase( token.find_last_of(':') );

        tempSS >> token2;
        if ( !token2.compare("SPACE") ) { // Pode ser SPACE
            if ( tempSS >> token2 ) { // Deve haver o numero de SPACEs
                try {
                    std::size_t numOfSpaces = std::stoi(token2);
                    holdNumOfSpaces = numOfSpaces;
                    while (numOfSpaces) {
                        endCode.push_back(0);
                        currentLine++;
                        numOfSpaces--;
                    }
                    ehConst = false;
                } catch(const std::exception& e) {
                    std::cerr << "Erro na conversao de SPACEs:\n" << e.what() << '\n';
                    continue;
                }

            } else { // Do contrario so ha um SPACE
                endCode.push_back(0);
                holdNumOfSpaces = 1;
                currentLine++;
                ehConst = false;
            }
        } else if ( !token2.compare( "CONST" ) ) { // Pode ser CONST
            if ( tempSS >> token2 ) { // Deve haver uma declaracao de constante
                try {
                    std::size_t val = std::stoi(token2);
                    endCode.push_back(val);
                    holdNumOfSpaces = 1;
                    ehConst = true;
                    currentLine++;
                } catch (const std::exception& e) {
                    std::cerr << "Erro na conversao de " << token2 << " para um valor de constante:\n" << e.what() << '\n';
                    codeIsFineToGo = false;
                    continue;
                }
            } else {
                std::cout << "Sintax erros: CONST sem numero" << std::endl;
                codeIsFineToGo = false;
                continue;
            }
        } else {
            std::cout << "Objeto Voador Nao Identificado: " << token2 << std::endl;
            continue;
        }
        // Resolver lista de pendencias com variaveis com ou sem soma
        itForDeps = deps.find(token);
        if ( itForDeps != deps.end() ) {
            // Checar se ja nao foi declarado como variavel: vai estar marcada como resolvida
            if ( itForDeps->second[0] == 65535 ) {
                std::cout << "Semantic error: redefinition of variable " + itForDeps->first + " in line " + line << std::endl;
                codeIsFineToGo = false;
                continue;
            }
            // Checar se já não foi declarado como label
            const std::map<std::string, int>::iterator it = labels.find(token);
            if ( it == labels.end() ) {
                for (auto &i : deps[token]) {
                    // Checar se nao esta saltando para seçao de dados
                    std::vector<std::string> insts = { "JMP", "JMPZ", "JMPP", "JMPN" };
                    if ( checkInst( i, insts ) ) {
                        std::vector < std::uint16_t > temp = { i };
                        std::cout << "jump to data section in line:\n " << std::endl;
                        showInstructions(temp);
                        codeIsFineToGo = false;
                    } else if (ehConst) { // Garantir que nao ha alteracao de constante
                        insts = {"INPUT", "STORE", "COPY"};
                        if ( checkInst( i, insts ) ) {
                            std::cout << "Semantic error: const value cannot be changed in line:" << std::endl;
                            std::vector < std::uint16_t > temp = { i };
                            showInstructions(temp);
                            codeIsFineToGo = false;
                        }
                    }
                    // Checar se está fazendo acesso correto
                    if ( holdNumOfSpaces > endCode[i]) {
                        endCode[i] = currentTokenLine + endCode[i];
                    } else {
                        std::cout << "Semantic error: access to address " + token + " + " << endCode[i] << " is illegal in line: " << std::endl;
                        codeIsFineToGo = false;
                        std::vector < std::uint16_t > temp = { i };
                        showInstructions(temp);
                    }
                }
                // Dependencias resolvidas
                deps[token][0] = 65535;
            } else {
                std::cout << it->first << " declared as a variable but it was already declared as label in:\n" << std::endl;
                codeIsFineToGo = false;
                std::vector<std::uint16_t> vec = { static_cast<std::uint16_t> (it->second) };
                showInstructions( vec );
            }
            
        } else {
            std::cout << token << " declared but not used in line " << currentLine << " in " + line << std::endl;
        }
        currentTokenLine = currentLine;
    }
    // Resolver todas a labels
    for (const auto& kv: labels) {
        // Label foi usada em algum lugar e precisa ser resolvida
        if ( deps.find(kv.first) != deps.end() ) {
            for (auto &i : deps[kv.first]) {
                endCode[i] = kv.second;
            }
            // Marca que a pendencia foi resolvida
            deps[kv.first][0] = 65535;
        } else {
            std::cout << "Label " + kv.first << " declared but not used." << std::endl;
        }
    }
    // Checar se todas as dependencias foram resolvidas
    for (const auto &i : deps) {
        if (i.second[0] != 65535) {
            std::cout << "Semantic error: " << i.first << " used but not declared in lines:\n" << std::endl;
            // Passar endereços para buscar instrução
            showInstructions(i.second);
            codeIsFineToGo = false;
        }
    }
}

std::size_t Montador::checkIfThereIsSum( std::string &variable, std::stringstream &instructionLine ) {
    std::size_t val = 0;
    std::string var1, varTemp1;

    if (variable.find('+') != std::string::npos ) {
        std::stringstream temp(variable);
        std::getline(temp, variable, '+');
        std::getline(temp, varTemp1);
        for (auto &i : varTemp1) {
            if (!std::isdigit(i)) {
                std::cout << varTemp1 << " cannot be converted to a valid index in " << instructionLine.str() << std::endl;
                codeIsFineToGo = false;
                return 0;
            }
        }
        
        try { // Tentar tranformar para inteiro
            val = std::stoi(varTemp1, nullptr, 10);
        } catch(const std::exception& e) {
            std::cerr << "Sintatic error in transform "<< instructionLine.str() << " to a valid instruction. Index " << varTemp1 << 
            "could not be converted to integer\n" << e.what() << '\n';
            codeIsFineToGo = false;
            return 0;
        }
    }
    return val;
}

void Montador::dealInstruction ( std::stringstream &instructionLine, std::string instruction, std::size_t &currentPosition ) {
    std::size_t val = 0, val1 = 0;
    std::uint8_t count = 0;

    if ( !instruction.compare("COPY") ) { // need to split and push two vars
        // Garante que há pelo menos um argumento
        if ( !(instructionLine >> instruction) ) {
            std::cout << "COPY is expecting at least two arguments, but none is provided in line " + instructionLine.str() << std::endl;
            codeIsFineToGo = false;
            return;
        }
        count = countOcurrences( instruction, ',');
        if ( count != 1 ) {
            std::cout << "Semantic error: COPY instruction is expecting 2 args in line " + instructionLine.str() << std::endl;
            codeIsFineToGo = false;
            return;
        }
        
        std::string var1, var2;
        std::stringstream temp(instruction);
        std::getline(temp, var1, ',');
        std::getline(temp, var2);
        // Tratar o caso de soma: COPY N1+3,N2+4
        val = Montador::checkIfThereIsSum(var1, instructionLine);
        val1 = Montador::checkIfThereIsSum(var2, instructionLine);
        if ( !Montador::checkVar(var1) ) { // Variável com síbolo errado. Apaga instrução e retorna
            std::cout << "Lexicon error: " << var1 << " is not spelled correctly in " << instructionLine.str() << ". Fix this." << std::endl;
            codeIsFineToGo = false;
            return;
        }
        if ( !Montador::checkVar(var2)) {
            std::cout << "Lexicon error: " << var2 << " is not spelled correctly in " << instructionLine.str() << ". Fix this." << std::endl;
            codeIsFineToGo = false;
            return;
        }

        endCode.push_back( val );
        deps[var1].push_back(currentPosition);
        currentPosition++;

        endCode.push_back( val1 );
        deps[var2].push_back(currentPosition);
        currentPosition++;
    // Instrução STOP não precisa de argumento. As outras precisam
    // Need to push one var
    } else if ( instruction.compare("STOP") ) {
        // Tratar o caso de soma: OPCODE N1+3
        std::string holdInstruction = instruction;
        // Checa se há argumentos
        if ( !(instructionLine >> instruction) ) {
            std::cout << holdInstruction + " is expecting at least one argument, but none is provided in line " + instructionLine.str() << std::endl;
            codeIsFineToGo = false;
            return;
        }
        
        // Make sure that only one argument is passed to check
        count = countOcurrences( instruction, ',');
        if ( count != 0 ) {
            std::cout << "Semantic error: " + holdInstruction + " instruction is expecting 1 arg in line " + instructionLine.str() << std::endl;
            codeIsFineToGo = false;
            return;
        }
        // Checar se se está referenciando uma posição de um vetor;
        val = Montador::checkIfThereIsSum(instruction, instructionLine);
        if ( !Montador::checkVar( instruction ) ) {
            std::cout << "Lexicon error: " + instruction + " is not spelled correctly in " + instructionLine.str() << std::endl;
            codeIsFineToGo = false;
            return;
        }
        endCode.push_back( val );
        deps[instruction].push_back(currentPosition);
        currentPosition++;
    } else if ( instructionLine >> instruction ) {
        std::cout << "Semantic error: STOP instruction is not expecting arguments in line " << instructionLine.str() << std::endl;
        codeIsFineToGo = false;
    }
}

/*
Retorna verdadeiro se a variável estiver correta: com caracteres alfanuméricos ou com underscore
Falso se começar com número ou tiver caracter especial como &, :, '
*/
bool Montador::checkVar(std::string &var) {
    for (const auto &i : var) {
        if ( !std::isalnum( i ) && '_' != i ) {
            return false;
        }
    }
    if (var.size() > 0 && std::isdigit(var[0])) {
        return false;
    }
    
    return true;
}

/*
Procura uma determinada instrução baseada na posição de um operando
*/
void Montador::showInstructions( const std::vector<std::uint16_t> &pos ) {
    for (auto &i : pos) {
        // Achar linha provavel. Forma muito tosca de procurar uma instrucao :(
        if (instructionLines.find(i) != instructionLines.end() ) {
            std::cout << "\t" << i << " " + instructionLines[i] << std::endl;
        } else if ( instructionLines.find(i-1) != instructionLines.end() ) {
            std::cout << "\t" << i-1 << " " + instructionLines[i-1] << std::endl;
        } else if ( instructionLines.find(i-2) != instructionLines.end() ) {
            std::cout << "\t" << i-2 << " " + instructionLines[i-2] << std::endl;
        } else if ( instructionLines.find(i-3) != instructionLines.end() ) {
            std::cout << "\t" << i-3 << " " + instructionLines[i-3] << std::endl;
        } else {
            std::cout << "Unable to find instruction." << std::endl;
        }
    }
}

std::uint8_t Montador::countOcurrences ( std::string &s, const char &character ){
    std::uint8_t count = 0;
    for (auto &i : s) {
        if ( i == character ) {
            count++;
        }
    }
    return count;
}

/*
Retorna verdadeiro se achar uma instrução do tipo jump, falso caso contrário
*/
bool Montador::checkInst( std::uint16_t i, std::vector<std::string> &instructions) {
    std::map <std::uint16_t, std::string>::iterator it;
    std::string temp;
    // Achar linha provavel. Forma muito tosca de procurar uma instrucao :(
    for (std::uint16_t j = i; j > i - 4; j--) {
        if ( (it = instructionLines.find(j)) != instructionLines.end()) {
            break;
        }
    }
    if ( it != instructionLines.end() ) { // Procura na linha pela instrução de salto
        std::stringstream tempSS (it->second);
        tempSS >> temp;
        if ( temp.size() > 0 && temp[temp.size() - 1] == ':' ) { // Pode ser uma label. Pegar próximo token
            tempSS >> temp;
        }
        for (auto &i : instructions) {
            if ( !temp.compare(i) ) {
                return true;
            }
        }
    } else {
        return false;
    }
    return false;
}