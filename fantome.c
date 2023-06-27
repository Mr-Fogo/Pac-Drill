//
// Created by hamza on 6/14/23.
//

#include "fantome.h"

struct Fantome *ghostList;

int count = 0;

SDL_Rect fantomeMangeable = {4, 196, 14, 14};
SDL_Rect fantomeMange = {72, 200, 14, 10};


int FantomVisualX;
int FantomVisualY;


void initFantom() {
    ghostList = malloc(sizeof(struct Fantome) * 4);

    for (int i = 0; i < 4; i++) {
        ghostList[i].numero = i;
        ghostList[i].rects = malloc(sizeof(SDL_Rect) * 4);
        ghostList[i].ghost = (SDL_Rect) {400, 31, 32, 32};

        SDL_Rect ghost = {4,
                          124 + i * (14 + 4),
                          14, 14};

        exportSprites(&ghost, ghostList[i].rects, 4, 2 * (14 + 3),
                      0);
        ghostList[i].state = PATROL;
    }
}

void drawAllFantom(SDL_Surface *win_surf, SDL_Surface *plancheSprites, bool isMalveillanceMax) {

    for (int i = 0; i < 4; i++) {
        drawFantom(win_surf, plancheSprites, &ghostList[i], isMalveillanceMax);
    }

}

void drawFantom(SDL_Surface *win_surf, SDL_Surface *plancheSprites, struct Fantome *sprite, bool isMalveillanceMax) {
    SDL_Rect rect = {sprite->ghost.x, sprite->ghost.y, 32, 32};

    SDL_Rect *ghost_in;
    if (sprite->state == EATABLE) {

        ghost_in = &fantomeMangeable;
    } else if (sprite->state == EATEN) {
        ghost_in = &fantomeMange;
    } else
        ghost_in = &(sprite->rects[0]);

    SDL_BlitScaled(plancheSprites, ghost_in, win_surf, &rect);

}

bool contactMur(int y, int x, char map[][166]) {
    bool contact = true;
    for (int i = -1; i <= 8; i++) {
        for (int j = -1; j <= 8; j++) {
            if (map[FantomVisualY + y + i][FantomVisualX + x + j] == 'x' ||
                map[FantomVisualY + y + i][FantomVisualX + x + j] == 'w') {
                return false;
            }

        }
    }
    return contact;
}

void moveAllFantom(char (*map)[255][166], time_t elapsedTime) {
    deleteGhostFromGrid(map);
    changeFantomeState(elapsedTime);
    for (int i = 0; i < 4; i++) {
        isInHouse(&ghostList[i]);
        moveFantome(map, &ghostList[i]);
    }
}

void moveFantome(char (*map)[255][166], struct Fantome *sprite) {
    if (sprite->currentDirection == RIGHT) {
        sprite->ghost.x += 4;
    } else if (sprite->currentDirection == LEFT) {
        sprite->ghost.x -= 4;
    } else if (sprite->currentDirection == UP) {
        sprite->ghost.y -= 4;
    } else if (sprite->currentDirection == DOWN) {
        sprite->ghost.y += 4;
    }
    (*map)[sprite->ghost.y / 4][sprite->ghost.x / 4] = 'G';
}

enum direction choisirDirectionAlea(enum direction *listeDirections, int nbDirections) {
    if (nbDirections == 0) {
        return -1;  // Pas de direction disponible
    }

    // Génération d'un indice aléatoire pour choisir une direction
    int indiceAleatoire = rand() % nbDirections;

    // Retourne la direction correspondant à l'indice aléatoire
    return listeDirections[indiceAleatoire];
}

void setALLFantomPositionAfterPacmanDied(char (*map)[255][166]) {
    deleteGhostFromGrid(map);
    for (int i = 0; i < 4; i++) {
        if (i == 0)
            ghostList[i].ghost.y = 380;
        else if (i == 1)
            ghostList[i].ghost.y = 390;
        else if (i == 2)
            ghostList[i].ghost.y = 400;
        else if (i == 3)
            ghostList[i].ghost.y = 412;

        ghostList[i].ghost.x = 319;
        ghostList[i].VisualPositionX = ghostList[i].ghost.x / 4;
        ghostList[i].VisualPositionY = ghostList[i].ghost.y / 4;
        (*map)[ghostList[i].VisualPositionY][ghostList[i].VisualPositionX] = 'G';
    }
}

void TPghots(char (*map)[255][166]) {
    int nbTPz;
    int nbTPy;
    for (int f = 0; f < 4; f++) {
        nbTPy = 0;
        nbTPz = 0;
        for (int i = -1; i <= 8; i++) {
            for (int j = -1; j <= 8; j++) {
                if ((*map)[ghostList[f].VisualPositionY + i][ghostList[f].VisualPositionX + j] == 'z') {
                    nbTPz++;
                } else if ((*map)[ghostList[f].VisualPositionY + i][ghostList[f].VisualPositionX + j] == 'y') {
                    nbTPy++;
                }
            }
        }
        if (nbTPz == 12) {
            deleteGhostFromGrid(map);
            ghostList[f].ghost.x = 34;
            ghostList[f].ghost.y = 412;
            ghostList[f].VisualPositionX = ghostList[f].ghost.x / 4;
            ghostList[f].VisualPositionY = ghostList[f].ghost.y / 4;
            (*map)[ghostList[f].VisualPositionY][ghostList[f].VisualPositionX] = 'G';
        }
        if (nbTPy == 12) {
            deleteGhostFromGrid(map);
            ghostList[f].ghost.x = 600;
            ghostList[f].ghost.y = 412;
            ghostList[f].VisualPositionX = ghostList[f].ghost.x / 4;
            ghostList[f].VisualPositionY = ghostList[f].ghost.y / 4;
            (*map)[ghostList[f].VisualPositionY][ghostList[f].VisualPositionX] = 'G';
        }
    }
}

void changementDirection(char map[][166]) {
    srand(time(NULL));
    for (int i = 0; i < 4; i++) {

        ghostList[i].VisualPositionX = ghostList[i].ghost.x / 4;
        ghostList[i].VisualPositionY = ghostList[i].ghost.y / 4;

        int nbDirections;
        enum direction *listeDirections = directionsDisponibles(0, 0, map, &nbDirections, &ghostList[i]);
        enum direction directionChoisie;
        if (ghostList[i].state == PATROL) {
            if (i == 0) {
                directionChoisie = trouverDistancePlusCourte(40, 0, map, &ghostList[i]);
            } else if (i == 1) {
                directionChoisie = trouverDistancePlusCourte(130, 165, map, &ghostList[i]);
            } else if (i == 2) {
                directionChoisie = trouverDistancePlusCourte(130, 0, map, &ghostList[i]);
            } else
                directionChoisie = trouverDistancePlusCourte(40, 166, map, &ghostList[i]);
        } else if (ghostList[i].state == CHASE) {
            if (i == 0) {
                directionChoisie = trouverDistancePlusCourte(getPacmanX(map), getPacmanY(map), map, &ghostList[i]);
            } else if (i == 1) {
                directionChoisie = trouverDistancePlusCourte(getPacmanX(map) + 11, getPacmanY(map) + 11, map,
                                                             &ghostList[i]);
            } else if (i == 2) {
                directionChoisie = trouverDistancePlusCourte(getPacmanX(map), getPacmanY(map), map, &ghostList[i]);
            } else
                directionChoisie = choisirDirectionAlea(listeDirections, nbDirections);
        } else if (ghostList[i].state == EATEN) {
            directionChoisie = trouverDistancePlusCourte(77, 79, map, &ghostList[i]);
        } else if (ghostList[i].state == EATABLE) {
            directionChoisie = choisirDirectionAlea(listeDirections, nbDirections);
        }
        ghostList[i].currentDirection = directionChoisie;

        free(listeDirections);  // Libérer la mémoire allouée pour la liste des directions
    }
}

enum direction *directionsDisponibles(int y, int x, char map[][166], int *nbDirections, struct Fantome *sprite) {
    enum direction *listeDirections = malloc(
            4 * sizeof(enum direction)); // Allouer de la mémoire pour la liste des directions
    *nbDirections = 0;  // Initialisation du nombre de directions disponibles

    // Vérification de chaque direction
    for (enum direction dir = RIGHT; dir <= DOWN; dir++) {
        int newY = 0;
        int newX = 0;

        if (dir != getOppositeDirection(sprite->currentDirection)) {
            int newY = sprite->VisualPositionY;
            int newX = sprite->VisualPositionX;
            // Mise à jour des coordonnées selon la direction
            switch (dir) {
                case RIGHT:
                    newX += 1;
                    break;
                case LEFT:
                    newX -= 1;
                    break;
                case UP:
                    newY -= 1;
                    break;
                case DOWN:
                    newY += 1;
                    break;
            }

            // Vérification de contact avec un mur
            if (contactMur(newY, newX, map)) {
                listeDirections[*nbDirections] = dir;  // Ajout de la direction à la liste
                (*nbDirections)++;  // Incrémentation du nombre de directions disponibles
            }
        }
    }

    return listeDirections;
}

enum direction getOppositeDirection(enum direction dir) {
    switch (dir) {
        case RIGHT:
            return LEFT;
        case LEFT:
            return RIGHT;
        case UP:
            return DOWN;
        case DOWN:
            return UP;
    }

    return -1;  // Valeur par défaut en cas de direction invalide
}

enum direction trouverDistancePlusCourte(int pacmanX, int pacmanY, char map[][166], struct Fantome *sprite) {
    int nbDirections;
    //enum direction currentDirection = /* la direction courante du fantôme */;
    enum direction *listeDirections = directionsDisponibles(0, 0, map, &nbDirections, sprite);

    long int distanceMin = 99999999999999;
    enum direction plusCourteDirection;

    // Parcourir les directions disponibles
    for (int i = 0; i < nbDirections; i++) {
        enum direction dir = listeDirections[i];

        // Obtenir les coordonnées du prochain pixel dans la direction choisie
        int newY = sprite->VisualPositionY;
        int newX = sprite->VisualPositionX;

        switch (dir) {
            case RIGHT:
                newX += 8;
                break;
            case LEFT:
                newX -= 1;
                break;
            case UP:
                newY -= 1;
                break;
            case DOWN:
                newY += 8;
                break;
        }

        // Calculer la distance la plus courte entre le prochain pixel et la position du Pacman
        int distance = calculeDistanceEntre2points(pacmanX, pacmanY, newX, newY);
        // Mettre à jour la distance minimale
        if (distance < distanceMin) {
            distanceMin = distance;
            plusCourteDirection = dir;
        }
    }

    free(listeDirections);  // Libérer la mémoire allouée pour la liste des directions
    return plusCourteDirection;
}

double calculeDistanceEntre2points(int pacmanX, int pacmanY, int sourceX, int sourceY) {
    int deltaX = abs(pacmanX - sourceX);
    int deltaY = abs(pacmanY - sourceY);
    double distance = sqrt(deltaX * deltaX + deltaY * deltaY);
    return distance;
}

void exportSprites(SDL_Rect *srcRect, SDL_Rect *destRect, int count, int xStep, int yStep) {
    for (int i = 0; i < count; i++) {
        destRect[i].x = srcRect->x + i * xStep;
        destRect[i].y = srcRect->y + i * yStep;
        destRect[i].w = srcRect->w;
        destRect[i].h = srcRect->h;
    }
}

void changeFantomeState(time_t timeElapsed) {

    static time_t dernierChangement = 0;

    if (ghostList[0].state == PATROL && (timeElapsed - dernierChangement >= 10)) {
        setAllFantomeState(CHASE);
        dernierChangement = timeElapsed;

    } else if (ghostList[0].state == CHASE && (timeElapsed - dernierChangement >= 20)) {
        setAllFantomeState(PATROL);
        dernierChangement = timeElapsed;
    }
}

void setAllFantomeState(enum fantomeState fantomeState) {
    for (int i = 0; i < 4; i++) {
        ghostList[i].state = fantomeState;
    }
}

bool contactWithPacman(char (*map)[255][166],bool isMalveillanceMax,int* score)
{
    bool cdead = false;
    for (int a =0; a<4; a++)
    {
        for (int i = -1; i <= 8; i++) {
            for (int j = -1; j <= 8; j++) {
                if ((*map)[ghostList[a].VisualPositionY + i][ghostList[a].VisualPositionX + j] == 'P') {
                    if (ghostList[a].state==EATABLE) {
                        ghostList[a].state = EATEN;
                        score+=100;
                    }
                    else if (ghostList[a].state==EATEN)
                    {

                    }
                    else {
                        cdead = true;
                    }
                }

            }
        }
    }
    return cdead;
}
void isInHouse(struct Fantome *sprite) {
    if (sprite->VisualPositionX == 77 && sprite->VisualPositionY == 79 && sprite->state == EATEN) {
        sprite->state = PATROL;
    }
}


void freeGhosts() {
    for (int i = 0; i < 4; i++) {
        free(ghostList[i].rects);
    }
    free(ghostList);
}

void setFantomeEatable()
{
    for (int i = 0; i < 4; i++)
    {
        if (ghostList[i].state!=EATEN)
        {
            ghostList[i].state = EATABLE;
        }
    }
}

void quitEatbleState() {
    for (int i = 0; i < 4; i++) {
        ghostList[i].state = PATROL;
    }
}


