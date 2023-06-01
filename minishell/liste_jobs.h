#ifndef __LISTE_JOBS_H
#define __LISTE_JOBS_H
#include <sys/types.h>

// Definitions de types
enum Etat {ACTIF, SUSPENDU, TERMINE};

struct job {
    int id;
    pid_t pid;
    char *commande;
    enum Etat statut;
    int avantPlan; // Vaut 1 si le processus est en avant plan.
};

struct cellule {
    struct job courante;
    struct cellule  *suivante;
};

// Renommer les types précédents
typedef struct job job;
typedef enum Etat Etat;
typedef struct cellule cellule;


/**
 * Ajouter un job à la liste de jobs.
*/
void ajouter_job(cellule **liste, int nID, int npid, char *ncommande, int nstatut, int avantPlan);

/**
 * Mettre à jour le statut d'un processus fils du shell,
 * identifié par son pid.
 * pid (in) : pid du processus dont on veut mettre à jour le statut
 * nouveauStatut (in) : nouveau statut du processus
*/
void setStatut(cellule **liste, int pid, int statut);

/**
 * Afficher la liste des jobs non terminés.
 * liste (in) : liste des jobs.
*/
void afficher_job(cellule *liste);

/**
 * Otenir le job associé à un id.
 * liste (in) : liste des jobs
 * id (in) : id du job à récuperer
 * j (out): job récupéré
*/
void getJob(cellule *liste, int id, job *j);

/**
 * Obtenir le pid du processus (éventuel) en avant plan et actif
 * liste (in) : liste des jobs
 * pid (out) : pid récupéré
*/
void getAvantPlan(cellule *liste, int *pid);

#endif