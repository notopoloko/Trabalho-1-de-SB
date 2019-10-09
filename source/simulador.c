#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NULO -999999

void num_enderecos(FILE *arq, int tamanho_arq, int *enderecos){
    char linha[tamanho_arq];
    // Por algum motivo que não sei, tem uns números que são partidos quando aloca memória
    //char *linha = (char *) malloc (tamanho_arq * sizeof(char));
    int contador = 0;
    while(fgets(linha, sizeof(linha), arq)){
        char *num = strtok(linha, " ");
        while(num){
            enderecos[contador] = atoi(num);
            num = strtok(NULL, " ");
            contador++;
        }
    }
    //free(linha);
}

void simulacao(FILE *arq){

    fseek(arq, 0, SEEK_END);
    int tamanho_arq = ftell(arq);
    fseek(arq, 0, SEEK_SET);

    int *enderecos = (int*) malloc(tamanho_arq * sizeof(int));
    for(int i = 0; i < tamanho_arq; i++){
        enderecos[i] = NULO;
    }
    num_enderecos(arq, tamanho_arq, enderecos);

    int num_operandos = 0, i = 0, acc = 0, input, end;
    while(enderecos[i] != NULO){
        if(num_operandos == 0){
            if((enderecos[i] > 0 && enderecos[i] < 9) || (enderecos[i] > 9 && enderecos[i] < 14)){
                num_operandos = 1;
            } else if(enderecos[i] == 9){
                num_operandos = 2;
            } else if(enderecos[i] == 14){
                break;
            }
        } else {
            while(num_operandos != 0){
                end = enderecos[i];
                switch(enderecos[i-1]){
                    case 1:
                        acc += enderecos[end];
                        break;
                    case 2:
                        acc -= enderecos[end];
                        break;
                    case 3:
                        acc *= enderecos[end];
                        break;
                    case 4:
                        acc /= enderecos[end];
                        break;
                    case 5:
                        i = enderecos[i]-1;
                        break;
                    case 6:
                        if(acc < 0)
                            i = enderecos[i]-1;
                        break;
                    case 7:
                        if(acc > 0)
                            i = enderecos[i]-1;
                        break;
                    case 8:
                        if(acc == 0)
                            i = enderecos[i]-1;
                        break;
                    case 9:
                        enderecos[enderecos[i+1]] = enderecos[end];
                        break;
                    case 10:
                        acc = enderecos[end];
                        break;
                    case 11:
                        enderecos[end] = acc;
                        break;
                    case 12:
                        printf("Favor, inserir um valor numerico: ");
                        scanf("%d", &input);
                        enderecos[end] = input;
                        break;
                    case 13:
                        printf("O valor de saida eh: %d\n", enderecos[end]);
                        break;
                    default:
                        break;
                }
                printf("ACC: %d\nEnd: %d\nValor no end: %d\n\n", acc, enderecos[i], enderecos[end]);
                if(num_operandos == 2){
                    i++;
                    num_operandos--;
                }
                num_operandos--;
            }
        }
        i++;
    }

    free(enderecos);
}

int main(int argc, char** argv)
{
    FILE *arq;
    arq = fopen(argv[1], "r");

    if(arq == NULL){
        printf("Erro ao abrir o arquivo!\n");
        exit(1);
    }

    simulacao(arq);

    fclose(arq);
    return 0;
}
