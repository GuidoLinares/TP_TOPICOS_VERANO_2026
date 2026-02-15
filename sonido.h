#ifndef SONIDO_H
#define SONIDO_H

#include <SDL_mixer.h>
#include <stdio.h>

typedef struct {
    Mix_Chunk* seleccion;
    Mix_Chunk* acierto;
    Mix_Chunk* fallo;
} Sonidos;

int inicializar_audio();
int cargar_sonidos(Sonidos* sonidos);
void reproducir_sonido(Mix_Chunk* sonido);
void liberar_sonidos(Sonidos* sonidos);
void cerrar_audio();

#endif