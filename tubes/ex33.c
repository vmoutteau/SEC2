#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void) {
    int p[2];
    pipe(p);
    int pid = fork();
    if (pid == 0) { // fils
        close(p[1]);
        int j;
        while(read(p[0], &j, sizeof(int)) > 0 ) {
            printf("%d\n", j);
        }
        close(p[0]);
        exit(0);
    } else {
        close(p[0]);
        for (int i = 0 ; i < 20 ; i++) {
            write(p[1], &i, sizeof(int));
        }
        sleep(10);
        close(p[1]);
        exit(0);
    }
}