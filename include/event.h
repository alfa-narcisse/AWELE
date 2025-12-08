#ifndef EVENT_H
#define EVENT_H

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "outils.h"


// Obtenir l'indice du trou cliqué en fonction des coordonnées de la souris
int getClickedHole(int x, int y, int POS_TROUS[12][2]);

// Gérer l'événement de fermeture de la fenêtre
void handleQuitEvent(SDL_Event *event, bool *running);

// Gérer l'événement de clic de souris
int handleMouseButtonDownEvent(SDL_Event *event, bool isPlayer1Turn,int PlateauList[12], int POS_TROUS[12][2]);


void handleButtonEvent(Button *btn, const SDL_Event *ev);

#endif // EVENT_H