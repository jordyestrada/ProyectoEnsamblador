.model small
.stack 100h

.data
    ; Mapa expandido a 20x15 tiles (requisito mínimo)
    mapa db '####################'
         db '#.GTCGT..#.GTCGT..#'
         db '#..++~+C.#..++~+C.#'
         db '#.T++~++T#.T++~++T#'
         db '#G.++O+.G#G.++O+.G#'
         db '#..T++TC.#..T++TC.#'
         db '#..++..E.#..++....#'
         db '####################'
         db '#GTCGT..##..GTCGT.#'
         db '#.++~+C.##.++~+C..#'
         db '#T++~++T##T++~++T.#'
         db '#.++O+.G##G.++O+..#'
         db '#.T++TC.##..T++TC.#'
         db '#.++....##..++..E.#'
         db '####################'
    
    px db 2
    py db 1
    tecla db 0
    fin db 0
    redraw db 1
    
    ; Viewport y cámara
    camera_x db 0
    camera_y db 0
    VIEWPORT_WIDTH equ 160
    VIEWPORT_HEIGHT equ 100
    VIEWPORT_TILES_X equ 10
    VIEWPORT_TILES_Y equ 6
    VIEWPORT_TILES_X_HALF equ 5
    VIEWPORT_TILES_Y_HALF equ 3
    
    ; Sistema de inventario
    gold_count db 0
    crystals_count db 0
    treasures_count db 0
    
    ; Objetivos
    gold_needed db 2
    crystals_needed db 2
    treasures_needed db 2
    
    MAPA_ANCHO equ 20
    MAPA_ALTO equ 15
    TILE_SIZE equ 20  ; Aumentado para mejor visibilidad
    
    ; Sistema de animación del personaje
    player_direction db 0    ; 0=abajo, 1=arriba, 2=izquierda, 3=derecha
    player_frame db 0        ; Frame actual de animación (0-3)
    animation_counter db 0   ; Contador para velocidad de animación
    
    ; Variables temporales para evitar problemas de stack
    temp_y dw 0
    temp_x dw 0
    
    ; Mensajes informativos
    mensaje_titulo db 'ADVENTURE QUEST - Recolecta tesoros y escapa!$'
    mensaje_controles db 'Controles: WASD para mover, ESC para salir$'
    mensaje_objetivo db 'Objetivo: Recolecta 2 ORO, 2 CRISTALES, 2 TESOROS$'
    mensaje_estado db 'Oro: 0/2  Cristales: 0/2  Tesoros: 0/2$'

.code
start:
    mov ax, @data
    mov ds, ax
    
    call show_intro
    call init_ega_mode
    
game_loop:
    cmp byte ptr [redraw], 1
    jne skip_draw
    
    call update_camera
    call clear_and_draw
    mov byte ptr [redraw], 0

skip_draw:
    call get_key_nowait
    cmp byte ptr [tecla], 0
    je game_loop
    
    call move_player
    call update_player_animation
    call check_item_collection
    call check_win
    
    cmp byte ptr [fin], 1
    jne game_loop
    
    call restore_screen
    mov ax, 4c00h
    int 21h

show_intro proc
    ; Mostrar pantalla de introducción
    mov ax, 0003h       ; Modo texto
    int 10h
    
    ; Posicionar cursor y mostrar título
    mov ah, 02h
    mov bh, 0
    mov dh, 5           ; Fila 5
    mov dl, 8           ; Columna 8
    int 10h
    
    mov ah, 09h
    mov dx, offset mensaje_titulo
    int 21h
    
    ; Mostrar controles
    mov ah, 02h
    mov dh, 8
    mov dl, 12
    int 10h
    
    mov ah, 09h
    mov dx, offset mensaje_controles
    int 21h
    
    ; Mostrar objetivo
    mov ah, 02h
    mov dh, 10
    mov dl, 8
    int 10h
    
    mov ah, 09h
    mov dx, offset mensaje_objetivo
    int 21h
    
    ; Información sobre modo EGA
    mov ah, 02h
    mov dh, 13
    mov dl, 18
    int 10h
    
    mov ah, 09h
    mov dx, offset mensaje_ega
    int 21h
    
    ; Esperar tecla
    mov ah, 02h
    mov dh, 16
    mov dl, 20
    int 10h
    
    mov ah, 09h
    mov dx, offset mensaje_presiona
    int 21h
    
    mov ah, 07h         ; Esperar tecla sin eco
    int 21h
    
    ret

mensaje_presiona db 'Presiona cualquier tecla para comenzar...$'
mensaje_ega db 'Modo EGA 640x350, 16 colores$'
show_intro endp

init_ega_mode proc
    ; Activar modo gráfico EGA 640x350, 16 colores
    mov ax, 0010h       ; Modo EGA 10h (640x350, 16 colores)
    int 10h
    ret
init_ega_mode endp

update_camera proc
    push ax
    push bx
    
    ; Centrar cámara en el jugador
    mov al, byte ptr [px]
    cmp al, VIEWPORT_TILES_X_HALF
    jl camera_x_min
    sub al, VIEWPORT_TILES_X_HALF
    jmp camera_x_check_max
camera_x_min:
    mov al, 0
camera_x_check_max:
    mov bl, MAPA_ANCHO - VIEWPORT_TILES_X
    cmp al, bl
    jle camera_x_valid
    mov al, bl
camera_x_valid:
    mov byte ptr [camera_x], al
    
    ; Centrar cámara en Y
    mov al, byte ptr [py]
    cmp al, VIEWPORT_TILES_Y_HALF
    jl camera_y_min
    sub al, VIEWPORT_TILES_Y_HALF
    jmp camera_y_check_max
camera_y_min:
    mov al, 0
camera_y_check_max:
    mov bl, MAPA_ALTO - VIEWPORT_TILES_Y
    cmp al, bl
    jle camera_y_valid
    mov al, bl
camera_y_valid:
    mov byte ptr [camera_y], al
    
    pop bx
    pop ax
    ret
update_camera endp

clear_and_draw proc
    call clear_screen_ega
    call draw_hud_enhanced
    call draw_viewport_enhanced
    call draw_player_enhanced
    call draw_status_enhanced
    ret
clear_and_draw endp

clear_screen_ega proc
    push ax
    push cx
    push dx
    
    ; Limpiar pantalla EGA completamente
    mov dx, 0
clear_y_loop:
    mov cx, 0
clear_x_loop:
    mov ah, 0Ch
    mov al, 0           ; Negro
    mov bh, 0
    int 10h
    inc cx
    cmp cx, 640
    jl clear_x_loop
    inc dx
    cmp dx, 350
    jl clear_y_loop
    
    pop dx
    pop cx
    pop ax
    ret
clear_screen_ega endp

draw_hud_enhanced proc
    push ax
    push bx
    push cx
    push dx
    
    ; Dibujar barra del HUD con gradiente
    mov dx, 0
hud_y_loop:
    mov cx, 0
hud_x_loop:
    mov ah, 0Ch
    ; Gradiente del HUD
    cmp dx, 8
    jl hud_dark
    cmp dx, 16
    jl hud_medium
    mov al, 3           ; Cyan claro para borde
    jmp hud_draw
hud_medium:
    mov al, 1           ; Azul medio
    jmp hud_draw
hud_dark:
    mov al, 8           ; Gris oscuro
hud_draw:
    mov bh, 0
    int 10h
    inc cx
    cmp cx, 640
    jl hud_x_loop
    inc dx
    cmp dx, 25
    jl hud_y_loop
    
    ; Texto del HUD con estilo
    call draw_hud_text
    
    pop dx
    pop cx
    pop bx
    pop ax
    ret
draw_hud_enhanced endp

draw_hud_text proc
    push ax
    push bx
    push cx
    push dx
    
    ; Dibujar etiquetas y contadores estilizados
    
    ; ORO
    mov cx, 30
    mov dx, 8
    mov al, 14          ; Amarillo brillante
    call draw_text_pixel
    mov al, 'O'
    call draw_char_fancy
    mov al, 'R'
    add cx, 12
    call draw_char_fancy
    mov al, 'O'
    add cx, 12
    call draw_char_fancy
    mov al, ':'
    add cx, 12
    call draw_char_fancy
    
    ; Contador de oro
    add cx, 15
    mov al, byte ptr [gold_count]
    add al, 30h
    call draw_char_large
    mov al, '/'
    add cx, 15
    call draw_char_large
    mov al, byte ptr [gold_needed]
    add al, 30h
    add cx, 15
    call draw_char_large
    
    ; CRISTALES
    mov cx, 200
    mov dx, 8
    mov al, 11          ; Cyan brillante
    call draw_text_pixel
    mov al, 'C'
    call draw_char_fancy
    mov al, 'R'
    add cx, 12
    call draw_char_fancy
    mov al, 'I'
    add cx, 12
    call draw_char_fancy
    mov al, 'S'
    add cx, 12
    call draw_char_fancy
    mov al, ':'
    add cx, 12
    call draw_char_fancy
    
    ; Contador de cristales
    add cx, 15
    mov al, byte ptr [crystals_count]
    add al, 30h
    call draw_char_large
    mov al, '/'
    add cx, 15
    call draw_char_large
    mov al, byte ptr [crystals_needed]
    add al, 30h
    add cx, 15
    call draw_char_large
    
    ; TESOROS
    mov cx, 400
    mov dx, 8
    mov al, 12          ; Rojo brillante
    call draw_text_pixel
    mov al, 'T'
    call draw_char_fancy
    mov al, 'E'
    add cx, 12
    call draw_char_fancy
    mov al, 'S'
    add cx, 12
    call draw_char_fancy
    mov al, ':'
    add cx, 12
    call draw_char_fancy
    
    ; Contador de tesoros
    add cx, 15
    mov al, byte ptr [treasures_count]
    add al, 30h
    call draw_char_large
    mov al, '/'
    add cx, 15
    call draw_char_large
    mov al, byte ptr [treasures_needed]
    add al, 30h
    add cx, 15
    call draw_char_large
    
    pop dx
    pop cx
    pop bx
    pop ax
    ret
draw_hud_text endp

draw_text_pixel proc
    ; AL = color, establecer color para texto
    ret
draw_text_pixel endp

draw_char_fancy proc
    ; AL = carácter, CX,DX = posición
    push ax
    push bx
    push si
    
    ; Dibujar carácter con borde
    mov bx, 0
char_fancy_y:
    mov si, 0
char_fancy_x:
    push ax
    push cx
    push dx
    mov ah, 0Ch
    mov al, 15          ; Blanco para carácter
    mov bh, 0
    add cx, si
    add dx, bx
    ; Solo dibujar en el patrón del carácter
    cmp bx, 1
    je char_fancy_draw
    cmp bx, 6
    je char_fancy_draw
    cmp si, 1
    je char_fancy_draw
    cmp si, 9
    je char_fancy_draw
    jmp char_fancy_skip
char_fancy_draw:
    int 10h
char_fancy_skip:
    pop dx
    pop cx
    pop ax
    inc si
    cmp si, 10
    jl char_fancy_x
    inc bx
    cmp bx, 8
    jl char_fancy_y
    
    pop si
    pop bx
    pop ax
    ret
draw_char_fancy endp

draw_char_large proc
    ; AL = carácter, CX,DX = posición
    push ax
    push bx
    push si
    
    ; Dibujar número grande
    mov bx, 2
char_large_y:
    mov si, 2
char_large_x:
    push ax
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
    pop ax
    inc si
    cmp si, 12
    jl char_large_x
    inc bx
    cmp bx, 12
    jl char_large_y
    
    pop si
    pop bx
    pop ax
    ret
draw_char_large endp

draw_viewport_enhanced proc
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    
    mov si, 0           ; Contador Y del viewport

viewport_y_loop_enh:
    mov di, 0           ; Contador X del viewport

viewport_x_loop_enh:
    ; Calcular posición Y en el mapa
    mov al, byte ptr [camera_y]
    mov ah, 0
    add ax, si          ; camera_y + viewport_y
    
    ; Calcular posición X en el mapa  
    mov bl, byte ptr [camera_x]
    mov bh, 0
    add bx, di          ; camera_x + viewport_x
    
    ; Calcular índice del mapa: Y * MAPA_ANCHO + X
    push bx             ; Guardar X
    mov dx, MAPA_ANCHO
    mul dx              ; AX = Y * MAPA_ANCHO
    pop bx              ; Recuperar X
    add ax, bx          ; AX = Y * MAPA_ANCHO + X
    mov bx, ax          ; BX = índice del mapa
    
    ; Verificar límites del mapa
    cmp bx, 300         ; 20*15 = 300 tiles máximo
    jge skip_current_tile_enh
    
    ; Obtener tile del mapa
    mov al, byte ptr [mapa + bx]
    
    ; Calcular posición en pantalla
    push ax             ; Guardar tipo de tile
    
    ; X en pantalla = di * TILE_SIZE + offset
    mov ax, di
    mov dx, TILE_SIZE
    mul dx
    add ax, 40          ; Offset desde el borde
    mov cx, ax          ; CX = X en pantalla
    
    ; Y en pantalla = si * TILE_SIZE + offset HUD
    mov ax, si
    mov dx, TILE_SIZE
    mul dx
    add ax, 50          ; Offset para HUD
    mov dx, ax          ; DX = Y en pantalla
    
    ; Verificar si está dentro del viewport visible
    cmp cx, 600         ; Límite derecho del viewport
    jge skip_tile_draw_enh
    cmp dx, 320         ; Límite inferior del viewport
    jge skip_tile_draw_enh
    
    pop ax              ; Recuperar tipo de tile
    call draw_tile_enhanced
    jmp continue_loop_enh

skip_tile_draw_enh:
    pop ax              ; Limpiar stack
    jmp continue_loop_enh

skip_current_tile_enh:
    ; No hay nada que limpiar del stack aquí

continue_loop_enh:
    inc di
    cmp di, VIEWPORT_TILES_X
    jl viewport_x_loop_enh
    
    inc si
    cmp si, VIEWPORT_TILES_Y
    jl viewport_y_loop_enh
    
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    ret
draw_viewport_enhanced endp

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
    
    ; Moneda dorada en el centro (corregido sin ESP)
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
    
    ; Cofre en el centro (corregido sin ESP)
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

; Funciones auxiliares para dibujo
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

draw_status_enhanced proc
    ; Mostrar estado del juego mejorado
    push ax
    push bx
    push cx
    push dx
    
    ; Verificar si el jugador ha completado los objetivos
    mov al, byte ptr [gold_count]
    cmp al, byte ptr [gold_needed]
    jl status_incomplete_enh
    mov al, byte ptr [crystals_count]
    cmp al, byte ptr [crystals_needed]
    jl status_incomplete_enh
    mov al, byte ptr [treasures_count]
    cmp al, byte ptr [treasures_needed]
    jl status_incomplete_enh
    
    ; Mensaje de victoria con efectos
    call draw_victory_message
    jmp status_end_enh
    
status_incomplete_enh:
    ; Mensaje de búsqueda con efectos
    call draw_search_message

status_end_enh:
    pop dx
    pop cx
    pop bx
    pop ax
    ret
draw_status_enhanced endp

draw_victory_message proc
    ; Dibujar "¡Ve a la salida!" con efectos
    mov dx, 330
    mov cx, 200
victory_loop:
    mov ah, 0Ch
    mov al, 14          ; Amarillo brillante
    mov bh, 0
    int 10h
    inc cx
    cmp cx, 440
    jl victory_loop
    ret
draw_victory_message endp

draw_search_message proc
    ; Dibujar "Busca tesoros" con efectos
    mov dx, 330
    mov cx, 180
search_loop:
    mov ah, 0Ch
    mov al, 12          ; Rojo para urgencia
    mov bh, 0
    int 10h
    inc cx
    cmp cx, 460
    jl search_loop
    ret
draw_search_message endp

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

check_item_collection proc
    push ax
    push bx
    push si
    
    mov al, byte ptr [py]
    mov ah, 0
    mov dx, MAPA_ANCHO
    mul dx
    mov dl, byte ptr [px]
    mov dh, 0
    add ax, dx
    mov si, ax
    
    ; Verificar límites
    cmp si, 300
    jge no_collection
    
    mov al, byte ptr [mapa + si]
    
    cmp al, 'G'
    je collect_gold
    cmp al, 'C'
    je collect_crystal
    cmp al, 'T'
    je collect_treasure
    jmp no_collection

collect_gold:
    inc byte ptr [gold_count]
    mov byte ptr [mapa + si], '+'
    mov byte ptr [redraw], 1
    jmp no_collection

collect_crystal:
    inc byte ptr [crystals_count]
    mov byte ptr [mapa + si], '+'
    mov byte ptr [redraw], 1
    jmp no_collection

collect_treasure:
    inc byte ptr [treasures_count]
    mov byte ptr [mapa + si], '+'
    mov byte ptr [redraw], 1

no_collection:
    pop si
    pop bx
    pop ax
    ret
check_item_collection endp

get_key_nowait proc
    mov byte ptr [tecla], 0
    mov ah, 01h
    int 16h
    jz no_key_available
    
    mov ah, 00h
    int 16h
    mov byte ptr [tecla], al

no_key_available:
    ret
get_key_nowait endp

move_player proc
    mov al, byte ptr [tecla]
    
    cmp al, 27
    je salida_juego
    
    cmp al, 'w'
    je mover_arriba
    cmp al, 'W'
    je mover_arriba
    
    cmp al, 's'
    je mover_abajo
    cmp al, 'S'
    je mover_abajo
    
    cmp al, 'a'
    je mover_izquierda
    cmp al, 'A'
    je mover_izquierda
    
    cmp al, 'd'
    je mover_derecha
    cmp al, 'D'
    je mover_derecha
    
    ret

salida_juego:
    mov byte ptr [fin], 1
    ret

mover_arriba:
    cmp byte ptr [py], 0
    je movimiento_invalido
    mov byte ptr [player_direction], 1  ; Dirección arriba
    dec byte ptr [py]
    call validar_movimiento
    ret

mover_abajo:
    cmp byte ptr [py], MAPA_ALTO-1
    je movimiento_invalido
    mov byte ptr [player_direction], 0  ; Dirección abajo
    inc byte ptr [py]
    call validar_movimiento
    ret

mover_izquierda:
    cmp byte ptr [px], 0
    je movimiento_invalido
    mov byte ptr [player_direction], 2  ; Dirección izquierda
    dec byte ptr [px]
    call validar_movimiento
    ret

mover_derecha:
    cmp byte ptr [px], MAPA_ANCHO-1
    je movimiento_invalido
    mov byte ptr [player_direction], 3  ; Dirección derecha
    inc byte ptr [px]
    call validar_movimiento
    ret

movimiento_invalido:
    ret
move_player endp

validar_movimiento proc
    push ax
    push bx
    push si
    
    mov al, byte ptr [py]
    mov ah, 0
    mov dx, MAPA_ANCHO
    mul dx
    mov dl, byte ptr [px]
    mov dh, 0
    add ax, dx
    mov si, ax
    
    ; Verificar límites
    cmp si, 300
    jge deshacer_movimiento
    
    mov al, byte ptr [mapa + si]
    cmp al, '#'
    je deshacer_movimiento
    cmp al, '~'
    je deshacer_movimiento
    cmp al, 'O'
    je deshacer_movimiento
    
    ; Movimiento válido
    mov byte ptr [redraw], 1
    jmp fin_validacion

deshacer_movimiento:
    mov al, byte ptr [tecla]
    cmp al, 'w'
    je deshacer_arriba
    cmp al, 'W'
    je deshacer_arriba
    cmp al, 's'
    je deshacer_abajo
    cmp al, 'S'
    je deshacer_abajo
    cmp al, 'a'
    je deshacer_izquierda
    cmp al, 'A'
    je deshacer_izquierda
    cmp al, 'd'
    je deshacer_derecha
    cmp al, 'D'
    je deshacer_derecha
    jmp fin_validacion

deshacer_arriba:
    inc byte ptr [py]
    jmp fin_validacion
deshacer_abajo:
    dec byte ptr [py]
    jmp fin_validacion
deshacer_izquierda:
    inc byte ptr [px]
    jmp fin_validacion
deshacer_derecha:
    dec byte ptr [px]
    jmp fin_validacion

fin_validacion:
    pop si
    pop bx
    pop ax
    ret
validar_movimiento endp

check_win proc
    push ax
    push bx
    push si
    
    mov al, byte ptr [gold_count]
    cmp al, byte ptr [gold_needed]
    jl no_ganaste
    
    mov al, byte ptr [crystals_count]
    cmp al, byte ptr [crystals_needed]
    jl no_ganaste
    
    mov al, byte ptr [treasures_count]
    cmp al, byte ptr [treasures_needed]
    jl no_ganaste
    
    mov al, byte ptr [py]
    mov ah, 0
    mov dx, MAPA_ANCHO
    mul dx
    mov dl, byte ptr [px]
    mov dh, 0
    add ax, dx
    mov si, ax
    
    cmp byte ptr [mapa + si], 'E'
    jne no_ganaste
    
    ; Victoria!
    mov ax, 0003h
    int 10h
    
    mov ah, 02h
    mov bh, 0
    mov dh, 8
    mov dl, 10
    int 10h
    
    mov ah, 09h
    mov dx, offset mensaje_victoria
    int 21h
    
    mov ah, 02h
    mov dh, 10
    mov dl, 15
    int 10h
    
    mov ah, 09h
    mov dx, offset mensaje_tecnico
    int 21h
    
    mov ah, 02h
    mov dh, 12
    mov dl, 12
    int 10h
    
    mov ah, 09h
    mov dx, offset mensaje_final
    int 21h
    
    mov ah, 07h
    int 21h
    mov byte ptr [fin], 1

no_ganaste:
    pop si
    pop bx
    pop ax
    ret

mensaje_victoria db '¡¡¡FELICIDADES!!! ¡Has completado Adventure Quest!$'
mensaje_tecnico db 'Modo EGA 640x350 con sprites detallados y animaciones.$'
mensaje_final db 'Proyecto de Assembly - Graficos EGA Avanzados$'
check_win endp

restore_screen proc
    mov ax, 0003h
    int 10h
    
    mov ah, 01h
    mov cx, 0607h
    int 10h
    ret
restore_screen endp

end start