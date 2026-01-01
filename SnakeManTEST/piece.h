#ifndef _SNAKE_STRUCT_H_
#define _SNAKE_STRUCT_H_

#include <MLV/MLV_all.h>

#define TABMAX 4  
#define NMAX 32

#define GRILLE_HAUTEUR 20
#define GRILLE_LARGEUR 20

#define SNAKE_MAX_LEN 400 
#define TAILLE_CASE 30    

/* Structure pour une coordonnée (x, y) */
typedef struct {
    int x;
    int y;
} Point;

/*Structure du Serpent*/ 
typedef struct {
    Point corps[SNAKE_MAX_LEN]; 
    int taille;                
    int dx;                     
    int dy;                              
} Snake;

/* Structure de la Pomme*/
typedef struct {
    Point pos;
} Pomme;

void creer_snake(Snake *s);
void creer_pomme(Pomme *p, Snake *s);

#endif