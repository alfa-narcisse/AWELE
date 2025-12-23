#ifndef OUTILS_H
#define OUTILS_H
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "sound.h"


// ==== NOTION IMPORTANTE ====
// Dans toutes les fonctions ci-dessous, on considère que le plateau est représenté par un tableau*d'entiers de taille 12,
// où les indices 0 à 5 représentent les trous du joueur 1 (potentiellement IA lorsqu'on utilise ces fonctions dans ce contexte)
// et les indices 6 à 11 représentent les trous du joueur 2 (humain).
// player1Turn est un booléen qui indique si c'est le tour du joueur 1 (true) ou du joueur 2 (false).
// ==========================


// tout ce qui concerne le button
typedef struct Button{
    SDL_FRect rect;
    SDL_Texture* normal;
    SDL_Texture* hover;
    SDL_Texture* pressed;
    bool isHover;
    bool isPressed;
} Button;

bool pointDansRect(float x, float y, SDL_FRect r);

void renderbutton(SDL_Renderer *ren,Button *b);

void freeButton(Button *btn);



// Tout ce qui concerne le plateau
// Copier le plateau
void CopyPlateau(int srcListe[12], int dstListe[12]);

// Obtenir le nombre de pions du joueur,
// Elle retourne le n: nombre des pions du joueur si c'est le joueur 1, le négatif sinon
int getNumPionsOfPlayer(int PlateauList[12], bool player1Turn);

// Vérifier si l'adversaire est affamé
bool isMyOpponentStarving(int PlateauList[12], bool player1Turn);


// Obtenir les coups possibles pour un joueur
// Elle remplit le tableau dstPossibleMoves avec les indices des trous possibles et retourne le nombre de coups possibles
int GetPossibleMoves(int PlateauList[12],  int dstPossibleMoves[6],bool player1Turn);

// Vérifier la finalité du jeu
bool ultimateState(int PlateauList[12], bool player1Turn);

bool detectLoop(int PlateauList[12]);


// Executer un mouvement à une position donnée
// Affiche l'état du plateau après le mouvement
// Met à jour les scores des joueurs

void doTheMoveDisplay(
    SDL_Renderer*plateauRenderer,
    TTF_Font* policePlateau,
    SDL_Texture *bgTexture,
    SDL_Texture *graineTexture,
    SDL_Texture *handTexture,
    SDL_Texture *handTextureLeft,

    Button*ListButtons[],
    int nbButtons,

    int POS_TROUS[12][2],
    int POS_RECT[12][2],

    int PlateauList[12], 
    int posInit,

    bool VsAI, 
    bool player1Turn,

    int* scorePlayer1, 
    int* scorePlayer2,

    AudioStreamInstance* ListePionSounds[4]
    );


#endif // OUTILS_H