; ===== juego.asm =====
.model small
.stack 100h

.data
    ; Sistema de doble buffer mejorado y scroll suave
    screen_dirty db 1
    last_px db 2
    last_py db 1
    last_camera_x db 0
    last_camera_y db 0
    
    ; Variables para scroll suave (interpolación pixel a pixel)
    scroll_offset_x db 0    ; Offset en píxeles (0-15) para scroll suave X
    scroll_offset_y db 0    ; Offset en píxeles (0-15) para scroll suave Y
    target_camera_x db 0    ; Posición objetivo de cámara X
    target_camera_y db 0    ; Posición objetivo de cámara Y
    scroll_speed db 4       ; Píxeles por frame de scroll (ajustable: 1-16)

    ; Incluir SOLO la parte .data de cada archivo
    include mapa.inc      ; contiene constantes y buffer mapa
    
    ; Variables de sprites (extra�das de sprites.inc)
    player_direction db 0
    player_frame     db 0
    animation_counter db 0
    temp_y dw 0
    temp_x dw 0
    temp_color db 0     ; Variable para dibujar n�meros
    char_base_x dw 0    ; Para draw_char_large y draw_char_fancy

    ; Variables de loader (extra�das de loader.inc)
    MAP_BYTES      equ MAPA_ANCHO * MAPA_ALTO
    mapa_file      db 'mapa.bin', 0
    __handle       dw 0

    ; Variables del jugador
    px db 2
    py db 1
    tecla db 0
    fin db 0
    game_paused db 0              ; Si el juego está en pausa (menú activo)
    menu_option db 0              ; Opción seleccionada en el menú (0-2)
    
    ; Propiedades de terreno
    slow_terrain_counter db 0    ; Contador para terreno lento
    ice_slide_active db 0         ; Si el jugador está deslizándose
    ice_slide_direction db 0      ; Dirección del deslizamiento

    ; Sistema de inventario (4 tipos de objetos, 2 de cada uno = 8 total)
    gold_count db 0           ; 'G' - Oro (máximo 2)
    crystals_count db 0       ; 'C' - Cristales (máximo 2)
    treasures_count db 0      ; 'T' - Tesoros (máximo 2)
    diamonds_count db 0       ; 'D' - Diamantes (máximo 2)
    
    ; Contadores anteriores para detectar cambios
    last_gold_count db 0
    last_crystals_count db 0
    last_treasures_count db 0
    last_diamonds_count db 0

    ; Objetivos (necesitas 2 de cada tipo para ganar = 8 objetos totales)
    gold_needed db 2
    crystals_needed db 2
    treasures_needed db 2
    diamonds_needed db 2
    max_per_item db 2         ; Máximo permitido por tipo

    ; Mensajes informativos
    ; Mensajes de la pantalla de inicio (historia de Zelda)
    msg_titulo1      db '          LA LEYENDA DE LINK          $'
    msg_titulo2      db '     La Busqueda de los Elementos     $'
    msg_linea1       db 'Link se encuentra atrapado en una cueva misteriosa...$'
    msg_linea2       db 'Zelda, su amada, lo espera en casa con preocupacion.$'
    msg_linea3       db 'Para regresar, Link debe encontrar los 4 Elementos$'
    msg_linea4       db 'Sagrados que activaran el portal magico de escape:$'
    msg_oro          db '  * 2 Lingotes de ORO (brillan con luz dorada)$'
    msg_cristal      db '  * 2 CRISTALES de poder (energia arcana pura)$'
    msg_tesoro       db '  * 2 TESOROS antiguos (reliquias del pasado)$'
    msg_diamante     db '  * 2 DIAMANTES eternos (gemas inmortales)$'
    msg_portal       db 'Solo entonces podra activar el portal y volver a Zelda.$'
    msg_controles    db 'Controles: [W][A][S][D] = Mover  |  [ESC] = Menu$'
    msg_presiona     db 'Presiona cualquier tecla para comenzar la aventura...$'
    
    ; Mensajes de la pantalla de victoria
    msg_vic_titulo   db '    FELICIDADES - MISION CUMPLIDA!    $'
    msg_vic_linea1   db 'Link ha encontrado todos los Elementos Sagrados!$'
    msg_vic_linea2   db 'El portal magico se activa con un destello verde...$'
    msg_vic_linea3   db 'En un parpadeo, Link aparece en su hogar.$'
    msg_vic_linea4   db 'Zelda corre a abrazarlo, aliviada de verlo sano.$'
    msg_vic_linea5   db 'Ahora Link y Zelda disfrutan de una taza de cafe,$'
    msg_vic_linea6   db 'recordando juntos esta increible aventura.$'
    msg_vic_final    db '        Gracias por jugar!        $'
    
    ; Mensajes antiguos (compatibilidad)
    mensaje_presiona db 'Presiona cualquier tecla para comenzar...$'
    mensaje_victoria db 'FELICIDADES!!! Has completado Adventure Quest!$'
    mensaje_tecnico  db 'Modo EGA 640x350 con sprites detallados y animaciones.$'
    mensaje_final    db 'Proyecto de Assembly - Graficos EGA Avanzados$'
    
    ; Mensajes del menú
    menu_titulo      db '=== MENU DE PAUSA ===$'
    menu_op1         db '1. Continuar Jugando$'
    menu_op2         db '2. Ver Inventario$'
    menu_op3         db '3. Salir del Juego$'
    menu_selecciona  db 'Selecciona una opcion (1-3): $'
    inv_titulo       db '=== INVENTARIO ===$'
    inv_oro          db 'Oro recolectado: $'
    inv_cristales    db 'Cristales recolectados: $'
    inv_tesoros      db 'Tesoros recolectados: $'
    inv_diamantes    db 'Diamantes recolectados: $'
    inv_volver       db 'Presiona cualquier tecla para volver...$'

.code
    ; Incluir SOLO la parte .code de cada archivo
    include sprites.inc   ; procedimientos de dibujo
    include loader.inc    ; procedimiento de carga

start:
    mov ax, @data
    mov ds, ax

    call load_map_from_file
    call show_intro
    call init_ega_mode
    
    ; Dibujar barras negras laterales una sola vez (ocultar área fuera del viewport)
    call clear_viewport_borders
    
    ; Inicializar contadores anteriores
    mov byte ptr [last_gold_count], 0
    mov byte ptr [last_crystals_count], 0
    mov byte ptr [last_treasures_count], 0
    mov byte ptr [last_diamonds_count], 0
    
    ; FORZAR DIBUJADO INICIAL COMPLETO
    call update_camera
    call clear_and_draw
    
    ; Inicializar �ltima posici�n de c�mara
    mov al, byte ptr [camera_x]
    mov byte ptr [last_camera_x], al
    mov al, byte ptr [camera_y]
    mov byte ptr [last_camera_y], al
    
    ; Inicializar �ltima posici�n del jugador
    mov al, byte ptr [px]
    mov byte ptr [last_px], al
    mov al, byte ptr [py]
    mov byte ptr [last_py], al
    
    mov byte ptr [screen_dirty], 0

game_loop:
    ; Verificar si el juego está en pausa
    cmp byte ptr [game_paused], 1
    je handle_menu
    
    ; Primero procesar entrada (más responsivo)
    call get_key_nowait
    cmp byte ptr [tecla], 0
    je no_input
    
    ; Verificar si se presionó ESC para abrir menú
    cmp byte ptr [tecla], 27
    je open_menu
    
    call move_player
    call update_player_animation
    call process_ice_slide        ; Procesar deslizamiento en hielo
    call check_item_collection
    call check_win
    
    ; Limpiar buffer SOLO después de procesar movimiento válido
    ; Esto evita que se acumulen teclas al mantener presionada
    call clear_keyboard_buffer
    
    ; Pequeño delay después de movimiento para evitar movimiento continuo
    mov cx, 0
    mov dx, 5000        ; ~5ms delay (ajustable)
    mov ah, 86h
    int 15h
    
    jmp no_input
    
open_menu:
    call clear_keyboard_buffer    ; Limpiar buffer antes de mostrar menú
    mov byte ptr [game_paused], 1
    call show_menu
    jmp game_loop
    
handle_menu:
    call get_key_nowait
    cmp byte ptr [tecla], 0
    je game_loop
    call process_menu_input
    call clear_keyboard_buffer    ; Limpiar buffer después de procesar
    jmp game_loop
    
no_input:
    ; Verificar si los contadores cambiaron para redibujar HUD
    mov al, byte ptr [gold_count]
    cmp al, byte ptr [last_gold_count]
    jne hud_changed
    mov al, byte ptr [crystals_count]
    cmp al, byte ptr [last_crystals_count]
    jne hud_changed
    mov al, byte ptr [treasures_count]
    cmp al, byte ptr [last_treasures_count]
    jne hud_changed
    jmp no_hud_change
    
hud_changed:
    ; Actualizar contadores (sin dibujar texto, solo para tracking)
    mov al, byte ptr [gold_count]
    mov byte ptr [last_gold_count], al
    mov al, byte ptr [crystals_count]
    mov byte ptr [last_crystals_count], al
    mov al, byte ptr [treasures_count]
    mov byte ptr [last_treasures_count], al
    ; NO dibujar texto de contadores (ya está en el menú)
    ; call draw_hud_text
    
no_hud_change:
    ; Luego dibujar si hay cambios
    cmp byte ptr [screen_dirty], 1
    jne skip_draw

    call update_camera

    ; Verificar si la cámara cambió
    mov al, byte ptr [camera_x]
    cmp al, byte ptr [last_camera_x]
    jne camera_changed
    mov al, byte ptr [camera_y]
    cmp al, byte ptr [last_camera_y]
    jne camera_changed

    ; Cámara no cambió - verificar si el jugador se movió
    mov al, byte ptr [px]
    cmp al, byte ptr [last_px]
    jne camera_changed     ; Si el jugador se movió, redibujar viewport
    mov al, byte ptr [py]
    cmp al, byte ptr [last_py]
    jne camera_changed     ; Si el jugador se movió, redibujar viewport
    
    ; Ni cámara ni jugador se movieron - solo redibujar sprite
    call draw_player_enhanced
    jmp draw_done

camera_changed:
    ; Redibujar SOLO viewport cuando la cámara cambia (no el HUD completo)
    call draw_viewport_enhanced
    call draw_player_enhanced
    call draw_status_enhanced
    
    ; Actualizar �ltima posici�n de c�mara
    mov al, byte ptr [camera_x]
    mov byte ptr [last_camera_x], al
    mov al, byte ptr [camera_y]
    mov byte ptr [last_camera_y], al

draw_done:
    ; Actualizar �ltima posici�n del jugador
    mov al, byte ptr [px]
    mov byte ptr [last_px], al
    mov al, byte ptr [py]
    mov byte ptr [last_py], al

    mov byte ptr [screen_dirty], 0

skip_draw:
    ; Pequeño delay para estabilidad (reducido porque ya hay delay en movimiento)
    mov cx, 0
    mov dx, 100         ; ~0.1ms delay
    mov ah, 86h
    int 15h
    
    cmp byte ptr [fin], 1
    je exit_game
    jmp game_loop

exit_game:
    call restore_screen
    mov ax, 4c00h
    int 21h

show_intro proc
    ; Modo texto 80x25
    mov ax, 0003h
    int 10h
    
    ; Cambiar color de fondo a verde oscuro (tema cueva)
    mov ah, 0Bh
    mov bh, 0
    mov bl, 2      ; Verde oscuro
    int 10h
    
    ; === TÍTULO ===
    mov ah, 02h
    mov bh, 0
    mov dh, 2
    mov dl, 14
    int 10h
    mov ah, 09h
    mov dx, offset msg_titulo1
    int 21h
    
    mov ah, 02h
    mov dh, 3
    mov dl, 12
    int 10h
    mov ah, 09h
    mov dx, offset msg_titulo2
    int 21h
    
    ; === HISTORIA ===
    mov ah, 02h
    mov dh, 5
    mov dl, 7
    int 10h
    mov ah, 09h
    mov dx, offset msg_linea1
    int 21h
    
    mov ah, 02h
    mov dh, 6
    mov dl, 7
    int 10h
    mov ah, 09h
    mov dx, offset msg_linea2
    int 21h
    
    mov ah, 02h
    mov dh, 8
    mov dl, 7
    int 10h
    mov ah, 09h
    mov dx, offset msg_linea3
    int 21h
    
    mov ah, 02h
    mov dh, 9
    mov dl, 7
    int 10h
    mov ah, 09h
    mov dx, offset msg_linea4
    int 21h
    
    ; === ELEMENTOS ===
    mov ah, 02h
    mov dh, 11
    mov dl, 10
    int 10h
    mov ah, 09h
    mov dx, offset msg_oro
    int 21h
    
    mov ah, 02h
    mov dh, 12
    mov dl, 10
    int 10h
    mov ah, 09h
    mov dx, offset msg_cristal
    int 21h
    
    mov ah, 02h
    mov dh, 13
    mov dl, 10
    int 10h
    mov ah, 09h
    mov dx, offset msg_tesoro
    int 21h
    
    mov ah, 02h
    mov dh, 14
    mov dl, 10
    int 10h
    mov ah, 09h
    mov dx, offset msg_diamante
    int 21h
    
    ; === PORTAL ===
    mov ah, 02h
    mov dh, 16
    mov dl, 7
    int 10h
    mov ah, 09h
    mov dx, offset msg_portal
    int 21h
    
    ; === CONTROLES ===
    mov ah, 02h
    mov dh, 19
    mov dl, 8
    int 10h
    mov ah, 09h
    mov dx, offset msg_controles
    int 21h
    
    ; === MENSAJE PRESIONAR ===
    mov ah, 02h
    mov dh, 22
    mov dl, 10
    int 10h
    mov ah, 09h
    mov dx, offset msg_presiona
    int 21h
    
    ; Esperar tecla
    mov ah, 07h
    int 21h
    ret
show_intro endp

init_ega_mode proc
    mov ax, 0010h
    int 10h
    ret
init_ega_mode endp

; ===== DOBLE BUFFER: Sincronización con VSync =====
; Espera al retrace vertical para evitar parpadeo (tearing)
wait_for_vsync proc
    push ax
    push dx
    
    ; Esperar a que termine el retrace actual (si está en uno)
    mov dx, 03DAh      ; Puerto de estado del CRT
wait_vsync_end:
    in al, dx
    test al, 08h       ; Bit 3 = vertical retrace activo
    jnz wait_vsync_end ; Esperar a que termine
    
    ; Ahora esperar al inicio del siguiente retrace
wait_vsync_start:
    in al, dx
    test al, 08h       ; Bit 3 = vertical retrace
    jz wait_vsync_start ; Esperar a que comience
    
    pop dx
    pop ax
    ret
wait_for_vsync endp

update_camera proc
    push ax
    push bx
    push cx

    ; Calcular posición objetivo de la cámara (target_camera_x/y)
    mov al, byte ptr [px]
    cmp al, VIEWPORT_TILES_X_HALF
    jl target_x_min
    sub al, VIEWPORT_TILES_X_HALF
    jmp target_x_check_max
target_x_min:
    mov al, 0
target_x_check_max:
    mov bl, MAPA_ANCHO - VIEWPORT_TILES_X
    cmp al, bl
    jle target_x_valid
    mov al, bl
target_x_valid:
    mov byte ptr [target_camera_x], al

    mov al, byte ptr [py]
    cmp al, VIEWPORT_TILES_Y_HALF
    jl target_y_min
    sub al, VIEWPORT_TILES_Y_HALF
    jmp target_y_check_max
target_y_min:
    mov al, 0
target_y_check_max:
    mov bl, MAPA_ALTO - VIEWPORT_TILES_Y
    cmp al, bl
    jle target_y_valid
    mov al, bl
target_y_valid:
    mov byte ptr [target_camera_y], al

    ; Interpolación suave hacia el objetivo en X
    mov al, byte ptr [camera_x]
    mov bl, byte ptr [target_camera_x]
    cmp al, bl
    je camera_x_done        ; Ya está en la posición objetivo
    jl camera_x_increase
    ; camera_x > target, decrementar
    dec al
    jmp camera_x_update
camera_x_increase:
    ; camera_x < target, incrementar
    inc al
camera_x_update:
    mov byte ptr [camera_x], al
camera_x_done:

    ; Interpolación suave hacia el objetivo en Y
    mov al, byte ptr [camera_y]
    mov bl, byte ptr [target_camera_y]
    cmp al, bl
    je camera_y_done        ; Ya está en la posición objetivo
    jl camera_y_increase
    ; camera_y > target, decrementar
    dec al
    jmp camera_y_update
camera_y_increase:
    ; camera_y < target, incrementar
    inc al
camera_y_update:
    mov byte ptr [camera_y], al
camera_y_done:

    pop cx
    pop bx
    pop ax
    ret
update_camera endp

draw_player_only proc
    call wait_for_vsync     ; Sincronizar con retrazado vertical
    push ax
    push bx
    push cx
    push dx

    ; Borrar jugador en posición anterior
    mov al, byte ptr [last_px]
    sub al, byte ptr [camera_x]      ; Usar camera_x actual, no last_camera_x
    mov ah, 0
    mov dx, TILE_SIZE
    mul dx
    add ax, 40
    mov cx, ax

    mov al, byte ptr [last_py]
    sub al, byte ptr [camera_y]      ; Usar camera_y actual, no last_camera_y
    mov ah, 0
    mov dx, TILE_SIZE
    mul dx
    add ax, 50
    mov dx, ax

    ; Redibujar el tile donde estaba el jugador
    push cx
    push dx
    mov al, byte ptr [last_py]
    mov ah, 0
    mov bx, MAPA_ANCHO
    mul bx
    mov bl, byte ptr [last_px]
    mov bh, 0
    add ax, bx
    mov bx, ax
    mov al, byte ptr [mapa + bx]
    pop dx
    pop cx
    call draw_tile_enhanced

    ; Dibujar jugador en nueva posición
    call draw_player_enhanced

    pop dx
    pop cx
    pop bx
    pop ax
    ret
draw_player_only endp

clear_and_draw proc
    ; call clear_screen_ega   ; <- desactivado
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
    mov dx, 0
clear_y_loop:
    mov cx, 0
clear_x_loop:
    mov ah, 0Ch
    mov al, 0
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
    
    ; Dibujar área superior del HUD negra (sin decoración visible, Y=0-7)
    mov dx, 0
hud_top_loop:
    mov cx, 0
hud_top_x_loop:
    mov ah, 0Ch
    mov al, 0           ; Negro (oculto)
    mov bh, 0
    int 10h
    inc cx
    cmp cx, 640
    jl hud_top_x_loop
    inc dx
    cmp dx, 8           ; Solo hasta Y=7
    jl hud_top_loop
    
    ; Dibujar FONDO para el �rea de recursos (Y=175-210) - JUSTO DEBAJO DEL VIEWPORT
    mov dx, 175
hud_resources_bg_loop:
    mov cx, 0
hud_resources_bg_x_loop:
    mov ah, 0Ch
    mov al, 0           ; Fondo negro para el texto
    mov bh, 0
    int 10h
    inc cx
    cmp cx, 640
    jl hud_resources_bg_x_loop
    inc dx
    cmp dx, 210
    jl hud_resources_bg_loop
    
    ; NO dibujar el texto de recursos (ya está en el menú de inventario)
    ; call draw_hud_text
    
    ; Dibujar área inferior negra (sin borde decorativo visible)
    mov dx, 210
hud_bottom_loop:
    mov cx, 0
hud_bottom_x_loop:
    mov ah, 0Ch
    mov al, 0           ; Negro (oculto)
    mov bh, 0
    int 10h
    inc cx
    cmp cx, 640
    jl hud_bottom_x_loop
    inc dx
    cmp dx, 220
    jl hud_bottom_loop
    
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
    push si
    
    ; NO limpiar toda el �rea, solo dibujar encima (mucho m�s r�pido)
    ; La limpieza solo se hace en draw_hud_enhanced al inicio
    
    ; Ahora dibujar los n�meros claramente
    ; ORO (amarillo)
    mov cx, 60
    mov dx, 185
    mov al, 14
    mov bl, byte ptr [gold_count]
    call draw_num_smart
    
    mov cx, 70
    mov al, 15
    mov bl, 10  ; C�digo para "/"
    call draw_num_smart
    
    mov cx, 80
    mov al, 14
    mov bl, byte ptr [gold_needed]
    call draw_num_smart
    
    ; CRISTALES (cyan)
    mov cx, 260
    mov dx, 185
    mov al, 11
    mov bl, byte ptr [crystals_count]
    call draw_num_smart
    
    mov cx, 270
    mov al, 15
    mov bl, 10  ; "/"
    call draw_num_smart
    
    mov cx, 280
    mov al, 11
    mov bl, byte ptr [crystals_needed]
    call draw_num_smart
    
    ; TESOROS (magenta)
    mov cx, 460
    mov dx, 185
    mov al, 12
    mov bl, byte ptr [treasures_count]
    call draw_num_smart
    
    mov cx, 470
    mov al, 15
    mov bl, 10  ; "/"
    call draw_num_smart
    
    mov cx, 480
    mov al, 12
    mov bl, byte ptr [treasures_needed]
    call draw_num_smart
    
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    ret
draw_hud_text endp

; Dibujar d�gito simple 7x9 p�xeles
; Dibujar n�mero inteligente (1 o 2 d�gitos autom�ticamente)
draw_num_smart proc
    push ax
    push bx
    push cx
    push dx
    
    mov byte ptr [temp_color], al
    
    ; Verificar si es de 1 o 2 d�gitos
    cmp bl, 10
    jl single_digit_ds
    
    ; 2 d�gitos: dividir
    mov al, bl
    mov ah, 0
    mov dl, 10
    div dl
    ; AL = decenas, AH = unidades
    
    ; Dibujar decenas
    push ax
    mov bl, al
    mov al, byte ptr [temp_color]
    call draw_digit_7x9
    pop ax
    
    ; Dibujar unidades (offset +8 p�xeles)
    add cx, 8
    mov bl, ah
    mov al, byte ptr [temp_color]
    call draw_digit_7x9
    jmp done_smart_ds
    
single_digit_ds:
    ; 1 d�gito: dibujar directamente
    mov al, byte ptr [temp_color]
    call draw_digit_7x9
    
done_smart_ds:
    pop dx
    pop cx
    pop bx
    pop ax
    ret
draw_num_smart endp

draw_digit_7x9 proc
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    
    mov word ptr [temp_x], cx
    mov word ptr [temp_y], dx
    mov byte ptr [temp_color], al
    
    ; Selector
    cmp bl, 0
    je digit_7x9_0
    cmp bl, 1
    je digit_7x9_1
    cmp bl, 2
    je digit_7x9_2
    cmp bl, 3
    je digit_7x9_3
    cmp bl, 4
    je digit_7x9_4
    cmp bl, 5
    je digit_7x9_5
    cmp bl, 6
    je digit_7x9_6
    cmp bl, 7
    je digit_7x9_7
    cmp bl, 8
    je digit_7x9_8
    cmp bl, 9
    je digit_7x9_9
    cmp bl, 10
    je digit_7x9_slash
    jmp digit_7x9_end

digit_7x9_0:
    call pattern_0
    jmp digit_7x9_end
digit_7x9_1:
    call pattern_1
    jmp digit_7x9_end
digit_7x9_2:
    call pattern_2
    jmp digit_7x9_end
digit_7x9_3:
    call pattern_3
    jmp digit_7x9_end
digit_7x9_4:
    call pattern_4
    jmp digit_7x9_end
digit_7x9_5:
    call pattern_5
    jmp digit_7x9_end
digit_7x9_6:
    call pattern_6
    jmp digit_7x9_end
digit_7x9_7:
    call pattern_7
    jmp digit_7x9_end
digit_7x9_8:
    call pattern_8
    jmp digit_7x9_end
digit_7x9_9:
    call pattern_9
    jmp digit_7x9_end
digit_7x9_slash:
    call pattern_slash

digit_7x9_end:
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    ret
draw_digit_7x9 endp

; P�xel helper
pix proc
    push ax
    push bx
    push cx
    push dx
    mov ah, 0Ch
    mov al, byte ptr [temp_color]
    mov bh, 0
    int 10h
    pop dx
    pop cx
    pop bx
    pop ax
    ret
pix endp

; Patrones 0-9 y slash
pattern_0 proc
    mov si, 0
p0_loop:
    mov cx, word ptr [temp_x]
    mov dx, word ptr [temp_y]
    add dx, si
    call pix
    add cx, 6
    call pix
    cmp si, 0
    je p0_h
    cmp si, 8
    je p0_h
    jmp p0_n
p0_h:
    mov cx, word ptr [temp_x]
    add cx, 1
    mov dx, word ptr [temp_y]
    add dx, si
    call pix
    add cx, 1
    call pix
    add cx, 1
    call pix
    add cx, 1
    call pix
p0_n:
    inc si
    cmp si, 9
    jl p0_loop
    ret
pattern_0 endp

pattern_1 proc
    mov si, 0
p1_loop:
    mov cx, word ptr [temp_x]
    add cx, 3
    mov dx, word ptr [temp_y]
    add dx, si
    call pix
    inc si
    cmp si, 9
    jl p1_loop
    ret
pattern_1 endp

pattern_2 proc
    mov cx, word ptr [temp_x]
    mov dx, word ptr [temp_y]
    call pix
    inc cx
    call pix
    inc cx
    call pix
    inc cx
    call pix
    inc cx
    call pix
    inc cx
    call pix
    inc cx
    call pix
    
    mov si, 1
p2_r:
    mov cx, word ptr [temp_x]
    add cx, 6
    mov dx, word ptr [temp_y]
    add dx, si
    call pix
    inc si
    cmp si, 5
    jl p2_r
    
    mov cx, word ptr [temp_x]
    mov dx, word ptr [temp_y]
    add dx, 4
    call pix
    inc cx
    call pix
    inc cx
    call pix
    inc cx
    call pix
    inc cx
    call pix
    inc cx
    call pix
    add cx, 6
    ret
pattern_2 endp

pattern_3 proc
    call pattern_2
    mov si, 5
p3_r:
    mov cx, word ptr [temp_x]
    add cx, 6
    mov dx, word ptr [temp_y]
    add dx, si
    call pix
    inc si
    cmp si, 8
    jl p3_r
    ret
pattern_3 endp

pattern_4 proc
    mov si, 0
p4_l:
    mov cx, word ptr [temp_x]
    mov dx, word ptr [temp_y]
    add dx, si
    call pix
    inc si
    cmp si, 5
    jl p4_l
    
    mov cx, word ptr [temp_x]
    mov dx, word ptr [temp_y]
    add dx, 4
    call pix
    inc cx
    call pix
    inc cx
    call pix
    inc cx
    call pix
    inc cx
    call pix
    inc cx
    call pix
    add cx, 6
    ret
    
    mov si, 0
p4_r:
    mov cx, word ptr [temp_x]
    add cx, 6
    mov dx, word ptr [temp_y]
    add dx, si
    call pix
    inc si
    cmp si, 9
    jl p4_r
    ret
pattern_4 endp

pattern_5 proc
    mov cx, word ptr [temp_x]
    mov dx, word ptr [temp_y]
    call pix
    inc cx
    call pix
    inc cx
    call pix
    inc cx
    call pix
    inc cx
    call pix
    inc cx
    call pix
    add cx, 6
    
    mov si, 1
p5_l:
    mov cx, word ptr [temp_x]
    mov dx, word ptr [temp_y]
    add dx, si
    call pix
    inc si
    cmp si, 5
    jl p5_l
    
    mov cx, word ptr [temp_x]
    mov dx, word ptr [temp_y]
    add dx, 4
    call pix
    inc cx
    call pix
    inc cx
    call pix
    inc cx
    call pix
    inc cx
    call pix
    inc cx
    call pix
    add cx, 6
    
    mov si, 5
p5_r:
    mov cx, word ptr [temp_x]
    add cx, 6
    mov dx, word ptr [temp_y]
    add dx, si
    call pix
    inc si
    cmp si, 8
    jl p5_r
    
    mov cx, word ptr [temp_x]
    mov dx, word ptr [temp_y]
    add dx, 8
    call pix
    inc cx
    call pix
    inc cx
    call pix
    inc cx
    call pix
    inc cx
    call pix
    inc cx
    call pix
    add cx, 6
    ret
pattern_5 endp

pattern_6 proc
    call pattern_5
    mov si, 5
p6_l:
    mov cx, word ptr [temp_x]
    mov dx, word ptr [temp_y]
    add dx, si
    call pix
    inc si
    cmp si, 8
    jl p6_l
    ret
pattern_6 endp

pattern_7 proc
    mov cx, word ptr [temp_x]
    mov dx, word ptr [temp_y]
    call pix
    inc cx
    call pix
    inc cx
    call pix
    inc cx
    call pix
    inc cx
    call pix
    inc cx
    call pix
    add cx, 6
    
    mov si, 1
p7_r:
    mov cx, word ptr [temp_x]
    add cx, 6
    mov dx, word ptr [temp_y]
    add dx, si
    call pix
    inc si
    cmp si, 9
    jl p7_r
    ret
pattern_7 endp

pattern_8 proc
    call pattern_0
    mov cx, word ptr [temp_x]
    mov dx, word ptr [temp_y]
    add dx, 4
    call pix
    inc cx
    call pix
    inc cx
    call pix
    inc cx
    call pix
    inc cx
    call pix
    inc cx
    call pix
    add cx, 6
    ret
pattern_8 endp

pattern_9 proc
    call pattern_8
    ret
pattern_9 endp

pattern_slash proc
    mov si, 0
psl_loop:
    mov cx, word ptr [temp_x]
    add cx, si
    mov dx, word ptr [temp_y]
    add dx, 8
    sub dx, si
    call pix
    inc si
    cmp si, 9
    jl psl_loop
    ret
pattern_slash endp


draw_viewport_enhanced proc
    call wait_for_vsync     ; Sincronizar con retrazado vertical
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    mov si, 0
viewport_y_loop_enh:
    mov di, 0
viewport_x_loop_enh:
    mov al, byte ptr [camera_y]
    mov ah, 0
    add ax, si
    mov dx, MAPA_ANCHO
    mul dx
    mov bl, byte ptr [camera_x]
    mov bh, 0
    add bx, di
    add ax, bx
    mov bx, ax
    cmp bx, 500
    jge skip_current_tile_enh
    mov al, byte ptr [mapa + bx]
    push ax
    mov ax, di
    mov dx, TILE_SIZE
    mul dx
    add ax, 40
    mov cx, ax
    mov ax, si
    mov dx, TILE_SIZE
    mul dx
    add ax, 50
    mov dx, ax
    pop ax
    call draw_tile_enhanced
    jmp short continue_loop_enh
skip_current_tile_enh:
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

clear_viewport_borders proc
    ; Dibujar barras negras verticales para ocultar todo fuera del viewport
    ; OPTIMIZADO: Dibuja líneas completas en vez de pixel por pixel
    push ax
    push bx
    push cx
    push dx
    push si
    
    ; Barra izquierda (x=0 a x=39, toda la altura)
    mov si, 0           ; Contador Y
left_bar_loop:
    mov dx, si
    mov cx, 0           ; X inicial
left_x_loop:
    mov ah, 0Ch
    mov al, 0           ; Negro
    mov bh, 0
    int 10h
    inc cx
    cmp cx, 40
    jl left_x_loop
    inc si
    cmp si, 350
    jl left_bar_loop
    
    ; Barra derecha (x=200 a x=639, toda la altura)  
    mov si, 0           ; Contador Y
right_bar_loop:
    mov dx, si
    mov cx, 200         ; X inicial (después del viewport)
right_x_loop:
    mov ah, 0Ch
    mov al, 0           ; Negro
    mov bh, 0
    int 10h
    inc cx
    cmp cx, 640
    jl right_x_loop
    inc si
    cmp si, 350
    jl right_bar_loop
    
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    ret
clear_viewport_borders endp

draw_status_enhanced proc
    push ax
    push bx
    push cx
    push dx
    ; Verificar si tiene todos los objetos necesarios (2 de cada tipo = 8 total)
    mov al, byte ptr [gold_count]
    cmp al, byte ptr [gold_needed]
    jl status_incomplete_enh
    mov al, byte ptr [crystals_count]
    cmp al, byte ptr [crystals_needed]
    jl status_incomplete_enh
    mov al, byte ptr [treasures_count]
    cmp al, byte ptr [treasures_needed]
    jl status_incomplete_enh
    mov al, byte ptr [diamonds_count]
    cmp al, byte ptr [diamonds_needed]
    jl status_incomplete_enh
    ; Tiene todos los objetos - mostrar barra AMARILLA (victoria lista)
    call draw_victory_message
    jmp status_end_enh
status_incomplete_enh:
    ; No tiene todos - mostrar barra ROJA (aún buscando)
    call draw_search_message
status_end_enh:
    pop dx
    pop cx
    pop bx
    pop ax
    ret
draw_status_enhanced endp

draw_victory_message proc
    mov dx, 330
    mov cx, 200
victory_loop:
    mov ah, 0Ch
    mov al, 14
    mov bh, 0
    int 10h
    inc cx
    cmp cx, 440
    jl victory_loop
    ret
draw_victory_message endp

draw_search_message proc
    mov dx, 330
    mov cx, 180
search_loop:
    mov ah, 0Ch
    mov al, 12
    mov bh, 0
    int 10h
    inc cx
    cmp cx, 460
    jl search_loop
    ret
draw_search_message endp

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
    cmp si, 500
    jge no_collection
    mov al, byte ptr [mapa + si]
    cmp al, 'G'
    je collect_gold
    cmp al, 'C'
    je collect_crystal
    cmp al, 'T'
    je collect_treasure
    cmp al, 'D'
    je collect_diamond
    jmp no_collection
    
collect_gold:
    ; Verificar si ya tiene el máximo de oro
    mov al, byte ptr [gold_count]
    cmp al, byte ptr [max_per_item]
    jge no_collection              ; Ya tiene 2, NO recoger (dejar en suelo)
    ; Tiene menos de 2, puede recoger
    inc byte ptr [gold_count]
    mov byte ptr [mapa + si], '.'  ; Solo eliminar del mapa si se recoge
    mov byte ptr [screen_dirty], 1
    jmp no_collection
    
collect_crystal:
    ; Verificar si ya tiene el máximo de cristales
    mov al, byte ptr [crystals_count]
    cmp al, byte ptr [max_per_item]
    jge no_collection              ; Ya tiene 2, NO recoger (dejar en suelo)
    ; Tiene menos de 2, puede recoger
    inc byte ptr [crystals_count]
    mov byte ptr [mapa + si], '.'  ; Solo eliminar del mapa si se recoge
    mov byte ptr [screen_dirty], 1
    jmp no_collection
    
collect_treasure:
    ; Verificar si ya tiene el máximo de tesoros
    mov al, byte ptr [treasures_count]
    cmp al, byte ptr [max_per_item]
    jge no_collection              ; Ya tiene 2, NO recoger (dejar en suelo)
    ; Tiene menos de 2, puede recoger
    inc byte ptr [treasures_count]
    mov byte ptr [mapa + si], '.'  ; Solo eliminar del mapa si se recoge
    mov byte ptr [screen_dirty], 1
    jmp no_collection
    
collect_diamond:
    ; Verificar si ya tiene el máximo de diamantes
    mov al, byte ptr [diamonds_count]
    cmp al, byte ptr [max_per_item]
    jge no_collection              ; Ya tiene 2, NO recoger (dejar en suelo)
    ; Tiene menos de 2, puede recoger
    inc byte ptr [diamonds_count]
    mov byte ptr [mapa + si], '.'  ; Solo eliminar del mapa si se recoge
    mov byte ptr [screen_dirty], 1
    jmp no_collection
    
no_collection:
    pop si
    pop bx
    pop ax
    ret
check_item_collection endp

get_key_nowait proc
    mov byte ptr [tecla], 0
    mov ah, 01h         ; Verificar si hay tecla disponible
    int 16h
    jz no_key_available ; Si ZF=1, no hay tecla
    mov ah, 00h         ; Leer y remover la tecla del buffer
    int 16h
    mov byte ptr [tecla], al
no_key_available:
    ret
get_key_nowait endp

clear_keyboard_buffer proc
    ; Limpiar todas las teclas pendientes en el buffer
clear_loop:
    mov ah, 01h         ; Verificar si hay tecla en buffer
    int 16h
    jz buffer_empty     ; Si no hay tecla, salir
    mov ah, 00h         ; Leer y descartar la tecla
    int 16h
    jmp clear_loop      ; Continuar limpiando
buffer_empty:
    ret
clear_keyboard_buffer endp

move_player proc
    mov al, byte ptr [tecla]
    ; ESC ahora abre el menú, no cierra el juego directamente
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
mover_arriba:
    cmp byte ptr [py], 0
    je movimiento_invalido
    mov byte ptr [player_direction], 1
    dec byte ptr [py]
    call validar_movimiento
    ret
mover_abajo:
    cmp byte ptr [py], MAPA_ALTO-1
    je movimiento_invalido
    mov byte ptr [player_direction], 0
    inc byte ptr [py]
    call validar_movimiento
    ret
mover_izquierda:
    cmp byte ptr [px], 0
    je movimiento_invalido
    mov byte ptr [player_direction], 2
    dec byte ptr [px]
    call validar_movimiento
    ret
mover_derecha:
    cmp byte ptr [px], MAPA_ANCHO-1
    je movimiento_invalido
    mov byte ptr [player_direction], 3
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
    
    ; Calcular posición en el mapa
    mov al, byte ptr [py]
    mov ah, 0
    mov dx, MAPA_ANCHO
    mul dx
    mov dl, byte ptr [px]
    mov dh, 0
    add ax, dx
    mov si, ax
    cmp si, 500
    jge deshacer_movimiento
    
    ; Obtener tipo de terreno
    mov al, byte ptr [mapa + si]
    
    ; Verificar muros (bloquean completamente)
    cmp al, '#'
    je deshacer_movimiento
    
    ; Verificar AGUA (~) - TERRENO PANTANOSO (ralentiza)
    cmp al, '~'
    je terreno_pantanoso
    
    ; Verificar POZOS (O) - TERRENO DE HIELO (deslizamiento)
    cmp al, 'O'
    je terreno_hielo
    
    ; Terreno normal - movimiento permitido
    mov byte ptr [slow_terrain_counter], 0
    mov byte ptr [screen_dirty], 1
    jmp fin_validacion
    
terreno_pantanoso:
    ; El agua ralentiza - solo permite moverse cada 4 movimientos (muy lento)
    inc byte ptr [slow_terrain_counter]
    cmp byte ptr [slow_terrain_counter], 4
    jl deshacer_movimiento_lento
    ; Permitir movimiento y resetear contador
    mov byte ptr [slow_terrain_counter], 0
    mov byte ptr [screen_dirty], 1
    jmp fin_validacion
    
terreno_hielo:
    ; El pozo oscuro actúa como hielo - el jugador se desliza una casilla extra
    mov byte ptr [screen_dirty], 1
    ; Guardar dirección para deslizamiento
    mov al, byte ptr [player_direction]
    mov byte ptr [ice_slide_direction], al
    mov byte ptr [ice_slide_active], 1
    jmp fin_validacion
    
deshacer_movimiento_lento:
    ; Deshacer movimiento pero mostrar que está en terreno lento
    mov byte ptr [screen_dirty], 1
    jmp deshacer_movimiento
    
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
    
    ; Verificar que tiene 2 de cada tipo (8 objetos totales)
    mov al, byte ptr [gold_count]
    cmp al, byte ptr [gold_needed]
    jge win_check_crystals
    jmp win_no_ganaste
win_check_crystals:
    mov al, byte ptr [crystals_count]
    cmp al, byte ptr [crystals_needed]
    jge win_check_treasures
    jmp win_no_ganaste
win_check_treasures:
    mov al, byte ptr [treasures_count]
    cmp al, byte ptr [treasures_needed]
    jge win_check_diamonds
    jmp win_no_ganaste
win_check_diamonds:
    mov al, byte ptr [diamonds_count]
    cmp al, byte ptr [diamonds_needed]
    jge win_check_exit_pos
    jmp win_no_ganaste
win_check_exit_pos:
    ; Verificar que está en la salida 'E'
    mov al, byte ptr [py]
    mov ah, 0
    mov dx, MAPA_ANCHO
    mul dx
    mov dl, byte ptr [px]
    mov dh, 0
    add ax, dx
    mov si, ax
    cmp byte ptr [mapa + si], 'E'
    je win_show_victory
    jmp win_no_ganaste
    
win_show_victory:
    ; === PANTALLA DE VICTORIA MEJORADA ===
    ; Modo texto 80x25
    mov ax, 0003h
    int 10h
    
    ; Fondo azul (teletransportación)
    mov ah, 0Bh
    mov bh, 0
    mov bl, 1      ; Azul
    int 10h
    
    ; === TÍTULO (Amarillo brillante) ===
    mov ah, 02h
    mov bh, 0
    mov dh, 3
    mov dl, 13
    int 10h
    mov ah, 09h
    mov dx, offset msg_vic_titulo
    int 21h
    
    ; === HISTORIA DE VICTORIA (Blanco) ===
    mov ah, 02h
    mov dh, 6
    mov dl, 10
    int 10h
    mov ah, 09h
    mov dx, offset msg_vic_linea1
    int 21h
    
    mov ah, 02h
    mov dh, 8
    mov dl, 8
    int 10h
    mov ah, 09h
    mov dx, offset msg_vic_linea2
    int 21h
    
    mov ah, 02h
    mov dh, 10
    mov dl, 15
    int 10h
    mov ah, 09h
    mov dx, offset msg_vic_linea3
    int 21h
    
    mov ah, 02h
    mov dh, 12
    mov dl, 10
    int 10h
    mov ah, 09h
    mov dx, offset msg_vic_linea4
    int 21h
    
    ; === CAFÉ CON ZELDA (Verde/Cyan - ambiente hogareño) ===
    mov ah, 02h
    mov dh, 14
    mov dl, 8
    int 10h
    mov ah, 09h
    mov dx, offset msg_vic_linea5
    int 21h
    
    mov ah, 02h
    mov dh, 15
    mov dl, 8
    int 10h
    mov ah, 09h
    mov dx, offset msg_vic_linea6
    int 21h
    
    ; === MENSAJE FINAL (Magenta brillante) ===
    mov ah, 02h
    mov dh, 19
    mov dl, 20
    int 10h
    mov ah, 09h
    mov dx, offset msg_vic_final
    int 21h
    
    ; Esperar tecla
    mov ah, 07h
    int 21h
    mov byte ptr [fin], 1
win_no_ganaste:
    pop si
    pop bx
    pop ax
    ret
check_win endp

process_ice_slide proc
    ; Verificar si hay deslizamiento activo
    cmp byte ptr [ice_slide_active], 0
    je no_slide
    
    ; Resetear flag
    mov byte ptr [ice_slide_active], 0
    
    ; Mover una casilla extra en la dirección actual
    mov al, byte ptr [ice_slide_direction]
    cmp al, 0  ; Abajo
    je slide_down
    cmp al, 1  ; Arriba
    je slide_up
    cmp al, 2  ; Izquierda
    je slide_left
    cmp al, 3  ; Derecha
    je slide_right
    jmp no_slide
    
slide_up:
    cmp byte ptr [py], 0
    je no_slide
    dec byte ptr [py]
    call check_slide_collision
    ret
    
slide_down:
    cmp byte ptr [py], MAPA_ALTO-1
    je no_slide
    inc byte ptr [py]
    call check_slide_collision
    ret
    
slide_left:
    cmp byte ptr [px], 0
    je no_slide
    dec byte ptr [px]
    call check_slide_collision
    ret
    
slide_right:
    cmp byte ptr [px], MAPA_ANCHO-1
    je no_slide
    inc byte ptr [px]
    call check_slide_collision
    ret
    
no_slide:
    ret
process_ice_slide endp

check_slide_collision proc
    ; Verificar si la nueva posición es válida
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
    
    ; Si es muro, deshacer el deslizamiento
    mov al, byte ptr [mapa + si]
    cmp al, '#'
    je undo_slide
    
    ; Marcar pantalla como sucia
    mov byte ptr [screen_dirty], 1
    pop si
    pop bx
    pop ax
    ret
    
undo_slide:
    ; Deshacer el movimiento
    mov al, byte ptr [ice_slide_direction]
    cmp al, 0  ; Si fue abajo, volver arriba
    je undo_down
    cmp al, 1  ; Si fue arriba, volver abajo
    je undo_up
    cmp al, 2  ; Si fue izquierda, volver derecha
    je undo_left
    cmp al, 3  ; Si fue derecha, volver izquierda
    je undo_right
    jmp end_undo
    
undo_up:
    inc byte ptr [py]
    jmp end_undo
undo_down:
    dec byte ptr [py]
    jmp end_undo
undo_left:
    inc byte ptr [px]
    jmp end_undo
undo_right:
    dec byte ptr [px]
    
end_undo:
    pop si
    pop bx
    pop ax
    ret
check_slide_collision endp

show_menu proc
    ; Guardar modo gráfico y cambiar a modo texto
    push ax
    mov ax, 0003h
    int 10h
    
    ; Mostrar título del menú
    mov ah, 02h
    mov bh, 0
    mov dh, 5
    mov dl, 25
    int 10h
    mov ah, 09h
    mov dx, offset menu_titulo
    int 21h
    
    ; Opción 1
    mov ah, 02h
    mov dh, 8
    mov dl, 25
    int 10h
    mov ah, 09h
    mov dx, offset menu_op1
    int 21h
    
    ; Opción 2
    mov ah, 02h
    mov dh, 10
    mov dl, 25
    int 10h
    mov ah, 09h
    mov dx, offset menu_op2
    int 21h
    
    ; Opción 3
    mov ah, 02h
    mov dh, 12
    mov dl, 25
    int 10h
    mov ah, 09h
    mov dx, offset menu_op3
    int 21h
    
    ; Mensaje de selección
    mov ah, 02h
    mov dh, 15
    mov dl, 20
    int 10h
    mov ah, 09h
    mov dx, offset menu_selecciona
    int 21h
    
    pop ax
    ret
show_menu endp

process_menu_input proc
    mov al, byte ptr [tecla]
    
    cmp al, '1'
    je menu_continuar
    cmp al, '2'
    je menu_inventario
    cmp al, '3'
    je menu_salir
    ret
    
menu_continuar:
    ; Volver al juego
    mov byte ptr [game_paused], 0
    call init_ega_mode
    call clear_keyboard_buffer
    ; Forzar redibujado completo
    call update_camera
    call clear_and_draw
    ret
    
menu_inventario:
    call show_inventory
    call show_menu  ; Volver a mostrar el menú
    ret
    
menu_salir:
    call clear_keyboard_buffer
    mov byte ptr [game_paused], 0  ; Desactivar pausa
    mov byte ptr [fin], 1
    ret
process_menu_input endp

show_inventory proc
    ; Limpiar pantalla
    mov ax, 0003h
    int 10h
    
    ; Título
    mov ah, 02h
    mov bh, 0
    mov dh, 3
    mov dl, 25
    int 10h
    mov ah, 09h
    mov dx, offset inv_titulo
    int 21h
    
    ; Mostrar oro
    mov ah, 02h
    mov dh, 6
    mov dl, 20
    int 10h
    mov ah, 09h
    mov dx, offset inv_oro
    int 21h
    mov al, byte ptr [gold_count]
    add al, '0'
    mov dl, al
    mov ah, 02h
    int 21h
    mov dl, '/'
    int 21h
    mov al, byte ptr [gold_needed]
    add al, '0'
    mov dl, al
    int 21h
    
    ; Mostrar cristales
    mov ah, 02h
    mov bh, 0
    mov dh, 8
    mov dl, 20
    int 10h
    mov ah, 09h
    mov dx, offset inv_cristales
    int 21h
    mov al, byte ptr [crystals_count]
    add al, '0'
    mov dl, al
    mov ah, 02h
    int 21h
    mov dl, '/'
    int 21h
    mov al, byte ptr [crystals_needed]
    add al, '0'
    mov dl, al
    int 21h
    
    ; Mostrar tesoros
    mov ah, 02h
    mov bh, 0
    mov dh, 10
    mov dl, 20
    int 10h
    mov ah, 09h
    mov dx, offset inv_tesoros
    int 21h
    mov al, byte ptr [treasures_count]
    add al, '0'
    mov dl, al
    mov ah, 02h
    int 21h
    mov dl, '/'
    int 21h
    mov al, byte ptr [treasures_needed]
    add al, '0'
    mov dl, al
    int 21h
    
    ; Mostrar diamantes
    mov ah, 02h
    mov bh, 0
    mov dh, 12
    mov dl, 20
    int 10h
    mov ah, 09h
    mov dx, offset inv_diamantes
    int 21h
    mov al, byte ptr [diamonds_count]
    add al, '0'
    mov dl, al
    mov ah, 02h
    int 21h
    mov dl, '/'
    int 21h
    mov al, byte ptr [diamonds_needed]
    add al, '0'
    mov dl, al
    int 21h
    
    ; Mensaje volver
    mov ah, 02h
    mov bh, 0
    mov dh, 15
    mov dl, 15
    int 10h
    mov ah, 09h
    mov dx, offset inv_volver
    int 21h
    
    ; Esperar tecla
    mov ah, 07h
    int 21h
    
    ret
show_inventory endp

restore_screen proc
    mov ax, 0003h
    int 10h
    mov ah, 01h
    mov cx, 0607h
    int 10h
    ret
restore_screen endp

end start
