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
        // assign writer id to cur_writer        
        cur_writer = *((int*)n);
        // count how many times the writer accessed, and assign it to cur_count
        cur_count = access_count[cur_writer]++;
        sem_post(&wrt);
    }
}

void reader() {
    int i;
    for(i = 0; i < COUNTING_NUMBER; i++) {
        usleep(30000);
        // prevent race condition of readcount
        sem_wait(&S);
        readcount++;
        if(readcount == 1) {
            // block writer's access when reader already accessed
            sem_wait(&wrt);
        }
        sem_post(&S);

        printf("The most recent writer id: [%d], count: [%d]\n", cur_writer, cur_count);
        
        // prevent race condition of readcount
        sem_wait(&S);
        readcount--;
        if(readcount == 0) {
            // if there's no reader in the critical section, then unlock the writer
            sem_post(&wrt);
        }
        sem_post(&S);
    }
}

int main(void) {
    // initialize semaphores to 1
    sem_init(&S, 0, 1);
    sem_init(&wrt, 0, 1);

    pthread_t r[2], w[5];
    int argument[5] = {0, 1, 2, 3, 4};
    
    int i;
    //create 2 threads and run reader func
    for(i = 0; i < 2; i++) {
        pthread_create(&r[i], NULL, (void*)&reader, NULL);
    }

    // create 5 threads and run writer func
    for(i = 0; i < 5; i++) {
        pthread_create(&w[i], NULL, (void*)&writer, (void*)&argument[i]);
    }

    // wait for the end of 2 reader threads
    for(i = 0; i < 2; i++) {
        pthread_join(r[i], NULL);
    }

    // wait for the end of 5 writer threads
    for(i = 0; i < 5; i++) {
        pthread_join(w[i], NULL);
    }

    // destroy semaphores
    sem_destroy(&S);
    sem_destroy(&wrt);

    return 0;
}
