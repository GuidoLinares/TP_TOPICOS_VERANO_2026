#include <SDL.h>
#include <stdio.h>
#include "menu.h"
#include "config.h"
#include "texto.h"
#include "imagenes.h"
#include "grafico.h"

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Error al inicializar SDL %s \n", SDL_GetError());
        return -1;
    }

    if (!imagenes_inicializar())
    {
        printf("No se pudieron inicializar formatos de imagen\n");
    }

    SDL_Window *ventana = SDL_CreateWindow(
        "Memotest - OMEGA",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        VENTANA_ANCHO,
        VENTANA_ALTO,
        SDL_WINDOW_SHOWN);

    if (ventana == NULL)
    {
        printf("Error al crear la ventana %s\n ", SDL_GetError());
        SDL_Quit();
        IMG_Quit();
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL)
    {
        printf("No se pudo crear el renderer %s\n", SDL_GetError());
        SDL_DestroyWindow(ventana);
        SDL_Quit();
        IMG_Quit();
        return 1;
    }

    if (!inicializar_ttf())
    {
        printf("Error al inicializar TTF:%s\n", TTF_GetError());

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(ventana);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    inicializar_fonts();
    EstadoMenu estado_menu;
    inicializar_menu(&estado_menu);
    cargar_recursos_menu(renderer);

    int ejecutando = 1;
    SDL_Event evento;
    int mouseX, mouseY;

    while (ejecutando)
    {
        SDL_GetMouseState(&mouseX, &mouseY);

        while (SDL_PollEvent(&evento))
        {
            if (evento.type == SDL_QUIT)
            {
                ejecutando = 0;
            }

            if (evento.type == SDL_KEYDOWN && evento.key.keysym.sym == SDLK_ESCAPE)
            {
                if (estado_menu.pantalla_actual == PANTALLA_JUEGO)
                {
                    estado_menu.pantalla_actual = PANTALLA_MENU;
                }
                else
                {
                    ejecutando = 0;
                }
            }

            if (estado_menu.pantalla_actual == PANTALLA_MENU)
            {
                procesar_menu_principal(renderer, &evento, &estado_menu, mouseX, mouseY);
            }
            else if (estado_menu.pantalla_actual == PANTALLA_CONFIG)
            {
                procesar_menu_config(renderer, &evento, &estado_menu, mouseX, mouseY);
            }
        }

        if (estado_menu.pantalla_actual == PANTALLA_SALIR)
        {
            ejecutando = 0;
        }

        if (estado_menu.pantalla_actual == PANTALLA_MENU)
        {
            dibujar_menu_principal(renderer, &estado_menu, mouseX, mouseY);
        }
        else if (estado_menu.pantalla_actual == PANTALLA_CONFIG)
        {
            dibujar_menu_config(renderer, &estado_menu, mouseX, mouseY);
        }
        else if (estado_menu.pantalla_actual == PANTALLA_JUEGO)
        {
            TDAVec *tablero = crear_tablero(estado_menu.config.filas, estado_menu.config.columnas);

            if (tablero != NULL)
            {
                s_Jugador jugador = {
                    .nombre = "JUGADOR1",
                    .puntos = 0,
                    .aciertos = 0,
                    .racha = 0,
                    .fallos = 0,
                    .turno_activo = 1};

                int carta_seleccionada1 = -1;
                int carta_seleccionada2 = -1;
                int estado_juego = ESTADO_ESPERANDO_PRIMERA;
                int carta_hover = -1;

                int cantidad_pares = (estado_menu.config.filas * estado_menu.config.columnas) / 2;
                SDL_Texture *texturas_cartas[10];

                for (int i = 0; i < cantidad_pares; i++)
                {
                    char ruta[100];
                    sprintf(ruta, "img/%d.png", i + 1);
                    texturas_cartas[i] = cargar_textura(renderer, ruta);
                }

                SDL_Texture *textura_dorso = cargar_textura(renderer, "img/20.png");

                int jugando = 1;
                while (jugando && ejecutando)
                {
                    SDL_GetMouseState(&mouseX, &mouseY);

                    while (SDL_PollEvent(&evento))
                    {
                        if (evento.type == SDL_QUIT)
                        {
                            ejecutando = 0;
                            jugando = 0;
                        }

                        if (evento.type == SDL_KEYDOWN && evento.key.keysym.sym == SDLK_ESCAPE)
                        {
                            jugando = 0;
                        }

                        if (evento.type == SDL_MOUSEBUTTONDOWN)
                        {
                            if (detectar_boton_menu(mouseX, mouseY))
                            {
                                jugando = 0;
                                continue;
                            }

                            int indice = convertir_clic_a_indice(mouseX, mouseY, estado_menu.config.filas, estado_menu.config.columnas);

                            if (indice != -1)
                            {
                                s_Carta *carta = (s_Carta *)obtenerVec(tablero, indice);

                                if (carta->estado == CARTA_OCULTA)
                                {
                                    if (estado_juego == ESTADO_ESPERANDO_PRIMERA)
                                    {
                                        carta_seleccionada1 = indice;
                                        carta->estado = CARTA_VISIBLE;
                                        estado_juego = ESTADO_ESPERANDO_SEGUNDA;
                                    }
                                    else if (estado_juego == ESTADO_ESPERANDO_SEGUNDA)
                                    {
                                        carta_seleccionada2 = indice;
                                        carta->estado = CARTA_VISIBLE;
                                        estado_juego = ESTADOS_COMPARANDO;
                                    }
                                }
                            }
                        }
                    }

                    if (evento.type == SDL_MOUSEBUTTONDOWN)
                    {
                        if (detectar_boton_menu(mouseX, mouseY))
                        {
                            jugando = 0;
                            continue;
                        }

                        int indice = convertir_clic_a_indice(mouseX, mouseY, estado_menu.config.filas, estado_menu.config.columnas);
                    }

                    if (estado_juego == ESTADOS_COMPARANDO)
                    {
                        SDL_Delay(800);

                        s_Carta *c1 = (s_Carta *)obtenerVec(tablero, carta_seleccionada1);
                        s_Carta *c2 = (s_Carta *)obtenerVec(tablero, carta_seleccionada2);

                        if (comparar_cartas(c1, c2))
                        {
                            procesar_acierto(&jugador, c1, c2);

                            if (verificar_fin_juego(tablero))
                            {
                                Estadistica stats;
                                strcpy(stats.nombre, jugador.nombre);
                                stats.puntos = jugador.puntos;
                                stats.aciertos = jugador.aciertos;
                                stats.fallos = jugador.fallos;
                                strcpy(stats.fecha, "2026-02-10");
                                guardar_estadistica(&stats);

                                SDL_Delay(2000);
                                jugando = 0;
                            }
                        }
                        else
                        {
                            procesar_fallo(&jugador, c1, c2);
                        }

                        carta_seleccionada1 = -1;
                        carta_seleccionada2 = -1;
                        estado_juego = ESTADO_ESPERANDO_PRIMERA;
                    }

                    SDL_SetRenderDrawColor(renderer, COLOR_FONDO_R, COLOR_FONDO_G, COLOR_FONDO_B, 255);
                    SDL_RenderClear(renderer);

                    carta_hover = convertir_clic_a_indice(mouseX, mouseY, estado_menu.config.filas, estado_menu.config.columnas);
                    dibujar_tablero(renderer, tablero, estado_menu.config.filas, estado_menu.config.columnas, textura_dorso, texturas_cartas, carta_hover);
                    dibujar_hud_juego(renderer, &jugador, mouseX, mouseY);

                    SDL_RenderPresent(renderer);
                    SDL_Delay(16);
                }

                for (int i = 0; i < cantidad_pares; i++)
                {
                    if (texturas_cartas[i] != NULL)
                    {
                        SDL_DestroyTexture(texturas_cartas[i]);
                    }
                }
                SDL_DestroyTexture(textura_dorso);
                destruirVec(tablero);
                free(tablero);

                estado_menu.pantalla_actual = PANTALLA_MENU;
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);
    SDL_Quit();
    finalizar_ttf();
    liberar_menu();

    return 0;
}