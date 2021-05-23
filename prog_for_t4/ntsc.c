#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int digit_cnt(char *arg)
{
    int counter = 0;
    int j = 0;
    char c;
    while (arg != NULL)
    {
        c = *(arg + j);
        if (c == '\0') break;
        
        if (c >= '0' && c <= '9')
            counter++;
        
        j++;
        
    }
    return counter;

}

int main(int argc, char **argv)
{
    int i;
    char *curr_arg;
    int counter = 0;

    if (argc == 1) return 0;

    for (i = 1; i<argc; i++)
    {
        counter = counter + digit_cnt(argv[i]);
        
            
    }
    printf("%d\n", counter);
    return 0;

}