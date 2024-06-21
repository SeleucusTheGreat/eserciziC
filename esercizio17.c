#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5

// Shared resource
int counter = 0;

// Mutex for protecting the shared resource
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

// Thread function
void *increment_counter(void *arg) {
    int thread_id = (int)(size_t)arg;

    // Lock the mutex before accessing the shared resource
    pthread_mutex_lock(&counter_mutex);

    // Critical section: increment the shared counter
    counter++;
    printf("Thread %d: counter = %d\n", thread_id, counter);

    // Unlock the mutex after accessing the shared resource
    pthread_mutex_unlock(&counter_mutex);

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int ret;

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        ret = pthread_create(&threads[i], NULL, increment_counter, (void *)(size_t)i);
        if (ret != 0) {
            printf("Error: pthread_create failed with code %d\n", ret);
            return EXIT_FAILURE;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&counter_mutex);

    printf("Final counter value: %d\n", counter);
    return EXIT_SUCCESS;
}
