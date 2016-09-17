#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include <dstring.h>

#define REQUEST_FILE "http_request.raw"
#define BUFFER_SIZE 512

int main(int argc, char **argv)
{
	char buffer[BUFFER_SIZE];

	DString *string = dstring_new();

	int fd = open(REQUEST_FILE, 0, O_RDONLY);

	int rc = 0;

	while((rc = read(fd, buffer, BUFFER_SIZE)) > 0) {
		dstring_nappend(string, buffer, rc);
	}

	char *x = dstring_to_string(string);

	printf("%s\n", x);

	free(x);


	dstring_free(string);
	close(fd);
}
