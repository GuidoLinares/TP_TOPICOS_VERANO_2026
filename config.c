#include "config.h"

void cargar_configuracion_defecto(Configuracion* config) {
    config->filas = 3;
    config->columnas = 4;
    config->set_imagenes = 0;
    config->modo_jugadores = 1;
    config->volumen = 50;
}

int cargar_configuracion(Configuracion* config) {
    FILE* archivo = fopen(ARCHIVO_CONFIG, "rb");
    
    if (archivo == NULL) {
        cargar_configuracion_defecto(config);
        return 0;
    }
    
    size_t leidos = fread(config, sizeof(Configuracion), 1, archivo);
    fclose(archivo);
    
    if (leidos != 1) {
        cargar_configuracion_defecto(config);
        return 0;
    }
    
    return 1;
}

int guardar_configuracion(Configuracion* config) {
    FILE* archivo = fopen(ARCHIVO_CONFIG, "wb");
    
    if (archivo == NULL) {
        printf("Error: No se pudo guardar la configuracion\n");
        return 0;
    }
    
    fwrite(config, sizeof(Configuracion), 1, archivo);
    fclose(archivo);
    
    printf("Configuracion guardada exitosamente\n");
    return 1;
}

int guardar_estadistica(Estadistica* stats) {
    FILE* archivo = fopen(ARCHIVO_STATS, "ab");
    
    if (archivo == NULL) {
        printf("Error: No se pudo guardar estadisticas\n");
        return 0;
    }
    
    fwrite(stats, sizeof(Estadistica), 1, archivo);
    fclose(archivo);
    
    return 1;
}

int cargar_estadisticas(Estadistica* stats, int max_registros) {
    FILE* archivo = fopen(ARCHIVO_STATS, "rb");
    
    if (archivo == NULL) {
        return 0;
    }
    
    int leidos = 0;
    while (leidos < max_registros && fread(&stats[leidos], sizeof(Estadistica), 1, archivo) == 1) {
        leidos++;
    }
    
    fclose(archivo);
    return leidos;
}