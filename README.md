# Paris Metro Pathfinder

A shortest path calculator for the Paris Metro system using Dijkstra's algorithm, built in C with a GTK3 graphical interface.

## Description

This project finds the optimal route between two metro stations, taking into account:
- Travel time between stations (1 minute per station)
- Line transfer penalties (5 minutes per transfer)

The metro network is modeled as a graph using an adjacency list structure.

## Features

- Calculates the shortest path between any two stations
- Considers line transfers in the travel time
- User-friendly GTK3 graphical interface
- Displays step-by-step itinerary with line information

## Data Structures

```c
typedef struct ligne {
    int dest;
    char nom_ligne[10];
    struct ligne* suivant;
} ligne_t;

typedef struct station {
    char nom[128];
    int id;
    ligne_t* successeurs;
} station_t;

typedef struct listes_stations {
    station_t* tab[nb_stations];
} listes_stations_t;
```
## Requirements

- GCC compiler
- GTK3 library

## Installation

### Install GTK3 on Ubuntu/Debian

```bash
sudo apt-get install libgtk-3-dev
```
## Compile and run
make run
