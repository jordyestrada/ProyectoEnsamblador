; ============================================
; SPRITES_TILES.INC
; All tile drawing procedures
; ============================================

draw_stone_wall proc
    push ax
    push bx
    push si
    mov al, 8
    call fill_tile_base
    mov bx, 2
    mov si, 2
    mov al, 15
    call draw_pixel_at_offset
    mov bx, 5
    mov si, 8
    mov al, 15
    call draw_pixel_at_offset
    mov bx, 8
    mov si, 3
    mov al, 15
    call draw_pixel_at_offset
    mov bx, 4
    mov si, 6
    mov al, 0
    call draw_pixel_at_offset
    mov bx, 12
    mov si, 10
    mov al, 0
    call draw_pixel_at_offset
    mov al, 7
    call draw_top_border
    pop si
    pop bx
    pop ax
    ret
draw_stone_wall endp

draw_water_animated proc
    push ax
    push bx
    push si
    mov al, 6        ; Color marrón/café para lodo
    call fill_tile_base
    mov bx, 3
    mov si, 4
    mov al, 14       ; Color amarillo para burbujas de lodo
    call draw_wave_pattern
    mov bx, 8
    mov si, 12
    mov al, 14       ; Color amarillo para burbujas de lodo
    call draw_wave_pattern
    mov bx, 13
    mov si, 6
    mov al, 14       ; Color amarillo para burbujas de lodo
    call draw_wave_pattern
    mov bx, 6
    mov si, 15
    mov al, 14       ; Color amarillo para detalles
    call draw_pixel_at_offset
    mov bx, 11
    mov si, 8
    mov al, 14       ; Color amarillo para detalles
    call draw_pixel_at_offset
    pop si
    pop bx
    pop ax
    ret
draw_water_animated endp

draw_golden_path proc
    push ax
    push bx
    push si
    mov al, 8        ; Color gris oscuro en lugar de amarillo
    call fill_tile_base
    mov bx, 0
path_y:
    mov si, 0
path_x:
    push ax
    push cx
    push dx
    mov ah, 0Ch
    mov bh, 0
    test bx, 3
    jnz path_check_si
    mov al, 15      ; Blanco para bordes
    jmp short path_draw
path_check_si:
    test si, 3
    jnz path_main
    mov al, 15      ; Blanco para bordes
    jmp short path_draw
path_main:
    mov al, 7       ; Gris claro en lugar de marrón
path_draw:
    add cx, si
    add dx, bx
    int 10h
    pop dx
    pop cx
    pop ax
    inc si
    cmp si, TILE_SIZE
    jl  path_x
    inc bx
    cmp bx, TILE_SIZE
    jl  path_y
    pop si
    pop bx
    pop ax
    ret
draw_golden_path endp

draw_magical_exit proc
    push ax
    push bx
    push si
    mov al, 13
    call fill_tile_base
    mov bx, 8
    mov si, 4
exit_horizontal:
    push cx
    push dx
    mov ah, 0Ch
    mov al, 15
    mov bh, 0
    add cx, si
    add dx, bx
    int 10h
    pop dx
    pop cx
    inc si
    cmp si, 16
    jl  exit_horizontal

    mov bx, 4
    mov si, 8
exit_vertical:
    push cx
    push dx
    mov ah, 0Ch
    mov al, 15
    mov bh, 0
    add cx, si
    add dx, bx
    int 10h
    pop dx
    pop cx
    inc bx
    cmp bx, 16
    jl  exit_vertical

    mov bx, 2
    mov si, 2
    mov al, 14
    call draw_pixel_at_offset
    mov bx, 2
    mov si, 17
    mov al, 14
    call draw_pixel_at_offset
    mov bx, 17
    mov si, 2
    mov al, 14
    call draw_pixel_at_offset
    mov bx, 17
    mov si, 17
    mov al, 14
    call draw_pixel_at_offset

    pop si
    pop bx
    pop ax
    ret
draw_magical_exit endp

draw_gold_coin proc
    push ax
    push bx
    push si
    mov al, 7
    call fill_tile_base
    mov word ptr [temp_y], 6
coin_y_loop:
    mov word ptr [temp_x], 6
coin_x_loop:
    push cx
    push dx
    mov ah, 0Ch
    mov al, 14
    mov bh, 0
    add cx, word ptr [temp_x]
    add dx, word ptr [temp_y]
    int 10h
    pop dx
    pop cx
    inc word ptr [temp_x]
    cmp word ptr [temp_x], 14
    jl  coin_x_loop
    inc word ptr [temp_y]
    cmp word ptr [temp_y], 14
    jl  coin_y_loop
    mov bx, 8
    mov si, 8
    mov al, 15
    call draw_pixel_at_offset
    mov bx, 9
    mov si, 9
    mov al, 15
    call draw_pixel_at_offset
    mov bx, 14
    mov si, 14
    mov al, 6
    call draw_pixel_at_offset
    mov bx, 15
    mov si, 15
    mov al, 6
    call draw_pixel_at_offset
    pop si
    pop bx
    pop ax
    ret
draw_gold_coin endp

draw_crystal_gem proc
    push ax
    push bx
    push si
    
    ; Fondo gris claro
    mov al, 7
    call fill_tile_base
    
    ; Cristal cian brillante más grande (color 11 = cian brillante)
    mov word ptr [temp_y], 4
crystal_y_loop:
    mov word ptr [temp_x], 5
crystal_x_loop:
    push cx
    push dx
    mov ah, 0Ch
    mov al, 11          ; Cian brillante
    mov bh, 0
    add cx, word ptr [temp_x]
    add dx, word ptr [temp_y]
    int 10h
    pop dx
    pop cx
    inc word ptr [temp_x]
    cmp word ptr [temp_x], 15
    jl  crystal_x_loop
    inc word ptr [temp_y]
    cmp word ptr [temp_y], 14
    jl  crystal_y_loop
    
    ; Brillo blanco grande superior
    mov bx, 6
    mov si, 6
    mov al, 15
    call draw_pixel_at_offset
    mov bx, 7
    mov si, 7
    mov al, 15
    call draw_pixel_at_offset
    mov bx, 8
    mov si, 8
    mov al, 15
    call draw_pixel_at_offset
    
    ; Sombra oscura inferior
    mov bx, 13
    mov si, 13
    mov al, 3           ; Cian oscuro
    call draw_pixel_at_offset
    
    pop si
    pop bx
    pop ax
    ret
draw_crystal_gem endp

draw_treasure_chest proc
    push ax
    push bx
    push si
    mov al, 7
    call fill_tile_base
    mov word ptr [temp_y], 7
chest_y_loop:
    mov word ptr [temp_x], 6
chest_x_loop:
    push cx
    push dx
    mov ah, 0Ch
    mov al, 6
    mov bh, 0
    add cx, word ptr [temp_x]
    add dx, word ptr [temp_y]
    int 10h
    pop dx
    pop cx
    inc word ptr [temp_x]
    cmp word ptr [temp_x], 14
    jl  chest_x_loop
    inc word ptr [temp_y]
    cmp word ptr [temp_y], 13
    jl  chest_y_loop
    mov bx, 10
    mov si, 10
    mov al, 14
    call draw_pixel_at_offset
    mov bx, 10
    mov si, 11
    mov al, 14
    call draw_pixel_at_offset
    mov bx, 8
    mov si, 7
    mov al, 8
    call draw_pixel_at_offset
    mov bx, 8
    mov si, 13
    mov al, 8
    call draw_pixel_at_offset
    pop si
    pop bx
    pop ax
    ret
draw_treasure_chest endp

draw_diamond_gem proc
    push ax
    push bx
    push si
    
    ; Fondo gris claro
    mov al, 7
    call fill_tile_base
    
    ; Diamante magenta brillante más grande (color 13 = magenta brillante)
    mov word ptr [temp_y], 4
diamond_y_loop:
    mov word ptr [temp_x], 5
diamond_x_loop:
    push cx
    push dx
    mov ah, 0Ch
    mov al, 13          ; Magenta brillante (muy visible)
    mov bh, 0
    add cx, word ptr [temp_x]
    add dx, word ptr [temp_y]
    int 10h
    pop dx
    pop cx
    inc word ptr [temp_x]
    cmp word ptr [temp_x], 15
    jl  diamond_x_loop
    inc word ptr [temp_y]
    cmp word ptr [temp_y], 14
    jl  diamond_y_loop
    
    ; Brillo blanco grande superior
    mov bx, 6
    mov si, 6
    mov al, 15         ; Blanco
    call draw_pixel_at_offset
    
    mov bx, 7
    mov si, 7
    mov al, 15         ; Blanco
    call draw_pixel_at_offset
    
    mov bx, 8
    mov si, 8
    mov al, 15         ; Blanco
    call draw_pixel_at_offset
    
    ; Sombra oscura inferior derecho
    mov bx, 13
    mov si, 13
    mov al, 5          ; Magenta oscuro
    call draw_pixel_at_offset
    
    pop si
    pop bx
    pop ax
    ret
draw_diamond_gem endp

draw_dark_pit proc
    push ax
    push bx
    push si
    mov al, 9        ; Color azul claro (celeste) para hielo
    call fill_tile_base
    mov al, 15       ; Color blanco para brillo/reflejo
    call draw_full_border
    mov bx, 3
    mov si, 3
    mov al, 15       ; Brillo blanco
    call draw_pixel_at_offset
    mov bx, 5
    mov si, 7
    mov al, 15       ; Brillo blanco
    call draw_pixel_at_offset
    mov bx, 10
    mov si, 10
    mov al, 11       ; Cyan claro para reflejos
    call draw_pixel_at_offset
    mov bx, 2
    mov si, 12
    mov al, 11       ; Cyan claro para reflejos
    call draw_pixel_at_offset
    pop si
    pop bx
    pop ax
    ret
draw_dark_pit endp

draw_stone_floor proc
    push ax
    push bx
    push si
    mov al, 7
    call fill_tile_base
    mov bx, 3
    mov si, 5
    mov al, 15
    call draw_pixel_at_offset
    mov bx, 8
    mov si, 12
    mov al, 15
    call draw_pixel_at_offset
    mov bx, 6
    mov si, 9
    mov al, 8
    call draw_pixel_at_offset
    mov bx, 13
    mov si, 4
    mov al, 8
    call draw_pixel_at_offset
    pop si
    pop bx
    pop ax
    ret
draw_stone_floor endp

draw_tile_enhanced proc
    push cx
    push dx
    push ax

    cmp al, '#'
    jne check_water
    call draw_stone_wall
    jmp tile_done
check_water:
    cmp al, '~'
    jne check_gold
    call draw_water_animated
    jmp tile_done
check_gold:
    cmp al, 'G'
    jne check_crystal
    call draw_gold_coin
    jmp tile_done
check_crystal:
    cmp al, 'C'
    jne check_treasure
    call draw_crystal_gem
    jmp tile_done
check_treasure:
    cmp al, 'T'
    jne check_diamond
    call draw_treasure_chest
    jmp tile_done
check_diamond:
    cmp al, 'D'
    jne check_pit
    call draw_diamond_gem
    jmp tile_done
check_pit:
    cmp al, 'O'
    jne check_path
    call draw_dark_pit
    jmp tile_done
check_path:
    cmp al, '+'
    jne check_exit
    call draw_golden_path
    jmp tile_done
check_exit:
    cmp al, 'E'
    jne check_floor
    call draw_magical_exit
    jmp tile_done
check_floor:
    cmp al, '.'
    jne default_tile
    call draw_stone_floor
    jmp tile_done
default_tile:
    call draw_stone_floor

tile_done:
    pop ax
    pop dx
    pop cx
    ret
draw_tile_enhanced endp
