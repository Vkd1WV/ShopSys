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

#include "global.h"

/************************************************************/
/*			print a heading for product lists				*/
/************************************************************/

void print_prod_heading(FILE* file){
	fprintf(file,
			"    ID               NAME             ON-HAND  UNIT PRICE\n"
	);
}

/**	product.txt has a header in the first line followed by one product record
 *	per line. Each product record has 4 tab-delimited fields.
 *	PRODUCT ID	PRODUCT NAME	QUANTITY ON HAND	PRICE
*/

/************************************************************/
/*					Print one Product Record				*/
/************************************************************/

void print_product(FILE* file, Prod p){
	fprintf(file,
			"%8s   %24s   %7d    $ %6.2f\n",
			p->ID,
			p->name,
			p->num_unit,
			p->price
	);
}

/************************************************************/
/*			Print a Heading for Transaction Lists			*/
/************************************************************/

void print_xaction_heading(FILE* file){
	fprintf(file,
			"FIRST NAME  LAST NAME AMOUNT  ADDRESS   \n"
	);
}


/**	transaction.txt has a header in the first line followed by one transaction
 *	record per line.
 *	each transaction record has 5 tab-delimited fields:
 *	First Name	last Name	Ammount of payment	date of payment	list of
 *	purchased products
*/

/************************************************************/
/*				Print one Transaction Record				*/
/************************************************************/

void print_xaction(FILE* file, Trans t){
	fprintf(file,
			"%10s %10s  $ %6.2f %s  %4d.%2d.%2d \n",
			t->f_name,
			t->l_name,
			t->pay,
			t->address,
			t->yy,
			t->mm,
			t->dd
	);
}
