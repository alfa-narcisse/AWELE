#include "ai.h"
#include "outils.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define INF 100000000

typedef struct Choice{
    int pos;
    int scoreGained;
} Choice;

static int doTheMovePos(int PlateauList[12], int pos, bool AI_Turn){
    if (PlateauList == NULL || pos <0 || pos >=12) return -1;
    int NBPions = PlateauList[pos];
    PlateauList[pos] =0;
    int i = 1;
    while (0<NBPions){
        if ((pos + i)%12 != pos){// éviter de déposer une pierre dans le trou de départ
            PlateauList[(pos + i)%12] +=1;
            NBPions--;
        }
        i++;
    }
    int finalPos = (pos + i-1) % 12; // position finale à corriger car dans le cas où on ne met pas dans la positio de départ, c'est faux.
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

int evaluatePlateau(int PlateauList[12], bool AI_Turn){
    // evaluer le plateau après le tour de AI?
    int heuristic = 0;
    for (int i=0; i<6;i++){
        if (PlateauList[i]==2){
            if (AI_Turn){
                heuristic -= 4;
            }
        }
        else if (PlateauList[i]==1){
            if (AI_Turn){
                heuristic -= 1;
            }
        }
        else if (PlateauList[i]%11 >= 5-i){
            if (!AI_Turn && (PlateauList[i]%11 -(5-i))/6  == 0){
                heuristic += ((PlateauList[i]%11 -(5-i))%6)*5*(PlateauList[i]/11+1);
            }
        }
    }
    for (int i=6; i<12;i++){
        if (PlateauList[i]==2){
            if (!AI_Turn){      
                heuristic += 4;
            }
        }
        else if (PlateauList[i]==1){
            if (!AI_Turn){
                heuristic += 1;
            }
        }
        else if (PlateauList[i]%11 >= 11-i){
            if (AI_Turn && (PlateauList[i]%11 -(11-i))/6  == 0){
                heuristic -= ((PlateauList[i]%11 -(11-i))%6)*5*(PlateauList[i]/11+1);
            }
        }
    }
    return heuristic;
}
static Choice optimalChoice(int PlateauList[12], bool AI_Turn, int depth, int score, int alpha, int beta){
    Choice bestChoice;
    bestChoice.pos = -1;
    bool fin = ultimateState(PlateauList, !AI_Turn); 
    if ( fin || depth ==0){
        if (fin){
            bestChoice.scoreGained = 200*( score + getNumPionsOfPlayer(PlateauList, !AI_Turn));
        }
        else{
            bestChoice.scoreGained = 200* score + evaluatePlateau(PlateauList, !AI_Turn);
        }
        return bestChoice;
    }
    int possibleMoves[6];
    int nbPos = GetPossibleMoves(PlateauList, possibleMoves, AI_Turn);
    bestChoice.scoreGained = (AI_Turn) ? -INF : INF;// valeur de référence pour la comparaison
    if (nbPos ==0){
        // Pas de coup possible, dans le cas où on ne peut plus nourir l'adversaire
        bestChoice.scoreGained = 200*(score +  getNumPionsOfPlayer(PlateauList, AI_Turn));
        return bestChoice;
    }
    for (int i=0;i<nbPos;i++){// pour chaque coup possible
        int pos = possibleMoves[i];
        int PlateauCopy[12];
        CopyPlateau(PlateauList, PlateauCopy);
        int newscore = doTheMovePos(PlateauCopy, pos, AI_Turn) + score;
        Choice childChoice = optimalChoice(PlateauCopy,!AI_Turn, depth -1, newscore, alpha, beta);
        if (AI_Turn){
            if (childChoice.scoreGained >= bestChoice.scoreGained){
                bestChoice.pos = pos;
                bestChoice.scoreGained = childChoice.scoreGained;
                }
            if (bestChoice.scoreGained>alpha){
                    alpha = bestChoice.scoreGained;
                }
            if (beta <= alpha){
                return bestChoice;
            }
        }
        else{
            if (childChoice.scoreGained<= bestChoice.scoreGained){
                bestChoice.pos = pos;
                bestChoice.scoreGained = childChoice.scoreGained;
            }
            if (beta > bestChoice.scoreGained){
                beta = bestChoice.scoreGained;
            }
            if (beta<= alpha){
                return bestChoice;
            }
        }
    }
    return bestChoice;
};

int bestChoiceAI(int PlateauList[12], int depth){
    return optimalChoice(PlateauList,true, depth, 0,-INF, INF).pos;
}





/*============================= Choix avec l'algorithme de MonteCarlo =======================================*/
#define MAX_CHILDREN 6

typedef struct MCTSNode {
    int plateau[12];
    bool AI_Turn;              
    int move;                
    int visits;
    double wins;
    int scoreParentAI;
    int scoreParentAdv;

    struct MCTSNode *parent;
    struct MCTSNode *children[MAX_CHILDREN];
    int childCount;
} MCTSNode;




double UCT(MCTSNode *parent, MCTSNode *child) {
    if (child->visits == 0)
        return INFINITY;

    double exploitation = child->wins / child->visits;
    double exploration = sqrt(log(parent->visits) / child->visits);

    return exploitation + 0.7 * exploration;
}


MCTSNode* selectNode(MCTSNode *node) {
    while (node->childCount > 0) {
        double bestUCT = -1e9;
        MCTSNode *bestChild = NULL;

        for (int i = 0; i < node->childCount; i++) {
            double uct = UCT(node, node->children[i]);
            if (uct > bestUCT) {
                bestUCT = uct;
                bestChild = node->children[i];
            }
        }
        node = bestChild;
    }
    return node;
}

MCTSNode* expandNode(MCTSNode *node) {
    int coups[6];
    int nbCoups = GetPossibleMoves(node->plateau, coups,node->AI_Turn);

    if (nbCoups == 0) return node;

    int move = coups[SDL_rand(nbCoups)];

    MCTSNode *child = malloc(sizeof(MCTSNode));
    memcpy(child->plateau, node->plateau, sizeof(int) * 12);
    child->scoreParentAI = node->scoreParentAI;
    child->scoreParentAdv = node->scoreParentAdv;

    int score = doTheMovePos(node->plateau,move,node->AI_Turn);
    child->scoreParentAI += (node->AI_Turn)? score:0;
    child->scoreParentAdv -= (node->AI_Turn)? 0:score;

    child->AI_Turn = !node->AI_Turn;
    child->move = move;
    child->visits = 0;
    child->wins = 0;
    child->parent = node;
    child->childCount = 0;

    node->children[node->childCount++] = child;
    return child;
}

bool gameOver(int plateau[12], bool AI_Turn, int scoreAI, int scoreP2){
    return scoreAI>= 25 || scoreP2>= 25 ||ultimateState(plateau,!AI_Turn);
}


bool simulateGame(int plateau[12], bool AI_Turn, int scoreAI, int scoreP2) {
    bool currentPlayer = AI_Turn;
     int temp[12];
    memcpy(temp, plateau, sizeof(int) * 12);
    if(!gameOver(plateau,currentPlayer, scoreAI,scoreP2)) {
        int coups[6];
        int n = GetPossibleMoves(plateau,coups,currentPlayer);
        int pos = coups[SDL_rand( n)];
        int score = doTheMovePos(temp, pos, currentPlayer);
        score = (currentPlayer)?score:-score;
        scoreAI += (currentPlayer)? score:0;
        scoreP2 += (!currentPlayer)?score: 0;
        currentPlayer = !currentPlayer;
    }
    scoreAI += getNumPionsOfPlayer(temp,true);
    scoreP2 -= getNumPionsOfPlayer(temp,false);
    return scoreAI > scoreP2;
}

void backpropagate(MCTSNode *node, bool result) {
    while (node != NULL) {
        node->visits++;
        if (result) node->wins += 1;
        else node->wins += 0;
        node = node->parent;
    }
}


void freeTree(MCTSNode *node) {
    if (!node) return;
    for (int i = 0; i < node->childCount; i++) {
        freeTree(node->children[i]);
    }
    free(node);
}

int iterations = 2000;
int MCTS(int plateau[12], int scoreAI, int scoreP2) {
    MCTSNode *root = malloc(sizeof(MCTSNode));
    memcpy(root->plateau, plateau, sizeof(int) * 12);

    root->AI_Turn = true;
    root->visits = 0;
    root->wins = 0;
    root->parent = NULL;
    root->childCount = 0;
    root->scoreParentAdv = scoreP2;
    root->scoreParentAI = scoreAI;
    int coups[6];
    int nbps = GetPossibleMoves(plateau, coups,true);
    for(int j=0; j<nbps; j++){
        int move = coups[j];
        MCTSNode *child = malloc(sizeof(MCTSNode));
        memcpy(child->plateau, root->plateau, sizeof(int) * 12);
        child->scoreParentAI = root->scoreParentAI;
        child->scoreParentAdv = root->scoreParentAdv;

        int score = doTheMovePos(root->plateau,move,root->AI_Turn);
        child->scoreParentAI += (root->AI_Turn)? score:0;
        child->scoreParentAdv -= (root->AI_Turn)? 0:score;

        child->AI_Turn = !root->AI_Turn;
        child->move = move;
        child->visits = 0;
        child->wins = 0;
        child->parent = root;
        child->childCount = 0;

        root->children[root->childCount++] = child;
        for (int i = 0; i < iterations; i++) {
            MCTSNode *node = selectNode(root);
            node = expandNode(node);
            int result = simulateGame(node->plateau, node->AI_Turn, node->scoreParentAI, node->scoreParentAdv);
            backpropagate(node, result);
        }
    }
    // meilleur coup = le plus visité
    int bestMove = -1;
    int maxWin = -1;

    for (int i = 0; i < root->childCount; i++) {
        if (root->children[i]->wins > maxWin) {
            maxWin = root->children[i]->wins;
            bestMove = root->children[i]->move;
        }
    }
    freeTree(root);
    return bestMove;
}


/*================================== BRUTE FORCE ================================*/

typedef struct choix{
    int profondeurVictory;
    int profondeurLoss;
    bool victoire;
    int moveP;
    int moveF;
}choix;

choix simulateTurn(int plateau[12], int scoreAI, int scoreP2, bool AI_Turn, int profondeur){
    choix bestchoice;
    bestchoice.profondeurVictory = profondeur;
    bestchoice.moveF = -1;
    if (gameOver(plateau, AI_Turn, scoreAI,scoreP2) && profondeur == 10){
        scoreAI+= getNumPionsOfPlayer(plateau, true);
        scoreP2 -= getNumPionsOfPlayer(plateau, false);
        bestchoice.victoire = scoreAI>scoreP2;
        return bestchoice;
    }
    if (AI_Turn){
        bestchoice.profondeurVictory = INFINITY;
        bestchoice.profondeurLoss = -INFINITY;
        bestchoice.victoire = false;
        int coups[6];
        int nbCps = GetPossibleMoves(plateau, coups, AI_Turn);
        for (int i=0; i<nbCps;i++){
            int pos = coups[i];
            int tmp[12];
            memcpy(tmp, plateau,sizeof(int)*12);
            int newScoreAI = scoreAI + doTheMovePos(tmp,pos, true);
            choix child = simulateTurn(tmp, newScoreAI,scoreP2,!AI_Turn, profondeur+1);
            if (child.victoire){
                if ( !bestchoice.victoire || child.profondeurVictory < bestchoice.profondeurVictory){
                    bestchoice.profondeurVictory = child.profondeurVictory;
                    bestchoice.victoire = true;
                    bestchoice.moveF = pos;
                }
            }
            else{
                if (!bestchoice.victoire && child.profondeurLoss > bestchoice.profondeurLoss){
                    bestchoice.profondeurLoss = child.profondeurLoss;
                    bestchoice.victoire = false;
                    bestchoice.moveF = pos;
                }
            }
        }
    }
    else {
        bestchoice.profondeurVictory = - INFINITY;
        bestchoice.profondeurLoss = INFINITY;
        bestchoice.victoire = true;
        int coups[6];
        int nbCps = GetPossibleMoves(plateau, coups,false);
        for (int i=0; i<nbCps;i++){
            int pos = coups[i];
            int tmp[12];
            memcpy(tmp, plateau,sizeof(int)*12);
            int newScoreP2 = scoreP2  -  doTheMovePos(tmp,pos, false);
            choix child = simulateTurn(tmp, scoreAI,newScoreP2,true, profondeur+1);
            if (!child.victoire){
                if ( !bestchoice.victoire || child.profondeurLoss < bestchoice.profondeurLoss){
                    bestchoice.profondeurLoss = child.profondeurLoss;
                    bestchoice.victoire = false;
                    bestchoice.moveF = pos;
                }
            }
            else{
                if (bestchoice.victoire && child.profondeurVictory > bestchoice.profondeurVictory){
                    bestchoice.profondeurVictory = child.profondeurVictory;
                    bestchoice.victoire = true;
                    bestchoice.moveF = pos;
                }
            }
        }
    }
    return bestchoice;
}

int bruteForceMethode(int plateau[12], int scoreAI, int scoreP2){
    return simulateTurn(plateau, scoreAI,scoreP2,true, 0).moveF;
}

/*====================================================== FIN =============================================================*/
/*
*Seul l'algorithme MCTS et l'algo bestChoiceAI sont fonctionnel, la version brute est impossible à gérer car
*il nécessite beaucoup de mémoire et de temps pour arriver aux feuilles de toutes les branches
*Mais compte tenu du fait que le jeu Awalé est à informations complète, le choix de MiniMax (bestChoiceAI) avec élegage est 
*pertinent
*
*/
/*========================================================-|-==============================================================*/