#include "menu.h"
#include "outils.h"
#include "run_game.h"
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#define b_widht 70
#define b_height 70
#define Menu_widht 1280
#define Menu_height 720
#define mode_width 110
#define mode_height 46






void AfficheHelp(SDL_Window *win, int ListePions[12]);


// type des boutons



// fonction pour verifier si on est dans un rectangle ou pas

// Creation de la fontion affiche help

void popUpRetakeGame(
                    SDL_Window *window,
                    int ListePions[12],
                    bool *twoPlayersMode,
                    bool *VsAI,
                    bool player1Turn
                    );// dans le cas où on trouve une solution pour enregistrer l'état du jeu et pour pouvoir reprendre à la prochaine fois.
                    // Cela éxige la création d'un boutton Reprendre

void AfficheMenu( SDL_Window *win, int ListePions[12]) {
    SDL_Event MenuEvent = {};
    SDL_Renderer *MenuRenderer =SDL_CreateRenderer(win,NULL);
    // Creation de la texture de l'arriere plan
    SDL_Texture *background=IMG_LoadTexture(MenuRenderer,"../assets/images/Background.png");
    if (!background) {
        printf("Error creating background texture %s\n",SDL_GetError());
    }


    bool isopen=true;
    bool wanthelp=false;
    bool VsAiMode = false;
    bool twoPlayersMode = false;

    // Rectangle pour le menu principal
    SDL_FRect mainRect ={0,0,1280,720};
    SDL_FRect srcrect ={0,0,1280,720  };


    //Creation du bouton help
    Button btn_Help;
    btn_Help.rect=(SDL_FRect){Menu_widht-100,100,b_widht,b_height};
    btn_Help.normal  = IMG_LoadTexture(MenuRenderer,"../assets/images/buttonHelp.png");
    btn_Help.hover   = IMG_LoadTexture(MenuRenderer, "../assets/images/buttonHelpH.png");
    btn_Help.pressed = IMG_LoadTexture(MenuRenderer, "../assets/images/buttonHelpP.png");
    btn_Help.isHover = false;
    btn_Help.isPressed = false;


    //Fin
    // Creation du bouton Exit
    Button btn_Exit;
    btn_Exit.rect=(SDL_FRect){Menu_widht-100,270,b_widht,b_height};
    btn_Exit.normal  = IMG_LoadTexture(MenuRenderer,"../assets/images/buttonExit.png");
    btn_Exit.hover   = IMG_LoadTexture(MenuRenderer, "../assets/images/buttonExitH.png");
    btn_Exit.pressed = IMG_LoadTexture(MenuRenderer, "../assets/images/buttonExitP.png");
    btn_Exit.isHover = false;
    btn_Exit.isPressed = false;
    // Fin
    // Creation bouton modeAI
    Button btn_ModeAI;
    btn_ModeAI.rect=(SDL_FRect){(Menu_widht/2)-110/2,(Menu_height/2)-mode_height/2,mode_width,mode_height};
    btn_ModeAI.normal  = IMG_LoadTexture(MenuRenderer,"../assets/images/buttonModeAI.png");
    btn_ModeAI.hover   = IMG_LoadTexture(MenuRenderer, "../assets/images/buttonModeAIH.png");
    btn_ModeAI.pressed = IMG_LoadTexture(MenuRenderer, "../assets/images/buttonModeAI.png");
    btn_ModeAI.isHover = false;
    btn_ModeAI.isPressed = false;
    // Fin
    // Creation bouton mode vsj
    Button btn_ModeVSJ;
    btn_ModeVSJ.rect=(SDL_FRect){(Menu_widht/2)-110/2,(Menu_height/2)+50+mode_height,mode_width,mode_height};
    btn_ModeVSJ.normal  = IMG_LoadTexture(MenuRenderer,"../assets/images/buttonModeVSJ.png");
    btn_ModeVSJ.hover   = IMG_LoadTexture(MenuRenderer, "../assets/images/buttonModeVSJH.png");
    btn_ModeVSJ.pressed = IMG_LoadTexture(MenuRenderer, "../assets/images/buttonModeVSJ.png");
    btn_ModeVSJ.isHover = false;
    btn_ModeVSJ.isPressed = false;
    // Fin


    while (isopen)
    {
        while (SDL_PollEvent(&MenuEvent))// ← IMPORTANT !
        {
            float px,py;
            SDL_GetMouseState(&px,&py);
            btn_Help.isHover=pointDansRect(px,py,btn_Help.rect);
            btn_Exit.isHover=pointDansRect(px,py,btn_Exit.rect);
            btn_ModeAI.isHover=pointDansRect(px,py,btn_ModeAI.rect);
            btn_ModeVSJ.isHover=pointDansRect(px,py,btn_ModeVSJ.rect);
            switch (MenuEvent.type)
            {
                case SDL_EVENT_QUIT:
                    isopen = false;
                    break;

                case SDL_EVENT_KEY_DOWN:
                    if (MenuEvent.key.key == SDLK_ESCAPE)
                        isopen = false;
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN :
                    if (btn_Help.isHover) btn_Help.isPressed = true;

                    if (btn_Exit.isHover) btn_Exit.isPressed = true;

                    if (btn_ModeAI.isHover) btn_ModeAI.isPressed = true;

                    if (btn_ModeVSJ.isHover) btn_ModeVSJ.isPressed = true;

                    break;
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    if (btn_Help.isHover && btn_Help.isPressed) {
                        btn_Help.isPressed = false;
                        wanthelp = true;
                        isopen = false;
                        break;


                    }

                    if (btn_Exit.isHover && btn_Exit.isPressed) {

                        isopen = false;
                        btn_Exit.isPressed = false;
                        break;
                    }

                    if (btn_ModeAI.isHover && btn_ModeAI.isPressed) {
                        btn_ModeAI.isPressed = false;
                        isopen = false;
                        VsAiMode = true;
                        twoPlayersMode = false;
                     };

                    if (btn_ModeVSJ.isHover && btn_ModeVSJ.isPressed) {
                        btn_ModeVSJ.isPressed = false;
                        isopen = false;
                        VsAiMode = false;
                        twoPlayersMode = true;
                    }
                    break;
            }

        }

        SDL_SetRenderDrawColor(MenuRenderer,0,0,0,255);
        SDL_RenderClear(MenuRenderer);
        SDL_RenderTexture(MenuRenderer,background,&srcrect,&mainRect);
        renderbutton(MenuRenderer,&btn_Help);
        renderbutton(MenuRenderer,&btn_Exit);
        renderbutton(MenuRenderer,&btn_ModeAI);
        renderbutton(MenuRenderer,&btn_ModeVSJ);

        SDL_RenderPresent(MenuRenderer);
    }

    //nettoyage
    SDL_DestroyTexture(btn_Help.pressed);
    SDL_DestroyTexture(btn_Help.hover);
    SDL_DestroyTexture(btn_Help.normal);
    SDL_DestroyTexture(btn_Exit.pressed);
    SDL_DestroyTexture(btn_Exit.hover);
    SDL_DestroyTexture(btn_Exit.normal);
    SDL_DestroyTexture(btn_ModeAI.pressed);
    SDL_DestroyTexture(btn_ModeAI.hover);
    SDL_DestroyTexture(btn_ModeAI.normal);
    SDL_DestroyTexture(btn_ModeVSJ.pressed);
    SDL_DestroyTexture(btn_ModeVSJ.hover);
    SDL_DestroyTexture(btn_ModeVSJ.normal);
    SDL_DestroyTexture(background);
    SDL_DestroyRenderer(MenuRenderer);
    if (wanthelp) AfficheHelp(win, ListePions);
    // à ajouter l'action à faire lorsque l'utilisateur choisit l'une des options ai ou 2 players.
    else if( VsAiMode || twoPlayersMode){
        launch_game(win,ListePions, VsAiMode,true, true,15,0,0);//Pour le moment, on met profondeur à 6
    }
}
void AfficheHelp(SDL_Window* win, int ListePions[12]) {
    SDL_Event Helpevent;
    SDL_Renderer *helprenderer=SDL_CreateRenderer(win,NULL);
    SDL_Texture *backgroundHelp = IMG_LoadTexture(helprenderer,"../assets/images/backgroundhelp.png");

    // Rectangle background help
    SDL_FRect mainRect ={0,0,1280,720};
    SDL_FRect srcrect ={0,0,1280,720  };

    // Creation du bouton qui permet de sortir de l'affichage help
    Button btn_Exithelp;
    btn_Exithelp.rect=(SDL_FRect){Menu_widht-100,270,b_widht,b_height};
    btn_Exithelp.normal  = IMG_LoadTexture(helprenderer,"../assets/images/buttonMenu.png");
    btn_Exithelp.hover   = IMG_LoadTexture(helprenderer, "../assets/images/buttonMenuH.png");
    btn_Exithelp.pressed = IMG_LoadTexture(helprenderer, "../assets/images/buttonMenuP.png");
    btn_Exithelp.isHover = false;
    btn_Exithelp.isPressed = false;
    // Fin de creation

    bool ishelpOpen=true;
    bool quitdirect=false;
    while (ishelpOpen) {
        while (SDL_PollEvent(&Helpevent)) {
            float px,py;
            SDL_GetMouseState(&px,&py);
            btn_Exithelp.isHover=pointDansRect(px,py,btn_Exithelp.rect);
            switch (Helpevent.type) {
                case SDL_EVENT_QUIT:
                    ishelpOpen=false;
                    quitdirect=true;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    if (Helpevent.key.key==SDLK_ESCAPE) {
                        ishelpOpen=false;
                        quitdirect=true;
                    }

                    break;

                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    if (btn_Exithelp.isHover) btn_Exithelp.isPressed=true;
                    break;
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    if (btn_Exithelp.isHover && btn_Exithelp.isPressed) {
                        btn_Exithelp.isPressed=false;
                        quitdirect=false;
                        ishelpOpen=false;

                    }
                    break;
            }


        }
        SDL_SetRenderDrawColor(helprenderer,0,0,0,255);
        SDL_RenderClear(helprenderer);
        SDL_RenderTexture(helprenderer,backgroundHelp,&srcrect,&mainRect);
        renderbutton(helprenderer,&btn_Exithelp);
        SDL_RenderPresent(helprenderer);
    }
    SDL_DestroyTexture(btn_Exithelp.pressed);
    SDL_DestroyTexture(btn_Exithelp.hover);
    SDL_DestroyTexture(btn_Exithelp.normal);
    SDL_DestroyTexture(backgroundHelp);
    SDL_DestroyRenderer(helprenderer);
    if (quitdirect) {
        SDL_DestroyWindow(win);
        SDL_Quit();
    }
    else {
        AfficheMenu(win, ListePions);
    }


}

