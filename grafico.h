#ifndef GRAFICO_H
#define GRAFICO_H

#include <SDL.h>
#include <stdio.h>
#include "imagenes.h"
#include "texto.h"
#include "menu.h"
#include "juego.h"


#define COLOR_FONDO_R 40
#define COLOR_FONDO_G 40
#define COLOR_FONDO_B 60

#define COLOR_CARTA_OCULTA_R 50
#define COLOR_CARTA_OCULTA_G 100
#define COLOR_CARTA_OCULTA_B 200

typedef struct
{
    int x;
    int y;
} Posicion;

Posicion calcular_posicion_carta(int indice, int filas, int columnas);
void dibujar_carta(SDL_Renderer *renderer, s_Carta *carta, int x, int y,
                   SDL_Texture *textura_dorso, SDL_Texture **texturas_cartas, int);
void dibujar_tablero(SDL_Renderer *renderer, TDAVec *tablero, int filas, int columnas,
                     SDL_Texture *textura_dorso, SDL_Texture **texturas_cartas, int carta_hover);

void dibujar_juego(SDL_Renderer *renderer, s_EstadoJuego *estado_juego, int mouseX, int mouseY);
void dibujar_hud_juego(SDL_Renderer *renderer, s_Jugador jugador, int mouseX, int mouseY);

#endif