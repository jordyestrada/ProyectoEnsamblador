@echo off
echo Compilando juego.asm...
C:\TASM\TASM.EXE /zi juego.asm
if errorlevel 1 goto error
echo.
echo Enlazando...
C:\TASM\TLINK.EXE /v juego.obj
if errorlevel 1 goto error
echo.
echo Compilacion exitosa!
goto end

:error
echo.
echo ERROR en la compilacion!
pause

:end
pause
