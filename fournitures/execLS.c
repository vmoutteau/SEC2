#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    int codeTerm;
    pid_t pidFils;
    pidFils = fork();

    if (pidFils == -1) {
    printf("Erreur fork\n");
    exit(1);
    }
    if (pidFils == 0) { // fils
        execlp("ls", "ls", "-l", argv[1], NULL);
        exit(EXIT_SUCCESS); // succes
    }
    else {  // père
        wait(&codeTerm);
        if (codeTerm == 0) {
            printf("Commande réussi\n");
        }
        else {
            printf("Échec\n");
        }
    }
    return EXIT_SUCCESS;
}