#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "request_handler.h"

void handle_request(int fd)
{
	ssize_t count;
	char buffer[512];
	int done = 0;
	while(1) {
		if((count = read(fd, buffer, sizeof buffer)) == -1) {
			if(errno != EAGAIN) {
				perror("read");
				done = 1;
			}
			break;
		}
		if(count == 0) {
			done = 1;
			break;
		}
		if(write(fd, buffer, count) == -1) {
			perror("write");
			done = 1;
			break;
		}
	}

	if(done) {
		printf("closing connection %d\n", fd);
		close(fd);
	}
}
