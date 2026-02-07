#include "tda_vector.h"
#include <time.h>

//ESTADOS CARTAS
#define CARTA_OCULTA 0
#define CARTA_VISIBLE 1
#define CARTA_ENCONTRADA 2

//PUNTOS
#define PUNTOS_DEFAULT 25
#define BONUS_RACHA 10

//ESTADOS TABLERO
#define ESTADO_ESPERANDO_PRIMERA 0
#define ESTADO_ESPERANDO_SEGUNDA 1
#define ESTADOS_COMPARANDO 2
#define ESTADO_FIN 3



typedef struct
{
    int id;
    int estado;
    int puntos;
    char ruta_imagen[100];
}s_Carta;

typedef struct 
{
    char nombre[50];
    int puntos;
    int aciertos;
    int turno_activo;
    int racha;
    int fallos;
}s_Jugador;

typedef struct 
{
    TDAVec* tablero;
    s_Jugador* jugador1;
    s_Jugador* jugador2;
    int modo_competitivo;
    int turno_actual;
    int carta_seleccionada_1;
    int carta_seleccionada_2;
    int estado;    
}s_EstadoJuego;


TDAVec* crear_tablero(int filas, int columnas);
void mezclar_cartas(TDAVec* tablero, int total_cartas);
int comparar_cartas(s_Carta* carta_i, s_Carta*carta_j);
int calcular_puntos(int aciertos_consecutivos);
void procesar_acierto(s_Jugador* jugador, s_Carta* carta_1, s_Carta* carta_2);
void procesar_fallo(s_Jugador* jugador, s_Carta* carta_1, s_Carta* carta_2);
int verificar_fin_juego(TDAVec* tablero);
void cambiar_turno(int*turno_actual);

