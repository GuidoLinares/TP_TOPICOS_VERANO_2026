#include <stdint.h>
#include <SDL_ttf.h>

int inicializar_ttf();
TTF_Font* cargar_font(const char *path, uint32_t fontSize);
void inicializar_fonts();
void cerrar_font(TTF_Font *fuente);
void finalizar_ttf();
