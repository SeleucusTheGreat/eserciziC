#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 10
#define NUM_ITEMS 20

// Shared buffer and its associated variables
int buffer[BUFFER_SIZE];
int count = 0;
int in = 0;
int out = 0;

// Mutex and condition variables
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_producer = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_consumer = PTHREAD_COND_INITIALIZER;

// Producer function
void *producer(void *arg) {
    for (int i = 0; i < NUM_ITEMS; i++) {
        // Lock the mutex before accessing the buffer
        pthread_mutex_lock(&mutex);

        // Wait if the buffer is full
        while (count == BUFFER_SIZE) {
            pthread_cond_wait(&cond_producer, &mutex);
        }

        // Produce an item
        buffer[in] = i;
        in = (in + 1) % BUFFER_SIZE;
        count++;
        printf("Produced: %d\n", i);

        // Signal the consumer that an item is available
        pthread_cond_signal(&cond_consumer);

        // Unlock the mutex after accessing the buffer
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// Consumer function
void *consumer(void *arg) {
    for (int i = 0; i < NUM_ITEMS; i++) {
        // Lock the mutex before accessing the buffer
        pthread_mutex_lock(&mutex);

        // Wait if the buffer is empty
        while (count == 0) {
            pthread_cond_wait(&cond_consumer, &mutex);
        }

        // Consume an item
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;
        printf("Consumed: %d\n", item);

        // Signal the producer that a slot is available
        pthread_cond_signal(&cond_producer);

        // Unlock the mutex after accessing the buffer
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;

    // Create producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Wait for the threads to finish
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Destroy the mutex and condition variables
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_producer);
    pthread_cond_destroy(&cond_consumer);

    return 0;
}
