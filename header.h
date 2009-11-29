#ifndef GJB_HEADER_H
#define GJB_HEADER_H

#include <sys/types.h>
#include "file_structure.h"
#include <stdio.h>

struct {
	char name[128];
	char author[128];
	char description[128];
	u_int64_t entry_count;
} gjb_header;

typedef struct gjb_header *gjb_header_t;

#define HEADER_STR_MAX 128

unsigned int gjb_header_write(gjb_header_t header, FILE *stream);
unsigned int gjb_header_read(FILE *stream, gjb_file_t file);
gjb_header_t gjb_header_create(char *name, char *author, char *description, u_int64_t entry_count);
void gjb_header_release(gjb_header_t header);

#endif