#include <netdb.h>
#include <stdio.h>


int main(int argc, char** argv) 
{
    if (argc != 2)
    {
        printf("Usage %s domaun_name\n", argv[0]);
        return -1;
    }
    
    struct hostent *h;
    h = gethostbyname(argv[1]);

    if (h == NULL)
    {
        printf("Error!\n");
        return -1;
    }

    printf("Cannonical name: %s\n", h->h_name);
    printf("Type = %s, len = %d\n", (h->h_addrtype == AF_INET) ? "IPV4" : "IPV6", h->h_length);
    
}



