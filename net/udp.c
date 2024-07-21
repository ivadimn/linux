#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUFF_SIZE (5 * 1024)

int main(int argc, char** argv)
{
    char buf[BUFF_SIZE];
    struct sockaddr_in addr;
    int sz = 0;


    if (argc != 2)
    {
        printf("Usage %s hostname\n", argv[0]);
        return -1;
    }

    int port = atoi(argv[1]);

    int sock_udp = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_udp < 0)
    {
        printf("Error create socket.\n");
        return -1;
    }
    memset(&addr, 0, sizeof(addr));
    addr.sin_family= AF_INET;
    inet_aton("127.0.0.1", &addr.sin_addr);
    addr.sin_port = htons(port);
    if (bind(sock_udp, (struct sockaddr*) &addr, sizeof(addr)) < 0)
    {
         printf("Error bind address.\n");
        return -1;
    }
    
    while (1)
    {
        sz = read(sock_udp, buf, BUFF_SIZE);
        buf[sz] = 0;
        if (strcmp(buf, "OFF\n") == 0)
            break;
        printf("%s\n", buf);
    }
    close(sock_udp);    
    return 0;

}