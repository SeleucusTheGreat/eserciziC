// reader.c
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
    // Locate the shared memory segment
    int shmid = shmget(key, 1024, 0666);
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

    // Locate the semaphore set
    int semid = semget(key, 1, 0666);
    if (semid == -1) {
        perror("semget failed");
        exit(1);
    }

    // Acquire the semaphore before reading
    sem_acquire(semid);

    // Read from the shared memory
    printf("Data in shared memory: %s\n", shared_memory);

    // Release the semaphore after reading
    sem_release(semid);

    // Detach the shared memory segment
    if (shmdt(shared_memory) == -1) {
        perror("shmdt failed");
        exit(1);
    }

    return 0;
}
