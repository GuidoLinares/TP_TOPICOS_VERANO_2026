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

SDL_Texture *cargar_textura(SDL_Renderer *renderer, const char *imagen)
{
    const char *extensiones[] = {".png", ".jpg", ".jpeg", ".bmp"};
    char ruta_completa[256];

    for (int i = 0; i < 4; i++)
    {
        sprintf(ruta_completa, "%s%s", imagen, extensiones[i]);
        SDL_Surface *surface = IMG_Load(ruta_completa);

        if (surface != NULL)
        {
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);

            if (texture != NULL)
            {
                return texture;
            }
        }
    }

    printf("Error al cargar imagen '%s': %s\n", ruta_completa, IMG_GetError());
    return NULL;
}
