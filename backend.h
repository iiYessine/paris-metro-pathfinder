#ifndef BACKEND_H
#define BACKEND_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "frontend.h"

#define nb_stations 400

// ============== STRUCTURES ==============

// Une ligne est définie par son nom : 11,7bis... , sa destination : quelle station et pointe vers une autre ligne
typedef struct ligne {
    int dest;
    char nom_ligne[10];
    struct ligne* suivant;
} ligne_t;

// Une station est définie par son id et par une ligne qui mène vers une autre station
typedef struct station {
    char nom[128];
    int id;
    ligne_t* successeurs;
} station_t;

// Ce tableau comportera toutes nos stations pour faciliter l'accès aux stations et leurs voisins
typedef struct listes_stations {
    station_t* tab[nb_stations];
} listes_stations_t;



// ============== FONCTIONS ==============

// Initialisation de la map
listes_stations_t initialiser_map(void);

// Ajout d'une station
void ajouter_station(listes_stations_t* liste, int id_csv,char* nom);

// Ajout d'une ligne entre deux stations
void ajouter_ligne(listes_stations_t* liste, int depart, int dest, char* nom_ligne);

// Initialisation des stations depuis le fichier
void initialiser_stations(FILE* file, listes_stations_t* liste);

// Chargement des lignes depuis le fichier
void load_lignes(FILE* file, listes_stations_t* liste);

// Affichage de la map (debugging essentiellement)
void afficher_map(listes_stations_t* liste);

// Initialisation des tableaux pour Dijkstra
void init_Dijkstra(int depart, int distance[], char ligne_precedente[][10], int predecesseur[], int traite[]);

// Trouver le sommet non traité avec la distance minimale
int trouver_min(int distance[], int traite[]);

// Calcul du poids (temps) entre deux stations
int calcul_poids(int sommet1, int sommet2, char ligne_precedente[][10], char* ligne_choix, listes_stations_t* liste);

// Mise à jour des distances dans Dijkstra
void maj_distances(int sommet1, int sommet2, char ligne_precedente[][10], char* ligne_choix,
                   int distance[], int predecesseur[], listes_stations_t* liste);

// Vérifie si tous les sommets ont été traités
bool tout_traite(int traite[]);

// Algorithme de Dijkstra complet
resultat_t Dijkstra(int depart, int dest, listes_stations_t* liste);


// Affichage du chemin trouvé (debugging essentiellement)
void afficher_chemin(int depart, int dest, int predecesseur[], char ligne_precedente[][10], listes_stations_t* liste);

// Charge le métro depuis les fichiers CSV
listes_stations_t charger_metro(const char* fichier_stations, const char* fichier_aretes);

resultat_t extraire_resultat(int depart, int dest, int predecesseur[], char ligne_precedente[][10], int temps_total, listes_stations_t* liste) ;
#endif
