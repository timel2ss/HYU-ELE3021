#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

#define MSG_SIZE 512

struct shared_memory {
    int flag;
    char msg[MSG_SIZE];
};

int main(void) {
    int shmid;
    void* shmaddr;
    int ret, flag;

    shmid = shmget((key_t)1234, 1024, IPC_CREAT | 0666);
    if(shmid == -1) {
        perror("Shared memory access is failed\n");
        return -1;
    }

    shmaddr = shmat(shmid, (void*)0, 0);
    if(shmaddr == (char*)-1) {
        perror("Attach failed\n");
        return -1;
    }
    
    struct shared_memory* data = (struct shared_memory*)shmaddr;
    data->flag = 0;

    while(1) {
        if(!data->flag) {
            fgets(data->msg, sizeof(data->msg), stdin);
            data->msg[strlen(data->msg) - 1] = '\0';

            data->flag = 1;

            if(!strcmp(data->msg, "quit")) {
                break;
            }
        }
    }

    ret = shmdt(shmaddr);
    if(ret == -1) {
        perror("Detach failed\n");
        return -1;
    }

    return 0;
}
