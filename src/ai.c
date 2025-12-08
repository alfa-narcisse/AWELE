#include "ai.h"
#include "outils.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

typedef struct Choice{
    int pos;
    int scorePlateau;
    int scoreGained;
} Choice;

static int doTheMovePos(int PlateauList[12], int pos, bool AI_Turn){
    if (PlateauList == NULL || pos <0 || pos >=12) return -1;
    int NBPions = PlateauList[pos];
    PlateauList[pos] =0;
    for (int i=1;i<=NBPions;i++){
        if ((pos + i)%12 != pos){// éviter de déposer une pierre dans le trou de départ
            PlateauList[(pos + i)%12] +=1;
        }
    }
    int finalPos = (pos + NBPions) % 12; // position finale
    int minRef = (AI_Turn) ? 6: 0;
    int maxRef = (AI_Turn) ? 11 : 5;
    int scoreGained =0;
    int NewPlateau [12];
    CopyPlateau(PlateauList, NewPlateau);
    // Vérification des captures
    while (finalPos >= minRef && finalPos <= maxRef && (NewPlateau[finalPos] == 2 || NewPlateau[finalPos] ==3)){
        scoreGained += NewPlateau[finalPos];
        NewPlateau[finalPos] =0;
        finalPos -=1;
    }

    // Si l'adversaire est affamé, le coup est toujours valide mais aucune prise n'est effectuée
    bool Starving = isMyOpponentStarving(NewPlateau, AI_Turn);
    if (!Starving){
        scoreGained= (AI_Turn) ? scoreGained : -scoreGained;
        CopyPlateau(NewPlateau, PlateauList);
        return scoreGained; 
    }
    return 0;
}

static Choice optimalChoice(int PlateauList[12], bool AI_Turn, int depth){
    Choice bestChoice;
    bestChoice.pos = -1;
    int val = getNumPionsOfPlayer(PlateauList, AI_Turn);
    if (val==0 || depth ==0){
        bestChoice.scorePlateau = val;
        bestChoice.scoreGained = 0;
        return bestChoice;
    }
    int possibleMoves[6];
    int nbPos = GetPossibleMoves(PlateauList, possibleMoves, AI_Turn);
    int valRef = (AI_Turn) ? -INFINITY : INFINITY;// valeur de référence pour la comparaison
    if (nbPos ==0){
        // Pas de coup possible, on est à la fin du jeu- On met beaucoup de poids car c'est une situaation
        //favorable pour soit meme (si AI_Turn) ou défavorable pour l'adversaire
        bestChoice.scorePlateau = getNumPionsOfPlayer(PlateauList, AI_Turn);
        bestChoice.scoreGained = 0;
        return bestChoice;
    }
    for (int i=0;i<nbPos;i++){// pour chaque coup possible
        int pos = possibleMoves[i];
        int PlateauCopy[12];
        CopyPlateau(PlateauList, PlateauCopy);
        int scoreGained = doTheMovePos(PlateauCopy, pos, AI_Turn);
        Choice childChoice = optimalChoice(PlateauCopy, !AI_Turn, depth -1);
        int totalScore = 1000*scoreGained + 1000*childChoice.scoreGained + childChoice.scorePlateau + val;// score total combinant la prise immédiate, le score futur et l'état du plateau
        if ((AI_Turn && totalScore >= valRef) || (!AI_Turn && totalScore <= valRef)){
                valRef = totalScore;
                bestChoice.pos = pos;
                bestChoice.scorePlateau = val;
                bestChoice.scoreGained = scoreGained;
            }
        }
    return bestChoice;
};

int bestChoiceAI(int PlateauList[12], int depth){
    return optimalChoice(PlateauList, true, depth).pos;
}

