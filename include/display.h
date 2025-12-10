#ifndef DISPLAY_H
#define DISPLAY_H
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include "outils.h"

// Affichage des bouto
//Afficher le score des joueurs dans le coin inférieur gauche de l'écran
void displayScores(SDL_Renderer* renderer, TTF_Font*police,bool VsAI, int scorePlayer1, int ScorePlayer2);


//Afficher le contenu des trous sur le plateau
void displayContainsOfHoles(int ListePions[12], SDL_Renderer * renderer, TTF_Font* police, int POS_TROUS[12][2], int POS_RECT[12][2]);

//Charger la texture de fond du plateau depuis un fichier image et le mettre dans renderer
SDL_Texture* loadPlateauBackground(SDL_Renderer * renderer, const char* filepath);


//Dessiner la texture de fond du plateau sur le renderer
void drawGameBG(SDL_Renderer* plateauRenderer);  

//Créer une texture de la surface du plateau de jeu
SDL_Texture* createSurfaceTexturePlateau(SDL_Renderer * renderer);

//Afficher tout avec un délai
void displayPlateauWithDelay(
    SDL_Renderer * rendererPlateau,
    SDL_Texture*plateauTexture, 
    TTF_Font* policePlateau,
    Button*ListButtons[],
    int nbButtons,
    int POS_TROUS[12][2],
    int POS_RECT[12][2],
    int ListePions[12],
    int scorePlayer1,
    int scorePlayer2,
    bool VsAI,
    int delayMs
);

#endif // DISPLAY_H