#include "cmd_internes.h"
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>


/**
 * Verifier si l'on doit éxecuter la commande exit, et le faire si nécessaire
 * cmd (in) : commande
 * sortir(out) : prend la valeur 1 si l'on a éxecuté la commande exit
*/
static int verifier_executer_exit(struct cmdline *cmd, int *sortir) {
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
        if (!cmd->seq[0][1]) {
            chdir(getenv("HOME"));
        }
        else {
            if (chdir(cmd->seq[0][1]) != 0) {
                printf("cd : usage : cd <path>");
            }
        }
    }
    return EXIT_SUCCESS;
}

/**
 * Verifier si l'on doit éxecuter la commande lj, et le faire si nécessaire
 * cmd (in) : commande
 * passer(out) : prend la valeur 1 si l'on a éxecuté la commande lj
*/
int verifier_executer_lj(struct cmdline *cmd, cellule *liste_job, int *passer) {
    char chaineLJ[30];
    strcpy(chaineLJ, "lj");
    if (!strcmp(cmd->seq[0][0], chaineLJ)) {
            *passer = 1;
            afficher_job(liste_job);
    }
    return EXIT_SUCCESS;
}

/**
 * Verifier si l'on doit éxecuter la commande sj, et le faire si nécessaire
 * cmd (in) : commande
 * passer(out) : prend la valeur 1 si l'on a éxecuté la commande sj
*/
int verifier_executer_sj(struct cmdline *cmd, cellule* liste_job, int *passer) {
    char chaineSJ[30];
    strcpy(chaineSJ, "sj");
    if (!strcmp(cmd->seq[0][0], chaineSJ)) {
            *passer = 1;
            int id = atoi(cmd->seq[0][1]);
            job j;
            getJob(liste_job, id, &j);
            kill(j.pid, SIGSTOP);
    }
    return EXIT_SUCCESS;
}

/**
 * Verifier si l'on doit éxecuter la commande bg, et le faire si nécessaire
 * cmd (in) : commande
 * passer(out) : prend la valeur 1 si l'on a éxecuté la commande bg
*/
int verifier_executer_bg(struct cmdline *cmd, cellule* liste_job, int *passer) {
    char chaineBG[30];
    strcpy(chaineBG, "bg");
    if (!strcmp(cmd->seq[0][0], chaineBG)) {
            *passer = 1;
            int id = atoi(cmd->seq[0][1]);
            job jobFG;
            getJob(liste_job, id, &jobFG);

            if (jobFG.statut != SUSPENDU) {
                perror("Le processus identifié doit être suspendu");
            } else {
                setStatut(&liste_job, jobFG.pid, ACTIF);
                kill(jobFG.pid, SIGCONT);
            }
    }
    return EXIT_SUCCESS;
}

/**
 * Verifier si l'on doit éxecuter la commande fg, et le faire si nécessaire
 * cmd (in) : commande
 * avant_plan(out) : prend la valeur 1 si l'on a éxecuté la commande bg
*/
int verifier_executer_fg(struct cmdline *cmd, cellule* liste_job, int *avant_plan) {
    char chaineFG[30];
    strcpy(chaineFG, "bg");
    if (!strcmp(cmd->seq[0][0], chaineFG)) {
            *avant_plan = 1;
            int id = atoi(cmd->seq[0][1]);
            job jobFG;
            getJob(liste_job, id, &jobFG);

            if (jobFG.statut != SUSPENDU) {
                perror("Le processus identifié doit être suspendu");
            } else {
                setStatut(&liste_job, jobFG.pid, ACTIF);
                kill(jobFG.pid, SIGCONT);
            }
    }
    return EXIT_SUCCESS;
}

void verifier_executer_cmd_internes(struct cmdline *cmd, cellule* liste_job, int *sortir, int *passer, int *avant_plan){
    verifier_executer_exit(cmd, sortir);
    verifier_executer_cd(cmd, passer);
    verifier_executer_lj(cmd, liste_job, passer);
    verifier_executer_sj(cmd, liste_job, passer);
    verifier_executer_bg(cmd, liste_job, passer);
    //verifier_executer_fg(cmd, liste_job, avant_plan);
    char chaineFG[30];
    strcpy(chaineFG, "fg");
    if (!strcmp(cmd->seq[0][0], chaineFG)) {
            *avant_plan = 1;
            int id = atoi(cmd->seq[0][1]);
            job jobFG;
            getJob(liste_job, id, &jobFG);

            if (jobFG.statut != SUSPENDU) {
                printf("Le processus identifié doit être suspendu");
            } else {
                setStatut(&liste_job, jobFG.pid, ACTIF);
                kill(jobFG.pid, SIGCONT);
            }
    }
}

