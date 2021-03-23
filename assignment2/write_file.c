#include <stdio.h>

int main(void) {
    FILE* file;

    for(int i = 0; i < 12; i++) {
        sleep(1);
        file = fopen("test.txt", "w");
        fprintf(file, "%d\n", i);
        printf("Child1 wrote %d.\n", i);
        fclose(file);
    }

    return 0;
}





