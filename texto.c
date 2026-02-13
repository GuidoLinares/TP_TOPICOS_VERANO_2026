#include "texto.h"
#include <stdio.h>

int inicializar_ttf() {
    if (TTF_Init() < 0) {
        return 0;
    }
    return 1;
}

TTF_Font* cargar_font(const char *path, uint32_t fontSize)
{
    TTF_Font *font = TTF_OpenFont(path, fontSize);
    if (!font)
    {
        fprintf(stderr, "No se pudo cargar la fuente \"%s\" %s\n", path, TTF_GetError());
    }

    return font;
}

void cerrar_font(TTF_Font *fuente)
{
    TTF_CloseFont(fuente);
}

void finalizar_ttf(void)
{
    TTF_Quit();
}