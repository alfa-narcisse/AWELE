#include <SDL3/SDL.h>
#include <stdbool.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include "engine.h"

#define WINDOW_WIDTH 1700
#define WINDOW_HEIGHT 800
#define FONT_PATH "../assets/fonts/font_film.ttf"
#define FONT_SIZE 40
#define LIST_PIONS_INIT {4,4,4,4,4,4,4,4,4,4,4,4}


SDL_Renderer* run_engine(void){
    if (!SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) < 0) {
        fprintf(stderr, "Erreur d'initialisation: %s\n", SDL_GetError());
        return NULL;
    }
    if (TTF_Init() == -1){
        fprintf(stderr, "Erreur de lancement: %s", SDL_GetError());
        return NULL;
    }
    SDL_Window* fenetre;
    SDL_Renderer* renderer; 
    bool p = SDL_CreateWindowAndRenderer("Game", WINDOW_WIDTH, WINDOW_HEIGHT, 
                                         SDL_WINDOW_ALWAYS_ON_TOP, 
                                         &fenetre, &renderer);
    if (!p) {
        fprintf(stderr, "Erreur de création de la fenêtre et du renderer: %s\n", SDL_GetError());
        return NULL;
    }
    return renderer;
}

TTF_Font* load_font(const char* font_path, int font_size){
    TTF_Font *police = TTF_OpenFont(font_path, font_size);
    if (!police){
        fprintf(stderr, "Erreur de chargement de la police: %s", SDL_GetError());
        return NULL;
    } 
    return police;
}

void stop_engine(SDL_Renderer*renderer, TTF_Font* ListFonts[], int nFont){
    for (int i =0;i<nFont; i++){
        TTF_CloseFont(ListFonts[i]);
    }
    SDL_Window* fenetre = SDL_GetRenderWindow(renderer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(fenetre);
    TTF_Quit();
    SDL_Quit();
}
