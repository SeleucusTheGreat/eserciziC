#include <stdio.h>

int main() {
    // Open a file for writing
    FILE *file = fopen("example.txt", "w");
    if (file == NULL) {
        perror("Error opening file for writing");
        return 1;
    }

    // Write to the file
    fprintf(file, "Hello, World!\n");
    fprintf(file, "Number: %d\n", 42);
    fclose(file);  // Close the file after writing

    // Open the file for reading
    file = fopen("example.txt", "r");
    if (file == NULL) {
        perror("Error opening file for reading");
        return 1;
    }

    // Read from the file
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    // Close the file after reading
    if (fclose(file) != 0) {
        perror("Error closing file");
    }

    return 0;
}
