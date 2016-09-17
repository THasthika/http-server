#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "request_handler.h"

#include "dstring.h"

#define BUFFER_SIZE 512

void handle_request(int fd)
{
	ssize_t count;
	char buffer[BUFFER_SIZE];
	DString *str = dstring_new();

	while(1) {
		if((count = read(fd, buffer, sizeof(buffer))) == -1) {
			if(errno != EAGAIN) {
				perror("read");
				goto cleanup;
			}
			break;
		}
		if(count == 0) {
			break;
		}
		dstring_nappend(str, buffer, count);
	}

	char *request_raw = dstring_to_string(str);

	printf("%s\n", request_raw);

	send(fd, request_raw, strlen(request_raw), 0);

	printf("closing connection %d\n", fd);
	close(fd);

cleanup:
	dstring_free(str);
}
