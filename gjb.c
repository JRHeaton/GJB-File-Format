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
	gjb_manifest_write(stream, file->manifest, file->header);
	
	u_int64_t manifest_size = (file->manifest->count * sizeof(struct gjb_manifest_entry));
	u_int64_t offset = (sizeof(struct gjb_header) + manifest_size);
	fseek(stream, offset, SEEK_SET);
	
	u_int64_t i;
	for(i=0;i<file->header->entry_count;++i) {
		size_t write_ret = fwrite(file->files[i], file->manifest->entries[i].size, 1, stream);
		if(!write_ret) return 0;
	}
	
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

gjb_header_t gjb_header_create(char *name, char *author, char *description) {
	gjb_header_t header = calloc(1, sizeof(struct gjb_header));
	
	strncpy(header->name, name, GJB_STR_MAX);
	strncpy(header->author, author, GJB_STR_MAX);
	strncpy(header->description, description, GJB_DESC_MAX);
	header->entry_count = 0;
	
	return header;
}

void gjb_header_release(gjb_header_t header) {
	free(header);
}

gjb_manifest_t gjb_manifest_read(FILE *stream, gjb_header_t header) {
	if(!stream || !header) return NULL;
	
	gjb_manifest_t manifest = calloc(1, sizeof(gjb_manifest_t));
	manifest->entries = calloc(1, sizeof(struct gjb_manifest_entry) * header->entry_count);
	manifest->count = header->entry_count;
	
	if(!stream) return NULL;
	if(!header) return NULL;
	
	fseek(stream, sizeof(struct gjb_header), SEEK_SET);

	size_t read_ret = fread(manifest->entries, sizeof(struct gjb_manifest_entry) * header->entry_count, 1, stream);
	if(!read_ret) return NULL;
	
	return manifest;
}

gjb_manifest_t gjb_manifest_create() {
	gjb_manifest_t manifest = calloc(1, sizeof(struct gjb_manifest));
	manifest->entries = malloc(sizeof(struct gjb_manifest_entry));
	manifest->count = 0;
	
	return manifest;
}

void gjb_manifest_release(gjb_manifest_t manifest) {
	if(!manifest) return;
	
	free(manifest->entries);
	free(manifest);
}

unsigned int gjb_manifest_write(FILE *stream, gjb_manifest_t manifest, gjb_header_t header) {
	if(!stream || !manifest || !header) return 0;
	
	fseek(stream, sizeof(struct gjb_header), SEEK_SET);
	
	size_t write_ret = fwrite(manifest->entries, sizeof(struct gjb_manifest_entry) * manifest->count, 1, stream);
	if(!write_ret) return 0;
	
	return 1;
}

unsigned int gjb_manifest_add_entry(gjb_manifest_t manifest, struct gjb_manifest_entry *entry, gjb_header_t header) {
	if(!entry || !header) return 0;
	
	manifest->count++;
	manifest->entries = realloc(manifest->entries, manifest->count * sizeof(struct gjb_manifest_entry));
	manifest->entries[manifest->count-1] = *entry;
	
	return 1;
}

struct gjb_manifest_entry *gjb_manifest_entry_create(char *name, u_int64_t size) {
	struct gjb_manifest_entry *entry = calloc(1, sizeof(struct gjb_manifest_entry));
	strncpy(entry->name, name, GJB_STR_MAX);
	entry->size = size;
	
	return entry;
}

void gjb_manifest_entry_release(struct gjb_manifest_entry *entry) {
	free(entry);
}

gjb_file_t gjb_file_read(FILE *stream) {
	if(!stream) return NULL;
	
	gjb_file_t file = calloc(1, sizeof(struct gjb_file));
	file->header = gjb_header_read(stream);
	file->manifest = gjb_manifest_read(stream, file->header);
	file->files = malloc(sizeof(unsigned char *));
	
	u_int64_t manifest_size = (file->manifest->count * sizeof(struct gjb_manifest_entry));
	u_int64_t offset = (sizeof(struct gjb_header) + manifest_size);
	fseek(stream, offset, SEEK_SET);
	
	u_int64_t i;
	for(i=0;i<file->header->entry_count;++i) {
		file->files[i] = malloc(file->manifest->entries[i].size);
		size_t read_ret = fread(file->files[i], file->manifest->entries[i].size, 1, stream);
		offset += file->manifest->entries[i].size;
		if(!read_ret) {
			free(file->header);
			free(file->manifest);
			free(file->files);
			free(file);
			
			return NULL;
		}
	}
	
	return file;
}

unsigned int gjb_file_add_file(gjb_file_t file, FILE *stream, char *name) {
	if(!file || !stream || !file->header || !file->manifest || !name) return 0;
	
	struct gjb_manifest_entry *entry = gjb_manifest_entry_create(name, (u_int64_t)ftell(stream));
	file->header->entry_count++;
	gjb_manifest_add_entry(file->manifest, entry, file->header);
	gjb_manifest_entry_release(entry);
	
	return 1;
}