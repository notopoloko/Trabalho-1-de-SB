	TRIANGULO: EQU 1
SECTION TEXT
		INPUT					;Instrução INPUT sem argumentos
		INPUT		H
		LOAD		B
		MULT		H
		IF TRIANGULO
		DIV		    DOIS
		STORE		R
		OUTPUT	    R
        copy        B,H,C		;Instrução COPY com argumentos errados
        copy        b,h,		;Instrução COPY com argumentos errados
		STOP		
		STOP		R			;Instrução STOP com argumento
SECTION DATA
	B:		SPACE
	H:		SPACE
	R:		SPACE
	DOIS:	CONST		2
