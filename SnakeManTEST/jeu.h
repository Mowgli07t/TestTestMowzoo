#ifndef _JEU_H_
#define _JEU_H_

#include <stdlib.h>
#include <stdio.h>
#include <MLV/MLV_all.h>

#include "piece.h"

#define FPS 60 
#define FRAME_TIME (1.0 / FPS)
#define CLASSEMENT_FICHIER "fichiers_textes/classement.txt"

#define FOND_PAUSE "images/Fond_Pause_Tetris.png" 
#define FOND_JEU "images/quadrillage_snake.png"   
#define FOND "images/background.jpg"
#define IMG_GHOST "images/ghost.png"


#define POLICE1 "fonts/police_tetris.ttf"
#define POLICE2 "fonts/retro.ttf"
#define POLICE3 "fonts/RetroGaming.ttf"

/* fonction de mise à jour logique (déplacement + collisions) */
void update_snake_logic(Snake *s, Pomme *p, int *quitter, int *score);

/* boucle du jeu de base*/
void boucleJeu(int slot_demarrage, float ancien_temps, int score, int w, int h, float ratio_x, float ratio_y);

/* menu principal */
void menuPrincipale();

/* fin du jeu */
void fin_jeu(int score);

/* menu pause*/
int menuPause(float ratio_x, float ratio_y);


#endif