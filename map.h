//
// Created by hamza on 6/14/23.
//
#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef PACMAN_MAP_H
#define PACMAN_MAP_H

void createmap(SDL_Surface* plancheSprites, char map[214][166]);
void showMap(char map[214][166]);
SDL_Color GetPixelColor(const SDL_Surface* pSurface, int X, int Y);
void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
void setMapColor(SDL_Surface *surface,int r,int g,int b,char map[214][166]);
void setMapTheme(SDL_Surface* plancheSprites, bool isMalveillanceMax,char map[214][166]);


#endif //PACMAN_MAP_H
