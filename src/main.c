#include "umbrella.h"



int main(int argc, char* argv[]){
    
    if (!SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) || !TTF_Init()){
        fprintf(stderr, "Erreur: %s", SDL_GetError());
    }
    
    SDL_Window *win=SDL_CreateWindow("Menu",1280,720,0);
    if (!win){
        fprintf(stderr, "Erreur: %s", SDL_GetError());
    }

    int ListePions[12] = {4,4,4,4,4,4,4,4,4,4,4,4};

    AfficheMenu(win, ListePions);
    SDL_DestroyWindow(win);
    SDL_Quit();
    
    return 0;
}
