#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

int POS_TROUS[12][2]={
    {1300, 175},
    {1075, 175},
    {850, 175},
    {625, 175},
    {400, 175},
    {175,175},
    {175,500},
    {400, 500},
    {625, 500},
    {850, 500},
    {1075, 500},
    {1300, 500}
};

int LIST_PIONS[12] = {4,4,4,4,4,4,4,4,4,4,4,4};

void drawCercle(int x, int y, int R, SDL_Surface* surface) {
    if (!surface) return;
    
    SDL_LockSurface(surface);
    
    int pixelsPerLine = surface->pitch / sizeof(Uint32);
    Uint32* pixels = (Uint32*)surface->pixels;
    
    // Calculer les limites
    int minX = SDL_max(x - R, 0);
    int maxX = SDL_min(x + R, surface->w - 1);
    int minY = SDL_max(y - R, 0);
    int maxY = SDL_min(y + R, surface->h - 1);
    
    // Créer la couleur
    Uint32 couleurBlanche = SDL_MapRGBA(SDL_GetPixelFormatDetails(surface->format), NULL, 255, 255, 255, 255);
    
    // Dessiner
    for (int i = minX; i <= maxX; i++) {
        for (int j = minY; j <= maxY; j++) {
            int dx = i - x;
            int dy = j - y;
            if (dx * dx + dy * dy <= R * R) {
                pixels[j * pixelsPerLine + i] = couleurBlanche;
            }
        }
    }
    
    SDL_UnlockSurface(surface);
}

// Elaboration des règles du jeu
typedef struct Choice{
    int pos;
    int score;
}Choice;

void CopyPlateau(int srcListe[12], int dstListe[12]){
    for (int i=0;i<12;i++){
        dstListe[i] = srcListe[i];
    };
}

int getNumPionsOfPlayer(int PlateauList[12], bool AI_Turn){
    int sum =0;
    int deb = (AI_Turn) ? 0 : 6;
    for (int i=deb;i<deb+6;i++){
        sum += PlateauList[i];
    }
    sum = (AI_Turn) ? sum : -sum;
    return sum;
}

bool isMyOpponentStarving(int PlateauList[12], bool AI_Turn){
    int deb = (AI_Turn) ? 6 : 0;
    return getNumPionsOfPlayer(PlateauList, !AI_Turn) == 0;
}


int GetPossibleMoves(int PlateauList[12],  int possibleMoves[6],bool AI_Turn){
    int deb = (AI_Turn) ? 0 : 6;
    bool Starving = isMyOpponentStarving(PlateauList, AI_Turn);
    int nbPos = 0;
    for (int p =0; p<6;p++){
        int ref= (Starving)? 6:p+1; // Il faut nourir l'adversaire si il est affamé
        if (PlateauList[deb + p] >ref-p-1){
            possibleMoves[nbPos] = deb + p;
            nbPos += 1;
        }
    }
    return nbPos;
}

int doTheMovePos(int PlateauList[12], int pos, bool AI_Turn){
    if (PlateauList == NULL || pos <0 || pos >=12) return -1;
    int NBPions = PlateauList[pos];
    PlateauList[pos] =0;
    for (int i=1;i<=NBPions;i++){
        if ((pos + i)%12 != pos){// éviter de déposer une pierre dans le trou de départ
            PlateauList[(pos + i)%12] +=1;
        }
    }
    int finalPos = (pos + NBPions) % 12; // position finale
    int minRef = (AI_Turn) ? 6 : 0;
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
    bool Starving = isMyOpponentStarving(NewPlateau, AI_Turn);
    if (!Starving){
        scoreGained= (AI_Turn) ? scoreGained : -scoreGained;
        CopyPlateau(NewPlateau, PlateauList);
        return scoreGained; 
    }
    return 0;
}


Choice MiniMax(int PlateauList[12], bool AI_Turn, int depth){
    Choice bestChoice;
    bestChoice.pos = -1;
    int val = getNumPionsOfPlayer(PlateauList, AI_Turn);
    if (val==0 || depth ==0){
        bestChoice.score = val;
        return bestChoice;
    }
    int possibleMoves[6];
    int nbPos = GetPossibleMoves(PlateauList, possibleMoves, AI_Turn);
    int valRef = (AI_Turn) ? -INFINITY : INFINITY;
    if (nbPos ==0){
        // Pas de coup possible, on est à la fin du jeu- on retourne 
        bestChoice.score += 100*getNumPionsOfPlayer(PlateauList, AI_Turn);
        return bestChoice;
    }
    for (int i=0;i<nbPos;i++){
        int pos = possibleMoves[i];
        int PlateauCopy[12];
        CopyPlateau(PlateauList, PlateauCopy);
        int scoreGained = doTheMovePos(PlateauCopy, pos, AI_Turn);
        Choice childChoice = MiniMax(PlateauCopy, !AI_Turn, depth -1);
        val = getNumPionsOfPlayer(PlateauCopy, AI_Turn);
        int totalScore = 1000*scoreGained + 1000*childChoice.score + val;
        if ((AI_Turn && totalScore >= valRef) || (!AI_Turn && totalScore <= valRef)){
                valRef = totalScore;
                bestChoice.pos = pos;
                bestChoice.score = totalScore;
            }
        }
    return bestChoice;
}

bool Finality(int PlateauList[12], bool AI_Turn){
    int possibleMoves[6];
    if (AI_Turn)
        return getNumPionsOfPlayer(PlateauList, false) ==0 && GetPossibleMoves(PlateauList, possibleMoves, true)==0 ;
    else
        return getNumPionsOfPlayer(PlateauList, true) ==0 && GetPossibleMoves(PlateauList, possibleMoves, false)==0 ;
}
    
// la géstion des évenements, càd, un clique sur un trou de tels coordonnées pointe vers quel trou en mémoire? En lien avec POS_TROUS;




void inCrementInPos(int PlateauList[12], int pos){
        if (PlateauList == NULL || pos < 0 || pos >= 12) return;
        PlateauList[pos] += 1;
    }
void DisplayPlateau(int PlateauList[12], SDL_Renderer* renderer, TTF_Font* police,SDL_Texture* pTextureText, SDL_FRect *dstTextRect){
    // Render the title/banner once (if provided)
    if (pTextureText && dstTextRect) {
        SDL_RenderTexture(renderer, pTextureText, NULL, dstTextRect);
    }

    for (int i = 0; i < 12; i++) {
        // Préparer le texte à afficher
        char buffer[4];
        snprintf(buffer, sizeof(buffer), "%d", PlateauList[i]);
        
        // Créer la surface du texte (signature correcte)
        SDL_Color couleurNoire = {0, 0, 0, 255};
        SDL_Surface* surfaceTexte = TTF_RenderText_Blended(police, buffer, sizeof(buffer),couleurNoire);
        if (!surfaceTexte) {
            fprintf(stderr, "Erreur de création du texte: %s\n", SDL_GetError());
            return;
        }
        
        // Créer la texture à partir de la surface
        SDL_Texture* textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
        SDL_DestroySurface(surfaceTexte);
        if (!textureTexte) {
            fprintf(stderr, "Erreur de création de la texture: %s\n", SDL_GetError());
            return;
        }
        
        // Définir la position pour dessiner le texte
        SDL_FRect dstRect;
        dstRect.w = 60; // Largeur fixe pour le texte
        dstRect.h = 60; // Hauteur fixe pour le texte
        dstRect.x = POS_TROUS[i][0] - dstRect.w / 2; // Centrer le texte
        dstRect.y = POS_TROUS[i][1] - dstRect.h / 2; // Centrer le texte
        
        // Dessiner la texture du nombre
        SDL_RenderTexture(renderer, textureTexte, NULL, &dstRect);
        
        // Libérer la texture
        SDL_DestroyTexture(textureTexte);
    }
}


int distance(int x1, int y1, int x2, int y2){
    return (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1);
}

int PosClicked(int x, int y){
    for (int i =0;i<12;i++){
        int dist = distance(x, y, POS_TROUS[i][0], POS_TROUS[i][1]);
        if (dist < 100*100){
            return i;
        }
    }
    return -1;
}

void displayScore(SDL_Renderer * renderer, TTF_Font* police,int ScorePlayer, int ScoreIA){
    char scoreText[80];
    snprintf(scoreText, sizeof(scoreText), "Player: %d  |  AI: %d", ScorePlayer, ScoreIA);
    SDL_Color Noire ={
        .a =255,
        .b = 45,
        .g = 100,
        .r = 100
    };
    SDL_Surface *SurfaceScore = TTF_RenderText_Blended(police, scoreText,sizeof(scoreText), Noire);
    if(!SurfaceScore){
        fprintf(stderr, "Erreur de création du texte: %s\n", SDL_GetError());
        return;
    }
    SDL_Texture*pTextureScore = SDL_CreateTextureFromSurface(renderer,SurfaceScore);
    if (!pTextureScore){
        fprintf(stderr, "Erreur de création de la texture du texte: %s\n", SDL_GetError());
        SDL_DestroySurface(SurfaceScore);
        return;
    }

    int rw = 0, rh = 0;
    SDL_GetCurrentRenderOutputSize(renderer, &rw, &rh); // dynamic renderer/window size

    SDL_FRect dstScoreRect;
    dstScoreRect.x = 10.0f;                                 // left margin
    dstScoreRect.y = (float)(rh - SurfaceScore->h - 10);    // bottom-left with 10px margin
    dstScoreRect.w = (float)SurfaceScore->w;
    dstScoreRect.h = (float)SurfaceScore->h;

    SDL_DestroySurface(SurfaceScore);
    SDL_RenderTexture(renderer, pTextureScore, NULL, &dstScoreRect);
    SDL_DestroyTexture(pTextureScore);    
}

void displayVictory(SDL_Renderer * renderer, TTF_Font* police,int ScorePlayer, int ScoreIA){
    const char* AiWIN = (ScoreIA>ScorePlayer)? "YOU LOOSE! AI WINS!":"YOU WIN! AI LOOSES!";
    SDL_Color Noire ={
        .a =255,
        .b = 45,
        .g = 100,
        .r = 100
    };
    SDL_Surface *SurfVicoire = TTF_RenderText_Blended(police, AiWIN, sizeof(AiWIN), Noire);
    if(!SurfVicoire){
        fprintf(stderr, "Erreur de création du texte: %s\n", SDL_GetError());
        return;
    }
    SDL_Texture*pTextureVicoire = SDL_CreateTextureFromSurface(renderer,SurfVicoire);
    if (!pTextureVicoire){
        fprintf(stderr, "Erreur de création de la texture du texte: %s\n", SDL_GetError());
        SDL_DestroySurface(SurfVicoire);
        return;
    }

    int rw = 0, rh = 0;
    SDL_GetCurrentRenderOutputSize(renderer, &rw, &rh);

    SDL_FRect dstVicoireRect;
    dstVicoireRect.x = (float)(rw/2 - SurfVicoire->w / 2);
    dstVicoireRect.y = (float)(rh/2 - SurfVicoire->h / 2);
    dstVicoireRect.w = SurfVicoire->w;
    dstVicoireRect.h = SurfVicoire->h;

    SDL_DestroySurface(SurfVicoire);
    SDL_RenderTexture(renderer, pTextureVicoire, NULL, &dstVicoireRect);
    SDL_DestroyTexture(pTextureVicoire);    
}

void doTheMoveStepAndDisplay(SDL_Renderer * renderer, TTF_Font* police, int PlateauList[12], int pos, bool AI_Turn, int*scorePlayer, int* scoreIA, SDL_Texture* pTextureText, SDL_FRect *dstTextRect, SDL_Texture* pTexture, SDL_FRect* dstRect){
    if (PlateauList == NULL || pos <0 || pos >=12) return;
    int NBPions = PlateauList[pos];
    PlateauList[pos] =0;
    for (int i=1;i<=NBPions;i++){
        if ((pos + i)%12 != pos){// éviter de déposer une pierre dans le trou de départ
            PlateauList[(pos + i)%12] +=1;
        }
        // redraw frame for animation step
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, pTexture, NULL, dstRect); // background texture (pTexture and dstRect are global in file)
        DisplayPlateau(PlateauList, renderer, police, pTextureText, dstTextRect);
        displayScore(renderer, police, *scorePlayer, *scoreIA);
        SDL_RenderPresent(renderer);
        SDL_Delay(800); // pause between increments
    }
    int finalPos = (pos + NBPions) % 12; // position finale
    int minRef = (AI_Turn) ? 6: 0;
    int maxRef = (AI_Turn) ? 11 : 5;
    int NewPlateau [12];
    int scoreGained =0;
    CopyPlateau(PlateauList, NewPlateau);
    while (finalPos >= minRef && finalPos <= maxRef && (NewPlateau[finalPos] == 2 || NewPlateau[finalPos] ==3)){
        scoreGained += NewPlateau[finalPos];
        NewPlateau[finalPos] =0;
        finalPos -=1;
    }
    bool Starving = isMyOpponentStarving(NewPlateau, AI_Turn);
    if (!Starving){ 
        if (AI_Turn){
            *scoreIA += scoreGained;
        } else {
            *scorePlayer += scoreGained;
        }
        finalPos = (pos + NBPions) % 12;
        // Afficher la prise étape par étape
        while (finalPos >= minRef && finalPos <= maxRef && (PlateauList[finalPos] == 2 || PlateauList[finalPos] ==3)){
            PlateauList[finalPos] =0;
            finalPos -=1;
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_RenderTexture(renderer, pTexture, NULL, dstRect);
            DisplayPlateau(PlateauList, renderer, police, pTextureText, dstTextRect);
            displayScore(renderer, police, *scorePlayer, *scoreIA);
            SDL_RenderPresent(renderer);
            SDL_Delay(800); // pause to show capture
        }
    }
}






// ============================================================== 
 
int main(int argc, char* argv[]) {

    int ScorePlayer =0;
    int ScoreIA =0;
    // Initialiser SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Erreur d'initialisation: %s\n", SDL_GetError());
        return -1;
    }
    if (TTF_Init() == -1){
        fprintf(stderr, "Erreur de lancement: %s", SDL_GetError());
        exit(-1);
    } 

    
    // Chargement de la police
    TTF_Font *police = TTF_OpenFont("../assets/fonts/font_film.ttf", 40);
    if (!police){
        fprintf(stderr, "Erreur de chargement de la police: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    } 

    
    // Création fenêtre et renderer
    SDL_Window* fenetre;
    SDL_Renderer* renderer;
    
    bool p = SDL_CreateWindowAndRenderer("Game", 1475, 1000, 
                                         SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_RESIZABLE, 
                                         &fenetre, &renderer);
    if (!p) {
        fprintf(stderr, "Error: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }
    
    SDL_Rect Rect;
    Rect.x = 0;
    Rect.y = 0;
    Rect.h = 600;
    Rect.w = 1375;

    SDL_FRect dstRect = {
        .x = 50,
        .y = 50,
        .h = 600,
        .w = 1375,
    };


    SDL_Color Noire ={
        .a =255,
        .b = 45,
        .g = 100,
        .r = 100
    };
    const char* welcomeMessage = "AWALE";
    SDL_Surface *SurfaceText;
    SurfaceText = TTF_RenderText_Blended(police, welcomeMessage,sizeof(welcomeMessage), Noire);
    if(!SurfaceText){
        fprintf(stderr, "Erreur de création du texte: %s", SDL_GetError());
        TTF_CloseFont(police);
        TTF_Quit();
        SDL_Quit();
        return -1;
    } 
 
    
    SDL_Texture*pTextureText = SDL_CreateTextureFromSurface(renderer,SurfaceText);
    if (!pTextureText){
        fprintf(stderr, "Erreur de création de la texture du texte: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }
     SDL_FRect dstTextRect;
    dstTextRect.x = 1475 / 2 - SurfaceText->w / 2;
    dstTextRect.y = 700;
    dstTextRect.w = SurfaceText->w;
    dstTextRect.h = SurfaceText->h;
    
    SDL_DestroySurface(SurfaceText);

    SDL_Surface* surface = SDL_CreateSurface(1375,600,SDL_PIXELFORMAT_RGBA32); 
    if (!surface){
        fprintf(stderr, "Erreur de création de la surface: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    } 
    Uint32 blanc = SDL_MapRGBA(SDL_GetPixelFormatDetails(surface->format),NULL,120,120,200, 255);
    SDL_FillSurfaceRect(surface, &Rect,blanc);
    
    //Dessiner les trous
    for (int i = 0; i<6;i++){
        drawCercle(225*i+125,125,100, surface);
        drawCercle(225*i+125, 450,100, surface);
    }

    // Création de la texture
    SDL_Texture* pTexture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!pTexture){
        fprintf(stderr, "Erreur de création de la texture: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }
    
    SDL_DestroySurface(surface);

    // On souhite garder une liste des pointeurs sur chaque trou pour faciliter la gestion des évenements
    SDL_Event event;
    bool isOpen = true;
    bool IA_Turn = true; // L'IA commence toujours
    while (isOpen) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, pTexture,NULL, &dstRect);

        DisplayPlateau(LIST_PIONS, renderer, police, pTextureText, &dstTextRect);
        displayScore(renderer, police, ScorePlayer, ScoreIA);
        SDL_RenderPresent(renderer);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                isOpen = false;
            }
            if (IA_Turn) {
                // Tour de l'IA
                Choice bestMove = MiniMax(LIST_PIONS, true, 5);
                if (bestMove.pos != -1) {
                    doTheMoveStepAndDisplay(renderer, police, LIST_PIONS, bestMove.pos, true, &ScorePlayer, &ScoreIA, pTextureText, &dstTextRect, pTexture, &dstRect);
                }
                if(Finality(LIST_PIONS, !IA_Turn)){
                    ScoreIA += getNumPionsOfPlayer(LIST_PIONS, true);
                    ScorePlayer -= getNumPionsOfPlayer(LIST_PIONS, false);
                    SDL_RenderTexture(renderer, pTextureText, NULL, &dstTextRect);
                    displayVictory( renderer, police, ScorePlayer, ScoreIA);
                    SDL_RenderPresent(renderer);
                    SDL_Delay(5000); // Afficher le message de victoire pendant 5 secondes
                    isOpen = false; // Fermer la fenêtre après l'affichage
                }
                IA_Turn = false; // Passer le tour au joueur humain
            }
            else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && !IA_Turn) {
                int x = event.button.x;
                int y = event.button.y;
                int pos = PosClicked(x, y);
                bool isValidMove = false;
                if (pos != -1) {
                    int ListPossibleMoves[6];
                    int nbPos = GetPossibleMoves(LIST_PIONS, ListPossibleMoves, false);
                    
                    for (int i = 0; i < nbPos; i++) {
                        if (ListPossibleMoves[i] == pos) {
                            isValidMove = true;
                            break;
                        }
                    }
                    if (isValidMove) {
                        doTheMoveStepAndDisplay(renderer, police, LIST_PIONS, pos, false, &ScorePlayer, &ScoreIA, pTextureText, &dstTextRect, pTexture, &dstRect);
                    }
                }
                if(Finality(LIST_PIONS, !IA_Turn)){
                    ScoreIA += getNumPionsOfPlayer(LIST_PIONS, true);
                    ScorePlayer -= getNumPionsOfPlayer(LIST_PIONS, false);
                    SDL_RenderTexture(renderer, pTextureText, NULL, &dstTextRect);
                    displayVictory( renderer, police, ScorePlayer, ScoreIA);
                    SDL_RenderPresent(renderer);
                    SDL_Delay(5000); // Afficher le message de victoire pendant 5 secondes
                    isOpen = false; // Fermer la fenêtre après l'affichage
                }
                if (isValidMove) {
                    IA_Turn = true; // Passer le tour à l'IA 
                }// Sinon, on attend que le joueur clique sur un trou valide
            }
        }
    }

    // Nettoyage et fermeture
    SDL_DestroyTexture(pTexture);
    SDL_DestroyTexture(pTextureText); 
    TTF_CloseFont(police);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(fenetre);
    SDL_Quit();   
    return 0;
}