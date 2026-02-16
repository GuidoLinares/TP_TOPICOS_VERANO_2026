@echo off

gcc -Wall -g main.c juego.c tda_vector.c menu.c texto.c config.c imagenes.c grafico.c sonido.c -o memotest.exe ^
-I./lib/SDL2-devel-2.32.8-mingw/SDL2-2.32.8/x86_64-w64-mingw32/include/SDL2 ^
-I./lib/SDL2_image-devel-2.8.8-mingw/SDL2_image-2.8.8/x86_64-w64-mingw32/include/SDL2 ^
-I./lib/SDL2_ttf-devel-2.24.0-mingw/SDL2_ttf-2.24.0/x86_64-w64-mingw32/include/SDL2 ^
-I./lib/SDL2_mixer-devel-2.8.1-mingw/SDL2_mixer-2.8.1/x86_64-w64-mingw32/include/SDL2 ^
-L./lib/SDL2-devel-2.32.8-mingw/SDL2-2.32.8/x86_64-w64-mingw32/lib ^
-L./lib/SDL2_image-devel-2.8.8-mingw/SDL2_image-2.8.8/x86_64-w64-mingw32/lib ^
-L./lib/SDL2_ttf-devel-2.24.0-mingw/SDL2_ttf-2.24.0/x86_64-w64-mingw32/lib ^
-L./lib/SDL2_mixer-devel-2.8.1-mingw/SDL2_mixer-2.8.1/x86_64-w64-mingw32/lib ^
-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

if %errorlevel% == 0 (
    echo Compilado exitosamente
    memotest.exe
) else (
    echo Error de compilacion
)

pause