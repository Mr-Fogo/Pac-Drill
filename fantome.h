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


struct Sprite
{
    int numero;
    int VisualPositionX;
    int VisualPositionY;
//    int gridPositionX;
//    int gridPositionY;
    enum direction currentDirection;
    SDL_Rect *rects;
    //SDL_Rect lastRect;
    SDL_Rect ghost;
    //MazeElement ghostElement;
};






void initFantom();
void moveFantome(char (*map)[255][166], struct Sprite *sprite);
void moveAllFantom(char (*map)[255][166]);
void drawFantom(SDL_Surface* win_surf, SDL_Surface* plancheSprites, struct Sprite *sprite);
bool contactMur(int y,int x,char map[][166]);
void changeDirection();
enum direction* directionsDisponibles(int y, int x, char map[][166], int *nbDirections, struct Sprite *sprite);
void changementDirection(char map[][166]);
enum direction choisirDirectionAlea(enum direction *listeDirections, int nbDirections);
void touch(char dir);
enum direction getOppositeDirection(enum direction dir);
double calculeDistanceEntre2points(int pacmanX, int pacmanY, int sourceX, int sourceY);
enum direction trouverDistancePlusCourte(int pacmanX, int pacmanY, char map[][166], struct Sprite *sprite);
void exportSprites(SDL_Rect *srcRect, SDL_Rect *destRect, int count, int xStep, int yStep);
void drawAllFantom(SDL_Surface* win_surf, SDL_Surface* plancheSprites);
void freeGhosts();





#endif //PACMAN_FANTOME_H