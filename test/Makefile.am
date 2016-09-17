AM_CFLAGS = -std=gnu11 -g3 -I../src/
AM_LDFLAGS = -g3 -L../src/

check_PROGRAMS = http_request_parser\
		 check_dstring

TESTS = $(check_PROGRAMS)

http_request_parser_SOURCES = http_request_parser.c
http_request_parser_CFLAGS = $(AM_CFLAGS)
http_request_parser_LDFLAGS = $(AM_LDFLAGS)
http_request_parser_LDADD = ../src/dstring.o

check_dstring_SOURCES = check_dstring.c
check_dstring_CFLAGS = $(AM_CFLAGS)
check_dstring_LDFLAGS = $(AM_LDFLAGS)
check_dstring_LDADD = ../src/dstring.o

# $(datadir) /usr/local/share
