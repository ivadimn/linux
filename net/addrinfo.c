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

void ipv4_print(struct sockaddr *addr)
{
    char ipv4[INET_ADDRSTRLEN];
    struct sockaddr_in *sa = ( struct sockaddr_in *) addr;
    inet_ntop(AF_INET, &(sa->sin_addr), ipv4, INET_ADDRSTRLEN);
    printf("IPv4 Address: %s\n", ipv4); 
}
void ipv6_print(struct sockaddr *addr)
{
    char ipv6[INET6_ADDRSTRLEN];
    struct sockaddr_in6 *sa = ( struct sockaddr_in6 *) addr;
    inet_ntop(AF_INET6, &(sa->sin6_addr), ipv6, INET6_ADDRSTRLEN);
    printf("IPv4 Address: %s\n", ipv6); 
}

char* what_protocol(int protocol)
{
    switch (protocol)
    {
    case IPPROTO_IP:
        return "IPPROTO_IP";
    case IPPROTO_IPV6:
        return "IPPROTO_IPV6";
    case IPPROTO_ICMP:
        return "IPPROTO_ICMP";    
    case IPPROTO_RAW:
        return "IPPROTO_RAW";
    case IPPROTO_TCP:
        return "IPPROTO_TCP";
    case IPPROTO_UDP:
        return "IPPROTO_UDP";        
    default:
        break;
    }
    return "Unknown protocol";
}

int main(int argc, char** argv)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    struct sockaddr_in *addr4;
    struct sockaddr_in6 *addr6;
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

    rp = result;
    while (rp)
    {
        printf("Cannonical name: %s\n", rp->ai_canonname);

        if (rp->ai_family == AF_INET)
        {
            ipv4_print(rp->ai_addr);
        }
        else if (rp->ai_family == AF_INET6)
        {
            ipv6_print(rp->ai_addr);
        }
        else
        {
            printf("Unknown internet address family\n");
        }
        printf("Protocol: %s\n", what_protocol(rp->ai_protocol));
        rp = rp->ai_next;
        printf("\n");
    }
    freeaddrinfo(result);
}