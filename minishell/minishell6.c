#define _POSIX_SOURCE
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include "readcmd.h"


#define NB_MAX_PROCESSUS 50

// Definitions de types
struct job {
    pid_t pid;
    char *commande;
    int statut;
};

enum Etat {ACTIF, SUSPENDU, TERMINE};

typedef struct job job;
typedef enum Etat Etat;

job tab_job[NB_MAX_PROCESSUS];
int dernier_id = 0;

/**
 * Ajouter un job au tableau de jobs
*/
void ajouter_job(int npid, char *ncommande, int nstatut) {
    job nouveau_job = {npid, ncommande, nstatut};
    dernier_id++;
    tab_job[dernier_id] = nouveau_job;
}

/**
 * Mettre à jour le statut d'un processus fils du shell,
 * identifié par son pid.
 * pid (in) : pid du processus dont on veut mettre à jour le statut
 * nouveauStatut (in) : nouveua statut du processus
*/
void setStatut(int pid, int statut) {
    for (int i = 0 ; i < dernier_id ; i++) {
        if (tab_job[i].pid == pid) {
            tab_job[i].statut = statut;
        }
    }
}

/**
 * Verifier si l'on doit éxecuter la commande exit, et le faire si nécessaire
 * cmd (in) : commande
 * sortir(out) : prend la valeur 1 si l'on a éxecuté la commande exit
*/
int verifier_executer_exit(struct cmdline *cmd, int *sortir) {
    char chaineExit[30];
    strcpy(chaineExit, "exit");
    if (!strcmp(cmd->seq[0][0], chaineExit)) {
            *sortir = 1;
    }
    return EXIT_SUCCESS;
}

/**
 * Verifier si l'on doit éxecuter la commande cd, et le faire si nécessaire
 * cmd (in) : commande
 * passer(out) : prend la valeur 1 si l'on a éxecuté la commande cd
*/
int verifier_executer_cd(struct cmdline *cmd, int *passer) {
    char chaineCD[30];
    strcpy(chaineCD, "cd");
    if (!strcmp(cmd->seq[0][0], chaineCD)) {
        *passer = 1;
        if (chdir(cmd->seq[0][1]) != 0) {
            perror("cd : usage : cd <path>");
        }
    }
    return EXIT_SUCCESS;
}

/**
 * Verifier si l'on doit éxecuter la commande lj, et le faire si nécessaire
 * cmd (in) : commande
 * passer(out) : prend la valeur 1 si l'on a éxecuté la commande lj
*/
int verifier_executer_lj(struct cmdline *cmd, int *passer) {
    char chaineLJ[30];
    strcpy(chaineLJ, "lj");
    if (!strcmp(cmd->seq[0][0], chaineLJ)) {
            *passer = 1;
            printf("id | pid | etat | ligne de commande \n");
            //Afficher les jobs
            int i = 0;
            while (tab_job[i].commande != NULL) {
                printf("%d | %d | %d | %s \n", i, tab_job[i].pid, tab_job[i].statut, tab_job[i].commande);
            }
    }
    return EXIT_SUCCESS;
}

/**
 * Verifier si l'on doit éxecuter la commande sj, et le faire si nécessaire
 * cmd (in) : commande
 * passer(out) : prend la valeur 1 si l'on a éxecuté la commande sj
*/
int verifier_executer_sj(struct cmdline *cmd, int *passer) {
    char chaineSJ[30];
    strcpy(chaineSJ, "sj");
    if (!strcmp(cmd->seq[0][0], chaineSJ)) {
            *passer = 1;
            setStatut(atoi(cmd->seq[0][1]), SUSPENDU);
    }
    return EXIT_SUCCESS;
}

/**
 * Verifier si l'on doit éxecuter la commande bg, et le faire si nécessaire
 * cmd (in) : commande
 * passer(out) : prend la valeur 1 si l'on a éxecuté la commande bg
*/
int verifier_executer_bg(struct cmdline *cmd, int *passer) {
    char chaineBG[30];
    strcpy(chaineBG, "bg");
    if (!strcmp(cmd->seq[0][0], chaineBG)) {
            *passer = 1;
            int id = atoi(cmd->seq[0][1]);

            if (tab_job[id].statut != SUSPENDU) {
                perror("Le processus identifié doit être suspendu");
            } else {
                setStatut(tab_job[id].pid, ACTIF);
                kill(tab_job[id].pid, SIGCONT);
            }
    }
    return EXIT_SUCCESS;
}


/** Récupère la chaîne de carectère d'une commande simple*/
char* getTexteCommande(struct cmdline *cmd) {
    int bufsize = 200;
    char* Texte = malloc(bufsize * sizeof(char));
    int i = 0;
    while (cmd->seq[0][i] != NULL) {
        strcat(Texte, cmd->seq[0][i]);
        strcat(Texte, " ");  // Ajouter un espace entre chaque argument
        i++;
    }
    return Texte;
}

/**
 * Executer une commande
 * cmd (in) : commande
*/
int executer(struct cmdline *cmd) {
    pid_t pidFils = fork();
    if (pidFils == -1) {
        printf("Erreur fork\n");
        exit(1);
    }
    if (pidFils == 0) { // fils
        int echec = execvp(cmd->seq[0][0], cmd->seq[0]);
        if (echec) {
            exit(EXIT_FAILURE);
        }
        else {
            exit(EXIT_SUCCESS); 
        }
    }
    else {  // père
        ajouter_job(getpid(), getTexteCommande(cmd), ACTIF);
        int etat_fils;
        pid_t pid_fils = (int) waitpid(-1, &etat_fils, WUNTRACED);
        setStatut(pid_fils, etat_fils);
    }
    return EXIT_SUCCESS;
}

void handler(int sgn) {
    int etat_fils;
    pid_t pid_fils = (int) waitpid(-1, &etat_fils, WUNTRACED);
    setStatut(pid_fils, etat_fils);
}

int main() {
    int sortir = 0; // Variable à 1 si on sort de la boucle.
   
    struct cmdline * cmd;
    char cwd[1024];

    // Abonner le signal SIGCHLD à un traitant.
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(SIGCHLD, &action, NULL);

    do {
        int passer = 0; // Variable à 1 si on ne lance pas de fils.
        // Invite de commande.
        getcwd(cwd, sizeof(cwd));
        printf("%s$ ", cwd);
        // Lire la commande.
        cmd = readcmd();

        verifier_executer_exit(cmd, &sortir);
        verifier_executer_cd(cmd, &passer);
        verifier_executer_lj(cmd, &passer);
        verifier_executer_sj(cmd, &passer);
        

        if (sortir == 1) {
            break;
        }
        if (cmd->err == NULL && passer != 1) {
            executer(cmd);
        } else if (cmd->err != NULL) {
            printf("%s", cmd->err);
        }
    } while (1);
    return EXIT_SUCCESS;
}