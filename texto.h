#include <stdint.h>
#include <SDL_ttf.h>

int inicializar_ttf();
TTF_Font* cargar_font(const char *path, uint32_t fontSize);
void inicializar_fonts();
void dibujar_texto_ttf(SDL_Renderer *renderer, const char *texto, int x, int y, int tamanio, SDL_Color color);
void cerrar_font(TTF_Font *fuente);
void finalizar_ttf();
