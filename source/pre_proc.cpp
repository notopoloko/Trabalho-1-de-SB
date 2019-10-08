#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pre_proc.hpp"

/*Função responsavel pelo pré-processamento*/
void pre_processamento(std::ifstream &arq, std::string nome_arq){

    /*Retira a extensão atual e coloca a nova .pre*/
    std::size_t point = nome_arq.find_last_of('.');
    if (point == std::string::npos)
    {
        nome_arq += ".pre";
    } else {
        try
        {
            nome_arq.replace(point, std::string::npos, ".pre");
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    /*Cria arquivo para escrita*/
    std::ofstream pre_processado;
    pre_processado.open(nome_arq);
    if ( !pre_processado.is_open() )
    {
        printf("Erro ao abrir o arquivo para pre processamento!\n");
        exit(1);
    }

    // TODO: Ler todo o arquivo de uma vez no lugar de linha a linha
    std::string linha;
    std::map < std::string, std::string> equDefinitions;

    /*Tratamento de EQU no inicio do arquivo*/
    // Adiciona novo caso não haja um
    while (!arq.eof())
    {
        std::getline(arq, linha);
        std::transform(linha.begin(), linha.end(), linha.begin(), ::toupper);

        if ( linha.find("EQU") != std::string::npos ) {
            // equDefinitions[""] = strtok( linha.c_str(), "" ); // Transformar en tokens
        } else if ( linha.find("SECTION") != std::string::npos ) // Fim das definicoes EQU
        {
            break;
        }
        
        
    }
    

    while (!arq.eof())
    {
        std::getline(arq, linha);
        // Torna os caracteres da linha maiúsculos
        std::transform(linha.begin(), linha.end(), linha.begin(), ::toupper);

        // arq >> linha;
        /*Tratamento de Comentários*/
        std::size_t comentario = linha.find_first_of(';');

        try {
            linha.erase(comentario);
        } catch(const std::exception& e) {
            std::cerr << "Nao ha comentario na linha: " << linha << " " << e.what() << '\n';
        }

        /*Tratamento de EQU*/
        // Adiciona novo caso não haja um


        pre_processado << (linha + "\n");
    }

    // Fechar arquivo preprocessado
    pre_processado.close();
}
