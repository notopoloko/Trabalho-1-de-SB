	TRIANGULO: EQU 1
SECTION TEXT
		INPUT		B
		INPUT		H
		LOAD		B
		MULT		H
		IF TRIANGULO
		DIV 		DOIS
        INPUT       DOIS
        STORE       DOIS
        COPY        R,DOIS
        INPUT       TRES
        STORE       TRES
        COPY        R,TRES
		STORE		R
		OUTPUT	    R
		STOP
SECTION DATA
	B:		SPACE
	H:		SPACE
	R:		SPACE
	DOIS:	CONST		2
    TRES:   CONST       3
