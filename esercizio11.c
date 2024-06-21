#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// Signal handler function
void handle_signal(int sig) {
    if (sig == SIGUSR1) {
        printf("Received SIGUSR1\n");
    }
}

int main() {
    // Set up the signal handler
    signal(SIGUSR1, handle_signal);

    pid_t pid = fork();

    if (pid < 0) {
        // Error occurred
        fprintf(stderr, "Fork Failed\n");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Child Process (PID: %d) is running\n", getpid());
        // Simulate some work with a loop
        for (int i = 0; i < 5; ++i) {
            printf("Child Process working: %d\n", i);
            sleep(1);
        }
    } else {
        // Parent process
        printf("Parent Process (PID: %d)\n", getpid());
        // Sleep to ensure the child process is running
        sleep(2);
        // Send SIGUSR1 to the child process
        kill(pid, SIGUSR1);
        // Wait for the child process to complete
        wait(NULL);
        printf("Child Complete\n");
    }

    return 0;
}
