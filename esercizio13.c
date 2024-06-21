#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

#define SEM_KEY 0x1234

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

void wait_semaphore(int semid, int semnum) {
    struct sembuf sb;
    sb.sem_num = semnum;
    sb.sem_op = -1;  // P operation
    sb.sem_flg = 0;
    if (semop(semid, &sb, 1) == -1) {
        perror("semop");
        exit(EXIT_FAILURE);
    }
}

void signal_semaphore(int semid, int semnum) {
    struct sembuf sb;
    sb.sem_num = semnum;
    sb.sem_op = 1;  // V operation
    sb.sem_flg = 0;
    if (semop(semid, &sb, 1) == -1) {
        perror("semop");
        exit(EXIT_FAILURE);
    }
}

int main() {
    int semid;
    pid_t pid;
    union semun sem_union;

    // Create a semaphore set with 1 semaphore
    semid = semget(SEM_KEY, 1, 0666 | IPC_CREAT);
    if (semid == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // Initialize semaphore to 1
    sem_union.val = 1;
    if (semctl(semid, 0, SETVAL, sem_union) == -1) {
        perror("semctl");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // Child process
        for (int i = 0; i < 5; i++) {
            wait_semaphore(semid, 0);  // P operation
            printf("Child process accessing the resource\n");
            sleep(1);
            printf("Child process releasing the resource\n");
            signal_semaphore(semid, 0);  // V operation
            sleep(1);
        }
    } else {  // Parent process
        for (int i = 0; i < 5; i++) {
            wait_semaphore(semid, 0);  // P operation
            printf("Parent process accessing the resource\n");
            sleep(1);
            printf("Parent process releasing the resource\n");
            signal_semaphore(semid, 0);  // V operation
            sleep(1);
        }

        // Wait for child process to finish
        wait(NULL);

        // Remove the semaphore set
        if (semctl(semid, 0, IPC_RMID, sem_union) == -1) {
            perror("semctl");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
