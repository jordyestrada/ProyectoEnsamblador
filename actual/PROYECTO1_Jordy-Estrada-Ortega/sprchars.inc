; top-level copy of sprites/chars.inc - HUD and digit drawing

draw_char_fancy proc
    push ax
    push bx
    push si
    
    mov word ptr [temp_x], cx
    mov word ptr [temp_y], dx
    
    mov bx, 0
char_fancy_y:
    mov si, 0
char_fancy_x:
    push ax
    push cx
    push dx
    
    mov cx, word ptr [temp_x]
    add cx, si
    mov dx, word ptr [temp_y]
    add dx, bx
    
    ; Draw only borders
    cmp bx, 1
    je  short char_fancy_draw
    cmp bx, 6
    je  short char_fancy_draw
    cmp si, 1
    je  short char_fancy_draw
    cmp si, 9
    je  short char_fancy_draw
    jmp short char_fancy_skip
    
char_fancy_draw:
    mov ah, 0Ch
    mov al, 14
    mov bh, 0
    int 10h
    
char_fancy_skip:
    pop dx
    pop cx
    pop ax
    inc si
    cmp si, 10
    jl  char_fancy_x
    inc bx
    cmp bx, 8
    jl  char_fancy_y
    
    pop si
    pop bx
    pop ax
    ret
draw_char_fancy endp

draw_char_large proc
    push ax
    push bx
    push si
    
    mov word ptr [temp_x], cx
    mov word ptr [temp_y], dx
    
    mov bx, 2
char_large_y:
    mov si, 2
char_large_x:
    push ax
    push cx
    push dx
    
    mov cx, word ptr [temp_x]
    add cx, si
    mov dx, word ptr [temp_y]
    add dx, bx
    
    mov ah, 0Ch
    mov al, 14
    mov bh, 0
    int 10h
    
    pop dx
    pop cx
    pop ax
    inc si
    cmp si, 12
    jl  char_large_x
    inc bx
    cmp bx, 12
    jl  char_large_y
    
    pop si
    pop bx
    pop ax
    ret
draw_char_large endp
