#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define MSG_SIZE 512
#define PIPENAME "./named_pipe_file"

int main(void) {
    int ret;
    char msg[MSG_SIZE];
    int fd;

    fd = open(PIPENAME, O_WRONLY);
    if(fd < 0) {
        printf("Open failed\n");
        return 1;
    }

    while(1) {
        fgets(msg, sizeof(msg), stdin);
        msg[strlen(msg) - 1] = '\0';

        ret = write(fd, msg, sizeof(msg));
        if(ret < 0) {
            printf("Write failed\n");
            return 1;
        }

        if(!strcmp(msg, "quit")) {
            break;
        }
    }

    return 0;
}
