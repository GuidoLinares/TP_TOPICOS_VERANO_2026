#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARCHIVO_CONFIG "config.dat"
#define ARCHIVO_STATS "stats.dat"

typedef struct {
    int filas;
    int columnas;
    int set_imagenes;
    int set_dorso;
    int modo_jugadores;
    int volumen;
} Configuracion;

typedef struct {
    char nombre[50];
    int puntos;
    int aciertos;
    int fallos;
    char fecha[20];
} Estadistica;

void cargar_configuracion_defecto(Configuracion* config);
int cargar_configuracion(Configuracion* config);
int guardar_configuracion(Configuracion* config);
int guardar_estadistica(Estadistica* stats);
int cargar_estadisticas(Estadistica* stats, int max_registros);

#endif