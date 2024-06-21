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
    struct mq_attr attr;
    char buffer[MAX_SIZE];

    // Initialize the queue attributes
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    // Create the message queue
    mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0644, &attr);
    if (mq == (mqd_t) -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    printf("Enter a message to send (type 'exit' to stop): ");
    while (fgets(buffer, MAX_SIZE, stdin)) {
        // Remove newline character from input
        buffer[strcspn(buffer, "\n")] = '\0';

        if (mq_send(mq, buffer, strlen(buffer) + 1, 0) == -1) {
            perror("mq_send");
            exit(EXIT_FAILURE);
        }

        if (strncmp(buffer, MSG_STOP, strlen(MSG_STOP)) == 0) {
            break;
        }
        
        printf("Enter a message to send (type 'exit' to stop): ");
    }

    // Close the message queue
    if (mq_close(mq) == -1) {
        perror("mq_close");
        exit(EXIT_FAILURE);
    }

    return 0;
}
