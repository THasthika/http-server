#include <stdlib.h>
#include <string.h>

#include "dstring.h"

DString* dstring_new()
{
	DString *ret = malloc(sizeof(DString));
	ret->head = NULL;
	ret->size = 0;
	return ret;
}

void dstring_free(DString *string)
{
	dstring_clear(string);
	free(string);
}

void dstring_clear(DString *string)
{
	struct DStringNode *node = string->head;
	if(node == NULL) return;
	struct DStringNode *next = NULL;
	while(node != NULL) {
		next = node->next;
		if(node->block != NULL)
			free(node->block);
		free(node);
		node = next;
	}
}

void dstring_prepend(DString *string, char *str)
{
	dstring_nprepend(string, str, strlen(str));
}

void dstring_nprepend(DString *string, char *str, size_t len)
{
	struct DStringNode *node;
	node = malloc(sizeof(struct DStringNode));

	node->size = len;
	node->next = string->head;
	node->block = malloc(sizeof(char) * len);

	memcpy(node->block, str, len);

	string->size += node->size;

	string->head = node;
}

void dstring_append(DString *string, char *str)
{
	dstring_nappend(string, str, strlen(str));
}

void dstring_nappend(DString *string, char *str, size_t len)
{
	struct DStringNode *node, *prev;
	prev = string->head;

	while(prev != NULL && prev->next != NULL) {
		prev = prev->next;
	}

	node = malloc(sizeof(struct DStringNode));

	node->size = len;
	node->next = NULL;
	node->block = malloc(sizeof(char) * len);

	memcpy(node->block, str, len);

	string->size += node->size;

	if(prev != NULL)
		prev->next = node;
	else
		string->head = node;
}

char* dstring_to_string(DString *string)
{
	char *str = malloc(string->size + 1);
	str[string->size] = '\0';

	struct DStringNode *node = string->head;
	char *p = str;
	while(node != NULL) {
		if(node->block != NULL) {
			memcpy(p, node->block, node->size);
			p += node->size;
		}
		node = node->next;
	}

	return str;
}
