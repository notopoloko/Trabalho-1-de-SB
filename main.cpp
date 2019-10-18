#include "pre_proc.h"
#include "montador.hpp"
#include "simulador.c"

int main(int argc, char** argv){

    // std::ifstream file (argv[1]);

    // if( !file.is_open() ){
    //     printf("Erro ao abrir o arquivo bruto!\n");
    //     return 1;
    // }

    // pre_processamento(file, argv[1]);

    // file.close();
    if (argc != 2){
        std::cout << "Forma de uso: pre_proc <nome_do_arquivo>" << std::endl;
        return 0;
    }
    
    int opt;
    std::cout << "Escolha:\n(1) Apenas pré-processar\n(2) Apenas montar\n(3) Apenas simular\n(4) Pré-processar e montar\n(5)Pré-processar, montar e simular"<< std::endl;
    std::cin >> opt;

    FILE *arq;
    arq = fopen(argv[1], "r");

    if(arq == NULL){
        printf("Erro ao abrir o arquivo!\n");
        exit(1);
    }

    Montador montador;

    switch (opt) {
        case 1:
            pre_processamento(argv[1]);
            break;
        case 2:
            montador.mount(argv[1]);
            break;
        case 3:
            simulacao(argv[1]);
            break;
        case 4:
            montador.mount( pre_processamento(argv[1]) );
            break;
        case 5:
            // Não rodar essa opção por enquanto. Riscos severos
            simulacao( montador.mount( pre_processamento(argv[1]) ).c_str() );
            break;
        default:
            std::cout << opt << " não é uma opção correta." << std::endl;
    }

    fclose(arq);
    return 0;
}
