#include <stdio.h>
#include <dstring.h>

int main(int argc, char **argv)
{
	DString *s = dstring_new();

	dstring_append(s, "Hello");
	dstring_append(s, "World");

	char *x = dstring_to_string(s);

	printf("%s\n", x);

	free(x);

	dstring_free(s);

	return 0;
}
