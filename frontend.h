#ifndef frontend_H
#define frontend_H

#define nb_stations 400

typedef struct {
    int id;
    char nom[128];
} GuiStation;

typedef struct {
    int stations[nb_stations];
    char noms_stations[nb_stations][128];
    char lignes[nb_stations][10];
    int longueur;
    int temps_total;
} resultat_t;

/* Afficher le dialogue de sélection des stations */
int gui_choisir_stations(GuiStation *stations, int num_stations, int *depart, int *arrivee);

/* Afficher l'itinéraire résultant */
void gui_afficher_solution(resultat_t res);

/* Getters pour gérer la boucle de recherche */
int gui_get_depart_id(void);
int gui_get_arrivee_id(void);
int gui_get_restart_search(void);

#endif
