#include <sys/types.h>

struct {
	char name[128];
	u_int64_t size;
} gjb_manifest_entry;

typedef struct gjb_manifest_entry **gjb_manifest;

gjb_manifest **gjb_manifest_read(FILE *stream);
