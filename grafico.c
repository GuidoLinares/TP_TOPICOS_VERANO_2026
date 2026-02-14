#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "grafico.h"

Posicion calcular_posicion_carta(int indice, int filas, int columnas)
{
    Posicion pos;

    int fila_carta = indice / columnas;
    int columna_carta = indice % columnas;

    int ancho_tablero = (columnas * CARTA_ANCHO) + ((columnas - 1) * ESPACIADO);
    int alto_tablero = (filas * CARTA_ALTO) + ((filas - 1) * ESPACIADO);

    int offset_x = (VENTANA_ANCHO - ancho_tablero) / 2;
    int offset_y = (VENTANA_ALTO - alto_tablero) / 2;

    pos.x = offset_x + (columna_carta * (CARTA_ANCHO + ESPACIADO));
    pos.y = offset_y + (fila_carta * (CARTA_ALTO + ESPACIADO));

    return pos;
}

int convertir_clic_a_indice(int mouseX, int mouseY, int filas, int columnas)
{
    int ancho_tablero = (columnas * CARTA_ANCHO) + ((columnas - 1) * ESPACIADO);
    int alto_tablero = (filas * CARTA_ALTO) + ((filas - 1) * ESPACIADO);
    int offset_x = (VENTANA_ANCHO - ancho_tablero) / 2;
    int offset_y = (VENTANA_ALTO - alto_tablero) / 2;

    int ajustado_x = mouseX - offset_x;
    int ajustado_y = mouseY - offset_y;

    if (ajustado_x < 0 || ajustado_y < 0)
        return -1;
    if (ajustado_x >= ancho_tablero || ajustado_y >= alto_tablero)
        return -1;

    int fila = ajustado_y / (CARTA_ALTO + ESPACIADO);
    int columna = ajustado_x / (CARTA_ANCHO + ESPACIADO);

    if (fila < 0 || fila >= filas)
        return -1;
    if (columna < 0 || columna >= columnas)
        return -1;

    int indice = (fila * columnas) + columna;

    return indice;
}

void dibujar_carta(SDL_Renderer *renderer, s_Carta *carta, int x, int y, SDL_Texture *textura_dorso, SDL_Texture **texturas_cartas, int es_hover)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = CARTA_ANCHO;
    rect.h = CARTA_ALTO;

    if (carta->estado == CARTA_OCULTA)
    {
        SDL_RenderCopy(renderer, textura_dorso, NULL, &rect);
    }
    else if (carta->estado == CARTA_VISIBLE || carta->estado == CARTA_ENCONTRADA)
    {
        SDL_RenderCopy(renderer, texturas_cartas[carta->id], NULL, &rect);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);

    if (es_hover && carta->estado == CARTA_OCULTA)
    {
        SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
        SDL_Rect borde_hover = {x - 2, y - 2, CARTA_ANCHO + 4, CARTA_ALTO + 4};
        SDL_RenderDrawRect(renderer, &borde_hover);
        SDL_Rect borde_hover2 = {x - 3, y - 3, CARTA_ANCHO + 6, CARTA_ALTO + 6};
        SDL_RenderDrawRect(renderer, &borde_hover2);
    }
}

void dibujar_tablero(SDL_Renderer *renderer, TDAVec *tablero, int filas, int columnas, SDL_Texture *textura_dorso, SDL_Texture **texturas_cartas, int carta_hover)
{
    for (int i = 0; i < longitudVec(tablero); i++)
    {
        s_Carta *carta = (s_Carta *)obtenerVec(tablero, i);
        Posicion pos = calcular_posicion_carta(i, filas, columnas);
        int es_hover = (i == carta_hover);
        dibujar_carta(renderer, carta, pos.x, pos.y, textura_dorso, texturas_cartas, es_hover);
    }
}