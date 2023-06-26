//
// Created by hamza on 6/14/23.
//

#include "map.h"
#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#ifndef PACMAN_FANTOME_H
#define PACMAN_FANTOME_H

typedef struct Fantome{
//    SDL_Rect ghost_r;
//    SDL_Rect ghost_l;
//    SDL_Rect ghost_d;
//    SDL_Rect ghost_u;
//    SDL_Rect ghost;
    int x;
} Fantome;

enum direction{
    RIGHT,
    LEFT,
    UP,
    DOWN
};

enum fantomeState{
    CHASE,
    PATROL,
    EATABLE,
    EATEN
};

//SDL_Rect chaseTargets[]={};


struct Sprite
{
    int numero;
    int VisualPositionX;
    int VisualPositionY;
    enum direction currentDirection;
    SDL_Rect *rects;
    //SDL_Rect lastRect;
    SDL_Rect ghost;
    enum fantomeState state;
};






void initFantom();
void moveFantome(char (*map)[255][166], struct Sprite *sprite);
void moveAllFantom(char (*map)[255][166],time_t elapsedTime);
void drawFantom(SDL_Surface* win_surf, SDL_Surface* plancheSprites, struct Sprite *sprite, bool isMalveillanceMax);
bool contactMur(int y,int x,char map[][166]);
void changeDirection();
void setALLFantomPositionAfterPacmanDied(char (*map)[255][166]);
enum direction* directionsDisponibles(int y, int x, char map[][166], int *nbDirections, struct Sprite *sprite);
void changementDirection(char map[][166]);
enum direction choisirDirectionAlea(enum direction *listeDirections, int nbDirections);
void touch(char dir);
void TPghots(char (*map)[255][166]);
enum direction getOppositeDirection(enum direction dir);
double calculeDistanceEntre2points(int pacmanX, int pacmanY, int sourceX, int sourceY);
enum direction trouverDistancePlusCourte(int pacmanX, int pacmanY, char map[][166], struct Sprite *sprite);
void exportSprites(SDL_Rect *srcRect, SDL_Rect *destRect, int count, int xStep, int yStep);
void drawAllFantom(SDL_Surface* win_surf, SDL_Surface* plancheSprites,bool isMalveillanceMax);
void changeFantomeState(time_t time);
void setAllFantomeState(enum fantomeState fantomeState);
bool contactWithPacman(char (*map)[255][166],bool isMalveillanceMax,int* score);
void isInHouse(struct Sprite *sprite);



void freeGhosts();





#endif //PACMAN_FANTOME_H