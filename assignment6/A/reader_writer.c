#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define COUNTING_NUMBER 1000

sem_t S;
sem_t wrt;

int readcount = 0;
int cur_writer;
int cur_count;
int access_count[5];

void writer(void* n) {
    int i;
    for(i = 0; i < COUNTING_NUMBER; i++) {
        usleep(100000);
        sem_wait(&wrt);
        
        cur_writer = *((int*)n);
        cur_count = access_count[cur_writer]++;
        
        sem_post(&wrt);
    }
}

void reader() {
    int i;
    for(i = 0; i < COUNTING_NUMBER; i++) {
        usleep(30000);
        sem_wait(&S);
        readcount++;
        if(readcount == 1) {
            sem_wait(&wrt);
        }
        sem_post(&S);

        printf("The most recent writer id: [%d], count: [%d]\n", cur_writer, cur_count);
        
        sem_wait(&S);
        readcount--;
        if(readcount == 0) {
            sem_post(&wrt);
        }
        sem_post(&S);
    }
}

int main(void) {
    sem_init(&S, 0, 1);
    sem_init(&wrt, 0, 1);

    pthread_t r[2], w[5];

    int argument[5] = {0, 1, 2, 3, 4};
    int i;
    for(i = 0; i < 2; i++) {
        pthread_create(&r[i], NULL, (void*)&reader, NULL);
    }

    for(i = 0; i < 5; i++) {
        pthread_create(&w[i], NULL, (void*)&writer, (void*)&argument[i]);
    }

    for(i = 0; i < 2; i++) {
        pthread_join(r[i], NULL);
    }

    for(i = 0; i < 5; i++) {
        pthread_join(w[i], NULL);
    }

    sem_destroy(&S);
    sem_destroy(&wrt);

    return 0;
}
