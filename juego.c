#include "juego.h"

TDAVec* crear_tablero(int filas, int columnas) {
    int total_cartas = filas * columnas;
    int cantidad_pares = total_cartas / 2;
    
    TDAVec* tablero = malloc(sizeof(TDAVec));
    crearVec(tablero, total_cartas, sizeof(s_Carta));
    
    // PASO 3: Llenar con pares
    // Para cada par (0 hasta cantidad_pares-1):
    //     Crear dos cartas con el mismo id
    //     Asignar puntos aleatorios (o fijos)
    //     Insertar ambas en el vector
    insertarVec();

    
    // PASO 4: Mezclar (con tu método de rand)
    
    
    return tablero;
}