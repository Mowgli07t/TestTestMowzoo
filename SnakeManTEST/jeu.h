#ifndef _JEU_H_
#define _JEU_H_

#include <stdlib.h>
#include <stdio.h>
#include <MLV/MLV_all.h>

#include "piece.h"

#define FPS 60 
#define FRAME_TIME (1.0 / FPS)
#define CLASSEMENT_FICHIER "fichiers_textes/classement.txt"

/* --- MODIFICATION ICI : On pointe vers les nouveaux fichiers/images --- */
#define FOND_PAUSE "images/Fond_Pause_Tetris.png" /* Ou Fond_Pause.png si renommé */
#define FOND_JEU "images/quadrillage_snake.png"   /* Ou ton fond généré */
#define FOND "images/background.jpg"
#define IMG_GHOST "images/ghost.png"

#define POLICE1 "fonts/police_tetris.ttf"
#define POLICE2 "fonts/retro.ttf"
#define POLICE3 "fonts/RetroGaming.ttf"

/* Fonction de mise à jour logique (déplacement + collisions) */
void update_snake_logic(Snake *s, Pomme *p, int *quitter, int *score);

/* --- CORRECTION ERREUR 1 : Ajout de 'int slot_demarrage' --- */
void boucleJeu(int slot_demarrage, float ancien_temps, int score, int w, int h, float ratio_x, float ratio_y);

/* Menu principal */
void menuPrincipale();

/* Fonctions utilitaires */
void fin_jeu(int score);
int menuPause(float ratio_x, float ratio_y);

/* --- SUPPRESSION : On a enlevé 'gestion_selection_sauvegarde' car inutile --- */

#endif