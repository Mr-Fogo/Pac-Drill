#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

SDL_Window* pWindow = NULL;
SDL_Surface* win_surf = NULL;
SDL_Surface* plancheSprites = NULL;


SDL_Rect PacMan = { 34,34, 32,32 };
SDL_Rect* PacMan_in = &(PacMan);
SDL_Rect pacman_r= {21,90,14,14};
SDL_Rect pacman_l= {47,90,14,14};
SDL_Rect pacman_u= {76,90,14,14};
SDL_Rect pacman_d= {110,91,14,14};
SDL_Rect pacman_c = { 4,90, 14,14};
SDL_Rect pacman_fr = { 36,90, 12,14 };
SDL_Rect pacman_fl = { 61,90, 12,14};
SDL_Rect pacman_fu = { 93,92, 14,12};
SDL_Rect pacman_fd = { 127,95, 14,12 };

int directionsprite = 0;
int middlesprite = 0;
int compteur = 0;
bool colorofmap = false;


SDL_Rect src_bg = { 201,4, 166,214 }; // x,y, w,h (0,0) en haut a gauche
SDL_Rect bg = { 4,4, 664,856 }; // ici scale x4

SDL_Rect ghost_r = { 3,123, 16,16 };
SDL_Rect ghost_l = { 37,123, 16,16 };
SDL_Rect ghost_d = { 105,123, 16,16 };
SDL_Rect ghost_u = { 71,123, 16,16 };
SDL_Rect ghost = { 34,34, 32,32 };

enum direction{
    RIGHT,
    LEFT,
    UP,
    DOWN
};


// TODO boolean de direction sur x ou y
// TODO Buffer de direction
// TODOs

bool moveX = false;
bool moveY = false;

int x_testPixel = 14;
int y_testPixel = 14;

int count;
char map[214][166];

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
void setMapColor(SDL_Surface *surface,int r,int g,int b)
{
    for(int i = 0 ; i < 214 ; i++)
    {
        for (int j = 0 ; j < 166 ; j++)
        {
            if(map[i][j] == 'x')
                set_pixel(surface,j+201,i+4,SDL_MapRGB(surface->format, r, g, b));

        }
    }
}
void showMap()
{
    system("clear");
    for(int i = 0 ; i < 214 ; i++)
    {
        printf("\n");
        for (int j = 0 ; j < 166 ; j++)
        {
            if(PacMan.x == j && PacMan.y == i)
                printf("P");
            else
            printf("%c",map[i][j]);
        }
    }
}
void movePixelTest(int x,int y)
{
    if(map[y_testPixel + y][x_testPixel + x] != 'x')
    {
        set_pixel(plancheSprites,x_testPixel+201,y_testPixel+4,SDL_MapRGB(plancheSprites->format, 0, 0, 0));
        x_testPixel = x_testPixel + x;
        y_testPixel = y_testPixel + y;
        set_pixel(plancheSprites,x_testPixel+201,y_testPixel+4,SDL_MapRGB(plancheSprites->format, 250, 250, 250));

    }
}
void movePacman(int t,char d)
{
    bool animation = true;
    for (int i = 0; i < t; i++)
    {
        if(d == 'l')
        {
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
                            PacMan_in = &(pacman_c);
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
        }
        else if(d == 'r')
        {
            if(map[PacmanVisualY][PacmanVisualX+1] != 'x') {
                PacMan.x++;

                if (compteur % 2 == 0 && animation) {
                    if (directionsprite == 0) {
                        if (middlesprite == 1) {
                            PacMan_in = &(pacman_r);
                            middlesprite = 0;
                            directionsprite = 1;
                        } else {
                            PacMan_in = &(pacman_c);
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
        }
        else if (d == 'u')
        {
            if(map[PacmanVisualY-1][PacmanVisualX] != 'x') {
                PacMan.y--;
                if (compteur % 2 == 0 && animation) {
                    if (directionsprite == 0) {
                        if (middlesprite == 1) {
                            PacMan_in = &(pacman_u);
                            middlesprite = 0;
                            directionsprite = 1;
                        } else {
                            PacMan_in = &(pacman_c);
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
        }
        else if (d == 'd')
        {
            if (map[PacmanVisualY+1][PacmanVisualX] != 'x') {
                PacMan.y++;
                if (compteur % 2 == 0 && animation ) {
                    if (directionsprite == 0) {
                        if (middlesprite == 1) {
                            PacMan_in = &(pacman_d);
                            middlesprite = 0;
                            directionsprite = 1;
                        } else {
                            PacMan_in = &(pacman_c);
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
void createmap()
{
    SDL_Color ColorT;
   for(int i = 0 ; i < 214 ; i++)
   {
      //+ printf("\n");
       for (int j = 0 ; j < 166 ; j++)
       {
           ColorT = GetPixelColor(plancheSprites,j+201,i+4);
           //printf("Pixel Color : %d/%d/%d", ColorT.r,ColorT.g,ColorT.b);
           if(ColorT.r == 32 && ColorT.g == 56 && ColorT.b == 236)
           {
               map[i][j] = 'x';
           }
           else
               map[i][j] = ' ';

           // printf("%c",map[i][j]);
       }
   }
}

void init()
{
	pWindow = SDL_CreateWindow("PacMan", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 664, 856, SDL_WINDOW_SHOWN);
	win_surf = SDL_GetWindowSurface(pWindow);

	plancheSprites = SDL_LoadBMP("./pacman_sprites.bmp");
    count = 0;
    createmap();

}

// fonction qui met à jour la surface de la fenetre "win_surf"
void draw()
{
    SDL_SetColorKey(plancheSprites, false, 0);
    SDL_BlitScaled(plancheSprites, &src_bg, win_surf, &bg);

    SDL_Rect* ghost_in = NULL;
    ghost_in = &(ghost_r);
    switch (count/128)
    {
        case 0:
            ghost_in = &(ghost_r);
            ghost.x++;
            break;
        case 1:
            ghost_in = &(ghost_d);
            ghost.y++;
            break;
        case 2:
            ghost_in = &(ghost_l);
            ghost.x--;
            break;
        case 3:
            ghost_in = &(ghost_u);
            ghost.y--;
            break;
    }

    SDL_Rect PacMan_in2 = *PacMan_in;
    SDL_BlitScaled(plancheSprites, &PacMan_in2, win_surf, &PacMan);
    count =(count+1)%(512);

    SDL_Rect ghost_in2 = *ghost_in;
    if ((count/4)%2)
        ghost_in2.x += 17;
        
    SDL_SetColorKey(plancheSprites, true, 0);
	SDL_BlitScaled(plancheSprites, &ghost_in2, win_surf, &ghost);

}



int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
		fprintf(stderr,"Echec de l'initialisation de la SDL %s",SDL_GetError());
       	return 1;
    }

	init();


	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		while (!quit && SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			default: break;
			}
		}
        int nbk;

        const Uint8* keys = SDL_GetKeyboardState(&nbk);
        if (keys[SDL_SCANCODE_SPACE])
        {
            if(colorofmap) {
                setMapColor(plancheSprites, 254, 0, 0);
                colorofmap = false;
            }
            else
            {
                setMapColor(plancheSprites, 32, 56, 236);
                colorofmap = true;
            }
        }
        if (keys[SDL_SCANCODE_ESCAPE])
        {
            showMap();
        }
        if (keys[SDL_SCANCODE_LEFT]) {
            //movePixelTest(-1,0);
             movePacman(4,'l');
        }
        if (keys[SDL_SCANCODE_RIGHT])
        {
            //movePixelTest(1,0);
            movePacman(4,'r');
        }
        if(keys[SDL_SCANCODE_UP]) {
            //movePixelTest(0,-1);
            movePacman(4,'u');
        }
        if(keys[SDL_SCANCODE_DOWN]) {
           // movePixelTest(0,1);
            movePacman(4,'d');
        }
		draw();
        // printf("Pacman_x : %d , Pacman_y : %d", PacMan.x,PacMan.y);
        //printf("Map[34][59] : %c",map[59][34]);
        set_pixel(plancheSprites,PacmanVisualX+201, PacmanVisualY+4, SDL_MapRGB(plancheSprites->format, 0, 254, 0));
        compteur++;
        SDL_Delay(20); // ~50 fps use SDL_GetTicks64() pour plus de precision
		SDL_UpdateWindowSurface(pWindow);
	}

    SDL_Quit();
    return 0;
}
