#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>       // For O_* constants
#include <sys/stat.h>    // For mode constants
#include <mqueue.h>
#include <string.h>

#define QUEUE_NAME  "/test_queue"
#define MAX_SIZE    256
#define MSG_STOP    "exit"

int main() {
    mqd_t mq;
    char buffer[MAX_SIZE];
    ssize_t bytes_read;

    // Open the message queue
    mq = mq_open(QUEUE_NAME, O_RDONLY);
    if (mq == (mqd_t) -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for messages...\n");

    while (1) {
        bytes_read = mq_receive(mq, buffer, MAX_SIZE, NULL);
        if (bytes_read == -1) {
            perror("mq_receive");
            exit(EXIT_FAILURE);
        }

        buffer[bytes_read] = '\0';
        printf("Received message: %s\n", buffer);

        if (strncmp(buffer, MSG_STOP, strlen(MSG_STOP)) == 0) {
            break;
        }
    }

    // Close the message queue
    if (mq_close(mq) == -1) {
        perror("mq_close");
        exit(EXIT_FAILURE);
    }

    // Unlink the message queue
    if (mq_unlink(QUEUE_NAME) == -1) {
        perror("mq_unlink");
        exit(EXIT_FAILURE);
    }

    return 0;
}
