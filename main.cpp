#include "pre_proc.h"
#include "montador.hpp"

int main(int argc, char** argv){

    // std::ifstream file (argv[1]);

    // if( !file.is_open() ){
    //     printf("Erro ao abrir o arquivo bruto!\n");
    //     return 1;
    // }

    // pre_processamento(file, argv[1]);

    // file.close();

    FILE *arq;
    arq = fopen(argv[1], "r");

    if(arq == NULL){
        printf("Erro ao abrir o arquivo!\n");
        exit(1);
    }

    // pre_processamento(arq, argv[1]);

    Montador montador;
    montador.mount("example/bin.asm");

    fclose(arq);
    return 0;
}
