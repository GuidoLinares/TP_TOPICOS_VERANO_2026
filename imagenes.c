#include <SDL.h>
#include <SDL_image.h>
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

SDL_Texture *cargar_textura(SDL_Renderer *renderer, const char *ruta)
{
    SDL_Surface *surface = IMG_Load(ruta);
    if (surface == NULL)
    {
        printf("Error al cargar imagen '%s': %s\n", ruta, IMG_GetError());
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (texture == NULL)
    {
        return NULL;
    }

    return texture;
}
