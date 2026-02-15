

=================================================
MEMOTEST - OMEGA
Trabajo Practico - Topicos de Programacion
Tercer Cuatrimestre 2025
=================================================

INTEGRANTES:
- Linares, Guido Hernan (DNI: 43170056)
- Lanari Varamo, Lara Victoria (DNI: 44263607)
=================================================
REQUISITOS DEL SISTEMA:
=================================================

- Sistema Operativo: Windows 10 o superior
- Compilador: MinGW-w64 (GCC)
- IDE recomendado: Code::Blocks 20.03

=================================================
BIBLIOTECAS NECESARIAS:
=================================================

1. SDL2 (v2.32.8)
   Descarga: https://github.com/libsdl-org/SDL/releases/tag/release-2.32.8
   Archivo: SDL2-devel-2.32.8-mingw.zip

2. SDL2_image (v2.8.8)
   Descarga: https://github.com/libsdl-org/SDL_image/releases/tag/release-2.8.8
   Archivo: SDL2_image-devel-2.8.8-mingw.zip

3. SDL2_ttf (v2.24.0)
   Descarga: https://github.com/libsdl-org/SDL_ttf/releases/tag/release-2.24.0
   Archivo: SDL2_ttf-devel-2.24.0-mingw.zip

4. SDL2_mixer (v2.8.1) - OPCIONAL
   Descarga: https://github.com/libsdl-org/SDL_mixer/releases/tag/release-2.8.1
   Archivo: SDL2_mixer-devel-2.8.1-mingw.zip

=================================================
INSTALACION:
=================================================

1. Instalar MinGW-w64 y Code::Blocks

2. Descomprimir las bibliotecas en la carpeta /lib del proyecto

3. Estructura de carpetas:
   /doc       - Documentacion
   /fnt       - Fuentes tipograficas
   /img       - Imagenes del juego
   /snd       - Efectos de sonido
   /lib       - Bibliotecas SDL2

4. Copiar las DLLs a la raiz del proyecto:
   - SDL2.dll
   - SDL2_image.dll
   - SDL2_ttf.dll
   - SDL2_mixer.dll (opcional)

=================================================
COMPILACION:
=================================================

OPCION A - Usando el script:
   Ejecutar: compile_sdl.bat

OPCION B - Manual desde terminal:
   gcc main.c juego.c tda_vector.c menu.c config.c grafico.c imagenes.c texto.c ^
   -o memotest.exe ^
   -I./lib/SDL2-devel-2.32.8-mingw/SDL2-2.32.8/x86_64-w64-mingw32/include/SDL2 ^
   -I./lib/SDL2_image-devel-2.8.8-mingw/SDL2_image-2.8.8/x86_64-w64-mingw32/include/SDL2 ^
   -I./lib/SDL2_ttf-devel-2.24.0-mingw/SDL2_ttf-2.24.0/x86_64-w64-mingw32/include/SDL2 ^
   -L./lib/SDL2-devel-2.32.8-mingw/SDL2-2.32.8/x86_64-w64-mingw32/lib ^
   -L./lib/SDL2_image-devel-2.8.8-mingw/SDL2_image-2.8.8/x86_64-w64-mingw32/lib ^
   -L./lib/SDL2_ttf-devel-2.24.0-mingw/SDL2_ttf-2.24.0/x86_64-w64-mingw32/lib ^
   -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

=================================================
EJECUCION:
=================================================

1. Asegurarse que las DLLs esten en la misma carpeta que memotest.exe
2. Ejecutar: memotest.exe
3. O hacer doble clic en memotest.exe

=================================================
CONTROLES:
=================================================

- Mouse: Seleccionar cartas
- ESC: Volver al menu / Salir
- Clic en botones: Navegar menus

=================================================
ESTRUCTURA DE ARCHIVOS:
=================================================

Archivos de codigo (.c y .h):
- main.c / main.h
- juego.c / juego.h
- menu.c / menu.h
- config.c / config.h
- grafico.c / grafico.h
- texto.c / texto.h
- imagenes.c / imagenes.h
- tda_vector.c / tda_vector.h

Archivos de configuracion:
- config.dat (generado al guardar configuracion)
- stats.dat (generado al finalizar partidas)

=================================================
NOTAS ADICIONALES:
=================================================

- El juego guarda automaticamente las configuraciones
- Las estadisticas se almacenan por jugador
- Se requieren al menos 2 sets de imagenes diferentes
- Soporta modo 1 jugador y 2 jugadores competitivo