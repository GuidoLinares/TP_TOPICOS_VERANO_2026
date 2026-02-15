#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "config.h"


#define PANTALLA_MENU 0
#define PANTALLA_CONFIG 1 
#define PANTALLA_JUEGO 2
#define PANTALLA_STATS 3
#define PANTALLA_SALIR 4
#define PANTALLA_INGRESO_NOMBRES 5


#define BOTON_ANCHO 200
#define BOTON_ALTO 50

typedef struct
{
    int x;
    int y;
    int ancho;
    int alto;
    char texto[50];
    int activo;
} Boton;

typedef struct
{
    int pantalla_actual;
    Configuracion config;
    char nombre_jugador1[50];
    char nombre_jugador2[50];
    int jugador_escribiendo;
} EstadoMenu;

void inicializar_menu(EstadoMenu *menu);
void cargar_recursos_menu(SDL_Renderer *renderer);
int punto_en_rectangulo(int px, int py, int rx, int ry, int rw, int rh);
void dibujar_rectangulo_relleno(SDL_Renderer *renderer, int x, int y, int w, int h, int r, int g, int b);
void dibujar_menu_principal(SDL_Renderer *renderer, EstadoMenu *menu, int mouseX, int mouseY);
int procesar_menu_principal(SDL_Renderer *renderer, SDL_Event *evento, EstadoMenu *menu, int mouseX, int mouseY);
void dibujar_menu_config(SDL_Renderer *renderer, EstadoMenu *menu, int mouseX, int mouseY);
int procesar_menu_config(SDL_Renderer *renderer, SDL_Event *evento, EstadoMenu *menu, int mouseX, int mouseY);
int detectar_boton_menu(int mouseX, int mouseY);
void liberar_menu();
void procesar_ingreso_nombres(SDL_Event *evento, EstadoMenu *menu);
void dibujar_ingreso_nombres(SDL_Renderer *renderer, EstadoMenu *menu);
void dibujar_menu_stats(SDL_Renderer *renderer, EstadoMenu *menu, int mouseX, int mouseY);
int procesar_menu_stats(SDL_Renderer *renderer, SDL_Event *evento, EstadoMenu *menu, int mouseX, int mouseY);
int borrar_estadisticas();



#endif