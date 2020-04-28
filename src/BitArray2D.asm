
        global  set_bit_elem
        global  get_bit_elem
        section .text

set_bit_elem:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame

        ; rdi contains array pointer
        ; rsi contains row width
        ; rdx contains row
        ; rcx contains col

        ; add your code here
        mov rax, rdx        ; move row to rax
        imul rax, rsi       ; multiply row by row width and save in rax
        add rax, rcx        ; add column to row*row_width and save in rax
        xor rdx, rdx        ; clear rdx
        mov rcx, 8          ; write 8 to rcx, which is the number of bits of a byte
        div rcx             ; divide rax by rcx (=8) and save ratio (which is the byte index) in rax and the remainder (which is the bit index) in rdx
        mov rbx, 1          ; write 1 to rbx
lbegin: cmp rdx, 0          ; check if the remainder is 0
        je lend             ; jump if our comparison was equal (meaning rdx == 0)
        shl rbx, 1          ; move the 1 to the correct bit position
        sub rdx, 1          ; decrease the remainder by one
        jmp lbegin          ; jump back to beginning of the loop
lend:   mov rcx, [rdi+rax]  ; write the array byte into rax
        or rcx, rbx         ; combine the existing byte value with the new value
        mov [rdi+rax], rcx  ; write rcx back to the array


        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                 ; return value in rax




get_bit_elem:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame

        ; rdi contains array pointer
        ; rsi contains row width
        ; rdx contains row
        ; rcx contains col

        ; add your code here - for now returning 0
        mov rax, rdx        ; move row to rax
        imul rax, rsi       ; multiply row by row width and save in rax
        add rax, rcx        ; add column to row*row_width and save in rax
        xor rdx, rdx        ; clear rdx
        mov rcx, 8          ; write 8 to rcx, which is the number of bits of a byte
        div rcx             ; divide rax by rcx (=8) and save ratio (which is the byte index) in rax and the remainder (which is the bit index) in rdx
        mov rbx, [rdi+rax]  ; write the array byte into rax
lbegin2:cmp rdx, 0          ; check if the remainder is 0
        je lend2            ; if the previous comparison was equal, jump to lend2
        shr rbx, 1          ; move the register to the right by 1 bit
        sub rdx, 1          ; decrement the remainder by 1
        jmp lbegin2         ; jump back to our loop
lend2:  mov rax, rbx        ; now our search bit is at position 0 of our register rbx; write rbx to rax
        and rax, 1          ; set rax to 1 if the first bit is 1, or to 0 if the first bit is 0. This value will be returned from this function

        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                 ; return value in rax
