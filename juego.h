#ifndef JUEGO_H
#define JUEGO_H

#include <SDL.h>
#include <SDL_mixer.h>
#include "tda_vector.h"
#include <time.h>
#include "config.h"
#include "menu.h"
#include "imagenes.h"
#include "sonido.h"

#define CARTA_OCULTA 0
#define CARTA_VISIBLE 1
#define CARTA_ENCONTRADA 2

#define PUNTOS_DEFAULT 25
#define BONUS_RACHA 10

#define ESTADO_ESPERANDO_PRIMERA 0
#define ESTADO_ESPERANDO_SEGUNDA 1
#define ESTADO_COMPARANDO 2
#define ESTADO_FIN 3

#define VENTANA_ANCHO 750
#define VENTANA_ALTO 750

#define CARTA_ANCHO 100
#define CARTA_ALTO 140
#define ESPACIADO 20
#define MARGEN_SUPERIOR 80

typedef struct
{
    int id;
    int estado;
    int puntos;
    char ruta_imagen[100];
} s_Carta;

typedef struct
{
    char nombre[50];
    int puntos;
    int aciertos;
    int turno_activo;
    int racha;
    int fallos;
} s_Jugador;

typedef struct
{
    TDAVec *tablero;

    s_Jugador jugador1;
    s_Jugador jugador2;
    int jugador_actual;

    int modo_competitivo;
    int turno_actual;
    int juego_iniciado;

    int carta_seleccionada_1;
    int carta_seleccionada_2;

    SDL_Texture *textura_dorso;
    SDL_Texture *textura_cartas[10];

    Configuracion config;

    int cant_pares;

    Uint32 tiempo_comparacion;
    int esperando_delay;
    
    Sonidos sonidos;
} s_EstadoJuego;

typedef struct
{
    int filas;
    int columnas;
    int set_imagenes;
    int modo_jugadores;
} s_configuracion;

TDAVec *crear_tablero(int filas, int columnas);
void mezclar_cartas(TDAVec *tablero, int total_cartas);
int comparar_cartas(s_Carta *carta_i, s_Carta *carta_j);
int calcular_puntos(int aciertos_consecutivos);
void procesar_acierto(s_Jugador *jugador, s_Carta *carta_1, s_Carta *carta_2);
void procesar_fallo(s_Jugador *jugador, s_Carta *carta_1, s_Carta *carta_2);
int verificar_fin_juego(TDAVec *tablero);
void cambiar_turno(s_EstadoJuego *estado);
void finalizar_juego(s_EstadoJuego *estado_juego);
void iniciar_juego(s_EstadoJuego *estado_juego, EstadoMenu *estado_menu, SDL_Renderer *renderer);
void procesar_eventos_juego(s_EstadoJuego *estado_juego, SDL_Event *evento, int mouseX, int mouseY, EstadoMenu *estado_menu);
void actualizar_juego(s_EstadoJuego *estado_juego, EstadoMenu *estado_menu);
int convertir_clic_a_indice(int mouseX, int mouseY, int filas, int columnas);

#endif