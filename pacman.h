//
// Created by hamza on 6/14/23.
//

#include "map.h"
#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef PACMAN_PACMAN_H
#define PACMAN_PACMAN_H

//SDL_Rect PacMan = { 32,32, 32,32 };

void movePacman(int t,char d, char (*map)[255][166],int compteur);
void drawPacman(SDL_Surface* win_surf, SDL_Surface* plancheSprites);
bool contact(int y,int x,char map[][166]);
#endif //PACMAN_PACMAN_H
