#include "gjb.h"
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

gjb_file_t gjb_file_create(gjb_header_t header, gjb_manifest_t manifest) {
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


unsigned int gjb_header_write(gjb_header_t header, FILE *stream) {
	rewind(stream);
	size_t write_ret = fwrite((void *)header, sizeof(struct gjb_header), 1, stream);
	
	return write_ret;
}

gjb_header_t gjb_header_read(FILE *stream) {
	rewind(stream);
	gjb_header_t header = calloc(1, sizeof(struct gjb_header));

	size_t read_ret = fread((void *)header, sizeof(struct gjb_header), 1, stream);
	if(!read_ret) return NULL;
	
	return header;
}

gjb_header_t gjb_header_create(char *name, char *author, char *description, u_int64_t entry_count) {
	gjb_header_t header = calloc(1, sizeof(struct gjb_header));
	
	strncpy(header->name, name, HEADER_STR_MAX);
	strncpy(header->author, author, HEADER_STR_MAX);
	strncpy(header->description, description, HEADER_STR_MAX);
	header->entry_count = entry_count;
	
	return header;
}

void gjb_header_release(gjb_header_t header) {
	free(header);
}

gjb_manifest_t gjb_manifest_read(FILE *stream, gjb_file_t file) {
	gjb_manifest_t manifest;
	
	if(!stream) return NULL;
	if(!file->header) return NULL;
	
	fseek(stream, sizeof(struct gjb_header), SEEK_SET);
	u_int64_t i;
	
	for(i=0;i<file->header->entry_count;++i) {
		struct gjb_manifest_entry *entry;
		size_t read_ret = fread(entry, sizeof(struct gjb_manifest_entry), 1, stream);
		if(!read_ret) return NULL;
		
		manifest[i] = calloc(1, sizeof(struct gjb_manifest_entry));
		memcpy(manifest[i], entry, sizeof(struct gjb_manifest_entry));
		
		free(entry);
	}
	
	return manifest;
}

unsigned int gjb_manifest_write(FILE *stream, gjb_manifest_t manifest, gjb_file_t file) {
	if(!stream || !manifest || !file) return 0;
	if(!file->header) return 0;
	
	fseek(stream, sizeof(struct gjb_header), SEEK_SET);
	u_int64_t i;
	for(i=0;i<file->header->entry_count;++i) {
		size_t write_ret = fwrite(manifest[i], sizeof(struct gjb_manifest_entry), 1, stream);
		if(!write_ret) return 0;
	}
	
	return 1;
}

unsigned int gjb_manifest_add_entry(gjb_manifest_t manifest, struct gjb_manifest_entry *entry, gjb_header_t header) {
	if(!entry || !header) return 0;
	
	u_int64_t i = header->entry_count;
	printf("entry: %d\n", i);
	manifest[i] = calloc(1, sizeof(struct gjb_manifest_entry));
	printf("allocated %d bytes\n", sizeof(struct gjb_manifest_entry));
	memcpy(manifest[i], entry, sizeof(struct gjb_manifest_entry));
	printf("Memory copied to manifest\n");
	
	return 1;
}

struct gjb_manifest_entry *gjb_manifest_entry_create(char *name, u_int64_t size) {
	struct gjb_manifest_entry *entry = calloc(1, sizeof(struct gjb_manifest_entry));
	strncpy(entry->name, name, 128);
	entry->size = size;
	
	return entry;
}

void gjb_manifest_entry_release(struct gjb_manifest_entry *entry) {
	free(entry);
}
