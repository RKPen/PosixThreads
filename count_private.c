#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define ARRAY_SIZE 100000000
#define NUM_THREADS 1

int array[ARRAY_SIZE];
int privateCounts[NUM_THREADS];

void *countOnes(void *threadId) {
    intptr_t id = (intptr_t)threadId;
    int chunkSize = ARRAY_SIZE / NUM_THREADS;
    int start = id * chunkSize;
    int end = (id + 1) * chunkSize;
    int localCount = 0;

    for (int i = start; i < end; i++) {
        if (array[i] == 1) {
            localCount++;
        }
    }

    privateCounts[id] = localCount;
    pthread_exit(NULL);
}

int main() {
    clock_t start, end;
    double total_time;

    // Generate random array
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % 6; // Values between 0 and 5 (inclusive)
    }

    start = clock(); // Start measuring time here

    pthread_t threads[NUM_THREADS];
    int rc;
    long t;

    for (t = 0; t < NUM_THREADS; t++) {
        rc = pthread_create(&threads[t], NULL, countOnes, (void *)(intptr_t)t);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    end = clock(); // Stop measuring time here

    // Merge private counts to get the total count
    int totalCount = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        totalCount += privateCounts[i];
    }

    printf("Total count of ones: %d\n", totalCount);

    total_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Runtime of threaded portion: %f seconds\n", total_time);

    pthread_exit(NULL);
}
