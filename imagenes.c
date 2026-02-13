#include <SDL2/SDL.h>
#include <stdio.h>
#include "imagenes.h"

tFormatoImg imagenes_inicializar(void)
{
    tFormatoImg formatos = IMAGEN_BMP;

    int32_t formatosAll = IMG_INIT_JPG | IMG_INIT_PNG;
    int32_t init = IMG_Init(formatosAll);

    if (!(init & IMG_INIT_JPG))
        fprintf(stderr, "Error inicializando JPG: %s\n", IMG_GetError());

    if (!(init & IMG_INIT_PNG))
        fprintf(stderr, "Error inicializando PNG: %s\n", IMG_GetError());

    if (init & IMG_INIT_JPG)
        formatos |= IMAGEN_JPG;

    if (init & IMG_INIT_PNG)
        formatos |= IMAGEN_PNG;

    return formatos;
}
