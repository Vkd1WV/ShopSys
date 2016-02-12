#ifndef input_h_INCLUDE
#define input_h_INCLUDE

#include <stdio.h>

char* grabline(FILE*);
/**	Get the next newline terminated line of text and store it in an array.
 *	The newline is discarded and the array is \0 teriminated.
 *	whitespace is newlines, spaces, and tabs.
 *	Discards leading and terminating whitespace
 *	returns NULL on failure
 */

char* grabword(FILE*);
/**	Get the next alphanumeric word from the input stream
 *	Discards leading and terminating whitespace
 *	whitespace is newlines, spaces, and tabs
 *	the program must free the array provided by grabword()
 *	returns NULL on failure
 */

#endif
