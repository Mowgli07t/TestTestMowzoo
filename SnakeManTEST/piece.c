#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <MLV/MLV_all.h>

#include "piece.h"



void creer_snake(Snake *s) {
    int i,start_x,start_y;
    
    s->taille = 3;       
    s->dx = 1;           
    s->dy = 0;
    
    start_x = GRILLE_LARGEUR / 2;
    start_y = GRILLE_HAUTEUR / 2;

    for (i = 0; i < s->taille; i++) {
        s->corps[i].x = start_x - i; 
        s->corps[i].y = start_y;
    }
}

void creer_pomme(Pomme *p, Snake *s) {
    int collision_pomme_serpant;
    int i;

    /* On fait une boucle tant que la pomme apparait sur une position interdite */
    do {
        collision_pomme_serpant = 0;
        
        /* on genere la pomme aleatoirement (entre 0 et taille-1) */
        p->pos.x = rand() % GRILLE_LARGEUR;
        p->pos.y = rand() % GRILLE_HAUTEUR;

        /* Vérif si position interdite */
        for (i = 0; i < s->taille; i++) {
            if (s->corps[i].x == p->pos.x && 
                s->corps[i].y == p->pos.y) { /*pomme sur serpant*/
                collision_pomme_serpant = 1; 
            }
        }
    } while (collision_pomme_serpant == 1);
}