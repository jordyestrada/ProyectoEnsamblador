import os
import subprocess
import sys
import tempfile
import shutil
from pathlib import Path

class AssemblerAssistant:
    def __init__(self):
        # Ruta específica de DOSBox
        self.dosbox_path = "C:\\Program Files (x86)\\DOSBox-0.74-3\\DOSBox.exe"
        self.tasm_path = self.find_tasm()
        self.work_dir = tempfile.mkdtemp(prefix="asm8086_")
        
        # Verificar que DOSBox existe en la ruta especificada
        if not os.path.exists(self.dosbox_path):
            print(f"Advertencia: DOSBox no encontrado en {self.dosbox_path}")
            print("Buscando en otras ubicaciones...")
            self.dosbox_path = self.find_dosbox_fallback()
        
    def find_dosbox_fallback(self):
        """Busca DOSBox en otras rutas si la principal no funciona"""
        possible_paths = [
            "C:\\Program Files (x86)\\DOSBox-0.74-3\\DOSBox.exe",
            "C:\\Program Files (x86)\\DOSBox-0.74\\DOSBox.exe",
            "C:\\Program Files\\DOSBox-0.74\\DOSBox.exe",
            "/usr/bin/dosbox",
            "/usr/local/bin/dosbox",
            "dosbox"
        ]
        
        for path in possible_paths:
            if shutil.which(path) or os.path.exists(path):
                print(f"DOSBox encontrado en: {path}")
                return path
        
        print("Error: No se encontró DOSBox en ninguna ubicación")
        return None
    
    def find_tasm(self):
        """Busca TASM en las rutas comunes"""
        possible_paths = [
            "C:\\TASM",
            "C:\\Program Files (x86)\\TASM",
            "C:\\Program Files\\TASM",
            os.path.join(os.getcwd(), "TASM"),
            os.path.join(os.path.expanduser("~"), "TASM")
        ]
        
        for path in possible_paths:
            if os.path.exists(path):
                print(f"TASM encontrado en: {path}")
                return path
        
        print("Error: TASM no encontrado en las rutas comunes")
        return None
    
    def copy_tasm_to_workdir(self):
        """Copia los archivos esenciales de TASM al directorio de trabajo"""
        if not self.tasm_path:
            return False
        
        try:
            # Copiar todos los archivos de TASM
            for item in os.listdir(self.tasm_path):
                source = os.path.join(self.tasm_path, item)
                destination = os.path.join(self.work_dir, item)
                
                if os.path.isfile(source):
                    shutil.copy2(source, destination)
                elif os.path.isdir(source):
                    shutil.copytree(source, destination)
            
            print(f"Archivos de TASM copiados a: {self.work_dir}")
            return True
            
        except Exception as e:
            print(f"Error copiando archivos de TASM: {e}")
            return False
    
    def create_dosbox_config(self, asm_file, debug_mode=False):
        """Crea un archivo de configuración temporal para DOSBox"""
        if debug_mode:
            # Configuración para modo depuración
            config_content = f"""
[autoexec]
# Montar directorio de trabajo como unidad C:
mount c {self.work_dir}
c:
        
# Ensamblar y enlazar con información de depuración
echo Ensamblando {asm_file} con información de depuración...
tasm /zi {asm_file}
        
echo Enlazando con información de depuración...
tlink /v {asm_file.replace('.asm', '.obj')}

echo.
echo Iniciando Turbo Debugger...
echo.
echo Comandos básicos de Turbo Debugger:
echo - F7: Step into
echo - F8: Step over  
echo - F9: Run
echo - F10: Menu
echo - Alt+F5: User screen
echo - Ctrl+F2: Reset program
echo - Alt+X: Salir
echo.
echo Presiona una tecla para iniciar Turbo Debugger...
pause
td {asm_file.replace('.asm', '.exe')}
        
exit
"""
        else:
            # Configuración normal
            config_content = f"""
[autoexec]
# Montar directorio de trabajo como unidad C:
mount c {self.work_dir}
c:
        
# Ensamblar y enlazar
echo Ensamblando {asm_file}...
tasm /zi {asm_file}
        
echo Enlazando...
tlink /v {asm_file.replace('.asm', '.obj')}

pause

echo Ejecutando programa...
{asm_file.replace('.asm', '.exe')}
        
echo.
echo Presiona una tecla para continuar...
pause
exit
"""
        config_path = os.path.join(self.work_dir, "dosbox.conf")
        with open(config_path, 'w', encoding='utf-8') as f:
            f.write(config_content)
        return config_path

    def create_batch_file(self, asm_file, debug_mode=False):
        """Crea un archivo batch para mayor control"""
        if debug_mode:
            batch_content = f"""@echo off
echo ===============================
echo    MODO DEPURACION - TURBO DEBUGGER
echo ===============================
echo.
echo Ensamblando con información de depuración...
tasm /zi {asm_file}
if errorlevel 1 goto error

echo Enlazando con información de depuración...
tlink /v {asm_file.replace('.asm', '.obj')}
if errorlevel 1 goto error

echo.
echo Iniciando Turbo Debugger...
echo.
echo Comandos útiles:
echo F7  - Step into
echo F8  - Step over
echo F9  - Run
echo F10 - Menu
echo Alt+F5 - Ver pantalla de usuario
echo Ctrl+F2 - Reiniciar programa
echo Alt+X - Salir
echo.
pause
td {asm_file.replace('.asm', '.exe')}
goto end

:error
echo ERROR en el proceso!
pause
exit 1

:end
echo Depuración finalizada.
"""
        else:
            batch_content = f"""@echo off
echo ===============================
echo    MODO EJECUCION NORMAL
echo ===============================
echo.
echo Ensamblando...
tasm /zi {asm_file}
if errorlevel 1 goto error

echo Enlazando...
tlink /v {asm_file.replace('.asm', '.obj')}
if errorlevel 1 goto error

echo Ejecutando programa...
echo.
{asm_file.replace('.asm', '.exe')}
echo.
echo Programa ejecutado. Presiona una tecla...
pause
goto end

:error
echo ERROR en el proceso!
pause
exit 1

:end
"""
        batch_path = os.path.join(self.work_dir, "run.bat")
        with open(batch_path, 'w', encoding='utf-8') as f:
            f.write(batch_content)
        return batch_path

    def create_dosbox_config_with_batch(self, batch_file):
        """Configuración simple que ejecuta el batch file"""
        config_content = f"""
[autoexec]
mount c {self.work_dir}
c:
{batch_file}
"""
        config_path = os.path.join(self.work_dir, "dosbox.conf")
        with open(config_path, 'w', encoding='utf-8') as f:
            f.write(config_content)
        return config_path
    
    def copy_asm_files_to_workdir(self, asm_file):
        """Copia los archivos ASM y relacionados al directorio de trabajo"""
        asm_path = Path(asm_file)
        work_asm = os.path.join(self.work_dir, asm_path.name)
        shutil.copy2(asm_file, work_asm)
        
        # Copiar archivos incluidos (.inc) desde el mismo directorio
        source_dir = asm_path.parent
        for file in source_dir.glob("*"):
            if file.suffix.lower() in ['.inc', '.h', '.mac']:
                shutil.copy2(file, self.work_dir)
        
        print(f"Archivos de programa copiados a: {self.work_dir}")
        return work_asm
    
    def assemble_and_run(self, asm_file, debug=False, debug_mode=False):
        """Ensambla, enlaza y ejecuta el código
        debug: muestra salida de consola
        debug_mode: usa Turbo Debugger en lugar de ejecutar directamente
        """
        if not self.dosbox_path:
            print("Error: No se encontró DOSBox")
            return False
        
        if not self.tasm_path:
            print("Error: No se encontró TASM")
            return False
        
        try:
            # Copiar archivos de TASM al directorio temporal
            if not self.copy_tasm_to_workdir():
                print("Error: No se pudieron copiar los archivos de TASM")
                return False
            
            # Copiar archivos del programa
            work_asm = self.copy_asm_files_to_workdir(asm_file)
            
            # Crear batch file y configuración
            batch_file = self.create_batch_file(os.path.basename(work_asm), debug_mode)
            config_path = self.create_dosbox_config_with_batch(os.path.basename(batch_file))
            
            # Comando para ejecutar DOSBox
            cmd = [self.dosbox_path, "-conf", config_path]
            
            if not debug:
                cmd.append("-noconsole")
            
            print("=" * 50)
            if debug_mode:
                print("🔍 INICIANDO MODO DEPURACIÓN")
                print("Se abrirá Turbo Debugger automáticamente")
            else:
                print("🚀 INICIANDO MODO EJECUCIÓN NORMAL")
            print(f"Archivo: {os.path.basename(asm_file)}")
            print(f"Directorio temporal: {self.work_dir}")
            print("=" * 50)
            
            if debug_mode:
                print("\n📝 COMANDOS BÁSICOS DE TURBO DEBUGGER:")
                print("F7  - Step into (entrar en procedimiento)")
                print("F8  - Step over (ejecutar línea)")
                print("F9  - Run (ejecutar hasta breakpoint)")
                print("F10 - Menu principal")
                print("Alt+F5  - Ver pantalla del programa")
                print("Ctrl+F2 - Reiniciar programa")
                print("Alt+X   - Salir del debugger")
                print("\nPresiona Ctrl+C en esta ventana para cancelar...")
            
            # Ejecutar DOSBox
            result = subprocess.run(cmd, capture_output=not debug, text=True, encoding='latin-1')
            
            if debug:
                if result.stdout:
                    print("Salida:", result.stdout)
                if result.stderr:
                    print("Errores:", result.stderr)
            
            # Verificar si se generaron los archivos
            obj_file = os.path.join(self.work_dir, asm_file.replace('.asm', '.obj'))
            exe_file = os.path.join(self.work_dir, asm_file.replace('.asm', '.exe'))
            
            if os.path.exists(exe_file):
                print("✓ Archivo .EXE generado exitosamente!")
                return True
            elif os.path.exists(obj_file):
                print("✓ Archivo .OBJ generado, pero falló el enlazado")
                return False
            else:
                print("✗ Fallo en el ensamblado")
                return False
                
        except Exception as e:
            print(f"Error ejecutando DOSBox: {e}")
            return False
    
    def clean_up(self):
        """Limpia los archivos temporales"""
        try:
            shutil.rmtree(self.work_dir)
            print(f"Directorio temporal limpiado: {self.work_dir}")
        except Exception as e:
            print(f"Error limpiando directorio: {e}")

def print_help():
    print("Asistente para ensamblador 8086/8088 con DOSBox y TASM")
    print("Uso: python tasm_assistant.py <archivo.asm> [opciones]")
    print("\nOpciones:")
    print("  --debug          : Mostrar salida detallada de DOSBox")
    print("  --debug-mode     : Ejecutar en Turbo Debugger (TD)")
    print("  --help           : Mostrar esta ayuda")
    print("\nEjemplos:")
    print("  python tasm_assistant.py programa.asm          # Ejecución normal")
    print("  python tasm_assistant.py programa.asm --debug  # Con salida detallada")
    print("  python tasm_assistant.py programa.asm --debug-mode  # Con Turbo Debugger")

def main():
    if len(sys.argv) < 2 or "--help" in sys.argv:
        print_help()
        sys.exit(1)
    
    asm_file = sys.argv[1]
    debug = "--debug" in sys.argv
    debug_mode = "--debug-mode" in sys.argv
    
    if not os.path.exists(asm_file):
        print(f"Error: El archivo {asm_file} no existe")
        sys.exit(1)
    
    assistant = AssemblerAssistant()
    
    try:
        success = assistant.assemble_and_run(asm_file, debug, debug_mode)
        if success:
            if debug_mode:
                print("🔍 Sesión de depuración finalizada")
            else:
                print("🎉 Proceso completado con éxito!")
        else:
            print("❌ Hubo errores durante el proceso")
    except KeyboardInterrupt:
        print("\n⏹️  Proceso interrumpido por el usuario")
    finally:
        assistant.clean_up()

if __name__ == "__main__":
    main()