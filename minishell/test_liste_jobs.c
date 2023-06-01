#include "liste_jobs.h"
#include <stdlib.h>

int main(void) {
    cellule *liste = NULL;
    ajouter_job(&liste, 0, 13, "ls", 0);
    ajouter_job(&liste, 1, 132, "sleep 10 &", 0);
    afficher_job(liste);
    setStatut(&liste, 132, TERMINE);
    afficher_job(liste);
}