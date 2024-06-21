#include <stdio.h>

struct Operation {
    void (*operate)(int, int);
};

void add(int a, int b) {
    printf("Sum: %d\n", a + b);
}

void multiply(int a, int b) {
    printf("Product: %d\n", a * b);
}

int main() {
    // Create struct instances
    struct Operation op_add = { add };
    struct Operation op_multiply = { multiply };

    // Use the function pointers in the structs
    op_add.operate(5, 3);
    op_multiply.operate(5, 3);

    return 0;
}
