#include "umbrella.h"


int main(int argc, char* argv[]){
    SDL_Renderer* renderer = run_engine();
    if (!renderer) return -1;

    TTF_Font* police = load_font("../assets/fonts/font_film.ttf", 40);
    if (!police){
        stop_engine(police, NULL, renderer);
        return -1;
    }

    // Ici, le reste de la logique du jeu serait implémentée
    bool running = true;
    bool VsAI = true;
    bool player1Turn = true;
    SDL_Event event;
    int POS_TROUS[12][2] = {
        {125, 125}, {350, 125}, {575, 125}, {800, 125}, {1025, 125}, {1250, 125},
        {125, 450}, {350, 450}, {575, 450}, {800, 450}, {1025, 450}, {1250, 450}
    };
    int ListePions[12] = {4,4,4,4,4,4,4,4,4,4,4,4};
    int depht = 6;
    int scorePlayer1 = 0;
    int scorePlayer2 = 0;
    
    
    SDL_Texture* plateauTexture = createSurfaceTexturePlateau(renderer);
    while(running){
        displayAllWithDelay(renderer,plateauTexture,police,ListePions,POS_TROUS,scorePlayer1,scorePlayer2,1);
        while (SDL_PollEvent(&event)){
            handleQuitEvent(&event,&running);
            if (VsAI && player1Turn){
                int pos = bestChoiceAI(ListePions, depht);
                if (pos!=-1){
                    doTheMoveDisplay(ListePions,pos,renderer,police,plateauTexture,player1Turn,&scorePlayer1, &scorePlayer2);
                }
                if (ultimateState(ListePions,player1Turn)){
                    scorePlayer1 += getNumPionsOfPlayer(ListePions, player1Turn);
                    scorePlayer2 -= getNumPionsOfPlayer(ListePions, !player1Turn);
                    displayVictory(renderer,police,scorePlayer1,scorePlayer2);
                    SDL_RenderPresent(renderer);
                    SDL_Delay(5000);
                    running = false;
                }
                player1Turn = false;
            }
            else if (!VsAI && player1Turn){
                executePlayerCommand(&event, renderer, police, plateauTexture, ListePions,POS_TROUS,&player1Turn, &scorePlayer1, &scorePlayer2);
                if (ultimateState(ListePions,true)){
                    scorePlayer1 += getNumPionsOfPlayer(ListePions, player1Turn);
                    scorePlayer2 -= getNumPionsOfPlayer(ListePions, !player1Turn);
                    displayVictory(renderer,police,scorePlayer1,scorePlayer2);
                    SDL_RenderPresent(renderer);
                    SDL_Delay(5000);
                    running = false;
                }
            }
            else if (!player1Turn)
            executePlayerCommand(&event, renderer, police, plateauTexture, ListePions,POS_TROUS,&player1Turn, &scorePlayer1, &scorePlayer2);
            if (ultimateState(ListePions,false)){
                scorePlayer1 += getNumPionsOfPlayer(ListePions, player1Turn);
                scorePlayer2 -= getNumPionsOfPlayer(ListePions, !player1Turn);
                displayVictory(renderer,police,scorePlayer1,scorePlayer2);
                SDL_RenderPresent(renderer);
                SDL_Delay(5000);
                running = false;
            }

        }
    }


    if (!plateauTexture){   
        stop_engine(police, NULL, renderer);
        return -1;
    }
    stop_engine(police, NULL, renderer);
    return 0;
}