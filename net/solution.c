#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define BAACKLOG 5

int func(const void *arg1, const void* arg2)
{
    return *(char*)arg2 - *(char*) arg1;
}

int main(int argc, char** argv)
{
    char buf[BUFSIZ];
    struct sockaddr_in addr, client_addr;
    int sz = 0;
    int sock_client;
    socklen_t client_len;


    if (argc != 2)
    {
        printf("Usage %s port\n", argv[0]);
        return -1;
    }

    int port = atoi(argv[1]);

    int sock_serv = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_serv < 0)
    {
        printf("Error create socket.\n");
        return -1;
    }
    memset(&addr, 0, sizeof(addr));
    addr.sin_family= AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    if (bind(sock_serv, (struct sockaddr*) &addr, sizeof(addr)) < 0)
    {
         printf("Error bind address.\n");
        return -1;
    }

    if (listen(sock_serv, 5) < 0)  {
  		printf("Ошибка начала прослушивания сокета.\n");
        return -1;
 	}
    client_len = sizeof(client_addr);
    sock_client = accept(sock_serv, (struct sockaddr*) &client_addr, &client_len);
    while (1)
    {
        sz = read(sock_client, buf, BUFSIZ);
        if (buf[sz-1] == '\n')
            buf[sz-1] = 0;
        if (strcmp(buf, "OFF") == 0)
            break;
        qsort(buf, strlen(buf), sizeof(char), func);
        write(sock_client, buf, strlen(buf) + 1);
    }
    close(sock_serv);    
    
    return 0;


}
