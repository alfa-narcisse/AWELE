#include "display.h"
#include <stdio.h>
#include <stdbool.h>
#include <SDL3/SDL.h> 
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include "outils.h"




static void displayScorePlayer(SDL_Renderer*scoreRenderer, TTF_Font* police, int ScorePlayer1,bool VsAi, bool player1Turn){
    char tscorePlayer1[3];
    snprintf(tscorePlayer1, sizeof(tscorePlayer1), "%d", ScorePlayer1);

    char name[3];
    char namePlayer1[3];
    snprintf(name, sizeof(name), "%s", player1Turn ? "J1" : "J2");
    snprintf(namePlayer1, sizeof(namePlayer1), "%s", VsAi ? "AI" : name);


    SDL_Color grisFonce ={
        .a =255,
        .b = 0,
        .g = 30,
        .r = 51
    };

    SDL_Surface *SurfaceScore = TTF_RenderText_Blended(police, tscorePlayer1,sizeof(tscorePlayer1), grisFonce);
    if(!SurfaceScore){
        fprintf(stderr, "Erreur de création du texte: %s\n", SDL_GetError());
        return;
    }
    SDL_Surface *SurfaceName = TTF_RenderText_Blended(police, namePlayer1,sizeof(namePlayer1), grisFonce);
    if(!SurfaceName){
        fprintf(stderr, "Erreur de création du texte: %s\n", SDL_GetError());
        return;
    }

    SDL_Texture *TextureScore = SDL_CreateTextureFromSurface(scoreRenderer, SurfaceScore);
    if(!TextureScore){
        fprintf(stderr, "Erreur de création de la texture: %s\n", SDL_GetError());
        SDL_DestroySurface(SurfaceScore);
        return;
    }
    SDL_Texture *TextureName = SDL_CreateTextureFromSurface(scoreRenderer, SurfaceName);
    if(!TextureName){
        fprintf(stderr, "Erreur de création de la texture: %s\n", SDL_GetError());
        SDL_DestroySurface(SurfaceName);
        return;
    }


    SDL_FRect nameRect = {
        .x=(player1Turn)? 15+10:1165+10,
        .y= 250+10,
        .h = 40,
        .w = 60
    };
    SDL_FRect scoreRect = {
        .x=(player1Turn)? 15+10:1165+10,
        .y= 470+10,
        .h = 40,
        .w = 60
    };

    SDL_DestroySurface(SurfaceScore);
    SDL_DestroySurface(SurfaceName);

    SDL_RenderTexture(scoreRenderer, TextureName, NULL, &nameRect);
    SDL_RenderTexture (scoreRenderer, TextureScore, NULL, &scoreRect);

    SDL_DestroyTexture(TextureName);
    SDL_DestroyTexture(TextureScore);
}

void displayScores(SDL_Renderer* renderer, TTF_Font*police,bool VsAI, int scorePlayer1, int scorePlayer2){
    displayScorePlayer(renderer, police, scorePlayer1,VsAI,true);
    displayScorePlayer(renderer, police, scorePlayer2,false,false);
};





void displayContainsOfHoles(int ListePions[12], SDL_Renderer * renderer, TTF_Font* police, int POS_TROUS[12][2], int POS_RECT[12][2]){
    for (int i = 0; i < 12; i++) {
        char pionText[3];
        snprintf(pionText, sizeof(pionText), "%d", ListePions[i]);
        
        SDL_Color Noire ={
            .a =255,
            .b = 45,
            .g = 100,
            .r = 100
        };
        SDL_Surface *SurfaceText = TTF_RenderText_Blended(police, pionText, sizeof(pionText), Noire);
        if(!SurfaceText){
            fprintf(stderr, "Erreur de création du texte: %s\n", SDL_GetError());
            continue;
        }   
        SDL_Texture *TextureText = SDL_CreateTextureFromSurface(renderer, SurfaceText);
        if(!TextureText){
            fprintf(stderr, "Erreur de création de la texture: %s\n", SDL_GetError());
            SDL_DestroySurface(SurfaceText);
            continue;
        }
        SDL_DestroySurface(SurfaceText);
        SDL_FRect r1 = { .x = POS_RECT[i][0] +30,
                        .y = POS_RECT[i][1] +10,
                        .h = 30,
                        .w = 40
                    };

        SDL_FRect dstRect;
        dstRect.w = 60; // Largeur fixe pour le texte
        dstRect.h = 70; // Hauteur fixe pour le texte
        dstRect.x = POS_TROUS[i][0] - dstRect.w / 2; // Centrer le texte
        dstRect.y = POS_TROUS[i][1] - dstRect.h / 2; // Centrer le texte
    
        // Dessiner la texture du nombre
        SDL_RenderTexture(renderer, TextureText, NULL, &dstRect);
        SDL_RenderTexture(renderer,TextureText, NULL, &r1);
        SDL_DestroyTexture(TextureText);
    }
}

SDL_Texture* loadPlateauBackground(SDL_Renderer * renderer, const char* filepath){
    SDL_Surface* surface = IMG_Load(filepath);
    if (!surface){
        fprintf(stderr, "Erreur de chargement de la surface: %s\n", SDL_GetError());
        return NULL;
    } 
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    return texture;
}


// Plateau de jeu (Pour essai)
static void drawCercle(int x, int y, int R, SDL_Surface* surface) {
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

void drawGameBG(SDL_Renderer* plateauRenderer){
    SDL_Texture* plateauTexture = IMG_LoadTexture(plateauRenderer, "../assets/images/plateauBg.png");
    if(!plateauTexture){
        fprintf(stderr,"Erreur lors du chargement de l'image: %s", SDL_GetError());
        return;
    }
    SDL_RenderTexture(plateauRenderer,plateauTexture,NULL,NULL);
    SDL_DestroyTexture(plateauTexture);
    SDL_RenderPresent(plateauRenderer);
}

SDL_Texture* createSurfaceTexturePlateau(SDL_Renderer * renderer){
    SDL_Surface* surface = SDL_CreateSurface(1280,720,SDL_PIXELFORMAT_RGBA32); 
    if (!surface){
        fprintf(stderr, "Erreur de création de la surface: %s\n", SDL_GetError());
        return NULL;
    } 
    Uint32 blanc = SDL_MapRGBA(SDL_GetPixelFormatDetails(surface->format),NULL,120,120,200, 255);
    SDL_Rect R1 ={
        .x =15,
        .y = 250,
        .h = 100,
        .w = 100,
    };
    SDL_Rect R2 ={
        .x =15,
        .y = 470,
        .h = 100,
        .w = 100,
    };

    SDL_Rect R3 ={
        .x =125,
        .y = 120,
        .h = 580,
        .w = 1030,
    };
    SDL_Rect R4 ={
        .x =1150,
        .y = 250,
        .h = 100,
        .w = 100,
    };
       SDL_Rect R5 ={
        .x =1150,
        .y = 250,
        .h = 100,
        .w = 100,
    };

    SDL_FillSurfaceRect(surface,&R1,blanc);
    SDL_FillSurfaceRect(surface,&R2,blanc);
    SDL_FillSurfaceRect(surface,&R3,blanc);
    SDL_FillSurfaceRect(surface,&R4,blanc);
    SDL_FillSurfaceRect(surface,&R5, blanc);
    //Dessiner les trous
    for (int i = 0; i<6;i++){
        SDL_Rect r1 = { .x = 135+170*i,
                        .y = 135,
                        .h = 70,
                        .w = 160
                    };
        SDL_Rect r2 = { .x = 135+170*i,
                        .y = 615,
                        .h = 70,
                        .w = 160
                    };
        SDL_FillSurfaceRect(surface,&r1,blanc);
        SDL_FillSurfaceRect(surface,&r2, blanc);
        drawCercle(215 + 170*i,300,80, surface);
        drawCercle(215 + 170*i, 520,80, surface);
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    return texture;
}


void displayPlateauWithDelay(
    SDL_Renderer * rendererPlateau,
    SDL_Texture*plateauTexture, 
    TTF_Font* policePlateau,
    Button*ListButtons[],
    int nbButtons,
    int POS_TROUS[12][2],
    int POS_RECT[12][2],
    int ListePions[12],
    int scorePlayer1,
    int scorePlayer2,
    bool VsAI,
    int delayMs
    ){
    SDL_RenderClear(rendererPlateau);
    SDL_RenderTexture(rendererPlateau, plateauTexture, NULL,NULL);
    for (int btn=0; btn<nbButtons; btn++){
                 renderbutton(rendererPlateau, ListButtons[btn]);
            } 
    //SDL_RenderTexture(rendererPlateau,plateauTexture,NULL,NULL);
    displayScores(rendererPlateau, policePlateau,VsAI, scorePlayer1,scorePlayer2);
    displayContainsOfHoles(ListePions, rendererPlateau, policePlateau, POS_TROUS,POS_RECT);
    SDL_RenderPresent(rendererPlateau);
    SDL_Delay(delayMs); 
}

//
