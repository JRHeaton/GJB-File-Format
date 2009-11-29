#include "file_structure.h"

gjb_file_t gjb_file_create(struct gjb_header *header, gjb_manifest manifest) {
	gjb_file_t file = calloc(1, sizeof(struct gjb_file));
	if(!file) return NULL;
	
	file->header = header;
	file->manifest = manifest;
	
	return file;
}

void gjb_file_release(gjb_file_t file) {
	free(file);
}