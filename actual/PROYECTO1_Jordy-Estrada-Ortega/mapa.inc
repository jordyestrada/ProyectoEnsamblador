; ===== mapa.inc =====
MAPA_ANCHO       equ 25
MAPA_ALTO        equ 20
TILE_SIZE        equ 16        ; Mínimo requerido: 16x16 píxeles

VIEWPORT_WIDTH        equ 160  ; Mínimo requerido: 160 píxeles
VIEWPORT_HEIGHT       equ 112  ; Mínimo 100px, ajustado a múltiplo de 16 (7 tiles)
VIEWPORT_TILES_X      equ 10   ; 160 / 16 = 10 tiles
VIEWPORT_TILES_Y      equ 7    ; 112 / 16 = 7 tiles
VIEWPORT_TILES_X_HALF equ 5
VIEWPORT_TILES_Y_HALF equ 3

; PROPIEDADES DE TERRENO (3 tipos diferentes):
; '#' = MURO - Bloquea completamente el paso (Propiedad 1: SOLIDO)
; '~' = AGUA - Terreno pantanoso que ralentiza (Propiedad 2: LENTO - solo 1 mov cada 4 intentos)
; 'O' = POZO - Terreno resbaladizo/hielo (Propiedad 3: DESLIZANTE - mueve 2 casillas)
; '+' = CAMINO DORADO - Terreno normal transitable
; '.' = PISO - Terreno normal transitable
; 'G' = Oro (necesitas 2, hay 5 disponibles)
; 'C' = Cristal (necesitas 2, hay 5 disponibles)
; 'T' = Tesoro (necesitas 2, hay 5 disponibles)
; 'D' = Diamante (necesitas 2, hay 5 disponibles)
; 'E' = Salida (necesitas 8 objetos totales: 2 de cada tipo)

; 25x20 = 500 bytes (mapa expandido)
; Distribuidos: 5 Oro (G), 5 Cristales (C), 5 Tesoros (T), 5 Diamantes (D) = 20 objetos totales
; Solo necesitas recoger 2 de cada tipo (8 totales) para ganar
mapa db '#########################'
     db '#..G....................#'
     db '#..G...##+##...C........#'
     db '#......##+##...C........#'
     db '#..~~~.......~~~........#'
     db '#..~~~..#.#..~~~..##..T.#'
     db '#.......#.#.......##....#'
     db '#...OOO.....OOO.....D...#'
     db '#.T...................D.#'
     db '###..#####....#####.....#'
     db '#.T........G.C..........#'
     db '#....~~~...C.......~~~..#'
     db '#....~~~...G.......~~~..#'
     db '#.......#######.........#'
     db '#..OOO..#.....#..OOO..D.#'
     db '#...D...#..E..#.........#'
     db '#.......#.....#.....C...#'
     db '#..D...................T#'
     db '#.....G.........T.......#'
     db '#########################'

camera_x db 0
camera_y db 0
