#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "../gjb.h"

int fsize(char *path) {
	struct stat st;
	if(stat(path, &st) != 0) return 0;
	
	return st.st_size;
}

int main(int argc, char *argv[]) {
	gjb_header_t header = gjb_header_create("My Test File", "Gojohnnyboi", "A test GJB file", 0);
	gjb_manifest_t manifest;
	
	DIR *dir = opendir(argv[1]);
	struct dirent *dp;
	
	printf("Let's enumerate %s\n", argv[1]);
	while((dp = readdir(dir)) != NULL) {
		if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
			continue;
		
		printf("Adding %s\n", dp->d_name);
		struct gjb_manifest_entry *entry = gjb_manifest_entry_create(dp->d_name, fsize(dp->d_name));
		printf("Created entry <%p>\n", entry);
		gjb_manifest_add_entry(manifest, entry, header);
		printf("Added entry\n");
		header->entry_count++;
		printf("Increased header entry count\n");
		gjb_manifest_entry_release(entry);
		printf("Released entry\n");
	}
	
	closedir(dir);
	
	printf("Phew, got past the directory crap\n");
	
	FILE *f = fopen(argv[2], "w+");
	
	gjb_file_t file = gjb_file_create(header, manifest);
	gjb_file_write(f, file);
	gjb_file_release(file);
	
	gjb_header_release(header);
	
	fclose(f);
	
	return 0;
}