#include <sys/types.h>
#include "file_structure.h"

struct {
	char name[128];
	u_int64_t size;
} gjb_manifest_entry;

typedef struct gjb_manifest_entry **gjb_manifest;

gjb_manifest gjb_manifest_read(FILE *stream, gjb_file_t file);
unsigned int gjb_manifest_write(FILE *stream, gjb_manifest manifest, gjb_file_t file); 