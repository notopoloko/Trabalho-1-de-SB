SECTION TEXT
    INPUT V1
    INPUT V1+1
    INPUT V1+2
    INPUT V2
    INPUT V2+1
    INPUT V2+2
    JMP SECONDPART
    LOAD V1
    add V2
    store V2
    jmp SECONDPART
FIRSTPART:
    load V1+1
    add V2+1
    store V2+1
    jmp final
SECONDPART:
    load V1+2
    add V2+2
    store V2+2
    jmp FIRSTPART
final:
    output V2
    output V2+1
    output V2+2
    stop
SECTION data
    V1: SPACE 3
    V2: SPACE 3
    FIRSTPART: CONST 1