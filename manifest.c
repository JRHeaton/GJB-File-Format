#include "manifest.h"
#include "header.h"

gjb_manifest gjb_manifest_read(FILE *stream, gjb_file_t file) {
	gjb_manifest manifest;
	
	if(!stream) return NULL;
	if(!file->header) return NULL;
	
	fseek(stream, sizeof(struct gjb_header), SEEK_SET);
	u_int64_t i;
	
	for(i=0;i<file->header.entry_count;++i) {
		struct gjb_manifest_entry *entry;
		size_t read_ret = fread(entry, sizeof(struct gjb_manifest_entry), 1, stream);
		if(!read_ret) return NULL;
		
		manifest[i] = calloc(1, sizeof(struct gjb_manifest_entry));
		memcpy(manifest[i], entry, sizeof(struct gjb_manifest_entry));
		
		free(entry);
	}
	
	return manifest;
}

unsigned int gjb_manifest_write(FILE *stream, gjb_manifest manifest, gjb_file_t file) {
	if(!stream || !manifest || !file) return 0;
	if(!file->header) return 0;
	
	fseek(stream, sizeof(struct gjb_header), SEEK_SET);
	u_int64_t i;
	for(i=0;i<file->header.entry_count;++i) {
		size_t write_ret = fwrite(manifest[i], sizeof(struct gjb_manifest_entry), 1, stream);
		if(!write_ret) return NULL;
	}
	
	return 1;
}