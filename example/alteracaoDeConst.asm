	TRIANGULO: EQU 1
SECTION TEXT
		INPUT		B
		INPUT		H
		LOAD		B
		MULT		H
		IF TRIANGULO
		DIV 		DOIS
        INPUT       DOIS		;Alteração de const
        STORE       DOIS		;Alteração de const
        COPY        R,DOIS		;Alteração de const
        INPUT       TRES		;Alteração de const
        STORE       TRES		;Alteração de const
        COPY        R,TRES		;Alteração de const
		COPY        TRES,R
		STORE		R
		OUTPUT	    R
		STOP
SECTION DATA
	B:		SPACE
	H:		SPACE
	R:		SPACE
	DOIS:	CONST		2
    TRES:   CONST       3
