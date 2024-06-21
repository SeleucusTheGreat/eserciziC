#include <stdio.h>
#include <string.h>

void main () {
    printf ("hello world \n");
    char name[50];
    char result[50];
    int number;
    fgets(name, 50, stdin);
    sscanf(name, "%d %s" , &number, result);
    printf ("you wrote %s and the number is %d \n", result , number);
}