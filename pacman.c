#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

SDL_Window* pWindow = NULL;
SDL_Surface* win_surf = NULL;
SDL_Surface* plancheSprites = NULL;
#define MUS_PATH "dua.wav"

bool isMalveillanceMax = false;

void my_audio_callback(void *userdata, Uint8 *stream, int len);

// variable declarations
static Uint8 *audio_pos; // global pointer to the audio buffer to be played
static Uint32 audio_len; // remaining length of the sample we have to play





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


int PacmanVisualX;
int PacmanVisualY;

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

struct fantomas{

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
bool contact(int y,int x)
{
    for (int i = 0; i < 1; i++)
    {
        for (int j = 0; j < 1; j++)
        {
            if (map[PacmanVisualY-y-j][PacmanVisualX-x-i] == 'x')
            {
                return false;
            }

        }
    }
    return true;
}
void movePacman(int t,char d)
{
    bool animation = true;
    for (int i = 0; i < t; i++)
    {
        if(d == 'l')
        {
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
            //if (contact(1,0)) {
            if(map[PacmanVisualY+1][PacmanVisualX] != 'x')
            {
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
    //Ligne
   for(int i = 0 ; i < 214 ; i++)
   {
      //+ printf("\n");
      //Colonne
       for (int j = 0 ; j < 166 ; j++)
       {
           ColorT = GetPixelColor(plancheSprites,j+201,i+4);
           //printf("Pixel Color : %d/%d/%d", ColorT.r,ColorT.g,ColorT.b);
           if(ColorT.r == 32 && ColorT.g == 56 && ColorT.b == 236) {
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

void setMapTheme()
{
    if(!isMalveillanceMax) {
        if (MUS_PATH == "coco.wav") {
            setMapColor(plancheSprites, 255, 192, 203);
        } else if (MUS_PATH == "PacManSong.wav") {
            setMapColor(plancheSprites, 255, 0, 0);
        } else if (MUS_PATH == "dua.wav") {
            setMapColor(plancheSprites, PacMan.x * 1 % 255, PacMan.x + 150 * 1 % 255, PacMan.x + PacMan.y * 1 % 255);
        }
    }
    else
        setMapColor(plancheSprites, 255, 0, 0);
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
int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0 )
    {
		fprintf(stderr,"Echec de l'initialisation de la SDL %s",SDL_GetError());
       	return 1;
    }
// local variables
    static Uint32 wav_length; // length of our sample
    static Uint8 *wav_buffer; // buffer containing our audio file
    static SDL_AudioSpec wav_spec; // the specs of our piece of music


    /* Load the WAV */
    // the specs, length and buffer of our wav are filled
    if( SDL_LoadWAV(MUS_PATH, &wav_spec, &wav_buffer, &wav_length) == NULL ){
        return 1;
    }
    // set the callback function
    wav_spec.callback = my_audio_callback;
    wav_spec.userdata = NULL;
    // set our global static variables
    audio_pos = wav_buffer; // copy sound buffer
    audio_len = wav_length; // copy file length

    /* Open the audio device */
    if ( SDL_OpenAudio(&wav_spec, NULL) < 0 ){
        fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
        exit(-1);
    }

    /* Start playing */
    SDL_PauseAudio(0);

    // wait until we're don't playing
//    while ( audio_len > 0 ) {
//        SDL_Delay(100);
//    }

    // shut everything down
    //SDL_CloseAudio();
    //SDL_FreeWAV(wav_buffer);

	init();
    PacMan.x = 25;
    PacMan.y = 25;

    PacMan_in = &(pacman_c);

	bool quit = false;
   // setMapTheme();
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
        setMapTheme();
        int nbk;
        PacmanVisualX = PacMan.x/4;
        PacmanVisualY = PacMan.y/4;
        const Uint8* keys = SDL_GetKeyboardState(&nbk);
        if (keys[SDL_SCANCODE_SPACE])
        {
            if(isMalveillanceMax)
                isMalveillanceMax = false;
            else
                isMalveillanceMax = true;
           // swapSong("coco.wav");

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

void my_audio_callback(void *userdata, Uint8 *stream, int len) {

    if (audio_len ==0)
        return;

    len = ( len > audio_len ? audio_len : len );
    SDL_memcpy (stream, audio_pos, len);                     // simply copy from one buffer into the other
    //SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);// mix from one buffer into another

    audio_pos += len;
    audio_len -= len;
}