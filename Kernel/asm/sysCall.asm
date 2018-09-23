GLOBAL _syscall_handler

extern syscall_handler

section .text


;syscall handler esta en kernel.c

_syscall_handler:

push rbp
mov rbp, rsp

push rbx
push rcx
push rdx
;push rbp
push rdi
push rsi
push r8
push r9
push r10
push r11
push r12
push r13
push r14
push r15


call syscall_handler

pop r15
pop r14
pop r13
pop r12
pop r11
pop r10
pop r9
pop r8
pop rsi
pop rdi
;pop rbp
pop rdx
pop rcx
pop rbx
pop rbx

mov rsp, rbp
pop rbp

iretq
