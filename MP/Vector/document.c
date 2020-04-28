#include "document.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

struct _document
{
	Vector *vector;
};

void *string_copy_constructor(void *elem)
{
	char *str = elem;
    assert(str);
    return strdup(str);
}

void string_destructor(void *elem)
{
	free(elem);
}

Document *Document_create()
{
	Document *document = malloc(sizeof(Document));
	assert(document);

	document -> vector = Vector_create(string_copy_constructor, string_destructor);
	assert(document -> vector);

	return document;
}

void Document_write_to_file(Document *document, const char *filename)
{
	assert(document);
	assert(filename);

	FILE *fd = fopen(filename, "w+");
	//check if the open is OK
	if (!fd) {
		fprintf(stderr, "%s\n", strerror(errno));
		return;
	}

	char *line;
	for (size_t i = 0; i < Vector_size(document -> vector); ++i) {
		if (line = Vector_get(document -> vector, i))
			fprintf(fd, "%s\n", line);
		else
			fprintf(fd, "\n");
	}
	fclose(fd);
}

Document *Document_create_from_file(const char *filename)
{
	assert(filename);

	Document *document = Document_create();
	//we don't need if check is OK
	//we should treat it as an empty file and move on
	FILE *fd = fopen(filename, "r");

	char *buffer = NULL;
	size_t bufferSize = 0;
	ssize_t size;
	size_t line_number = 0; //count from number 0

	while ((size = getline(&buffer, &bufferSize, fd)) != -1) {
		buffer[size - 1] = '\0';
		Document_insert_line(document, line_number, buffer);
		++line_number;
	}
	free(buffer);
	buffer = NULL;

	fclose(fd);

	return document;
}

size_t Document_size(Document *document)
{
	assert(document);

	return Vector_size(document -> vector);
}

void Document_destroy(Document *document)
{
	assert(document);

	Vector_destroy(document -> vector);
	free(document);
}

void Document_set_line(Document *document, size_t line_number, const char *str)
{
	assert(document);
	assert(str);
	assert(line_number >= 0);

	Vector_set(document -> vector, line_number, (void *)str);
}

const char *Document_get_line(Document *document, size_t line_number)
{
	assert(document);
	assert(line_number >= 0);

	return (const char *)Vector_get(document -> vector, line_number);
}

void Document_insert_line(Document *document, size_t line_number, const char *str)
{
	assert(document);
	assert(str);
	assert(line_number >= 0);

	Vector_insert(document -> vector, line_number, (void *)str);
}

void Document_delete_line(Document *document, size_t line_number)
{
	assert(document);
	assert(line_number >= 0);

	Vector_delete(document -> vector, line_number);
}
