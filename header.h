#include <sys/types.h>

struct {
	char name[128];
	char author[128];
	char description[128];
	u_int64_t entry_count;
} gjb_header;

#define HEADER_STR_MAX 128

unsigned int gjb_header_write(struct gjb_header *header, FILE *stream);
unsigned int gjb_header_write(FILE *stream);
struct gjb_header *gjb_header_create(char *name, char *author, char *description, u_int64_t entry_count);
void gjb_header_release(struct gjb_header *header);