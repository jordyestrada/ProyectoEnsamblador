MAPA_ANCHO equ 20
MAPA_ALTO equ 15
TILE_SIZE equ 20

VIEWPORT_WIDTH equ 160
VIEWPORT_HEIGHT equ 100
VIEWPORT_TILES_X equ 10
VIEWPORT_TILES_Y equ 6
VIEWPORT_TILES_X_HALF equ 5
VIEWPORT_TILES_Y_HALF equ 3

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

camera_x db 0
camera_y db 0
