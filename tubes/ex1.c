#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int fd = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (fd == -1) {
        perror("imposssible d'ouvrir le fichier");
        exit(1);
    }
    dup2(fd, 1); // Rediriger la sortie standart vers le fichier
    close(fd);
    execlp("ls", "ls", "-u", "-lt", NULL);
}