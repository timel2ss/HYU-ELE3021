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

    // open the named pipe
    fd = open(PIPENAME, O_WRONLY);
    if(fd < 0) {
        printf("Open failed\n");
        return -1;
    }

    printf("Hello, this is A process. I\'ll give the data to B.\n");

    while(1) {
        // write user input to msg
        fgets(msg, sizeof(msg), stdin);
        msg[strlen(msg) - 1] = '\0';

        // write msg to fd
        ret = write(fd, msg, sizeof(msg));
        if(ret < 0) {
            printf("Write failed\n");
            return -1;
        }

        // if msg is "quit", then escape loop
        if(!strcmp(msg, "quit")) {
            break;
        }
    }

    return 0;
}
