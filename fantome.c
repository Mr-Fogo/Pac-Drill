//
// Created by hamza on 6/14/23.
//

#include "fantome.h"
//#include "pacman.h"


//Fantome clyde;
//Fantome blinky;
//Fantome pinky;
//Fantome inky;

int count=0;

SDL_Rect ghost_r = { 3,123, 16,16 };
SDL_Rect ghost_l = { 37,123, 16,16 };
SDL_Rect ghost_d = { 105,123, 16,16 };
SDL_Rect ghost_u = { 71,123, 16,16 };

SDL_Rect ghost = { 31,31, 32,32 };
bool isBlocked=false;
//void initFantome()
//{
//    SDL_Rect clyde.ghost_r =  {3,123, 16,16};
//    clyde.ghost_l = { 37,123, 16,16 };
//    clyde.ghost_d = { 105,123, 16,16 };
//    clyde.ghost_u = { 71,123, 16,16 };
//    clyde.ghost = { 3,123, 16,16 };
//}

int FantomVisualX;
int FantomVisualY;

enum direction currentDirection;

void drawFantom(SDL_Surface* win_surf, SDL_Surface* plancheSprites)
{
    SDL_Rect* ghost_in = NULL;
    ghost_in = &(ghost_r);
//    switch (count/128)
//    {
//        case 0:
//            ghost_in = &(ghost_r);
//            ghost.x++;
//            break;
//        case 1:
//            ghost_in = &(ghost_d);
//            ghost.y++;
//            break;
//        case 2:
//            ghost_in = &(ghost_l);
//            ghost.x--;
//            break;
//        case 3:
//            ghost_in = &(ghost_u);
//            ghost.y--;
//            break;
//    }
//
//    count =(count+1)%(512);
//
SDL_Rect ghost_in2 = *ghost_in;
//    if ((count/4)%2)
//        ghost_in2.x += 17;

    SDL_BlitScaled(plancheSprites, &ghost_in2, win_surf, &ghost);

}

bool contactMur(int y,int x,char map[][166])
{
    bool contact = true;
    for (int i = -1; i <= 8; i++)
    {
        for (int j = -1; j <= 8; j++)
        {
            if (map[FantomVisualY+y+i][FantomVisualX+x+j] == 'x' || map[FantomVisualY+y+i][FantomVisualX+x+j] == 'w' )
            {
                return false;
            }

        }
    }
    return contact;
}

void moveFantomeAleatoirement(char map[][166])
{
    changeDirection();
    FantomVisualX=ghost.x/4;
    FantomVisualY=ghost.y/4;

    //currentDirection=changeDirection();

    if (currentDirection==RIGHT)
    {
        if(contactMur(0,1,map)) {
            ghost.x++;
            //isBlocked = false;
        }
        else
            isBlocked=true;
    }
    else if (currentDirection==LEFT)
    {
        if(contactMur(0,-1,map)) {
            ghost.x--;
        }
        else
            isBlocked=true;
    }
    else if (currentDirection==UP)
    {
        if(contactMur(1,0,map))
            ghost.y++;
        else
            isBlocked=true;
    }
    else if (currentDirection==DOWN)
    {
        if(contactMur(-1,0,map))
            ghost.y--;
        else
            isBlocked=true;
    }
}

void changeDirection()
{
    srand(time(NULL));
    // enum direction newDirection;
    while (isBlocked) {

        // Initialization, should only be called once.
        currentDirection = rand() % 4;
        isBlocked=false;
    }

}

void findPath()
{

}

