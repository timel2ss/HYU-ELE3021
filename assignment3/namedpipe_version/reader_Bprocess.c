#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MSG_SIZE 512
#define PIPENAME "./named_pipe_file"

int main(void) {
    int ret;
    char msg[MSG_SIZE];
    int fd;

    ret = access(PIPENAME, F_OK);
    if(ret == 0) {
        unlink(PIPENAME);
    }

    ret = mkfifo(PIPENAME, 0666);
    if(ret < 0) {
        printf("Creation of named pipe failed\n");
        return -1;
    }

    fd = open(PIPENAME, O_RDWR);
    if(fd < 0) {
        printf("Opening of named pipe failed\n");
        return -1;
    }

    printf("Hello. this is B process. give me the data.\n");

    while(1) {
        ret = read(fd, msg, sizeof(msg));
        if(ret < 0) {
            printf("Read failed\n");
            return -1;
        }

        if(!strcmp(msg, "quit")) {
            break;
        }

        printf("%s\n", msg);    
    }

    return 0;
}
