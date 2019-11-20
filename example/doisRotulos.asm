	TRIANGULO: EQU 1
SECTION TEXT
		INPUT		B
		INPUT		H
		LOAD		B
SOMELABEL:						;labels com mesmo endereço
SECONDLABEL:					;labels com mesmo endereço
ANOTHERLABEL:					;labels com mesmo endereço
        MULT		H
		IF TRIANGULO
		DIV			DOIS
		STORE		R
		OUTPUT		R
		STOP
SECTION DATA
	B:						;labels com mesmo endereço
    C:      SPACE			;labels com mesmo endereço
	H:		SPACE
	R:		SPACE
	DOIS:	CONST		2
