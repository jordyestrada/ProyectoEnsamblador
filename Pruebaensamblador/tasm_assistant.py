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
    
    def create_dosbox_config(self, asm_file):
        """Crea un archivo de configuración temporal para DOSBox"""
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
    
    def assemble_and_run(self, asm_file, debug=False):
        """Ensambla, enlaza y ejecuta el código"""
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
            
            # Crear configuración de DOSBox
            config_path = self.create_dosbox_config(os.path.basename(work_asm))
            
            # Comando para ejecutar DOSBox
            cmd = [self.dosbox_path, "-conf", config_path]
            
            if not debug:
                cmd.append("-noconsole")
            
            print("=" * 50)
            print(f"Iniciando proceso de ensamblado...")
            print(f"Archivo: {os.path.basename(asm_file)}")
            print(f"Directorio temporal: {self.work_dir}")
            print("=" * 50)
            
            # Ejecutar DOSBox
            result = subprocess.run(cmd, capture_output=not debug, text=True, encoding='latin-1')
            
            if debug:
                if result.stdout:
                    print("Salida:", result.stdout)
                if result.stderr:
                    print("Errores:", result.stderr)
            
            if result.returncode == 0:
                print("✓ Proceso completado exitosamente")
                return True
            else:
                print("✗ Error en el proceso (código de retorno:", result.returncode, ")")
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

def main():
    if len(sys.argv) < 2:
        print("Uso: python asm_assistant.py <archivo.asm> [--debug]")
        print("Ejemplo: python asm_assistant.py programa.asm --debug")
        sys.exit(1)
    
    asm_file = sys.argv[1]
    debug_mode = "--debug" in sys.argv
    
    if not os.path.exists(asm_file):
        print(f"Error: El archivo {asm_file} no existe")
        sys.exit(1)
    
    assistant = AssemblerAssistant()
    
    try:
        success = assistant.assemble_and_run(asm_file, debug_mode)
        if success:
            print("🎉 Proceso completado con éxito!")
        else:
            print("❌ Hubo errores durante el proceso")
    finally:
        assistant.clean_up()

if __name__ == "__main__":
    main()