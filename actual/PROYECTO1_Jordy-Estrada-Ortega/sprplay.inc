; top-level copy of sprites/player.inc - player drawing and animation

draw_player_enhanced proc
    push ax
    push bx
    push cx
    push dx
    push si

    mov al, byte ptr [px]
    sub al, byte ptr [camera_x]
    mov ah, 0
    mov dx, TILE_SIZE
    mul dx
    add ax, 40
    mov cx, ax

    mov al, byte ptr [py]
    sub al, byte ptr [camera_y]
    mov ah, 0
    mov dx, TILE_SIZE
    mul dx
    add ax, 50
    mov dx, ax

    cmp cx, 600
    jg  player_skip_enh
    cmp dx, 320
    jg  player_skip_enh

    call draw_hero_sprite

player_skip_enh:
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    ret
draw_player_enhanced endp

;========================================================
; SPRITE DE LINK - The Legend of Zelda
; Adaptado para tiles de 16x16 p�xeles
;========================================================
; Paleta EGA:
; 2=verde oscuro, 6=marr�n, 7=piel, 8=sombra,
; 10=verde claro, 12=rojo, 14=amarillo, 15=blanco
;========================================================

draw_hero_sprite proc
    push ax
    push bx
    push si
    mov al, byte ptr [player_direction]
    cmp al, 0
    je short go_front
    cmp al, 1
    je short go_back
    cmp al, 2
    je short go_side
    cmp al, 3
    je short go_side
    jmp short go_front

go_front:
    jmp draw_link_front
go_back:
    jmp draw_link_back
go_side:
    jmp draw_link_side

draw_link_front:
    ; Linea 0 - Gorro
    mov bx, 0
    mov si, 3
    mov al, 2
    call draw_horizontal_line_8
    
    ; Linea 1 - Gorro con borde
    mov bx, 1
    mov si, 2
    mov al, 10
    call draw_pixel_at_offset
    mov si, 3
    mov al, 2
    call draw_horizontal_line_8
    mov si, 11
    mov al, 10
    call draw_pixel_at_offset
    
    ; Linea 2 - Pelo
    mov bx, 2
    mov si, 2
    mov al, 14
    call draw_horizontal_line_10
    
    ; Linea 3 - Cara con ojos
    mov bx, 3
    mov si, 3
    mov al, 7
    call draw_pixel_at_offset
    mov si, 4
    mov al, 15
    call draw_pixel_at_offset
    mov si, 5
    mov al, 7
    call draw_horizontal_line_4
    mov si, 9
    mov al, 15
    call draw_pixel_at_offset
    mov si, 10
    mov al, 7
    call draw_pixel_at_offset
    
    ; Linea 4-5 - Cara
    mov bx, 4
    mov si, 3
    mov al, 7
    call draw_horizontal_line_8
    mov bx, 5
    mov si, 4
    mov al, 7
    call draw_horizontal_line_6
    
    ; Lineas 6-8 - Tunica verde parte superior
    mov bx, 6
    mov si, 2
    mov al, 2
    call draw_horizontal_line_10
    mov bx, 7
    mov si, 1
    mov al, 10
    call draw_pixel_at_offset
    mov si, 2
    mov al, 2
    call draw_horizontal_line_10
    mov si, 12
    mov al, 10
    call draw_pixel_at_offset
    mov bx, 8
    mov si, 2
    mov al, 2
    call draw_horizontal_line_10
    
    ; Linea 9 - Cinturon
    mov bx, 9
    mov si, 3
    mov al, 6
    call draw_horizontal_line_8
    
    ; Lineas 10-12 - Tunica parte inferior
    mov bx, 10
    mov si, 2
    mov al, 2
    call draw_horizontal_line_10
    mov bx, 11
    call draw_horizontal_line_10
    mov bx, 12
    mov si, 3
    call draw_horizontal_line_8
    
    ; Lineas 13-15 - Piernas
    mov bx, 13
    mov si, 4
    mov al, 12
    call draw_horizontal_line_6
    mov bx, 14
    mov si, 3
    mov al, 6
    call draw_pixel_at_offset
    mov si, 4
    mov al, 12
    call draw_horizontal_line_2
    mov si, 8
    call draw_horizontal_line_2
    mov si, 10
    mov al, 6
    call draw_pixel_at_offset
    mov bx, 15
    mov si, 3
    mov al, 6
    call draw_pixel_at_offset
    mov si, 10
    call draw_pixel_at_offset
    
    jmp draw_link_end

draw_link_back:
    ; Lineas 0-4 - Gorro/capucha grande
    mov bx, 0
    mov si, 3
    mov al, 10
    call draw_horizontal_line_8
    mov bx, 1
    mov si, 2
    call draw_horizontal_line_10
    mov bx, 2
    mov si, 1
    call draw_horizontal_line_12
    mov bx, 3
    mov si, 2
    call draw_horizontal_line_10
    mov bx, 4
    mov si, 3
    mov al, 2
    call draw_horizontal_line_8
    
    ; Lineas 5-8 - Espalda de tunica
    mov bx, 5
    mov si, 2
    call draw_horizontal_line_10
    mov bx, 6
    call draw_horizontal_line_10
    mov bx, 7
    mov si, 1
    mov al, 10
    call draw_pixel_at_offset
    mov si, 2
    mov al, 2
    call draw_horizontal_line_10
    mov si, 12
    mov al, 10
    call draw_pixel_at_offset
    mov bx, 8
    mov si, 2
    mov al, 2
    call draw_horizontal_line_10
    
    ; Linea 9 - Cinturon
    mov bx, 9
    mov si, 3
    mov al, 6
    call draw_horizontal_line_8
    
    ; Lineas 10-12 - Tunica inferior
    mov bx, 10
    mov si, 2
    mov al, 2
    call draw_horizontal_line_10
    mov bx, 11
    call draw_horizontal_line_10
    mov bx, 12
    mov si, 3
    call draw_horizontal_line_8
    
    ; Lineas 13-15 - Piernas
    mov bx, 13
    mov si, 4
    mov al, 12
    call draw_horizontal_line_6
    mov bx, 14
    mov si, 3
    mov al, 6
    call draw_pixel_at_offset
    mov si, 4
    mov al, 12
    call draw_horizontal_line_2
    mov si, 8
    call draw_horizontal_line_2
    mov si, 10
    mov al, 6
    call draw_pixel_at_offset
    mov bx, 15
    mov si, 3
    mov al, 6
    call draw_pixel_at_offset
    mov si, 10
    call draw_pixel_at_offset
    
    jmp draw_link_end

draw_link_side:
    ; Linea 0-1 - Gorro lateral
    mov bx, 0
    mov si, 4
    mov al, 2
    call draw_horizontal_line_6
    mov bx, 1
    mov si, 3
    mov al, 10
    call draw_pixel_at_offset
    mov si, 4
    mov al, 2
    call draw_horizontal_line_6
    mov si, 10
    mov al, 10
    call draw_pixel_at_offset
    
    ; Lineas 2-3 - Pelo y cara
    mov bx, 2
    mov si, 4
    mov al, 14
    call draw_horizontal_line_6
    mov bx, 3
    mov si, 4
    mov al, 7
    call draw_pixel_at_offset
    mov si, 5
    mov al, 15
    call draw_pixel_at_offset
    mov si, 6
    mov al, 7
    call draw_horizontal_line_4
    
    ; Lineas 4-5 - Cara
    mov bx, 4
    mov si, 4
    mov al, 7
    call draw_horizontal_line_6
    mov bx, 5
    mov si, 5
    mov al, 7
    call draw_horizontal_line_4
    
    ; Lineas 6-8 - Tunica
    mov bx, 6
    mov si, 3
    mov al, 2
    call draw_horizontal_line_8
    mov bx, 7
    mov si, 2
    mov al, 10
    call draw_pixel_at_offset
    mov si, 3
    mov al, 2
    call draw_horizontal_line_8
    mov bx, 8
    mov si, 3
    call draw_horizontal_line_8
    
    ; Linea 9 - Cinturon
    mov bx, 9
    mov si, 4
    mov al, 6
    call draw_horizontal_line_6
    
    ; Lineas 10-12 - Tunica inferior
    mov bx, 10
    mov si, 3
    mov al, 2
    call draw_horizontal_line_8
    mov bx, 11
    call draw_horizontal_line_8
    mov bx, 12
    mov si, 4
    call draw_horizontal_line_6
    
    ; Lineas 13-15 - Piernas
    mov bx, 13
    mov si, 5
    mov al, 12
    call draw_horizontal_line_4
    mov bx, 14
    mov si, 4
    mov al, 6
    call draw_pixel_at_offset
    mov si, 5
    mov al, 12
    call draw_horizontal_line_2
    mov si, 9
    mov al, 6
    call draw_pixel_at_offset
    mov bx, 15
    mov si, 4
    mov al, 6
    call draw_pixel_at_offset
    mov si, 9
    call draw_pixel_at_offset

draw_link_end:
    pop si
    pop bx
    pop ax
    ret
draw_hero_sprite endp

draw_horizontal_line_2 proc
    push si
    push di
    mov di, 2
hl2_loop:
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
    dec di
    jnz hl2_loop
    pop di
    pop si
    ret
draw_horizontal_line_2 endp

draw_horizontal_line_4 proc
    push si
    push di
    mov di, 4
hl4_loop:
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
    dec di
    jnz hl4_loop
    pop di
    pop si
    ret
draw_horizontal_line_4 endp

draw_horizontal_line_6 proc
    push si
    push di
    mov di, 6
hl6_loop:
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
    dec di
    jnz hl6_loop
    pop di
    pop si
    ret
draw_horizontal_line_6 endp

draw_horizontal_line_8 proc
    push si
    push di
    mov di, 8
hl8_loop:
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
    dec di
    jnz hl8_loop
    pop di
    pop si
    ret
draw_horizontal_line_8 endp

draw_horizontal_line_10 proc
    push si
    push di
    mov di, 10
hl10_loop:
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
    dec di
    jnz hl10_loop
    pop di
    pop si
    ret
draw_horizontal_line_10 endp

draw_horizontal_line_12 proc
    push si
    push di
    mov di, 12
hl12_loop:
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
    dec di
    jnz hl12_loop
    pop di
    pop si
    ret
draw_horizontal_line_12 endp

draw_enhanced_arms proc
    ret
draw_enhanced_arms endp

draw_enhanced_legs proc
    ret
draw_enhanced_legs endp

draw_sword proc
    ret
draw_sword endp

update_player_animation proc
    push ax
    inc byte ptr [animation_counter]
    cmp byte ptr [animation_counter], 10
    jl  no_frame_update
    mov byte ptr [animation_counter], 0
    inc byte ptr [player_frame]
    cmp byte ptr [player_frame], 4
    jl  no_frame_update
    mov byte ptr [player_frame], 0
no_frame_update:
    pop ax
    ret
update_player_animation endp
