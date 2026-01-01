#ifndef _JEU_C_
#define _JEU_C_

#include <stdlib.h>
#include <stdio.h>
#include <MLV/MLV_all.h>
#include <time.h>
#include <math.h>

#include "classement.h"
#include "sauvegarde.h"
#include "jeu.h"

#include "piece.h"



void fin_jeu(int score){
    /*Definition des variables*/
    int tab[TABMAX2];

    /*Obtention des anciens scores, triage, puis recopie dans le fichier*/
    fichier_classement(CLASSEMENT_FICHIER, tab, score);
    tri(tab);
    printfichier(CLASSEMENT_FICHIER, tab);
}

int menuPause(float ratio_x, float ratio_y){
    /*Definition des variables*/
    int x,y,quitter=0, taille_y=150*ratio_y, taille_x=510*ratio_x;
    unsigned w,h;
    MLV_Image* fond_pause;

    /*Design menu pause*/
    fond_pause = MLV_load_image(FOND_PAUSE);
    MLV_get_desktop_size(&w,&h);
    MLV_resize_image_with_proportions(fond_pause, w, h);    
    MLV_draw_image(fond_pause, 0, 0);

    /*Tant qu on ne clique pas sur un bouton*/
    while (quitter!=1){
        MLV_actualise_window();
        MLV_wait_mouse(&x, &y);
        if (x>=706*ratio_x && x<=(708*ratio_x+taille_x) && y>=200*ratio_y && y<=(200*ratio_y+taille_y)){
            /*On reprend le jeu*/
            quitter=1;
            return 2;
        }
        if (x>=706*ratio_x && x<=(708*ratio_x+taille_x) && y>=500*ratio_y && y<=(500*ratio_y+taille_y)){
            /*On sauvegarde*/
            quitter=1;
            return 1;
        }
        if (x>=706*ratio_x && x<=(708*ratio_x+taille_x) && y>=800*ratio_y && y<=(800*ratio_y+taille_y)){
            /*On quitte*/
            quitter=1;
            return 0;
        }
    }
    return 2;
}
 
void update_snake_logic(Snake *s, Pomme *p, int *quitter, int *score) {
    int i;

    /* 1. Déplacer le corps */
    for (i = s->taille - 1; i > 0; i--) {
        s->corps[i] = s->corps[i-1];
    }

    /* 2. Bouger la tête */
    s->corps[0].x += s->dx;
    s->corps[0].y += s->dy;

    /* 3. Traversée des murs */
    if (s->corps[0].x >= GRILLE_LARGEUR) s->corps[0].x = 0;
    else if (s->corps[0].x < 0) s->corps[0].x = GRILLE_LARGEUR - 1;
    
    if (s->corps[0].y >= GRILLE_HAUTEUR) s->corps[0].y = 0;
    else if (s->corps[0].y < 0) s->corps[0].y = GRILLE_HAUTEUR - 1;

    /* 4. Collision avec soi-même */
    for (i = 1; i < s->taille; i++) {
        if (s->corps[0].x == s->corps[i].x && s->corps[0].y == s->corps[i].y) {
            *quitter = 1;
        }
    }

    /* 5. Manger la pomme */
    if (s->corps[0].x == p->pos.x && s->corps[0].y == p->pos.y) {
        if (s->taille < SNAKE_MAX_LEN) {
            /* CORRECTION BUG (0,0) : On initialise la nouvelle case sur la queue actuelle */
            s->corps[s->taille] = s->corps[s->taille - 1]; 
            s->taille++;
        }
        *score += 10;
        creer_pomme(p, s);
    }
}


void boucleJeu(int slot_demarrage, float ancien_temps, int score, int w, int h, float ratio_x, float ratio_y){
    int quitter = 0;
    int compteur_vitesse = 0;
    int vitesse_jeu = 10;
    int x, y, rep_pause = 2;
    int secondes, heures, minutes;
    float taille_case = 30;
    float offset_x = 650;
    float offset_y = 240;
    int i;
    
    int direction_lock = 0; 

    int frame_anim = 0;    /* Compteur pour l'animation */

    MLV_Image* img_ghost; /* Variable pour l'image du fantôme */

    float temps_passe, temps_total = 0;
    char s_timer[NMAX];
    char s_score[NMAX];
    struct timespec debut, fin, debut_total, remaining;
    
    MLV_Font* police3;
    MLV_Image* fond_jeu;
    MLV_Music* fond;

    Snake snake;
    Pomme pomme;

    /* Initialisations */
    sprintf(s_timer, "00:00:00");
    sprintf(s_score, "0");

    fond_jeu = MLV_load_image(FOND_JEU);
    fond = MLV_load_music("musique.mp3");
    MLV_play_music(fond, 0, -1);
    police3 = MLV_load_font(POLICE3, 63*ratio_x);

    /* --- CORRECTION ICI : On utilise slot_demarrage --- */
    if (slot_demarrage > 0) {
        /* Si on demande un chargement (slot 1, 2, 3 ou 4) */
        if (charger_partie(slot_demarrage, &snake, &pomme, &score)) {
            printf("Chargement du slot %d réussi.\n", slot_demarrage);
        } else {
            /* Echec, on démarre à zéro */
            creer_snake(&snake);
            creer_pomme(&pomme, &snake);
            score = 0;
        }
    } else {
        /* Nouvelle partie (slot 0) */
        creer_snake(&snake);
        creer_pomme(&pomme, &snake);
        score = 0;
    }
    
    /* On force la vitesse fixe */
    vitesse_jeu = 5; 


    img_ghost = MLV_load_image(IMG_GHOST);
    /* On redimensionne le fantôme pour qu'il rentre pile dans une case */
    MLV_resize_image_with_proportions(img_ghost, taille_case, taille_case);
    
    creer_snake(&snake);
    creer_pomme(&pomme, &snake);

    clock_gettime(CLOCK_REALTIME, &debut_total);

    while(quitter == 0){
        clock_gettime(CLOCK_REALTIME, &debut);
        compteur_vitesse++;

        frame_anim++;
        if (frame_anim >= 20) frame_anim = 0;

        /* --- 1. AFFICHAGE --- */
        MLV_clear_window(MLV_COLOR_BLACK);
        
        /* On affiche d'abord l'image de fond (le paysage spatial/nuages) */
        MLV_resize_image_with_proportions(fond_jeu, w, h);
        MLV_draw_image(fond_jeu, 0, 0);
        
        /* Pomme ronde */
        MLV_draw_image(
            img_ghost, 
            offset_x + pomme.pos.x * taille_case, 
            offset_y + pomme.pos.y * taille_case
        );


        for(i = snake.taille - 1; i >= 0; i--) {
            int px = offset_x + snake.corps[i].x * taille_case;
            int py = offset_y + snake.corps[i].y * taille_case;
            
            if (i > 0) {
                int px_prev = offset_x + snake.corps[i-1].x * taille_case;
                int py_prev = offset_y + snake.corps[i-1].y * taille_case;
                
                int cx_prev = px_prev + taille_case / 2;
                int cy_prev = py_prev + taille_case / 2;

                int cx = px + taille_case / 2; /* Centre X de la case courante */
                int cy = py + taille_case / 2; /* Centre Y de la case courante */
           

                /* ASTUCE ANTI-TRAIT GÉANT : On vérifie si les deux segments sont voisins.
                   S'ils sont trop loin (plus d'une case d'écart), c'est qu'il y a eu une
                   traversée de mur. Dans ce cas, on ne dessine PAS le trait. */
                if (abs(px - px_prev) <= taille_case + 5 && abs(py - py_prev) <= taille_case + 5) {
                    
                    int epaisseur = taille_case / 4; /* Largeur du trait de liaison */
                    
                    /* Si alignés verticalement */
                    if (cx == cx_prev) {
                        int y_min = (cy < cy_prev) ? cy : cy_prev;
                        MLV_draw_filled_rectangle(
                            cx - epaisseur/2, y_min, 
                            epaisseur, abs(cy - cy_prev), 
                            MLV_rgba(255, 200, 0, 255) /* Même jaune que le corps */
                        );
                    } 
                    /* Si alignés horizontalement */
                    else if (cy == cy_prev) {
                        int x_min = (cx < cx_prev) ? cx : cx_prev;
                        MLV_draw_filled_rectangle(
                            x_min, cy - epaisseur/2, 
                            abs(cx - cx_prev), epaisseur, 
                            MLV_rgba(255, 200, 0, 255)
                        );
                    }
                }
            }


            /* TÊTE (PAC-MAN) */
            if (i == 0) {
                int cx = px + taille_case / 2; /* Centre X */
                int cy = py + taille_case / 2; /* Centre Y */
                int demi = taille_case / 2;

                /* 1. On dessine toujours le rond jaune complet d'abord */
                MLV_draw_filled_circle(cx, cy, demi, MLV_COLOR_YELLOW);

                /* 2. Animation de la bouche */
                if (frame_anim < 10) {
                    /* --- BOUCHE OUVERTE --- */
                    /* On dessine un TRIANGLE NOIR par dessus pour simuler l'ouverture */
                    int poly_x[3];
                    int poly_y[3];
                    
                    /* Point 1 : Le centre de la tête */
                    poly_x[0] = cx; 
                    poly_y[0] = cy;

                    /* Points 2 et 3 : Dépendent de la direction */
                    if (snake.dx == 1) { /* Vers la DROITE */
                        poly_x[1] = px + taille_case; poly_y[1] = py;               /* Coin Haut-Droit */
                        poly_x[2] = px + taille_case; poly_y[2] = py + taille_case; /* Coin Bas-Droit */
                    }
                    else if (snake.dx == -1) { /* Vers la GAUCHE */
                        poly_x[1] = px; poly_y[1] = py;               /* Coin Haut-Gauche */
                        poly_x[2] = px; poly_y[2] = py + taille_case; /* Coin Bas-Gauche */
                    }
                    else if (snake.dy == -1) { /* Vers le HAUT */
                        poly_x[1] = px;               poly_y[1] = py; /* Coin Haut-Gauche */
                        poly_x[2] = px + taille_case; poly_y[2] = py; /* Coin Haut-Droit */
                    }
                    else if (snake.dy == 1) { /* Vers le BAS */
                        poly_x[1] = px;               poly_y[1] = py + taille_case; /* Coin Bas-Gauche */
                        poly_x[2] = px + taille_case; poly_y[2] = py + taille_case; /* Coin Bas-Droit */
                    }
                    /* Cas immobile (début de partie) : on ouvre vers la droite par défaut */
                    else {
                        poly_x[1] = px + taille_case; poly_y[1] = py;
                        poly_x[2] = px + taille_case; poly_y[2] = py + taille_case;
                    }

                    /* On dessine le triangle noir pour masquer le jaune */
                    MLV_draw_filled_polygon(poly_x, poly_y, 3, MLV_COLOR_BLACK);
                    
                } else {
                    /* --- BOUCHE FERMEE --- */
                    /* Juste un trait noir pour la bouche */
                    MLV_draw_line(
                        cx, cy,
                        cx + (snake.dx * demi),
                        cy + (snake.dy * demi),
                        MLV_COLOR_BLACK
                    );
                }
            }else {
                /* On dessine des cercles légèrement plus petits que la tête */
                MLV_draw_filled_circle(
                    px + taille_case/2, py + taille_case/2, 
                    taille_case/2 - 4, /* Un peu plus petit pour le style */
                    MLV_rgba(255, 200, 0, 255)
                );
            } 
        }

        /* HUD */
        MLV_get_size_of_text_with_font(s_timer, &x, &y, police3);
        MLV_draw_text_with_font(0, 225*ratio_y, s_timer, police3, MLV_COLOR_WHITE);
        MLV_get_size_of_text_with_font(s_score, &x, &y, police3);
        MLV_draw_text_with_font(0, 225*ratio_y, s_score, police3, MLV_COLOR_WHITE);
        MLV_actualise_window();

        /* --- 2. INPUT --- */
        if (MLV_get_keyboard_state(MLV_KEYBOARD_ESCAPE) == MLV_PRESSED){
             rep_pause = menuPause(ratio_x, ratio_y);
             if (rep_pause == 0) quitter = 1;
        }
        

        /* On ajoute la condition '&& direction_lock == 0' */
        if (direction_lock == 0) {
            if ((MLV_get_keyboard_state(MLV_KEYBOARD_LEFT) == MLV_PRESSED || MLV_get_keyboard_state(MLV_KEYBOARD_q) == MLV_PRESSED) && snake.dx == 0) { 
                snake.dx = -1; snake.dy = 0; direction_lock = 1; 
            }
            else if ((MLV_get_keyboard_state(MLV_KEYBOARD_RIGHT) == MLV_PRESSED || MLV_get_keyboard_state(MLV_KEYBOARD_d) == MLV_PRESSED) && snake.dx == 0) { 
                snake.dx = 1; snake.dy = 0; direction_lock = 1; 
            }
            else if ((MLV_get_keyboard_state(MLV_KEYBOARD_UP) == MLV_PRESSED || MLV_get_keyboard_state(MLV_KEYBOARD_z) == MLV_PRESSED) && snake.dy == 0) { 
                snake.dx = 0; snake.dy = -1; direction_lock = 1; 
            }
            else if ((MLV_get_keyboard_state(MLV_KEYBOARD_DOWN) == MLV_PRESSED || MLV_get_keyboard_state(MLV_KEYBOARD_s) == MLV_PRESSED) && snake.dy == 0) { 
                snake.dx = 0; snake.dy = 1; direction_lock = 1; 
            }
        }


        /* --- 3. LOGIQUE --- */
        if (compteur_vitesse >= vitesse_jeu) {
            update_snake_logic(&snake, &pomme, &quitter, &score);
            compteur_vitesse = 0;
            direction_lock = 0;
        }

        /* --- 4. TEMPS --- */
        clock_gettime(CLOCK_REALTIME, &fin);
        temps_passe = (fin.tv_sec - debut.tv_sec) + (fin.tv_nsec - debut.tv_nsec) / 1e9;
        temps_total = (fin.tv_sec - debut_total.tv_sec) + (fin.tv_nsec - debut_total.tv_nsec) / 1e9 + ancien_temps;        
        heures = (int)(temps_total / 3600); minutes = (int)((temps_total / 60)) % 60; secondes = (int)temps_total % 60;
        sprintf(s_timer, "%02d:%02d:%02d", heures, minutes, secondes);
        sprintf(s_score, "%d", score);

        if (temps_passe < FRAME_TIME) {
            remaining.tv_sec = 0; remaining.tv_nsec = (FRAME_TIME - temps_passe) * 1e9;
            nanosleep(&remaining, NULL);
        }  
    }
    fin_jeu(score);
    MLV_free_music(fond); 
    MLV_free_font(police3); 
    MLV_free_image(fond_jeu);
    MLV_free_image(img_ghost); /* Important de libérer l'image du fantôme */
}

void menuPrincipale(){
    int x, y, score=0, quitter=0;
    int slot = 0;
    float ratio_x, ratio_y, ancien_temps=0;
    unsigned int w,h;
    MLV_Font* police; MLV_Font* police2; MLV_Image* image;
    
    MLV_get_desktop_size(&w, &h);
    ratio_x = w / (float)1920; ratio_y = h / (float)1080;
    MLV_create_window("Snake", "Snake", w, h);
    image = MLV_load_image(FOND);
    MLV_resize_image_with_proportions(image, w, h);        

    while (quitter != 1){
        MLV_draw_image(image, 0, 0);
        police = MLV_load_font(POLICE1, 100*ratio_x);
        police2 = MLV_load_font(POLICE2, 33*ratio_x);
    
        MLV_draw_text_with_font((1920/3)*ratio_x, 1080/10*ratio_y, "SNAKE", police, MLV_COLOR_GREEN);
    
        /* Bouton Quitter */
        MLV_draw_filled_circle(810*ratio_x, 790*ratio_y,  80*ratio_y/2, MLV_COLOR_BLACK);
        MLV_draw_filled_circle(1110*ratio_x, 790*ratio_y,  80*ratio_y/2, MLV_COLOR_BLACK);
        MLV_draw_text_box_with_font(810*ratio_x, 750*ratio_y, 300*ratio_x, 80*ratio_y, "QUITTER", police2, 0, MLV_COLOR_BLACK, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER );

        /* Bouton Top */
        MLV_draw_filled_circle(810*ratio_x, 640*ratio_y,  80*ratio_y/2, MLV_COLOR_GOLD);
        MLV_draw_filled_circle(1110*ratio_x, 640*ratio_y,  80*ratio_y/2, MLV_COLOR_GOLD);
        MLV_draw_text_box_with_font(810*ratio_x, 600*ratio_y, 300*ratio_x, 80*ratio_y, "TOP", police2, 0, MLV_COLOR_GOLD, MLV_COLOR_WHITE, MLV_COLOR_GOLD, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER );

        /* Bouton Charger (Grisé) */
        MLV_draw_filled_circle(810*ratio_x, 490*ratio_y,  80*ratio_y/2, MLV_COLOR_GREY);
        MLV_draw_filled_circle(1110*ratio_x, 490*ratio_y,  80*ratio_y/2, MLV_COLOR_GREY);
        MLV_draw_text_box_with_font(810*ratio_x, 450*ratio_y, 300*ratio_x, 80*ratio_y, "CHARGER", police2, 0, MLV_COLOR_GREY, MLV_COLOR_WHITE, MLV_COLOR_GREY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER );

        /* Bouton Start */
        MLV_draw_filled_circle(810*ratio_x, 340*ratio_y,  80*ratio_y/2, MLV_COLOR_GREEN2);
        MLV_draw_filled_circle(1110*ratio_x, 340*ratio_y,  80*ratio_y/2, MLV_COLOR_GREEN2);
        MLV_draw_text_box_with_font(810*ratio_x, 300*ratio_y, 300*ratio_x, 80*ratio_y, "START", police2, 0, MLV_COLOR_GREEN2, MLV_COLOR_WHITE, MLV_COLOR_GREEN2, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER );

        MLV_actualise_window();
        MLV_wait_mouse(&x, &y);

        /* Clic CHARGER (Bouton du milieu) */
        if ((sqrt((x-810*ratio_x)*(x-810*ratio_x) + (y-490*ratio_y)*(y-490*ratio_y))<=80*ratio_y/2 || sqrt((x-1110*ratio_x)*(x-1110*ratio_x) + (y-490*ratio_y)*(y-490*ratio_y))<=80*ratio_y/2 || (x>=810*ratio_x && x<=1110*ratio_x && y>=450*ratio_y && y<=530*ratio_y))){
            
            /* 1. On ouvre le menu de choix du slot */
            slot = selectionner_slot(ratio_x, ratio_y);
            
            /* 2. Si le joueur a choisi un slot (1, 2, 3 ou 4) */
            if (slot > 0) {
                /* On lance le jeu en mode chargement */
                boucleJeu(slot, ancien_temps, score, w, h, ratio_x, ratio_y);
            }
        }

        if ((sqrt((x-810*ratio_x)*(x-810*ratio_x) + (y-790*ratio_y)*(y-790*ratio_y))<=80*ratio_y/2 || sqrt((x-1110*ratio_x)*(x-1110*ratio_x) + (y-790*ratio_y)*(y-790*ratio_y))<=80*ratio_y/2 || (x>=810*ratio_x && x<=1110*ratio_x && y>=750*ratio_y && y<=830*ratio_y))) quitter=1;
        
        if ((sqrt((x-810*ratio_x)*(x-810*ratio_x) + (y-640*ratio_y)*(y-640*ratio_y))<=80*ratio_y/2 || sqrt((x-1110*ratio_x)*(x-1110*ratio_x) + (y-640*ratio_y)*(y-640*ratio_y))<=80*ratio_y/2 || (x>=810*ratio_x && x<=1110*ratio_x && y>=600*ratio_y && y<=680*ratio_y))) gestion_ecran_leaderboard(ratio_x, ratio_y);

        /* Clic Start (Pas de paramètre 'charger') */
        if ((sqrt((x-810*ratio_x)*(x-810*ratio_x) + (y-340*ratio_y)*(y-340*ratio_y))<=80*ratio_y/2 || sqrt((x-1110*ratio_x)*(x-1110*ratio_x) + (y-340*ratio_y)*(y-340*ratio_y))<=80*ratio_y/2 || (x>=810*ratio_x && x<=1110*ratio_x && y>=300*ratio_y && y<=380*ratio_y)))
            boucleJeu(slot,ancien_temps, score, w, h, ratio_x, ratio_y);



        MLV_free_font(police); MLV_free_font(police2);

        
    }
    MLV_free_image(image); MLV_free_window();
}

#endif /*_JEU_C_*/