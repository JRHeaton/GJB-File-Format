#include "header.h"

unsigned int gjb_header_write(gjb_header_t header, FILE *stream) {
	rewind(stream);
	size_t write_ret = fwrite((void *)header, sizeof(struct gjb_header), 1, stream);
	
	return write_ret;
}

gjb_header_t gjb_header_read(FILE *stream, gjb_file_t file) {
	rewind(stream);
	gjb_header_t header;
	
	size_t read_ret = fread((void *)header, sizeof(gjb_header), 1, stream);
	if(!read_ret) return NULL;
	
	file.header = header;
	
	return header;
}

gjb_header_t gjb_header_create(char *name, char *author, char *description, u_int64_t entry_count) {
	gjb_header_t header = calloc(1, sizeof(gjb_header));

	strncpy(header->name, name, HEADER_STR_MAX);
	strncpy(header->author, author, HEADER_STR_MAX);
	strncpy(header->description, description, HEADER_STR_MAX);
	header->entry_count = entry_count;
	
	return header;
}

unsigned int gjb_header_release(gjb_header_t header) {
	free(header);
}