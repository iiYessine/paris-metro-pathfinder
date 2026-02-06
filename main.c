#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include "backend.h"
#include "frontend.h"

int main() {
    /* ===== INITIALISATION GTK ===== */
    gtk_init(NULL, NULL);
    
    /* ===== INITIALISATION DU BACKEND ===== */
    
    FILE* fichier_aretes = fopen("aretes.csv", "r");
    if (!fichier_aretes) {
        perror("Erreur ouverture fichier aretes.csv");
        return 1;
    }
    
    FILE* fichier_stations = fopen("stations.csv", "r");
    if (!fichier_stations) {
        perror("Erreur ouverture fichier stations.csv");
        fclose(fichier_aretes);
        return 1;
    }
    
    // Initialisation de la map
    listes_stations_t liste = initialiser_map();
    initialiser_stations(fichier_stations, &liste);
    load_lignes(fichier_aretes, &liste);
    
    fclose(fichier_aretes);
    fclose(fichier_stations);
    
    /* ===== AFFICHAGE DES STATIONS POUR L'INTERFACE ===== */
    
    // Créer le tableau de stations pour l'interface
    GuiStation stations[nb_stations];
    int nb_stations_valides = 0;
    
    for (int i = 0; i < nb_stations; i++) {
        if (liste.tab[i] != NULL) {
            stations[nb_stations_valides].id = liste.tab[i]->id;
            strncpy(stations[nb_stations_valides].nom, liste.tab[i]->nom, 127);
            stations[nb_stations_valides].nom[127] = '\0';
            nb_stations_valides++;
        }
    }
    
    /* ===== BOUCLE DE RECHERCHE D'ITINERRAIRES ===== */
    
    while (1) {
        gui_choisir_stations(stations, nb_stations_valides, NULL, NULL);
        
        gtk_main();
        
        int depart = gui_get_depart_id();
        int arrivee = gui_get_arrivee_id();
        
        if (depart < 0 || arrivee < 0) {
            printf("Sélection annulée.\n");
            break;
        }
        
        printf("Départ: %d, Arrivée: %d\n", depart, arrivee);
        
        /* ===== CALCUL DE L'ITINÉRAIRE ET AFFICHAGE GRAPHIQUE ===== */
        
        printf("Calcul en cours...\n");
        resultat_t resultat = Dijkstra(depart, arrivee, &liste);
        
        printf("\nItinéraire calculé: %d étapes, %d minutes\n", resultat.longueur, resultat.temps_total);
        
        // Afficher la solution dans la GUI
        gui_afficher_solution(resultat);
        
        gtk_main();
        
        if (!gui_get_restart_search()) {
            break;
        }
    }
    
    return 0;
}
