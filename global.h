/******************************************************************************/
//	Author:	Ammon Dodson
//			Daryan Hanshew
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
//	ShopSys.c	contains the main menu, and the menu prompt
//		: int main()
//		; int prompt()
//
//	global.h	contains type definitions, prototypes, and includes used
//				throughout
//
//	file_access.c	This includes functions for reading and writing to files
//		: Prod	read_product			(FILE* product_file_discriptor			);
//		: DS	read_product_file		(const char* file_name					);
//		: int	write_product_file		(const char* file_name, DS product_data	);
//		: int	append_transaction_file	(const char* file_name, DS xaction_data	);
//
//	formatting.c	Prints out data to the screen or files
//		: void print_prod_heading		(FILE* file_discriptor			);
//		: void print_product			(FILE* file_discriptor, Prod p	);
//		: void print_xaction_heading	(FILE* file_discriptor			);
//		: void print_xaction			(FILE* file_discriptor, Trans t	);
//
//	owner.c		Contains the owner's menu and functions
//		: bool owner_login				();
//		: void owner_menu				(DS product_data, DS transaction_data);
//		: void print_product_list		(DS product_data		);
//		: void add_product				(DS product_data		);
//		: void delete_product			(DS product_data		);
//		: void edit_product				(DS product_data		);
//		: void print_transaction_list	(DS transaction_data	);
//		: void clear_xactions			(DS transaction_data	);
//
/******************************************************************************/

#ifndef Global_LIST_H_INCLUDED
#define Global_LIST_H_INCLUDED

/********************************** INCLUDES **********************************/

#include <stdlib.h> // we use the EXIT_FAILURE macros everywhere
#include <stdio.h>
#include "data.h"   // data structure library
#include "input.h"  // input library

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
DS items;			// I CHANGED THIS
};

typedef struct Product* Prod;
typedef struct Transaction* Trans;

// Convenience Definitions
#define TRUE 1
#define FALSE 0
typedef enum {false, true} bool;


/******************************** PROTOTYPES **********************************/
// These are functions defined in one file and used in others so their
// prototypes have to be here

// In services.c
int prompt();
bool owner_login();
void print_product_list		(DS				);
void print_prod_heading		(FILE*			);
void print_product			(FILE*, Prod	);
void print_xaction			(FILE*, Trans	);

// In owner.c
void owner_menu(DS, DS);

// In customer.c
void customer_menu(DS, DS);

#endif

