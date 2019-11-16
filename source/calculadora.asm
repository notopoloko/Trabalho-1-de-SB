section .data
perg_nome db "Digite o seu nome: ",0
perg_nome_size EQU $-perg_nome
oi db "Olá, ",0
oi_size EQU $-oi
oi2 db ", bem-vindo ao programa de CALC IA-32", 0dh, 0ah
oi2_size EQU $-oi2
menu db "ESCOLHA UMA OPÇÃO:", 0dh, 0ah, "- 1: SOMA", 0dh, 0ah, "- 2: SUBTRAÇÃO", 0dh, 0ah, "- 3: MULTIPLICAÇÃO", 0dh, 0ah, "- 4: DIVISÃO", 0dh, 0ah, "- 5: MOD", 0dh, 0ah, "- 6: SAIR", 0dh, 0ah, "->" 
menu_size EQU $-menu

section .bss
nome resb 20
opcao resb 2

section .text
global _start
_start:
    mov eax, 4              ; Imprime perg_nome
    mov ebx, 1
    mov ecx, perg_nome
    mov edx, perg_nome_size
    int 80h

    mov eax, 3              ; Lê o nome
    mov ebx, 0
    mov ecx, nome
    mov edx, 20
    int 80h

    mov eax, 4              ; Imprime o olá
    mov ebx, 1
    mov ecx, oi
    mov edx, oi_size
    int 80h

    mov esi, 0              ; Evita imprimir o nl do nome
remove_nl:
    mov al, [nome+esi]
    cmp al, 0ah
    je imprime
    inc esi
    cmp esi, 20
    je imprime
    jmp remove_nl

imprime:
    mov eax, 4              ; Imprime nome sem nl
    mov ebx, 1
    mov ecx, nome
    mov edx, esi
    int 80h

    mov eax, 4              ; Imprime resto das boas-vindas
    mov ebx, 1
    mov ecx, oi2
    mov edx, oi2_size
    int 80h

repete_menu:
    mov eax, 4              ; Imprime menu
    mov ebx, 1
    mov ecx, menu
    mov edx, menu_size
    int 80h

    mov eax, 3              ; Lê a opcao
    mov ebx, 0
    mov ecx, opcao
    mov edx, 1
    int 80h

    mov al, [opcao]
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
