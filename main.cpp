#include "pre_proc.h"
#include "montador.hpp"
#include "ligador.h"

int main(int argc, char** argv){
    std::string _objFileName1, _objFileName2;
    char *objFileName1, *objFileName2;

    if (argc > 3 || argc < 2){
        std::cout << "Forma de uso: ./montador <nome_do_arquivo> <opt_nome_do_arquivo>" << std::endl;
        return 0;
    }

    // Faz o primeiro processamento
    Montador montador1, montador2;
    _objFileName1 = montador1.mount( pre_processamento(argv[1]) );
    if ( argc == 3 ) {
        _objFileName2 = montador2.mount( pre_processamento(argv[2]) );
    }
    objFileName1 = (char*) malloc ( (_objFileName1.size() + 1) * sizeof(char));
    objFileName2 = (char*) malloc ( (_objFileName2.size() + 1) * sizeof(char));

    size_t i;

    for (i = 0; i < _objFileName1.size(); i++) {
        *(objFileName1 + i) = _objFileName1.c_str()[i];
    }
    *(objFileName1 + i) = '\0';

    for (i = 0; i < _objFileName2.size(); i++) {
        *(objFileName2 + i) = _objFileName2.c_str()[i];
    }
    *(objFileName2 + i) = '\0';
    
    // printf("%s %s\n", objFileName1, objFileName2);
    ligador( objFileName1, objFileName2 );
    // montador.mount( argv[1] );

    return 0;
}
