#include <stdio.h>
#include <string.h>


int main() {
    // Open a file for writing
    FILE *file = fopen("example.txt", "w");
    if (file == NULL) {
        perror("Error opening file for writing");
        return 1;
    }

    // Write to the file
    char str[50] = "hello world\n";
    fwrite(str,sizeof(*str),strlen(str),file);
    fwrite(str,sizeof(*str),strlen(str),file);
    fclose(file);  // Close the file after writing

    // Open the file for reading
    file = fopen("example.txt", "r");
    if (file == NULL) {
        perror("Error opening file for reading");
        return 1;
    }

    // Read from the file
    char buffer[256];
    fread(buffer,sizeof(*buffer),256,file);
    printf("%s", buffer);
    

    // Close the file after reading
    if (fclose(file) != 0) {
        perror("Error closing file");
    }

    return 0;
}
