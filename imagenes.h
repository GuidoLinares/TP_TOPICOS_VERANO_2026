#ifndef IMAGENES_H
#define IMAGENES_H

#include <SDL.h>
#include <SDL_image.h>
#include <stdint.h>

typedef enum {
    IMAGEN_BMP  = 0x00,
    IMAGEN_JPG  = 0x01,
    IMAGEN_PNG  = 0x02,
}tFormatoImg;

tFormatoImg imagenes_inicializar(void);
SDL_Texture *cargar_textura(SDL_Renderer *renderer, const char *imagen);

#endif
