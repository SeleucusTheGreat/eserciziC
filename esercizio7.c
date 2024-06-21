#include <stdio.h>

// Define a struct with typedef
typedef struct {
    int x;
    int y;
} Point;

int main() {
    // Declare a variable of type Point
    Point p = {3, 4};

    printf("Point: (%d, %d)\n", p.x, p.y); // Outputs: Point: (3, 4)

    return 0;
}
