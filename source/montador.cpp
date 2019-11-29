#include "montador.hpp"

Montador::Montador()
{
}

Montador::~Montador()
{
}

std::string Montador::mount ( std::string fileName) {
    std::ifstream arq;

    std::cout << "\tFase de montagem:\n" << std::endl;

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

    std::string content, token, token2;
    // Procura pela parte de útil para montagem
    // MODULE_NAME: BEGIN | MODULE_NAME:BEGIN
    // ...
    // END
    std::size_t jmpChars = 0;
    while ( std::getline( buffer, content ) ) {
        // Verifica se há dois pontos definindo o modulo
        std::size_t doublePoints = content.find(':');
        jmpChars += content.size();
        if ( doublePoints != std::string::npos ) {
            token = content.substr( 0, doublePoints );
            token2 = content.substr( doublePoints + 1 ); // Pula o ':'
            // Arrumar outros casos
            if ( token2.find("BEGIN") != std::string::npos ) {
                buffer.seekg(0);
                content = buffer.str();
                // Sem delimitação final de módulo
                std::size_t endModule = content.find("END");
                if ( endModule == std::string::npos ) {
                    std::cout << "There's no END delimiter of module." << std::endl;
                    exit( 0 );
                }
                content = content.substr(jmpChars, endModule - jmpChars);
                this->programName = token;
                // std::cout << "programName: " + token + "\nprogramContent: " + content << std::endl;
            }
            break;
        }
    }

    // Procurar pela parte de codigo e de dados
    std::size_t textBegin = content.find("SECTION TEXT");
    std::size_t dataBegin = content.find("SECTION DATA");

    std::string code, data;

    // Tratamento da falta de parte de codigo aqui
    if (textBegin == std::string::npos) {
        std::cerr << "Sem parte de codigo." << std::endl;
        exit(1);
    } else {
        // Tratamento da falta da parte de dados aqui
        if (dataBegin == std::string::npos) {
            std::cerr << "Sem parte de dados." << std::endl;
            code = content.substr(textBegin + 13);
        } else if (textBegin > dataBegin) { // Parte de codigo vem depois da parte de dados
            // std::string("SECTION TEXT").size() == std::string("SECTION TEXT").size() == 13
            code = content.substr(textBegin + 13);
            data = content.substr(dataBegin + 13, textBegin - dataBegin - 13);
        } else { // Parte de codigo vem antes da parte de dados
            code = content.substr(textBegin + 13, dataBegin - textBegin - 13);
            data = content.substr(dataBegin + 13);
        }
    }

    Montador::mountCode(code);
    Montador::mountData(data);

    std::ostringstream oss, ossCode;

    // Dump file
    if (codeIsFineToGo) {
        // Pre aloca uma quantidade razoavel para evitar operação de alocação
        mountedCode.reserve( 512 );
        fileName.replace( fileName.find_last_of('.'), std::string::npos, ".obj" );
        std::ofstream outputFile(fileName);
        if (outputFile.is_open()) {
            // Coloca o nome do programa
            mountedCode = "H: " + programName + "\nH: ";
            // Coloca o tamanho do programa
            mountedCode += std::to_string( endCode.size() ) + "\nH: ";
            // Coloca informação de relocação
            // Transforma vetor de int para string
            std::copy(relocable.begin(), relocable.end(), std::ostream_iterator<int>(oss, ""));
            mountedCode += oss.str() + "\nH: ";
            // Insere tabela de uso
            for ( auto &i : useTable ) {
                for ( auto &j : i.second ) {
                    mountedCode += i.first + ' ' + std::to_string( j ) + ' ';
                }
            }
            mountedCode += "\nH: ";
            // Insere tabela de definições
            for ( auto &i : defTable ) {
                for ( auto &j : i.second ) {
                    mountedCode += i.first + ' ' + std::to_string( j ) + ' ';
                }
            }
            mountedCode += "\nT: ";
            // Escreve o codigo
            std::copy(endCode.begin(), endCode.end(), std::ostream_iterator<std::int16_t>( ossCode, " "));
            // Joga no arquivo
            mountedCode += ossCode.str();
            outputFile << mountedCode;
            outputFile.close();
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
    std::size_t currentPosition = 0, doublePoints = 0;
    std::map < std::string, std::vector< int > >::iterator it;

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
            relocable.push_back( 0 );
            Montador::dealInstruction(temp, token, currentPosition);
        } else if ( !token.compare ("PUBLIC") ) {
            // Pode ser um símbolo público
            temp >> token;
            this->defTable[token].push_back( 0 );
        } else { // Pode ser uma label
            doublePoints = token.find(':');
            if ( doublePoints == std::string::npos ) { // não identificado
                if ( !checkVar(token) ) {
                    std::cout << "Lexicon error: " + token + " is not spelled correctly in line " + line << std::endl;
                } else {
                    std::cout << "Sintax error:" + token << " in line " << currentPosition << " " + line << " is not a valid instruction." << std::endl;
                    codeIsFineToGo = false;
                }
            } else if ( doublePoints == token.size() - 1 ) { // Achou uma label
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

                // Por enquanto resolvendo labels publicas aqui
                if ( (it = defTable.find(token)) != defTable.end() ) {
                    it->second[0] = currentPosition;
                    // std::cout << "Simbolo publico resolvido: " + token << std::endl;
                }
                

                std::string possibleExtern = token;
                temp >> token;
                if ( token.size() > 0 && token[ token.size()-1 ] == ':' ) { // Achou outra label
                    std::cout << "Sintax error: labels with same address in line: " + line << std::endl;
                    codeIsFineToGo = false;
                    continue;
                }
                if ( codes.find(token) != codes.end() ) {
                    // É um mnemonico. Não eh relocavel
                    endCode.push_back(codes[token]);
                    relocable.push_back(0);
                    currentPosition++;
                    Montador::dealInstruction(temp, token, currentPosition);
                } else if ( !token.compare("EXTERN") ) {
                    // É uma label definida em outro modulo
                    this->useTable[ possibleExtern ];
                    continue;
                } else {
                    if ( !checkVar(token) ) {
                        std::cout << "Lexicon error: " + token + " is not spelled correctly in line " + line << std::endl;
                    } else {
                        std::cout << "Sintax error: " << token << " is not recognized as instruction after a label." << std::endl;
                    }
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
    int val = 0;
    std::size_t currentTokenLine = currentLine;
    bool ehConst = false;
    std::map< std::string, std::vector<std::uint16_t> >::iterator itForDeps;
    std::map< std::string, std::vector<int> >::iterator itForDef;
    while (std::getline(data_, line)) {
        if (line.size() == 0) { // Nada para fazer, eh so um \n
            continue;
        }
        
        std::stringstream tempSS(line);
        // Checar se começa com label
        if ( tempSS >> token ) {
            if (token.size() > 0 && token[token.size() - 1] == ':') {
                token.erase( token.find_last_of(':') );
            } else { // Não começa com declaração de variável
                std::cout << "Sintax error: "+ line + " is not recognized as a valid variable declaration." << std::endl;
                codeIsFineToGo = false;
                continue;
            }
            if ( !checkVar( token ) ) {
                std::cout << "Lexic error: " + token + " is not spelled correctly in line: " + line << std::endl;
                codeIsFineToGo = false;
                continue;
            }
            
        }
        tempSS >> token2;
        if ( !token2.compare("SPACE") ) { // Pode ser SPACE
            // Procurar por símbolos publicos
            if ( (itForDef = defTable.find( token ) ) != defTable.end() ) {
                // Deve definir um símbolo na tabela de definições
                itForDef->second[0] = endCode.size();
                // std::cout << "Simbolo publico achado: " + token << std::endl;
            }
            if ( tempSS >> token2 ) { // Deve haver o numero de SPACEs
                for (auto &i : token2) { // Verificar se pode haver conversao errada
                    if ( !std::isdigit( i ) ) {
                        std::cout << "Sintax error: " + token2 + " is expected to be a number in line: " + line << std::endl;
                        codeIsFineToGo = false;
                        break;
                    }
                }
                try {
                    std::size_t numOfSpaces = std::stoi(token2);
                    holdNumOfSpaces = numOfSpaces;
                    while (numOfSpaces) {
                        endCode.push_back(0);
                        relocable.push_back( 0 );
                        currentLine++;
                        numOfSpaces--;
                    }
                    ehConst = false;
                } catch(const std::exception& e) {
                    // std::cerr << "Erro na conversao de SPACEs in line:" + line + "\n" << e.what() << '\n';
                    continue;
                }

            } else { // Do contrario so ha um SPACE
                endCode.push_back(0);
                holdNumOfSpaces = 1;
                relocable.push_back( 0 );
                currentLine++;
                ehConst = false;
            }
        } else if ( !token2.compare( "CONST" ) ) { // Pode ser CONST
            std::string minusSign;
            if ( tempSS >> token2 ) { // Deve haver uma declaracao de constante
                if ( token2.size() > 0 && token2[0] == '-' ) { // eh um numero negativo
                    minusSign = token2[0];
                    token2 = token2.substr(1); // retira o sinal negativo
                }
                for (auto &i : token2) { // Verificar se pode haver conversao errada
                    if ( !std::isdigit( i ) ) {
                        std::cout << "Sintax error: " + token2 + " is expected to be a number in line: " + line << std::endl;
                        codeIsFineToGo = false;
                        break;
                    }
                }
                token2 = minusSign + token2; // recebe o sinal novamente
                try {
                    val = std::stoi(token2);
                    endCode.push_back(val);
                    holdNumOfSpaces = 1;
                    ehConst = true;
                    relocable.push_back( 0 );
                    currentLine++;
                    if ( (itForDef = this->defTable.find( token ) ) != defTable.end() ) {
                        // Deve definir um símbolo na tabela de definições
                        itForDef->second[0] = endCode.size() - 1;
                        std::cout << "Simbolo publico achado: " + token << std::endl;
                    }
                    
                } catch (const std::exception& e) {
                    // std::cerr << "Lexicon error: erro na conversao de " << token2 << " para um valor de constante:\n" << e.what() << '\n';
                    codeIsFineToGo = false;
                    continue;
                }
            } else {
                std::cout << "Sintax error: CONST not initialized in line " << currentTokenLine << " " << line << std::endl;
                codeIsFineToGo = false;
                continue;
            }
        } else {
            std::cout << "Sintax error: " + token2 + " is not recognized as valid token in line: " + line << std::endl;
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
                        std::cout << "Jump to data section in line:\n " << std::endl;
                        showInstructions(temp);
                        codeIsFineToGo = false;
                    } else if (ehConst) { // Garantir que nao ha alteracao de constante
                        insts = {"COPY"};
                        if ( checkInst( i, insts ) && isSecondArg) { // Não pode alterar apenas o segundo argumento de Copy
                            std::cout << "Semantic error: const value cannot be changed in line:" << std::endl;
                            std::vector < std::uint16_t > temp = { i };
                            showInstructions(temp);
                            codeIsFineToGo = false;
                            isSecondArg = false;
                            continue;
                        }
                        insts = {"INPUT", "STORE" };
                        if ( checkInst( i, insts ) ) { // Não pode alterar const
                            std::cout << "Semantic error: const value cannot be changed in line:" << std::endl;
                            std::vector < std::uint16_t > temp = { i };
                            showInstructions(temp);
                            codeIsFineToGo = false;
                            continue;
                        } else if ( val == 0 && checkInst(i, insts = { "DIV" } )  ) { // Garantir que nao ha divisao por 0
                            std::cout << "Semantic error:" + token + " have value " << val <<  ". Division by zero in line" << std::endl;
                            std::vector < std::uint16_t > temp = { i };
                            showInstructions(temp);
                            codeIsFineToGo = false;
                            continue;
                        }
                    }
                    // Checar se está fazendo acesso correto
                    if ( holdNumOfSpaces > static_cast<size_t> (endCode[i])) {
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
                std::cout << "Semantic error: " << it->first << " declared as a variable but it was already declared as label in:\n" << std::endl;
                codeIsFineToGo = false;
                std::vector<std::uint16_t> vec = { static_cast<std::uint16_t> (it->second) };
                showInstructions( vec );
            }
            
        } else {
            std::cout << token << " declared but not used in line " << currentLine << " in " + line << std::endl;
        }
        currentTokenLine = currentLine;
    }
    std::map < std::string, std::vector< int > >::iterator it;
    // Resolver todas a labels
    for (const auto& kv: labels) {
        // Label foi usada em algum lugar e precisa ser resolvida
        if ( deps.find(kv.first) != deps.end() ) {
            std::vector<std::string> insts = { "JMP", "JMPZ", "JMPP", "JMPN" };
            for (auto &i : deps[kv.first]) {
                if ( (it = useTable.find (kv.first)) != useTable.end() ) { // Checa se não é um simbolo externo
                    it->second.push_back ( i );
                    // endCode[i] = 0;
                } else if ( checkInst( i, insts ) ) { // Checa se a label esta sendo usada na instrucao correta
                    endCode[i] = kv.second;
                } else {
                    std::vector < std::uint16_t > temp = { i };
                    std::cout << "Semantic error: invalid arguments in line:\n " << std::endl;
                    showInstructions(temp);
                    codeIsFineToGo = false;
                }
            }
            // Marca que a pendencia foi resolvida
            deps[kv.first][0] = 65535;
        } else {
            // std::cout << "Label " + kv.first << " declared but not used." << std::endl;
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

    // Resolve os simbolos publicos
    
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
            std::cerr << "Sintax error in transform "<< instructionLine.str() << " to a valid instruction. Index " << varTemp1 << 
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
        // Garantir que o argumento não seja um mnemonico
        for (const auto &i : codes) {
            if ( !var1.compare(i.first) || !var2.compare(i.first) ) {
                std::cout << "Semantic error: Argument cannot be a mnemonic in line " + instructionLine.str() << std::endl;
                codeIsFineToGo = false;
                return;
            }
        }
        
        // Tratar o caso de soma: COPY N1+3,N2+4
        val = Montador::checkIfThereIsSum(var1, instructionLine);
        val1 = Montador::checkIfThereIsSum(var2, instructionLine);
        if ( !Montador::checkVar(var1) ) { // Variável com síbolo errado. Apaga instrução e retorna
            std::cout << "Lexicon error: " << var1 << " is not spelled correctly in " << instructionLine.str() << std::endl;
            codeIsFineToGo = false;
            return;
        }
        if ( !Montador::checkVar(var2)) {
            std::cout << "Lexicon error: " << var2 << " is not spelled correctly in " << instructionLine.str() << std::endl;
            codeIsFineToGo = false;
            return;
        }

        endCode.push_back( val );
        deps[var1].push_back(currentPosition);
        relocable.push_back( 1 );
        currentPosition++;

        endCode.push_back( val1 );
        deps[var2].push_back(currentPosition);
        relocable.push_back( 1 );
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
        // Garantir que o argumento não seja um mnemonico
        for (const auto &i : codes) {
            if ( !instruction.compare(i.first) ) {
                std::cout << "Semantic error: argument cannot be a mnemonic in line " + instructionLine.str() << std::endl;
                codeIsFineToGo = false;
                return;
            }
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
        relocable.push_back( 1 );
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
Retorna verdadeiro se achar uma instrução do tipo [instructions], falso caso contrário
*/
bool Montador::checkInst( std::uint16_t i, std::vector<std::string> &instructions) {
    std::map <std::uint16_t, std::string>::iterator it;
    std::string temp;
    std::uint16_t j;
    // Achar linha provavel. Forma muito tosca de procurar uma instrucao :(
    for (j = i; j > i - 4; j--) {
        if ( (it = instructionLines.find(j)) != instructionLines.end()) {
            break;
        }
    }
    if ( it != instructionLines.end() ) { // Procura na linha pela instrução de salto
        std::stringstream tempSS (it->second);
        tempSS >> temp;
        if ( temp.size() > 0 && temp[temp.size() - 1] == ':' ) { // Pode ser uma label. Pegar próximo token
            tempSS >> temp;
            j++;
        }
        for (auto &inst : instructions) {
            if ( !temp.compare(inst) ) {
                if ( !inst.compare("COPY") && j == i - 2 ) {
                    isSecondArg = true;
                }
                return true;
            }
        }
    } else {
        return false;
    }
    return false;
}