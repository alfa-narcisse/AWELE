#ifndef PLAYER_H
#define PLAYER_H
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>
#include "outils.h"


// Declare player-related functions here

int executePlayerCommand(
    SDL_Event *event,
    SDL_Renderer * renderer,
    TTF_Font* policePlateau,
    TTF_Font* policeButton,
    SDL_Texture* plateauTexture,
    int PlateauList[12],
    int POS_TROUS[12][2],
    bool player1Turn,
    int* scorePlayer1,
    int* scorePlayer2,
    bool VsAI,
    Button*btn
);

#endif // PLAYER_H