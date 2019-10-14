#include "pre_proc.h"

/*Função que torna todos os tokens maiúsculos*/
void ToUp(char *p){
    while(*p){
        *p=toupper(*p);
        p++;
    }
}

/*Função responsavel pelo pré-processamento*/
void pre_processamento(FILE *arq, char *nome_arq){
    printf("Pre-processamento:\n");

    /*Retira a extensão atual e coloca a nova .pre*/
    char aux[50];
    for(size_t i = 0; i < strlen(nome_arq); i++){
	    if(nome_arq[i] == '.'){
	        nome_arq[i+1] = '\0';
	    }
    }
    strcat(aux, nome_arq);
    strcat(aux, "pre");

    /*Cria arquivo para escrita*/
    FILE *pre_processado = fopen(aux, "w");
    if(pre_processado == NULL){
        printf("Erro ao abrir o arquivo!\n");
        exit(1);
    }

    char pen_token[50], linha[300], str[50], antepen_token[50];
    char *equ[10], *equ_trocado[10];
    for (int k = 0; k < 10; k++) {
        equ[k] = (char*)malloc(50*sizeof(char));
        strcpy(equ[k], "undefined");
        equ_trocado[k] = (char*)malloc(50*sizeof(char));
        strcpy(equ_trocado[k], "undefined");
    }
    int equ_total = 0, num_linha = 1, flag_enter = 0, f = 0;
    int flag_comment = 0, flag_if = 0, flag_section = 0, flag_if_print = 0, flag_pula_linha = 0, flag_espaco = 0;

    while(fgets(linha, sizeof(linha), arq)){
        flag_enter = 0;
        char *token = strtok(linha, " \n\t");	// Separa cada token por espaco, nova linha e tab
        while(token){

	        if(flag_espaco == 1)	// Responsável pelo espaço entre tokens no novo arquivo
		        fprintf(pre_processado, " ");

            ToUp(token);	// Torna os caracteres do token maiúsculos

            /*Tratamento de Comentários*/
            if(token[0] == ';')     // Retira comentários no formato: token ;comentario / ; comentario
                break;
            for(size_t i = 0; i < strlen(token); i++){ // Sinaliza comentários grudados ao token: token;comentario / token; comentario
                if(token[i] == ';'){
                    token[i] = '\0';
                    flag_comment = 1;
                }
            }

            /*Tratamento de Hexadecimais*/
            if(token[0] == '0' && token[1] == 'X'){
                long n = strtol(token, NULL, 16);
                sprintf(token, "%ld", n);
            }

            /*Tratamento de rótulo com quebra de linha*/
            if(flag_enter == 1){
                flag_enter++;
            }
            for(int j = 0; j < 50; j++){
                if(token[j] == ':')
                    flag_enter = 1;
            }

            /*Tratamento de EQU*/
            for(int i = 0; i < equ_total; i++){		//	Substitui o token do EQU pelo novo token equivalente
                if(!strcmp(token, equ[i])){
                    sprintf(token, "%s", equ_trocado[i]);
                }
            }

            if(!strcmp(pen_token, "EQU")){			// Detecta o EQU e recebe os tokens equivalentes
                strcpy(str, antepen_token);
                str[(strlen(str)-1)] = '\0';
                strcpy(equ[equ_total], str);
                strcpy(equ_trocado[equ_total], token);
                equ_total++;
                int tem_dp = 0;
                for(int i = 0; i < 50; i++){
                    if(antepen_token[i] == ':')
                        tem_dp = 1;
                }
                if(tem_dp == 0)
                    printf("Erro sintatico na linha %d\n", num_linha);
            }

            /*Tratamento de IF*/
            if(flag_if == 1){				// Pula a linha caso o IF seja acionado
                flag_if = 0;
                f = 1;
                break;
            }

            if(!strcmp(token, "IF"))		// Faz o IF não ser escrito no arquivo
                flag_if_print = 1;

            if(!strcmp(pen_token, "IF")){	// Detecta o IF
                if(!strcmp(token, "0")){
                    flag_if = 1;
                }
		        flag_if_print = 2;
            }

            strcpy(antepen_token, pen_token);
            strcpy(pen_token, token);

            if(!strcmp(token, "SECTION")){	// Detecta o nome de sessões
                flag_section = 1;
            }

            if(flag_section == 1 && flag_if_print != 1 && flag_if_print != 2){ // Escreve no arquivo a partir de SECTION TEXT
                fprintf(pre_processado, "%s", token);
		        flag_pula_linha = 1;
		        flag_espaco = 1;
	        }

            if(flag_if_print == 2)
                flag_if_print = 0;

            if(flag_comment == 1){   // Retira os comentários sinalizados
                flag_comment = 0;
                break;
            }

            token = strtok(NULL, " \n\t");
        }
        if(flag_enter == 1 && f != 1){
            fprintf(pre_processado, " ");
        }

	    if(flag_pula_linha == 1 && flag_enter != 1)	// Controle da escrita de nova linha no arquivo
	        fprintf(pre_processado, "\n");

    	flag_pula_linha = 0;
    	flag_espaco = 0;
        f = 0;
        num_linha++;
    }

    for (int k = 0; k < 10; k++){
        free(equ[k]);
        free(equ_trocado[k]);
    }

    fclose(pre_processado);
}
