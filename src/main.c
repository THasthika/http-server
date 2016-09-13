#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>

#include <netdb.h>

#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "request_handler.h"

#define SOCKET_CREATE	0x01
#define SOCKET_BIND	0x02
#define SOCKET_CONNECT	0x04
#define SOCKET_REUSE	0x08

#define SOCKET_TCP	SOCK_STREAM
#define SOCKET_UDP	SOCK_DGRAM

#define PORT "8080"

#define MAXEVENTS 64

char exit_loop = 0;

int socket_create(int *sockfd, char *hostname, char *port, int socktype, int flags);
int socket_non_blocking(int sockfd);

void handle_new_connections(int sockfd, int epollfd);

void sig_handler(int signo)
{
	if(signo == SIGINT)
		exit_loop = 1;
}

int main(int argc, char **argv)
{
	int sockfd, epollfd;

	struct epoll_event event;
	struct epoll_event *events;

	if(signal(SIGINT, sig_handler) == SIG_ERR) {
		fprintf(stderr, "signal not handled\n");
		return -1;
	}

	if(socket_create(&sockfd, NULL, PORT, SOCKET_TCP, SOCKET_CREATE | SOCKET_BIND | SOCKET_REUSE) == -1)
		return -1;


	if(socket_non_blocking(sockfd) == -1)
		return -1;

	if(listen(sockfd, SOMAXCONN) == -1) {
		perror("listen");
		return -1;
	}

	if((epollfd = epoll_create1(0)) == -1) {
		perror("epoll_create1");
		return -1;
	}

	event.data.fd = sockfd;
	event.events = EPOLLIN | EPOLLET;

	if(epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &event) == -1) {
		perror("epoll_ctl");
		return -1;
	}

	events = calloc(MAXEVENTS, sizeof event);

	int n, i;

	while(!exit_loop)
	{
		n = epoll_wait(epollfd, events, MAXEVENTS, 1000);
		/*printf("%d events\n", n);*/
		for(i = 0; i < n; i++) {
			if((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || !(events[i].events & EPOLLIN)) {
				fprintf(stderr, "epoll error\n");
				close(events[i].data.fd);
				continue;
			} else if(sockfd == events[i].data.fd) {
				handle_new_connections(sockfd, epollfd);
			} else {
				handle_request(events[i].data.fd);
			}
		}
	}

	free(events);

	close(sockfd);

	return 0;
}

int socket_create(int *sockfd, char *hostname, char *port, int socktype, int flags)
{
	struct addrinfo hints, *results, *p;
	int rv;
	int yes = 1;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = socktype;

	if(hostname == NULL)
		hints.ai_flags = AI_PASSIVE;
	if((rv = getaddrinfo(hostname, port, &hints, &results)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return -1;
	}

	for(p = results; p != NULL; p = p->ai_next) {
		if(flags & SOCKET_CREATE &&\
				(*sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("socket");
			continue;
		}
		if(flags & SOCKET_REUSE &&\
				setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("setsockopt");
			if(flags & SOCKET_CREATE)
				close(*sockfd);
			continue;
		}
		if(flags & SOCKET_BIND &&\
				bind(*sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			perror("bind");
			if(flags & SOCKET_CREATE)
				close(*sockfd);
			continue;
		}
		if(flags & SOCKET_CONNECT &&\
				connect(*sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			perror("connect");
			if(flags & SOCKET_CREATE)
				close(*sockfd);
			continue;
		}
		break;
	}

	if(p == NULL) {
		fprintf(stderr, "could not config socket\n");
		return -1;
	}

	freeaddrinfo(results);
	return 0;
}

int socket_non_blocking(int sockfd)
{
	int flags;
	if((flags = fcntl(sockfd, F_GETFL, 0)) == -1) {
		perror("fcntl");
		return -1;
	}

	flags |= O_NONBLOCK;
	if(fcntl(sockfd, F_SETFL, flags) == -1) {
		perror("fcntl");
		return -1;
	}

	return 0;
}

void handle_new_connections(int sockfd, int epollfd)
{
	struct epoll_event event;
	struct sockaddr in_addr;
	socklen_t in_len;
	int fd;
	int rv;
	char hostbuf[NI_MAXHOST], servbuf[NI_MAXSERV];
	
	while(1) {
		in_len = sizeof in_addr;
		if((fd = accept(sockfd, &in_addr, &in_len)) == -1) {
			if((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
				break;
			} else {
				perror("accept");
				break;
			}
		}
		
		if((rv = getnameinfo(&in_addr, in_len, hostbuf, sizeof hostbuf, servbuf, sizeof servbuf, NI_NUMERICHOST | NI_NUMERICSERV)) != 0) {
			fprintf(stderr, "getnameinfo: %s\n", gai_strerror(rv));
			break;
		}

		printf("Accepted from %d (host=%s, port=%s)\n", fd, hostbuf, servbuf);

		if(socket_non_blocking(fd) == -1)
			break;

		event.data.fd = fd;
		event.events = EPOLLIN | EPOLLET;

		if(epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event) == -1) {
			perror("epoll_ctl");
			break;
		}
	}
}
