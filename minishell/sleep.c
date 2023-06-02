#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("usage : ./sleep <temps d'attente>");
    }
    int n = atoi(argv[1]);
    for (int i = 1 ; i <= n ; i++) {
        sleep(1);
        printf("plus que %d secondes à attendres ! \n", n-i);
    }
    printf("fin de l'attente de %d secondes.\n", atoi(argv[1]));
}