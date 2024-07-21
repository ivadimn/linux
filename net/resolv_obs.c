#include <stdio.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char** argv)
{
    struct hostent *he = NULL;
    struct in_addr *addr;

    if (argc != 2)
    {
        printf("Usage %s hostname\n", argv[0]);
        return -1;
    }

    he = gethostbyname(argv[1]);
    if (he == NULL)
    {
        printf("Error gethostbyname\n");
        return -1;
    }
    
    int index = 0;
    while (he->h_addr_list[index])
    {
        addr = (struct in_addr *) he->h_addr_list[index];
        printf("%s\n", inet_ntoa(*addr));
        index++;
    }
    return 0;
}

