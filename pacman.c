//
// Created by hamza on 6/14/23.
//

#include "pacman.h"

SDL_Rect PacMan = {325, 478, 32, 32};
SDL_Rect *PacMan_in = &(PacMan);
SDL_Rect pacman_r = {21, 90, 14, 14};
SDL_Rect pacman_l = {47, 90, 14, 14};
SDL_Rect pacman_u = {76, 90, 14, 14};
SDL_Rect pacman_d = {110, 91, 14, 14};
SDL_Rect pacman_c = {4, 90, 14, 14};
SDL_Rect pacman_fr = {36, 90, 12, 14};
SDL_Rect pacman_fl = {61, 90, 12, 14};
SDL_Rect pacman_fu = {93, 92, 14, 12};
SDL_Rect pacman_fd = {127, 95, 14, 12};
SDL_Rect pacman_nextAnimation = {0, 0, 0, 0};
SDL_Rect pacman_nextAnimationfull = {0, 0, 0, 0};


struct Coordonees {
    int x;
    int y;
};
int PacmanVisualX;
int PacmanVisualY;
int x_actuel;
int y_actuel;
int directionsprite = 0;
int middlesprite = 0;
int compteur = 0;
bool animation = false;


bool contactwithwall(int y, int x, char (*map)[255][166]) {
    bool contact = true;
    for (int i = -1; i <= 8; i++) {
        for (int j = -1; j <= 8; j++) {
            if ((*map)[PacmanVisualY + y + i][PacmanVisualX + x + j] == 'x' ||
                (*map)[PacmanVisualY + y + i][PacmanVisualX + x + j] == 'w') {
                return false;
            }

        }
    }
    return contact;
}

bool contactwithcut(char (*map)[255][166]) {
    struct Coordonees dollards[9];
    int nbcut = 0;
    for (int i = -1; i <= 8; i++) {
        for (int j = -1; j <= 8; j++) {
            if ((*map)[PacmanVisualY + i][PacmanVisualX + j] == 'c') {
                dollards[nbcut].x = PacmanVisualX + j;
                dollards[nbcut].y = PacmanVisualY + i;
                nbcut++;
            }

        }
    }
    if (nbcut == 9) {
        for (int i = 0; i < 9; i++) {
            (*map)[dollards[i].y][dollards[i].x] = ' ';
        }
        return true;
    } else {
        return false;
    }
}
bool contactGhost(char (*map)[255][166],bool isMalveillanceMax)
{
    bool cdead = false;
    for (int i = -1; i <= 8; i++) {
        for (int j = -1; j <= 8; j++) {
            if ((*map)[PacmanVisualY + i][PacmanVisualX + j] == 'G')
            {
                if(isMalveillanceMax)
                {
                    cdead = false;
                }
                else
                {
                    cdead = true;
                }
            }

        }
    }
    return cdead;
}



void handleAnimation(int compteurAnimation) {
    if (x_actuel == 0 && y_actuel == 1) {
        pacman_nextAnimation = pacman_d;
        pacman_nextAnimationfull = pacman_fd;
    } else if (x_actuel == 0 && y_actuel == -1) {
        pacman_nextAnimation = pacman_u;
        pacman_nextAnimationfull = pacman_fu;
    } else if (x_actuel == 1 && y_actuel == 0) {
        pacman_nextAnimation = pacman_r;
        pacman_nextAnimationfull = pacman_fr;
    } else if (x_actuel == -1 && y_actuel == 0) {
        pacman_nextAnimation = pacman_l;
        pacman_nextAnimationfull = pacman_fl;
    }

    if (compteurAnimation % 2 == 0 && animation) {
        if (directionsprite == 0) {
            if (middlesprite == 1) {
                PacMan_in = &(pacman_nextAnimationfull);
                middlesprite = 0;
                directionsprite = 1;
            } else {
                PacMan_in = &(pacman_c);
                middlesprite = 1;
            }
        } else {
            if (middlesprite == 1) {
                PacMan_in = &(pacman_c);
                middlesprite = 0;
                directionsprite = 0;
            } else {
                PacMan_in = &(pacman_nextAnimation);
                middlesprite = 1;
            }
        }
    }
}


int contactwithdollars(char (*map)[255][166]) {
    struct Coordonees dollards[4];
    int nbdollars = 0;
    for (int i = -1; i <= 8; i++) {
        for (int j = -1; j <= 8; j++) {
            if ((*map)[PacmanVisualY + i][PacmanVisualX + j] == 'd') {
                dollards[nbdollars].x = PacmanVisualX + j;
                dollards[nbdollars].y = PacmanVisualY + i;
                nbdollars++;
            }

        }
    }
    if (nbdollars == 4) {
        for (int i = 0; i < 4; i++) {
            (*map)[dollards[i].y][dollards[i].x] = ' ';
        }
        return 10;
    } else {
        return 0;
    }
}


void movePacmanbutBetter(int vitesse, char (*map)[255][166], int x, int y) {
    PacmanVisualX = PacMan.x / 4;
    PacmanVisualY = PacMan.y / 4;
    PacMan_in = &(pacman_c);
    animation = true;
    for (int i = 0; i < vitesse; i++) {
        if (contactwithwall(y, x, map)) {
            PacMan.x += x;
            PacMan.y += y;
            deletePacManFromGrid(map);
            (*map)[PacmanVisualY + y][PacmanVisualX + x] = 'P';
            x_actuel = x;
            y_actuel = y;

        } else if (contactwithwall(y_actuel, x_actuel, map)) {
            PacMan.x += x_actuel;
            PacMan.y += y_actuel;
            deletePacManFromGrid(map);
            (*map)[PacmanVisualY + y_actuel][PacmanVisualX + x_actuel] = 'P';
        }
        else
        {
            animation = false;
        }
    }
    compteur++;

}

void setPacManPosition(int x, int y, char (*map)[255][166]) {
    PacMan.x = x;
    PacMan.y = y;
    PacmanVisualX = PacMan.x / 4;
    PacmanVisualY = PacMan.y / 4;
    deletePacManFromGrid(map);
    (*map)[PacmanVisualY][PacmanVisualX] = 'P';
}

void drawPacman(SDL_Surface *win_surf, SDL_Surface *plancheSprites) {
    SDL_Rect PacMan_in2 = *PacMan_in;
    SDL_BlitScaled(plancheSprites, &PacMan_in2, win_surf, &PacMan);
    //printf("\nPacman_x : %d , Pacman_y : %d", PacMan.x,PacMan.y);
    //set_pixel(plancheSprites, PacmanVisualX + 201, PacmanVisualY + 4, SDL_MapRGB(plancheSprites->format, 0, 254, 0));
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

