#define _POSIX_SOURCE
#define _XOPEN_SOURCE 700
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "readcmd.h"
#include "liste_jobs.h"
#include "cmd_internes.h"

#define NB_MAX_PROCESSUS 50


// initialiser une liste de jobs.
cellule *liste_job = NULL;



/** Récupère la chaîne de carectère d'une commande simple*/
char* getTexteCommande(struct cmdline *cmd) {
    int bufsize = 200;
    char* Texte = malloc(bufsize * sizeof(char));
    int i = 0;
    if (cmd == NULL) {
        cmd = readcmd();
    }
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
int executer(struct cmdline *cmd, int id) {
    pid_t pidFils = fork();
    char *texteCommande = getTexteCommande(cmd);
    ajouter_job(&liste_job, id, pidFils, texteCommande, ACTIF);
    if (pidFils == -1) {
        printf("Erreur fork\n");
        exit(1);
    }
    if (pidFils == 0) { // fils
        execvp(cmd->seq[0][0], cmd->seq[0]);
        perror("execvp");
        exit(1);
    }
    else {  // père
        if (cmd->backgrounded) {
            // Dans le cas d'une tache de fond, on passe à la suite
        }
        else {
            // Dans l'autre cas, on attend le signal SIGCHLD avant de continuer.
            pause();
        }
    }
    return EXIT_SUCCESS;
}

void handler() {
    int etat_fils;
    pid_t pid_fils;

    while ((pid_fils = waitpid(-1, &etat_fils, WNOHANG | WUNTRACED | WCONTINUED)) > 0) {
        if (WIFSTOPPED(etat_fils)) {
            setStatut(&liste_job, pid_fils, SUSPENDU);
        } else if (WIFCONTINUED(etat_fils)) {
            setStatut(&liste_job, pid_fils, ACTIF);
        } else {
            setStatut(&liste_job, pid_fils, TERMINE);
        } 
    }
}

void handler_SIGINT(int sgn) {
    printf("SIGNAL RECU : SIGINT");
}

int main() {
    int sortir = 0; // Variable à 1 si on sort de la boucle.
    int id = 1; // id des processus lanés par le minishell.
    struct cmdline * cmd;
    char cwd[1024];

    // Abonner le signal SIGCHLD à un traitant.
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(SIGCHLD, &action, NULL);

    // Abonner le signal SIGCHLD à un traitant.
    action.sa_handler = handler_SIGINT;
    sigaddset(&action.sa_mask, SIGINT);
    sigaction(SIGCHLD, &action, NULL);

    do {
        int passer = 0; // Variable à 1 si on ne lance pas de fils.
        int avant_plan = 0; // Variable à 1 si on ne lance pas de fils, et qu'on reprend une tache en avant plan.
        // Invite de commande.
        getcwd(cwd, sizeof(cwd));
        printf("%s$ ", cwd);
        // Lire la commande.
        do { 
            cmd = readcmd();
        } while (cmd == NULL); 

    
        verifier_executer_cmd_internes(cmd, liste_job, &sortir, &passer, &avant_plan);

        if (avant_plan == 1) {
            // attendre un sigal SIGCHLD avant de reprendre
            pause();
        }      

        if (sortir == 1) {
            break;
        }
        if (cmd->err == NULL && passer != 1 && avant_plan != 1) {
            if (cmd->out != NULL) { // Rediriger la sortie standard vers le fichier.
                int fd_sortie = open(cmd->out, O_WRONLY | O_TRUNC | O_CREAT, 0644);
                dup2(fd_sortie, 1);
                close(fd_sortie);
            }
            if (cmd->in != NULL) { // Rediriger l'entrée standard vers le fichier.
                int fd_entree = open(cmd->in, O_RDONLY | O_TRUNC | O_CREAT, 0644);
                dup2(fd_entree, 0);
                close(fd_entree);
            }

            executer(cmd, id);

            if (cmd->out != NULL) { // Remettre en place la sortie standard.
                int nouvelleSortieStandard = open("/dev/tty", O_WRONLY);
                dup2(nouvelleSortieStandard, 1);
                close(nouvelleSortieStandard);
                printf("nouvelle sortie standard");
            }
            if (cmd->in != NULL) { // Remettre en place l'entrée stardard.
                freopen("dec/tty", "r", stdin);
                printf("entrée standard");
            }

            id++;
        } else if (cmd->err != NULL) {
            printf("%s", cmd->err);
        }
    } while (1);
    return EXIT_SUCCESS;
}