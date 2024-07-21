#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    char buf[BUFSIZ];
    struct sockaddr_in addr, client_addr;
    int sz = 0;
    int sock_client;


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
  		prinyf("Ошибка начала прослушивания сокета.\n");
        return -1;
 	}
    
    while (1)
    {
        sock_client = accept(sock_serv, (struct sockaddr*) &client_addr, sizeof(client_addr);)
        sz = read(sock_serv, buf, BUFF_SIZE);
        buf[sz] = 0;
        if (strcmp(buf, "OFF\n") == 0)
            break;
        printf("%s\n", buf);
    }
    close(sock_serv);    
    return 0;

}
