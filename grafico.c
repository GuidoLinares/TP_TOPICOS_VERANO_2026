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
    SDL_Color gris = {30, 35, 45, 255};
    SDL_Color naranja = {255, 140, 0, 255};

    int ancho_p = 210;
    int alto_p = 58;
    int margen = 20;

    int x_menu = 340;
    int y_menu = 750;

    SDL_Color c1 = (estado_juego->jugador_actual == 1) ? naranja : gris;
    dibujar_rectangulo_relleno(renderer, margen, margen, ancho_p, alto_p, c1.r, c1.g, c1.b);

    if (estado_juego->jugador_actual == 1)
    {
        SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
        SDL_Rect r1 = {margen - 2, margen - 2, ancho_p + 4, alto_p + 4};
        SDL_RenderDrawRect(renderer, &r1);
    }

    char nombre1[60], puntos1[50];
    sprintf(nombre1, "%s", estado_juego->jugador1.nombre);
    sprintf(puntos1, "SCORE_VALUE: %d", estado_juego->jugador1.puntos);
    dibujar_texto_ttf(renderer, nombre1, margen + ancho_p / 2, margen + 18, 16, blanco);
    dibujar_texto_ttf(renderer, puntos1, margen + ancho_p / 2, margen + 40, 14, blanco);

    if (estado_juego->config.modo_jugadores == 2)
    {
        s_Jugador *jugador_en_turno = (estado_juego->jugador_actual == 1) ? &estado_juego->jugador1 : &estado_juego->jugador2;

        dibujar_texto_ttf(renderer, "ACTIVE_THREAD", 400, 40, 18, amarillo);
        dibujar_texto_ttf(renderer, jugador_en_turno->nombre, 400, 65, 28, amarillo);

        int x2 = 800 - ancho_p - margen;
        SDL_Color c2 = (estado_juego->jugador_actual == 2) ? naranja : gris;
        dibujar_rectangulo_relleno(renderer, x2, margen, ancho_p, alto_p, c2.r, c2.g, c2.b);

        if (estado_juego->jugador_actual == 2)
        {
            SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
            SDL_Rect r2 = {x2 - 2, margen - 2, ancho_p + 4, alto_p + 4};
            SDL_RenderDrawRect(renderer, &r2);
        }

        char nombre2[60], puntos2[50];
        sprintf(nombre2, "%s", estado_juego->jugador2.nombre);
        sprintf(puntos2, "SCORE_VALUE: %d", estado_juego->jugador2.puntos);
        dibujar_texto_ttf(renderer, nombre2, x2 + ancho_p / 2, margen + 18, 16, blanco);
        dibujar_texto_ttf(renderer, puntos2, x2 + ancho_p / 2, margen + 40, 14, blanco);
    }

    int hover_menu = detectar_boton_menu(mouseX, mouseY);
    dibujar_rectangulo_relleno(renderer, x_menu, y_menu, 120, 40,
                               hover_menu ? 100 : 70,
                               hover_menu ? 80 : 60,
                               hover_menu ? 80 : 60);
    dibujar_texto_ttf(renderer, "MAIN_MENU", x_menu + 60, y_menu + 20, 16, hover_menu ? amarillo : blanco);

    char stats[100];
    s_Jugador *actual = (estado_juego->jugador_actual == 1) ? &estado_juego->jugador1 : &estado_juego->jugador2;

    sprintf(stats, "|  SUCCESS_STREAK: x%d  |  MATCH_SUCCESS: %d  |", actual->racha, actual->aciertos);
    dibujar_texto_ttf(renderer, stats, 400, 725, 16, amarillo);
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

    SDL_RenderClear(renderer);

    SDL_Texture *fondo_juego = NULL;
    fondo_juego = cargar_textura(renderer, "img/fondo_menu");
    if (fondo_juego)
        SDL_RenderCopy(renderer, fondo_juego, NULL, NULL);

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

void dibujar_fin_juego(SDL_Renderer *renderer, s_EstadoJuego *estado)
{
    char titulo[100];
    char detalle[100];
    SDL_Color blanco = {255, 255, 255, 255};
    SDL_Color naranja = {255, 140, 0, 255};

    int aciertos_j1 = estado->jugador1.aciertos;
    int aciertos_j2 = estado->jugador2.aciertos;

    if (!estado->modo_competitivo)
    {
        snprintf(titulo, sizeof(titulo), "PROCESS_FINISHED");
        snprintf(detalle, sizeof(detalle), "SCORE_VALUE: %d MATCH_SUCCESS", aciertos_j1);
    }
    else
    {
        if (aciertos_j1 > aciertos_j2)
            snprintf(titulo, sizeof(titulo), "PROCESS COMPLETED BY: %s", estado->jugador1.nombre);
        else if (aciertos_j2 > aciertos_j1)
            snprintf(titulo, sizeof(titulo), "PROCESS COMPLETED BY: %s", estado->jugador2.nombre);
        else
            snprintf(titulo, sizeof(titulo), "PARALLEL EXECUTION COMPLETE");

        snprintf(detalle, sizeof(detalle), "%d vs %d MATCH_SUCCESS", aciertos_j1, aciertos_j2);
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
    SDL_Rect pantalla = {0, 0, 800, 600};
    SDL_RenderFillRect(renderer, &pantalla);

    int ancho = 500;
    int alto = 300;
    int centro_x = 400;
    int centro_y = 300;

    SDL_Rect overlay = {
        400 - ancho / 2,
        300 - alto / 2,
        ancho,
        alto};

    SDL_SetRenderDrawColor(renderer, 30, 35, 45, 255);
    SDL_RenderFillRect(renderer, &overlay);

    SDL_SetRenderDrawColor(renderer, 255, 140, 0, 255);
    SDL_RenderDrawRect(renderer, &overlay);

    dibujar_texto_ttf(renderer, titulo, centro_x, centro_y - 80, 55, blanco);
    dibujar_texto_ttf(renderer, detalle, centro_x, centro_y, 22, blanco);
    dibujar_texto_ttf(renderer, "PRESS ENTER TO RETURN TO MAIN_MENU", centro_x, centro_y + 120, 18, naranja);

    SDL_RenderPresent(renderer);
}

void dibujar_presentacion(SDL_Renderer *renderer, EstadoMenu *estado)
{
    SDL_Color blanco = {255, 255, 255, 255};
    SDL_Color naranja = {255, 140, 0, 255};

    int centro = 400;

    Uint32 tiempo_transcurrido = SDL_GetTicks() - estado->inicio_presentacion;

    SDL_SetRenderDrawColor(renderer, 10, 10, 10, 150);
    SDL_RenderClear(renderer);

    dibujar_texto_ttf(renderer, "OMEGA MEMORY TEST", centro, centro - 150, 72, naranja);
    dibujar_texto_ttf(renderer, "--------------------------------------", centro, centro - 120, 20, blanco);

    if (tiempo_transcurrido > 500)
    {
        dibujar_texto_ttf(renderer, "> Activating memory challenge...", centro, centro - 40, 19, naranja);
    }
    if (tiempo_transcurrido > 1000)
    {
        dibujar_texto_ttf(renderer, "> Loading matrix...", centro, centro - 10, 19, naranja);
    }
    if (tiempo_transcurrido > 1500)
    {
        dibujar_texto_ttf(renderer, "> System ready.", centro, centro + 20, 19, naranja);
    }
    if (tiempo_transcurrido > 2000)
    {
        dibujar_texto_ttf(renderer, "Press ENTER to begin _", centro, centro + 150, 16, blanco);
        estado->final_presentacion = 1;
    }
}
