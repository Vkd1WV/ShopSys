/******************************************************************************/
//	Ammon Dodson
//	Winter 2016
//
//	data.c is a general purpose data structure library
//	functions are documented below
//
/******************************************************************************/


#ifndef DATA_H_INCLUDE
#define DATA_H_INCLUDE

typedef struct _root* DS;

/********************** ACTIONS ON WHOLE DATA STRUCTURE ***********************/

DS new_DS(const char type);
/**	create a new data structure
 *	type may be:
 *	'l' linked list. may be used as stack or queue
 *	'c' circular linked list
 *	't' sorted tree the first entry in data must be a pointer to a null-
 *	terminated index string that will be used to sort the tree entries
 *	'b' b-tree
 *	'q' quad tree
 *	Returns NULL on failure
 */

int drop(DS);
/**	delete an entire data structure
 */
int empty(const DS);
/**	Empty the contents of a data structure without deleting the structure itself
 */

void dump(const DS);
/**	Dump the entire contents of the data structure to the console
 *	the stored data is not changed
 */

/**************************** ADD TO DATA STRUCTURE ***************************/

#define nq(A,B) push(A,B)
int push(const DS, const void*);
/**	Push a new record on top of a linked list
 */
int append(const DS, const void*);
/**	Append a new record to the bottom of a linked list
 *	
 */
int insert(DS, void*, int);
/**	Insert a node at some count of positions from either end of the ll
 *	positive counts from the head, negative from the tail
 */
int sort(DS, void*, char*);
/**	insert node in to the structure in sort order
 *	the third value must be a \0 terminated string used for sorting
 *	duplicate indexes are not allowed
 */

/*********************** REMOVE FROM DATA STRUCTURE ***************************/

void* pop(DS);
/**	Pop the top record from a linked list
 */
void* dq(DS);
/**	de-queue the bottom record from a linked list
 */
int iremove(DS, char*);
/**	remove a record from a data structure
 *	searches for the record by index
 */
int premove(DS, int);
/**	remove a node by position
 *	positive counts from the head, negative from the tail
 */
int truncate(DS, int);
/**	truncate a number of nodes from the head or tail
 *	positive counts from the head, negative from the tail
 */

/********************** VIEW RECORD IN DATA STRUCTURE *************************/

void* iview(const DS, const char*);
/**	search for a node by index in a sorted link list and return a pointer to
 *	its contents
 *	does not change contents of data structure
 */
void* pview(DS, int);
/**	search for a node by position and return its contents
 *	positive counts from the head, negative from the tail
 *	setting count to 0 returns NULL and sets the view pointer to NULL
 */

void* view_next(DS);
/**	View each member of a linked list in order
 *	each call to this function returns the next member of the ll
 *	return to the first member with pview(root, 1);
 */

#endif
