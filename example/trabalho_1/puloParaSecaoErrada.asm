	TRIANGULO: EQU 1
SECTION TEXT
		INPUT		B
		INPUT		H
		LOAD		B
		MULT		H
		IF TRIANGULO
		DIV		DOIS
LABELMAROTA:
        JMP         B           ;Salto para a seção de dados
LABELMAROTA1:
        JMPP        B           ;Salto para a seção de dados
1LABELMAROTA:
        JMPN        R           ;Salto para a seção de dados
        JMPZ        R           ;Salto para a seção de dados
		STORE		R
		OUTPUT  	R
		STOP
SECTION DATA
	B:		SPACE
	H:		SPACE
	R:		SPACE
	DOIS:	CONST		2
