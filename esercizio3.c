#include <stdio.h>
#include <string.h>

void main() {
    char str[] = "Hello, world! This is a test.";
    char delimiters[] = " ,.!"; 
    char *token;

    // Get the first token
    token = strtok(str, delimiters);

    // Walk through the rest of the tokens
    while (token != NULL) {
        printf("%s\n", token);
        token = strtok(NULL, delimiters);
    }

    printf("%s",str);
}
  