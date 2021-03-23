#include <stdio.h>

int main(void) {
    FILE* file;
    
    int first_row;
    for(int i = 0; i < 6; i++) {
        sleep(2);
        
        file = fopen("test.txt", "r");
        if(file == NULL) {
            printf("File cannot be found.\n");
            return 1;
        }
        
        fscanf(file, "%d", &first_row);
        fclose(file);

        printf("%d\n", first_row);
    }

    return 0;
}




