#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("usage : ./sleep <temps d'attente>");
    }
    sleep(atoi(argv[1]));
    printf("fin de l'attente de %d secondes.\n", atoi(argv[1]));
}