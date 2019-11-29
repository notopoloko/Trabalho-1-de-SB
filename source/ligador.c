#include "ligador.h"

// Função responsável por ligar o código. Recebe o(s) nomes do(s) arquivo(s)
void ligador(char *nome_arq1, char *nome_arq2){
    FILE *arq1;
    arq1 = fopen(nome_arq1, "r");   // Abre arquivo com o primeiro nome

    if(arq1 == NULL){
        printf("Erro ao abrir o arquivo!\n");
        exit(1);
    }

    char aux[50];
    for(size_t i = 0; i < strlen(nome_arq1); i++){
        aux[i] = nome_arq1[i];
        if(nome_arq1[i] == '.') {
            aux[i] = '\0';  // Retira a extensão do nome do primeiro arquivo
            break;
        }
    }

    FILE *saida = fopen(aux, "w");  // Cria um arquivo o nome do primeiro arquivo sem extensão
    if(saida == NULL){
        printf("Erro ao abrir o arquivo de saida!\n");
        exit(1);
    }

    char linha[500];

    int flag_t = 0;

    if(nome_arq2 != NULL){      // Caso tenha como entrada dois arquivos
        int codigo1[300], codigo2[300], cont = 0, cont_h = 0, cont_u = 0, cont_d = 0, fator_correcao;        
        char *tu1[100], *tu2[100], *td1[100], *td2[100], map_bits[300];

        for (int k = 0; k < 100; k++) {  // Preenche com undefined
            tu1[k] = (char*)malloc(50*sizeof(char));
            strcpy(tu1[k], "undefined");
            tu2[k] = (char*)malloc(50*sizeof(char));
            strcpy(tu2[k], "undefined");
            td1[k] = (char*)malloc(50*sizeof(char));
            strcpy(td1[k], "undefined");
            td2[k] = (char*)malloc(50*sizeof(char));
            strcpy(td2[k], "undefined");
        }
        for(int i = 0; i < 300; i++){   // Preenche com NULO
            codigo1[i] = NULO;
            codigo2[i] = NULO;
        }
        
        FILE *arq2;
        arq2 = fopen(nome_arq2, "r");   // Abre o segundo arquivo

        if(arq2 == NULL){
            printf("Erro ao abrir o segundo arquivo para ligação!\n");
            exit(1);
        }

        while(fgets(linha, sizeof(linha), arq1)){   // Enquanto ainda tiverem linhas a se ler
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

                if(!strcmp(token, "H:"))    // Conta o número de H:
                    cont_h++;

                if(!strcmp(token, "T:")){   // Marca se tem T:
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
                cont_d += 2;
            }
            cont_u += 2;
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
                cont_d += 2;
            }
            cont_u += 2;
            cont_d = 0;
        }

        while(codigo2[cont] != NULO){   // Escreve o modulo 2
            if(map_bits[cont] == '1')
                codigo2[cont] += fator_correcao;    // Resolve o fator de correcao para aqueles que ainda precisam
            fprintf(saida, "%d ", codigo2[cont]);
            cont++;
        }

        for(int k = 0; k < 100; k++){
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

// int main(int argc, char** argv){

//     ligador(argv[1], argv[2]);

//     return 0;
// }
