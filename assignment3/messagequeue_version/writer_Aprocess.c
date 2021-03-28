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
    struct msgbuf sndbuf;
    char input[MSG_SIZE];

    key_id = msgget((key_t)1234, IPC_CREAT | 0666);
    if(key_id == -1) {
        perror("msgget error: ");
        return -1;
    }

    sndbuf.msgtype = 3;

    while(1) {
        fgets(input, sizeof(input), stdin);
        input[strlen(input) - 1] = '\0';
        strcpy(sndbuf.mtext, input);

        if(msgsnd(key_id, (void*)&sndbuf, sizeof(struct msgbuf), IPC_NOWAIT) == -1) {
            perror("msgsnd error: ");
        }

        if(!strcmp(input, "quit")) {
            break;
        }
    }

    return 0;
}
