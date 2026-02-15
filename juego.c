#include "juego.h"

TDAVec *crear_tablero(int filas, int columnas)
{
    if (filas <= 0 || columnas <= 0)
        return NULL;

    if ((filas * columnas) % 2 != 0)
        return NULL;

    int total_cartas = filas * columnas;
    int cantidad_pares = total_cartas / 2;

    TDAVec *tablero = malloc(sizeof(TDAVec));
    crearVec(tablero, total_cartas, sizeof(s_Carta));

    if (!tablero)
        return NULL;

    for (size_t i = 0; i < cantidad_pares; i++)
    {
        s_Carta carta;

        carta.id = i;
        carta.estado = CARTA_OCULTA;
        carta.puntos = PUNTOS_DEFAULT;

        insertarVec(tablero, &carta);
        insertarVec(tablero, &carta);
    }

    mezclar_cartas(tablero, total_cartas);

    return tablero;
}

void mezclar_cartas(TDAVec *tablero, int total_cartas)
{
    srand(time(NULL));

    for (size_t i = 0; i < total_cartas; i++)
    {
        int j = rand() % total_cartas;

        s_Carta *carta_i = (s_Carta *)obtenerVec(tablero, i);
        s_Carta *carta_j = (s_Carta *)obtenerVec(tablero, j);

        s_Carta carta_temp;

        carta_temp = *carta_i;
        *carta_i = *carta_j;
        *carta_j = carta_temp;
    }
}

int comparar_cartas(s_Carta *carta_1, s_Carta *carta_2)
{

    if (carta_1 == NULL || carta_2 == NULL)
        return -1;

    if (carta_1 == carta_2)
        return -1;

    if (carta_1->id == carta_2->id)
        return 1;
    else
        return 0;
}

int calcular_puntos(int aciertos_consecutivos)
{

    return PUNTOS_DEFAULT + ((aciertos_consecutivos - 1) * BONUS_RACHA);
}

void procesar_acierto(s_Jugador *jugador, s_Carta *carta_1, s_Carta *carta_2)
{

    jugador->racha++;
    jugador->puntos += calcular_puntos(jugador->racha);
    jugador->aciertos++;

    carta_1->estado = CARTA_ENCONTRADA;
    carta_2->estado = CARTA_ENCONTRADA;
}

void procesar_fallo(s_Jugador *jugador, s_Carta *carta_1, s_Carta *carta_2)
{

    jugador->racha = 0;
    carta_1->estado = CARTA_OCULTA;
    carta_2->estado = CARTA_OCULTA;
    jugador->fallos++;
}

int verificar_fin_juego(TDAVec *tablero)
{

    if (!tablero)
        return 0;

    for (size_t i = 0; i < longitudVec(tablero); i++)
    {
        s_Carta *carta = (s_Carta *)obtenerVec(tablero, i);

        if (carta->estado != CARTA_ENCONTRADA)
            return 0;
    }

    return 1;
}

void cambiar_turno(int *turno_actual)
{

    if (*turno_actual == 1)
        *turno_actual = 2;
    else
        *turno_actual = 1;
}

void finalizar_juego(s_EstadoJuego *estado_juego)
{
    if (estado_juego == NULL)
        return;

    for (int i = 0; i < 10; i++)
    {
        if (estado_juego->textura_cartas[i] != NULL)
        {
            SDL_DestroyTexture(estado_juego->textura_cartas[i]);
            estado_juego->textura_cartas[i] = NULL;
        }
    }

    if (estado_juego->textura_dorso != NULL)
    {
        SDL_DestroyTexture(estado_juego->textura_dorso);
        estado_juego->textura_dorso = NULL;
    }

    if (estado_juego->tablero != NULL)
    {
        destruirVec(estado_juego->tablero);
        free(estado_juego->tablero);
        estado_juego->tablero = NULL;
    }

    estado_juego->juego_iniciado = 0;
    estado_juego->carta_seleccionada_1 = -1;
    estado_juego->carta_seleccionada_2 = -1;
    estado_juego->turno_actual = ESTADO_ESPERANDO_PRIMERA;
}

int convertir_clic_a_indice(int mouseX, int mouseY, int filas, int columnas)
{
    int ancho_tablero = (columnas * CARTA_ANCHO) + ((columnas - 1) * ESPACIADO);
    int alto_tablero = (filas * CARTA_ALTO) + ((filas - 1) * ESPACIADO);
    int offset_x = (VENTANA_ANCHO - ancho_tablero) / 2;
    int offset_y = (VENTANA_ALTO - alto_tablero) / 2;

    int ajustado_x = mouseX - offset_x;
    int ajustado_y = mouseY - offset_y;

    if (ajustado_x < 0 || ajustado_y < 0)
        return -1;
    if (ajustado_x >= ancho_tablero || ajustado_y >= alto_tablero)
        return -1;

    int fila = ajustado_y / (CARTA_ALTO + ESPACIADO);
    int columna = ajustado_x / (CARTA_ANCHO + ESPACIADO);

    if (fila < 0 || fila >= filas)
        return -1;
    if (columna < 0 || columna >= columnas)
        return -1;

    int indice = (fila * columnas) + columna;

    return indice;
}

void iniciar_juego(s_EstadoJuego *estado_juego, EstadoMenu *estado_menu, SDL_Renderer *renderer)
{
    estado_juego->tablero = crear_tablero(estado_juego->config.filas, estado_juego->config.columnas);

    if (!estado_juego->tablero)
    {
        printf("Error al crear tablero\n");
        return;
    }

    estado_juego->cant_pares = (estado_juego->config.filas * estado_juego->config.columnas) / 2;

    strcpy(estado_juego->jugador1.nombre, estado_menu->nombre_jugador1);
    estado_juego->jugador1.puntos = 0;
    estado_juego->jugador1.aciertos = 0;
    estado_juego->jugador1.fallos = 0;
    estado_juego->jugador1.racha = 0;

    if (estado_juego->modo_competitivo)
    {
        strcpy(estado_juego->jugador2.nombre, estado_menu->nombre_jugador2);
        estado_juego->jugador2.puntos = 0;
        estado_juego->jugador2.aciertos = 0;
        estado_juego->jugador2.fallos = 0;
        estado_juego->jugador2.racha = 0;
    }

    estado_juego->jugador_actual = 1;

    estado_juego->carta_seleccionada_1 = -1;
    estado_juego->carta_seleccionada_2 = -1;
    estado_juego->turno_actual = ESTADO_ESPERANDO_PRIMERA;

    for (int i = 0; i < estado_juego->cant_pares; i++)
    {
        char ruta[100];
        sprintf(ruta, "img/%d.png", i + 1);
        estado_juego->textura_cartas[i] = cargar_textura(renderer, ruta);
    }

    char ruta_dorso[100];
    if (estado_juego->config.set_dorso == 0)
    {
        strcpy(ruta_dorso, "img/dorso_0.jpg");
    }
    else
    {
        strcpy(ruta_dorso, "img/dorso_1.jpg");
    }

    estado_juego->textura_dorso = cargar_textura(renderer, ruta_dorso);
    estado_juego->juego_iniciado = 1;
}

void procesar_eventos_juego(s_EstadoJuego *estado_juego, SDL_Event *evento, int mouseX, int mouseY, EstadoMenu *estado_menu)
{
    if (!estado_juego || !estado_juego->tablero)
        return;

    if (evento->type == SDL_MOUSEBUTTONDOWN)
    {
        if (detectar_boton_menu(mouseX, mouseY))
        {
            finalizar_juego(estado_juego);
            estado_menu->pantalla_actual = PANTALLA_MENU;
            return;
        }

        int indice = convertir_clic_a_indice(mouseX, mouseY, estado_juego->config.filas, estado_juego->config.columnas);

        if (indice == -1)
            return;

        s_Carta *carta = (s_Carta *)obtenerVec(
            estado_juego->tablero,
            indice);

        if (carta->estado != CARTA_OCULTA)
            return;

        if (estado_juego->turno_actual == ESTADO_ESPERANDO_PRIMERA)
        {
            estado_juego->carta_seleccionada_1 = indice;
            carta->estado = CARTA_VISIBLE;
            estado_juego->turno_actual = ESTADO_ESPERANDO_SEGUNDA;
        }

        else if (estado_juego->turno_actual == ESTADO_ESPERANDO_SEGUNDA)
        {
            estado_juego->carta_seleccionada_2 = indice;
            carta->estado = CARTA_VISIBLE;
            estado_juego->turno_actual = ESTADO_COMPARANDO;
        }
    }
}

void actualizar_juego(s_EstadoJuego *estado_juego, EstadoMenu *estado_menu)
{
    if (!estado_juego || !estado_juego->tablero)
        return;

    if (estado_juego->turno_actual == ESTADO_COMPARANDO)
    {
        if (!estado_juego->esperando_delay)
        {
            estado_juego->tiempo_comparacion = SDL_GetTicks();
            estado_juego->esperando_delay = 1;
            return;
        }

        if (SDL_GetTicks() - estado_juego->tiempo_comparacion < 800)
            return;

        s_Carta *c1 = (s_Carta *)obtenerVec(estado_juego->tablero, estado_juego->carta_seleccionada_1);

        s_Carta *c2 = (s_Carta *)obtenerVec(estado_juego->tablero, estado_juego->carta_seleccionada_2);

        s_Jugador *jugador_actual =
            (estado_juego->jugador_actual == 1)
                ? &estado_juego->jugador1
                : &estado_juego->jugador2;

        if (comparar_cartas(c1, c2))
        {
            procesar_acierto(jugador_actual, c1, c2);

            if (verificar_fin_juego(estado_juego->tablero))
            {
                finalizar_juego(estado_juego);
                estado_menu->pantalla_actual = PANTALLA_MENU;
                return;
            }
        }
        else
        {
            procesar_fallo(jugador_actual, c1, c2);

            if (estado_juego->modo_competitivo)
            {
                estado_juego->turno_actual =
                    (estado_juego->turno_actual == 1) ? 2 : 1;
            }
        }

        estado_juego->carta_seleccionada_1 = -1;
        estado_juego->carta_seleccionada_2 = -1;
        estado_juego->turno_actual = ESTADO_ESPERANDO_PRIMERA;
        estado_juego->esperando_delay = 0;
    }
}