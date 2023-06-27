//
// Created by hamza on 6/14/23.
//
#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>



#ifndef PACMAN_MAIN_H
#define PACMAN_MAIN_H

void my_audio_callback(void *userdata, Uint8 *stream, int len);
void init();
void draw();
void isMalveillanceTimer();
struct nextDirection {
    int x;
    int y;
};

#endif //PACMAN_MAIN_H
