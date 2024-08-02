#include "log.h"
#include <liburing.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BACKLOG 512
#define MAX_CONNECTIONS 4096
#define MAX_MESSAGE_LEN 4096

void add_accept(struct io_uring *ring, int fd, struct sockaddr* client_addr, socklen_t* client_len);

void add_socket_read(struct io_uring *ring, int fd, size_t size);

void add_socket_write(struct io_uring *ring, int fd, size_t size);


typedef struct conn_info {
    int fd;
    unsigned type;
} conn_info;

enum {
    ACCEPT, READ, WRITE
};

conn_info conns[MAX_CONNECTIONS];

char bufs[MAX_CONNECTIONS][MAX_MESSAGE_LEN];


int main(int argc, char** argv)
{
    struct sockaddr_in server_addr = {0}, client_addr;
    int sock_listen_fd = 0;

    if (argc != 2)
        err_quit("Please give a port number: ./eserv port_number\n");

    int portno = strtol(argv[1], NULL, 10);
        
    socklen_t client_len = sizeof(client_addr);

    const int val = 1;   
    //создаём серверный сокет, соединения на который будем слушать
    sock_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_listen_fd < 0)
        err_sys("Error creating socket:");

    setsockopt(sock_listen_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    
    //задаём порт, тип адреса IP4, адрес для привязки - любой локальный адрес
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portno);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    //привязываем сокет с адресу сервера
    if (bind(sock_listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
        err_sys("Error binding socket: ");

    //начинаем прослушивать порт
    if (listen(sock_listen_fd, BACKLOG) < 0)
        err_sys("Error listening: ");

    printf("eserv echo server listening for connection on port: %d\n", portno);

    struct io_uring ring;
    struct io_uring_params params;
    memset(&params, 0, sizeof(params));

    if(io_uring_queue_init_params(4096, &ring, &params) < 0)
    {
        err_sys("Error io_uring_queue_init_params: ");
    }

    add_accept(&ring, sock_listen_fd, &client_addr, &client_len);
    while (1)
    {
        /* code */
    }
    

}

void add_accept(struct io_uring *ring, int fd, struct sockaddr* client_addr, socklen_t* client_len)
{
    struct io_uring_seq *sqe = io_uring_get_sqe(&ring);
    io_uring_prep_accept(sqe, fd, client_addr, client_len, 0);
    conn_info *conn = &conns[fd];
    conn->fd = fd;
    conn->type = ACCEPT;

    io_uring_sqe_set_data(sqe, conn);
}

void add_socket_read(struct io_uring *ring, int fd, size_t size)
{

}

void add_socket_write(struct io_uring *ring, int fd, size_t size)
{

}


