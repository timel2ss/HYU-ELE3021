#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid1, pid2;
    int status;

    printf("Waiting for Child Processes\n");

    if((pid1 = fork()) == 0) {
        execl("./write_file", NULL);
    }

    if((pid2 = fork()) == 0) {
        execl("./read_file", NULL);
    }

    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);
    
    printf("Child processes are exit (0) (0)\n");
    return 0;
}



