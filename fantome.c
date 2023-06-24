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

SDL_Rect ghost = { 400,31, 32,32 };
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

void moveFantome(char map[][166])
{
    //moveFantome(map);
        if (currentDirection == RIGHT) {
            //if (contactMur(0, 1, map)) {
                ghost.x+=4;
//                //isBlocked = false;
//            } else
//                isBlocked = true;
        }
        else if (currentDirection==LEFT)
        {
//            if(contactMur(0,-1,map)) {
                ghost.x-=4;
//            }
//            else
//                isBlocked=true;
        }
        else if (currentDirection==UP)
        {
//            if(contactMur(1,0,map))
                ghost.y-=4;
//            else
//                isBlocked=true;
        }
        else if (currentDirection==DOWN)
        {
//            if(contactMur(-1,0,map))
                ghost.y+=4;
//            else
//                isBlocked=true;
        }
}
enum direction choisirDirection(enum direction *listeDirections, int nbDirections)
{
    if (nbDirections == 0) {
        return -1;  // Pas de direction disponible
    }

    // Génération d'un index aléatoire pour choisir une direction
    int indexAleatoire = rand() % nbDirections;

    return listeDirections[indexAleatoire];
}

void changementDirection(char map[][166])
{
    FantomVisualX=ghost.x/4;
    FantomVisualY=ghost.y/4;

    int nbDirections;
    enum direction *listeDirections = directionsDisponibles(0, 0, map, &nbDirections);
    enum direction directionChoisie = trouverDistancePlusCourte(getPacmanX(map),getPacmanY(map),map);
    currentDirection=directionChoisie;
    // Mise à jour des coordonnées selon la direction choisie
 /*   switch (directionChoisie) {
        case RIGHT:
            ghost.x++;
            break;
        case LEFT:
            ghost.x--;
            break;
        case UP:
            ghost.y++;
            break;
        case DOWN:
            ghost.y--;
            break;
    }*/

//    printf("Directions disponibles : ");
//    for (int i = 0; i < nbDirections; i++) {
//        switch (listeDirections[i]) {
//            case RIGHT:
//                printf("RIGHT ");
//                break;
//            case LEFT:
//                printf("LEFT ");
//                break;
//            case UP:
//                printf("UP ");
//                break;
//            case DOWN:
//                printf("DOWN ");
//                break;
//        }
//    }
//    printf("\n");
//
//    printf("Direction choisi : %d", directionChoisie);

    free(listeDirections);  // Libérer la mémoire allouée pour la liste des directions
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

enum direction* directionsDisponibles(int y, int x, char map[][166], int *nbDirections) {
    enum direction *listeDirections = malloc(
            4 * sizeof(enum direction)); // Allouer de la mémoire pour la liste des directions
    *nbDirections = 0;  // Initialisation du nombre de directions disponibles

    // Vérification de chaque direction
    for (enum direction dir = RIGHT; dir <= DOWN; dir++) {
        int newY = 0;
        int newX = 0;

        if (dir != getOppositeDirection(currentDirection)) {
            int newY = y;
            int newX = x;
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

            //printf("%d - %d\n", newX, newY);

            // Vérification de contact avec un mur
            if (contactMur(newY, newX, map)) {
                //printf("right");
                //printf("\n%d\n",dir);
                listeDirections[*nbDirections] = dir;  // Ajout de la direction à la liste
                (*nbDirections)++;  // Incrémentation du nombre de directions disponibles
            }
        }
    }

    return listeDirections;
}

enum direction getOppositeDirection(enum direction dir)
{
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

void touch(char dir)
{
    if(dir=='l') {
        ghost.x--;
        currentDirection=LEFT;
    }
    else if (dir=='r') {
        ghost.x++;
        currentDirection=RIGHT;
    }
    else if (dir=='u')
    {
        ghost.y--;
        currentDirection=UP;

    }
    else if (dir=='d')
    {
        ghost.y++;
        currentDirection=DOWN;
    }

}

enum direction trouverDistancePlusCourte(int pacmanX, int pacmanY, char map[][166])
{
    int nbDirections;
    //enum direction currentDirection = /* la direction courante du fantôme */;
    enum direction *listeDirections = directionsDisponibles(0, 0,  map, &nbDirections);

    long int distanceMin = 99999999999999;
    enum direction plusCourteDirection;

    // Parcourir les directions disponibles
    for (int i = 0; i < nbDirections; i++) {
        enum direction dir = listeDirections[i];

        // Obtenir les coordonnées du prochain pixel dans la direction choisie
        int newY = FantomVisualY;
        int newX = FantomVisualX;

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
        //Point source = {.y = newY, .x = newX};
        int distance = calculeDistanceEntre2points(pacmanX, pacmanY, newX, newY);
        //printf("Distance %d = %d", dir, distance);
        // Mettre à jour la distance minimale
        if (distance < distanceMin) {
            distanceMin=distance;
            plusCourteDirection = dir;
        }
    }

    free(listeDirections);  // Libérer la mémoire allouée pour la liste des directions
//    printf("PacMan position : %d - %d \n", pacmanX,pacmanY);
//    printf("Fantome position : %d - %d \n", FantomVisualX,FantomVisualY);
    //printf("Plus courte direction : %d\n", plusCourteDirection);
    return plusCourteDirection;
}

double calculeDistanceEntre2points(int pacmanX, int pacmanY, int sourceX, int sourceY)
{
    int deltaX = abs(pacmanX - sourceX);
    int deltaY = abs (pacmanY - sourceY);
//    printf("delX = %d, delY = %d\n", deltaX, deltaY);
    double distance = sqrt(deltaX * deltaX + deltaY * deltaY);
    //printf("distance = %d \n", distance);
    return distance;
}




