#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void) {
    int p[2];
    pipe(p);
    int entier = 54;
    write(p[1],&entier, sizeof(int));
    close(p[1]);
    int pid = fork();
    if (pid == 0) { // fils
        int entier;
        close(p[1]);
        read(p[0], &entier, sizeof(int));
        printf("%d\n", entier);
        close(p[0]);
    }
}