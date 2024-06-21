#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // Open the file
    int fd = open("example.txt", O_RDWR);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Get the file size
    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("Error getting the file size");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Memory map the file
    size_t length = sb.st_size;
    void *mapped = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("Error mmapping the file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Access the mapped memory
    char *data = (char *)mapped;
    printf("First character in the file: %c\n", data[0]);

    // Modify the file via memory mapping
    strcpy(data, "Hello, world!");

    // Sync the file with memory
    if (msync(mapped, length, MS_SYNC) == -1) {
        perror("Error syncing file");
        munmap(mapped, length);
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Unmap the memory
    if (munmap(mapped, length) == -1) {
        perror("Error unmapping the file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Close the file
    close(fd);
    return 0;
}
