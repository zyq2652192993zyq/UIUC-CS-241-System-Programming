#include "document.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int main()
{
	Document *document;

	// TEST 1
	document = Document_create();
	Document_insert_line(document, 0, "line1");
	Document_insert_line(document, 1, "line2");
	Document_set_line(document,    0, "newline1");
	Document_insert_line(document, 3, "line4");
	Document_write_to_file(document,  "output");
	Document_destroy(document);


	// TEST 2
	document = Document_create_from_file("output");

	assert(*Document_get_line(document, 0) == 'n');
	assert(*Document_get_line(document, 1) == 'l');
	assert(*Document_get_line(document, 2) == '\0');
	assert(*Document_get_line(document, 3) == 'l');


	// // TEST 3
	Document_delete_line(document, 2);
	assert(*Document_get_line(document, 0) == 'n');
	assert(*Document_get_line(document, 1) == 'l');
	assert(*Document_get_line(document, 2) == 'l');
	Document_destroy(document);


	// TEST 4
	document = Document_create();
	Document_write_to_file(document,  "output");
	Document_destroy(document);

	document = Document_create_from_file("output");
	Document_destroy(document);

	return 0;
}