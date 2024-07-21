#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#ifndef AI_ALL
#define AI_ALL 0x0100
#endif

#define ADDR_LEN 128


int main(int argc, char** argv)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    char address[ADDR_LEN];
    int s;
    
    if (argc != 2)
    {
        printf("Usage %s hostname\n", argv[0]);
        return -1;
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_flags = AI_ALL;
    s = getaddrinfo(argv[1], NULL, &hints, &result);

    if (s != 0)
    {
        printf("Error resolving %s: %s\n", argv[1], gai_strerror(h_errno));
        return -1;
    }
    printf("Remote address is:\n");

    rp = result;
    do
    {
        getnameinfo(rp->ai_addr, rp->ai_addrlen,
                    address, ADDR_LEN, 
                    0, 0, 
                    NI_NUMERICHOST);
        printf("\t%s\n", address);            
    } while ((rp = rp->ai_next));
    
    freeaddrinfo(result);
}