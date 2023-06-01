#include "liste_jobs.h"
#include <stdlib.h>
#include <stdio.h>

void ajouter_job(cellule **liste, int nID, int npid, char *ncommande, int nstatut) {
    // Allouer une nouvelle cellule pour le nouveau job
    struct cellule *nouvelle_cellule = (struct cellule*) malloc(sizeof(struct cellule));
    nouvelle_cellule->courante.id = nID;
    nouvelle_cellule->courante.pid = npid;
    nouvelle_cellule->courante.commande = ncommande;
    nouvelle_cellule->courante.statut = nstatut;
    nouvelle_cellule->suivante = NULL;

    // Ajouter le nouveau job à la fin de la liste
    if (*liste == NULL) {
        *liste = nouvelle_cellule;
    } else {
        struct cellule *curseur = *liste;
        while (curseur->suivante != NULL) {
            curseur = curseur->suivante;
        }
        curseur->suivante = nouvelle_cellule;
    }
}

void setStatut(cellule **liste, int pid, int statut) {
    // Parcourir la liste de jobs et trouver le job avec le pid donné
    struct cellule *curseur = *liste;
    while (curseur != NULL && curseur->courante.pid != pid) {
        curseur = curseur->suivante;
    }

    // Mettre à jour le statut du job avec le nouveau statut donné
    if (curseur != NULL) {
        curseur->courante.statut = statut;
    }
}


void afficher_job(cellule *liste) {
    printf("id | pid | etat | ligne de commande \n");
    while (liste) {
        if (liste->courante.statut != TERMINE) {
            printf("%d | %d | %d | %s\n", liste->courante.id, liste->courante.pid, liste->courante.statut, liste->courante.commande);
        }
        liste = liste->suivante;
    }
}

void getJob(cellule *liste, int id, job *j) {
    // Parcourir la liste de jobs et trouver le job avec l'id donné
    struct cellule *curseur = liste;
    while (curseur != NULL && curseur->courante.id != id) {
        curseur = curseur->suivante;
    }

    *j = curseur->courante;
}