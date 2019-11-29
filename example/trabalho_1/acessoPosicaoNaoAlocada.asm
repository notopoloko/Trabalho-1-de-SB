L1: EQU 1

SECTION TEXT
    INPUT V1
    INPUT V1+1          ;Acesso a regiao nao reservada
    INPUT V1+2          ;Acesso a regiao nao reservada
    INPUT V2
    INPUT V2+1          ;Acesso a regiao nao reservada
    INPUT V2+2          ;Acesso a regiao nao reservada
    IF L1
        LOAD V1
    ADD V2
    STORE V2
    LOAD V1+1           ;Acesso a regiao nao reservada
    ADD V2+1            ;Acesso a regiao nao reservada
    STORE V2+1          ;Acesso a regiao nao reservada
    LOAD V1+2           ;Acesso a regiao nao reservada
    ADD V2+2            ;Acesso a regiao nao reservada
    STORE V2+2          ;Acesso a regiao nao reservada
    OUTPUT V2
    OUTPUT V2+1         ;Acesso a regiao nao reservada
    OUTPUT V2+2         ;Acesso a regiao nao reservada
    COPY V2+3,V2+3      ;2 acessos a regiao nao reservada
    STOP


SECTION DATA
    V1: SPACE
    V2: SPACE