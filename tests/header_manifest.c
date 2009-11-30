#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>

#include "../gjb.h"

long fsize(char *path) {
	struct stat st;
	if(stat(path, &st) != 0) return 0;
	
	return (long)st.st_size;
}

int main(int argc, char *argv[]) {
	gjb_header_t header = gjb_header_create("My Test File", "Gojohnnyboi", "A test GJB file");
	gjb_manifest_t manifest = gjb_manifest_create();
	
	DIR *dir = opendir(argv[1]);
	struct dirent *dp;
	
	printf("Let's enumerate %s\n", argv[1]);
	int i = header->entry_count;
	while((dp = readdir(dir)) != NULL) {
		if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
			continue;
		
		printf("Adding %s\n", dp->d_name);
		struct gjb_manifest_entry *entry = gjb_manifest_entry_create(dp->d_name, (u_int64_t)fsize(dp->d_name));
		printf("Created entry <%p>\n", entry);
		
		header->entry_count++;
		printf("Increased header entry count\n");
		

		gjb_manifest_add_entry(manifest, entry, header);
		printf("Added entry\n");
		gjb_manifest_entry_release(entry);
		printf("Released entry\n");
		
		++i;
	}
	
	closedir(dir);
	
//	printf("%d\n", i);
	printf("Phew, got past the directory crap\n");
	
	FILE *f = fopen(argv[2], "w+");
	
	gjb_file_t file = gjb_file_create(header, manifest);
	printf("created\n");
	gjb_file_write(f, file);
	
	printf("written\n");
	gjb_file_release(file);
	
	gjb_manifest_release(manifest);
	gjb_header_release(header);
	
	fclose(f);
	
	return 0;
}