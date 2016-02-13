/******************************************************************************/
// Ammon Dodson
//	CES202
//	Winter 2016
//
//	This program should be build with make. A makefile has been provided.
//
/******************************* COMPONENTS ***********************************/
//
//	data.c data.h		stand-alone data structure library
//	input.c input.h		stand-alone input library
//
//	Makefile	contains build instructions
//
//	ShopSys.c	: main()	including main menu
//				; prompt()	prompt user for menu inputs
//	global.h	contains type definitions, prototypes, and includes used
//				throughout
//
//	file_access.c	This includes functions for reading and writing to files
//					: read_product_file()
//					: read_product()		called for each line of product.txt
//					: write_product_file()
//					: append_transaction_file()
//
//	owner.c		Contains the owner's menu and functions
//				; owner_login()
//				; owner_menu()
//				; print_product_list()
//				; print_product()
//				; add_product()
//				; delete_product()
//				; edit_product()
//				; print_transaction_list()
//
/******************************************************************************/

#ifndef Global_LIST_H_INCLUDED
#define Global_LIST_H_INCLUDED

/********************************** INCLUDES **********************************/

#include <stdlib.h> // we use the EXIT_FAILURE macros everywhere
#include <stdio.h>
#include "data.h"
#include "input.h"

/**************************** TYPE DEFINITIONS ********************************/

struct Product{
char*  ID;
char* name;
int num_unit;
float price;
};

struct Transaction{
char* f_name;
char* l_name;
char* address;
float pay;
int dd, mm, yy;
struct Product * p;
};

typedef struct Product* Prod;
typedef struct Transaction* Trans;

// Convenience Definitions
#define TRUE 1
#define FALSE 0
typedef enum {false, true} bool;


/******************************** PROTOTYPES **********************************/
// These are functions defined in one file and used in others

DS read_product_file(const char*);
/**	Read the contents of products.txt and store that data in a linked list
 *	returns a pointer to the linked list
 *	returns NULL on failure
 */

int write_product_file(const char*, DS);
/**	Write the product data to the product.txt file.
 *	returns EXIT_FAILURE or EXIT_SUCCESS
 */

int append_transaction_file(const char*, DS);
/**	append new transactions to the end of transactions.txt
 *	returns EXIT_FAILURE or EXIT_SUCCESS
 */

int prompt();
bool owner_login();
void owner_menu(DS, DS);

void customer_menu();

void print_prod_heading(FILE*);
void print_product(FILE*, Prod);
void print_xaction_heading(FILE*);
void print_xaction(FILE*, Trans);

#endif

