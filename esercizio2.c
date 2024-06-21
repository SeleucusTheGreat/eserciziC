#include <stdio.h>
#include <string.h>

void main () {
    printf ("first string and the seocon string \n");
    char input[100];
    char str1[50];
    char str2[50];
    fgets(input, 50, stdin);
    sscanf(input, "%s %s" , str1, str2);
    int result = strcmp(str1,str2);
    printf("the first string is %s and the second is %s and the result is %d", str1, str2 , result);
    
    switch (result) {
    case 1:
        printf(" %s is > di %s \n",str1 , str2);
        break;
    case -1:
        printf(" %s is = di %s \n",str1 , str2);
        break;
    
    case 0:
        printf(" %s is = di %s \n",str1 , str2);
        break;
    }

}   