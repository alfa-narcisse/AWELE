#include "player.h"
#include "outils.h"
#include "event.h"
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>


// Ici, vous pouvez implémenter des fonctions liées aux joueurs,
// comme la gestion des tours, les interactions avec l'IA, etc.

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
) {
    int p = handleMouseButtonDownEvent(event, player1Turn, PlateauList,POS_TROUS);
    if (p!=-1){
        doTheMoveDisplay(
            POS_TROUS,
            PlateauList,
            p,
            renderer,
            policePlateau,
            policeButton,
            plateauTexture,
            player1Turn,
            scorePlayer1,
            scorePlayer2,
            VsAI,
            btn
        );
        return 0;
    }
    return -1;
 }