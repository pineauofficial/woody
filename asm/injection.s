section .data
    msg db "...WOODY...", 0

section .text
    global print_woody

print_woody:
    push rdi

    mov rdi, 1
    lea rsi, [msg]
    mov rdx, 12 
    syscall

    pop rdi
    jmp rdi



