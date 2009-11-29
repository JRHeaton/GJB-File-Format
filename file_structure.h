#ifndef GJB_FILE_STRUCTURE_H
#define GJB_FILE_STRUCTURE_H

#include "header.h"
#include "manifest.h"

struct {
	struct gjb_header *header;
	gjb_manifest manifest;
} gjb_file;

typedef struct gjb_file *gjb_file_t;

gjb_file_t gjb_file_create(struct gjb_header *header, gjb_manifest manifest);
void gjb_file_release(gjb_file_t file);

unsigned int gjb_file_write(FILE *stream, gjb_file_t file);

#endif 