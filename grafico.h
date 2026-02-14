#ifndef GRAFICO_H
#define GRAFICO_H

#include <SDL.h>
#include <stdio.h>
#include "juego.h"

#define VENTANA_ANCHO 750
#define VENTANA_ALTO 750

#define CARTA_ANCHO 100
#define CARTA_ALTO 140
#define ESPACIADO 20
#define MARGEN_SUPERIOR 80

#define COLOR_FONDO_R 40
#define COLOR_FONDO_G 40
#define COLOR_FONDO_B 60

#define COLOR_CARTA_OCULTA_R 50
#define COLOR_CARTA_OCULTA_G 100
#define COLOR_CARTA_OCULTA_B 200

typedef struct {
    int x;
    int y;
} Posicion;

Posicion calcular_posicion_carta(int indice, int filas, int columnas);
int convertir_clic_a_indice(int, int , int, int);
void dibujar_carta(SDL_Renderer* renderer, s_Carta* carta, int x, int y, 
                SDL_Texture* textura_dorso, SDL_Texture** texturas_cartas, int );
void dibujar_tablero(SDL_Renderer* renderer, TDAVec* tablero, int filas, int columnas, 
                    SDL_Texture* textura_dorso, SDL_Texture** texturas_cartas, int carta_hover);


#endif