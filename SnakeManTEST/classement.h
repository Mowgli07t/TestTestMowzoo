#ifndef _CLASSEMENT_H_
#define _CLASSEMENT_H_

#include <stdlib.h>
#include <stdio.h>
#include <MLV/MLV_all.h>

#define TABMAX2 15
#define POLICE "fonts/RetroGaming.ttf"
#define FOND_CLASSEMENT "images/Fond_Leadboard_Tetris.png"
#define FICHIER_CLASSEMENT "fichiers_textes/classement.txt"

void fichier_classement(char* fichier, int tab[TABMAX2], int score);

void tri(int tab[TABMAX2]);

void printfichier(char* fichier,int tab[TABMAX2]);

void gestion_ecran_leaderboard(float ratio_x, float ratio_y);
    
#endif /*_CLASSEMENT_H_*/