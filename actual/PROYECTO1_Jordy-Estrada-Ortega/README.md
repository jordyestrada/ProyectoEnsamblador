# Adventure Quest - Juego en Assembly 8086

## Estructura del Proyecto

### Archivos Principales
- `juego.asm` - Archivo principal del juego (logica, game loop, HUD)
- `mapa.inc` - Definiciones del mapa y constantes de viewport
- `loader.inc` - Carga de archivos binarios del mapa
- `sprites.inc` - Indice que incluye todos los archivos de sprites

### Sprites Organizados (archivos separados)
Cada objeto/tile ahora tiene su propio archivo:

**Helpers y Utilidades:**
- `sprites_helpers.inc` - Funciones base de dibujo (fill_tile_base, draw_pixel_at_offset, etc.)

**Tiles del Mapa:**
- `sprites_wall.inc` - Muro de piedra (#)
- `sprites_water.inc` - Agua animada (~)
- `sprites_path.inc` - Camino dorado (+)
- `sprites_exit.inc` - Salida magica (E)
- `sprites_floor.inc` - Piso de piedra (.)
- `sprites_pit.inc` - Pozo oscuro (O)

**Items Colectables:**
- `sprites_gold.inc` - Moneda de oro (G)
- `sprites_crystal.inc` - Gema de cristal (C)
- `sprites_treasure.inc` - Cofre del tesoro (T)

**Personaje:**
- `sprites_player.inc` - Heroe jugable con animacion y espada

**HUD:**
- `sprites_chars.inc` - Dibujado de caracteres para el HUD
- `sprites_draw_tile.inc` - Selector principal que llama a cada tile especifico

### Carpeta sprites/
Contiene copias organizadas por categoria (opcional, para desarrollo):
- `sprites/helpers.inc`
- `sprites/wall.inc`
- `sprites/water.inc`
- etc.

## Compilacion

```powershell
python tasm_assistant.py juego.asm
```

## Modo Debug

```powershell
python tasm_assistant.py juego.asm --debug-mode
```

## Controles
- **W/A/S/D** - Mover el jugador
- **ESC** - Salir del juego

## Objetivo
Recolectar 2 ORO, 2 CRISTALES y 2 TESOROS, luego llegar a la salida (E).

## Detalles Tecnicos
- Modo grafico: EGA 640x350, 16 colores
- Tiles: 20x20 pixeles
- Viewport: 10x6 tiles visibles
- Mapa total: 20x15 tiles
- Sistema de camara que sigue al jugador
- Doble buffer para optimizar el rendimiento
