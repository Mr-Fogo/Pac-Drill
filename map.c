//
// Created by hamza on 6/14/23.
//

#include "map.h"
#define MUS_PATH "../dua.wav"
//char map[214][166];

void createmap(SDL_Surface* plancheSprites, char map[][166])
{
    SDL_Color ColorT;
    //Ligne
    for(int i = 0 ; i < 214 ; i++)
    {
        //+ printf("\n");
        //Colonne
        for (int j = 0 ; j < 166 ; j++)
        {
            ColorT = GetPixelColor(plancheSprites,j+201,i+4);
            //printf("Pixel Color : %d/%d/%d", ColorT.r,ColorT.g,ColorT.b);
            if(ColorT.r == 32 && ColorT.g == 56 && ColorT.b == 236)
            {
                map[i][j] = 'x';
            }
            else if(ColorT.r == 0 && ColorT.g == 0 && ColorT.b == 1)
            {
                map[i][j] = 'x';
            }
            else
                map[i][j] = ' ';

            // printf("%c",map[i][j]);
        }
    }
}

void showMap(char map[][166])
{
    system("clear");
    for(int i = 0 ; i < 214 ; i++)
    {
        printf("\n");
        for (int j = 0 ; j < 166 ; j++)
        {
//            if(PacMan.x == j && PacMan.y == i)
//                printf("P");
//            else
                printf("%c",map[i][j]);
        }
    }
}

SDL_Color GetPixelColor(const SDL_Surface* pSurface, int X, int Y)
{
    // Bytes per pixel
    const Uint8 Bpp = pSurface->format->BytesPerPixel;

    /*
    Retrieve the address to a specific pixel
    pSurface->pixels	= an array containing the SDL_Surface' pixels
    pSurface->pitch		= the length of a row of pixels (in bytes)
    X and Y				= the offset on where on the image to retrieve the pixel, (0, 0) is in the upper left corner of the image
    */
    Uint8* pPixel = (Uint8*)pSurface->pixels + Y * pSurface->pitch + X * Bpp;

    Uint32 PixelData = *(Uint32*)pPixel;

    SDL_Color Color = {0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE};

    // Retrieve the RGB values of the specific pixel
    SDL_GetRGB(PixelData, pSurface->format, &Color.r, &Color.g, &Color.b);

    return Color;
}
void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    Uint32 * const target_pixel = (Uint32 *) ((Uint8 *) surface->pixels
                                              + y * surface->pitch
                                              + x * surface->format->BytesPerPixel);
    *target_pixel = pixel;
}

void setMapColor(SDL_Surface *surface,int r,int g,int b, char map[][166])
{
    for(int i = 0 ; i < 214 ; i++)
    {
        for (int j = 0 ; j < 166 ; j++)
        {
            if(map[i][j] == 'x' || map[i][j] == 'w' )
                set_pixel(surface,j+201,i+4,SDL_MapRGB(surface->format, r, g, b));

        }
    }
}
void setMapTheme(SDL_Surface* plancheSprites, bool isMalveillanceMax, char map[][166])
{
    if(!isMalveillanceMax) {
        if (MUS_PATH == "coco.wav") {
            setMapColor(plancheSprites, 255, 192, 203, map);
        } else if (MUS_PATH == "PacManSong.wav") {
            setMapColor(plancheSprites, 255, 0, 0, map);
        } else if (MUS_PATH == "dua.wav") {
            //setMapColor(plancheSprites, PacMan.x * 1 % 255, PacMan.x + 150 * 1 % 255, PacMan.x + PacMan.y * 1 % 255);
        }
    }
    else
        setMapColor(plancheSprites, 255, 0, 0,map);
}

// Supprimer PacMan de la map
void deletePacManFromGrid(char (*map)[255][166])
{
    for(int i = 0 ; i < 214 ; i++)
    {
        for (int j = 0 ; j < 166 ; j++)
        {
            if((*map)[i][j] == 'P') {
                (*map)[i][j] = ' ';
            }
        }
    }
}

int getPacmanX(char (*map)[255][166])
{
    for(int i = 0 ; i < 214 ; i++)
    {
        for (int j = 0 ; j < 166 ; j++)
        {
            if((*map)[i][j] == 'P') {
                return j;
            }
        }
    }
    return 0;
}

int getPacmanY(char (*map)[255][166])
{
    for(int i = 0 ; i < 214 ; i++)
    {
        for (int j = 0 ; j < 166 ; j++)
        {
            if((*map)[i][j] == 'P') {
                return i;
            }
        }
    }
    return 0;
}