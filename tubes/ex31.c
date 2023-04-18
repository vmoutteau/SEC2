#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void) {
    int pid = fork();
    int p[2];
    if (pid == 0) { // fils
        int entier;
        close(p[1]);
        read(p[0], &entier, sizeof(int));
        printf("%d", entier);
        close(p[0]);
    } else {
        pipe(p);
        close(p[0]);
        int entier = 54;
        write(p[1],&entier, sizeof(int));
        close(p[1]);
    }
}