; sprites_helpers.inc - low level drawing helpers used by tiles and sprites

fill_tile_base proc
    push bx
    push si
    mov bx, 0
base_y:
    mov si, 0
base_x:
    push ax
    push cx
    push dx
    mov ah, 0Ch
    mov bh, 0
    add cx, si
    add dx, bx
    int 10h
    pop dx
    pop cx
    pop ax
    inc si
    cmp si, TILE_SIZE
    jl  base_x
    inc bx
    cmp bx, TILE_SIZE
    jl  base_y
    pop si
    pop bx
    ret
fill_tile_base endp

draw_pixel_at_offset proc
    push cx
    push dx
    mov ah, 0Ch
    mov bh, 0
    add cx, si
    add dx, bx
    int 10h
    pop dx
    pop cx
    ret
draw_pixel_at_offset endp

draw_top_border proc
    push bx
    push si
    mov bx, 0
    mov si, 0
border_top_loop:
    push ax
    push cx
    push dx
    mov ah, 0Ch
    mov bh, 0
    add cx, si
    add dx, bx
    int 10h
    pop dx
    pop cx
    pop ax
    inc si
    cmp si, TILE_SIZE
    jl  border_top_loop
    pop si
    pop bx
    ret
draw_top_border endp

draw_full_border proc
    push bx
    push si
    mov bx, 0
    mov si, 0
border_horizontal:
    push ax
    push cx
    push dx
    mov ah, 0Ch
    mov bh, 0
    add cx, si
    add dx, bx
    int 10h
    add dx, TILE_SIZE-1
    int 10h
    pop dx
    pop cx
    pop ax
    inc si
    cmp si, TILE_SIZE
    jl  border_horizontal

    mov bx, 0
    mov si, 0
border_vertical:
    push ax
    push cx
    push dx
    mov ah, 0Ch
    mov bh, 0
    add cx, si
    add dx, bx
    int 10h
    add cx, TILE_SIZE-1
    int 10h
    pop dx
    pop cx
    pop ax
    inc bx
    cmp bx, TILE_SIZE
    jl  border_vertical

    pop si
    pop bx
    ret
draw_full_border endp

draw_wave_pattern proc
    push cx
    push dx
    mov ah, 0Ch
    mov bh, 0
    add cx, si
    add dx, bx
    int 10h
    inc cx
    int 10h
    inc dx
    int 10h
    dec cx
    int 10h
    pop dx
    pop cx
    ret
draw_wave_pattern endp

draw_diamond_shape proc
    push cx
    push dx
    mov ah, 0Ch
    mov bh, 0
    add cx, si
    add dx, bx
    sub dx, 2
    int 10h
    dec cx
    inc dx
    int 10h
    add cx, 2
    int 10h
    dec cx
    inc dx
    int 10h
    inc dx
    int 10h
    pop dx
    pop cx
    ret
draw_diamond_shape endp
