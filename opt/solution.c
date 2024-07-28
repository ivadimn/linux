#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

int main(int argc, char** argv)
{
    int index = 0;
    int opchar = 0;   
    char right = '+';

    struct option opts[] = {
        {"query", required_argument, 0, 'q'},
        {"version", no_argument, 0, 'v'},
        {"longinformationrequest", no_argument, 0, 'i'},
        {0, 0, 0, 0},
    };

    while ((opchar = getopt_long(argc, argv, "q:vi", opts, &index)) != -1)
    {
        switch (opchar)
        {
        case 'i':
            break;
        case 'q':
            if (optarg == NULL)
                right = '-';            
            break;
        case 'v':
            break;    
        default:
            right = '-';
        }
    }
    
    printf("%c\n", right);
    
}