//
// Created by hamza on 6/14/23.
//


#include <SDL2/SDL_ttf.h>
#include "main.h"
#include "map.h"
#include "pacman.h"
#include "fantome.h"

SDL_Window* pWindow = NULL;
SDL_Surface* win_surf = NULL;
SDL_Surface* plancheSprites = NULL;
#define MUS_PATH "../dua.wav"
bool isMalveillanceMax = false;
int score = 0;
SDL_Rect src_bg = { 201,4, 166,214 }; // x,y, w,h (0,0) en haut a gauche
SDL_Rect bg = { 4,4, 664,856 }; // ici scale x4

// variables pour le son
static Uint8 *audio_pos; // global pointer to the audio buffer to be played
static Uint32 audio_len; // remaining length of the sample we have to play

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
    struct nextDirection nextDirection = {0,0};
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
            nextDirection.x = -1;
            nextDirection.y = 0;
        }
        if (keys[SDL_SCANCODE_RIGHT])
        {
            nextDirection.x = 1;
            nextDirection.y = 0;
        }
        if(keys[SDL_SCANCODE_UP]) {
            nextDirection.x = 0;
            nextDirection.y = -1;
        }
        if(keys[SDL_SCANCODE_DOWN]) {
            nextDirection.x = 0;
            nextDirection.y = 1;
        }
        movePacmanbutBetter(4,map,nextDirection.x,nextDirection.y);
        moveFantome(map);
        changementDirection(map);
        score += contactwithdollars(map);
        //changeDirection();
        draw();
        drawPacman(win_surf,plancheSprites);
        drawFantom(win_surf,plancheSprites);
        //printf("Map[0][10] : %c",map[0][10]);
        SDL_Delay(20); // ~50 fps use SDL_GetTicks64() pour plus de precision
        SDL_UpdateWindowSurface(pWindow);
        updateMap(plancheSprites,map);
        setMapTheme(plancheSprites,map);
    }

    SDL_Quit();
    return 0;
}
void init()
{
    pWindow = SDL_CreateWindow("PacMan", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 664, 856, SDL_WINDOW_SHOWN);
    win_surf = SDL_GetWindowSurface(pWindow);

    plancheSprites = SDL_LoadBMP("./pacman_sprites.bmp");
    createmap(plancheSprites, map);


}

// fonction qui met à jour la surface de la fenetre "win_surf"
void draw()
{
    SDL_SetColorKey(plancheSprites, false, 0);
    SDL_BlitScaled(plancheSprites, &src_bg, win_surf, &bg);
    SDL_SetColorKey(plancheSprites, true, 0);
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