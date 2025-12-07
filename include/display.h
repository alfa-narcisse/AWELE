#ifndef DISPLAY_H
#define DISPLAY_H
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include "outils.h"

// Affichage des boutons
void drawButton(SDL_Renderer* renderer, Button* button, TTF_Font* police);


//Afficher le score des joueurs dans le coin inférieur gauche de l'écran
void displayScores(SDL_Renderer* renderer, TTF_Font*police,bool VsAI, int scorePlayer1, int ScorePlayer2);


//Afficher le message de victoire au centre de l'écran
void displayVictory(SDL_Renderer * renderer, TTF_Font* police, int ScorePlayer1, int ScorePlayer2, bool VsAI);


//Afficher le contenu des trous sur le plateau
void displayContainsOfHoles(int ListePions[12], SDL_Renderer * renderer, TTF_Font* police, int POS_TROUS[12][2] );

//Charger la texture de fond du plateau depuis un fichier image et le mettre dans renderer
SDL_Texture* loadPlateauBackground(SDL_Renderer * renderer, const char* filepath);


//Dessiner la texture de fond du plateau sur le renderer
void drawPlateauBackground(SDL_Renderer * renderer, SDL_Texture* plateauTexture);   

//Créer une texture de la surface du plateau de jeu
SDL_Texture* createSurfaceTexturePlateau(SDL_Renderer * renderer);

//Afficher tout avec un délai
void displayAllWithDelay(
    SDL_Renderer * renderer,
    SDL_Texture*plateauTexture, 
    TTF_Font* policePlateau,
    TTF_Font* policeButton,
    int ListePions[12],
    int POS_TROUS[12][2],
    int scorePlayer1,
    int scorePlayer2,
    bool VsAI,
    int delayMs,
    Button* btn
);

void displayFinality(
    SDL_Renderer*renderer,
    TTF_Font*policePlateau,
    int ListePions[12],
    int scorePlayer1,
    int scorePlayer2,
    bool VsAI
);
#endif // DISPLAY_H