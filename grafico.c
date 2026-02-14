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

void dibujar_hud_juego(SDL_Renderer *renderer, s_Jugador jugador, int mouseX, int mouseY)
{
    SDL_Color blanco = {255, 255, 255, 255};
    SDL_Color amarillo = {255, 215, 0, 255};
    SDL_Color verde = {50, 255, 100, 255};
    SDL_Color rojo = {255, 80, 80, 255};

    int hover_menu = detectar_boton_menu(mouseX, mouseY);
    dibujar_rectangulo_relleno(renderer, 20, 20, 120, 40,
                               hover_menu ? 100 : 70,
                               hover_menu ? 80 : 60,
                               hover_menu ? 80 : 60);
    dibujar_texto_ttf(renderer, "MENU", 80, 40, 16, hover_menu ? amarillo : blanco);

    char nombre_texto[60];
    sprintf(nombre_texto, "Jugador: %s", jugador.nombre);
    dibujar_texto_ttf(renderer, nombre_texto, 400, 25, 20, blanco);

    char puntos_texto[50];
    sprintf(puntos_texto, "Puntos: %d", jugador.puntos);
    dibujar_rectangulo_relleno(renderer, 630, 15, 150, 35, 50, 80, 120);
    dibujar_texto_ttf(renderer, puntos_texto, 705, 32, 20, blanco);

    SDL_Color color_racha = blanco;
    if (jugador.racha >= 5)
        color_racha = rojo;
    else if (jugador.racha >= 3)
        color_racha = amarillo;
    else if (jugador.racha >= 1)
        color_racha = verde;

    char racha_texto[50];
    if (jugador.racha > 0)
    {
        sprintf(racha_texto, "Racha x%d", jugador.racha);
        dibujar_rectangulo_relleno(renderer, 630, 55, 150, 30, 40, 60, 90);
        dibujar_texto_ttf(renderer, racha_texto, 705, 70, 18, color_racha);
    }

    char stats_texto[100];
    sprintf(stats_texto, "Aciertos: %d  |  Fallos: %d", jugador.aciertos, jugador.fallos);
    dibujar_texto_ttf(renderer, stats_texto, 400, 760, 16, blanco);
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

void dibujar_juego(SDL_Renderer *renderer, s_EstadoJuego *estado_juego, int mouseX, int mouseY)
{
    if (!estado_juego || !estado_juego->tablero)
    {
        return;
    }

    int carta_hover = convertir_clic_a_indice(mouseX, mouseY, estado_juego->config.filas, estado_juego->config.columnas);

    dibujar_tablero(renderer,
                    estado_juego->tablero,
                    estado_juego->config.filas,
                    estado_juego->config.columnas,
                    estado_juego->textura_dorso,
                    estado_juego->textura_cartas,
                    carta_hover);

    if (estado_juego->turno_actual == 1)
    {
        dibujar_hud_juego(renderer,
                          estado_juego->jugador1,
                          mouseX,
                          mouseY);
    }
    else
    {
        dibujar_hud_juego(renderer,
                          estado_juego->jugador2,
                          mouseX,
                          mouseY);
    }

    if (estado_juego->modo_competitivo)
    {
        SDL_Color blanco = {255, 255, 255, 255};

        char turno_texto[50];
        sprintf(turno_texto, "Turno: %s",
                estado_juego->turno_actual == 1
                    ? estado_juego->jugador1.nombre
                    : estado_juego->jugador2.nombre);

        dibujar_texto_ttf(renderer,
                          turno_texto,
                          400,
                          60,
                          18,
                          blanco);
    }
}
