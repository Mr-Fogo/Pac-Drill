#include <SDL2/SDL_ttf.h>
#include "main.h"
#include "map.h"
#include "pacman.h"
#include "fantome.h"
#include <time.h>


SDL_Window* pWindow = NULL;
SDL_Surface* win_surf = NULL;
SDL_Surface* plancheSprites = NULL;
SDL_Surface* plancheSpritesBackup = NULL;
TTF_Font* gFont = NULL; // Police de caractères pour le texte
TTF_Font* font = NULL; // Police de caractères pour le texte

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
static Uint8* audio_pos; // global pointer to the audio buffer to be played
static Uint32 audio_len; // remaining length of the sample we have to play

char map[214][166];
static Uint32 wav_length; // length of our sample
static Uint8* wav_buffer; // buffer containing our audio file
static SDL_AudioSpec wav_spec; // the specs of our piece of music
time_t start, current, stop, lastChange;


void renderText(const char* message, int posX, int posY);
void my_audio_callback(void *userdata, Uint8 *stream, int len) {
    if (audio_len == 0)
        return;

    len = (len > audio_len ? audio_len : len);
    SDL_memcpy(stream, audio_pos, len);
    audio_pos += len;
    audio_len -= len;
}
void cleanup() {
    // Libérer les ressources
    if (gFont != NULL) {
        TTF_CloseFont(gFont);
        gFont = NULL;
    }

    if (plancheSprites != NULL) {
        SDL_FreeSurface(plancheSprites);
        plancheSprites = NULL;
    }

    if (win_surf != NULL) {
        SDL_FreeSurface(win_surf);
        win_surf = NULL;
    }

    if (pWindow != NULL) {
        SDL_DestroyWindow(pWindow);
        pWindow = NULL;
    }

    TTF_Quit();
    SDL_Quit();
    freeGhosts();
}

void drawMenu()
{
    SDL_Color textColor = { 255, 255, 255 }; // Couleur du texte (blanc)

    // Effacer la fenêtre avec une couleur de fond
    SDL_FillRect(win_surf, NULL, SDL_MapRGB(win_surf->format, 0, 0, 0));

    // Affichage du premier texte du menu
    char menuText1[] = "Appuyez sur espace pour commencer";
    SDL_Surface* menuSurface1 = TTF_RenderText_Solid(font, menuText1, textColor); // Surface du premier texte
    SDL_Rect menuPos1 = { 115, 400 }; // Position du premier texte
    SDL_BlitSurface(menuSurface1, NULL, win_surf, &menuPos1); // Affichage du premier texte
    SDL_FreeSurface(menuSurface1); // Libération de la surface du premier texte



    SDL_SetColorKey(plancheSprites, false, 0);
    SDL_BlitScaled(plancheSprites, &src_MenuPacman, win_surf, &MenuPacman);
    SDL_SetColorKey(plancheSprites, true, 0);
}

void drawEndScreen()
{
    SDL_Color textColor = { 255, 255, 255 }; // Couleur du texte (blanc)

    // Effacer la fenêtre avec une couleur de fond
    SDL_FillRect(win_surf, NULL, SDL_MapRGB(win_surf->format, 0, 0, 0));
    // Affichage du texte de fin
    char endText[50];
    sprintf(endText, "GAME OVER");

    SDL_Surface* endSurface = TTF_RenderText_Solid(gFont, endText, textColor); // Surface du texte
    SDL_Rect endPos = { 270, 400 }; // Position du texte
    SDL_BlitSurface(endSurface, NULL, win_surf, &endPos); // Affichage du texte
    SDL_FreeSurface(endSurface); // Libération de la surface du texte

    char scoreText[20]; // Texte du score
    sprintf(scoreText, "Score: %d", score);
    renderText(scoreText, 270, 430 ); // Exemple : Afficher le score à la position (10, 10)


    char levelText[20]; // Texte du score
    sprintf(levelText, "Niveau: %d", level);
    renderText(levelText, 270, 460 ); // Exemple : Afficher le score à la position (10, 10)

    char timeText[20]; // Texte du temps
    // Obtenir le temps actuel
    time_t elapsedTime = current - start; // Calculer le temps écoulé depuis le démarrage du jeu
    int minutes = (elapsedTime % 3600) / 60;
    int seconds = elapsedTime % 60;

    sprintf(timeText, "%02d:%02d",minutes, seconds);
    renderText(timeText, 270, 490 ); // Afficher le temps formaté à la position (550, 350)

    SDL_SetColorKey(plancheSprites, false, 0);
    SDL_BlitScaled(plancheSprites, &src_MenuPacman, win_surf, &MenuPacman);
    SDL_SetColorKey(plancheSprites, true, 0);
}
void init() {
    pWindow = SDL_CreateWindow("PacMan", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 664, 856, SDL_WINDOW_SHOWN);
    win_surf = SDL_GetWindowSurface(pWindow);

    plancheSprites = SDL_LoadBMP("../pacman_sprites.bmp");

    if (TTF_Init() != 0) {
        fprintf(stderr, "Erreur lors de l'initialisation de SDL_ttf : %s\n", TTF_GetError());
        exit(1);
    }

    // Chargement de la police de caractères
    font = TTF_OpenFont("PAC-FONT.ttf",
                        15); // Remplacez "chemin/vers/la/police.ttf" par le chemin vers votre police de caractères

    if (font == NULL) {
        fprintf(stderr, "Erreur lors du chargement de la police de caractères : %s\n", TTF_GetError());
        exit(1);
    }

    createmap(plancheSprites, map);
}
void renderText(const char *message, int posX, int posY) {
    SDL_Color textColor = {255, 255, 255}; // Couleur du texte (blanc dans cet exemple)
    SDL_Surface *textSurface = TTF_RenderText_Solid(gFont, message, textColor); // Rendre le texte sur une surface

    if (textSurface == NULL) {
        fprintf(stderr, "Erreur lors du rendu du texte : %s", TTF_GetError());
        return;
    }

    SDL_Rect textRect = {posX, posY, textSurface->w, textSurface->h}; // Position et taille du texte
    SDL_BlitSurface(textSurface, NULL, win_surf, &textRect); // Copier la surface du texte sur la fenêtre

    SDL_FreeSurface(textSurface); // Libérer la surface du texte
}
void draw() {
    if (showMenu) {
        drawMenu();
    } else if (gameStarted) {
        SDL_SetColorKey(plancheSprites, false, 0);
        SDL_BlitScaled(plancheSprites, &src_bg, win_surf, &bg);
        SDL_SetColorKey(plancheSprites, true, 0);

        // Afficher le texte
        char scoreText[20]; // Texte du score
        sprintf(scoreText, "Score: %d", score);
        renderText(scoreText, 10, 325); // Exemple : Afficher le score à la position (10, 10)

        char lifeText[20]; // Texte du score
        sprintf(lifeText, "Vie: %d", nbVies);
        renderText(lifeText, 10, 350); // Exemple : Afficher le score à la position (10, 10)

        char levelText[20]; // Texte du score
        sprintf(levelText, "Niveau: %d", level);
        renderText(levelText, 550, 325); // Exemple : Afficher le score à la position (10, 10)

        char timeText[20]; // Texte du temps
         // Obtenir le temps actuel
        time_t elapsedTime = current - start; // Calculer le temps écoulé depuis le démarrage du jeu
        int minutes = (elapsedTime % 3600) / 60;
        int seconds = elapsedTime % 60;

        sprintf(timeText, "%02d:%02d",minutes, seconds);
        renderText(timeText, 550, 350); // Afficher le temps formaté à la position (550, 350)
    }
    else if (gameNextLevel)
    {
        SDL_Color textColor = { 255, 255, 255 }; // Couleur du texte (blanc)

        // Effacer la fenêtre avec une couleur de fond
        SDL_FillRect(win_surf, NULL, SDL_MapRGB(win_surf->format, 0, 0, 0));
        // Affichage du texte de fin
        char nextText[70];
        sprintf(nextText, "Appuyer sur espace pour lancer le prochain niveau");
        SDL_Surface* nextSurface = TTF_RenderText_Solid(gFont, nextText, textColor); // Surface du texte
        SDL_Rect nextPos = { 100, 400 }; // Position du texte
        SDL_BlitSurface(nextSurface, NULL, win_surf, &nextPos); // Affichage du texte
        SDL_FreeSurface(nextSurface); // Libération de la surface du texte
        SDL_SetColorKey(plancheSprites, false, 0);
        SDL_BlitScaled(plancheSprites, &src_MenuPacman, win_surf, &MenuPacman);
        SDL_SetColorKey(plancheSprites, true, 0);
    }
    else if (gameEnded) {
        drawEndScreen();
    }

}
void handleTP()
{
    TPpacman(map);
    TPghots(map);
}
void PacManestMortWesh()
{
    setPacManPosition(325,478,map);
    setALLFantomPositionAfterPacmanDied(map);
    setAllFantomeState(PATROL);
    isMalveillanceMax = false;
    current = time(NULL);
    loadSong = true;
}
void NextLevel()
{
    plancheSprites = SDL_LoadBMP("../pacman_sprites.bmp");
    createmap(plancheSprites, map);
    setPacManPosition(325,478,map);
    setALLFantomPositionAfterPacmanDied(map);
    setAllFantomeState(PATROL);
    isMalveillanceMax = false;
    level++;
    loadSong = true;

}
void swapSong() {
    if(isMalveillanceMax && loadSong) {

        SDL_CloseAudio();
        SDL_FreeWAV(wav_buffer);
        SDL_LoadWAV(MUS_PATH_PacMan, &wav_spec, &wav_buffer, &wav_length);
        wav_spec.callback = my_audio_callback;
        wav_spec.userdata = NULL;
        // set our global static variables
        audio_pos = wav_buffer; // copy sound buffer
        audio_len = wav_length; // copy file length
        SDL_OpenAudio(&wav_spec, NULL);
        SDL_PauseAudio(0);

        loadSong = false;
    } else if (!isMalveillanceMax && loadSong)
    {
        SDL_CloseAudio();
        SDL_FreeWAV(wav_buffer);
        SDL_LoadWAV(MUS_PATH, &wav_spec, &wav_buffer, &wav_length);
        wav_spec.callback = my_audio_callback;
        wav_spec.userdata = NULL;
        // set our global static variables
        audio_pos = wav_buffer; // copy sound buffer
        audio_len = wav_length; // copy file length
        SDL_OpenAudio(&wav_spec, NULL);
        SDL_PauseAudio(0);

        loadSong = false;
    }
}
void ResetAll() {
    plancheSprites = SDL_LoadBMP("../pacman_sprites.bmp");
    createmap(plancheSprites, map);
    setPacManPosition(325, 478, map);
    setALLFantomPositionAfterPacmanDied(map);
    setAllFantomeState(PATROL);
    isMalveillanceMax = false;
    level = 1;
    score = 0;
    nbVies = 3;
    start = time(NULL);
    loadSong = true;
    current = time(NULL);
    gameStarted = true;
    gameEnded = false;
    gameNextLevel = false;
}
int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        fprintf(stderr, "Echec de l'initialisation de la SDL %s", SDL_GetError());
        return 1;
    }

    if (TTF_Init() != 0)
    {
        fprintf(stderr, "Echec de l'initialisation de SDL_ttf: %s", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Charger la police de caractères
    gFont = TTF_OpenFont("arial.ttf", 22);
    if (gFont == NULL)
    {
        fprintf(stderr, "Impossible de charger la police de caractères : %s", TTF_GetError());
        cleanup();
        return 1;
    }

    /* Load the WAV */
    // the specs, length and buffer of our wav are filled
    if (SDL_LoadWAV(MUS_PATH, &wav_spec, &wav_buffer, &wav_length) == NULL)
    {
        cleanup();
        return 1;
    }
    // set the callback function
    wav_spec.callback = my_audio_callback;
    wav_spec.userdata = NULL;
    // set our global static variables
    audio_pos = wav_buffer; // copy sound buffer
    audio_len = wav_length; // copy file length

    /* Open the audio device */
    if (SDL_OpenAudio(&wav_spec, NULL) < 0)
    {
        fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
        cleanup();
        return 1;
    }

    /* Start playing */
    SDL_PauseAudio(0);

    init();

    bool quit = false;
    struct nextDirection nextDirection = { 0, 0 };
    initFantom();
    setALLFantomPositionAfterPacmanDied(map);
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
                default:
                    break;
            }
        }

        int nbk;
        const Uint8* keys = SDL_GetKeyboardState(&nbk);
        if (showMenu)
        {
            if (keys[SDL_SCANCODE_SPACE])
            {
                showMenu = false;
                gameStarted = true; // La partie commence
                gameNextLevel = false;
                start = time(NULL);
            }
            draw();
            SDL_UpdateWindowSurface(pWindow);
        }
        else if(gameNextLevel)
        {
            if (keys[SDL_SCANCODE_SPACE])
            {
                showMenu = false;
                gameNextLevel = false;
                gameStarted = true; // La partie commence
            }
            draw();
            SDL_UpdateWindowSurface(pWindow);
        }
        else if (gameEnded)
        {
            gameStarted = false;
            stop = time(NULL);
            if (keys[SDL_SCANCODE_ESCAPE]) {
                quit = true;
            }
            else if (keys[SDL_SCANCODE_SPACE])
            {
                ResetAll();
            }
            draw();
            SDL_UpdateWindowSurface(pWindow);
        }
        else if (gameStarted)
        {
            current = time(NULL);

            if (keys[SDL_SCANCODE_ESCAPE])
            {
//                showMap(map);
//                NextLevel();
//                gameEnded = false;
//                gameStarted = false;
//                gameNextLevel = true;
            }
            if (keys[SDL_SCANCODE_LEFT])
            {
                nextDirection.x = -1;
                nextDirection.y = 0;
            }
            if (keys[SDL_SCANCODE_RIGHT])
            {
                nextDirection.x = 1;
                nextDirection.y = 0;
            }
            if (keys[SDL_SCANCODE_UP])
            {
                nextDirection.x = 0;
                nextDirection.y = -1;
            }
            if (keys[SDL_SCANCODE_DOWN])
            {
                nextDirection.x = 0;
                nextDirection.y = 1;
            }

            movePacmanbutBetter(vitesse, map, nextDirection.x, nextDirection.y);
            handleAnimation(compteurAnimation);
            compteurAnimation++;
            moveAllFantom(map, current-start);            changementDirection(map);
            score += contactwithdollars(map);
            isPacManDead = contactWithPacman(map,isMalveillanceMax,&score);
            handleTP();
            if(contactwithcut(map))
            {
                isMalveillanceMax = true;
                lastChange =current-start;
                setFantomeEatable();
                loadSong = true;
            }
            isMalveillanceTimer(current-start, lastChange);
            draw();
            drawPacman(win_surf, plancheSprites);
            drawAllFantom(win_surf, plancheSprites,isMalveillanceMax);

            SDL_Delay(20); // ~50 fps use SDL_GetTicks64() pour plus de precision
            SDL_UpdateWindowSurface(pWindow);
            updateMap(plancheSprites, map);
            setMapTheme(plancheSprites, map,isMalveillanceMax);
            swapSong();
            if(allDollarEat(map))
            {
                gameEnded = false;
                gameStarted = false;
                gameNextLevel = true;
                NextLevel();
            }
            if (isPacManDead) {
                if(nbVies > 0) {
                    nbVies--;
                    isPacManDead = false;
                    PacManestMortWesh();
                }
                else {
                    gameEnded = true;
                    gameStarted = false;
                }
            }
        }
    }

    cleanup();
    return 0;
}
void isMalveillanceTimer(time_t current, time_t lastChange)
{
    time_t elapsedTime = current - start;


    printf("debut %ld\n", current);
    printf("last %ld\n", lastChange);
    //printf("elapsed time %ld\n", elapsedTime);
    if(isMalveillanceMax) {
        //time_t dernierchangement=0;
        //lastChange=current;
        time_t dernierchangement=current;
        printf("dernier change %ld\n", dernierchangement);

        if (dernierchangement-lastChange >= 15) {
            printf("hello");
            isMalveillanceMax = false;
            loadSong = true;
            quitEatbleState();
        }

    }

}

