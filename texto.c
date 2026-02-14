#include "texto.h"
#include <stdio.h>

static TTF_Font *fuente_grande = NULL;
static TTF_Font *fuente_mediana = NULL;
static TTF_Font *fuente_pequena = NULL;

int inicializar_ttf()
{
    if (TTF_Init() < 0)
    {
        return 0;
    }
    return 1;
}

TTF_Font *cargar_font(const char *path, uint32_t fontSize)
{
    TTF_Font *font = TTF_OpenFont(path, fontSize);
    if (!font)
    {
        fprintf(stderr, "No se pudo cargar la fuente \"%s\" %s\n", path, TTF_GetError());
    }

    return font;
}

void inicializar_fonts()
{
    fuente_grande = cargar_font("fnt/arial.ttf", 32);
    fuente_mediana = cargar_font("fnt/arial.ttf", 24);
    fuente_pequena = cargar_font("fnt/arial.ttf", 16);
}

void dibujar_texto_ttf(SDL_Renderer *renderer, const char *texto, int x, int y, int tamanio, SDL_Color color)
{
    TTF_Font *fuente = fuente_mediana;

    if (tamanio >= 32)
        fuente = fuente_grande;
    else if (tamanio >= 24)
        fuente = fuente_mediana;
    else
        fuente = fuente_pequena;

    if (!fuente)
        return;

    SDL_Surface *superficie = TTF_RenderText_Blended(fuente, texto, color);
    if (!superficie)
        return;

    SDL_Texture *textura = SDL_CreateTextureFromSurface(renderer, superficie);

    int texto_ancho = superficie->w;
    int texto_alto = superficie->h;

    SDL_Rect destino = {
        x - texto_ancho / 2,
        y - texto_alto / 2,
        texto_ancho,
        texto_alto};

    SDL_RenderCopy(renderer, textura, NULL, &destino);

    SDL_DestroyTexture(textura);
    SDL_FreeSurface(superficie);
}

void cerrar_font(TTF_Font *fuente)
{
    TTF_CloseFont(fuente);
}

void finalizar_ttf(void)
{
    TTF_Quit();
}