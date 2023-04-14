#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <libgen.h>

#define BUFSIZE 32

int verif_execution(int retour_primitive, char * msg, int code_erreur) {
    if (retour_primitive == -1 ) {
        perror(msg);
        exit(code_erreur);
    }
    return retour_primitive;
}


int main(int argc, char * argv[]) {

    char tampon[BUFSIZE]; // tableau de mémoire tampon.
    int nblus;

    if (argc != 3) {
        fprintf(stderr, "usage : %s <src> <dest> \n", basename(argv[0]));
        return 1;
    }

    int desc_source = verif_execution(open(argv[1], O_RDONLY), "Fichier source n'existe pas ", 2);
    int desc_destination = verif_execution(open(argv[2], O_WRONLY | O_CREAT| O_TRUNC, 0640), "Fichier destination ne peut pas être ouvert", 3);

    while ((nblus = read(desc_source, &tampon, BUFSIZE)) > 0) {
        write(desc_destination, &tampon, nblus);

    }

    verif_execution(close(desc_source), "Fichier source infermable", 4);
    verif_execution(close(desc_destination), "Fichier destination infermable", 5);

    return EXIT_SUCCESS;
}