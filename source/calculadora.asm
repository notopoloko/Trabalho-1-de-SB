;Universidade de Brasilia - 02/2019
;Software Basico - Turma B
;Professor Bruno Macchiavello
;Trabalho Pratico 2 - Calculadora IA-32
;
;Alunos: 
;        Rafael Amaral Soares        15/0145331
;        Pedro Vitor Valença Mizuno  17/0043665

section .data
perg_nome db "Digite o seu nome: ",0
perg_nome_size EQU $-perg_nome
oi db "Olá, ",0
oi_size EQU $-oi
oi2 db ", bem-vindo ao programa de CALC IA-32", 0dh, 0ah
oi2_size EQU $-oi2
menu db 0dh, 0ah, "ESCOLHA UMA OPÇÃO:", 0dh, 0ah, "- 1: SOMA", 0dh, 0ah, "- 2: SUBTRAÇÃO", 0dh, 0ah, "- 3: MULTIPLICAÇÃO", 0dh, 0ah, "- 4: DIVISÃO", 0dh, 0ah, "- 5: MOD", 0dh, 0ah, "- 6: SAIR", 0dh, 0ah, "-> " 
menu_size EQU $-menu
invalida db "OPÇÃO INVÁLIDA", 0dh, 0ah
invalida_size EQU $-invalida
digite_num db "=> "
digite_num_size EQU $-digite_num
igual db "Resultado: "
igual_size EQU $-igual

section .bss
nome resb 20
opcao resb 2
num1 resb 11
num2 resb 11
enter_fim resb 2
resultado resd 1
inteiro resd 1

section .text
global _start

; Funçao para impressão. Recebe um inteiro de 32 
; bits para impressao com passagem de parâmetro pela pilha
; Registradores usados: eax, ebc, ecx, edx, esi (empilhe antes de chamar)
print_int:
    push ebp
    mov ebp, esp
    mov esi, 0
    mov eax, 0
    mov ecx, 0
    ; Coloca os 16 bits menos significativos em ax
    ; e os mais significativos em dx
    mov eax, [ebp + 8]
    mov ebx, 10

    ; 4294967296 valor máximo de impressão (10 caracteres no maximo + '\0')
    sub esp, 11
    ; Coloca o '\n' no final da string
    inc esi
    mov ecx, ebp
    sub ecx, esi
    mov byte [ecx], 0ah
next:
    inc esi
    mov edx, 0
    ; mov edx, eax
    ; shr edx, 16
    div ebx

    ; Coloca o valor na pilha
    mov ecx, ebp
    sub ecx, esi
    add edx, '0'
    mov byte [ecx], dl
    cmp eax, 0
    jne next

    ; enter 0,0
    ; Imprime o valor apontado por esp - <tamanho_da_string>
    mov eax, 4
    mov ebx, 1

    mov ecx, ebp
    sub ecx, esi
    ; sub ecx, 1

    mov edx, esi
    int 80h

    add esp, 11
    pop ebp
    ret 4

; Função que lê string e transforma em seu valor inteiro
recebe_int:
        push ebp
        mov ebp, esp
        push eax
        push ebx
        push ecx
        push edx
        push esi            ; Empilha registradores que serão usados

        mov eax, 3
        mov ebx, 0
        mov ecx, [ebp+12]
        mov edx, [ebp+8]
        int 80h             ; Lê a string que será o inteiro

        mov esi, 0
        mov eax, 0
        mov ecx, 0
        mov ebx, 0
        mov edx, 0

    repete_int:
        mov ecx, 0
        mov ecx, [ebp+12]
        mov cl, [ecx+esi]   ; Acessa cada caracter da string
        cmp cl, 2dh         ; Compara se é -
        je negativo
        cmp cl, 0ah         ; Compara se é um new line
        je fim
        sub cl, 30h         ; Subtrai 0x30 do atual pra ficar decimal
        mov edx, 10
        push eax
        mov al, cl          ; Move de cl para al, para ser extendido de byte para dword
        cbw
        cwde
        mov ecx, eax        ; Move de volta para ecx
        pop eax
        mul edx             ; Multiplica por 10 o total
        add eax, ecx        ; Soma o atual ao total
    volta:
        inc esi
        cmp esi, 12         ; Compara se já chegou o limite dos 11 caracteres
        je fim
        jmp repete_int

    negativo:               ; Marca o ebx se o número começar com -
        mov ebx, 1
        jmp volta

    fim:
        cmp ebx, 1          ; Se começar com - pula
        je trata_neg
        mov dword [inteiro], eax

        mov esi, 0

    apaga1:                 ; Zera o buffer da entrada
        mov ecx, [ebp+12]
        mov byte [ecx+esi], 0
        inc esi
        cmp esi, 12
        jne apaga1

        pop esi
        pop edx
        pop ecx
        pop ebx
        pop eax
        pop ebp
        ret 8              ; Desempilha usados e retorna

    trata_neg:              ; Obtem o negativo do número
        neg eax
        mov ebx, 0
        jmp fim

; Função que escreve strings, recebendo o endereço e seu tamanho
imprime_string:
    push ebp
    mov ebp, esp
    push eax
    push ebx
    push ecx
    push edx                ; Empilha parametros de registradores usados
    mov eax,4
    mov ebx,1
    mov ecx,[ebp+12]
    mov edx,[ebp+8]
    int 80h                 ; Imprime a string
    pop edx
    pop ecx
    pop ebx
    pop eax
    pop ebp
    ret 8                   ; Desempilha parametros, eip e endereço e tamanho da string

; Função que lê strings, recebendo o endereço e seu tamanho máximo
recebe_string:
    push ebp
    mov ebp, esp
    push eax
    push ebx
    push ecx
    push edx                ; Empilha parametros de registradores usados
    mov eax, 3
    mov ebx, 0
    mov ecx, [ebp+12]
    mov edx, [ebp+8]
    int 80h                 ; Recebe a string
    pop edx
    pop ecx
    pop ebx
    pop eax
    pop ebp
    ret 8                   ; Desempilha parametros, eip e endereço e tamanho da leitura

_start:
    push perg_nome          ; Imprime a pergunta do nome
    push dword perg_nome_size
    call imprime_string

    push nome               ; Recebe o nome
    push dword 20
    call recebe_string

    push oi                 ; Imprime o olá
    push dword oi_size
    call imprime_string

    mov esi, 0              ; Evita imprimir o nl do nome
remove_nl:
    mov al, [nome+esi]
    cmp al, 0ah
    je imprime              ; Se encontrou o new line, pula para imprime
    inc esi
    cmp esi, 20
    je imprime              ; Se, ao passar por toda a string, não achar new line, imprime
    jmp remove_nl

imprime:
    push nome               ; Imprime o nome
    push dword esi
    call imprime_string

    push oi2                ; Imprime o resto das boas-vindas
    push dword oi2_size
    call imprime_string

repete_menu:
    push menu               ; Imprime o menu
    push dword menu_size
    call imprime_string

    push opcao              ; Recebe a opção escolhida
    push dword 2
    call recebe_string

    mov eax, 0
    mov al, [opcao]         ; Pulamos para o operação escolhida
    cmp al, '6'
    je sai

    push eax

    push digite_num         ; Imprime o =>
    push digite_num_size
    call imprime_string

    push num1
    push dword 11
    call recebe_int         ; Chama função para o primeiro inteiro lido
    push dword [inteiro]    ; Empilha valor do primeiro inteiro

    push digite_num
    push digite_num_size
    call imprime_string

    push num2
    push dword 11
    call recebe_int         ; Chama função para o segundo inteiro
    
    pop edx                 ; Desempilha valor do primeiro inteiro em edx

    pop eax

    cmp al, '1'             ; Realiza o jump para a operação escolhida
    je soma
    cmp al, '2'
    je subt
    cmp al, '3'
    je mult
    cmp al, '4'
    je divi
    cmp al, '5'
    je mod

    push invalida           ; Imprime mensagem de opção invalida
    push invalida_size
    call imprime_string
    jmp repete_menu

mostra_result:
    push igual              ; Imprime a string "Resultado:"
    push igual_size
    call imprime_string

    push eax
    push ebx
    push ecx
    push edx
    push esi
    push dword [resultado]  ; Chama a função para transformar e imprimir o inteiro em string
    call print_int
    pop esi
    pop edx
    pop ecx
    pop ebx
    pop eax

espera_enter:               ; Após imprimir o resultado, espera receber ENTER
    push enter_fim
    push dword 2
    call recebe_string
    mov al, [enter_fim]
    cmp al, 0ah
    jne espera_enter

    jmp repete_menu

soma:
    mov eax, edx
    add eax, [inteiro]     ; Realiza as somas

    mov dword [resultado], eax  ; Passa a soma para resultado
    jmp mostra_result

subt:
    mov eax, edx
    sub eax, [inteiro]     ; Realiza a subtração

    mov dword [resultado], eax  ; Passa a subtração para resultado
    jmp mostra_result

mult:
    mov eax, edx
    imul dword [inteiro]     ; Realiza a multiplicação

    mov dword [resultado], eax  ; Passa a multiplicação para resultado

    jmp mostra_result

divi:
    mov eax, edx
    cdq
    idiv dword [inteiro]       ; Realiza a divisão

    mov dword [resultado], eax  ; Passa a divisão para resultado

    jmp mostra_result

mod:
    mov eax, edx
    cdq
    idiv dword [inteiro]     ; Realiza a divisão

    mov dword [resultado], edx  ; Passa o resto para resultado

    jmp mostra_result

sai:
    mov eax, 1              ; Retorna 0
    mov ebx, 0
    int 80h
