//
// Created by hamza on 6/14/23.
//

#include "main.h"
#include "map.h"
#include "pacman.h"
#include "fantome.h"

SDL_Window* pWindow = NULL;
SDL_Surface* win_surf = NULL;
SDL_Surface* plancheSprites = NULL;
#define MUS_PATH "PacManSong.wav"
bool isMalveillanceMax = false;

SDL_Rect src_bg = { 201,4, 166,214 }; // x,y, w,h (0,0) en haut a gauche
SDL_Rect bg = { 4,4, 664,856 }; // ici scale x4

SDL_Rect ghost_r = { 3,123, 16,16 };
SDL_Rect ghost_l = { 37,123, 16,16 };
SDL_Rect ghost_d = { 105,123, 16,16 };
SDL_Rect ghost_u = { 71,123, 16,16 };
SDL_Rect ghost = { 34,34, 32,32 };

// variables pour le son
static Uint8 *audio_pos; // global pointer to the audio buffer to be played
static Uint32 audio_len; // remaining length of the sample we have to play


int count;
char map[214][166];

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
        setMapTheme(plancheSprites, isMalveillanceMax, map);
        int nbk;

        const Uint8* keys = SDL_GetKeyboardState(&nbk);
//        if (keys[SDL_SCANCODE_SPACE])
//        {
//            if(isMalveillanceMax)
//                isMalveillanceMax = false;
//            else
//                isMalveillanceMax = true;
//            // swapSong("coco.wav");
//
//        }
        if (keys[SDL_SCANCODE_ESCAPE])
        {
            showMap(map);
        }
        if (keys[SDL_SCANCODE_LEFT]) {
            //movePixelTest(-1,0);
            movePacman(4,'l',map);
        }
        if (keys[SDL_SCANCODE_RIGHT])
        {
            //movePixelTest(1,0);
            movePacman(4,'r',map);
        }
        if(keys[SDL_SCANCODE_UP]) {
            //movePixelTest(0,-1);
            movePacman(4,'u',map);
        }
        if(keys[SDL_SCANCODE_DOWN]) {
            // movePixelTest(0,1);
            movePacman(4,'d',map);
        }
        draw();
        drawPacman(win_surf,plancheSprites);
        //printf("Map[0][10] : %c",map[0][10]);
        //compteur++;
        SDL_Delay(20); // ~50 fps use SDL_GetTicks64() pour plus de precision
        SDL_UpdateWindowSurface(pWindow);
    }

    SDL_Quit();
    return 0;
}
void init()
{
    pWindow = SDL_CreateWindow("PacMan", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 664, 856, SDL_WINDOW_SHOWN);
    win_surf = SDL_GetWindowSurface(pWindow);

    plancheSprites = SDL_LoadBMP("./pacman_sprites.bmp");
    count = 0;
    createmap(plancheSprites, map);

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

    count =(count+1)%(512);

    SDL_Rect ghost_in2 = *ghost_in;
    if ((count/4)%2)
        ghost_in2.x += 17;

    SDL_SetColorKey(plancheSprites, true, 0);
    SDL_BlitScaled(plancheSprites, &ghost_in2, win_surf, &ghost);

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