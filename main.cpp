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
    int opt;
    std::cout << "Escolha:\n(1) Pré-processar\n(2) Montar\n(3) Simular\n" << argv[1] << std::endl;
    std::cin >> opt;

    FILE *arq;
    // arq = fopen(argv[1], "r");

    // if(arq == NULL){
    //     printf("Erro ao abrir o arquivo!\n");
    //     exit(1);
    // }

    Montador montador;

    switch (opt) {
        case 1:
            pre_processamento(arq, argv[1]);
            break;
        case 2:
            // pre_processamento(arq, argv[1]);
            montador.mount("example/fatorialesomadegaus.pre");
            break;
        case 3:
            // Não rodar essa opção por enquanto
            pre_processamento(arq, argv[1]);
            montador.mount("example/bin.asm");
            simulacao(arq);
            break;
        default:
            std::cout << opt << " não é uma opção correta." << std::endl;
    }

    fclose(arq);
    return 0;
}
