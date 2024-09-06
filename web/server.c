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
    struct io_uring_cqe* cqes[MAX_CONNECTIONS];

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

    // инициализация io_uring, params передаются пустые и заполняются
    // в случае успешной инициализации
    if(io_uring_queue_init_params(MAX_CONNECTIONS, &ring, &params) < 0)
    {
        err_sys("Error io_uring_queue_init_params: ");
    }

    add_accept(&ring, sock_listen_fd, (struct sockaddr*)&client_addr, &client_len);
    while (1)
    {
        // подписываемся на добавленные в sqe события 
        if(io_uring_submit(&ring) < 0)
            err_sys("Error io_uring_submit: ");

        struct io_uring_cqe* cqe;
        // ожидаем наступление событий на котрые мы подписались в sqe
        // выполненные события помещаются в cqe
        if(io_uring_wait_cqe(&ring, &cqe) < 0)
            err_sys("Error io_uring_submit: ");

        // получаем массив состоявшихся событий в количестве count и кладём их в буфер cqes
        int count = io_uring_peek_batch_cqe(&ring, cqes, MAX_CONNECTIONS);

        for (int i = 0; i < count; i++)
        {
            // значение результата зависит от типа операции
            int result = cqes[i]->res;
            conn_info *ud = (struct conn_info*) cqes[i]->user_data;

            // если пришло новое соединение
            if (ud->type == ACCEPT)
            {
                // добавляем в очередь для чтения клиента
                add_socket_read(&ring, result, MAX_MESSAGE_LEN);
                // серверный сокет снова добавляем в очередь для приёма соединений
                add_accept(&ring, sock_listen_fd, (struct sockaddr*)&client_addr, &client_len);
            }
            else if (ud->type == READ)
            {
                // здесь в result лежит количество прочитанных байт
                if (result <= 0)
                    // если прочитали 0 то закрываем соединение
                    shutdown(ud->fd, SHUT_RDWR);
                else
                    add_socket_write(&ring, ud->fd, result);    
            }
            else if (ud->type == WRITE)
            {
                add_socket_read(&ring, ud->fd, MAX_MESSAGE_LEN);
            }
            
            io_uring_cqe_seen(&ring, cqes[i]);
        }
    }
    return 0;
}

//подготовить серверный сокет к приёму соединенйи
void add_accept(struct io_uring *ring, int fd, struct sockaddr* client_addr, socklen_t* client_len)
{
    // получить экземпляр очереди sqe для заполнения
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring);
    //готовим fd к приёму соединений 
    //в client_addr положится адрес клиента
    io_uring_prep_accept(sqe, fd, client_addr, client_len, 0);
    // записываем общую для sqe и cqe область данных информацию о серверном сокете и его состоянии
    conn_info *conn = &conns[fd];
    conn->fd = fd;
    conn->type = ACCEPT;

    io_uring_sqe_set_data(sqe, conn);
}

void add_socket_read(struct io_uring *ring, int fd, size_t size)
{
    // получить экземпляр очереди sqe для заполнения
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring);
    //готовим fd к чтению 
    // читать будем в bufs - предаллоцированный массиы буферов по количеству соединений
    io_uring_prep_read(sqe, fd, bufs[fd], size, 0);
    // записываем общую для sqe и cqe область данных информацию сокете из которого будем читать
    conn_info *conn = &conns[fd];
    conn->fd = fd;
    conn->type = READ;
    io_uring_sqe_set_data(sqe, conn);
}

void add_socket_write(struct io_uring *ring, int fd, size_t size)
{
    // получить экземпляр очереди sqe для заполнения
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring);
    //готовим fd к записи 
    // писать будем из bufs в котрый раньше читали
    io_uring_prep_write(sqe, fd, bufs[fd], size, 0);
    // записываем общую для sqe и cqe область данных информацию сокете в котрый будем писать
    conn_info *conn = &conns[fd];
    conn->fd = fd;
    conn->type = WRITE;
    io_uring_sqe_set_data(sqe, conn);
}


