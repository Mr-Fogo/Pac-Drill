//
// Created by hamza on 6/14/23.
//

#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

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


void initFantome();
void moveFantomeAleatoirement(char map[][166]);
void drawFantom(SDL_Surface* win_surf, SDL_Surface* plancheSprites);
bool contactMur(int y,int x,char map[][166]);
void changeDirection();
enum direction* directionsDisponibles(int y, int x, char map[][166], int *nbDirections);


#endif //PACMAN_FANTOME_H