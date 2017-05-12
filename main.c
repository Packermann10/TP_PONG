// TP PONG SDL2

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <time.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 780


typedef struct game{
     SDL_Window *g_pWindow;
     SDL_Renderer *g_pRenderer;
     SDL_Texture *g_texture;
     SDL_Surface *g_surface;
}game;

typedef struct gameState{
    int g_bRunning;
}gameState;

typedef struct font{
    TTF_Font *g_font;
}font;

typedef struct coord{
    int x;
    int y;
}coord;

typedef struct balle_jeu{
    coord milieu;
    int direction;
    int rayon;
}balle_jeu;

int init(char *title, int xpos,int ypos,int height, int width,int flags,game *myGame, font *mFont, SDL_Rect *joueur1, SDL_Rect *joueur2, int *scorej1, int *scorej2, balle_jeu *balle);
void controle(gameState *state, SDL_Rect *joueur1, SDL_Rect *joueur2, game *myGame);
void delay(unsigned int frameLimit);
void destroy(game *myGame,font *mFont);
void affiche(game *myGame,font mFont, int scorej1, int scorej2, SDL_Rect joueur1, SDL_Rect joueur2, balle_jeu balle);
void dessineballe(game *myGame,int x0, int y0, int radius);

//initialisation
int init(char *title, int xpos,int ypos,int height, int width,int flags,game *myGame, font *mFont, SDL_Rect *joueur1, SDL_Rect *joueur2, int *scorej1, int *scorej2, balle_jeu *balle){

    //initialize SDL
    if(SDL_Init(SDL_INIT_EVERYTHING)>=0){
            //if succeeded create our window
            myGame->g_pWindow=SDL_CreateWindow(title,xpos,ypos,height,width,flags);
            //if succeeded create window, create our render
            if(myGame->g_pWindow!=NULL){
                myGame->g_pRenderer=SDL_CreateRenderer(myGame->g_pWindow,-1,SDL_RENDERER_ACCELERATED);
            }
    }else{
        return 0;
    }

    if(TTF_Init() == -1){
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    //choisir la police
    mFont->g_font=TTF_OpenFont("C:/Windows/Fonts/Arial/Arial.ttf",65);

    //init position raquettes
    joueur1->x=20;
    joueur1->y=20;
    joueur1->h=100;
    joueur1->w=20;

    joueur2->x=SCREEN_WIDTH-40;
    joueur2->y=20;
    joueur2->h=100;
    joueur2->w=20;

    //init scores
    scorej1=0;
    scorej2=0;

    //init balle
    balle->milieu.x=SCREEN_WIDTH/2;
    balle->milieu.y=SCREEN_HEIGHT/2;
    balle->direction=rand()%4+1;
    balle->rayon=10;

    return 1;
}

//procédure pour contrôler les raquettes
void controle(gameState *state, SDL_Rect *joueur1, SDL_Rect *joueur2, game *myGame){

    SDL_Event event;

    if(SDL_PollEvent(&event)){
        switch(event.type){
        case SDL_QUIT:
              state->g_bRunning=0;break;
        case SDL_KEYDOWN:
                        switch (event.key.keysym.sym)
                            {
                                case SDLK_z:
                                    if (joueur1->y>=20){
                                        joueur1->y=joueur1->y-30;
                                    }
                                    break;
                                case SDLK_s:
                                    if (joueur1->y<SCREEN_HEIGHT-100){
                                        joueur1->y=joueur1->y+30;
                                    }
                                     break;
                                case SDLK_UP:
                                     if (joueur2->y>=20){
                                        joueur2->y=joueur2->y-30;
                                    }
                                     break;
                                case SDLK_DOWN:
                                    if (joueur2->y<SCREEN_HEIGHT-100){
                                        joueur2->y=joueur2->y+30;
                                    }
                                     break;
                            }
                            break;
        case SDL_KEYUP:;break;

        default:break;

        }
    }
}

//procédure d'affichage
void affiche(game *myGame,font mFont,int scorej1, int scorej2, SDL_Rect joueur1, SDL_Rect joueur2, balle_jeu balle) {

        SDL_Color fontColor={255,255,255};
        char buffer[30];
        sprintf(buffer,"%i - %i",scorej1,scorej2);

        // Afficher le score
        myGame->g_surface=TTF_RenderText_Blended(mFont.g_font, buffer, fontColor);//Charge la police

        if(myGame->g_surface){
                SDL_Rect rectangle;
                rectangle.x=(SCREEN_WIDTH/2)-100;//debut x
                rectangle.y=20;//debut y
                rectangle.w=30; //Largeur
                rectangle.h=50; //Hauteur

                 myGame->g_texture = SDL_CreateTextureFromSurface(myGame->g_pRenderer,myGame->g_surface);
                 SDL_FreeSurface(myGame->g_surface);

                 if(myGame->g_texture){
                        SDL_RenderCopy(myGame->g_pRenderer,myGame->g_texture,NULL,&rectangle);
                 }
                 else{
                        fprintf(stdout,"Échec de création de la texture (%s)\n",SDL_GetError());
                }
        }else{
            fprintf(stdout,"Échec de creation surface pour chaine (%s)\n",SDL_GetError());
        }

        //Affichage raquettes
        SDL_SetRenderDrawColor(myGame->g_pRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(myGame->g_pRenderer,&joueur1);
        SDL_RenderFillRect(myGame->g_pRenderer,&joueur2);

        //Affichage balle
        dessineballe(myGame,balle.milieu.x,balle.milieu.y,balle.rayon);

        //Affichage RENDERER
        SDL_RenderPresent(myGame->g_pRenderer);
}

//procédure pour la gestion des FPS
void delay(unsigned int frameLimit){

    unsigned int ticks = SDL_GetTicks();

    if (frameLimit < ticks){
        return;
    }

    if (frameLimit > ticks + 16){
        SDL_Delay(16);
    }else{
        SDL_Delay(frameLimit - ticks);
    }
}

//procédure de destruction
void destroy(game *myGame,font *mFont){

    if(mFont->g_font!=NULL){
        TTF_CloseFont(mFont->g_font);
        mFont->g_font=NULL;
    }

    //Destroy texture
    if(myGame->g_texture!=NULL)
            SDL_DestroyTexture(myGame->g_texture);

    //Destroy render
    if(myGame->g_pRenderer!=NULL)
        SDL_DestroyRenderer(myGame->g_pRenderer);

    //Destroy window
    if(myGame->g_pWindow!=NULL)
        SDL_DestroyWindow(myGame->g_pWindow);
}

//procédure pour dessiner la balle et lui rendre sa forme ronde
void dessineballe(game *myGame,int x0, int y0, int radius){
    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y){
        SDL_RenderDrawPoint(myGame->g_pRenderer,x0+x,y0+y);
        SDL_RenderDrawPoint(myGame->g_pRenderer,x0+y,y0+x);
        SDL_RenderDrawPoint(myGame->g_pRenderer,x0-y,y0+x);
        SDL_RenderDrawPoint(myGame->g_pRenderer,x0-x,y0+y);
        SDL_RenderDrawPoint(myGame->g_pRenderer,x0-x,y0-y);
        SDL_RenderDrawPoint(myGame->g_pRenderer,x0-y,y0-x);
        SDL_RenderDrawPoint(myGame->g_pRenderer,x0+y,y0-x);
        SDL_RenderDrawPoint(myGame->g_pRenderer,x0+x,y0-y);

        y=y+1;
        if (err <= 0){
            err += 2*y + 1;
        }
        if (err > 0){
            x -= 1;
            err -= 2*x + 1;
        }
    }
}

int main(int argc, char *argv[]){
     balle_jeu balle;
     game myGame;
     gameState state;
     font mFont;
     SDL_Rect joueur1;
     SDL_Rect joueur2;
     int scorej1;
     int scorej2;

    srand(time(NULL));

    //60fps
    unsigned int frameLimit = SDL_GetTicks() + 16;

    if(init("SDL_Pong",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN,&myGame,&mFont, &joueur1, &joueur2, &scorej1, &scorej2, &balle)){
        state.g_bRunning=1;
    }else{
        return 1;
    }

    //Création texture
    myGame.g_texture=SDL_CreateTexture(myGame.g_pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,SCREEN_WIDTH,SCREEN_HEIGHT);

    while(state.g_bRunning){

           controle(&state, &joueur1, &joueur2, &myGame);

          //mouvement balle
         if (balle.direction==1){
            balle.milieu.x=balle.milieu.x-5;
            balle.milieu.y=balle.milieu.y-5;
          }else if (balle.direction==2){
            balle.milieu.x=balle.milieu.x+5;
            balle.milieu.y=balle.milieu.y-5;
          }else if (balle.direction==3){
            balle.milieu.x=balle.milieu.x+5;
            balle.milieu.y=balle.milieu.y+5;
          }else if (balle.direction==4){
            balle.milieu.x=balle.milieu.x-5;
            balle.milieu.y=balle.milieu.y+5;
          }

          //collision raquette j1
          if ((balle.milieu.x==40+balle.rayon) && ((balle.milieu.y<=joueur1.y+joueur1.h) && (balle.milieu.y>=joueur1.y)) && (balle.direction==1)){
            balle.direction=2;
            }else if ((balle.milieu.x==40+balle.rayon) && ((balle.milieu.y<=joueur1.y+joueur1.h) && (balle.milieu.y>=joueur1.y)) && (balle.direction==4)){
                balle.direction=3;
        //collision raquette j2
          }else if ((balle.milieu.x==SCREEN_WIDTH-40-balle.rayon) && ((balle.milieu.y<=joueur2.y+joueur2.h) && (balle.milieu.y>=joueur2.y)) && (balle.direction==3)){
            balle.direction=4;
          }else if ((balle.milieu.x==SCREEN_WIDTH-40-balle.rayon) && ((balle.milieu.y<=joueur2.y+joueur2.h) && (balle.milieu.y>=joueur2.y)) && (balle.direction==2)) {
            balle.direction=1;
          }
          //collision haut
          if (balle.milieu.y==balle.rayon){
            if (balle.direction==1){
              balle.direction=4;
            }else{
              balle.direction=3;
            }
          }
        //collision bas
          if (balle.milieu.y==SCREEN_HEIGHT-balle.rayon){
            if (balle.direction==4){
              balle.direction=1;
            }else{
              balle.direction=2;
            }
          }

          // ajout points
          if (balle.milieu.x==10){
            balle.milieu.x=SCREEN_WIDTH/2;
            balle.milieu.y=SCREEN_HEIGHT/2;
            //direction de la balle pour la remise en jeu (aléatoire)
            balle.direction=rand()%4+1;
            scorej2=scorej2+1;
        }

          if (balle.milieu.x==SCREEN_WIDTH-10){
            balle.milieu.x=SCREEN_WIDTH/2;
            balle.milieu.y=SCREEN_HEIGHT/2;
            //direction de la balle pour la remise en jeu (aléatoire)
            balle.direction=rand()%4+1;
            scorej1=scorej1+1;
          }

            SDL_SetRenderDrawColor(myGame.g_pRenderer,0,0,0,255);
            SDL_RenderClear(myGame.g_pRenderer);

          affiche(&myGame,mFont,scorej1,scorej2,joueur1,joueur2,balle);

        // Gestion des 60 fps
            delay(frameLimit);
            frameLimit = SDL_GetTicks() + 16;

    }
    destroy(&myGame,&mFont);

    TTF_Quit();
    SDL_Quit();

  return 0;
}
