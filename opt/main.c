#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

int main(int argc, char** argv)
{
    int is_brackets = 0;
    int opindex = 0;
    int opchar = 0;
    char string[] = "Und wenn du lange in enian Abgrund, blickt der Abgrund auch in dich hinein";
    int length = 0;

    struct option opts[] = {
        //set flag example
        {"brackets", no_argument, &is_brackets, 1},
        //option args
        {"length", required_argument, 0, 'l'},
        //longname options
        {"authornameshouldbeprinted", no_argument, 0, 'a'},
        {0, 0, 0, 0},
    };

    while ((opchar = getopt_long(argc, argv, "bl:aq", opts, &opindex)) != -1)
    {
        switch (opchar)
        {
        case 0:
            printf("0\n");
            break;
        case 'l':
            length = atoi(optarg);
            printf("length = %d\n", length);
            break;
        case 'a':
            printf("(c) Friedich Nietzsche\n");
            break;    
        default:
            printf("optchar: %c\n", opchar);
        }
    }
    
    if (strlen(string) > length && length != 0)
        string[length] = '\0';

    if (is_brackets)
        printf("[%s]\n", string);
    else 
        printf("%s\n", string);

    printf("Done\n");
    
}