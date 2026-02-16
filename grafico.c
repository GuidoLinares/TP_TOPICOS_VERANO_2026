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

void dibujar_hud_juego(SDL_Renderer *renderer, s_EstadoJuego *estado_juego, int mouseX, int mouseY)
{
    SDL_Color blanco = {255, 255, 255, 255};
    SDL_Color amarillo = {255, 215, 0, 255};
    SDL_Color verde = {50, 255, 100, 255};
    SDL_Color rojo = {255, 80, 80, 255};
    SDL_Color azul_turno = {50, 120, 200, 255};

    int ancho_p = 180;
    int alto_p = 60;
    int margen = 20;

    int x_menu = 340;
    int y_menu = 680;

    SDL_Color c1 = (estado_juego->jugador_actual == 1) ? azul_turno : verde;
    dibujar_rectangulo_relleno(renderer, margen, margen, ancho_p, alto_p, c1.r, c1.g, c1.b);

    if (estado_juego->jugador_actual == 1)
    {
        SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
        SDL_Rect r1 = {margen - 2, margen - 2, ancho_p + 4, alto_p + 4};
        SDL_RenderDrawRect(renderer, &r1);
    }

    char nombre1[60], puntos1[50];
    sprintf(nombre1, "Jugador: %s", estado_juego->jugador1.nombre);
    sprintf(puntos1, "Puntos: %d", estado_juego->jugador1.puntos);
    dibujar_texto_ttf(renderer, nombre1, margen + ancho_p / 2, margen + 18, 16, blanco);
    dibujar_texto_ttf(renderer, puntos1, margen + ancho_p / 2, margen + 40, 14, amarillo);

    if (estado_juego->config.modo_jugadores == 2)
    {
        char msj_turno[100];
        s_Jugador *jugador_en_turno = (estado_juego->jugador_actual == 1) ? &estado_juego->jugador1 : &estado_juego->jugador2;
        sprintf(msj_turno, "TURNO DE: %s", jugador_en_turno->nombre);
        dibujar_texto_ttf(renderer, msj_turno, 400, 35, 24, amarillo);

        int x2 = 800 - ancho_p - margen;
        SDL_Color c2 = (estado_juego->jugador_actual == 2) ? azul_turno : verde;
        dibujar_rectangulo_relleno(renderer, x2, margen, ancho_p, alto_p, c2.r, c2.g, c2.b);

        if (estado_juego->jugador_actual == 2)
        {
            SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
            SDL_Rect r2 = {x2 - 2, margen - 2, ancho_p + 4, alto_p + 4};
            SDL_RenderDrawRect(renderer, &r2);
        }

        char nombre2[60], puntos2[50];
        sprintf(nombre2, "Jugador: %s", estado_juego->jugador2.nombre);
        sprintf(puntos2, "Puntos: %d", estado_juego->jugador2.puntos);
        dibujar_texto_ttf(renderer, nombre2, x2 + ancho_p / 2, margen + 18, 16, blanco);
        dibujar_texto_ttf(renderer, puntos2, x2 + ancho_p / 2, margen + 40, 14, amarillo);
    }

    int hover_menu = detectar_boton_menu(mouseX, mouseY);
    dibujar_rectangulo_relleno(renderer, x_menu, y_menu, 120, 40,
                               hover_menu ? 100 : 70,
                               hover_menu ? 80 : 60,
                               hover_menu ? 80 : 60);
    dibujar_texto_ttf(renderer, "MENU", x_menu + 60, y_menu + 20, 16, hover_menu ? amarillo : blanco);

    char stats[100];
    s_Jugador *actual = (estado_juego->jugador_actual == 1) ? &estado_juego->jugador1 : &estado_juego->jugador2;

    sprintf(stats, "Turno de %s  |  Racha: x%d  |  Aciertos: %d", actual->nombre, actual->racha, actual->aciertos);
    dibujar_texto_ttf(renderer, stats, 400, 650, 16, blanco);
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

    SDL_SetRenderDrawColor(renderer, 20, 80, 20, 255);
    SDL_RenderClear(renderer);

    int carta_hover = convertir_clic_a_indice(mouseX, mouseY, estado_juego->config.filas, estado_juego->config.columnas);

    dibujar_tablero(renderer,
                    estado_juego->tablero,
                    estado_juego->config.filas,
                    estado_juego->config.columnas,
                    estado_juego->textura_dorso,
                    estado_juego->textura_cartas,
                    carta_hover);

    dibujar_hud_juego(renderer,
                      estado_juego,
                      mouseX,
                      mouseY);
}
