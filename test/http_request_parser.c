#include <stdlib.h>
#include <stdio.h>

#define REQUEST_FILE "http_request.raw"

int main(int argc, char **argv)
{
	FILE *fp = fopen(REQUEST_FILE, "r");



	fclose(fp);
}
