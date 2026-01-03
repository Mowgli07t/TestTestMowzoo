#ifndef _CLASSEMENT_C_
#define _CLASSEMENT_C_

#include <stdlib.h>
#include <stdio.h>
#include <MLV/MLV_all.h>

#include "classement.h"
#include "piece.h"

void fichier_classement(char* fichier, int tab[TABMAX2], int score){
    /*Definition des variables*/
    int i=0;
    char line[NMAX];
    FILE* f;

    /*Ouverture du fichier*/
    f = fopen(fichier,"a+");

    /*Boucle qui remplis tab avec 0 si il est non defini sinon par les elements deja dans le fichier*/
    while (i<11){
        if (f == NULL){
            tab[i]= 0;
        }       
        else{
            /*Le nouveau score est mis en indice 10 du tableau*/
            if (i==10){
                tab[i]=score;
            }
            
            else{
                if (fgets(line,NMAX,f) == NULL) tab[i]=0; 
                else tab[i]=atoi(line); 
            }                           
        }
        i++; 
    }

    /*Fermeture du fichier*/
    fclose(f);
}

void tri(int tab[TABMAX2]){
    /*Definition des variables*/
    int i,j,cmp;

    /*Algorithme du tri à bulle sur les 11 scores du tableau*/
    for (i=10;i>0;i--){
        for (j=0;j<i;j++){
            if (tab[j+1]>tab[j]){
                cmp=tab[j];
                tab[j]=tab[j+1];
                tab[j+1]=cmp;
            }
        }
    }
}

void printfichier(char* fichier,int tab[TABMAX2]){
    /*Definition des variables*/
    int i=0;      
    FILE* f;

    /*Ouverture du fichier*/
    f = fopen(fichier,"w+");
    if (f != NULL){ 
        while (i<11){
            /*Ecriture des 10 premier scorees dans le fichier*/
            if (tab[i]!=0){  
                fprintf(f,"%d\n",tab[i]);
            }
            i++;   
        }
        /*Fermeture du fichier*/
        fclose(f);
    }
}

void gestion_ecran_leaderboard(float ratio_x, float ratio_y){
    /*Definition des variables*/
    int score, x, y, i, val=1;
    unsigned int w, h;
    char s_score[NMAX];
    MLV_Image* fond_leaderboard;
    MLV_Font* police;
    FILE* f;
    char line[NMAX];

    /*Design ecran leaderboard*/
    police = MLV_load_font(POLICE, 50*ratio_x);
    MLV_get_desktop_size(&w, &h);
    MLV_clear_window(MLV_COLOR_BLACK);
    fond_leaderboard = MLV_load_image(FOND_CLASSEMENT);
    MLV_resize_image_with_proportions(fond_leaderboard, w, h);
    MLV_draw_image(fond_leaderboard, 0, 0);

    /*Ouverture du fichier*/
    f = fopen(FICHIER_CLASSEMENT,"a+");

    /*Si le fichier n'existe pas, rien à afficher*/
    if (f == NULL){
        val=0;
    }
    /*Sinon afficher les 10 meilleurs scores (les seuls dans le fichier)*/
    if (val ==1){
        for(i=0;i<10;i++){
            sprintf(s_score, "%s", "");
            if (fgets(line,NMAX,f) != NULL){
                score=atoi(line);
                sprintf(s_score, "%d", score);
            }
            /*Affichage des scores selon l ecran utilise*/
            
            MLV_get_size_of_text_with_font(s_score, &x, &y, police);
            MLV_draw_text_with_font((960*ratio_x-x/2), ((100+80.5*(i+1))*ratio_y-y/2), s_score, police, MLV_COLOR_WHITE);
        }
    }
    /*Fermeture du fichier*/
    fclose(f);
    MLV_actualise_window();
    MLV_wait_mouse(&x, &y);
}
#endif /*_CLASSEMENT_C_*/