#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>



int main(int argc, char * argv[]) {
    int pid1 = fork();
    char nouvelleLigne = '\n';
    if (pid1 == 0) {
        int descEcriture = open("temp", O_WRONLY | O_CREAT| O_TRUNC, 0640);
        for (int i = 1 ; i <= 30 ; i++) {
            if (i % 10 == 0) {
                lseek(descEcriture, 0, SEEK_SET);
            }
            write(descEcriture, &i, sizeof(int));
            write(descEcriture, &nouvelleLigne, sizeof(char[1]));
            sleep(1);
        }
        return EXIT_SUCCESS;
    }
    else {
        int pid2 = fork();
        int nblus;
        if (pid2 == 0) {
            int descLecture = open("temp", O_RDONLY | O_CREAT| O_TRUNC, 0640);
            sleep(5);
            int entier=-1;
            for (int i = 1 ; i <= 25 ; i++) {
                while ((nblus = read(descLecture, &entier, sizeof(int))) > 0) {
                    printf("%d \n", entier);
                }
                sleep(5);
            }
        }
        return EXIT_SUCCESS;
    }
}