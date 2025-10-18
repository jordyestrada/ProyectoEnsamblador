; ============================================
; SPRITES / TILES / PERSONAJE - sprites.inc
; Solo procedimientos (sin .data ni .code)
; ============================================

; ------------ Selector de tile ------------
draw_tile_enhanced proc
    push cx
    push dx
    push ax

    cmp al, '#'
    je  short tile_wall_enhanced
    cmp al, '~'
    je  short tile_water_enhanced
    cmp al, 'G'
    je  short tile_gold_enhanced
    cmp al, 'C'
    je  short tile_crystal_enhanced
    cmp al, 'T'
    je  short tile_treasure_enhanced
    cmp al, 'O'
    je  short tile_pit_enhanced
    cmp al, '+'
    je  short tile_path_enhanced
    cmp al, 'E'
    je  short tile_exit_enhanced
    cmp al, '.'
    je  short tile_floor_enhanced

    call draw_stone_floor
    jmp short tile_enhanced_end

tile_wall_enhanced:     call draw_stone_wall      ; muro
                        jmp  short tile_enhanced_end
tile_water_enhanced:    call draw_water_animated  ; agua
                        jmp  short tile_enhanced_end
tile_path_enhanced:     call draw_golden_path     ; camino
                        jmp  short tile_enhanced_end
tile_exit_enhanced:     call draw_magical_exit    ; salida
                        jmp  short tile_enhanced_end
tile_gold_enhanced:     call draw_gold_coin       ; oro
                        jmp  short tile_enhanced_end
tile_crystal_enhanced:  call draw_crystal_gem     ; cristal
                        jmp  short tile_enhanced_end
tile_treasure_enhanced: call draw_treasure_chest  ; cofre
                        jmp  short tile_enhanced_end
tile_pit_enhanced:      call draw_dark_pit        ; pozo
                        jmp  short tile_enhanced_end
tile_floor_enhanced:    call draw_stone_floor

tile_enhanced_end:
    pop ax
    pop dx
    pop cx
    ret
draw_tile_enhanced endp

; ------------ Tiles individuales ------------
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
    mov al, 1
    call fill_tile_base
    mov bx, 3
    mov si, 4
    mov al, 9
    call draw_wave_pattern
    mov bx, 8
    mov si, 12
    mov al, 11
    call draw_wave_pattern
    mov bx, 13
    mov si, 6
    mov al, 9
    call draw_wave_pattern
    mov bx, 6
    mov si, 15
    mov al, 15
    call draw_pixel_at_offset
    mov bx, 11
    mov si, 8
    mov al, 15
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
    mov al, 14
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
    jz  path_border
    test si, 3
    jz  path_border
    mov al, 6
    jmp short path_draw
path_border:
    mov al, 15
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
    mov al, 7
    call fill_tile_base
    mov bx, 8
    mov si, 10
    mov al, 11
    call draw_diamond_shape
    mov bx, 6
    mov si, 8
    mov al, 15
    call draw_pixel_at_offset
    mov bx, 7
    mov si, 9
    mov al, 15
    call draw_pixel_at_offset
    mov bx, 12
    mov si, 14
    mov al, 8
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

draw_dark_pit proc
    push ax
    push bx
    push si
    mov al, 0
    call fill_tile_base
    mov al, 8
    call draw_full_border
    mov bx, 3
    mov si, 3
    mov al, 8
    call draw_pixel_at_offset
    mov bx, 5
    mov si, 7
    mov al, 8
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

; ------------ Helpers ------------
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

; ------------ Personaje ------------
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

draw_hero_sprite proc
    push ax
    push bx
    push si

    ; Cabeza
    mov bx, 2
    mov si, 6
head_enh_y:
    push si
    mov si, 6
head_enh_x:
    push ax
    push cx
    push dx
    mov ah, 0Ch
    mov al, 14
    mov bh, 0
    add cx, si
    add dx, bx
    int 10h
    pop dx
    pop cx
    pop ax
    inc si
    cmp si, 14
    jl  head_enh_x
    pop si
    inc bx
    cmp bx, 8
    jl  head_enh_y

    ; Brillos casco
    mov bx, 3
    mov si, 8
    mov al, 15
    call draw_pixel_at_offset
    mov bx, 3
    mov si, 12
    mov al, 15
    call draw_pixel_at_offset

    ; Cuerpo (armadura)
    mov bx, 8
body_enh_y:
    mov si, 7
body_enh_x:
    push ax
    push cx
    push dx
    mov ah, 0Ch
    mov al, 9
    mov bh, 0
    add cx, si
    add dx, bx
    int 10h
    pop dx
    pop cx
    pop ax
    inc si
    cmp si, 13
    jl  body_enh_x
    inc bx
    cmp bx, 14
    jl  body_enh_y

    ; Detalle metálico
    mov bx, 10
    mov si, 10
    mov al, 11
    call draw_pixel_at_offset

    ; Brazos y piernas
    call draw_enhanced_arms
    call draw_enhanced_legs

    ; Ojos
    mov bx, 5
    mov si, 8
    mov al, 15
    call draw_pixel_at_offset
    mov bx, 5
    mov si, 12
    mov al, 15
    call draw_pixel_at_offset

    ; Espada
    call draw_sword

    pop si
    pop bx
    pop ax
    ret
draw_hero_sprite endp

draw_enhanced_arms proc
    push ax
    push bx
    push si
    mov al, byte ptr [player_direction]
    cmp al, 2
    je  short arms_left_enh
    cmp al, 3
    je  short arms_right_enh
arms_normal_enh:
    mov bx, 10
    mov si, 4
    mov al, 14
    call draw_enhanced_arm_part
    mov si, 16
    mov al, 14
    call draw_enhanced_arm_part
    jmp short arms_end_enh
arms_left_enh:
    mov bx, 10
    mov si, 2
    mov al, 14
    call draw_extended_arm
    jmp short arms_end_enh
arms_right_enh:
    mov bx, 10
    mov si, 18
    mov al, 14
    call draw_extended_arm
arms_end_enh:
    pop si
    pop bx
    pop ax
    ret
draw_enhanced_arms endp

draw_enhanced_arm_part proc
    call draw_pixel_at_offset
    inc si
    call draw_pixel_at_offset
    inc bx
    call draw_pixel_at_offset
    dec si
    call draw_pixel_at_offset
    ret
draw_enhanced_arm_part endp

draw_extended_arm proc
    call draw_pixel_at_offset
    inc si
    call draw_pixel_at_offset
    inc si
    call draw_pixel_at_offset
    ret
draw_extended_arm endp

draw_enhanced_legs proc
    push ax
    push bx
    push si
    mov al, byte ptr [player_frame]
    cmp al, 1
    je  short legs_frame1_enh
    cmp al, 3
    je  short legs_frame3_enh
legs_normal_enh:
    mov bx, 14
legs_normal_y_enh:
    mov si, 8
    mov al, 6
    call draw_enhanced_leg_part
    mov si, 12
    mov al, 6
    call draw_enhanced_leg_part
    inc bx
    cmp bx, 18
    jl  legs_normal_y_enh
    mov bx, 18
    mov si, 7
    mov al, 0
    call draw_boot
    mov si, 11
    call draw_boot
    jmp short legs_end_enh
legs_frame1_enh:
    mov bx, 14
    mov si, 6
    mov al, 6
    call draw_enhanced_leg_part
    mov si, 14
    mov al, 6
    call draw_enhanced_leg_part
    jmp short legs_end_enh
legs_frame3_enh:
    mov bx, 14
    mov si, 10
    mov al, 6
    call draw_enhanced_leg_part
    mov si, 8
    mov al, 6
    call draw_enhanced_leg_part
legs_end_enh:
    pop si
    pop bx
    pop ax
    ret
draw_enhanced_legs endp

draw_enhanced_leg_part proc
    call draw_pixel_at_offset
    inc si
    call draw_pixel_at_offset
    inc bx
    call draw_pixel_at_offset
    dec si
    call draw_pixel_at_offset
    ret
draw_enhanced_leg_part endp

draw_boot proc
    call draw_pixel_at_offset
    inc si
    call draw_pixel_at_offset
    inc si
    call draw_pixel_at_offset
    ret
draw_boot endp

draw_sword proc
    push ax
    push bx
    push si
    mov al, byte ptr [player_direction]
    cmp al, 3
    je  short sword_right
    ; izquierda
    mov bx, 6
    mov si, 2
    mov al, 8
    call draw_pixel_at_offset
    inc bx
    call draw_pixel_at_offset
    inc bx
    mov al, 15
    call draw_pixel_at_offset
    inc bx
    call draw_pixel_at_offset
    jmp short sword_end
sword_right:
    mov bx, 6
    mov si, 18
    mov al, 8
    call draw_pixel_at_offset
    inc bx
    call draw_pixel_at_offset
    inc bx
    mov al, 15
    call draw_pixel_at_offset
    inc bx
    call draw_pixel_at_offset
sword_end:
    pop si
    pop bx
    pop ax
    ret
draw_sword endp

; ------------ Animación ------------
update_player_animation proc
    push ax
    inc byte ptr [animation_counter]
    cmp byte ptr [animation_counter], 10   ; Cambiado de 6 a 10 (más lento = más suave)
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

; ------------ Letras HUD ------------
; draw_text_pixel se define en juego.asm
draw_char_fancy proc
    push ax
    push bx
    push si
    mov bx, 0
char_fancy_y:
    mov si, 0
char_fancy_x:
    push ax
    push cx
    push dx
    mov ah, 0Ch
    mov al, 15
    mov bh, 0
    add cx, si
    add dx, bx
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
    mov bx, 2
char_large_y:
    mov si, 2
char_large_x:
    push ax
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
