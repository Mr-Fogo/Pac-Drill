//
// Created by hamza on 6/14/23.
//

#include "pacman.h"

SDL_Rect PacMan = { 25,25, 32,32 };
SDL_Rect* PacMan_in = &(PacMan);
SDL_Rect pacman_r= {21,90,14,14};
SDL_Rect pacman_l= {47,90,14,14};
SDL_Rect pacman_u= {76,90,14,14};
SDL_Rect pacman_d= {110,91,14,14};
SDL_Rect pacman_c2 = { 4,90, 14,14};
SDL_Rect pacman_fr = { 36,90, 12,14 };
SDL_Rect pacman_fl = { 61,90, 12,14};
SDL_Rect pacman_fu = { 93,92, 14,12};
SDL_Rect pacman_fd = { 127,95, 14,12 };


int PacmanVisualX;
int PacmanVisualY;

int directionsprite = 0;
int middlesprite = 0;
int compteur = 0;
bool colorofmap = false;




enum direction{
    RIGHT,
    LEFT,
    UP,
    DOWN
};

struct fantomas{

};


// TODO boolean de direction sur x ou y
// TODO Buffer de direction
// TODOs

bool moveX = false;
bool moveY = false;

//int count;

//bool contact(int y,int x)
//{
//    for (int i = 0; i < 1; i++)
//    {
//        for (int j = 0; j < 1; j++)
//        {
//            if (map[PacmanVisualY-y-j][PacmanVisualX-x-i] == 'x')
//            {
//                return false;
//            }
//
//        }
//    }
//    return true;
//}

void movePacman(int t,char d, char map[214][166])
{
    PacmanVisualX = PacMan.x/4;
    PacmanVisualY = PacMan.y/4;

    PacMan_in = &(pacman_c2);

    bool animation = true;
    for (int i = 0; i < t; i++) {
        if (d == 'l') {
            //if(contact(0,-1))
            if(map[PacmanVisualY][PacmanVisualX-1] != 'x')
            {
            PacMan.x--;
            if (compteur % 2 == 0 && animation) {
                if (directionsprite == 0) {
                    if (middlesprite == 1) {
                        PacMan_in = &(pacman_l);
                        middlesprite = 0;
                        directionsprite = 1;
                    } else {
                        PacMan_in = &(pacman_c2);
                        middlesprite = 1;
                    }
                } else {
                    if (middlesprite == 1) {
                        PacMan_in = &(pacman_l);
                        middlesprite = 0;
                        directionsprite = 0;
                    } else {
                        PacMan_in = &(pacman_fl);
                        middlesprite = 1;
                    }
                }
                animation = false;
            }
            }
        } else if (d == 'r') {
            //if(contact(0,1)) {
            if(map[PacmanVisualY][PacmanVisualX+1] != 'x'){
            PacMan.x++;
            if (compteur % 2 == 0 && animation) {
                if (directionsprite == 0) {
                    if (middlesprite == 1) {
                        PacMan_in = &(pacman_r);
                        middlesprite = 0;
                        directionsprite = 1;
                    } else {
                        PacMan_in = &(pacman_c2);
                        middlesprite = 1;
                    }
                } else {
                    if (middlesprite == 1) {
                        PacMan_in = &(pacman_r);
                        middlesprite = 0;
                        directionsprite = 0;
                    } else {
                        PacMan_in = &(pacman_fr);
                        middlesprite = 1;
                    }
                }
                animation = false;
            }
            }
        } else if (d == 'u') {
            // if(contact(-1,0)) {
            if(map[PacmanVisualY-1][PacmanVisualX] != 'x')
            {
            PacMan.y--;
            if (compteur % 2 == 0 && animation) {
                if (directionsprite == 0) {
                    if (middlesprite == 1) {
                        PacMan_in = &(pacman_u);
                        middlesprite = 0;
                        directionsprite = 1;
                    } else {
                        PacMan_in = &(pacman_c2);
                        middlesprite = 1;
                    }
                } else {
                    if (middlesprite == 1) {
                        PacMan_in = &(pacman_u);
                        middlesprite = 0;
                        directionsprite = 0;
                    } else {
                        PacMan_in = &(pacman_fu);
                        middlesprite = 1;
                    }
                }
                animation = false;
            }

            }
        } else if (d == 'd') {
            //if (contact(1,0)) {
            if(map[PacmanVisualY+1][PacmanVisualX] != 'x')
            {
            PacMan.y++;
            if (compteur % 2 == 0 && animation) {
                if (directionsprite == 0) {
                    if (middlesprite == 1) {
                        PacMan_in = &(pacman_d);
                        middlesprite = 0;
                        directionsprite = 1;
                    } else {
                        PacMan_in = &(pacman_c2);
                        middlesprite = 1;
                    }
                } else {
                    if (middlesprite == 1) {
                        PacMan_in = &(pacman_d);
                        middlesprite = 0;
                        directionsprite = 0;
                    } else {
                        PacMan_in = &(pacman_fd);
                        middlesprite = 1;
                    }
                }
                animation = false;
            }
            }

        }
    }
}

void drawPacman(SDL_Surface* win_surf, SDL_Surface* plancheSprites)
{
    SDL_Rect PacMan_in2 = *PacMan_in;
    SDL_BlitScaled(plancheSprites, &PacMan_in2, win_surf, &PacMan);
    //printf("\nPacman_x : %d , Pacman_y : %d", PacMan.x,PacMan.y);
    set_pixel(plancheSprites,PacmanVisualX+201, PacmanVisualY+4, SDL_MapRGB(plancheSprites->format, 0, 254, 0));
}

//void swapSong(char path[] )
//{
//    MUS_PATH = path;
//    SDL_CloseAudio();
//    SDL_FreeWAV(wav_buffer);
//    SDL_LoadWAV(MUS_PATH, &wav_spec, &wav_buffer, &wav_length);
//    SDL_OpenAudio(&wav_spec, NULL);
//    SDL_PauseAudio(0);
//}

