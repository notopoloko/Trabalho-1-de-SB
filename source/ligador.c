#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NULO -11037

void ligador(char *nome_arq1, char *nome_arq2){
    FILE *arq1;
    arq1 = fopen(nome_arq1, "r");

    if(arq1 == NULL){
        printf("Erro ao abrir o arquivo!\n");
        exit(1);
    }

    FILE *saida = fopen("ligado.obj", "w");
    if(saida == NULL){
        printf("Erro ao abrir o arquivo de saida!\n");
        exit(1);
    }

    char linha[300];

    int flag_h = 0, flag_t = 0;

    if(nome_arq2 != NULL){      // Caso tenha como entrada dois arquivos
        int codigo1[300], codigo2[300], cont = 0, cont_h = 0, cont_u = 0, cont_d = 0, fator_correcao;        
        char *tu1[30], *tu2[30], *td1[30], *td2[30], map_bits[150];

        for (int k = 0; k < 30; k++) {
            tu1[k] = (char*)malloc(50*sizeof(char));
            strcpy(tu1[k], "undefined");
            tu2[k] = (char*)malloc(50*sizeof(char));
            strcpy(tu2[k], "undefined");
            td1[k] = (char*)malloc(50*sizeof(char));
            strcpy(td1[k], "undefined");
            td2[k] = (char*)malloc(50*sizeof(char));
            strcpy(td2[k], "undefined");
        }
        for(int i = 0; i < 300; i++){
            codigo1[i] = NULO;
            codigo2[i] = NULO;
        }
        
        FILE *arq2;
        arq2 = fopen(nome_arq2, "r");

        if(arq2 == NULL){
            printf("Erro ao abrir o arquivo!\n");
            exit(1);
        }

        while(fgets(linha, sizeof(linha), arq1)){
            char *token = strtok(linha, " \n\t");
            while(token){

                if(flag_t == 1){
                    sscanf(token, "%d", &codigo1[cont]);    // Armazena em codigo1 a parte de texto do arq1
                    cont++;
                }

                if(cont_h == 2 && strcmp(token, "H:") && strcmp(token, "T:")){  // Armazena o tamanho do arq1
                    sscanf(token, "%d", &fator_correcao);
                }

                if(cont_h == 4 && strcmp(token, "H:") && strcmp(token, "T:")){  // Armazena tabela de uso do arq1
                    strcpy(tu1[cont_u], token);
                    cont_u++;
                }

                if(cont_h == 5 && strcmp(token, "H:") && strcmp(token, "T:")){  // Armazena tabela de definicoes do arq1
                    strcpy(td1[cont_d], token);
                    cont_d++;
                }

                if(!strcmp(token, "H:"))
                    cont_h++;

                if(!strcmp(token, "T:")){
                    flag_t = 1;
                    cont_h++;
                }

                token = strtok(NULL, " \n\t");
            }
            flag_t = 0;
        }
        cont_h = 0;
        cont_u = 0;
        cont_d = 0;
        cont = 0;

        while(fgets(linha, sizeof(linha), arq2)){
            char *token = strtok(linha, " \n\t");
            while(token){
                if(flag_t == 1){
                    sscanf(token, "%d", &codigo2[cont]);    // Armazena em codigo2 a parte de texto do arq2
                    cont++;
                }

                if(cont_h == 3 && strcmp(token, "H:") && strcmp(token, "T:")){  // Armazena o mapa de bits do arq2
                    strcpy(map_bits, token);
                }

                if(cont_h == 4 && strcmp(token, "H:") && strcmp(token, "T:")){  // Armazena tabela de uso do arq2
                    strcpy(tu2[cont_u], token);
                    cont_u++;
                }

                if(cont_h == 5 && strcmp(token, "H:") && strcmp(token, "T:")){  // Armazena tabela de definicoes do arq2
                    strcpy(td2[cont_d], token);
                    cont_d++;
                }

                if(!strcmp(token, "H:"))
                    cont_h++;

                if(!strcmp(token, "T:")){
                    flag_t = 1;
                    cont_h++;
                }

                token = strtok(NULL, " \n\t");
            }
            flag_t = 0;
        }
        
        cont_h = 0;
        cont_u = 0;
        cont_d = 0;
        cont = 0;
        int aux1, aux2;
        
        /*Resolve TU, TD e fator de correcao do modulo 1*/
        while(strcmp(tu1[cont_u], "undefined")){
            while(strcmp(td2[cont_d], "undefined")){

                if(!strcmp(tu1[cont_u], td2[cont_d])){
                    sscanf(tu1[cont_u + 1], "%d", &aux1);   // Soma valor da tabela de definicao do 2
                    sscanf(td2[cont_d + 1], "%d", &aux2);   // Soma o fator de correcao do 2
                    codigo1[aux1] += aux2;
                    codigo1[aux1] += fator_correcao;
                }
                cont_d++;
            }
            cont_u++;
            cont_d = 0;
        }
        while(codigo1[cont] != NULO){   // Escreve o modulo 1
            fprintf(saida, "%d ", codigo1[cont]);
            cont++;
        }

        cont_h = 0;
        cont_u = 0;
        cont_d = 0;
        cont = 0;

        /*Resolve TU e TD do modulo 2*/
        while(strcmp(tu2[cont_u], "undefined")){
            while(strcmp(td1[cont_d], "undefined")){

                if(!strcmp(tu2[cont_u], td1[cont_d])){
                    sscanf(tu2[cont_u + 1], "%d", &aux1);
                    sscanf(td1[cont_d + 1], "%d", &aux2);
                    codigo2[aux1] += aux2;
                    map_bits[aux1] = '0';   // Marca para nao precisar do fator de correcao
                }
                cont_d++;
            }
            cont_u++;
            cont_d = 0;
        }

        while(codigo2[cont] != NULO){   // Escreve o modulo 2
            if(map_bits[cont] == '1')
                codigo2[cont] += fator_correcao;    // Resolve o fator de correcao para aqueles que ainda precisam
            fprintf(saida, "%d ", codigo2[cont]);
            cont++;
        }

        for(int k = 0; k < 30; k++){
            free(tu1[k]);
            free(tu2[k]);
            free(td1[k]);
            free(td2[k]);
        }

    } else {            // Caso tenha como entrada só um arquivo
        while(fgets(linha, sizeof(linha), arq1)){
            char *token = strtok(linha, " \n\t");
            while(token){

                if(flag_t == 1){
                    fprintf(saida, "%s ", token);
                }

                if(!strcmp(token, "H:"))
                    break;
                
                if(!strcmp(token, "T:"))
                    flag_t = 1;

                token = strtok(NULL, " \n\t");
            }
            flag_t = 0;
        }
    }
}

int main(int argc, char** argv){

    ligador(argv[1], argv[2]);

    return 0;
}