#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "data.h"

// Convenience Definitions
#define TRUE 1
#define FALSE 0

typedef enum {false, true} bool;

/**************************** PRIVATE STRUCTURES ******************************/

struct _node {
	void* data;
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
	int flags;
	// int size? // number of nodes in the structure
};

/*********************************** FLAGS ************************************/

//Structure type

// 0000 0000 0000 0000

//sorting
//

/********************************* MESSAGES ***********************************/

// ERRORS
	// internal error
const char* _e_noimp	="data.h: ERROR: Feature Not yet Implemented";
const char* _e_invtype	="data.h: ERROR: invalid DS type";
	// os error
const char* _e_mem		="data.h: ERROR; Could not allocate more memory";
	// caller error
const char* _e_repeat	="data.h: ERROR: repeated index in a sorted Linked List";
const char* _e_nsense	="data.h: ERROR: Nonsensical action for given structure type";

// NOTICE
const char* _n_empty	="data.h: NOTICE: The data structure is empty";
const char* _n_end		="data.h: NOTICE: The end of the list has been reached";

/**************************** PRIVATE FUNCTIONS *******************************/

int  _sortll		(DS, void*	, char*		);
// sort new data into a linked list
void _add_first_node(DS, struct _node*		);
// add the first node of a structure
void _delete_node	(const DS				);
// delete the node pointed to by view
void _isearch		(const DS, const char*	);
// set the view pointer to the node with the given index
void _psearch		(const DS, const int	);
// set the view pointer to the node with the given position

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
	case 'l':		// linked list
	case 'c':		// circular linked list
	break;
	case 't':		// tree
	case 'b':
		puts(_e_noimp);
		return NULL;
	default:
		puts(_e_invtype);
		return NULL;
	}
	
	new_structure=malloc(sizeof (struct _root));
	if (new_structure == NULL) {
		puts(_e_mem);
		return NULL;
	}
	
	new_structure->type=type;
	new_structure->head=NULL;
	new_structure->tail=NULL;
	new_structure->view=NULL;
	
	return new_structure;
}


int isempty(DS root){
	if (root->head == NULL) return TRUE;
	else return FALSE;
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
	
		if (root->view == NULL) break;
		do {
			puts(root->view->index);
			root->view=root->view->right;
		} while (root->view != root->head);
	
	break;
	case 't':
	case 'b':
	
		puts(_e_noimp);
	break;
	default:
		puts(_e_invtype);
	}
}

/**************************** ADD TO DATA STRUCTURE ***************************/

/**	Push a new record on top of a linked list
 */
int push(const DS root, void* data) {
	struct _node* new_node;
	
	// allocate a new node
	new_node=malloc(sizeof(struct _node));
	if (new_node == NULL){
		puts(_e_mem);
		return EXIT_FAILURE;
	}
	
	// assign data
	new_node->data=data;
	new_node->index=NULL;
	
	if (root->head == NULL) { // this will be the first node in the structure
		(void) _add_first_node(root, new_node);
		return EXIT_SUCCESS;
	}
	
	root->view=new_node;
	
	switch (root->type){
	case 'l':
	
		new_node->previous	=NULL;
		new_node->left		=NULL;
		new_node->right		=root->head;
	
		root->head->previous=new_node;
		root->head			=new_node;
	
	break;
	case 'c':
	
		new_node->previous	=root->tail;
		new_node->left		=NULL;
		new_node->right		=root->head;
		
		root->tail->right	=new_node;
		root->head->previous=new_node;
		root->head			=new_node;
	
	break;
	case 't':
	case 'b':
	
		puts(_e_noimp);
	
	return EXIT_FAILURE;
	default:
	
		puts(_e_invtype);
	
	return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

/**	Append a new record to the bottom of a linked list
 */
int append(const DS root, void* data) {
	struct _node* new_node;
	
	// allocate a new node
	new_node=malloc(sizeof(struct _node));
	if (new_node == NULL){
		puts(_e_mem);
		return EXIT_FAILURE;
	}
	
	// assign data
	new_node->data=data;
	new_node->index=NULL;
	
	if (root->head == NULL) { // this will be the first node in the structure
		(void) _add_first_node(root, new_node);
		return EXIT_SUCCESS;
	}
	
	root->view=new_node;
	
	switch (root->type){
	case 'l':
	
		new_node->previous	=root->tail;
		new_node->left		=NULL;
		new_node->right		=NULL;
		
		root->tail->right	=new_node;
		root->tail			=new_node;
	
	break;
	case 'c':
	
		new_node->previous	=root->tail;
		new_node->left		=NULL;
		new_node->right		=root->head;
		
		root->tail->right	=new_node;
		root->head->previous=new_node;
		root->tail			=new_node;
	
	break;
	case 't':
	case 'b':
	
		puts(_e_noimp);
		return EXIT_FAILURE;
	
	break;
	default:
	
		puts(_e_invtype);
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}

/**	Insert a node at some count of positions from either end of the ll
 *	positive counts from the head, negative from the tail
 */
//int insert(DS root, void* data, int position) {
//	return EXIT_FAILURE;
//}

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
		puts(_e_noimp);
		return EXIT_FAILURE;
	default:
		puts(_e_invtype);
		return EXIT_FAILURE;
	}
}

/*********************** REMOVE FROM DATA STRUCTURE ***************************/

/**	Pop the top record from a linked list
 */
void* pop(const DS root) {
	void* data;
	struct _node* temp;
	
	if (root->head==NULL)
		return NULL;
	
	if(root->view == root->head)
		root->view= NULL;
	
	data=root->head->data;
	temp=root->head;
	
	switch (root->type){
	case 'l':
	
		if (root->head->right == NULL){ // if this is the last node
			root->head=NULL;
			root->tail=NULL;
		}else{
			root->head->right->previous=NULL;
			root->head=root->head->right;
		}
	
	break;
	case 'c':
	
		if(root->head->right == root->head){ // if this is the last node
			root->head=NULL;
			root->tail=NULL;
		}else{
			root->tail->right			=root->head->right;
			root->head->right->previous	=root->tail;
			root->head					=root->head->right;
		}
	
	break;
	case 't':
	case 'b':
		puts(_e_nsense);
		return NULL;
	default:
		puts(_e_invtype);
		return NULL;
	}
	free(temp);
	return data;
}
/**	de-queue the bottom record from a linked list
 */
void* dq(DS root) {
	void* data;
	struct _node* temp;
	
	if (root->tail==NULL)
		return NULL;
	
	if(root->view == root->tail)
		root->view= NULL;
	
	data=root->tail->data;
	temp=root->tail;
	
	switch (root->type){
	case 'l':
	
		if (root->tail->previous == NULL){ // if this is the last node
			root->head=NULL;
			root->tail=NULL;
		}else{
			root->tail->previous->right=NULL;
			root->tail=root->tail->previous;
		}
	
	break;
	case 'c':
	
		if(root->tail->right == root->tail){ // if this is the last node
			root->head=NULL;
			root->tail=NULL;
		}else{
			root->head->previous		=root->tail->previous;
			root->tail->previous->right	=root->head;
			root->tail					=root->tail->previous;
		}
	
	break;
	case 't':
	case 'b':
		puts(_e_nsense);
		return NULL;
	default:
		puts(_e_invtype);
		return NULL;
	}
	free(temp);
	return data;
}

/**	remove a record from a linked list
 *	record is searched by ID
 */
int iremove(DS root, char* index) {
	_isearch(root, index);
	if (root->view == NULL){
		printf("iremove(): ERROR: %s not found.\n", index);
		return EXIT_FAILURE;
	}
	_delete_node(root);
	return EXIT_SUCCESS;
}

/**	remove a node by position
 *	positive counts from the head, negative from the tail
 */
//int premove(DS root, int position) {
//	return EXIT_FAILURE;
//}

/********************** VIEW RECORD IN DATA STRUCTURE *************************/

/**	search for a node by index in a sorted link list and return its contents
 */
void* iview(const DS root, const char* index){
	_isearch(root, index);
	if (root->view == NULL)
		return NULL;
	return root->view->data;
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
	if (root->head == NULL) // empty data structure
		return NULL;
	
	if (root->view == NULL){
		root->view = root->head;
		return root->view->data;
	}
	
	if (root->view->right == NULL) {
		root->view = root->head;
		return NULL;
	}
	
	root->view = root->view->right;
	return root->view->data;
}

/***************************** PRIVATE FUNCTIONS ******************************/

void _add_first_node(DS root, struct _node* node){
	root->head		=node;
	root->tail		=node;
	root->view		=node;
	
	switch (root->type){
	case 'c':
	
		node->previous	=node;
		node->left		=NULL;
		node->right		=node;
	
	break;
	case 'l':
	
		node->previous	=NULL;
		node->left		=NULL;
		node->right		=NULL;
	
	break; // this will probably be the same for trees
	case 't':
	case 'b':
	
		puts(_e_noimp);
	
	break;
	default:
		puts(_e_invtype);
	}
}

void _isearch(const DS root, const char* index){
	if (root->head == NULL){
		root->view=NULL;
		return;
	}
	
	// FIXME this function should check that the index pointer is set. 
	// is this a sorted list?

	root->view=root->head;
	while (( strcmp(index,root->view->index) )>0){
		if (root->view->right == NULL) break;
		root->view=root->view->right;
	}
	if(!strcmp(index,root->view->index))
		return;
	root->view = NULL;
}

void _delete_node(const DS root){
	if (root->view == NULL) return;
	
	switch (root->type){
	case 'l':
	case 'c':
	
		if (root->view == root->head){
			(void) pop(root);
			return;
		}
		if (root->view == root->tail){
			(void) dq(root);
			return;
		}
		
		// It we're in the middle of the list
		root->view->previous->right=root->view->right;
		root->view->right->previous=root->view->previous;
	
	break;
	case 't':
	case 'b':
	
		puts(_e_noimp);
	
	break;
	default:
		puts(_e_invtype);
	}
	
	free(root->view);
	root->view = NULL;
	return;
}

/**	add new data to a link list data structure in sort order
 *	data must be a structure with field int* ID that will be used for sorting
 */
int _sortll(DS root, void* data_pt, char* index) {
	struct _node* new_node;
	struct _node* current_position;
	
	// allocate a new node
	new_node=malloc(sizeof(struct _node));
	if (new_node == NULL){
		puts(_e_mem);
		return EXIT_FAILURE;
	}
	
	// setup the new node
	new_node->data=data_pt;
	new_node->index=index;
	new_node->left=NULL;
	root->view=new_node;
	
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
		puts(_e_repeat);
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


