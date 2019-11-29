L1: EQU 1

SECTION TEXT
    INPUT V1
    INPUT V1+1
    INPUT V1+2
    INPUT V2
    INPUT V2+1
    INPUT V2+2
    IF L1
        LOAD V1
    ADD V2
    STORE V2
    LOAD V1+1
    ADD V2+1
    STORE V2+1
    LOAD V1+2
    ADD V2+2
    STORE V2+2
    OUTPUT V2
    OUTPUT V2+1
    OUTPUT V2+2
    COPY V2+2,V2+2
    STOP


SECTION DATA
    V1: SPACE 3
    V2: SPACE 3
    V3: SPACE 3S            ;Erro léxico
    V4: SPACE S3            ;Erro léxico
    B1: CONST 3S            ;Erro léxico
    B2: CONST S3            ;Erro léxico
    1B: SPACE               ;Erro léxico
    2B: CONST -0            ;Erro léxico