#include "outils.h"
#include <stdbool.h>
#include <stdio.h>
#include "display.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

typedef struct Button{
    SDL_FRect rect;
    SDL_Texture* normal;
    SDL_Texture* hover;
    SDL_Texture* pressed;
    bool isHover;
    bool isPressed;
} Button;


bool pointDansRect(float x, float y, SDL_FRect r) {
    return (x >= r.x && x <= r.x + r.w &&
            y >= r.y && y <= r.y + r.h);
}

// Dessin du bouton en fonction de son état
void renderbutton(SDL_Renderer *ren,Button *b) {
    SDL_Texture* tex = b->normal;

    if (b->isPressed && b->isHover)
        tex = b->pressed;
    else if (b->isHover)
        tex = b->hover;

    SDL_RenderTexture(ren, tex, NULL, &b->rect);
}

// Copier le plateau
void CopyPlateau(int srcListe[12], int dstListe[12]){
    for (int i=0;i<12;i++){
        dstListe[i] = srcListe[i];
    };
}

int getNumPionsOfPlayer(int PlateauList[12], bool player1Turn){
    int sum =0;
    int deb = (player1Turn) ? 0 : 6;
    for (int i=deb;i<deb+6;i++){
        sum += PlateauList[i];
    }
    sum = (player1Turn) ? sum : -sum;
    return sum;
}


bool isMyOpponentStarving(int PlateauList[12], bool player1Turn){
    return getNumPionsOfPlayer(PlateauList, !player1Turn) == 0;
}


int GetPossibleMoves(int PlateauList[12],  int dstPossibleMoves[6],bool player1Turn){
    int deb = (player1Turn) ? 0 : 6;
    bool Starving = isMyOpponentStarving(PlateauList, player1Turn);
    int nbPos = 0;
    for (int p =0; p<6;p++){
        int ref= (Starving)? 6:p+1; // Il faut nourir l'adversaire si il est affamé
        if (PlateauList[deb + p] >ref-p-1){
            dstPossibleMoves[nbPos] = deb + p;
            nbPos += 1;
        }
    }
    return nbPos;
}


bool ultimateState(int PlateauList[12], bool player1Turn){
    int possibleMoves[6];
    if (player1Turn)
        return isMyOpponentStarving(PlateauList, false) ==0 && GetPossibleMoves(PlateauList, possibleMoves, true)==0 ;
    else
        return isMyOpponentStarving(PlateauList, true) ==0 && GetPossibleMoves(PlateauList, possibleMoves, false)==0 ;
}

void inCrementInPos(int PlateauList[12], int pos){
        if (PlateauList == NULL || pos < 0 || pos >= 12) return;
        PlateauList[pos] += 1;
}


void doTheMoveDisplay(
    SDL_Renderer*plateauRenderer,
    TTF_Font* policePlateau,
    SDL_Texture *bgTexture,
    Button*ListButtons[],
    int nbButtons,
    int POS_TROUS[12][2],
    int POS_RECT[12][2],
    int PlateauList[12], 
    int posInit,
    bool VsAI, 
    bool player1Turn,
    int* scorePlayer1, 
    int* scorePlayer2
    )
{
    if (PlateauList == NULL || posInit <0 || posInit >=12 ||  policePlateau == NULL ||  scorePlayer1 == NULL || scorePlayer2 == NULL) return;
    int NBPions = PlateauList[posInit];
    PlateauList[posInit] =0;
    for (int i=1;i<=NBPions;i++){
        if ((posInit + i)%12 != posInit){// éviter de déposer une pierre dans le trou de départ
            PlateauList[(posInit + i)%12] +=1;
            displayPlateauWithDelay(
                plateauRenderer,
                bgTexture,
                policePlateau,
                ListButtons,
                nbButtons,
                POS_TROUS,
                POS_RECT,
                PlateauList,
                *scorePlayer1,
                *scorePlayer2,
                VsAI,
                800
            );
        }
    }
    int finalPos = (posInit + NBPions) % 12; // position finale
    int minRef = (player1Turn) ? 6: 0;
    int maxRef = (player1Turn) ? 11 : 5;
    int NewPlateau [12];
    int scoreGained =0;
    CopyPlateau(PlateauList, NewPlateau);
    while (finalPos >= minRef && finalPos <= maxRef && (NewPlateau[finalPos] == 2 || NewPlateau[finalPos] ==3)){
        scoreGained += NewPlateau[finalPos];
        NewPlateau[finalPos] =0;
        finalPos -=1;
    }
    bool Starving = isMyOpponentStarving(NewPlateau, player1Turn);
    if (!Starving){// Si l'adversaire n'est pas affamé, on peut capturer, 
        // s'il l'est, on peut faire le move sans capturer ses pions
        if (player1Turn){
            *scorePlayer1 += scoreGained;
        } else {
            *scorePlayer2 += scoreGained;
        }
        finalPos = (posInit + NBPions) % 12;
        // Afficher la prise étape par étape
        while (finalPos >= minRef && finalPos <= maxRef && (PlateauList [finalPos] == 2 || PlateauList[finalPos] ==3)){
            PlateauList[finalPos] =0;
            finalPos -=1;
            //SDL_RenderClear(plateauRenderer);
            //SDL_RenderTexture(plateauRenderer, bgTexture, NULL,NULL);
            displayPlateauWithDelay(
                plateauRenderer,
                bgTexture,
                policePlateau,
                ListButtons,
                nbButtons,
                POS_TROUS,
                POS_RECT,
                PlateauList,
                *scorePlayer1,
                *scorePlayer2,
                VsAI,
                800
            );
        } 
    }
}


void freeButton(Button *btn){
    SDL_DestroyTexture(btn->pressed);
    SDL_DestroyTexture(btn->hover);
    SDL_DestroyTexture(btn->normal);
}