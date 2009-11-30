#include <stdio.h>
#include "../gjb.h"
#include <sys/stat.h>

u_int64_t fsize(char *path) {
	struct stat st;
	if(stat(path, &st) != 0) return 0;
	
	return st.st_size;
}

int main(int argc, char *argv[]) {
	// arg1 = gjb file name
	// arg2 = first file 2 add
	// arg3 = second file 2 add
	
	gjb_header_t header = gjb_header_create("First GJB File :)", "John Heaton", "The first working GJB file with other files!");
	gjb_manifest_t manifest = gjb_manifest_create();
	gjb_file_t file = gjb_file_create(header, manifest);
	
	FILE *s1 = fopen(argv[1], "w+"), *s2 = fopen(argv[2], "r"), *s3 = fopen(argv[3], "r");
	if(!s1 || !s2 || !s3) 
		return 1;
	
	gjb_file_add_file(file, s2, argv[2], fsize(argv[2]));
	gjb_file_add_file(file, s3, argv[3], fsize(argv[3]));

	gjb_file_write(s1, file);

	gjb_file_release(file);
	
	fclose(s1);
	fclose(s2);
	fclose(s3);
	
	return 0;
}