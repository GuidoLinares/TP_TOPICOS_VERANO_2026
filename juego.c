#include "juego.h"

TDAVec* crear_tablero(int filas, int columnas) {

    if (filas <= 0 || columnas <= 0) return NULL; //si viene vacio filas y columnas
    

    if ((filas*columnas) % 2 == 0) return NULL; //si no coincide la cantidad de pares no se puede armar el tablero
    

    int total_cartas = filas * columnas;
    int cantidad_pares = total_cartas / 2;
    
    TDAVec* tablero = malloc(sizeof(TDAVec));
    crearVec(tablero, total_cartas, sizeof(s_Carta));

    if (!tablero) return NULL;

    
    for (size_t i = 0; i < cantidad_pares; i++)
    {
        s_Carta  carta;

        carta.id = i;
        carta.estado = CARTA_OCULTA;
        carta.puntos = PUNTOS_DEFAULT;     
        
        insertarVec(tablero, &carta);
        insertarVec(tablero, &carta);

    }
    
    //mezcla las cartas de la estructuta
    mezclarCartas(tablero, total_cartas);


    return tablero;
}

void mezclar_cartas(TDAVec* tablero, int total_cartas){
    srand(time(NULL));

    for (size_t i = 0; i < total_cartas; i++)
    {
        int j = rand() % total_cartas;

        s_Carta* carta_i = (s_Carta*) obtenerVec(tablero,i)  ;
        s_Carta* carta_j = (s_Carta*) obtenerVec(tablero,j);
    
        s_Carta carta_temp;

        //swap de cartas
        carta_temp = *carta_i;
        *carta_i = *carta_j;
        *carta_j = carta_temp;
    }
}

int comparar_cartas(s_Carta* carta_1, s_Carta* carta_2){

    if (carta_1 == NULL || carta_2 == NULL) return NULL;

    if (carta_1 == carta_2) return NULL;
    
    if (carta_1->id == carta_2->id) return 1; //mismas cartas
    else return 0;
}

int calcular_puntos(int aciertos_consecutivos){

    int puntos;

    for (size_t i = 0; i < aciertos_consecutivos; i++)
    {
        puntos += PUNTOS_DEFAULT + (i * BONUS_RACHA);
    }
    
    return puntos;
}

void procesar_acierto(s_Jugador* jugador, s_Carta* carta_1, s_Carta* carta_2){

    jugador->racha++;
    jugador->puntos = calcular_puntos(jugador->racha);
    jugador->aciertos++;

    carta_1->estado = CARTA_ENCONTRADA;
    carta_2->estado = CARTA_ENCONTRADA;

}


void procesar_fallo(s_Jugador* jugador, s_Carta* carta_1, s_Carta* carta_2){

    jugador->racha = 0;
    carta_1->estado = CARTA_OCULTA;
    carta_2 ->estado = CARTA_OCULTA;
    jugador->fallos++;
}

int verificar_fin_juego(TDAVec* tablero){

    if(!tablero) return 0;

    for (size_t i = 0; i < tablero->cantEl; i++)
    {
        s_Carta*carta = (s_Carta*) obtenerVec(tablero,i);

        if (carta->estado != CARTA_ENCONTRADA) return 0;
        
    }
    
    return 1;
}


void cambiar_turno(int*turno_actual){


}



































