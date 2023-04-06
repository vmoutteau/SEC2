#include <stdio.h>    /* entrées/sorties */
#include <unistd.h>   /* primitives de base : fork, ...*/
#include <stdlib.h>   /* exit */
#include <signal.h>
#include <sys/wait.h>

#define MAX_PAUSES 10     /* nombre d'attentes maximum */



void afficherPPID(int sgn) {
	printf("PID du père : %d", getppid());
	alarm(5);
}

int main(int argc, char *argv[]) {
	int nbPauses;
	
	nbPauses = 0;
	printf("Processus de pid %d\n", getpid());
	int pid = fork();
	if (pid == 0) {	// fils
		struct sigaction action;
		sigemptyset(&action.sa_mask);
		action.sa_handler = afficherPPID;
    	action.sa_flags = 0;
		sigaction(SIGALRM, &action, NULL);
		alarm(5);
		while (1) {
		}
		return EXIT_SUCCESS;
	}
	else {
		for (nbPauses = 0 ; nbPauses < MAX_PAUSES ; nbPauses++) {
			pause();		// Attente d'un signal
			printf("pid = %d - NbPauses = %d\n", getpid(), nbPauses);
		} ;
		return EXIT_SUCCESS;
	}
}
