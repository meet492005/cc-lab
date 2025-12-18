#include <stdio.h>
#include <string.h>

int main() {
    char str[100];
    int i = 0;
    int len;

    printf("Enter the string: ");
    fgets(str, sizeof(str), stdin);

    //comments 1
    str[strcspn(str, "\n")] = '\0';

    len = strlen(str);

    //comments 2
    if (len < 2) 
    {
        printf("Invalid String\n");
        return 0;
    }


    while (str[i] == 'a') 
    {
        i++;
    }

    
    if (str[i] == 'b' && str[i + 1] == 'b' && i + 2 == len) 
    {
        printf("Valid String\n");
    } 
    else 
    {
        printf("Invalid String\n");
    }

    return 0;
}
