; ============================================
; ARCHIVO DE SPRITES - personajes.spr
; Contiene todos los procedimientos de dibujo
; de sprites, tiles y animaciones
; ============================================

; Variables de animación del personaje
player_direction db 0    ; 0=abajo, 1=arriba, 2=izquierda, 3=derecha
player_frame db 0        ; Frame actual de animación (0-3)
animation_counter db 0   ; Contador para velocidad de animación

; Variables temporales para evitar problemas de stack
temp_y dw 0
temp_x dw 0

; ============================================
; PROCEDIMIENTOS DE DIBUJO DE TILES
; ============================================

draw_tile_enhanced proc
    ; AL = tipo de tile, CX,DX = posición
    push cx
    push dx
    push ax
    
    ; Dibujar tiles con diseños detallados
    cmp al, '#'
    je tile_wall_enhanced
    cmp al, '~'
    je tile_water_enhanced
    cmp al, 'G'
    je tile_gold_enhanced
    cmp al, 'C'
    je tile_crystal_enhanced
    cmp al, 'T'
    je tile_treasure_enhanced
    cmp al, 'O'
    je tile_pit_enhanced
    cmp al, '+'    
    je tile_path_enhanced
    cmp al, 'E'
    je tile_exit_enhanced
    cmp al, '.'    
    je tile_floor_enhanced
    
    ; Default - suelo de piedra con textura
    call draw_stone_floor
    jmp tile_enhanced_end

tile_wall_enhanced:
    call draw_stone_wall
    jmp tile_enhanced_end

tile_water_enhanced:
    call draw_water_animated
    jmp tile_enhanced_end

tile_path_enhanced:
    call draw_golden_path
    jmp tile_enhanced_end

tile_exit_enhanced:
    call draw_magical_exit
    jmp tile_enhanced_end

tile_gold_enhanced:
    call draw_gold_coin
    jmp tile_enhanced_end

tile_crystal_enhanced:
    call draw_crystal_gem
    jmp tile_enhanced_end

tile_treasure_enhanced:
    call draw_treasure_chest
    jmp tile_enhanced_end

tile_pit_enhanced:
    call draw_dark_pit
    jmp tile_enhanced_end

tile_floor_enhanced:
    call draw_stone_floor

tile_enhanced_end:
    pop ax
    pop dx
    pop cx
    ret
draw_tile_enhanced endp

; ============================================
; TILES INDIVIDUALES
; ============================================

draw_stone_wall proc
    push ax
    push bx
    push si
    
    ; Base gris
    mov al, 8           ; Gris medio
    call fill_tile_base
    
    ; Textura de piedras
    mov bx, 2
    mov si, 2
    mov al, 15          ; Blanco para highlights
    call draw_pixel_at_offset
    
    mov bx, 5
    mov si, 8
    mov al, 15
    call draw_pixel_at_offset
    
    mov bx, 8
    mov si, 3
    mov al, 15
    call draw_pixel_at_offset
    
    ; Sombras
    mov bx, 4
    mov si, 6
    mov al, 0           ; Negro para sombras
    call draw_pixel_at_offset
    
    mov bx, 12
    mov si, 10
    mov al, 0
    call draw_pixel_at_offset
    
    ; Bordes definidos
    mov al, 7           ; Gris claro para borde superior
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
    
    ; Base azul
    mov al, 1           ; Azul base
    call fill_tile_base
    
    ; Ondas animadas
    mov bx, 3
    mov si, 4
    mov al, 9           ; Azul claro
    call draw_wave_pattern
    
    mov bx, 8
    mov si, 12
    mov al, 11          ; Cyan brillante
    call draw_wave_pattern
    
    mov bx, 13
    mov si, 6
    mov al, 9
    call draw_wave_pattern
    
    ; Reflejos
    mov bx, 6
    mov si, 15
    mov al, 15          ; Blanco para reflejos
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
    
    ; Base amarilla
    mov al, 14          ; Amarillo brillante
    call fill_tile_base
    
    ; Patrón de adoquines
    mov bx, 0
path_y:
    mov si, 0
path_x:
    push ax
    push cx
    push dx
    ; Alternar colores para efecto adoquín
    mov ah, 0Ch
    mov bh, 0
    ; Calcular si es borde de adoquín
    test bx, 3
    jz path_border
    test si, 3
    jz path_border
    mov al, 6           ; Marrón para sombra
    jmp path_draw
path_border:
    mov al, 15          ; Blanco para resalte
path_draw:
    add cx, si
    add dx, bx
    int 10h
    pop dx
    pop cx
    pop ax
    inc si
    cmp si, TILE_SIZE
    jl path_x
    inc bx
    cmp bx, TILE_SIZE
    jl path_y
    
    pop si
    pop bx
    pop ax
    ret
draw_golden_path endp

draw_magical_exit proc
    push ax
    push bx
    push si
    
    ; Base magenta brillante
    mov al, 13          ; Magenta brillante
    call fill_tile_base
    
    ; Cruz brillante en el centro
    mov bx, 8
    mov si, 4
exit_horizontal:
    push cx
    push dx
    mov ah, 0Ch
    mov al, 15          ; Blanco brillante
    mov bh, 0
    add cx, si
    add dx, bx
    int 10h
    pop dx
    pop cx
    inc si
    cmp si, 16
    jl exit_horizontal
    
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
    jl exit_vertical
    
    ; Efectos mágicos en las esquinas
    mov bx, 2
    mov si, 2
    mov al, 14          ; Amarillo para chispas
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
    
    ; Base del suelo
    mov al, 7           ; Gris claro
    call fill_tile_base
    
    ; Moneda dorada en el centro
    mov word ptr [temp_y], 6
    
coin_y_loop:
    mov word ptr [temp_x], 6
coin_x_loop:
    push cx
    push dx
    mov ah, 0Ch
    mov al, 14          ; Amarillo brillante
    mov bh, 0
    add cx, word ptr [temp_x]
    add dx, word ptr [temp_y]
    int 10h
    pop dx
    pop cx
    inc word ptr [temp_x]
    cmp word ptr [temp_x], 14
    jl coin_x_loop
    inc word ptr [temp_y]
    cmp word ptr [temp_y], 14
    jl coin_y_loop
    
    ; Brillo en la moneda
    mov bx, 8
    mov si, 8
    mov al, 15          ; Blanco para brillo
    call draw_pixel_at_offset
    
    mov bx, 9
    mov si, 9
    mov al, 15
    call draw_pixel_at_offset
    
    ; Sombra de la moneda
    mov bx, 14
    mov si, 14
    mov al, 6           ; Marrón oscuro para sombra
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
    
    ; Base del suelo
    mov al, 7           ; Gris claro
    call fill_tile_base
    
    ; Cristal en forma de diamante
    mov bx, 8
    mov si, 10
    mov al, 11          ; Cyan brillante
    call draw_diamond_shape
    
    ; Reflejos del cristal
    mov bx, 6
    mov si, 8
    mov al, 15          ; Blanco para reflejos
    call draw_pixel_at_offset
    
    mov bx, 7
    mov si, 9
    mov al, 15
    call draw_pixel_at_offset
    
    ; Sombra del cristal
    mov bx, 12
    mov si, 14
    mov al, 8           ; Gris oscuro
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
    
    ; Base del suelo
    mov al, 7           ; Gris claro
    call fill_tile_base
    
    ; Cofre en el centro
    mov word ptr [temp_y], 7
    
chest_y_loop:
    mov word ptr [temp_x], 6
chest_x_loop:
    push cx
    push dx
    mov ah, 0Ch
    mov al, 6           ; Marrón para madera
    mov bh, 0
    add cx, word ptr [temp_x]
    add dx, word ptr [temp_y]
    int 10h
    pop dx
    pop cx
    inc word ptr [temp_x]
    cmp word ptr [temp_x], 14
    jl chest_x_loop
    inc word ptr [temp_y]
    cmp word ptr [temp_y], 13
    jl chest_y_loop
    
    ; Cerradura dorada
    mov bx, 10
    mov si, 10
    mov al, 14          ; Amarillo para cerradura
    call draw_pixel_at_offset
    
    mov bx, 10
    mov si, 11
    mov al, 14
    call draw_pixel_at_offset
    
    ; Herrajes metálicos
    mov bx, 8
    mov si, 7
    mov al, 8           ; Gris para metal
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
    
    ; Base negra
    mov al, 0           ; Negro
    call fill_tile_base
    
    ; Borde del pozo
    mov al, 8           ; Gris oscuro para borde
    call draw_full_border
    
    ; Efectos de profundidad
    mov bx, 3
    mov si, 3
    mov al, 8           ; Gris oscuro
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
    
    ; Base gris claro
    mov al, 7           ; Gris claro
    call fill_tile_base
    
    ; Textura de piedra
    mov bx, 3
    mov si, 5
    mov al, 15          ; Blanco para highlights
    call draw_pixel_at_offset
    
    mov bx, 8
    mov si, 12
    mov al, 15
    call draw_pixel_at_offset
    
    ; Pequeñas sombras para textura
    mov bx, 6
    mov si, 9
    mov al, 8           ; Gris oscuro
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

; ============================================
; FUNCIONES AUXILIARES PARA DIBUJO DE TILES
; ============================================

fill_tile_base proc
    ; AL = color base
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
    jl base_x
    inc bx
    cmp bx, TILE_SIZE
    jl base_y
    
    pop si
    pop bx
    ret
fill_tile_base endp

draw_pixel_at_offset proc
    ; AL = color, BX = offset Y, SI = offset X
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
    ; AL = color del borde superior
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
    jl border_top_loop
    
    pop si
    pop bx
    ret
draw_top_border endp

draw_full_border proc
    ; AL = color del borde
    push bx
    push si
    
    ; Borde superior e inferior
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
    jl border_horizontal
    
    ; Bordes izquierdo y derecho
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
    jl border_vertical
    
    pop si
    pop bx
    ret
draw_full_border endp

draw_wave_pattern proc
    ; Dibujar patrón de onda
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
    ; Dibujar forma de diamante
    push cx
    push dx
    mov ah, 0Ch
    mov bh, 0
    ; Punto superior
    add cx, si
    add dx, bx
    sub dx, 2
    int 10h
    ; Lados
    dec cx
    inc dx
    int 10h
    add cx, 2
    int 10h
    dec cx
    inc dx
    int 10h
    ; Punto inferior
    inc dx
    int 10h
    pop dx
    pop cx
    ret
draw_diamond_shape endp

; ============================================
; DIBUJO DEL PERSONAJE
; ============================================

draw_player_enhanced proc
    push ax
    push bx
    push cx
    push dx
    push si
    
    ; Calcular posición del jugador en viewport
    mov al, byte ptr [px]
    sub al, byte ptr [camera_x]
    mov ah, 0
    mov dx, TILE_SIZE
    mul dx
    add ax, 40          ; Offset
    mov cx, ax
    
    mov al, byte ptr [py]
    sub al, byte ptr [camera_y]
    mov ah, 0
    mov dx, TILE_SIZE
    mul dx
    add ax, 50          ; Offset para HUD
    mov dx, ax
    
    ; Solo dibujar si está visible en viewport
    cmp cx, 600
    jg player_skip_enh
    cmp dx, 320
    jg player_skip_enh
    
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
    
    ; Dibujar héroe detallado con animación
    
    ; Cabeza con casco
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
    mov al, 14          ; Amarillo para piel/casco
    mov bh, 0
    add cx, si
    add dx, bx
    int 10h
    pop dx
    pop cx
    pop ax
    inc si
    cmp si, 14
    jl head_enh_x
    pop si
    inc bx
    cmp bx, 8
    jl head_enh_y
    
    ; Detalles del casco
    mov bx, 3
    mov si, 8
    mov al, 15          ; Blanco para brillo del casco
    call draw_pixel_at_offset
    
    mov bx, 3
    mov si, 12
    mov al, 15
    call draw_pixel_at_offset
    
    ; Cuerpo con armadura
    mov bx, 8
body_enh_y:
    mov si, 7
body_enh_x:
    push ax
    push cx
    push dx
    mov ah, 0Ch
    mov al, 9           ; Azul para armadura
    mov bh, 0
    add cx, si
    add dx, bx
    int 10h
    pop dx
    pop cx
    pop ax
    inc si
    cmp si, 13
    jl body_enh_x
    inc bx
    cmp bx, 14
    jl body_enh_y
    
    ; Detalles de la armadura
    mov bx, 10
    mov si, 10
    mov al, 11          ; Cyan para detalles metálicos
    call draw_pixel_at_offset
    
    ; Brazos con gauntlets
    call draw_enhanced_arms
    
    ; Piernas animadas con botas
    call draw_enhanced_legs
    
    ; Ojos brillantes
    mov bx, 5
    mov si, 8
    mov al, 15          ; Blanco brillante
    call draw_pixel_at_offset
    
    mov bx, 5
    mov si, 12
    mov al, 15
    call draw_pixel_at_offset
    
    ; Arma (espada)
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
    
    ; Brazos según dirección
    mov al, byte ptr [player_direction]
    
    cmp al, 2           ; Izquierda
    je arms_left_enh
    cmp al, 3           ; Derecha  
    je arms_right_enh
    
    ; Brazos normales
arms_normal_enh:
    mov bx, 10
    mov si, 4
    mov al, 14          ; Piel
    call draw_enhanced_arm_part
    
    mov si, 16
    mov al, 14
    call draw_enhanced_arm_part
    jmp arms_end_enh
    
arms_left_enh:
    ; Brazo izquierdo extendido
    mov bx, 10
    mov si, 2
    mov al, 14
    call draw_extended_arm
    jmp arms_end_enh
    
arms_right_enh:
    ; Brazo derecho extendido
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
    ; Dibujar parte del brazo mejorada
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
    ; Dibujar brazo extendido
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
    
    ; Piernas con animación mejorada
    mov al, byte ptr [player_frame]
    
    cmp al, 1
    je legs_frame1_enh
    cmp al, 3
    je legs_frame3_enh
    
    ; Frame neutral con botas
legs_normal_enh:
    mov bx, 14
legs_normal_y_enh:
    mov si, 8
    mov al, 6           ; Marrón para pantalones
    call draw_enhanced_leg_part
    
    mov si, 12
    mov al, 6
    call draw_enhanced_leg_part
    
    inc bx
    cmp bx, 18
    jl legs_normal_y_enh
    
    ; Botas
    mov bx, 18
    mov si, 7
    mov al, 0           ; Negro para botas
    call draw_boot
    mov si, 11
    call draw_boot
    jmp legs_end_enh
    
legs_frame1_enh:
    ; Pierna izquierda adelante
    mov bx, 14
    mov si, 6           ; Pierna izq. adelante
    mov al, 6
    call draw_enhanced_leg_part
    
    mov si, 14          ; Pierna der. atrás
    mov al, 6
    call draw_enhanced_leg_part
    jmp legs_end_enh
    
legs_frame3_enh:
    ; Pierna derecha adelante
    mov bx, 14
    mov si, 10          ; Pierna izq. atrás
    mov al, 6
    call draw_enhanced_leg_part
    
    mov si, 8           ; Pierna der. adelante
    mov al, 6
    call draw_enhanced_leg_part

legs_end_enh:
    pop si
    pop bx
    pop ax
    ret
draw_enhanced_legs endp

draw_enhanced_leg_part proc
    ; Dibujar parte de pierna mejorada
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
    ; Dibujar bota
    call draw_pixel_at_offset
    inc si
    call draw_pixel_at_offset
    inc si
    call draw_pixel_at_offset
    ret
draw_boot endp

draw_sword proc
    ; Dibujar espada según dirección
    push ax
    push bx
    push si
    
    mov al, byte ptr [player_direction]
    cmp al, 3           ; Derecha
    je sword_right
    
    ; Espada a la izquierda
    mov bx, 6
    mov si, 2
    mov al, 8           ; Gris para empuñadura
    call draw_pixel_at_offset
    inc bx
    call draw_pixel_at_offset
    inc bx
    mov al, 15          ; Blanco para hoja
    call draw_pixel_at_offset
    inc bx
    call draw_pixel_at_offset
    jmp sword_end
    
sword_right:
    ; Espada a la derecha
    mov bx, 6
    mov si, 18
    mov al, 8           ; Gris para empuñadura
    call draw_pixel_at_offset
    inc bx
    call draw_pixel_at_offset
    inc bx
    mov al, 15          ; Blanco para hoja
    call draw_pixel_at_offset
    inc bx
    call draw_pixel_at_offset

sword_end:
    pop si
    pop bx
    pop ax
    ret
draw_sword endp

; ============================================
; ANIMACIÓN DEL PERSONAJE
; ============================================

update_player_animation proc
    push ax
    
    ; Actualizar contador de animación
    inc byte ptr [animation_counter]
    cmp byte ptr [animation_counter], 6  ; Animación más rápida
    jl no_frame_update
    
    ; Reiniciar contador y avanzar frame
    mov byte ptr [animation_counter], 0
    inc byte ptr [player_frame]
    cmp byte ptr [player_frame], 4  ; 4 frames de animación
    jl no_frame_update
    mov byte ptr [player_frame], 0
    
no_frame_update:
    pop ax
    ret
update_player_animation endp
