#include "sonido.h"

int inicializar_audio() {
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        printf("Error al inicializar SDL_mixer: %s\n", Mix_GetError());
        return 0;
    }
    printf("SDL_mixer inicializado correctamente\n");
    return 1;
}

int cargar_sonidos(Sonidos* sonidos) {
    sonidos->seleccion = Mix_LoadWAV("snd/seleccion.wav");
    if (!sonidos->seleccion) {
        printf("Advertencia: No se pudo cargar snd/seleccion.wav: %s\n", Mix_GetError());
    }
    
    sonidos->acierto = Mix_LoadWAV("snd/acierto.wav");
    if (!sonidos->acierto) {
        printf("Advertencia: No se pudo cargar snd/acierto.wav: %s\n", Mix_GetError());
    }
    
    sonidos->fallo = Mix_LoadWAV("snd/fallo.wav");
    if (!sonidos->fallo) {
        printf("Advertencia: No se pudo cargar snd/fallo.wav: %s\n", Mix_GetError());
    }
    
    if (sonidos->seleccion && sonidos->acierto && sonidos->fallo) {
        printf("Sonidos cargados correctamente\n");
        return 1;
    }
    
    return 0;
}

void reproducir_sonido(Mix_Chunk* sonido) {
    if (sonido) {
        Mix_PlayChannel(-1, sonido, 0);
    }
}

void liberar_sonidos(Sonidos* sonidos) {
    if (sonidos->seleccion) {
        Mix_FreeChunk(sonidos->seleccion);
        sonidos->seleccion = NULL;
    }
    if (sonidos->acierto) {
        Mix_FreeChunk(sonidos->acierto);
        sonidos->acierto = NULL;
    }
    if (sonidos->fallo) {
        Mix_FreeChunk(sonidos->fallo);
        sonidos->fallo = NULL;
    }
}

void cerrar_audio() {
    Mix_CloseAudio();
    Mix_Quit();
}