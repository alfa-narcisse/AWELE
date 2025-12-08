#include "umbrella.h"


int main(int argc, char* argv[]){


    SDL_Window *win=SDL_CreateWindow("Menu",1280,720,0);
    SDL_Init(SDL_INIT_VIDEO);


    AfficheMenu(win);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
    /*
    SDL_Renderer* renderer = run_engine();
    if (!renderer) return -1;


    TTF_Font* ListeFont[2];
    TTF_Font* policeButton = load_font("../assets/fonts/font_button.ttf", 20);
    if (!policeButton){
        stop_engine(renderer, NULL,0);
        return -1;
    }

    ListeFont[0] = policeButton;
    TTF_Font* policePlateau = load_font("../assets/fonts/font_plateau.ttf", 60);
    if (!policePlateau){
        stop_engine(renderer, ListeFont, 1);
        return -1;
    }
    ListeFont[1] = policePlateau;

    // Ici, le reste de la logique du jeu serait implémentée
    bool running = true;
    bool VsAI = true;
    bool player1Turn = true;
    SDL_Event event;
    int POS_TROUS[12][2] = {
        {1250+ 115, 225},{1025+ 115, 225}, {800+ 115, 225}, {575+ 115, 225},{350+ 115, 225},{125+ 115, 225},   
        {125+ 115, 525}, {350+ 115, 525}, {575+ 115, 525}, {800+ 115, 525}, {1025+ 115, 525}, {1250+ 115, 525}
    };
    int ListePions[12] = {4,4,4,4,4,4,4,4,4,4,4,4};
    int depht = 7;
    int scorePlayer1 = 0;
    int scorePlayer2 = 0;
    Button *EndGame;
    SDL_FRect buttonRec;
    buttonRec.x = 1495;
    buttonRec.y = 10;
    buttonRec.w = 50;
    buttonRec.h = 20;

    EndGame->hovered = false;
    EndGame->pressed = false;
    EndGame->rect = &buttonRec;
    snprintf(EndGame->text, sizeof(EndGame->text), "END GAME");
    
    SDL_Texture* plateauTexture = createSurfaceTexturePlateau(renderer);
    if (!plateauTexture){
        stop_engine(renderer, ListeFont, 2);
    }
    while(running){
        displayAllWithDelay(renderer,plateauTexture,policePlateau, policeButton,ListePions,POS_TROUS,scorePlayer1,scorePlayer2,VsAI,1, EndGame);
        while (SDL_PollEvent(&event)){
            handleQuitEvent(&event,&running);
            handleButtonEvent(EndGame,&event);
            if (EndGame->pressed){
                displayFinality(renderer, policePlateau,ListePions,scorePlayer1,scorePlayer2, VsAI);
                running = false;
            }
            else if (VsAI && player1Turn){
                int pos = bestChoiceAI(ListePions, depht);
                if (pos!=-1){
                    SDL_Delay(500);
                    doTheMoveDisplay(POS_TROUS,ListePions,pos,renderer,policePlateau,policeButton,plateauTexture,player1Turn,&scorePlayer1, &scorePlayer2, VsAI,EndGame);
                    player1Turn = false;
                    if(ultimateState(ListePions, false)){
                        displayFinality(renderer, policePlateau,ListePions,scorePlayer1,scorePlayer2, VsAI);
                        running = false;
                    }
                }
            }
            else if (!VsAI && player1Turn){
                if(-1 != executePlayerCommand(&event, renderer, policePlateau, policeButton, plateauTexture, ListePions,POS_TROUS,player1Turn, &scorePlayer1, &scorePlayer2,VsAI,EndGame)){
                    player1Turn = false;
                    if(ultimateState(ListePions, false)){
                        displayFinality(renderer, policePlateau,ListePions,scorePlayer1,scorePlayer2, VsAI);
                        running = false;
                    }
                };
            }
            else if (!player1Turn){
                if(-1 != executePlayerCommand(&event, renderer, policePlateau,policeButton, plateauTexture, ListePions,POS_TROUS,player1Turn, &scorePlayer1, &scorePlayer2, VsAI, EndGame)){
                    player1Turn = true;
                    if(ultimateState(ListePions, true)){
                        displayFinality(renderer, policePlateau,ListePions,scorePlayer1,scorePlayer2, VsAI);
                        running = false;
                    }
                }
            }
        }
    }


    stop_engine(renderer, ListeFont, 2);
    return 0;
    */
}