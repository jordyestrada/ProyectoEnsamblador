@echo off
cd /d %~dp0
echo Compilando juego.asm...
C:\TASM\TASM.EXE /zi juego.asm > compile_output.txt 2>&1
type compile_output.txt
pause
