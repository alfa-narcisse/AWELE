#ifndef AI_H
#define AI_H

// ======== NOTION IMPORTANTE ========
// Il est important de noter que dans toutes les fonctions ci-dessous,le plateau est représenté par un tableau d'entiers de taille 12,
// où les indices 0 à 5 représentent les trous du joueur 1 (potentiellement IA lorsqu'on utilise ces fonctions dans ce contexte)
// et les indices 6 à 11 représentent les trous du joueur 2 (humain).


//Dans ce jeu, l'IA est toujours le joueur 1 (indices 0 à 5 dans le tableau du plateau)
// et le joueur humain est toujours le joueur 2 (indices 6 à 11 dans le tableau du plateau).
//====================================



// Trouver le meilleur coup pour l'IA en utilisant l'algorithme MiniMax
// Retourne la position du meilleur coup s'il y en a un, sinon -1
int bestChoiceAI(int PlateauList[12], int depth);

int MCTS(int plateau[12], int scoreAI, int scoreP2);

int bruteForceMethode(int plateau[12], int scoreAI, int scoreP2);

#endif // AI_H