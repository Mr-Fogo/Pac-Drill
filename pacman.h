//
// Created by hamza on 6/14/23.
//

#include "map.h"
#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef PACMAN_PACMAN_H
#define PACMAN_PACMAN_H

void movePacman(int t,char d, char map[214][166]);
void drawPacman(SDL_Surface* win_surf, SDL_Surface* plancheSprites);

#endif //PACMAN_PACMAN_H
