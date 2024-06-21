#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Declare a key for thread-specific data
pthread_key_t key;

// Destructor function for the thread-specific data
void destructor(void *data) {
    free(data);
}

void *thread_function(void *arg) {
    // Allocate memory for the thread-specific data
    int *thread_data = malloc(sizeof(int));
    if (thread_data == NULL) {
        perror("malloc");
        pthread_exit(NULL);
    }

    // Store thread-specific data
    *thread_data = (int)(size_t)arg;
    pthread_setspecific(key, thread_data);

    // Retrieve and print thread-specific data
    int *retrieved_data = (int *)pthread_getspecific(key);
    printf("Thread %d: thread-specific data = %d\n", *retrieved_data, *retrieved_data);

    // Free the allocated memory (done automatically when thread exits)
    pthread_exit(NULL);
}

int main() {
    // Initialize the key with the destructor function
    pthread_key_create(&key, destructor);

    // Create threads
    pthread_t threads[5];
    for (int i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, thread_function, (void *)(size_t)i);
    }

    // Join threads
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    // Delete the key
    pthread_key_delete(key);

    return 0;
}
