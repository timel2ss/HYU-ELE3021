#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>

#define COUNTING_NUMBER 2000000

struct smStruct {
    int processidassign;
    int turn;
    int flag[2];
    int critical_section_variable;
};

void lock(struct smStruct* smstruct, int order) {
    smstruct->flag[order] = 1;
    smstruct->turn = 1 - order;
    while(smstruct->flag[1 - order] == 1 && smstruct->turn == 1 - order);
}

void unlock(struct smStruct* smstruct, int order) {
    smstruct->flag[order] = 0;
}


int main(void) {
    struct smStruct* smstruct;
    
    int shmid;
    void *shmaddr;
    int ret;

    shmid = shmget((key_t)1234, 1024, IPC_CREAT|0666);
    if(shmid == -1) {
        perror("shared memory access is failed\n");
        return -1;
    }

    shmaddr = shmat(shmid, (void*)0, 0);
    if(shmaddr == (char*)-1) {
        perror("failed attach address\n");
        return -1;
    }

    smstruct = (struct smStruct*)shmaddr;

    int Myorder = 0;
    if(smstruct->processidassign != 0) {
        Myorder = 1;
    }

    smstruct->processidassign = getpid();

    printf("Myorder = %d, process pid = %d\n", Myorder, smstruct->processidassign);
   
    int i;
    int localcount = 0;
    for(i = 0; i < COUNTING_NUMBER; i++) {
        localcount++;
        lock(smstruct, Myorder);
        smstruct->critical_section_variable++;
        unlock(smstruct, Myorder);
    }

    printf("child finish! local count = %d\n", localcount);

    ret = shmdt(shmaddr);
    if(ret == -1) {
        perror("detach failed\n");
        return -1;
    }

    return 0;
}
