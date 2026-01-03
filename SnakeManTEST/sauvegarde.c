#ifndef _SAUVEGARDE_C_
#define _SAUVEGARDE_C_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <MLV/MLV_all.h>
#include "sauvegarde.h"

/* Génère le nom : fichiers_textes/save_1.txt */
void generer_nom_fichier(int slot, char *buffer) {
    sprintf(buffer, "%s%d%s", PATH_SAVE_PREFIX, slot, EXTENSION_SAVE);
}

int sauvegarder_partie(int slot, Snake *s, Pomme *p, int score) {
    char nom_fichier[256];
    FILE *f;
    int i;
    
    generer_nom_fichier(slot, nom_fichier);
    f = fopen(nom_fichier, "w"); /* "w" pour écriture texte */
    
    if (f != NULL) {
        /*On écrit le score */
        fprintf(f, "%d\n", score);
        
        /*On écrit la pos de la pomme (x y) */
        fprintf(f, "%d %d\n", p->pos.x, p->pos.y);
        
        /*On écrit la direction du serpent (dx dy) */
        fprintf(f, "%d %d\n", s->dx, s->dy);

        /*On écrit la taille puis tout le corps */
        fprintf(f, "%d\n", s->taille);
        for(i = 0; i < s->taille; i++) {
            fprintf(f, "%d %d\n", s->corps[i].x, s->corps[i].y);
        }
        
        fclose(f);
        printf("Partie sauvegardee (Slot %d) !\n", slot);
        return 1;
    }
    return 0;
}

int charger_partie(int slot, Snake *s, Pomme *p, int *score) {
    char nom_fichier[256];
    FILE *f;
    int i, taille_lue;
    
    generer_nom_fichier(slot, nom_fichier);
    f = fopen(nom_fichier, "r"); /* "r" pour lecture texte */
    
    if (f != NULL) {
        /* Lecture dans le même ordre */
        if(fscanf(f, "%d", score) != 1) return 0;
        
        if(fscanf(f, "%d %d", &p->pos.x, &p->pos.y) != 2) return 0;
       
        if(fscanf(f, "%d %d", &s->dx, &s->dy) != 2) return 0;
        
        if(fscanf(f, "%d", &taille_lue) != 1) return 0;
        s->taille = taille_lue;
        
        for(i = 0; i < s->taille; i++) {
            /* On vérifie que fscanf a bien lu 2 nombres */
            if (fscanf(f, "%d %d", &s->corps[i].x, &s->corps[i].y) != 2) {
                printf("Erreur : Fichier de sauvegarde corrompu (segment %d)\n", i);
                fclose(f);
                return 0; /* On arrête tout et on signale l'erreur */
            }
        }  

        fclose(f);
        printf("Partie chargee (Slot %d) !\n", slot);
        return 1;
    }
    printf("Pas de sauvegarde au slot %d\n", slot);
    return 0;
}

int selectionner_slot(float ratio_x, float ratio_y) {
    int x, y, slot = 0;
    int taille = 283 * ratio_x;
    int quitter = 0;
    MLV_Image *fond;
    
    fond = MLV_load_image(FOND_SAVE);
    if (!fond) {
        fprintf(stderr, "Erreur image: %s\n", FOND_SAVE);
        return 0;
    }
    MLV_resize_image_with_proportions(fond, 1920*ratio_x, 1080*ratio_y);
    
    MLV_clear_window(MLV_COLOR_BLACK);
    MLV_draw_image(fond, 0, 0);
    MLV_actualise_window();
    
    while (quitter == 0) {
        MLV_wait_mouse(&x, &y);
        
        /* Clic sur les 4 carrés */
        if (x >= 400*ratio_x && x <= (400*ratio_x + taille)) {
            if (y >= 200*ratio_y && y <= (200*ratio_y + taille)) { slot = 1; quitter = 1; }
            else if (y >= 650*ratio_y && y <= (650*ratio_y + taille)) { slot = 3; quitter = 1; }
        }
        else if (x >= 1220*ratio_x && x <= (1220*ratio_x + taille)) {
            if (y >= 200*ratio_y && y <= (200*ratio_y + taille)) { slot = 2; quitter = 1; }
            else if (y >= 650*ratio_y && y <= (650*ratio_y + taille)) { slot = 4; quitter = 1; }
        }
        /* Zone invisible au centre pour retourner au jeu */
        else if (x > 800*ratio_x && x < 1100*ratio_x && y > 400*ratio_y && y < 600*ratio_y) {
            quitter = 1; slot = 0;
        }
    }
    
    MLV_free_image(fond);
    return slot;
}


#endif /*_SAUVEGARDE_C_*/