SECTION TEXT
    INPUT V1
    INPUT V1+1
    INPUT V1+2
    INPUT V2
    INPUT V2+1
    INPUT V2+2
    JMP SECONDPART
    LOAD V1
    ADD V2
    STORE V2
    JMP SECONDPART
FIRSTPART:
    LOAD V1+1
    ADD V2+1
    STORE V2+1
    JMP FINAL
SECONDPART:
    LOAD V1+2
    ADD V2+2
    STORE V2+2
    JMP FIRSTPART
FINAL:
    OUTPUT V2
    OUTPUT V2+1
    OUTPUT V2+2
    COPY V2+2,V2+2
    STOP


SECTION DATA
    V1: SPACE 3
    V2: SPACE 3