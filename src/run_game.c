#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>
#include "run_game.h"
#include "outils.h"
#include "ai.h"
#include "display.h"
#include "menu.h"


int POS_TROUS[12][2]={{1065, 300}, {895, 300}, {725, 300}, {555, 300}, {385, 300}, {215, 300},
                     {215, 520}, {385, 520}, {555, 520}, {725, 520}, {895, 520}, {1065, 520}};


int POS_RECT[12][2] = {{985,135},{815,135},{645,135},{475,135},{305,135},{135,135},
                        {135,615},{305,615},{475,615},{645,615},{815,615},{985,615}};


static int distanceSquared(int x1, int y1, int x2, int y2) {
    int dx = x1 - x2;
    int dy = y1 - y2;
    return dx * dx + dy * dy;
}

int getClickedHole(int x, int y, int POS_TROUS[12][2]) {
    for (int i = 0; i < 12; i++) {
        int distSq = distanceSquared(x, y, POS_TROUS[i][0], POS_TROUS[i][1]);
        if (distSq <= 80 * 80) { // Rayon de 100 pixels
            return i;
        }
    }
    return -1; // Aucun trou cliqué
}



int handleMouseButtonDownEvent(int x, int y, bool isPlayer1Turn,int PlateauList[12], int POS_TROUS[12][2]) {
    int possibleMoves[6];
    int nbPos = GetPossibleMoves(PlateauList, possibleMoves, isPlayer1Turn); 
    int pos = getClickedHole(x, y, POS_TROUS);
    if (pos != -1) {
            // Vérifier si le trou cliqué est un coup possible
        for (int i = 0; i < nbPos; i++) {
            if (possibleMoves[i] == pos) {
                return pos; // Retourner la position du trou cliqué
            }
        }
    }
    return -1;
}


void confirmGoToMenuPopUp(
                      SDL_Window *window,
                      int ListePions[12],
                      bool VsAI,
                      bool player1Turn,
                      int scorePlayer1,
                      int scorePlayer2,
                      int depht
                        );


void popUpPausedGame(SDL_Window *window,
                      int ListePions[12],
                      bool VsAI,
                      bool player1Turn,
                      int scorePlayer1,
                      int scorePlayer2,
                      int depht
                    );


static void popUpFinalityOfGame(SDL_Window *window,
                        int ListePions[12],
                        int scorePlayer1,
                        int scorePlayer2,
                        bool VsAI,
                        int depht
                    );

void launch_game(SDL_Window *window, int ListePions[12],bool VsAiMode, bool player1Turn, int depht,int scorePlayer1, int scorePlayer2){
    bool running = true;
    bool quitDirectly = false;
    bool goToMenu = false;
    bool pauseGame = false;
    bool finalState = false;
    bool initial = true;
    SDL_Event event;
    
    TTF_Font*police = TTF_OpenFont("../assets/fonts/font_plateau.ttf", 60);

    // Création du renderer du background, y compris les bouttons de bases dans le coin haut gauche
    SDL_Renderer* bgRenderer = SDL_CreateRenderer(window,NULL);
    if (!bgRenderer){
        fprintf(stderr, "Erreur dans la création du renderer 1: %s", SDL_GetError());
        return;
    }
    

    //Renderer responsable de l'affichage du contenu du plateau

   
    //Renderer responsable de l'affichage des scores de chaque joueurs

    // Chargement des affichages de base (backGround)
    SDL_Texture *bgTexture = IMG_LoadTexture(bgRenderer,"../assets/images/plateauAwale.png");
    if (!bgTexture){
        fprintf(stderr, "Erreur dans la création du renderer: %s", SDL_GetError());
        return;
    }

    // Création des buttons qui vont s'afficher pendant le jeu

    Button btn_menuRunning;
    btn_menuRunning.isHover = false;
    btn_menuRunning.isPressed = false;
    btn_menuRunning.normal = IMG_LoadTexture(bgRenderer, "../assets/images/buttonMenu.png");
    btn_menuRunning.pressed = IMG_LoadTexture(bgRenderer, "../assets/images/buttonMenuP.png");
    btn_menuRunning.hover = IMG_LoadTexture(bgRenderer, "../assets/images/buttonMenuH.png");
    btn_menuRunning.rect = (SDL_FRect){
        .x = 55,
        .y = 10,
        .w = 70,
        .h = 70
    };

    Button btn_pauseGame;
    btn_pauseGame.isHover = false;
    btn_pauseGame.isPressed = false;
    btn_pauseGame.normal = IMG_LoadTexture(bgRenderer, "../assets/images/buttonExit.png");
    btn_pauseGame.pressed = IMG_LoadTexture(bgRenderer, "../assets/images/buttonExitP.png");
    btn_pauseGame.hover = IMG_LoadTexture(bgRenderer, "../assets/images/buttonExitH.png");
    btn_pauseGame.rect = (SDL_FRect){
        .x = 1155,
        .y = 10,
        .w = 70,
        .h = 70
    };

    Button* ListButtons[2];
    ListButtons[0] = &btn_menuRunning;
    ListButtons[1] = &btn_pauseGame;
    int nbButtons = 2;
    while(running){
        displayPlateauWithDelay(bgRenderer,bgTexture,police,ListButtons,nbButtons,POS_TROUS,POS_RECT, ListePions,scorePlayer1,scorePlayer2,VsAiMode,1);
        
        while (SDL_PollEvent(&event) || initial)
        {   
            renderbutton(bgRenderer, &btn_menuRunning);
            renderbutton(bgRenderer, &btn_pauseGame);
            float px,py;
            SDL_GetMouseState(&px,&py);
            // La souris touche-t-elle le bouton pour pauser le jeu?
            btn_pauseGame.isHover=pointDansRect(px,py,btn_pauseGame.rect);
            // La souris touche-t-elle le bouton pour aller au menu?
            btn_menuRunning.isHover=pointDansRect(px,py, btn_menuRunning.rect);

          
            if (event.type == SDL_EVENT_QUIT || (event.type ==SDL_EVENT_KEY_DOWN &&event.key.key == SDLK_ESCAPE)){
                    running = false;
                    quitDirectly = false;
                }
            
            if (VsAiMode && player1Turn){
                int pos = bestChoiceAI(ListePions, depht);
                if (pos != -1){
                    doTheMoveDisplay(bgRenderer, police,bgTexture,ListButtons,nbButtons,POS_TROUS, POS_RECT, ListePions, pos,VsAiMode, player1Turn,&scorePlayer1, &scorePlayer2);
                    finalState = ultimateState(ListePions, player1Turn);
                    player1Turn = false;
                }
                finalState = ultimateState(ListePions, player1Turn);
                if (finalState){
                    running = false;
                    break;
                }
                 
            }
            switch (event.type)
                {
                case SDL_EVENT_MOUSE_BUTTON_DOWN :
                    if (btn_pauseGame.isHover) btn_pauseGame.isPressed = true;
                    if (btn_menuRunning.isHover) btn_menuRunning.isPressed = true;
                    if ((!VsAiMode && player1Turn) || !player1Turn){
                        int p = handleMouseButtonDownEvent(px,py,player1Turn, ListePions, POS_TROUS);
                        if (p!=-1){
                            doTheMoveDisplay(bgRenderer, police,bgTexture,ListButtons,nbButtons,POS_TROUS,POS_RECT, ListePions, p,VsAiMode, player1Turn,&scorePlayer1, &scorePlayer2);
                            finalState = ultimateState(ListePions, player1Turn);
                            if (finalState){
                                running = false;
                                break;
                            }
                            player1Turn = !player1Turn;
                        }
                    }
                    break;
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    if (btn_pauseGame.isHover && btn_pauseGame.isPressed) {
                        btn_pauseGame.isPressed = false;
                        pauseGame = true;
                        running = false;
                        break;
                    }
                    if (btn_menuRunning.isHover && btn_menuRunning.isPressed) {
                        running = false;
                        goToMenu = true;
                        btn_menuRunning.isPressed = false;
                        break;
                    }
                    break;
            }
            initial = false; // Pour lancer directement l'IA même si on ne fait aucun mouvement
        } 
    }
    freeButton(&btn_menuRunning);
    freeButton(&btn_pauseGame);
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyRenderer(bgRenderer);
    if (goToMenu){
        confirmGoToMenuPopUp(window, ListePions, VsAiMode, player1Turn, scorePlayer1, scorePlayer2, depht);
    }
    else if(pauseGame){
        popUpPausedGame(window, ListePions, VsAiMode, player1Turn, scorePlayer1,scorePlayer2,depht);
    }
    else if (finalState){
        popUpFinalityOfGame(window, ListePions, scorePlayer1, scorePlayer2, VsAiMode, depht);
    }
    else if (quitDirectly){
        TTF_Quit();
        SDL_DestroyWindow(window);
    }
}


void popUpFinalityOfGame(
    SDL_Window *window,
    int ListePions[12],
    int scorePlayer1,
    int scorePlayer2,
    bool VsAI,
    int depht
)
{
    bool quitDirectly = false;
    bool running = true;
    bool toMenu = false;
    bool replay = false;
    SDL_Event event;
    scorePlayer1 += getNumPionsOfPlayer(ListePions, true);
    scorePlayer2 -= getNumPionsOfPlayer(ListePions, false);

    SDL_Renderer* victoryRenderer = SDL_CreateRenderer(window, NULL);
    if (!victoryRenderer){
        fprintf(stderr, "Erreur dans la création du renderer 3: %s", SDL_GetError());
        return;
    }
    SDL_Texture *bgTexture = IMG_LoadTexture(victoryRenderer,"../assets/images/plateauAwale.png");
    if (!bgTexture){
        fprintf(stderr, "Erreur du chargement de l'image: %s", SDL_GetError());
        return;
    }
    SDL_Texture* victoryTexture;
    if (scorePlayer1==scorePlayer2){
        victoryTexture = IMG_LoadTexture(victoryRenderer, "../assets/images/equality.png");
    }
    else if(scorePlayer1>scorePlayer2 && VsAI){
        victoryTexture = IMG_LoadTexture(victoryRenderer, "../assets/images/aiWin.png");
    }
    else if(scorePlayer1>scorePlayer2 && !VsAI){
        victoryTexture = IMG_LoadTexture(victoryRenderer, "../assets/images/player1Win.png");
    }
    else{
        victoryTexture = IMG_LoadTexture(victoryRenderer, "../assets/images/player2Win.png");
        if (VsAI){
            depht+=1;
        } 
    }

    if (!victoryTexture){
        fprintf( stderr,"Erreur dans le chargement de la surface: %s", SDL_GetError());
        return;
    }

    Button btn_replay;
    btn_replay.isHover = false;
    btn_replay.isPressed = false;
    btn_replay.normal = IMG_LoadTexture(victoryRenderer, "../assets/images/buttonReplay.png");
    btn_replay.pressed = IMG_LoadTexture(victoryRenderer, "../assets/images/buttonReplayP.png");
    btn_replay.hover = IMG_LoadTexture(victoryRenderer, "../assets/images/buttonReplayH.png");
    btn_replay.rect = (SDL_FRect){
        .x = 690,
        .y = 565,
        .w = 70,
        .h = 70
    };

    Button btn_tomenu;
    btn_tomenu.isHover = false;
    btn_tomenu.isPressed = false;
    btn_tomenu.normal = IMG_LoadTexture(victoryRenderer, "../assets/images/buttonMenu.png");
    btn_tomenu.pressed = IMG_LoadTexture(victoryRenderer, "../assets/images/buttonMenuP.png");
    btn_tomenu.hover = IMG_LoadTexture(victoryRenderer, "../assets/images/buttonMenuH.png");
    btn_tomenu.rect = (SDL_FRect){
        .x = 520,
        .y = 565,
        .w = 70,
        .h = 70
    };



    SDL_FRect victoryRect = {
        .h = 360,
        .w = 360,
        .x = 385+75,
        .y = 200
    };
    
    while(running){
        SDL_RenderTexture(victoryRenderer,bgTexture,NULL,NULL);
        SDL_RenderTexture(victoryRenderer, victoryTexture, NULL, &victoryRect);
        renderbutton(victoryRenderer, &btn_replay);
        renderbutton(victoryRenderer, &btn_tomenu);
        SDL_RenderPresent(victoryRenderer);
        while(SDL_PollEvent(&event)){
            float px,py;
            SDL_GetMouseState(&px,&py);
            // La souris touche-t-elle le bouton pour pauser le jeu?
            btn_replay.isHover=pointDansRect(px,py,btn_replay.rect);
            // La souris touche-t-elle le bouton pour aller au menu?
            btn_tomenu.isHover=pointDansRect(px,py, btn_tomenu.rect);
            switch(event.type){
                case SDL_EVENT_QUIT :{
                    running = false;
                    quitDirectly = false;
                    break;
                }
                
                case SDL_EVENT_KEY_DOWN:{
                    if (event.key.key == SDLK_ESCAPE)
                        running = false;
                        quitDirectly = false;
                    break;
                    }
                case SDL_EVENT_MOUSE_BUTTON_DOWN:{
                    if (btn_tomenu.isHover) btn_tomenu.isPressed = true;
                    if (btn_replay.isHover) btn_replay.isPressed = true;
                    break;
                    }
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    if (btn_replay.isHover && btn_replay.isPressed) {
                        btn_replay.isPressed = false;
                        replay = true;
                        running = false;
                        break;
                    }
                    if (btn_tomenu.isHover && btn_tomenu.isPressed) {
                        running = false;
                        toMenu = true;
                        btn_tomenu.isPressed = false;
                        break;
                    }
                
                }
            }
        }
    int ListeP[12] = {4,4,4,4,4,4,4,4,4,4,4,4};
    freeButton(&btn_replay);
    freeButton(&btn_tomenu);
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyTexture(victoryTexture);
    SDL_DestroyRenderer(victoryRenderer);

    if (replay){
        launch_game(window, ListeP,VsAI, true,depht,0,0);
    }

    else if(toMenu){
        AfficheMenu(window, ListeP);
    }
    else if (quitDirectly){
        TTF_Quit();
        SDL_DestroyRenderer(victoryRenderer);
        SDL_DestroyWindow(window);
    }    
}


void popUpPausedGame(
    SDL_Window *window,
    int ListePions[12],
    bool VsAI,
    bool player1Turn,
    int scorePlayer1,
    int scorePlayer2,
    int depht
){

    bool quitDirectly = false;
    bool running = true;
    bool toMenu = false;
    bool replay = false;
    bool endGame = false;
    SDL_Event event;

    SDL_Renderer* pauseRenderer = SDL_CreateRenderer(window, NULL);
    if (!pauseRenderer){
        fprintf(stderr, "Erreur dans la création du renderer 4 : %s", SDL_GetError());
        return;
    }
    SDL_Texture *bgTexture = IMG_LoadTexture(pauseRenderer,"../assets/images/plateauAwale.png");
    if (!bgTexture){
        fprintf(stderr, "Erreur du chargement de l'image: %s", SDL_GetError());
        return;
    }
    SDL_Texture* pauseTexture = IMG_LoadTexture(pauseRenderer,"../assets/images/pauseGame.png");

    Button btn_replay;
    btn_replay.isHover = false;
    btn_replay.isPressed = false;
    btn_replay.normal = IMG_LoadTexture(pauseRenderer, "../assets/images/buttonReplay.png");
    btn_replay.pressed = IMG_LoadTexture(pauseRenderer, "../assets/images/buttonReplayP.png");
    btn_replay.hover = IMG_LoadTexture(pauseRenderer, "../assets/images/buttonReplayH.png");
    btn_replay.rect = (SDL_FRect){
        .x = 810,
        .y = 480,
        .w = 70,
        .h = 70
    };

    Button btn_EndGame;
    btn_EndGame.isHover = false;
    btn_EndGame.isPressed = false;
    btn_EndGame.normal = IMG_LoadTexture(pauseRenderer, "../assets/images/buttonGameExit.png");
    btn_EndGame.pressed = IMG_LoadTexture(pauseRenderer, "../assets/images/buttonGameExitP.png");
    btn_EndGame.hover = IMG_LoadTexture(pauseRenderer, "../assets/images/buttonGameExitH.png");
    btn_EndGame.rect = (SDL_FRect){
        .x = 595,
        .y = 480,
        .w = 70,
        .h = 70
    };

    Button btn_tomenu;
    btn_tomenu.isHover = false;
    btn_tomenu.isPressed = false;
    btn_tomenu.normal = IMG_LoadTexture(pauseRenderer, "../assets/images/buttonMenu.png");
    btn_tomenu.pressed = IMG_LoadTexture(pauseRenderer, "../assets/images/buttonMenuP.png");
    btn_tomenu.hover = IMG_LoadTexture(pauseRenderer, "../assets/images/buttonMenuH.png");
    btn_tomenu.rect = (SDL_FRect){
        .x = 395,
        .y = 480,
        .w = 70,
        .h = 70
    };

    SDL_FRect pauseRect = {
        .h = 330,
        .w = 610,
        .x = 335,
        .y = 250
    };
    
    while(running){
        SDL_RenderTexture(pauseRenderer,bgTexture,NULL,NULL);
        SDL_RenderTexture(pauseRenderer, pauseTexture, NULL, &pauseRect);
        renderbutton(pauseRenderer, &btn_EndGame);
        renderbutton(pauseRenderer, &btn_replay);
        renderbutton(pauseRenderer, &btn_tomenu);
        SDL_RenderPresent(pauseRenderer);
        while(SDL_PollEvent(&event)){
            float px,py;
            SDL_GetMouseState(&px,&py);
            // La souris touche-t-elle le bouton pour pauser le jeu?
            btn_replay.isHover=pointDansRect(px,py,btn_replay.rect);
            // La souris touche-t-elle le bouton pour aller au menu?
            btn_tomenu.isHover=pointDansRect(px,py, btn_tomenu.rect);
            btn_EndGame.isHover = pointDansRect(px,py, btn_EndGame.rect);
            switch(event.type){
                case SDL_EVENT_QUIT :{
                    running = false;
                    quitDirectly = false;
                    break;
                }
                
                case SDL_EVENT_KEY_DOWN:{
                    if (event.key.key == SDLK_ESCAPE)
                        running = false;
                        quitDirectly = false;
                    break;
                    }
                case SDL_EVENT_MOUSE_BUTTON_DOWN:{
                    if (btn_tomenu.isHover) btn_tomenu.isPressed = true;
                    if (btn_replay.isHover) btn_replay.isPressed = true;
                    if (btn_EndGame.isHover) btn_EndGame.isPressed = true;
                    break;
                    }
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    if (btn_replay.isHover && btn_replay.isPressed) {
                        btn_replay.isPressed = false;
                        replay = true;
                        running = false;
                        break;
                    }
                    if (btn_tomenu.isHover && btn_tomenu.isPressed) {
                        running = false;
                        toMenu = true;
                        btn_tomenu.isPressed = false;
                        break;
                    }
                    if (btn_EndGame.isHover && btn_EndGame.isPressed){
                        running = false;
                        endGame = true;
                        btn_EndGame.isPressed = false;
                        break;                    
                    }
                }
            }
        }
    freeButton(&btn_replay);
    freeButton(&btn_tomenu);
    freeButton(&btn_EndGame);
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyTexture(pauseTexture);
    SDL_DestroyRenderer(pauseRenderer);

    if (replay){
        launch_game(window, ListePions,VsAI, player1Turn,depht,scorePlayer1,scorePlayer2);
    }
    else if(toMenu){
        int ListeP[12] = {4,4,4,4,4,4,4,4,4,4,4,4};
        AfficheMenu(window, ListeP);
    }
    else if (endGame){
        popUpFinalityOfGame(window,ListePions,scorePlayer1,scorePlayer2,VsAI,depht);
    }
    else if (quitDirectly){
        TTF_Quit();
        SDL_DestroyRenderer(pauseRenderer);
        SDL_DestroyWindow(window);
    }
}



void confirmGoToMenuPopUp(
                SDL_Window *window,
                int ListePions[12],
                bool VsAI,
                bool player1Turn,
                int scorePlayer1,
                int scorePlayer2,
                int depht

){
    
    bool quitDirectly = false;
    bool running = true;
    bool toMenu = false;
    bool replay = false;
    bool endGame = false;
    SDL_Event event;

    SDL_Renderer* confirmRenderer = SDL_CreateRenderer(window, NULL);
    if (!confirmRenderer){
        fprintf(stderr, "Erreur dans la création du renderer 5: %s", SDL_GetError());
        return;
    }
    SDL_Texture *bgTexture = IMG_LoadTexture(confirmRenderer,"../assets/images/plateauAwale.png");
    if (!bgTexture){
        fprintf(stderr, "Erreur du chargement de l'image: %s", SDL_GetError());
        return;
    }
    SDL_Texture* pauseTexture = IMG_LoadTexture(confirmRenderer,"../assets/images/confirmGoToMenu.png");

    Button btn_replay;
    btn_replay.isHover = false;
    btn_replay.isPressed = false;
    btn_replay.normal = IMG_LoadTexture(confirmRenderer, "../assets/images/buttonReplay.png");
    btn_replay.pressed = IMG_LoadTexture(confirmRenderer, "../assets/images/buttonReplayP.png");
    btn_replay.hover = IMG_LoadTexture(confirmRenderer, "../assets/images/buttonReplayH.png");
    btn_replay.rect = (SDL_FRect){
        .x = 680,
        .y = 445,
        .w = 70,
        .h = 70
    };


    Button btn_tomenu;
    btn_tomenu.isHover = false;
    btn_tomenu.isPressed = false;
    btn_tomenu.normal = IMG_LoadTexture(confirmRenderer, "../assets/images/buttonMenu.png");
    btn_tomenu.pressed = IMG_LoadTexture(confirmRenderer, "../assets/images/buttonMenuP.png");
    btn_tomenu.hover = IMG_LoadTexture(confirmRenderer, "../assets/images/buttonMenuH.png");
    btn_tomenu.rect = (SDL_FRect){
        .x = 530,
        .y = 445,
        .w = 70,
        .h = 70
    };



    SDL_FRect confirmMenu = {
        .h = 360,
        .w = 360,
        .x = 385+75,
        .y = 200
    };
    
    while(running){
        SDL_RenderTexture(confirmRenderer,bgTexture,NULL,NULL);
        SDL_RenderTexture(confirmRenderer, pauseTexture, NULL, &confirmMenu);
        renderbutton(confirmRenderer, &btn_replay);
        renderbutton(confirmRenderer, &btn_tomenu);
        SDL_RenderPresent(confirmRenderer);
        while(SDL_PollEvent(&event)){
            float px,py;
            SDL_GetMouseState(&px,&py);
            // La souris touche-t-elle le bouton pour pauser le jeu?
            btn_replay.isHover=pointDansRect(px,py,btn_replay.rect);
            // La souris touche-t-elle le bouton pour aller au menu?
            btn_tomenu.isHover=pointDansRect(px,py, btn_tomenu.rect);
            switch(event.type){
                case SDL_EVENT_QUIT :{
                    running = false;
                    quitDirectly = false;
                    break;
                }
                
                case SDL_EVENT_KEY_DOWN:{
                    if (event.key.key == SDLK_ESCAPE)
                        running = false;
                        quitDirectly = false;
                    break;
                    }
                case SDL_EVENT_MOUSE_BUTTON_DOWN:{
                    if (btn_tomenu.isHover) btn_tomenu.isPressed = true;
                    if (btn_replay.isHover) btn_replay.isPressed = true;
                    break;
                    }
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    if (btn_replay.isHover && btn_replay.isPressed) {
                        btn_replay.isPressed = false;
                        replay = true;
                        running = false;
                        break;
                    }
                    if (btn_tomenu.isHover && btn_tomenu.isPressed) {
                        running = false;
                        toMenu = true;
                        btn_tomenu.isPressed = false;
                        break;
                    }
                }
            }
        }
    freeButton(&btn_replay);
    freeButton(&btn_tomenu);
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyTexture(pauseTexture);
    SDL_DestroyRenderer(confirmRenderer);

    if (replay){
        launch_game(window, ListePions,VsAI, player1Turn,depht,scorePlayer1,scorePlayer2);
    }
    else if(toMenu){
        int ListeP[12] = {4,4,4,4,4,4,4,4,4,4,4,4};
        AfficheMenu(window, ListeP);
    }
    else if (quitDirectly){
        TTF_Quit();
        SDL_DestroyWindow(window);
    }
}
