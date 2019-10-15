
SECTION TEXT
    ; FAT: EXTERN
    ; GAUSS_SUM: EXTERN
    ; N1: EXTERN
    ; N2: EXTERN
    ; IS_INITIALIZED: EXTERN
    ; PUBLIC RET_GAUSS
    ; PUBLIC RET_FAT
    
    INPUT N
    
    COPY N,N1                       ; 
    JMP FAT                         ; realiza Fatorial de N1
RET_FAT:                            ; volta do fatorial
    STORE ACC                       ; salva na variavel o valor do fatorial
    OUTPUT ACC                      ; e exibe
    
    COPY N,N2                       ; Copia N para N2
    JMP GAUSS_SUM                   ; realiza a soma de gauss
RET_GAUSS:                          ; volta da soma
    STORE ACC                       ; salva na variavel o valor da soma
    OUTPUT ACC                      ; e exibe
    
    STOP                            ; Termina o programa

FAT:                                ; Checa se é o inicio da função
    LOAD IS_INITIALIZED             ; Se
    JMPP MAKE_FAT                   ; ja foi inicializado realiza o fat
    COPY ONE,IS_INITIALIZED         ; se nao muda para inicializado
    COPY ONE,AUX                    ; e garante que aux comece com valor 1
    
MAKE_FAT:                           ; Realiza o fatorial:
    LOAD N1                         ; Se N1 for > 0
    JMPP MULT_N1                    ; Multiplica
    JMP RET1                        ; Se não retorna valor acumulado
    
RET1:                               ; 
    LOAD AUX                        ; Carrega o auxiliar
    COPY ZERO,IS_INITIALIZED        ; muda variavel de inicialização para não inicializado
    JMP RET_FAT                     ; e retorna valor acumulado no auxiliar

GAUSS_SUM:                          ; Checa se é o inicio da função
    LOAD IS_INITIALIZED             ; Se
    JMPP MAKE_SUM                   ; ja foi inicializado realiza a soma
    COPY ONE,IS_INITIALIZED         ; se nao muda para inicializado
    COPY ZERO,AUX                   ; e garante que aux comece com valor 1
    
MAKE_SUM:                           ; Realiza o fatoria:
    LOAD N2                         ; Se N2 for > 0
    JMPP ADD_N2                     ; Soma
    JMP RET2                        ; Se não retorna valor acumulado
    
RET2:                               ; 
    LOAD AUX                        ; Carrega o auxiliar
    COPY ZERO,IS_INITIALIZED        ; muda variavel de inicialização para não inicializado
    JMP RET_GAUSS 
    
MULT_N1:
    LOAD AUX                        ; Carrega valor acumulado
    MUL N1                          ; Multiplica por N1
    STORE AUX                       ; e salva o resultado
    
    LOAD N1                         ; Subtrai 1 de N1
    SUB ONE                         ;
    STORE N1                        ; e salva novo N1
    JMP FAT                         ; realiza o fatorial de N1-1

ADD_N2:
    LOAD AUX                        ; Carrega valor acumulado
    ADD N2                          ; Soma N2
    STORE AUX                       ; e salva o resultado
    
    LOAD N2                         ; Subtrai 1 de N2
    SUB ONE                         ; 
    STORE N2                        ; e salva novo N2
    JMP GAUSS_SUM                   ; realiza o soma de N2-1

    
SECTION DATA
    ACC: SPACE
    N: SPACE
    N1: SPACE
    N2: SPACE
    AUX: SPACE

    IS_INITIALIZED: SPACE

    ONE: CONST 1
    ZERO: CONST 0