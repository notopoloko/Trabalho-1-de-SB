#include "pre_proc.h"
#include "montador.hpp"

int main(int argc, char** argv){
    if (argc != 2){
        std::cout << "Forma de uso: pre_proc <nome_do_arquivo>" << std::endl;
        return 0;
    }
    
    Montador montador;
    montador.mount( pre_processamento(argv[1]) );

    return 0;
}
