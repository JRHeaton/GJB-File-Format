#ifndef GJB_MANIFEST_H
#define GJB_MANIFEST_H

#include <sys/types.h>
#include "file_structure.h"
#include "header.h"

struct {
	char name[128];
	u_int64_t size;
} gjb_manifest_entry;

typedef struct gjb_manifest_entry **gjb_manifest;

gjb_manifest gjb_manifest_read(FILE *stream, gjb_file_t file);
unsigned int gjb_manifest_write(FILE *stream, gjb_manifest manifest, gjb_file_t file); 
void gjb_manifest_add_entry(gjb_manifest manifest, struct gjb_manifest_entry *entry, gjb_header_t header);

struct gjb_manifest_entry *gjb_manifest_entry_create(char *name, u_int64_t size);
void gjb_manifest_entry_release(struct gjb_manifest_entry *entry);

#endif