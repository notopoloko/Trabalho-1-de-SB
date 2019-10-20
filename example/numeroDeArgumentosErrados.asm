	TRIANGULO: EQU 1
SECTION TEXT
		INPUT		B
		INPUT		H
		LOAD		
		MULT		H
		IF TRIANGULO
		DIV		    DOIS
		STORE		R
		OUTPUT	    R
        COPY        B,H,C
        COPY        B,H,

		STOP
SECTION DATA
	B:		SPACE
	H:		SPACE
	R:		SPACE
	DOIS:	CONST		2
