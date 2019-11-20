SECTION TEXT
    INPUT V1
    INPUT V1
    INPUT V1+2
    INPUT V2
    INPUT V2+1
    INPUT V2+2
    jmp SECONDPART
    LOAD V1
    ADD V2
    STORE V2
    JMP SECONDPART
FIRSTPART:
    LOAD V1+1
SECONDPART:
    ADD V2+1
    STORE V2+1
    Jmp FINAL
SECONDPART:                     ;Rótulo SECONDPART repetido
    LOAD V1+2
    ADD V2+2
    STORE V2+2
    jmp FIRSTPART
FINAL:
    OUTPUT V2
    OUTPUT V2+1
    OUTPUT V2+2
    COPY V2+2,V2+2
    LOAD DOIS
    STOP


SECTION DATA
    V1: SPACE 3
    V2: SPACE 3
    V2: SPACE 3                 ;Redefinição de space
    SECONDPART: SPACE 3         ;Rótulo SECONDPART redeclarado como variavel
    DOIS: CONST 2
    DOIS: CONST 3               ;Redefinição de constante