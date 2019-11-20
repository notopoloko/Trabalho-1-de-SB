	TRIANGULO: EQU 1
SECTION TEXT
		INPUT		B
		INPUT		H
		LOAD		B
		MULT		H
		IF TRIANGULO
		DIV 		DOIS        ;divisao por zero
        LOAD        R
        DIV         ZERO        ;divisao por zero
		STORE		R
		OUTPUT  	R
		STOP
SECTION DATA
	B:		SPACE
	H:		SPACE
	R:		SPACE
	DOIS:	CONST		0
	ZERO:	CONST		0
