#include "../gjb.h"

void print_header(gjb_header_t header) {
	printf("------------------------\n");
	printf("         Header         \n");
	printf("------------------------\n");
	printf("Name:        %s\n", header->name);
	printf("Author:      %s\n", header->author);
	printf("Description: %s\n", header->description);
	printf("Entry Count: %llu\n", header->entry_count);
	printf("------------------------\n");
}

void print_manifest(gjb_manifest_t manifest) {
	printf("------------------------\n");
	printf("        Manifest        \n");
	printf("------------------------\n");
	int i;
	for(i=0;i<manifest->count;++i) {
		printf("%d: %s; size: %llu\n", i, manifest->entries[i].name, manifest->entries[i].size);
	}
	printf("------------------------\n");
}

int main(int argc, char *argv[]) {
	FILE *stream = fopen(argv[1], "r");
	if(!stream) return 1;
	
	gjb_file_t file = gjb_file_read(stream);
	
	if(file->header) print_header(file->header);
	
	if(file->manifest) {
		print_manifest(file->manifest);
	}
	
	int i;
	for(i=0;i<file->header->entry_count;++i) {
		FILE *f = fopen(file->manifest->entries[i].name, "w+");
		fwrite(file->files[i], file->manifest->entries[i].size, 1, f);
		fclose(f);
	}
	
	gjb_file_release(file);
	fclose(stream);
	
	return 0;
}