#include <stdio.h>
#include <string.h>

enum Weekday {
    Sunday,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday
};

int main() {
    enum Weekday today;
    today = Wednesday;

    if (today == Wednesday) {
        printf("Today is Wednesday\n");
    }

    return 0;
}