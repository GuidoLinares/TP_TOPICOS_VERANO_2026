#include "tda_vector.h"


int crearVec (TDAVec *TDA, size_t longitud, size_t tamanio){

    TDA->vec = malloc(longitud*tamanio);

    if (!TDA->vec){
        return 0;
    }
    
    TDA->cantEl = 0 ;
    TDA->longitud = longitud;
    TDA->tamanio = tamanio;

    return 1;
};


void destruirVec(TDAVec *TDA){
    free(TDA->vec);

    TDA->cantEl = 0;
    TDA->longitud = 0;
    TDA->tamanio = 0;
};


int insertarVec(TDAVec *TDA, void*ingreso){
    
    if (TDA->cantEl >= TDA->longitud)return 0;

    void* fin = TDA->vec + TDA->tamanio * TDA->cantEl;

    memcpy(fin, ingreso, TDA->tamanio);
    TDA->cantEl ++;

    return 1;
};


void mostrarVec(TDAVec *TDA){
};


void* obtenerVec(TDAVec*TDA,size_t indice){
    if (indice >= TDA->cantEl) return NULL;

    void*fin = TDA->vec + TDA->tamanio*indice;

    return fin;
};


size_t longitudVec(TDAVec* TDA){
    return TDA->cantEl;
};




