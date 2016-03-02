/******************************************************************************/
//  Author: Ammon Dodson
//          Daryan Hanshew
//  CES202
//  Winter 2016
//
//  This program should be built with make. A makefile has been provided.
//
/******************************* COMPONENTS ***********************************/
//
// Makefile     contains build instructions
//
//  data.c  data.h      stand-alone data structure library
// input.c input.h      stand-alone input library
//
// global.h     contains type definitions, prototypes, and includes used
//              throughout
//
// ShopSys.c    contains the main menu and file activities
//   int  main                   (                                      )
//   bool owner_login            (                                      )
//   Prod read_product           (FILE* product_file_discriptor         )
//   DS   read_product_file      (const char* file_name                 )
//   int  write_product_file     (const char* file_name, DS product_data)
//   int  append_transaction_file(const char* file_name, DS xaction_data)
//
// owner.c      Contains the owner's menu and functions
//   void owner_menu            (DS product_data    , DS transaction_data)
//   void add_product           (DS product_data                         )
//   void delete_product        (DS product_data                         )
//   void edit_product          (DS product_data                         )
//   void print_transaction_list(DS transaction_data                     )
//   void clear_xactions        (DS transaction_data                     )
//   void print_file            (const char* filename                    )
//
// customer.c   Contains the customer's menu and functions
//   void customer_menu(DS prod_list, DS trans_list               )
//   int cart_menu     (Trans cart  , DS prod_list , DS trans_list)
//   void update_cart  (Trans cart                                )
//   void add_to_cart  (Trans cart  , DS prod_list                )
//   void searchByName (DS prod_list                              )
//   void edit_item    (Trans cart  , DS prod_list                )
//   int checkout      (Trans cart  , DS prod_list , DS trans_list)
//
// services.c   Cotains functions used in multiple places
//   int  prompt               (                              )
//   void print_product_list   (DS product_data               )
//   void print_prod_heading   (FILE* file_discriptor         )
//   void print_product        (FILE* file_discriptor, Prod p )
//   void print_xaction_heading(FILE* file_discriptor         )
//   void print_xaction        (FILE* file_discriptor, Trans t)
//
/******************************************************************************/

#include "global.h"

#include <string.h>

/************************************************************/
/*						The Menu Prompt						*/
/************************************************************/
//USES:	input.h:	grabword()

int prompt(){
	char* temp;
	int result;
	
	printf("ShopSys:>");
	temp=grabword(stdin);
	
	if (!(result=atoi(temp)))	// If the input was not a number
		result=*temp;			// we assume it's a letter
	free(temp);
	return result;
}

/************************************************************/
/*		Print the whole product List to the Screen			*/
/************************************************************/
//USES:	data.h:			pview()
//						view_next()

void print_product_list(DS prod_list){
	Prod product;
	
	(void) pview(prod_list, 0); // set the view pointer to NULL
	
	print_prod_heading(stdout);
	
	while ((product=view_next(prod_list)) != NULL){
		print_product(stdout, product);
	}
}

/************************************************************/
/*			print a heading for product lists				*/
/************************************************************/

void print_prod_heading(FILE* file){
	fprintf(file,
			"   ID   \t             NAME             \tQUANTITY\tUNIT PRICE\n"
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
			"%8s\t%30s\t%8d\t $ %7.2f\n",
			p->ID,
			p->name,
			p->num_unit,
			p->price
	);
}

/**	each transaction record has 5 tab-delimited fields:
 *	First Name	last Name	Ammount of payment	date of payment	list of
 *	purchased products
*/

/************************************************************/
/*				Print one Transaction Record				*/
/************************************************************/

void print_xaction(FILE* file, Trans t){
	Prod item;
	
	fprintf(file,
			"\n%4d.%02d.%02d NAME: %s, %s\n        ADDRESS: %s\n",
			t->yy,
			t->mm,
			t->dd,
			t->l_name,
			t->f_name,
			t->address
	);
	if (isempty(t->items))
		fputs("\tYour cart is empty\n", file);
	else{
		print_prod_heading(file);
		pview(t->items, 0);
		while((item=view_next(t->items)) != NULL)
			print_product(file, item);
	}
	fprintf(file,
			"========\t==============================\t========\t==========\n");
	fprintf(file,
			"        \t                              \t  TOTAL:\t $ %7.2f\n",
			t->pay
	);
}
