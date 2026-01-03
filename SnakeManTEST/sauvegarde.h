#ifndef _SAUVEGARDE_H_
#define _SAUVEGARDE_H_

#include "piece.h"

/* Chemins et Fichiers */
#define FOND_SAVE "images/Fond_Save_Tetris.png"
#define PATH_SAVE_PREFIX "fichiers_textes/save_" /* save_1.txt, save_2.txt... */
#define EXTENSION_SAVE ".txt"

/* Affiche l'écran de choix de slot (1 à 4). Renvoie 0 si annulé. */
int selectionner_slot(float ratio_x, float ratio_y);

/* Sauvegarde dans un fihcier texte */
int sauvegarder_partie(int slot, Snake *s, Pomme *p, int score);

/* Charge depuis un fichier texte*/
int charger_partie(int slot, Snake *s, Pomme *p, int *score);

#endif