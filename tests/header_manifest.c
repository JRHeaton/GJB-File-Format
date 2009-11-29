#include <file_structure.h>
#include <header.h>
#include <manifest.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

int fsize(char *path) {
	struct stat st;
	if(stat(path, &st) != 0) return 0;
	
	return st.st_size;
}

int main(int argc, char *argv[]) {
	gjb_header_t header = gjb_header_create("My Test File", "Gojohnnyboi", "A test GJB file", 0);
	gjb_manifest manifest;
	
	DIR *dir = opendir(argv[1]);
	struct dirent *dp;
	while((dp = readdir(dir)) != NULL) {
		struct gjb_manifest_entry *entry = gjb_manifest_entry_create(dp->d_name, fsize(dp->d_name));
		gjb_manifest_add_entry(manifest, entry, header);
		header->entry_count += 1;
		gjb_manifest_entry_release(entry);
	}
	
	closedir(dir);
	
	FILE *f = fopen(argv[2], "w+");
	
	gjb_file_t file = gjb_file_create(header, manifest);
	gjb_file_write(f, file);
	gjb_file_release(file);
	
	fclose(f);
	
	return 0;
}