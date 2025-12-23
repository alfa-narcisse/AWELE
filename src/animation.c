#include "umbrella.h"

#define graine_widht 40
#define M_PI 3.14159265358979323846
#define R_TROU 160
#define MAX_DISPERSION 30



void drawThehand(SDL_Renderer *PlateauRenderer,SDL_Texture *handTexture,SDL_Texture *handTextureLeft,SDL_Texture *bgTexture,SDL_Texture *graineTexture,int POS_TROUS[12][2],TTF_Font* policePlateau,
    int PlateauList[12],int pos_debut,int pos_suiv,Button*ListButtons[],int nbButtons,int POS_RECT[12][2],bool VsAI,bool player1Turn,int *scorePlayer1,int *scorePlayer2){




    int POS_RECT_HAND[12][2]={{985,220},{815,220},{645,220},{475,220},{305,220},{135,220},
                        {135,440},{305,440},{475,440},{645,440},{815,440},{985,440}};

    int x=POS_RECT_HAND[pos_debut][0];
    int y=POS_RECT_HAND[pos_debut][1];
    int xa=POS_RECT_HAND[pos_suiv][0];
    int ya=POS_RECT_HAND[pos_suiv][1];
    float duree=300; // durée de l'animation en millisecondes
    float tempdebut=SDL_GetTicks();
    

    while(1){
        
        float temps_ecoule=SDL_GetTicks() - tempdebut;
        float t= (float)temps_ecoule/duree;
        if (t>1.0f) t=1.0f;
        SDL_RenderClear(PlateauRenderer);
        SDL_RenderTexture(PlateauRenderer,bgTexture,NULL,NULL);
        displayScores(PlateauRenderer, policePlateau,VsAI, *scorePlayer1, *scorePlayer2);  
        displayContainsOfHoles(PlateauList, PlateauRenderer, policePlateau, POS_TROUS,POS_RECT);
               
         for (int btn=0; btn<nbButtons; btn++){
                 renderbutton(PlateauRenderer, ListButtons[btn]);
            }
        
        drawAllThePlaterSeeds(PlateauRenderer,graineTexture,POS_TROUS,PlateauList);
        
        //SDL_RenderPresent(PlateauRenderer);
        float x_t= x + t*(xa - x);
        float y_t= y + t*(ya - y);
        SDL_FRect destRect={
            x_t ,
            y_t,
            160,
            160

        };
        if (player1Turn){ 
            SDL_RenderTexture(PlateauRenderer,handTextureLeft,NULL,&destRect);
        }
        else {
            SDL_RenderTexture(PlateauRenderer,handTexture,NULL,&destRect);
        }
        SDL_RenderPresent(PlateauRenderer);
        if (t>=1.0f) break;
        
        

        SDL_Delay(10); 
        



    }


    
}


void drawTheHandToScore(SDL_Renderer *PlateauRenderer,SDL_Texture *handTexture,SDL_Texture *handTextureLeft,SDL_Texture *bgTexture,SDL_Texture *graineTexture,int POS_TROUS[12][2],TTF_Font* policePlateau,
    int PlateauList[12],int POS_RECT[12][2],bool VsAI,bool player1Turn,int *scorePlayer1,int *scorePlayer2,int pos ){

        int POS_RECT_SCORE[2][2]={{35,500},{1185,500}};
        int POS_RECT_HAND[12][2]={{985,220},{815,220},{645,220},{475,220},{305,220},{135,220},
                        {135,440},{305,440},{475,440},{645,440},{815,440},{985,440}};
    int x=POS_RECT_HAND[pos][0];
    int y=POS_RECT_HAND[pos][1];
    int xa= (player1Turn)? POS_RECT_SCORE[0][0]:POS_RECT_SCORE[1][0];
    int ya= (player1Turn)? POS_RECT_SCORE[0][1]:POS_RECT_SCORE[1][1];
    float duree=300; // durée de l'animation en millisecondes
    float tempdebut=SDL_GetTicks();
    while(1){
        
        float temps_ecoule=SDL_GetTicks()-tempdebut;
        float t= (float)temps_ecoule/duree;
        if (t>1.0f) t=1.0f;
        SDL_RenderClear(PlateauRenderer);
        SDL_RenderTexture(PlateauRenderer,bgTexture,NULL,NULL);
        displayScores(PlateauRenderer, policePlateau,VsAI, *scorePlayer1, *scorePlayer2);  
        displayContainsOfHoles(PlateauList, PlateauRenderer, policePlateau, POS_TROUS,POS_RECT);
               
        
        drawAllThePlaterSeeds(PlateauRenderer,graineTexture,POS_TROUS,PlateauList);
        
        //SDL_RenderPresent(PlateauRenderer);
        float x_t= x + t*(xa - x);
        float y_t= y + t*(ya - y);
        SDL_FRect destRect={
            x_t ,
            y_t,
            160,
            160

        };
        if (player1Turn){
            SDL_RenderTexture(PlateauRenderer,handTextureLeft,NULL,&destRect);
        } else {
            SDL_RenderTexture(PlateauRenderer,handTexture,NULL,&destRect);
        }
        
        SDL_RenderPresent(PlateauRenderer);
        if (t>=1.0f) break;
        
        

        SDL_Delay(10);        


    }

}


void drawSeedInTheHole(SDL_Renderer *PlateauRenderer,SDL_Texture *graineTexture,int tx,int ty,int n){

    if (n!=0){for (int i=0;i<n;i++){

        float angle = i * 137.5f * (M_PI / 180.0f); 

        
        float rayon = (i == 0) ? 0 : 20.0f * sqrtf((float)i);

        // Limiter le rayon pour ne pas sortir du trou
        if (rayon > MAX_DISPERSION) rayon = MAX_DISPERSION;
        //float angle=i*(360.0f/n)*(M_PI/180.0f);
        //float rayon=R_TROU;
        int px=(int)(rayon*cosf(angle));
        int py=(int)(rayon*sinf(angle));
        SDL_FRect destRect={
            tx + px - graine_widht/2,
            ty + py - graine_widht/2,
            graine_widht,
            graine_widht
        };
        SDL_RenderTexture(PlateauRenderer,graineTexture,NULL,&destRect);
    }}

}


void drawAllThePlaterSeeds(SDL_Renderer *PlateauRenderer,SDL_Texture *graineTexture,int POS_TROUS[12][2],int ListePions[12]){
    int x,y,n;
    for (int i=0;i<12;i++){
        x=POS_TROUS[i][0];
        y=POS_TROUS[i][1];
        n=ListePions[i];
        drawSeedInTheHole(PlateauRenderer,graineTexture,x,y,n);
    }
}