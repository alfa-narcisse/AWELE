#ifndef RUN_GAME_H
#define RUN_GAME_H

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "outils.h"


// Obtenir l'indice du trou cliqué en fonction des coordonnées de la souris
int getClickedHole(int x, int y, int POS_TROUS[12][2]);

// Gérer l'événement de fermeture de la fenêtre

// Gérer l'événement de clic de souris
int handleMouseButtonDownEvent(int x, int y, bool isPlayer1Turn,int PlateauList[12], int POS_TROUS[12][2]);


void launch_game(SDL_Window *window, int ListePions[12],bool VsAiMode,bool start, bool player1Turn, int depht,int scorePlayer1, int scorePlayer2);

#endif // EVENT_H