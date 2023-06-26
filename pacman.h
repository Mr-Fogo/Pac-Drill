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
void movePacmanbutBetter(int vitesse, char (*map)[255][166], int x, int y);
void movePacman(int t,char d, char (*map)[255][166],int compteur);
int contactwithdollars(char (*map)[255][166]);
void drawPacman(SDL_Surface* win_surf, SDL_Surface* plancheSprites);
bool contact(int y,int x,char (*map)[255][166]);
bool contactGhost(char (*map)[255][166],bool isMalveillanceMax);
void setPacManPosition(int x, int y, char (*map)[255][166]);
void handleAnimation(int compteurAnimation);
bool contactwithcut(char (*map)[255][166]);
void TPpacman(char (*map)[255][166]);
#endif //PACMAN_PACMAN_H
