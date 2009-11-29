#include "header.h"
#include "manifest.h"

struct {
	struct gjb_header *header;
	gjb_manifest manifest;
} gjb_file;

typedef struct gjb_file *gjb_file_t;

gjb_file_t gjb_file_create(struct gjb_header *header, gjb_manifest manifest);
void gjb_file_release(gjb_file_t file);