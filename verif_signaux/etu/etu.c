#include <stdio.h>    /* entrées/sorties */
#include <unistd.h>   /* primitives de base : fork, ...*/
#include <stdlib.h>   /* exit */
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#define MAX_PAUSES 10     /* nombre d'attentes maximum */



void handlerAfficher(int num_sgn) {
    printf("Reception %d\n", num_sgn);
}


int main(int argc, char *argv[]) {
    struct sigaction action;
    // Créer une action
    action.sa_handler = handlerAfficher;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);

    // S'abonner
    sigaction(SIGUSR1, &action, NULL);
    sigaction(SIGUSR2, &action, NULL);

    // Masquer les signaux SIGINT et SIGUSR1
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_SETMASK, &mask, NULL);

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
    sigdelset(&mask, SIGUSR1);
    sigprocmask(SIG_SETMASK, &mask, NULL);

    // Attendre 10 secondes au plus et demasquer SIGINT
    sleep(10);
    sigdelset(&mask, SIGINT);
    sigprocmask(SIG_SETMASK, &mask, NULL);

    printf("Salut, FIN\n"); 
    return EXIT_SUCCESS;
}