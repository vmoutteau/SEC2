#include <stdio.h>    /* entrées/sorties */
#include <unistd.h>   /* primitives de base : fork, ...*/
#include <stdlib.h>   /* exit */
#include <signal.h>
#include <sys/types.h>

#define MAX_PAUSES 10     /* nombre d'attentes maximum */

void handlerAfficher(int num_sgn) {
    printf("\nnuméro de signal reçu : %d \n", num_sgn);
    printf("numéro de pid : %d \n", getpid());
}

int main(int argc, char *argv[]) {
	int nbPauses;
	
	nbPauses = 0;
	printf("Processus de pid %d\n", getpid());

    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_handler = handlerAfficher;
    action.sa_flags = 0;

    
    for (int sgn = 1 ; sgn < SIGRTMAX ; sgn ++) {
        sigaction(sgn, &action, NULL);
    };
    
    fork();
   
    for (nbPauses = 0 ; nbPauses < MAX_PAUSES ; nbPauses++) {
        pause();		// Attente d'un signal
        printf("pid = %d - NbPauses = %d\n", getpid(), nbPauses);
    } ;
    printf("-------------");
    return EXIT_SUCCESS;
}
	


