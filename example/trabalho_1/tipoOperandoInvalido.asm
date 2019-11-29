	TRIANGULO: EQU 1
SECTION TEXT
		INPUT		B
		INPUT		H
		LOAD		B
		MULT		H
		IF TRIANGULO
		DIV		    DOIS
SOMELABEL:
        ADD         1                   ;instrucao com tipo de argumento errado
        ADD         SOMELABEL           ;instrucao com tipo de argumento errado
        COPY        1,SOMELABEL         ;instrucao com tipo de argumento errado
        COPY        SOMELABEL,1         ;instrucao com tipo de argumento errado
        COPY        R,TRIANGULO         ;instrucao com tipo de argumento errado
		STORE		R
        COPY        COPY,ADD            ;instrucao com tipo de argumento errado
        ADD         STOP                ;instrucao com tipo de argumento errado
		STOP
SECTION DATA
	B:		SPACE
	H:		SPACE
	R:		SPACE
	DOIS:	CONST		2
