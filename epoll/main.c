
#include "log.h"
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BACKLOG 512
#define MAX_EVENTS 256
#define MAX_MESSAGE_LEN 4096

char buffer[MAX_MESSAGE_LEN];

/*
* установить неблокирующий режим для сокета
*/
int set_non_blocking(int sock) {
	 int opts;

 	opts = fcntl(sock,F_GETFL);
 	if (opts < 0) {
  		perror("fcntl(F_GETFL)");
  		return -1;
 	}
 	opts = (opts | O_NONBLOCK);
 	if (fcntl(sock,F_SETFL,opts) < 0) {
  		perror("fcntl(F_SETFL)");
  		return -1;
 	}

 return 0;
}

/*
* прочитать данные из сокета
*/
void do_read(int fd) {
	int rc = recv(fd, buffer, sizeof(buffer), 0);
 	if (rc < 0)  {
  		perror("read");
  		return;
 	}
 	buffer[rc] = 0;
 	printf("read: %s\n", buffer);
}

/*
* записать данные в сокет
*/
void do_write(int fd) {
	static const char* greeting = "O hai!\n";
 	int rc = send(fd, greeting, strlen(greeting), 0);
 	if (rc < 0) {
  		perror("write");
  		return;
 	}
}

void process_error(int fd)  {
	printf("fd %d error!\n", fd);
}

int main(int argc, char** argv)
{
    struct sockaddr_in server_addr = {0}, client_addr;
    int sock_listen_fd = 0;

    if (argc != 2)
        err_quit("Please give a port number: ./eserv port_number\n");

    int portno = strtol(argv[1], NULL, 10);
        
    socklen_t client_len = sizeof(client_addr);
    memset(buffer, 0, sizeof(buffer));
    
    //создаём серверный сокет, соединения на который будем слушать
    sock_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_listen_fd < 0)
        err_sys("Error creating socket:");

    set_non_blocking(sock_listen_fd);    
    
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

    struct epoll_event ev, events[MAX_EVENTS];
    int new_events, sock_conn_fd, epollfd;

    epollfd = epoll_create(MAX_EVENTS);
    if (epollfd < 0)
        err_sys("Error creating epoll: ");
    
    ev.events = EPOLLIN;
    ev.data.fd = sock_listen_fd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sock_listen_fd, &ev))
        err_sys("Error adding new listening socket to epoll:");
    
    while (1)
    {
        new_events = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (new_events == -1)
            err_sys("Error in epoll_wait: ");
        
        for (int i = 0; i < new_events; i++)
        {
            if (events[i].data.fd == sock_listen_fd)
            {
                sock_conn_fd = accept(sock_listen_fd, (struct sockaddr*)&client_addr, &client_len);
                if (sock_conn_fd == -1)
                    err_sys("Error accepting new connection: ");

                set_non_blocking(sock_conn_fd)    ;
                ev.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP;
                ev.data.fd = sock_conn_fd;  
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sock_conn_fd, &ev) < 0) {
     				err_quit("epoll_ctl: ");
     				close(sock_conn_fd);
     				continue;
    			}
            }
            else
            {
                int newsockfd = events[i].data.fd;
                if (events[i].events & EPOLLIN)    //читаем если что-то пришло
                {
     				do_read(newsockfd);
                }

    			if (events[i].events & EPOLLOUT)	//или пишем если надо что-то заисать
     				do_write(newsockfd);

    			if (events[i].events & EPOLLRDHUP)
     				process_error(newsockfd);

                /*int rc = recv(newsockfd, buffer, MAX_MESSAGE_LEN, 0);
                if (rc <= 0)
                {
                    printf("Empty\n");
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, newsockfd, NULL);
                    shutdown(newsockfd, SHUT_RDWR);
                }
                else
                {
                    send(newsockfd, buffer, rc, 0);
                }*/
                
            }
        }
        
    }

} 