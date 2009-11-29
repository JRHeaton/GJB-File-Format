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

unsigned int gjb_file_write(FILE *stream, gjb_file_t file) {
	if(!stream || !file) return 0;
	
	gjb_header_write(file->header, stream);
	gjb_manifest_write(stream, file->manifest, file);
	
	return 1;
}
