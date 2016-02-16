#include "input.h"
#include <stdlib.h>
#include <string.h>

#define IN	1 /* inside a word */
#define OUT	2 /* outside a word */
#define ARRAY_SIZE 50 // this is just a starting size

/**	Get the next alphanumeric word from the input stream
 *	Discards leading and terminating whitespace
 *	whitespace is newlines, spaces, and tabs
 *	the program must free the array provided by grabname()
 *	returns NULL on failure
 */
char* grabword (FILE* source){
	// temporary character, position in the array, current size of the array
	int i, size=ARRAY_SIZE, state;
	// a variable array to temporarily hold the word
	char* store;
	// an array to return to caller
	char* output;
	char c;
	
	if (source == NULL)
		return NULL;
	
	// allocate
	if ( (store=calloc(ARRAY_SIZE, sizeof(char))) == NULL ) {
		puts("ERROR: calloc() failed.");
		return NULL;
	}
	// initialize
	//for(i=0; i<ARRAY_SIZE; i++) store[i]=0;
	
	state = OUT; // assume that the file pointer is not already in a word
	i=0;
	// while c is not whitespace read it into store
	do {
		c = fgetc(source);
		if (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == EOF){
			if (state == IN)
				break; // stop when we find whitespace at the end of the word
		} else {
			state = IN;
			if (i == size-1) { // if store isn't big enough we double it.
				if ( (store=realloc(store, size *=2)) == NULL ) {
					puts("ERROR: realloc() failed.");
					free(store);
					return NULL;
				}
			}
			// store c in the array and increment i
			store[i]=c;
			i++;
		}
	} while (!feof(source));
	
	if (*store == '\0') { // if nothing was read
		free(store);
		return NULL;
	}
	// create a correctly sized array to output the result
	if ( (output=malloc(i)) == NULL ) {
		puts("ERROR: malloc() failed.");
		free(store);
		return NULL;
	}
	strcpy(output, store);
	free(store);
	return output;
}

/**	Get the next newline terminated line of text and store it in an array.
 *	The newline is discarded and the array is NULL teriminated.
 *	Ignores and discards leading newlines.
 */
char* grabline(FILE* source) {
	// temporary character, position in the array, current size of the array
	int c, i, size=ARRAY_SIZE, state;
	// a variable array to temporarily hold the word
	// and an array to return to caller
	char *store, *output;
	
	// allocate and initialize store
	if ( (store=malloc(ARRAY_SIZE)) == NULL ) {
		puts("ERROR: malloc() failed.");
		return NULL;
	}
	for(i=0; i<ARRAY_SIZE; i++)
		store[i]=0;
	
	state = OUT;
	i=0;
	// while c is not whitespace read it into store
	while ((c = fgetc(source)) != EOF) {
		if (c == '\n' || c == '\r' || c == EOF){
			if (state == IN)
				break; // stop when we find whitespace at the end of the word
		} else {
			state = IN;
			if (i == size-1) { // if store isn't big enough we double it.
				if ( (store=realloc(store, size *=2)) == NULL ) {
					puts("ERROR: realloc() failed.");
					return NULL;
				}
			}
			// store c in the array and increment i
			store[i]=c;
			i++;
		}
	}
	
	if (store=='\0') { // if nothing was read
		return NULL;
	}
	// create a correctly sized array to output the result
	if ( (output=malloc(i)) == NULL ) {
		puts("ERROR: malloc() failed.");
		return NULL;
	}
	strcpy(output, store);
	free(store);
	return output;
}

/**	Get the next tab, 2x space, or newline terminated string from the input stream
 *	Discards leading and terminating whitespace
 *	whitespace is newlines, tabs, and leading spaces
 *	the program must free the array provided by grabfield()
 *	returns NULL on failure
 */
char* grabfield(FILE* source){
	// temporary character, position in the array, current size of the array
	int i, size=ARRAY_SIZE;
	int state, space_count=0;
	// a variable array to temporarily hold the word
	char* store;
	// an array to return to caller
	char* output;
	char c;
	
	if (source == NULL)
		return NULL;
	
	// allocate
	if ( (store=calloc(ARRAY_SIZE, sizeof(char))) == NULL ) {
		puts("ERROR: calloc() failed.");
		return NULL;
	}
	
	state = OUT; // assume that the file pointer is not already in a field
	i=0;
	
	// while c is not EOF read it into store
	do {
		c = fgetc(source);
		if (c == ' ' && state == OUT){
			// do nothing
		
		} else if ( (c>='\t' && c<='\r') || c == EOF || space_count >2){
			if (state == IN)
				break; // stop when we find whitespace at the end of the word
		
		} else {
			state = IN;
			if (i == size-1) { // if store isn't big enough we double it.
				if ( (store=realloc(store, size *=2)) == NULL ) {
					puts("ERROR: realloc() failed.");
					free(store);
					return NULL;
				}
			}
			if (c == ' ') space_count++;
			else space_count=0;
			
			// store c in the array and increment i
			store[i]=c;
			i++;
		}
	} while (!feof(source));
	
	// get rid of any trailing spaces we might have captured
	while(store[i-1] == ' '){
		store[i-1]='\0';
		i--;
	}
	
	if (*store == '\0') { // if nothing was read
		free(store);
		return NULL;
	}
	
	// create a correctly sized array to output the result
	if ( (output=malloc(i)) == NULL ) {
		puts("ERROR: malloc() failed.");
		free(store);
		return NULL;
	}
	strcpy(output, store);
	free(store);
	return output;
}

