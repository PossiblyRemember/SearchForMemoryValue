; NASM syntax for x64
global add_numbers
section .text
add_numbers:
    mov rax, rdi   ; rdi holds the first argument (a)
    add rax, rsi   ; rsi holds the second argument (b)
    ret            ; return the result in rax