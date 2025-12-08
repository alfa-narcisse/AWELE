#include "outils.h"
#include <stdbool.h>
#include <stdio.h>
#include "display.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
typedef struct Button{
    SDL_FRect* rect;
    char text[10];
    bool pressed;
    bool hovered;
}Button;



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
    int POS_TROUS[12][2],
    int PlateauList[12],
    int pos,
    SDL_Renderer * renderer,
    TTF_Font* policePlateau,
    TTF_Font* policeButton,
    SDL_Texture* plateauTexture,
    bool player1Turn,
    int*scorePlayer1,
    int* scorePlayer2,
    bool VsAI,
    Button *btn
    )
{
    if (PlateauList == NULL || pos <0 || pos >=12 || renderer == NULL || policePlateau == NULL || plateauTexture == NULL || scorePlayer1 == NULL || scorePlayer2 == NULL) return;
    int NBPions = PlateauList[pos];
    PlateauList[pos] =0;
    for (int i=1;i<=NBPions;i++){
        if ((pos + i)%12 != pos){// éviter de déposer une pierre dans le trou de départ
            PlateauList[(pos + i)%12] +=1;
            displayAllWithDelay(
                renderer,
                plateauTexture,
                policePlateau,
                policeButton,
                PlateauList,
                POS_TROUS,
                *scorePlayer1,
                *scorePlayer2,
                VsAI,
                800,
                btn
            );
        }
    }
    int finalPos = (pos + NBPions) % 12; // position finale
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
        finalPos = (pos + NBPions) % 12;
        // Afficher la prise étape par étape
        while (finalPos >= minRef && finalPos <= maxRef && (PlateauList [finalPos] == 2 || PlateauList[finalPos] ==3)){
            PlateauList[finalPos] =0;
            finalPos -=1;
            displayAllWithDelay(
                renderer,
                plateauTexture,
                policePlateau,
                policeButton,
                PlateauList,
                POS_TROUS,
                *scorePlayer1,
                *scorePlayer2,
                VsAI,
                800,
                btn
            );
        }
    }
}