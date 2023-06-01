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
 * Executer une commande basique (sans pipeline)
 * cmd (in) : commande
 * id (in) : id de la commande à effectuer
 * p (in) : numéro de la commande dans la struture cmd : cmd->seq[p]
*/
int executer(struct cmdline *cmd, int id, int p) {
    pid_t pidFils = fork();
    char *texteCommande = getTexteCommande(cmd);
    if (pidFils == -1) {
        printf("Erreur fork\n");
        exit(1);
    }
    if (pidFils == 0) { // fils
        execvp(cmd->seq[p][0], cmd->seq[p]);
        perror("execvp");
        exit(1);
    }
    else {  // père
        if (cmd->backgrounded) {
            // Dans le cas d'une tache de fond, on passe à la suite
            ajouter_job(&liste_job, id, pidFils, texteCommande, ACTIF, 0);
        }
        else {
            // Dans l'autre cas, on attend le signal SIGCHLD avant de continuer.
            ajouter_job(&liste_job, id, pidFils, texteCommande, ACTIF, 1);
            pause();
        }
    }
    return EXIT_SUCCESS;
}

int executer_pipeline(struct cmdline *cmd, int id) {
    int nb_commandes = 0; // Compte le nombre de commandes dans le cas d'un tube

    // Compter le nombre de commandes.
    while(cmd->seq[nb_commandes] != NULL) {
        nb_commandes++;
    }

    // Créer nb_commandes - 1 pipes pour relier les commandes entre elles
    int nb_tubes = nb_commandes - 1;
    int tubes[nb_tubes][2];
    for (int i = 0; i < nb_tubes; i++) {
        if (pipe(tubes[i]) == -1) {
            perror("pip\n");
        }
    }

    for (int i = 0 ; i < nb_commandes ; i++) {
        // Créer nb_commandes fils, pour l'éxécution de chaque commande.
        pid_t pid = fork();

        if (pid == 0) { // processus fils
            // Fermer les descripteurs de fichier inutilisés.
            for (int j = 0; j < nb_tubes; j++) {
                if (j != i - 1) {
                    close(tubes[j][0]);
                }
                if (j != i) {
                    close(tubes[j][1]);
                }
            }


            // Redirection des descripteurs
            if (i != 0) {
                dup2(tubes[i - 1][0], 0);
                close(tubes[i - 1][0]);
            }
            if (i != nb_tubes) {
                dup2(tubes[i][1], 1);
                close(tubes[i][1]);
            }

            // Exécution de la commande
            if (execvp(cmd->seq[i][0], cmd->seq[i]) == -1) {
                perror("Erreur lors de l'exécution de la commande");
                exit(EXIT_FAILURE);
            }
        }
    }

    // Fermer les descripteurs de fichier inutilisés dans le processus père
    for (int i = 0; i < nb_tubes; i++) {
        close(tubes[i][0]);
        close(tubes[i][1]);
    }

    // Attendre la fin des processus enfants
    for (int i = 0; i < nb_commandes; i++) {
        wait(NULL);
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
    printf("SIGNAL RECU : %d \n", sgn);
    int pid = 0;
    printf("pid1 : %d\n", pid);
    getAvantPlan(liste_job, &pid);
    printf("okay");
    if (pid) {
        printf("pid3 : %d\n", pid);
        kill(pid, SIGKILL);
    }
}

void handler_SIGSTOP(int sgn) {
    printf("SIGNAL RECU : %d \n", sgn);
    int pid = 0;
    printf("pid1 : %d\n", pid);
    getAvantPlan(liste_job, &pid);
    printf("okay\n");
    if (pid) {
        printf("pid3 : %d\n", pid);
        kill(pid, SIGSTOP);
    }
}

void handler_vide() {};

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

    // Abonner le signal SIGINT à un traitant.
    struct sigaction action_SIGINT;
    action_SIGINT.sa_handler = handler_vide;
    action_SIGINT.sa_flags = SIG_IGN;
    sigaction(SIGINT, &action_SIGINT, NULL);

    // Abonner le signal SIGSTOP à un traitant.
    struct sigaction action_SIGSTOP;
    action_SIGSTOP.sa_flags = SIG_IGN;
    sigaction(SIGTSTP, &action_SIGSTOP, NULL);

    do {
        int passer = 0; // Variable à 1 si on ne lance pas de fils.
        int avant_plan = 0; // Variable à 1 si on ne lance pas de fils, et qu'on reprend une tache en avant plan.
        // Invite de commande.
        getcwd(cwd, sizeof(cwd));
        printf("%s$ ", cwd);
        // Lire la commande.
        do { 
            cmd = readcmd();
        } while (cmd == NULL || cmd->seq == NULL); 

    
        verifier_executer_cmd_internes(cmd, liste_job, &sortir, &passer, &avant_plan);

        if (avant_plan == 1) {
            // attendre un sigal SIGCHLD avant de reprendre
            pause();
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

            //executer(cmd, id, 0);
            executer_pipeline(cmd, id);
            id++;
            if (cmd->out != NULL) { // Remettre en place la sortie standard.
                int nouvelleSortieStandard = open("/dev/tty", O_WRONLY);
                dup2(nouvelleSortieStandard, 1);
                close(nouvelleSortieStandard);
                printf("nouvelle sortie standard");
            }
            if (cmd->in != NULL) { // Remettre en place l'entrée stardard.
                int nouvelleEntreeStandard = open("/dev/tty", O_RDONLY);
                dup2(nouvelleEntreeStandard, 1);
                close(nouvelleEntreeStandard);
                printf("entrée standard");
            }

            
        } else if (cmd->err != NULL) {
            printf("%s", cmd->err);
        }
    } while (1);
    return EXIT_SUCCESS;
}