//
// Created by hamza on 6/14/23.
//
#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL2/SDL_ttf.h>
#include "map.h"
#include "pacman.h"
#include "fantome.h"
#include <time.h>




#ifndef PACMAN_MAIN_H
#define PACMAN_MAIN_H


SDL_Window *pWindow = NULL;
SDL_Surface *win_surf = NULL;
SDL_Surface *plancheSprites = NULL;
SDL_Surface *plancheSpritesBackup = NULL;
TTF_Font *gFont = NULL; // Police de caractères pour le texte
TTF_Font *font = NULL; // Police de caractères pour le texte

bool showMenu = true;
bool gameStarted = false;
bool gameEnded = false;
bool gameNextLevel = false;
#define MUS_PATH "../dua.wav"
#define MUS_PATH_PacMan "../PacManSong.wav"
bool isMalveillanceMax = false;
bool isPacManDead = false;
bool loadSong = false;
int score = 0;
int nbVies = 3;
int compteurAnimation = 0;
int vitesse = 4;
int level = 1;
SDL_Rect src_bg = {201, 4, 166, 214}; // x,y, w,h (0,0) en haut a gauche
SDL_Rect bg = {4, 4, 664, 856}; // ici scale x4
SDL_Rect src_MenuPacman = {0, 0, 190, 50};
SDL_Rect MenuPacman = {0, 0, 720, 200};
// variables pour le son
static Uint8 *audio_pos; // global pointer to the audio buffer to be played
static Uint32 audio_len; // remaining length of the sample we have to play

char map[214][166];
static Uint32 wav_length; // length of our sample
static Uint8 *wav_buffer; // buffer containing our audio file
static SDL_AudioSpec wav_spec; // the specs of our piece of music
time_t start, current, stop, lastChange;


void my_audio_callback(void *userdata, Uint8 *stream, int len);
void init();
void draw();
void renderText(const char *message, int posX, int posY);
void isMalveillanceTimer(time_t current, time_t lastchange);
struct nextDirection {
    int x;
    int y;
};

#endif //PACMAN_MAIN_H
