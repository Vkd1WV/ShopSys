#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "data.h"

/**************************** PRIVATE STRUCTURES ******************************/

struct _node {
	const void* data;
	const char* index;
	// data type?
	struct _node* previous;
	struct _node* left;  // not used in linked lists
	struct _node* right; // next pointer for linked lists
};

struct _root {
	struct _node* head;
	struct _node* tail;
	struct _node* view;
	char type;
	// int size;
};

/**************************** PRIVATE FUNCTIONS *******************************/

int  _sortll		(DS, void*	, char*); // sort new data into a linked list
void _add_first_node(DS, struct _node*	); // add the first node of a structure

/********************** ACTIONS ON WHOLE DATA STRUCTURE ***********************/

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
DS new_DS(const char type){
	DS new_structure;
	
	switch (type){
	case 'l':
	break;
	case 'c':
	case 't':
	case 'b':
		puts("not yet implemented");
		return NULL;
	default:
		puts("ERROR: invalid DS type");
		return NULL;
	}
	
	new_structure=malloc(sizeof (struct _root));
	if (new_structure == NULL) {
		puts("ERROR: malloc() returned NULL");
		return NULL;
	}
	
	new_structure->type=type;
	new_structure->head=NULL;
	new_structure->tail=NULL;
	new_structure->view=NULL;
	
	return new_structure;
}

/**	delete an entire data structure, freeing all memory
 */
int drop(DS root){
	return EXIT_FAILURE;
}

/**	Dump the entire contents of the data structure to the console
 *	the stored data is not changed
 */
void dump(const DS root){
	root->view=root->head;
	
	switch (root->type){
	case 'l':
		while(root->view != NULL) {
			puts(root->view->index);
			root->view=root->view->right;
		}
		
	break;
	case 'c':
	case 't':
	case 'b':
		puts("not yet implemented");
		break;
	default:
		puts("ERROR: invalid DS type");
	}
}

/**************************** ADD TO DATA STRUCTURE ***************************/

/**	Push a new record on top of a linked list
 */
int push(const DS root, const void* data) {
	struct _node* new_node;
	
	// allocate a new node
	new_node=malloc(sizeof(struct _node));
	if (new_node == NULL){
		puts("ERROR: malloc() returned NULL");
		return EXIT_FAILURE;
	}
	
	new_node->data=data;
	
	if (root->head == NULL) // this will be the first node in the list
		_add_first_node(root, new_node);
	else {
		new_node->previous	=NULL;
		new_node->left		=NULL;
		new_node->right		=root->head;
		
		root->head->previous=new_node;
		root->head			=new_node;
	}
	
	return EXIT_SUCCESS;
}

/**	Append a new record to the bottom of a linked list
 */
int append(const DS root, const void* data) {
	return EXIT_FAILURE;
}

/**	Insert a node at some count of positions from either end of the ll
 *	positive counts from the head, negative from the tail
 */
int insert(DS root, void* data, int position) {
	return EXIT_FAILURE;
}

/**	insert node in to the structure in sort order
 *	data must contain a char* ID used for sort ordering
 */
int sort(DS root, void* data, char* index) {
	switch (root->type){
	case 'l':
		return _sortll(root, data, index);
	break;
	case 'c':
	case 't':
	case 'b':
		puts("not yet implemented");
		return EXIT_FAILURE;
	default:
		puts("ERROR: invalid DS type");
		return EXIT_FAILURE;
	}
}

/*********************** REMOVE FROM DATA STRUCTURE ***************************/

/**	Pop the top record from a linked list
 */
void* pop(const DS root) {
	void* data;
	struct _node* temp;
	
	switch (root->type){
	case 'l':
	case 'c':
		if (root->head==NULL) {
			puts("Empty tree");
			return NULL;
		}
		data=root->head->data;
		
		temp=root->head;
		
		if (root->head->right == NULL){
			root->head=NULL;
		}else{
			root->head->right->previous=root->head->previous;
			root->head=root->head->right;
		}
		free(temp);
		
		return data;
	case 't':
	case 'b':
		puts("ERROR: you can't pop() the root of a tree. that makes no effin' sense. retard.");
		return NULL;
	default:
		puts("ERROR: invalid DS type");
		return NULL;
	}
	return NULL;
}
/**	de-queue the bottom record from a linked list
 */
void* dq(DS root) {
	return NULL;
}

/**	remove a record from a linked list
 *	record is searched by ID
 */
int iremove(DS root, char* index) {
	return EXIT_FAILURE;
}

/**	remove a node by position
 *	positive counts from the head, negative from the tail
 */
int premove(DS root, int position) {
	return EXIT_FAILURE;
}

int truncate(DS root, int position) {
	return EXIT_FAILURE;
}
/**	truncate a number of nodes from the head or tail
 *	positive counts from the head, negative from the tail
 */

/********************** VIEW RECORD IN DATA STRUCTURE *************************/

/**	search for a node by index in a sorted link list and return its contents
 */
void* iview(const DS root, const char* index){
	if (root->head == NULL)
		return NULL;

	root->view=root->head;
	while (( strcmp(index,root->view->index) )>0){
		root->view=root->view->right;
	}
	if(!strcmp(index,root->view->index))
		return root->view->data;
	return NULL;
}

/**	search for a node by position and return its contents
 *	positive counts from the head, negative from the tail
 */
void* pview(const DS root, int position){
	if (position > 0){}
	// FIXME
	else if (position < 0) {}
	else
		root->view=NULL;
	
	return root->view;
}

/**	View each member of a linked list in order
 *	each call to this function returns the next member of the ll
 *	return to the first member with pview(root, 1);
 */
void* view_next(DS root) {
	if (root->view == NULL){
		root->view = root->head;
		return root->view->data;
	}
	
	if (root->view->right == NULL) {
		puts("End of list");
		root->view = root->head;
		return NULL;
	}
	
	root->view = root->view->right;
	return root->view->data;
}

/**************************** LINK LIST FUNCTIONS *****************************/

void _add_first_node(DS root, struct _node* node){
	node->previous	=NULL;
	node->left		=NULL;
	node->right		=NULL;
	
	root->head		=node;
	root->tail		=node;
}

/**	add new data to a link list data structure in sort order
 *	data must be a structure with field int* ID that will be used for sorting
 */
int _sortll(DS root, void* data_pt, char* index) {
	struct _node* new_node;
	struct _node* current_position;
	
	// allocate memory for the new node
	new_node=malloc(sizeof(struct _node));
	if (new_node == NULL) {
		puts("ERROR: malloc() returned NULL");
		return EXIT_FAILURE;
	}
	
	// setup the new node
	new_node->data=data_pt;
	new_node->index=index;
	new_node->left=NULL;
	
	if (root->head == NULL) { // if the data structure is empty
		_add_first_node(root, new_node);
		return EXIT_SUCCESS;
	}
	
	current_position=root->head;
	while (( strcmp(index,current_position->index) )>0) {
		if (current_position->right == NULL) {
			// we are at the end of the linked list,
			// and the new data must go last
			
			current_position->right=new_node;
			
			new_node->previous=current_position;
			new_node->right=NULL;
			
			root->tail=new_node;
			
			return EXIT_SUCCESS;
		}
		
		current_position=current_position->right;
	}
	
	if (!strcmp(index,current_position->index)) {
		puts("ERROR: repeated index in a sorted Linked List");
		free(new_node);
		return EXIT_FAILURE;
	}
	
	if (current_position == root->head){
		// we are at the head of the linked list and the new data must go before
		
		current_position->previous=new_node;
		
		new_node->previous=NULL;
		new_node->right=current_position;
		
		root->head=new_node;
		
		return EXIT_SUCCESS;
	}
	
	// If we are still running, then there is a node before and after us,
	// and the new node must go before the current node
	
	current_position->previous->right=new_node;
	
	new_node->previous=current_position->previous;
	new_node->right=current_position;
	
	current_position->previous=new_node;
	
	return EXIT_SUCCESS;
}


