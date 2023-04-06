#include <stdio.h>    /* entrées/sorties */
#include <unistd.h>   /* primitives de base : fork, ...*/
#include <stdlib.h>   /* exit */
#include <signal.h>
#include <stdio.h>
#define MAX_PAUSES 10     /* nombre d'attentes maximum */

struct sigaction action;

void handlerAfficher(int num_sgn) {

    printf("Réception %d\n", num_sgn);

    // Masquer les signaux SIGINT et SIGUSR1
    struct sigaction action;
    sigemptyset(&action.sa_mask);
    sigaddset(&action.sa_mask, SIGINT);
    sigaddset(&action.sa_mask, SIGUSR1);
    action.sa_handler = handlerAfficher;
    action.sa_flags = 0;
    sigaction(num_sgn, &action, NULL);

    // Attendre 10 secondes
    sleep(10);

    // S'envoyer 2 SIGUSR1
    kill(getpid(), SIGUSR1);
    kill(getpid(), SIGUSR1);

    //Attendre 5 secondes
    sleep(5);

    // S'envoyer 2 SIGUSR2
    kill(getpid(), SIGUSR2);
    kill(getpid(), SIGUSR2);

    // Demasquer SIGUSR1
    sigdelset(&action.sa_mask, SIGUSR1);

    // Attendre 10 secondes au plus et demasquer SIGINT
    sleep(10);
    sigdelset(&action.sa_mask, SIGINT);

    printf("Salut, FIN"); //loser
}

int main(int argc, char *argv[]) {

    // Créer une action
    sigemptyset(&action.sa_mask);
    action.sa_handler = handlerAfficher;
    action.sa_flags = 0;

    // S'abonner
    sigaction(SIGUSR1, &action, NULL);
    sigaction(SIGUSR2, &action, NULL);

    // Attendre des signaux
    while (1) {
      pause();
    }
    return EXIT_SUCCESS;
}