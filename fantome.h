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


struct Fantome
{
    int numero;
    int VisualPositionX;
    int VisualPositionY;
    enum direction currentDirection;
    SDL_Rect *rects;
    SDL_Rect ghost;
    enum fantomeState state;

};






void initFantom();
void moveFantome(char (*map)[255][166], struct Fantome *sprite);
void moveAllFantom(char (*map)[255][166],time_t elapsedTime);
void drawFantom(SDL_Surface* win_surf, SDL_Surface* plancheSprites, struct Fantome *sprite, bool isMalveillanceMax);
bool contactMur(int y,int x,char map[][166]);
void changeDirection();
void setALLFantomPositionAfterPacmanDied(char (*map)[255][166]);
enum direction* directionsDisponibles(int y, int x, char map[][166], int *nbDirections, struct Fantome *sprite);
void changementDirection(char map[][166]);
enum direction choisirDirectionAlea(enum direction *listeDirections, int nbDirections);
void touch(char dir);
void TPghots(char (*map)[255][166]);
enum direction getOppositeDirection(enum direction dir);
double calculeDistanceEntre2points(int pacmanX, int pacmanY, int sourceX, int sourceY);
enum direction trouverDistancePlusCourte(int pacmanX, int pacmanY, char map[][166], struct Fantome *sprite);
void exportSprites(SDL_Rect *srcRect, SDL_Rect *destRect, int count, int xStep, int yStep);
void drawAllFantom(SDL_Surface* win_surf, SDL_Surface* plancheSprites,bool isMalveillanceMax);
void changeFantomeState(time_t time);
void setAllFantomeState(enum fantomeState fantomeState);
bool contactWithPacman(char (*map)[255][166],bool isMalveillanceMax,int* score);
void isInHouse(struct Fantome *sprite);
void setFantomeEatable();
void quitEatbleState();





void freeGhosts();





#endif //PACMAN_FANTOME_H