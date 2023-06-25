//
// Created by hamza on 6/14/23.
//
#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef PACMAN_MAP_H
#define PACMAN_MAP_H

void createmap(SDL_Surface* plancheSprites, char map[][166]);
void showMap(char map[][166]);
SDL_Color GetPixelColor(const SDL_Surface* pSurface, int X, int Y);
void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
void deletePacManFromGrid(char (*map)[255][166]);
void deleteGhostFromGrid(char (*map)[255][166]);
void updateGrid();
void setMapTheme(SDL_Surface *surface,char (*map)[255][166]);
void updateMap(SDL_Surface *surface,char (*map)[255][166]);
int getPacmanX(char (*map)[255][166]);
int getPacmanY(char (*map)[255][166]);




#endif //PACMAN_MAP_H
