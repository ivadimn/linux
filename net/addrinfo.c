/**
 * @file addrinfo.c
 * @author ivadimn.ru
 * @brief 
 * @version 0.1
 * @date 2024-09-25
 * 
 * @copyright Copyright (c) 2024
 * программа определяет перечень возможныз ip адресов
 * по доменному шмени
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

/**
 * основная структура для работы с функцией getaddrinfo
 struct addrinfo {
    int ai_flags;     дополнительные флаги
    int ai_family;
    int ai_socktype;
    int ai_protocol;
    socklen_t ai_addrlen;
    struct sockaddr *ai_addr;
    char *ai_canonname;
    struct addrinfo *ai_next;
 };
 */

#ifndef AI_ALL
#define AI_ALL 0x0100   // для поля 
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


/*
 выдаёт тип импользуемого протокола
 уровня IP
*/
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

/*
 выдаёт тип сокета
 
*/
char* what_socket(int socket)
{
    switch (socket)
    {
    case 0:
        return "Any socket type";
    case SOCK_STREAM:
        return "SOCK_STREAM";
    case SOCK_DGRAM:
        return "SOCK_DGRAM";    
           
    default:
        break;
    }
    return "Unknown socket";
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

    //передвызовом hints обнуляется задаются только флаги ai_flags
    //
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
        printf("Socket: %s\n", what_socket(rp->ai_socktype));
        rp = rp->ai_next;
        printf("\n");
    }
    freeaddrinfo(result);
}