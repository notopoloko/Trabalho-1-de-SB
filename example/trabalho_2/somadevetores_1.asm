SOMAVETORES_1: BEGIN
SECTION TEXT
    SOMADEVETORES_2: EXTERN
    PUBLIC CONTINUE
    PUBLIC V1
    PUBLIC V2

    JMP SOMADEVETORES_2
    
CONTINUE:
    OUTPUT V2
    OUTPUT V2+1
    OUTPUT V2+2
    COPY V2+2,V2+2
    STOP
SECTION DATA
    V1: SPACE 3
    V2: SPACE 3
END