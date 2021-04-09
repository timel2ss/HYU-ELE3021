#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/wait.h>

#define COUNTING_NUMBER 2000000

struct smStruct {
    int processidassign;
    int turn;
    int flag[2];
    int critical_section_variable;
};

int main(void) {
    struct smStruct* smstruct;
    
    int shmid;
    void *shmaddr;
    int ret;

    // get shared memory id
    shmid = shmget((key_t)1234, 1024, IPC_CREAT|0666);
    if(shmid == -1) {
        perror("Shared memory access is failed\n");
        return -1;
    }

    // attach the shared memory
    shmaddr = shmat(shmid, (void*)0, 0);
    if(shmaddr == (char*)-1) {
        perror("failed attach address\n");
        return -1;
    }

    smstruct = (struct smStruct*)shmaddr;
    smstruct->turn = 0;
    smstruct->processidassign = 0;
    smstruct->flag[0] = 0;
    smstruct->flag[1] = 0;
    smstruct->critical_section_variable = 0;

    pid_t pid1, pid2;
    int status;

    // create child process and execute "child" program
    if((pid1 = fork()) == 0) {
        execl("./child", NULL);
    }
    if((pid2 = fork()) == 0) {
        execl("./child", NULL);
    }

    // wait for the end of child processes
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);

    printf("Actual Count: %d | Expected Count: %d\n", smstruct->critical_section_variable, COUNTING_NUMBER * 2);

    // detach the shared memory
    ret = shmdt(shmaddr);
    if(ret == -1) {
        perror("detach failed\n");
        return -1;
    }

    // remove the shared memory
    ret = shmctl(shmid, IPC_RMID, 0);
    if(ret == -1) {
        perror("remove failed\n");
        return -1;
    }

    return 0;
}
