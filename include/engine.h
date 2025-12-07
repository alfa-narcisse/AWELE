#ifndef ENGINE_H
#define ENGINE_H
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

// Initialiser le moteur SDL, créer la fenêtre et le renderer
SDL_Renderer* run_engine(void);

// Charger une police de caractères depuis un fichier
TTF_Font* load_font(const char* font_path, int font_size);

// Arrêter le moteur SDL et libérer les ressources
void stop_engine(SDL_Renderer*renderer, TTF_Font* ListFonts[], int nFont);
#endif // ENGINE_H