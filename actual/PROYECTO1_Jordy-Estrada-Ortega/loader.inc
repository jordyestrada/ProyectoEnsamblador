; loader.inc - carga "mapa.bin" (300 bytes) si existe
; Solo procedimiento (sin .data ni .code)

load_map_from_file proc
    push ax
    push bx
    push cx
    push dx

    mov ax, 3D00h
    mov dx, offset mapa_file
    int 21h
    jc  short __loader_fail

    mov word ptr [__handle], ax
    mov bx, ax

    mov ah, 3Fh
    mov cx, MAP_BYTES
    mov dx, offset mapa
    int 21h

    mov ah, 3Eh
    mov bx, word ptr [__handle]
    int 21h

    mov byte ptr [screen_dirty], 1

__loader_fail:
    pop dx
    pop cx
    pop bx
    pop ax
    ret
load_map_from_file endp
