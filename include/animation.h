#include "umbrella.h"
#ifndef ANIMATION_H
#define ANIMATION_H

#endif // ANIMATION_H
void drawThehand(SDL_Renderer *PlateauRenderer,SDL_Texture *handTexture,SDL_Texture *handTextureLeft,SDL_Texture *bgTexture,SDL_Texture *graineTexture,int POS_TROUS[12][2],TTF_Font* policePlateau,
    int PlateauList[12],int pos_debut,int pos_suiv,Button*ListButtons[],int nbButtons,int POS_RECT[12][2],bool VsAI,bool player1Turn,int *scorePlayer1,int *scorePlayer2);


    void drawTheHandToScore(SDL_Renderer *PlateauRenderer,SDL_Texture *handTexture,SDL_Texture * handTextureLeft,SDL_Texture *bgTexture,SDL_Texture *graineTexture,int POS_TROUS[12][2],TTF_Font* policePlateau,
    int PlateauList[12],int POS_RECT[12][2],bool VsAI,bool player1Turn,int *scorePlayer1,int *scorePlayer2,int pos );


    void drawSeedInTheHole(SDL_Renderer *PlateauRenderer,SDL_Texture *graineTexture,int tx,int ty,int n);


    void drawAllThePlaterSeeds(SDL_Renderer *PlateauRenderer,SDL_Texture *graineTexture,int POS_TROUS[12][2],int ListePions[12]);