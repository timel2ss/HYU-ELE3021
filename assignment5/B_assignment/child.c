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

// peterson algorithm
void lock(struct smStruct* smstruct, int order) {
    smstruct->flag[order] = 1;
    smstruct->turn = 1 - order;
    // when an other process is using critical section variable, this process has busy waiting status.
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

    // get shared memory id
    shmid = shmget((key_t)1234, 1024, IPC_CREAT|0666);
    if(shmid == -1) {
        perror("shared memory access is failed\n");
        return -1;
    }

    // attach the shared memory
    shmaddr = shmat(shmid, (void*)0, 0);
    if(shmaddr == (char*)-1) {
        perror("failed attach address\n");
        return -1;
    }

    smstruct = (struct smStruct*)shmaddr;

    // set process order
    int Myorder = 0;
    if(smstruct->processidassign != 0) {
        Myorder = 1;
    }

    smstruct->processidassign = getpid();

    printf("Myorder = %d, process pid = %d\n", Myorder, smstruct->processidassign);
   
    int i;
    int localcount = 0;
    // increase localcount and critical_section_variable
    for(i = 0; i < COUNTING_NUMBER; i++) {
        localcount++;
        lock(smstruct, Myorder);
        smstruct->critical_section_variable++;
        unlock(smstruct, Myorder);
    }

    printf("child finish! local count = %d\n", localcount);

    // detach the shared memory
    ret = shmdt(shmaddr);
    if(ret == -1) {
        perror("detach failed\n");
        return -1;
    }

    return 0;
}
