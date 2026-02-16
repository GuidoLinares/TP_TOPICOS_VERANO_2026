#include "menu.h"
#include "texto.h"
#include "imagenes.h"

static SDL_Texture *fondo_menu = NULL;

void inicializar_menu(EstadoMenu *menu)
{
    menu->pantalla_actual = PANTALLA_MENU;
    cargar_configuracion(&menu->config);
}

void cargar_recursos_menu(SDL_Renderer *renderer)
{
    fondo_menu = cargar_textura(renderer, "img/fondo_menu");

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
    int inicio_y = 200;
    int espaciado = 80;

    SDL_Color blanco = {255, 255, 255, 255};
    SDL_Color amarillo = {255, 215, 0, 255};
    SDL_Color negro = {13, 17, 23, 255};

    dibujar_rectangulo_relleno(renderer, centro_x - 200, 50, 400, 60, 10, 15, 25);
    SDL_SetRenderDrawColor(renderer, 0, 180, 215, 255);
    SDL_Rect borde_titulo = {centro_x - 200, 50, 400, 60};
    SDL_RenderDrawRect(renderer, &borde_titulo);
    dibujar_texto_ttf(renderer, "MEMOTEST-OMEGA", centro_x, 80, 60, blanco);

    int hover_jugar = punto_en_rectangulo(mouseX, mouseY, (centro_x - (BOTON_ANCHO / 2)), inicio_y, BOTON_ANCHO, BOTON_ALTO);
    dibujar_rectangulo_relleno(renderer, centro_x - (BOTON_ANCHO / 2), inicio_y, BOTON_ANCHO, BOTON_ALTO,
                               hover_jugar ? 255 : 35, hover_jugar ? 140 : 40, hover_jugar ? 0 : 50);
    dibujar_texto_ttf(renderer, "JUGAR", centro_x, inicio_y + 25, 40, hover_jugar ? negro : blanco);

    int hover_config = punto_en_rectangulo(mouseX, mouseY, (centro_x - (BOTON_ANCHO / 2)), inicio_y + espaciado, BOTON_ANCHO, BOTON_ALTO);
    dibujar_rectangulo_relleno(renderer, centro_x - (BOTON_ANCHO / 2), inicio_y + espaciado, BOTON_ANCHO, BOTON_ALTO,
                               hover_config ? 255 : 35, hover_config ? 140 : 40, hover_config ? 0 : 50);
    dibujar_texto_ttf(renderer, "CONFIGURACION", centro_x, inicio_y + espaciado + 25, 40, hover_config ? negro : blanco);

    int hover_stats = punto_en_rectangulo(mouseX, mouseY, (centro_x - (BOTON_ANCHO / 2)), inicio_y + espaciado * 2, BOTON_ANCHO, BOTON_ALTO);
    dibujar_rectangulo_relleno(renderer, centro_x - (BOTON_ANCHO / 2), inicio_y + espaciado * 2, BOTON_ANCHO, BOTON_ALTO,
                               hover_stats ? 255 : 35, hover_stats ? 140 : 40, hover_stats ? 0 : 50);
    dibujar_texto_ttf(renderer, "ESTADISTICAS", centro_x, inicio_y + espaciado * 2 + 25, 40, hover_stats ? negro : blanco);

    int hover_salir = punto_en_rectangulo(mouseX, mouseY, (centro_x - (BOTON_ANCHO / 2)), inicio_y + espaciado * 3, BOTON_ANCHO, BOTON_ALTO);
    dibujar_rectangulo_relleno(renderer, centro_x - (BOTON_ANCHO / 2), inicio_y + espaciado * 3, BOTON_ANCHO, BOTON_ALTO,
                               hover_salir ? 150 : 100, hover_salir ? 70 : 50, hover_salir ? 70 : 50);
    dibujar_texto_ttf(renderer, "SALIR", centro_x, inicio_y + espaciado * 3 + 25, 40, hover_salir ? amarillo : blanco);
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
            menu->pantalla_actual = PANTALLA_INGRESO_NOMBRES;
            menu->jugador_escribiendo = 1;
            menu->nombre_jugador1[0] = '\0';
            menu->nombre_jugador2[0] = '\0';

            SDL_StartTextInput();
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
    SDL_Color blanco = {255, 255, 255, 255};
    SDL_Color amarillo = {255, 215, 0, 255};
    SDL_Color negro = {13, 17, 23, 255};

    SDL_RenderClear(renderer);
    if (fondo_menu)
        SDL_RenderCopy(renderer, fondo_menu, NULL, NULL);

    dibujar_rectangulo_relleno(renderer, 200, 30, 400, 50, 20, 35, 60);
    dibujar_texto_ttf(renderer, "CONFIGURACION", 400, 55, 65, blanco);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 13, 17, 23, 140);
    SDL_Rect panel_config = {50, 90, 700, 520};
    SDL_RenderFillRect(renderer, &panel_config);

    dibujar_texto_ttf(renderer, "DIMENSIONES DEL TABLERO", 400, 130, 19, blanco);

    int es_3x4 = (menu->config.filas == 3 && menu->config.columnas == 4);
    dibujar_rectangulo_relleno(renderer, 250, 150, 80, 50,
                               es_3x4 ? 255 : 35,
                               es_3x4 ? 140 : 40,
                               es_3x4 ? 0 : 50);
    dibujar_texto_ttf(renderer, "3x4", 290, 175, 30, es_3x4 ? negro : blanco);

    int es_4x4 = (menu->config.filas == 4 && menu->config.columnas == 4);
    dibujar_rectangulo_relleno(renderer, 360, 150, 80, 50,
                               es_4x4 ? 255 : 35,
                               es_4x4 ? 140 : 40,
                               es_4x4 ? 0 : 50);
    dibujar_texto_ttf(renderer, "4x4", 400, 175, 30, es_4x4 ? negro : blanco);

    int es_4x5 = (menu->config.filas == 4 && menu->config.columnas == 5);
    dibujar_rectangulo_relleno(renderer, 470, 150, 80, 50,
                               es_4x5 ? 255 : 35,
                               es_4x5 ? 140 : 40,
                               es_4x5 ? 0 : 50);
    dibujar_texto_ttf(renderer, "4x5", 510, 175, 30, es_4x5 ? negro : blanco);

    dibujar_texto_ttf(renderer, "SET DE CARTAS", 400, 240, 19, blanco);

    int set_lenguajes = (menu->config.set_imagenes == 0);
    dibujar_rectangulo_relleno(renderer, 50, 260, 340, 60,
                               set_lenguajes ? 255 : 35,
                               set_lenguajes ? 140 : 40,
                               set_lenguajes ? 0 : 50);
    dibujar_texto_ttf(renderer, "LENGUAJES DE PROGRAMACION", 220, 290, 21, set_lenguajes ? negro : blanco);

    int set_navegadores = (menu->config.set_imagenes == 1);
    dibujar_rectangulo_relleno(renderer, 410, 260, 340, 60,
                               set_navegadores ? 255 : 35,
                               set_navegadores ? 140 : 40,
                               set_navegadores ? 0 : 50);
    dibujar_texto_ttf(renderer, "NAVEGADORES WEB", 580, 290, 21, set_navegadores ? negro : blanco);

    dibujar_texto_ttf(renderer, "DORSO CARTAS", 400, 350, 19, blanco);

    int set_rojo = (menu->config.set_dorso == 0);
    dibujar_rectangulo_relleno(renderer, 250, 370, 130, 60,
                               set_rojo ? 255 : 35,
                               set_rojo ? 140 : 40,
                               set_rojo ? 0 : 50);
    dibujar_texto_ttf(renderer, "ROJO", 315, 400, 24, set_rojo ? negro : blanco);

    int set_violeta = (menu->config.set_dorso == 1);
    dibujar_rectangulo_relleno(renderer, 420, 370, 130, 60,
                               set_violeta ? 255 : 35,
                               set_violeta ? 140 : 40,
                               set_violeta ? 0 : 50);
    dibujar_texto_ttf(renderer, "VIOLETA", 485, 400, 24, set_violeta ? negro : blanco);

    dibujar_texto_ttf(renderer, "CANTIDAD DE JUGADORES", 400, 460, 19, blanco);

    int modo_1 = (menu->config.modo_jugadores == 1);
    dibujar_rectangulo_relleno(renderer, 320, 480, 80, 60,
                               modo_1 ? 255 : 35,
                               modo_1 ? 140 : 40,
                               modo_1 ? 0 : 50);
    dibujar_texto_ttf(renderer, "1", 360, 510, 30, modo_1 ? negro : blanco);

    int modo_2 = (menu->config.modo_jugadores == 2);
    dibujar_rectangulo_relleno(renderer, 420, 480, 80, 60,
                               modo_2 ? 255 : 35,
                               modo_2 ? 140 : 40,
                               modo_2 ? 0 : 50);
    dibujar_texto_ttf(renderer, "2", 460, 510, 30, modo_2 ? negro : blanco);

    int hover_guardar = punto_en_rectangulo(mouseX, mouseY, 250, 640, 140, 50);
    dibujar_rectangulo_relleno(renderer, 250, 640, 140, 50,
                               hover_guardar ? 100 : 60, hover_guardar ? 180 : 130, hover_guardar ? 100 : 70);
    dibujar_texto_ttf(renderer, "GUARDAR", 320, 665, 20, hover_guardar ? amarillo : blanco);

    int hover_volver = punto_en_rectangulo(mouseX, mouseY, 410, 640, 140, 50);
    dibujar_rectangulo_relleno(renderer, 410, 640, 140, 50,
                               hover_volver ? 150 : 100, hover_volver ? 80 : 60, hover_volver ? 80 : 60);
    dibujar_texto_ttf(renderer, "VOLVER", 480, 665, 20, hover_volver ? amarillo : blanco);
}

int procesar_menu_config(SDL_Renderer *renderer, SDL_Event *evento, EstadoMenu *menu, int mouseX, int mouseY)
{
    if (evento->type == SDL_MOUSEBUTTONDOWN)
    {
        if (punto_en_rectangulo(mouseX, mouseY, 250, 150, 80, 50))
        {
            menu->config.filas = 3;
            menu->config.columnas = 4;
        }

        if (punto_en_rectangulo(mouseX, mouseY, 360, 150, 80, 50))
        {
            menu->config.filas = 4;
            menu->config.columnas = 4;
        }

        if (punto_en_rectangulo(mouseX, mouseY, 470, 150, 80, 50))
        {
            menu->config.filas = 4;
            menu->config.columnas = 5;
        }

        if (punto_en_rectangulo(mouseX, mouseY, 50, 260, 340, 60))
        {
            menu->config.set_imagenes = 0;
        }

        if (punto_en_rectangulo(mouseX, mouseY, 410, 260, 340, 60))
        {
            menu->config.set_imagenes = 1;
        }

        if (punto_en_rectangulo(mouseX, mouseY, 250, 370, 130, 60))
        {
            menu->config.set_dorso = 0;
        }

        if (punto_en_rectangulo(mouseX, mouseY, 420, 370, 130, 60))
        {
            menu->config.set_dorso = 1;
        }

        if (punto_en_rectangulo(mouseX, mouseY, 320, 480, 80, 60))
        {
            menu->config.modo_jugadores = 1;
        }

        if (punto_en_rectangulo(mouseX, mouseY, 420, 480, 80, 60))
        {
            menu->config.modo_jugadores = 2;
        }

        if (punto_en_rectangulo(mouseX, mouseY, 250, 640, 140, 50))
        {
            guardar_configuracion(&menu->config);
            menu->pantalla_actual = PANTALLA_MENU;
            return 1;
        }

        if (punto_en_rectangulo(mouseX, mouseY, 410, 640, 140, 50))
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
    return punto_en_rectangulo(mouseX, mouseY, 340, 700, 120, 40);
}

void liberar_menu()
{
    if (fondo_menu)
        SDL_DestroyTexture(fondo_menu);
}

void procesar_ingreso_nombres(SDL_Event *evento, EstadoMenu *menu)
{
    if (evento->type == SDL_TEXTINPUT)
    {
        char *nombre_actual = (menu->jugador_escribiendo == 1)
                                  ? menu->nombre_jugador1
                                  : menu->nombre_jugador2;
        if (strlen(nombre_actual) + strlen(evento->text.text) < 20)
        {
            strcat(nombre_actual, evento->text.text);
        }
    }

    else if (evento->type == SDL_KEYDOWN)
    {
        if (evento->key.keysym.sym == SDLK_BACKSPACE)
        {
            char *nombre = (menu->jugador_escribiendo == 1)
                               ? menu->nombre_jugador1
                               : menu->nombre_jugador2;

            int len = strlen(nombre);
            if (len > 0)
                nombre[len - 1] = '\0';
        }

        if (evento->key.keysym.sym == SDLK_RETURN)
        {
            if (menu->config.modo_jugadores == 2 && menu->jugador_escribiendo == 1)
            {
                menu->jugador_escribiendo = 2;
            }

            else
            {
                menu->pantalla_actual = PANTALLA_JUEGO;
                SDL_StopTextInput();
            }
        }
    }
}

void dibujar_ingreso_nombres(SDL_Renderer *renderer, EstadoMenu *menu)
{
    if (fondo_menu)
        SDL_RenderCopy(renderer, fondo_menu, NULL, NULL);

    int centro_x = 400;
    int inicio_y = 180;
    int espaciado = 140;

    SDL_Color blanco = {255, 255, 255, 255};
    SDL_Color amarillo = {255, 255, 0, 255};
    SDL_Color gris = {100, 100, 100, 255};

    dibujar_texto_ttf(renderer, "CONFIGURACION DE JUGADORES", centro_x, 70, 50, blanco);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 13, 17, 23, 140);
    SDL_Rect panel_config = {150, 130, 500, 420};
    SDL_RenderFillRect(renderer, &panel_config);

    SDL_Color colorJ1 = (menu->jugador_escribiendo == 1) ? amarillo : blanco;
    dibujar_texto_ttf(renderer, "Jugador 1:", centro_x, inicio_y, 24, colorJ1);

    SDL_Rect rectJ1 = {centro_x - 150, inicio_y + 20, 300, 50};
    SDL_SetRenderDrawColor(renderer, colorJ1.r, colorJ1.g, colorJ1.b, 255);
    SDL_RenderDrawRect(renderer, &rectJ1);

    if (strlen(menu->nombre_jugador1) > 0)
    {
        dibujar_texto_ttf(renderer, menu->nombre_jugador1, centro_x, inicio_y + 52, 24, blanco);
    }
    else if (menu->jugador_escribiendo == 1)
    {
        dibujar_texto_ttf(renderer, "Escribiendo...", centro_x, inicio_y + 85, 16, gris);
    }

    if (menu->config.modo_jugadores == 2)
    {
        SDL_Color colorJ2 = (menu->jugador_escribiendo == 2) ? amarillo : blanco;
        dibujar_texto_ttf(renderer, "Jugador 2:", centro_x, inicio_y + espaciado, 24, colorJ2);

        SDL_Rect rectJ2 = {centro_x - 150, inicio_y + espaciado + 20, 300, 45};
        SDL_SetRenderDrawColor(renderer, colorJ2.r, colorJ2.g, colorJ2.b, 255);
        SDL_RenderDrawRect(renderer, &rectJ2);

        if (strlen(menu->nombre_jugador2) > 0)
        {
            dibujar_texto_ttf(renderer, menu->nombre_jugador2, centro_x, inicio_y + espaciado + 45, 24, blanco);
        }
        else if (menu->jugador_escribiendo == 2)
        {
            dibujar_texto_ttf(renderer, "Escribiendo...", centro_x, inicio_y + espaciado + 85, 16, gris);
        }
    }
    dibujar_texto_ttf(renderer, "Presione ENTER para confirmar", centro_x, 520, 18, blanco);
}
