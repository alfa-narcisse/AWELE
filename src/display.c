#include "display.h"
#include <stdio.h>
#include <stdbool.h>
#include <SDL3/SDL.h> 
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include "outils.h"


void drawButton(SDL_Renderer* renderer, Button* button, TTF_Font* police){
    SDL_Color colorText = {255,255,255,255};
    Uint32 colorButton = (button->hovered)?SDL_MapRGBA(SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_ARGB32),NULL,100,100,100, 255):SDL_MapRGBA(SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_ARGB32),NULL,120,120,120, 255);
    SDL_Surface *SurfaceButton = TTF_RenderText_Blended(police,button->text,sizeof(button->text), colorText);
    if (!SurfaceButton){
        fprintf(stderr, "Erreur de création du texte: %s\n", SDL_GetError());
        return;
    }
    SDL_Surface* btnSurface= SDL_CreateSurface((int)button->rect->w, (int)button->rect->h, SDL_PIXELFORMAT_ARGB32);
    if (!btnSurface){
        fprintf(stderr, "Erreur de création du texte: %s\n", SDL_GetError());
        return;
    }
    SDL_FillSurfaceRect(btnSurface,NULL,colorButton);

    SDL_FRect fondRect ={
        .x = button->rect->x-5,
        .y = button->rect->y,
        .h = button->rect->h+5,
        .w = button->rect->w+5
    };
    SDL_Texture* fondBtn = SDL_CreateTextureFromSurface(renderer, btnSurface);
    SDL_Texture * textureButton = SDL_CreateTextureFromSurface(renderer, SurfaceButton);
    SDL_RenderTexture(renderer,fondBtn,NULL,&fondRect);
    SDL_RenderTexture(renderer, textureButton, NULL, button->rect);
    SDL_DestroySurface(btnSurface);
    SDL_DestroySurface(SurfaceButton);
    SDL_DestroyTexture(fondBtn);
    SDL_DestroyTexture(textureButton);
};

static void displayScorePlayer(SDL_Renderer * renderer, TTF_Font* police, int ScorePlayer1,bool VsAi, bool player1Turn){
    char tscorePlayer1[3];
    snprintf(tscorePlayer1, sizeof(tscorePlayer1), "%d", ScorePlayer1);

    char name[5];
    char namePlayer1[5];
    snprintf(name, sizeof(name), "%s", player1Turn ? "J-1" : "J-2");
    snprintf(namePlayer1, sizeof(namePlayer1), "%s", VsAi ? "AI" : name);


    SDL_Color Noire ={
        .a =255,
        .b = 45,
        .g = 100,
        .r = 100
    };

    SDL_Surface *SurfaceScore = TTF_RenderText_Blended(police, tscorePlayer1,sizeof(tscorePlayer1), Noire);
    if(!SurfaceScore){
        fprintf(stderr, "Erreur de création du texte: %s\n", SDL_GetError());
        return;
    }
    SDL_Surface *SurfaceName = TTF_RenderText_Blended(police, namePlayer1,sizeof(namePlayer1), Noire);
    if(!SurfaceName){
        fprintf(stderr, "Erreur de création du texte: %s\n", SDL_GetError());
        return;
    }

    SDL_Texture *TextureScore = SDL_CreateTextureFromSurface(renderer, SurfaceScore);
    if(!TextureScore){
        fprintf(stderr, "Erreur de création de la texture: %s\n", SDL_GetError());
        SDL_DestroySurface(SurfaceScore);
        return;
    }
    SDL_Texture *TextureName = SDL_CreateTextureFromSurface(renderer, SurfaceName);
    if(!TextureName){
        fprintf(stderr, "Erreur de création de la texture: %s\n", SDL_GetError());
        SDL_DestroySurface(SurfaceName);
        return;
    }
    int renderer_w = 0, renderer_h = 0;
    SDL_GetCurrentRenderOutputSize(renderer, &renderer_w, &renderer_h); // dynamic renderer/window size
    SDL_FRect ScoreRect = {
        .x=(player1Turn)? 55:1550,
        .y= 440,
        .h = 20,
        .w = 30
    };
    SDL_FRect nameRect = {
        .x=(player1Turn)? 55:1550,
        .y= 270,
        .h = 20,
        .w = 30
    };

    SDL_DestroySurface(SurfaceScore);
    SDL_DestroySurface(SurfaceName);
    SDL_RenderTexture(renderer, TextureName, NULL, &nameRect);
    SDL_RenderTexture (renderer, TextureScore, NULL, &ScoreRect);

    SDL_DestroyTexture(TextureName);
    SDL_DestroyTexture(TextureScore);
}

void displayScores(SDL_Renderer* renderer, TTF_Font*police,bool VsAI, int scorePlayer1, int scorePlayer2){
    displayScorePlayer(renderer, police, scorePlayer1,VsAI,true);
    displayScorePlayer(renderer, police, scorePlayer2,false,false);
};

void displayVictory(SDL_Renderer * renderer, TTF_Font* police, int ScorePlayer1, int ScorePlayer2, bool VsAI){
    char victoryMessage[15];
    if (ScorePlayer1 == ScorePlayer2){
        snprintf(victoryMessage, sizeof(victoryMessage), "== EXAEQUO %s", "==");
    }
    else{
        if (VsAI){
            snprintf(victoryMessage, sizeof(victoryMessage), "WINNER: %s", (ScorePlayer1>ScorePlayer2)?"AI":"Joueur 2");
        }
        else{
            snprintf(victoryMessage, sizeof(victoryMessage), "WINNER: %s", (ScorePlayer1>ScorePlayer2)?"Joueur 1":"Joueur 2");
        }
    }

    SDL_Color Noire ={
        .a =255,
        .b = 45,
        .g = 100,
        .r = 100
    };
    SDL_Color yellow= {
        .a = 255,
        .b = 255,
        .g = 255,
        .r =0
    };
    SDL_Surface *SurfaceVictory = TTF_RenderText_Shaded(police, victoryMessage, sizeof(victoryMessage), Noire, yellow);
    if(!SurfaceVictory){
        fprintf(stderr, "Erreur de création du texte: %s\n", SDL_GetError());
        return;
    }   
    SDL_Texture *TextureVictory = SDL_CreateTextureFromSurface(renderer, SurfaceVictory);
    if(!TextureVictory){
        fprintf(stderr, "Erreur de création de la texture: %s\n", SDL_GetError());
        SDL_DestroySurface(SurfaceVictory);
        return;
    }
    int renderer_w = 0, renderer_h = 0;
    SDL_GetCurrentRenderOutputSize(renderer, &renderer_w, &renderer_h); // dynamic renderer/window size
    SDL_FRect VictoryRect = {
        .x = (renderer_w - SurfaceVictory->w) / 2.0f, // centered horizontally
        .y = (renderer_h - SurfaceVictory->h) / 2.0f, // centered vertically
        .h = 100,// width
        .w = 400 // height
    };

    SDL_DestroySurface(SurfaceVictory);
    SDL_RenderTexture   (renderer, TextureVictory, NULL, &VictoryRect);
    SDL_DestroyTexture(TextureVictory);
}


void displayContainsOfHoles(int ListePions[12], SDL_Renderer * renderer, TTF_Font* police, int POS_TROUS[12][2]){
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

        SDL_FRect dstRect;
        dstRect.w = 50; // Largeur fixe pour le texte
        dstRect.h = 50; // Hauteur fixe pour le texte
        dstRect.x = POS_TROUS[i][0] - dstRect.w / 2; // Centrer le texte
        dstRect.y = POS_TROUS[i][1] - dstRect.h / 2; // Centrer le texte
    
        // Dessiner la texture du nombre
        SDL_RenderTexture(renderer, TextureText, NULL, &dstRect);
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

void drawPlateauBackground(SDL_Renderer * renderer, SDL_Texture* plateauTexture){
    if (!plateauTexture) return;
    
    float t_w = 0, t_h = 0;
    // dynamic texture size
    SDL_GetTextureSize(plateauTexture, &t_w, &t_h);
    SDL_FRect dstRect = {
        .x = 25,
        .y = 100,
        .w = (float)t_w,
        .h = (float)t_h,
    };

    SDL_RenderTexture(renderer, plateauTexture, NULL, &dstRect);
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

void displayTitleOfGame(SDL_Renderer * renderer, TTF_Font* police){
    const char* welcomeMessage = "AWELE";
    SDL_Color Noire ={
        .a =255,
        .b = 45,
        .g = 100,
        .r = 100
    };
    SDL_Surface *SurfaceText = TTF_RenderText_Blended(police, welcomeMessage,sizeof(welcomeMessage), Noire);
    if(!SurfaceText){
        fprintf(stderr, "Erreur de création du texte: %s\n", SDL_GetError());
        return;
    } 
 
    
    SDL_Texture*pTextureText = SDL_CreateTextureFromSurface(renderer,SurfaceText);
    if (!pTextureText){
        fprintf(stderr, "Erreur de création de la texture du texte: %s\n", SDL_GetError());
        SDL_DestroySurface(SurfaceText);
        return;
    }
    int r_w =0, r_h=0;
    SDL_GetCurrentRenderOutputSize(renderer, &r_w, &r_h);
    SDL_FRect dstTextRect;
    dstTextRect.x = r_w / 2 - SurfaceText->w / 2;
    dstTextRect.y = 10;
    dstTextRect.w = SurfaceText->w;
    dstTextRect.h = SurfaceText->h;
    
    SDL_DestroySurface(SurfaceText);
    SDL_RenderTexture(renderer, pTextureText, NULL, &dstTextRect);
    SDL_DestroyTexture(pTextureText);  
}


SDL_Texture* createSurfaceTexturePlateau(SDL_Renderer * renderer){
    SDL_Surface* surface = SDL_CreateSurface(1595,550,SDL_PIXELFORMAT_RGBA32); 
    if (!surface){
        fprintf(stderr, "Erreur de création de la surface: %s\n", SDL_GetError());
        return NULL;
    } 
    Uint32 blanc = SDL_MapRGBA(SDL_GetPixelFormatDetails(surface->format),NULL,120,120,200, 255);
    SDL_Rect R1 ={
        .x =0,
        .y = 150,
        .h = 80,
        .w = 100,
    };
    SDL_Rect R2 ={
        .x =0,
        .y = 280,
        .h = 120,
        .w = 100,
    };

    SDL_Rect R3 ={
        .x =110,
        .y = 0,
        .h = 550,
        .w = 1375,
    };
    SDL_Rect R4 ={
        .x =1495,
        .y = 150,
        .h = 80,
        .w = 100,
    };
       SDL_Rect R5 ={
        .x =1495,
        .y = 280,
        .h = 120,
        .w = 100,
    };

    SDL_FillSurfaceRect(surface,&R1,blanc);
    SDL_FillSurfaceRect(surface,&R2,blanc);
    SDL_FillSurfaceRect(surface,&R3,blanc);
    SDL_FillSurfaceRect(surface,&R4,blanc);
    SDL_FillSurfaceRect(surface,&R5, blanc);
    
    //Dessiner les trous
    for (int i = 0; i<6;i++){
        drawCercle(225*i+125+110,125,100, surface);
        drawCercle(225*i+125+110, 425,100, surface);
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    return texture;
}


void displayAllWithDelay(
    SDL_Renderer * renderer,
    SDL_Texture*plateauTexture, 
    TTF_Font* policePlateau,
    TTF_Font* policeButton,
    int ListePions[12],
    int POS_TROUS[12][2],
    int scorePlayer1,
    int scorePlayer2,
    bool VsAI,
    int delayMs,
    Button* btn
    ){

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    drawPlateauBackground(renderer, plateauTexture);
    displayContainsOfHoles(ListePions, renderer, policePlateau, POS_TROUS);
    displayTitleOfGame(renderer, policePlateau);
    drawButton(renderer, btn, policeButton);
    displayScores(renderer, policePlateau,VsAI, scorePlayer1, scorePlayer2);
    SDL_RenderPresent(renderer);
    SDL_Delay(delayMs); 
}


void displayFinality(
    SDL_Renderer*renderer,
    TTF_Font*policePlateau,
    int ListePions[12],
    int scorePlayer1,
    int scorePlayer2,
    bool VsAI
){
    scorePlayer1 += getNumPionsOfPlayer(ListePions,true);            
    scorePlayer2 -= getNumPionsOfPlayer(ListePions,false);
    SDL_SetRenderDrawColor(renderer,0,255,255,255);
    SDL_RenderClear(renderer);
    displayVictory(renderer,policePlateau,scorePlayer1, scorePlayer2, VsAI);
    SDL_RenderPresent(renderer);
    SDL_Delay(5000);
}