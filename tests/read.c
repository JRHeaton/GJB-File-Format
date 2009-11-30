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
		printf("%d: %s; size: %d\n", i, manifest->entries[i].name, manifest->entries[i].size);
	}
	printf("------------------------\n");
}

int main(int argc, char *argv[]) {
	FILE *stream = fopen(argv[1], "r");
	if(!stream) return 1;
	
	gjb_file_t file;
	
	gjb_header_t header = gjb_header_read(stream);
	if(header) print_header(header);
	
	gjb_manifest_t manifest = gjb_manifest_read(stream, header);
	if(manifest) {
		file = gjb_file_create(header, manifest);
		print_manifest(manifest);
	}
	
	gjb_header_release(header);
	gjb_manifest_release(manifest);
	gjb_file_release(file);
	
	fclose(stream);
	
	return 0;
}