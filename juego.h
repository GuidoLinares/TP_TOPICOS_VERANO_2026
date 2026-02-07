#include "tda_vector.h"

#define CARTA_OCULTA 0
#define CARTA_VISIBLE 1
#define CARTA_ENCONTRADA 2

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
}s_Jugador;


TDAVec* crear_tablero(int filas, int columnas);







