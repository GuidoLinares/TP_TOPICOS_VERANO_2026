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
    SDL_RenderClear(renderer);
    if (fondo_menu)
        SDL_RenderCopy(renderer, fondo_menu, NULL, NULL);

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

    dibujar_texto_ttf(renderer, "Set de cartas:", centro_x, inicio_y + espaciado_y - 20, 20, blanco);

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

    dibujar_texto_ttf(renderer, "Color dorso de cartas:", centro_x, inicio_y + espaciado_y * 2 - 20, 20, blanco);
//ordenar variables de cada boton
    int ancho_boton_dorso = 130;
    int x_izq_set = centro_x - ancho_boton_dorso - separacion_horizontal / 2;
    int x_der_set = centro_x + separacion_horizontal / 2;

    dibujar_rectangulo_relleno(renderer, x_izq_set, inicio_y + espaciado_y * 2, ancho_boton_dorso, alto_boton,
                               menu->config.set_dorso == 0 ? 100 : 60,
                               menu->config.set_dorso == 0 ? 150 : 90, 70);
    dibujar_texto_ttf(renderer, "ROJO", x_izq_set + ancho_boton_dorso / 2, inicio_y + espaciado_y * 2 + alto_boton / 2, 20, blanco);

    dibujar_rectangulo_relleno(renderer, x_der_set + 20, inicio_y + espaciado_y * 2, ancho_boton_dorso, alto_boton,
                               menu->config.set_dorso == 1 ? 100 : 60,
                               menu->config.set_dorso == 1 ? 150 : 90, 70);
    dibujar_texto_ttf(renderer, "VIOLETA", x_der_set + ancho_boton_dorso / 2, inicio_y + espaciado_y * 2 + alto_boton / 2, 20, blanco);

    dibujar_texto_ttf(renderer, "Numero de jugadores:", centro_x, inicio_y + espaciado_y * 3 - 20, 20, blanco);

    dibujar_rectangulo_relleno(renderer, centro_x - 80, inicio_y + espaciado_y * 3, 80, alto_boton,
                               menu->config.modo_jugadores == 1 ? 100 : 60,
                               menu->config.modo_jugadores == 1 ? 150 : 90, 70);
    dibujar_texto_ttf(renderer, "1", centro_x - 50, inicio_y + espaciado_y * 3 + 25, 20, blanco);

    dibujar_rectangulo_relleno(renderer, centro_x + 20, inicio_y + espaciado_y * 3, 80, alto_boton,
                               menu->config.modo_jugadores == 2 ? 100 : 60,
                               menu->config.modo_jugadores == 2 ? 150 : 90, 70);
    dibujar_texto_ttf(renderer, "2", centro_x + 50, inicio_y + espaciado_y * 3 + 25, 20, blanco);

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
        int inicio_y = 120;
        int espaciado_y = 100;

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

        if (punto_en_rectangulo(mouseX, mouseY, centro_x - 150, inicio_y + espaciado_y * 2, 120, 50))
        {
            menu->config.set_dorso = 0;
        }

        if (punto_en_rectangulo(mouseX, mouseY, centro_x + 30, inicio_y + espaciado_y * 2, 120, 50))
        {
            menu->config.set_dorso = 1;
        }

        if (punto_en_rectangulo(mouseX, mouseY, centro_x - 80, inicio_y + espaciado_y * 3, 60, 50))
        {
            menu->config.modo_jugadores = 1;
        }

        if (punto_en_rectangulo(mouseX, mouseY, centro_x + 20, inicio_y + espaciado_y * 3, 60, 50))
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
    int inicio_y = 150;
    int espaciado = 120;

    SDL_Color blanco = {255, 255, 255, 255};
    SDL_Color amarillo = {255, 255, 0, 255};
    SDL_Color gris = {100, 100, 100, 255};

    dibujar_texto_ttf(renderer, "CONFIGURACION DE JUGADORES", centro_x, 60, 32, blanco);

    SDL_Color colorJ1 = (menu->jugador_escribiendo == 1) ? amarillo : blanco;
    dibujar_texto_ttf(renderer, "Nombre Jugador 1:", centro_x, inicio_y, 24, colorJ1);

    SDL_Rect rectJ1 = {centro_x - 150, inicio_y + 30, 300, 50};
    SDL_SetRenderDrawColor(renderer, colorJ1.r, colorJ1.g, colorJ1.b, 255);
    SDL_RenderDrawRect(renderer, &rectJ1);

    if (strlen(menu->nombre_jugador1) > 0)
    {
        dibujar_texto_ttf(renderer, menu->nombre_jugador1, centro_x, inicio_y + 52, 24, blanco);
    }
    else if (menu->jugador_escribiendo == 1)
    {
        dibujar_texto_ttf(renderer, "Escribiendo...", centro_x, 240, 18, gris);
    }

    if (menu->config.modo_jugadores == 2)
    {
        SDL_Color colorJ2 = (menu->jugador_escribiendo == 2) ? amarillo : blanco;
        dibujar_texto_ttf(renderer, "Ingresar Nombre Jugador 2:", centro_x, inicio_y + espaciado, 24, colorJ2);

        SDL_Rect rectJ2 = {centro_x - 150, inicio_y + espaciado + 30, 300, 45};
        SDL_SetRenderDrawColor(renderer, colorJ2.r, colorJ2.g, colorJ2.b, 255);
        SDL_RenderDrawRect(renderer, &rectJ2);

        if (strlen(menu->nombre_jugador2) > 0)
        {
            dibujar_texto_ttf(renderer, menu->nombre_jugador2, centro_x, inicio_y + espaciado + 52, 24, blanco);
        }
        else if (menu->jugador_escribiendo == 2)
        {
            dibujar_texto_ttf(renderer, "...", centro_x, inicio_y + espaciado + 52, 24, blanco);
        }
    }
    dibujar_texto_ttf(renderer, "Presione ENTER para confirmar", centro_x, 520, 18, blanco);
}

int comparar_estadisticas(const void* a, const void* b) {
    Estadistica* stat_a = (Estadistica*)a;
    Estadistica* stat_b = (Estadistica*)b;
    return stat_b->puntos - stat_a->puntos;
}

int borrar_estadisticas() {
    if (remove(ARCHIVO_STATS) == 0) {
        printf("Estadisticas borradas correctamente\n");
        return 1;
    }
    return 0;
}

void dibujar_menu_stats(SDL_Renderer *renderer, EstadoMenu *menu, int mouseX, int mouseY) {
    SDL_SetRenderDrawColor(renderer, 30, 30, 50, 255);
    SDL_RenderClear(renderer);
    
    int centro_x = 400;
    
    SDL_Color blanco = {255, 255, 255, 255};
    SDL_Color amarillo = {255, 215, 0, 255};
    SDL_Color gris = {150, 150, 150, 255};
    SDL_Color verde = {0,255,0,1};
    
    dibujar_rectangulo_relleno(renderer, centro_x - 300, 30, 600, 60, 70, 130, 180);
    dibujar_texto_ttf(renderer, "MEJORES PUNTAJES", centro_x, 60, 32, blanco);
    
    Estadistica stats[100];
    int cantidad = cargar_estadisticas(stats, 100);
    
    if (cantidad > 0) {
        qsort(stats, cantidad, sizeof(Estadistica), comparar_estadisticas);
        
        int limite = cantidad < 10 ? cantidad : 10;
        
        int tabla_x = centro_x - 280;
        int tabla_y = 130;
        int fila_alto = 45;
        
        dibujar_texto_ttf(renderer, "Pos", tabla_x + 30, tabla_y, 20, amarillo);
        dibujar_texto_ttf(renderer, "Nombre", tabla_x + 150, tabla_y, 20, amarillo);
        dibujar_texto_ttf(renderer, "Puntos", tabla_x + 320, tabla_y, 20, amarillo);
        dibujar_texto_ttf(renderer, "Aciertos", tabla_x + 430, tabla_y, 20, amarillo);
        dibujar_texto_ttf(renderer, "Fallos", tabla_x + 530, tabla_y, 20, amarillo);
        
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderDrawLine(renderer, tabla_x, tabla_y + 20, tabla_x + 560, tabla_y + 20);
        
        for (int i = 0; i < limite; i++) {
            int fila_y = tabla_y + 40 + (i * fila_alto);
            
            if (i % 2 == 0) {
                dibujar_rectangulo_relleno(renderer, tabla_x - 10, fila_y - 15, 580, 40, 40, 50, 70);
            }
            
            char pos_texto[10];
            sprintf(pos_texto, "%d", i + 1);
            dibujar_texto_ttf(renderer, pos_texto, tabla_x + 30, fila_y, 18, blanco);
            
            dibujar_texto_ttf(renderer, stats[i].nombre, tabla_x + 150, fila_y, 18, blanco);
            
            char puntos_texto[20];
            sprintf(puntos_texto, "%d", stats[i].puntos);
            dibujar_texto_ttf(renderer, puntos_texto, tabla_x + 320, fila_y, 18, 
                            stats[i].puntos > 200 ? amarillo : blanco);
            
            char aciertos_texto[10];
            sprintf(aciertos_texto, "%d", stats[i].aciertos);
            dibujar_texto_ttf(renderer, aciertos_texto, tabla_x + 430, fila_y, 18,verde);
            
            char fallos_texto[10];
            sprintf(fallos_texto, "%d", stats[i].fallos);
            SDL_Color color_fallos = {255, 100, 100, 255};
            dibujar_texto_ttf(renderer, fallos_texto, tabla_x + 530, fila_y, 18, color_fallos);
        }
        
        char total_texto[50];
        sprintf(total_texto, "Mostrando %d de %d registros", limite, cantidad);
        dibujar_texto_ttf(renderer, total_texto, centro_x, 650, 16, gris);
        
    } else {
        dibujar_texto_ttf(renderer, "No hay estadisticas guardadas", centro_x, 350, 24, gris);
        dibujar_texto_ttf(renderer, "Juega algunas partidas para ver tus records!", centro_x, 390, 18, gris);
    }
    
    int hover_borrar = punto_en_rectangulo(mouseX, mouseY, centro_x - 220, 700, 180, 50);
    int hover_volver = punto_en_rectangulo(mouseX, mouseY, centro_x + 40, 700, 180, 50);
    
    if (cantidad > 0) {
        dibujar_rectangulo_relleno(renderer, centro_x - 220, 700, 180, 50,
                                    hover_borrar ? 180 : 120, 
                                    hover_borrar ? 60 : 40, 
                                    hover_borrar ? 60 : 40);
        dibujar_texto_ttf(renderer, "BORRAR TODO", centro_x - 130, 725, 18, hover_borrar ? amarillo : blanco);
    }
    
    dibujar_rectangulo_relleno(renderer, centro_x + 40, 700, 180, 50,
                                hover_volver ? 100 : 70, 
                                hover_volver ? 150 : 100, 
                                hover_volver ? 100 : 70);
    dibujar_texto_ttf(renderer, "VOLVER", centro_x + 130, 725, 18, hover_volver ? amarillo : blanco);
}

int procesar_menu_stats(SDL_Renderer *renderer, SDL_Event *evento, EstadoMenu *menu, int mouseX, int mouseY) {
    if (evento->type == SDL_MOUSEBUTTONDOWN) {
        int centro_x = 400;
        
        Estadistica stats[100];
        int cantidad = cargar_estadisticas(stats, 100);
        
        if (cantidad > 0 && punto_en_rectangulo(mouseX, mouseY, centro_x - 220, 700, 180, 50)) {
            if (borrar_estadisticas()) {
                printf("Todas las estadisticas han sido eliminadas\n");
            }
            return 1;
        }
        
        if (punto_en_rectangulo(mouseX, mouseY, centro_x + 40, 700, 180, 50)) {
            menu->pantalla_actual = PANTALLA_MENU;
            return 1;
        }
    }
    
    return 0;
}






