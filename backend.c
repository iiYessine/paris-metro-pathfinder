#include "backend.h"




listes_stations_t initialiser_map(){


listes_stations_t liste;

for (int i = 0; i < nb_stations; i++) {
    liste.tab[i] = NULL; // pas encore de station assignée
}

return liste;

}

void ajouter_station(listes_stations_t* liste, int id_csv,char* nom){

station_t* s = malloc(sizeof(station_t));
s->id = id_csv;
s->successeurs = NULL; // pas encore de successeur

liste->tab[s->id] = s;   // place la station dans le tableau
strcpy(liste->tab[s->id]->nom, nom);
}




void ajouter_ligne(listes_stations_t* liste,int depart,int dest,char* nom_ligne){

ligne_t* ligne = malloc(sizeof(ligne_t));
ligne -> dest = dest;
strncpy(ligne->nom_ligne,nom_ligne,10);
ligne->suivant = NULL;

station_t* s = liste->tab[depart];
if(s->successeurs==NULL){
	s->successeurs = ligne;
}
else{
	ligne_t* temp = s->successeurs;
	while(temp->suivant != NULL){
		temp = temp->suivant;
	}
	temp->suivant = ligne;
}

}

void initialiser_stations(FILE* file,listes_stations_t* liste){
char ligne[256];
char chaine[256];
fgets(ligne, sizeof(ligne),file);
int i = 1;
while ((fgets(ligne,256,file)!=NULL)){
	sscanf(ligne, "%255[^,\n]",chaine);
	ajouter_station(liste,i,chaine);
	i++;
}



}



void load_lignes(FILE* file,listes_stations_t* liste){

int dest,depart;
char nom_ligne[10],r[1024];

while ((fgets(r,1024,file)!=NULL)){
	if (r[0]=='O'){
		continue;
	}
	sscanf(r, "%d,%d,%9s", &depart, &dest, nom_ligne);
	ajouter_ligne(liste,depart,dest,nom_ligne);
	
}	
}



void afficher_map(listes_stations_t* liste) {
    for (int i = 0; i < 30; i++) {
        station_t* s = liste->tab[i];
        if (s == NULL) continue; // station non initialisée

        printf("Station %d :\n", s->id);

        ligne_t* l = s->successeurs;
        while (l != NULL) {
            printf("  -> Dest : %d, Nom de la ligne : %s\n", l->dest, l->nom_ligne);
            l = l->suivant;
        }
    }
}

void init_Dijkstra(int depart,int distance[],char ligne_precedente[][10],int predecesseur[],int traite[]){

	
	for (int i = 1; i <= nb_stations; i++){
		distance[i] = INT_MAX; // Cela représente l'infini
		predecesseur[i] = -1;
		strcpy(ligne_precedente[i],"?");
		traite[i] = 0;
	}
	distance[depart] = 0;


}



int trouver_min(int distance[],int traite[]){

	int mini = INT_MAX;
	int sommet = -1;
	for (int i =1;i <= nb_stations; i++){
		if (traite[i] == 0){
			if (distance[i] < mini){
				mini = distance[i];
				sommet = i;
			}			
		}
	}
	return sommet;
}

int calcul_poids(int sommet1, int sommet2, char ligne_precedente[][10], 
                 char* ligne_choix, listes_stations_t* liste) {
    station_t* stat1 = liste->tab[sommet1];
    ligne_t* l = stat1->successeurs;
    int temps_min = INT_MAX;

    while (l != NULL) {
        if (l->dest == sommet2) {  
            int temps;  
            
            if (strcmp(ligne_precedente[sommet1], "?") == 0) {
                temps = 1;  // Premier trajet
            } else if (strcmp(l->nom_ligne, ligne_precedente[sommet1]) == 0) {
                temps = 1;  // Même ligne
            } else {
                temps = 6;  // Correspondance
            }
            
            if (temps < temps_min) {  // 
                temps_min = temps;
                strcpy(ligne_choix, l->nom_ligne);
            }
        }  
        l = l->suivant;
    }
    return temps_min;
}

void maj_distances(int sommet1,int sommet2,char ligne_precedente[][10],char* ligne_choix,int distance[],int predecesseur[],listes_stations_t* liste){
	int poids = calcul_poids(sommet1,sommet2,ligne_precedente,ligne_choix,liste);
	if (distance[sommet2] > distance[sommet1] + poids){
		distance[sommet2] = distance[sommet1] + poids;
		predecesseur[sommet2] = sommet1;
		strcpy(ligne_precedente[sommet2], ligne_choix);
	}
	

}

bool tout_traite(int traite[]){
	for (int i = 1 ; i <= nb_stations ; i++){
		if (traite[i] == 0){
			return 0;
		}
	}
	return 1;
	}



resultat_t Dijkstra(int depart, int dest,listes_stations_t* liste){

	int distance[nb_stations],sommet1;
	char ligne_choix[10];
	strcpy(ligne_choix, "?");
	int traite[nb_stations];
	char ligne_precedente[nb_stations][10];
	int predecesseur[nb_stations];
	init_Dijkstra(depart,distance,ligne_precedente,predecesseur,traite);
	while(!tout_traite(traite)){
		

		sommet1 = trouver_min(distance,traite);
		if (sommet1 == -1) break;
		traite[sommet1] = 1;
		station_t* stat1 = liste->tab[sommet1];
		ligne_t* l = stat1->successeurs;
		while (l != NULL) {
			maj_distances(sommet1,l->dest,ligne_precedente,ligne_choix,distance,predecesseur,liste);
			l = l->suivant;
			
		}
		//printf("Traitement du sommet %d, distance=%d\n", sommet1, distance[sommet1]);
		
	}
	

	//afficher_chemin(depart,dest,predecesseur,ligne_precedente,liste);
	return extraire_resultat(depart, dest, predecesseur, ligne_precedente, distance[dest],liste);

}


void afficher_chemin(int depart, int dest, int predecesseur[], char ligne_precedente[][10], listes_stations_t* liste) {
    int chemin[nb_stations];
    char lignes[nb_stations][10];
    int longueur = 0;
    int s = dest;

    // Remonter le chemin depuis la destination
    while (s != depart) {
        chemin[longueur] = s;
        strcpy(lignes[longueur], ligne_precedente[s]);
        longueur++;
        s = predecesseur[s];
    }
    chemin[longueur] = depart;
    strcpy(lignes[longueur], "?"); // aucune ligne avant le départ
    longueur++;

    // Afficher dans le bon ordre (départ -> destination)
    printf("Itinéraire :\n");
    for (int i = longueur - 1; i >= 0; i--) {
        int id_station = chemin[i];
        printf("Station %d", liste->tab[id_station]->id);
        if (i < longueur - 1) {
            printf(" via ligne %s", lignes[i]);
        }
        printf("\n");
    }
}

listes_stations_t charger_metro(const char* fichier_stations, const char* fichier_aretes) {
    FILE* f_aretes = fopen(fichier_aretes, "r");
    if (!f_aretes) {
        perror("Erreur ouverture fichier aretes");
        exit(1);
    }

    FILE* f_stations = fopen(fichier_stations, "r");
    if (!f_stations) {
        perror("Erreur ouverture fichier stations");
        exit(1);
    }

    listes_stations_t liste = initialiser_map();
    initialiser_stations(f_stations, &liste);
    load_lignes(f_aretes, &liste);

    fclose(f_aretes);
    fclose(f_stations);

    return liste;
}

resultat_t extraire_resultat(int depart, int dest, int predecesseur[], char ligne_precedente[][10], int temps_total, listes_stations_t* liste) {
    resultat_t res;
    res.temps_total = temps_total;
    res.longueur = 0;
    
    int chemin[nb_stations];
    char lignes[nb_stations][10];
    int longueur = 0;
    int s = dest;

    // Remonter le chemin depuis la destination
    while (s != depart) {
        chemin[longueur] = s;
        strcpy(lignes[longueur], ligne_precedente[s]);
        longueur++;
        s = predecesseur[s];
    }
    chemin[longueur] = depart;
    strcpy(lignes[longueur], "?");
    longueur++;

    // Remplir la structure de résultat dans le bon ordre (départ -> destination)
    for (int i = longueur - 1; i >= 0; i--) {
        res.stations[res.longueur] = chemin[i];
        strncpy(res.noms_stations[res.longueur], liste->tab[chemin[i]]->nom, 127);
        res.noms_stations[res.longueur][127] = '\0';
        if (i < longueur - 1) {
            strcpy(res.lignes[res.longueur-1], lignes[i-1]);
        } else {
            strcpy(res.lignes[res.longueur], "");
        }
        res.longueur++;
    }

    return res;
}
