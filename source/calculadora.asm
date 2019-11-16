section .data
perg_nome db "Digite o seu nome: ",0
perg_nome_size EQU $-perg_nome
oi db "Olá, ",0
oi_size EQU $-oi
oi2 db ", bem-vindo ao programa de CALC IA-32", 0dh, 0ah
oi2_size EQU $-oi2
menu db 0dh, 0ah, "ESCOLHA UMA OPÇÃO:", 0dh, 0ah, "- 1: SOMA", 0dh, 0ah, "- 2: SUBTRAÇÃO", 0dh, 0ah, "- 3: MULTIPLICAÇÃO", 0dh, 0ah, "- 4: DIVISÃO", 0dh, 0ah, "- 5: MOD", 0dh, 0ah, "- 6: SAIR", 0dh, 0ah, "->" 
menu_size EQU $-menu
invalida db "OPÇÃO INVÁLIDA", 0dh, 0ah
invalida_size EQU $-invalida

section .bss
nome resb 20
opcao resb 2

section .text
global _start

imprime_string:             ; Função que escreve strings
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
    ret 12                  ; Desempilha parametros, eip e endereço e tamanho da string

recebe_string:              ; Função que lê strings
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
    ret 12                  ; Desempilha parametros, eip e endereço e tamanho da leitura

_start:
    push perg_nome          ; Imprime a pergunta do nome
    push perg_nome_size
    call imprime_string

    push nome               ; Recebe inicio
    push 20
    call recebe_string

    push oi                 ; Imprime o olá
    push oi_size
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
    push esi
    call imprime_string

    push oi2                ; Imprime o resto das boas-vindas
    push oi2_size
    call imprime_string

repete_menu:
    push menu               ; Imprime o menu
    push menu_size
    call imprime_string

    push opcao              ; Recebe a opção escolhida
    push 2
    call recebe_string

    mov al, [opcao]         ; Pulamos para o operação escolhida
    cmp al, 31h
    je soma
    cmp al, 32h
    je subt
    cmp al, 33h
    je mult
    cmp al, 34h
    je divi
    cmp al, 35h
    je mod
    cmp al, 36h
    je sai

    push invalida
    push invalida_size
    call imprime_string
    jmp repete_menu

soma:
    jmp repete_menu

subt:
    jmp repete_menu

mult:
    jmp repete_menu

divi:
    jmp repete_menu

mod:
    jmp repete_menu

sai:
    mov eax, 1              ; Retorna 0
    mov ebx, 0
    int 80h
