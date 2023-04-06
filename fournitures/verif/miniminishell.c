#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h> // strcmp()

int main(int argc, char* argv[]) {
    int ret;
    char buf [30];
    int codeTerm;
    pid_t pidFils;
    char chaineExit[30];
    strcpy(chaineExit, "exit");
    do { 
        printf(">>> ");
        ret = scanf("%s", buf);
        
        if (ret != EOF && strcmp(buf, chaineExit)) {
            pidFils = fork();
            if (pidFils == -1) {
            printf("Erreur fork\n");
            exit(1);
            }
            if (pidFils == 0) { // fils
                int echec = execlp(buf, buf, NULL);
                if (echec) {
                    exit(EXIT_FAILURE);
                }
                else {
                    exit(EXIT_SUCCESS); 
                }
            }
            else {  // père
                wait(&codeTerm);
                if (codeTerm == 0) {
                    printf("SUCCES\n");
                }
                else {
                    printf("ECHEC\n");
                }
            }
        }
    } while (ret != EOF && strcmp(buf, chaineExit));
    printf("\nSalut, fin du mininishell\n");
}
