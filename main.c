/*
=================================================
TRABAJO PRACTICO - TOPICOS DE PROGRAMACION
Tercer Cuatrimestre 2025 - Curso de Verano
Grupo: OMEGA
=================================================

Apellido: LINARES, GUIDO HERNAN
DNI: 43170056
Entrega: Si

Apellido: LANARI VARAMO, LARA VICTORIA
DNI: 44263607
Entrega: Si

Apellido: BRUNO, SANTINO
DNI: 44392059
Entrega: No

Apellido: CONDORI CHUMACERO, DANIEL JESUS
DNI: 95488949
Entrega: No

=================================================
*/

#include <SDL.h>
#include <stdio.h>
#include "menu.h"
#include "config.h"
#include "texto.h"
#include "grafico.h"
#include "juego.h"
#include "sonido.h"

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
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

    if (!inicializar_audio())
    {
        printf("Advertencia: Audio no disponible\n");
    }

    inicializar_fonts();
    EstadoMenu estado_menu;
    s_EstadoJuego estado_juego = {0};
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
                    finalizar_juego(&estado_juego);
                    estado_menu.pantalla_actual = PANTALLA_MENU;
                }
                else
                {
                    ejecutando = 0;
                }
            }
            switch (estado_menu.pantalla_actual)
            {
            case PANTALLA_MENU:
                procesar_menu_principal(renderer, &evento, &estado_menu, mouseX, mouseY);
                break;
            case PANTALLA_CONFIG:
                procesar_menu_config(renderer, &evento, &estado_menu, mouseX, mouseY);
                break;
            case PANTALLA_INGRESO_NOMBRES:
                procesar_ingreso_nombres(&evento, &estado_menu);
                break;
            case PANTALLA_JUEGO:
                procesar_eventos_juego(&estado_juego, &evento, mouseX, mouseY, &estado_menu);
                break;
            case PANTALLA_STATS:
                procesar_menu_stats(renderer,&evento,&estado_menu,mouseX, mouseY);
                break;
            case PANTALLA_SALIR:
                ejecutando = 0;                
                break;
            }
        }

        if (estado_menu.pantalla_actual == PANTALLA_JUEGO)
        {
            if (!estado_juego.juego_iniciado)
            {
                estado_juego.config = estado_menu.config;
                iniciar_juego(&estado_juego, &estado_menu, renderer);
            }
            actualizar_juego(&estado_juego, &estado_menu);
        }

        SDL_RenderClear(renderer);

        switch (estado_menu.pantalla_actual)
        {
        case PANTALLA_MENU:
            dibujar_menu_principal(renderer, &estado_menu, mouseX, mouseY);
            break;
        case PANTALLA_CONFIG:
            dibujar_menu_config(renderer, &estado_menu, mouseX, mouseY);
            break;
        case PANTALLA_INGRESO_NOMBRES:
            dibujar_ingreso_nombres(renderer, &estado_menu);
            break;
        case PANTALLA_JUEGO:
            dibujar_juego(renderer, &estado_juego, mouseX, mouseY);
            break;
        case PANTALLA_STATS:
            dibujar_menu_stats(renderer, &estado_menu, mouseX, mouseY);
            break;
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    cerrar_audio();
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);
    SDL_Quit();
    finalizar_ttf();
    liberar_menu();

    return 0;
}