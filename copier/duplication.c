#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char * argv[]) {
    int pid;
    pid = fork();
    if (pid == 0) { // fils
        int desc = open("temp.txt", O_WRONLY | O_CREAT| O_TRUNC, 0640);
        for (int i = 0 ; i < 10 ; i++) {
            dprintf(desc, "FILS\n");
            sleep(1);
        }
    }
    else {  // père
        int desc = open("temp.txt", O_WRONLY | O_CREAT| O_TRUNC, 0640);
        for (int i = 0 ; i < 10 ; i++) {
            dprintf(desc, "PERE\n");
            sleep(1);
        }
    }
    return 0;
}