#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define ITERATE_TIMES 10

sem_t chopstick[6];

void LR_solution(int number) {
    int left = number;
    int right = (number + 1) % 6;
    
    // if number is even, the philosopher picks up the right chopstick first
    if(number % 2 == 0) {
        // pick up right chopstick
        sem_wait(&chopstick[right]);
        printf("Philosopher %d picks up the right chopstick(%d)\n", number, right);

        // pick up left chopstick
        sem_wait(&chopstick[left]);
        printf("Philosopher %d picks up the left chopstick(%d)\n", number, left);
        
        printf("Philosopher %d starts meal.\n", number);
        sleep(1);
        printf("Philosopher %d has finished meal.\n", number);
        
        // put down left chopstick
        sem_post(&chopstick[left]);
        
        // put down right chopstick
        sem_post(&chopstick[right]);
    }
    // if number is odd, the philosopher picks up the left chopstick first
    else {
        // pick up left chopstick
        sem_wait(&chopstick[left]);
        printf("Philosopher %d picks up the left chopstick(%d)\n", number, left);

        // pick up right chopstick
        sem_wait(&chopstick[right]);
        printf("Philosopher %d picks up the right chopstick(%d)\n", number, right);
        
        printf("Philosopher %d starts meal.\n", number);
        sleep(1);
        printf("Philosopher %d has finished meal.\n", number);
  
        // put down right chopstick
        sem_post(&chopstick[right]);
        
        // put down left chopstick
        sem_post(&chopstick[left]);
    }
}

void philosopher_func(void* n) {
    int i;
    // philosophers have meal for ITERATE_TIMES
    for(i = 0; i < ITERATE_TIMES; i++) {
        // solve dining philosophers problem using the LR Solution
        LR_solution(*((int*)n));
    }
}

int main(void) {
    int i;
    // initialize semaphores to 1
    for(i = 0; i < 6; i++) {
        sem_init(&chopstick[i], 0, 1);
    }

    pthread_t philosopher[6];
    int argument[6] = {0, 1, 2, 3, 4, 5};

    // create 6 threads and run philosopher_func
    for(i = 0; i < 6; i++) {
        pthread_create(&philosopher[i], NULL, (void*)&philosopher_func, (void*)&argument[i]);
    }

    // wait for the end of 6 threads
    for(i = 0; i < 6; i++) {
        pthread_join(philosopher[i], NULL);
    }

    // destroy semaphores
    for(i = 0; i < 6; i++) {
        sem_destroy(&chopstick[i]);
    }

    return 0;
}
