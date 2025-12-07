#include <SDL3/SDL.h>
#include "event.h"
#include "outils.h"

static int distanceSquared(int x1, int y1, int x2, int y2) {
    int dx = x1 - x2;
    int dy = y1 - y2;
    return dx * dx + dy * dy;
}

int getClickedHole(int x, int y, int POS_TROUS[12][2]) {
    for (int i = 0; i < 12; i++) {
        int distSq = distanceSquared(x, y, POS_TROUS[i][0], POS_TROUS[i][1]);
        if (distSq <= 100 * 100) { // Rayon de 100 pixels
            return i;
        }
    }
    return -1; // Aucun trou cliqué
}
void handleQuitEvent(SDL_Event *event, bool *running) {
    if (event->type == SDL_EVENT_QUIT) {
        *running = false;
    }
}

void handleButtonEvent(Button *btn, const SDL_Event *ev) {
    if (ev->type == SDL_EVENT_MOUSE_MOTION) {
        float mx = ev->motion.x;
        float my = ev->motion.y;
        btn->hovered = (mx >= btn->rect->x && mx <= btn->rect->x + btn->rect->w &&
                        my >= btn->rect->y && my <= btn->rect->y + btn->rect->h);
    }

    if (ev->type == SDL_EVENT_MOUSE_BUTTON_DOWN && btn->hovered) {
        btn->pressed = true;
    }
}


int handleMouseButtonDownEvent(SDL_Event *event, bool isPlayer1Turn,int PlateauList[12], int POS_TROUS[12][2]) {
    int possibleMoves[6];
    int nbPos = GetPossibleMoves(PlateauList, possibleMoves, isPlayer1Turn); //
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN ) {
        int x = event->button.x;
        int y = event->button.y;
        int pos = getClickedHole(x, y, POS_TROUS);
        if (pos != -1) {
            // Vérifier si le trou cliqué est un coup possible
            for (int i = 0; i < nbPos; i++) {
                if (possibleMoves[i] == pos) {
                    return pos; // Retourner la position du trou cliqué
                }
            }
        }
    }
    return -1;
}