#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_TEXT 512

struct my_msg {
    long msg_type;
    char msg_text[MAX_TEXT];
};

int main() {
    key_t key;
    int msgid;
    struct my_msg received_message;

    // Generate a unique key
    key = ftok("progfile", 65);

    // Create or access a message queue
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    // Infinite loop to keep receiving messages
    while (1) {
        // Receive a message
        if (msgrcv(msgid, &received_message, sizeof(received_message.msg_text), 0, 0) == -1) {
            perror("msgrcv failed");
            exit(EXIT_FAILURE);
        }
        printf("Message received: %s\n", received_message.msg_text);
    }

    // This part will never be reached in this example
    // But if you want to stop the receiver gracefully, you could add a termination condition
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl(IPC_RMID) failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}
