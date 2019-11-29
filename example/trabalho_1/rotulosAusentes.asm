SECTION TEXT
    INPUT V1
    INPUT V1
    INPUT V1+2
    INPUT V2                ;V2 nao declarado
    INPUT V2+1              ;V2 nao declarado
    INPUT V2+2              ;V2 nao declarado
    jmp SECONDPART
    LOAD V1
    ADD V2                  ;V2 nao declarado
    STORE V2                ;V2 nao declarado
    JMP SECONDPART
ANYPART:
    LOAD V1+1
    ADD V2+1                ;V2 nao declarado
    STORE V2+1              ;V2 nao declarado
    Jmp FINAL
SECONDPART:
    LOAD V1+2
    ADD V2+2                ;V2 nao declarado
    STORE V2+2              ;V2 nao declarado
    jmp FIRSTPART           ;Rótulos ausentes
FINAL:
    OUTPUT V2               ;V2 nao declarado
    OUTPUT V2+1             ;V2 nao declarado
    OUTPUT V2+2             ;V2 nao declarado
    COPY V2+2,V2+2          ;V2 nao declarado
    STOP


SECTION DATA
    V1: SPACE 3
    V3: SPACE 3