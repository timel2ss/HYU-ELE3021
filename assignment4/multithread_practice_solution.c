#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARGUMENT_NUMBER 20

// store computational result for each thread
long long result[ARGUMENT_NUMBER];
// sum all results for each thread
long long total_result = 0;

void *ThreadFunc(void *n) {
    long long i;
    long long number = *((long long*)n);
    
    printf("number = %lld\n", number);
    
    // store 25,000,000 times of argument number to the result of thread
    for(i = 0; i < 25000000; i++) {
        result[number] += number;
    }
}

int main(void) {
    pthread_t threadID[ARGUMENT_NUMBER];

    long long argument[ARGUMENT_NUMBER];
    long long i;

    for(i = 0; i < ARGUMENT_NUMBER; i++) {
        argument[i] = i;
    }
    
    // create 20 threads and run ThreadFunc function
    for(i = 0; i < ARGUMENT_NUMBER; i++) {
        pthread_create(&(threadID[i]), NULL, ThreadFunc, (void*)&argument[i]);
    }
   
    // When all threads are created, print a string on console
    printf("Main Thread is waiting for Child Thread!\n");

    // wait for the end of 20 threads
    for(i = 0; i < ARGUMENT_NUMBER; i++) {
        pthread_join(threadID[i], NULL);
    }

    // add the result of each thread to total_result
    for(i = 0; i < ARGUMENT_NUMBER; i++) {
        total_result += result[i];
    }

    printf("result = %lld\n", total_result);
    
    return 0;
}
