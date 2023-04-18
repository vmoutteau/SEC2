#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int compteur_erreur;

int ok(int retour_commande, char *message) {
    if (retour_commande == -1) {
        perror(message);
        exit(compteur_erreur++);
    }
    return retour_commande;
}

int main(int argc, char *argv[]) {
    if (argc != 2 ) {
        printf("usage : ");
        exit(1);
    }

    int pipe_who_grep[2];
    ok(pipe(pipe_who_grep), "erreur ouverture pipe 1");
   
    int pid1 = ok(fork(), "erreur fork 1");
    if (pid1 == 0) {
        ok(close(pipe_who_grep[0]), "erreur close pipe");
        ok(dup2(pipe_who_grep[1], 1), "erreur dup");
        ok(execlp("who", "who", NULL), "erreur who");
    } 
    else {
        ok(close(pipe_who_grep[1]), "erreur dup");
        int pipe_grep_wc[2];
        ok(pipe(pipe_grep_wc), "erreur ouverture pipe 2");
       
        int pid2 = ok(fork(), "erreur fork 2");

        if (pid2 == 0) {
            ok(close(pipe_grep_wc[0]), "erreur close");
            ok(dup2(pipe_who_grep[0], 0), "erreur dup");
            ok(dup2(pipe_grep_wc[1], 1), "erreur dup");
            ok(execlp("grep", "grep", argv[1], NULL), "erreur grep");
        } else {
            ok(close(pipe_grep_wc[1]), "erreur close");
            ok(close(pipe_who_grep[0]), "erreur close");
            ok(dup2(pipe_grep_wc[0], 0), "erreur dup");
            ok(execlp("wc", "wc", "-l", NULL), "erreur wc");
        }
    }
    return EXIT_SUCCESS;
}