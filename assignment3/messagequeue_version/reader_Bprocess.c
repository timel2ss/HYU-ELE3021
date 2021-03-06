#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>

#define MSG_SIZE 20

struct msgbuf {
    long msgtype;
    char mtext[MSG_SIZE];
};

int main(void) {
    key_t key_id;
    struct msgbuf rsvbuf;
    int msgtype = 3;

    // get message queue's id with key 1234
    key_id = msgget((key_t)1234, IPC_CREAT | 0666);
    if(key_id == -1) {
        perror("msgget error: ");
        return -1;
    }

    while(1) {
        // receive message from message queue
        if(msgrcv(key_id, (void*)&rsvbuf, sizeof(struct msgbuf), msgtype, 0) == -1) {
            perror("msgrcv error: ");
        }
        else {
            // if message is "quit", then escape loop
            if(!strcmp(rsvbuf.mtext, "quit")) {
                break;
            }

            printf("The received message: %s\n", rsvbuf.mtext);
        }

    }

    return 0;
}
