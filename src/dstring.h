#ifndef DSTRING_H
#define DSTRING_H 1

#include <stdlib.h>

struct DStringNode {
	size_t size;
	char *block;
	struct DStringNode *next;
};

typedef struct {
	size_t size;
	struct DStringNode *head;
} DString;


DString* dstring_new();
void dstring_free(DString *string);

void dstring_clear(DString *string);

void dstring_prepend(DString *string, char *str);
void dstring_nprepend(DString *string, char *str, size_t len);

void dstring_append(DString *string, char *str);
void dstring_nappend(DString *string, char *str, size_t len);

char* dstring_to_string(DString *string);

#endif
