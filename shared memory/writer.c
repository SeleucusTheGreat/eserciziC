// writer.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <unistd.h>

#define SHM_KEY 1234

// Function to acquire a semaphore
void sem_acquire(int semid) {
    struct sembuf sb = {0, -1, 0}; // Set to allocate resource
    if (semop(semid, &sb, 1) == -1) {
        perror("semop failed");
        exit(1);
    }
}

// Function to release a semaphore
void sem_release(int semid) {
    struct sembuf sb = {0, 1, 0}; // Set to release resource
    if (semop(semid, &sb, 1) == -1) {
        perror("semop failed");
        exit(1);
    }
}

int main() {
    // Generate a unique key for shared memory
    key_t key = SHM_KEY;

    // Create a shared memory segment
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    // Attach the shared memory segment
    char *shared_memory = (char *)shmat(shmid, NULL, 0);
    if (shared_memory == (char *)-1) {
        perror("shmat failed");
        exit(1);
    }

    // Create a semaphore set with one semaphore
    int semid = semget(key, 1, 0666 | IPC_CREAT);
    if (semid == -1) {
        perror("semget failed");
        exit(1);
    }

    // Initialize the semaphore to 1 (available)
    semctl(semid, 0, SETVAL, 1);

    // Acquire the semaphore before writing
    sem_acquire(semid);

    // Write to the shared memory
    strcpy(shared_memory, "Hello from Writer Process!");

    // Release the semaphore after writing
    sem_release(semid);

    // Sleep for a while to allow the reader to read the data
    sleep(10);

    // Detach the shared memory segment
    if (shmdt(shared_memory) == -1) {
        perror("shmdt failed");
        exit(1);
    }

    // Remove the shared memory segment
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl failed");
        exit(1);
    }

    // Remove the semaphore set
    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl failed");
        exit(1);
    }

    return 0;
}
