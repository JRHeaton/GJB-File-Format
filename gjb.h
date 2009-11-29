#include <stdio.h>
#include <sys/types.h>

#define HEADER_STR_MAX 128

struct gjb_header {
	char name[128];
	char author[128];
	char description[128];
	u_int64_t entry_count;
};

typedef struct gjb_header *gjb_header_t;

struct gjb_manifest_entry {
	char name[128];
	u_int64_t size;
};

typedef struct gjb_manifest_entry **gjb_manifest_t;

struct gjb_file {
	gjb_header_t header;
	gjb_manifest_t manifest;
};

typedef struct gjb_file *gjb_file_t;

///////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////


unsigned int gjb_header_write(gjb_header_t header, FILE *stream);
gjb_header_t gjb_header_create(char *name, char *author, char *description, u_int64_t entry_count);
void gjb_header_release(gjb_header_t header);
gjb_header_t gjb_header_read(FILE *stream);

gjb_manifest_t gjb_manifest_read(FILE *stream, gjb_file_t file);
unsigned int gjb_manifest_write(FILE *stream, gjb_manifest_t manifest, gjb_file_t file); 
unsigned int gjb_manifest_add_entry(gjb_manifest_t manifest, struct gjb_manifest_entry *entry, gjb_header_t header);

struct gjb_manifest_entry *gjb_manifest_entry_create(char *name, u_int64_t size);
void gjb_manifest_entry_release(struct gjb_manifest_entry *entry);

gjb_file_t gjb_file_create(gjb_header_t header, gjb_manifest_t manifest);
void gjb_file_release(gjb_file_t file);

unsigned int gjb_file_write(FILE *stream, gjb_file_t file);