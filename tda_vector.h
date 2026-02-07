
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct 
{
    void*vec;
    size_t longitud;
    size_t cantEl;
    size_t tamanio;
}TDAVec;

//FUNCIONES TDA
int crearVec(TDAVec *TDA, size_t longitud, size_t tamanio);
void destruirVec (TDAVec *TDA);
int insertarVec(TDAVec *TDA, void*ingreso);
void mostrarVec(TDAVec*TDA);
void* obtenerVec(TDAVec*TDA, size_t indice);
size_t longitudVec(TDAVec *TDA);




