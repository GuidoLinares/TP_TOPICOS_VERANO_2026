#include "menu.h"
#include "texto.h"
#include "imagenes.h"

static SDL_Texture *fondo_menu = NULL;

void inicializar_menu(EstadoMenu *menu)
{
    menu->pantalla_actual = PANTALLA_MENU;
    cargar_configuracion(&menu->config);
    strcpy(menu->nombre_jugador1, "JUGADOR1");
    strcpy(menu->nombre_jugador2, "JUGADOR2");
}

void cargar_recursos_menu(SDL_Renderer *renderer)
{
    fondo_menu = cargar_textura(renderer, "img/fondo_menu.jpg");

    if (!fondo_menu)
    {
        printf("ERROR: fondo_menu es NULL\n");
    }
}

int punto_en_rectangulo(int px, int py, int rx, int ry, int rw, int rh)
{
    return (px >= rx && px <= rx + rw && py >= ry && py <= ry + rh);
}

void dibujar_rectangulo_relleno(SDL_Renderer *renderer, int x, int y, int w, int h, int r, int g, int b)
{
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);
}

void dibujar_menu_principal(SDL_Renderer *renderer, EstadoMenu *menu, int mouseX, int mouseY)
{
    SDL_RenderClear(renderer);
    if (fondo_menu)
        SDL_RenderCopy(renderer, fondo_menu, NULL, NULL);

    int centro_x = 400;
    int inicio_y = 150;
    int espaciado = 80;

    SDL_Color blanco = {255, 255, 255, 255};
    SDL_Color amarillo = {255, 215, 0, 255};

    dibujar_rectangulo_relleno(renderer, centro_x - 150, 50, 300, 60, 70, 130, 180);
    dibujar_texto_ttf(renderer, "MEMOTEST-OMEGA", centro_x, 80, 28, blanco);

    int hover_jugar = punto_en_rectangulo(mouseX, mouseY, centro_x - 100, inicio_y, BOTON_ANCHO, BOTON_ALTO);
    dibujar_rectangulo_relleno(renderer, centro_x - 100, inicio_y, BOTON_ANCHO, BOTON_ALTO,
                               hover_jugar ? 100 : 70, hover_jugar ? 150 : 100, hover_jugar ? 100 : 70);
    dibujar_texto_ttf(renderer, "JUGAR", centro_x, inicio_y + 25, 20, hover_jugar ? amarillo : blanco);

    int hover_config = punto_en_rectangulo(mouseX, mouseY, centro_x - 100, inicio_y + espaciado, BOTON_ANCHO, BOTON_ALTO);
    dibujar_rectangulo_relleno(renderer, centro_x - 100, inicio_y + espaciado, BOTON_ANCHO, BOTON_ALTO,
                               hover_config ? 100 : 70, hover_config ? 150 : 100, hover_config ? 100 : 70);
    dibujar_texto_ttf(renderer, "CONFIGURACION", centro_x, inicio_y + espaciado + 25, 20, hover_config ? amarillo : blanco);

    int hover_stats = punto_en_rectangulo(mouseX, mouseY, centro_x - 100, inicio_y + espaciado * 2, BOTON_ANCHO, BOTON_ALTO);
    dibujar_rectangulo_relleno(renderer, centro_x - 100, inicio_y + espaciado * 2, BOTON_ANCHO, BOTON_ALTO,
                               hover_stats ? 100 : 70, hover_stats ? 150 : 100, hover_stats ? 100 : 70);
    dibujar_texto_ttf(renderer, "ESTADISTICAS", centro_x, inicio_y + espaciado * 2 + 25, 20, hover_stats ? amarillo : blanco);

    int hover_salir = punto_en_rectangulo(mouseX, mouseY, centro_x - 100, inicio_y + espaciado * 3, BOTON_ANCHO, BOTON_ALTO);
    dibujar_rectangulo_relleno(renderer, centro_x - 100, inicio_y + espaciado * 3, BOTON_ANCHO, BOTON_ALTO,
                               hover_salir ? 150 : 100, hover_salir ? 70 : 50, hover_salir ? 70 : 50);
    dibujar_texto_ttf(renderer, "SALIR", centro_x, inicio_y + espaciado * 3 + 25, 24, hover_salir ? amarillo : blanco);
}

int procesar_menu_principal(SDL_Renderer *renderer, SDL_Event *evento, EstadoMenu *menu, int mouseX, int mouseY)
{
    if (evento->type == SDL_MOUSEBUTTONDOWN)
    {
        int centro_x = 400;
        int inicio_y = 150;
        int espaciado = 80;

        if (punto_en_rectangulo(mouseX, mouseY, centro_x - 100, inicio_y, BOTON_ANCHO, BOTON_ALTO))
        {
            menu->pantalla_actual = PANTALLA_JUEGO;
            return 1;
        }

        if (punto_en_rectangulo(mouseX, mouseY, centro_x - 100, inicio_y + espaciado, BOTON_ANCHO, BOTON_ALTO))
        {
            menu->pantalla_actual = PANTALLA_CONFIG;
            return 1;
        }

        if (punto_en_rectangulo(mouseX, mouseY, centro_x - 100, inicio_y + espaciado * 2, BOTON_ANCHO, BOTON_ALTO))
        {
            menu->pantalla_actual = PANTALLA_STATS;
            return 1;
        }

        if (punto_en_rectangulo(mouseX, mouseY, centro_x - 100, inicio_y + espaciado * 3, BOTON_ANCHO, BOTON_ALTO))
        {
            menu->pantalla_actual = PANTALLA_SALIR;
            return 1;
        }
    }

    return 0;
}

void dibujar_menu_config(SDL_Renderer *renderer, EstadoMenu *menu, int mouseX, int mouseY)
{
    SDL_SetRenderDrawColor(renderer, 30, 30, 50, 255);
    SDL_RenderClear(renderer);

    int centro_x = 400;
    int inicio_y = 120;
    int espaciado_y = 100;

    int ancho_boton = 270;
    int alto_boton = 60;
    int separacion_horizontal = 40;

    SDL_Color blanco = {255, 255, 255, 255};
    SDL_Color amarillo = {255, 215, 0, 255};

    dibujar_rectangulo_relleno(renderer, centro_x - 200, 30, 400, 50, 70, 130, 180);
    dibujar_texto_ttf(renderer, "CONFIGURACION", centro_x, 55, 32, blanco);

    dibujar_texto_ttf(renderer, "Dimensiones del tablero:", centro_x, inicio_y - 20, 20, blanco);

    dibujar_rectangulo_relleno(renderer, centro_x - 150, inicio_y, 80, 50,
                               menu->config.filas == 3 && menu->config.columnas == 4 ? 100 : 60,
                               menu->config.filas == 3 && menu->config.columnas == 4 ? 150 : 90, 70);
    dibujar_texto_ttf(renderer, "3x4", centro_x - 110, inicio_y + 25, 20, blanco);

    dibujar_rectangulo_relleno(renderer, centro_x - 40, inicio_y, 80, 50,
                               menu->config.filas == 4 && menu->config.columnas == 4 ? 100 : 60,
                               menu->config.filas == 4 && menu->config.columnas == 4 ? 150 : 90, 70);
    dibujar_texto_ttf(renderer, "4x4", centro_x, inicio_y + 25, 20, blanco);

    dibujar_rectangulo_relleno(renderer, centro_x + 70, inicio_y, 80, 50,
                               menu->config.filas == 4 && menu->config.columnas == 5 ? 100 : 60,
                               menu->config.filas == 4 && menu->config.columnas == 5 ? 150 : 90, 70);
    dibujar_texto_ttf(renderer, "4x5", centro_x + 110, inicio_y + 25, 20, blanco);

    dibujar_texto_ttf(renderer, "Set de imagenes:", centro_x, inicio_y + espaciado_y - 20, 20, blanco);

    int x_izq = centro_x - ancho_boton - separacion_horizontal / 2;
    int x_der = centro_x + separacion_horizontal / 2;

    dibujar_rectangulo_relleno(renderer, x_izq, inicio_y + espaciado_y, ancho_boton, alto_boton,
                               menu->config.set_imagenes == 0 ? 100 : 60,
                               menu->config.set_imagenes == 0 ? 150 : 90, 70);
    dibujar_texto_ttf(renderer, "LENGUAJES DE PROGRAMACION", x_izq + ancho_boton / 2, inicio_y + espaciado_y + alto_boton / 2, 20, blanco);

    dibujar_rectangulo_relleno(renderer, x_der, inicio_y + espaciado_y, ancho_boton, alto_boton,
                               menu->config.set_imagenes == 1 ? 100 : 60,
                               menu->config.set_imagenes == 1 ? 150 : 90, 70);
    dibujar_texto_ttf(renderer, "NAVEGADORES WEB", x_der + ancho_boton / 2, inicio_y + espaciado_y + alto_boton / 2, 20, blanco);

    dibujar_texto_ttf(renderer, "Numero de jugadores:", centro_x, inicio_y + espaciado_y * 2 - 20, 20, blanco);

    dibujar_rectangulo_relleno(renderer, centro_x - 80, inicio_y + espaciado_y * 2, 60, 50,
                               menu->config.modo_jugadores == 1 ? 100 : 60,
                               menu->config.modo_jugadores == 1 ? 150 : 90, 70);
    dibujar_texto_ttf(renderer, "1", centro_x - 50, inicio_y + espaciado_y * 2 + 25, 20, blanco);

    dibujar_rectangulo_relleno(renderer, centro_x + 20, inicio_y + espaciado_y * 2, 60, 50,
                               menu->config.modo_jugadores == 2 ? 100 : 60,
                               menu->config.modo_jugadores == 2 ? 150 : 90, 70);
    dibujar_texto_ttf(renderer, "2", centro_x + 50, inicio_y + espaciado_y * 2 + 25, 20, blanco);

    int hover_guardar = punto_en_rectangulo(mouseX, mouseY, centro_x - 120, 550, 100, 50);
    dibujar_rectangulo_relleno(renderer, centro_x - 120, 550, 100, 50,
                               hover_guardar ? 100 : 60, hover_guardar ? 180 : 130, hover_guardar ? 100 : 70);
    dibujar_texto_ttf(renderer, "GUARDAR", centro_x - 70, 575, 16, hover_guardar ? amarillo : blanco);

    int hover_volver = punto_en_rectangulo(mouseX, mouseY, centro_x + 20, 550, 100, 50);
    dibujar_rectangulo_relleno(renderer, centro_x + 20, 550, 100, 50,
                               hover_volver ? 150 : 100, hover_volver ? 80 : 60, hover_volver ? 80 : 60);
    dibujar_texto_ttf(renderer, "VOLVER", centro_x + 70, 575, 16, hover_volver ? amarillo : blanco);
}

int procesar_menu_config(SDL_Renderer *renderer, SDL_Event *evento, EstadoMenu *menu, int mouseX, int mouseY)
{
    if (evento->type == SDL_MOUSEBUTTONDOWN)
    {
        int centro_x = 400;
        int inicio_y = 100;
        int espaciado_y = 80;

        if (punto_en_rectangulo(mouseX, mouseY, centro_x - 150, inicio_y, 80, 50))
        {
            menu->config.filas = 3;
            menu->config.columnas = 4;
        }

        if (punto_en_rectangulo(mouseX, mouseY, centro_x - 40, inicio_y, 80, 50))
        {
            menu->config.filas = 4;
            menu->config.columnas = 4;
        }

        if (punto_en_rectangulo(mouseX, mouseY, centro_x + 70, inicio_y, 80, 50))
        {
            menu->config.filas = 4;
            menu->config.columnas = 5;
        }

        if (punto_en_rectangulo(mouseX, mouseY, centro_x - 150, inicio_y + espaciado_y, 120, 50))
        {
            menu->config.set_imagenes = 0;
        }

        if (punto_en_rectangulo(mouseX, mouseY, centro_x + 30, inicio_y + espaciado_y, 120, 50))
        {
            menu->config.set_imagenes = 1;
        }

        if (punto_en_rectangulo(mouseX, mouseY, centro_x - 80, inicio_y + espaciado_y * 2, 60, 50))
        {
            menu->config.modo_jugadores = 1;
        }

        if (punto_en_rectangulo(mouseX, mouseY, centro_x + 20, inicio_y + espaciado_y * 2, 60, 50))
        {
            menu->config.modo_jugadores = 2;
        }

        if (punto_en_rectangulo(mouseX, mouseY, centro_x - 120, 550, 100, 50))
        {
            guardar_configuracion(&menu->config);
            menu->pantalla_actual = PANTALLA_MENU;
            return 1;
        }

        if (punto_en_rectangulo(mouseX, mouseY, centro_x + 20, 550, 100, 50))
        {
            cargar_configuracion(&menu->config);
            menu->pantalla_actual = PANTALLA_MENU;
            return 1;
        }
    }

    return 0;
}

int detectar_boton_menu(int mouseX, int mouseY)
{
    return punto_en_rectangulo(mouseX, mouseY, 20, 20, 120, 40);
}

void dibujar_hud_juego(SDL_Renderer *renderer, s_Jugador *jugador, int mouseX, int mouseY)
{
    SDL_Color blanco = {255, 255, 255, 255};
    SDL_Color amarillo = {255, 215, 0, 255};
    SDL_Color verde = {50, 255, 100, 255};
    SDL_Color rojo = {255, 80, 80, 255};

    int hover_menu = detectar_boton_menu(mouseX, mouseY);
    dibujar_rectangulo_relleno(renderer, 20, 20, 120, 40,
                               hover_menu ? 100 : 70,
                               hover_menu ? 80 : 60,
                               hover_menu ? 80 : 60);
    dibujar_texto_ttf(renderer, "MENU", 80, 40, 16, hover_menu ? amarillo : blanco);

    char nombre_texto[60];
    sprintf(nombre_texto, "Jugador: %s", jugador->nombre);
    dibujar_texto_ttf(renderer, nombre_texto, 400, 25, 20, blanco);

    char puntos_texto[50];
    sprintf(puntos_texto, "Puntos: %d", jugador->puntos);
    dibujar_rectangulo_relleno(renderer, 630, 15, 150, 35, 50, 80, 120);
    dibujar_texto_ttf(renderer, puntos_texto, 705, 32, 20, blanco);

    SDL_Color color_racha = blanco;
    if (jugador->racha >= 5)
        color_racha = rojo;
    else if (jugador->racha >= 3)
        color_racha = amarillo;
    else if (jugador->racha >= 1)
        color_racha = verde;

    char racha_texto[50];
    if (jugador->racha > 0)
    {
        sprintf(racha_texto, "Racha x%d", jugador->racha);
        dibujar_rectangulo_relleno(renderer, 630, 55, 150, 30, 40, 60, 90);
        dibujar_texto_ttf(renderer, racha_texto, 705, 70, 18, color_racha);
    }

    char stats_texto[100];
    sprintf(stats_texto, "Aciertos: %d  |  Fallos: %d", jugador->aciertos, jugador->fallos);
    dibujar_texto_ttf(renderer, stats_texto, 400, 760, 16, blanco);
}

void liberar_menu()
{
    if (fondo_menu)
        SDL_DestroyTexture(fondo_menu);
}
