#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        perror("usage : ./ex2 <source> <destination>");
        exit(1);
    }
    int fd_destination = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0444);
    if (fd_destination == -1) {
        perror("erreur d'ouverture du fichier destination");
        exit(2);
    }
    if (dup2(fd_destination, 1) == -1) {
        perror("erreur de duplication de descripteur");
        exit(3);
    }
    if (execlp("cat", "cat", argv[1], NULL) == -1 ) {
        
    }
    
}